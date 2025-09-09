{
  description = "Libmahjong Riichi Mahjong Game Engine";

  inputs = {
    nixpkgs.url = "github:NixOS/nixpkgs";
    flake-utils.url = "github:numtide/flake-utils";
  };

  outputs =
    {
      self,
      nixpkgs,
      flake-utils,
    }:
    flake-utils.lib.eachDefaultSystem (
      system:
      let
        pkgs = import nixpkgs { inherit system; };

        buildPackages = with pkgs; [
          cmake
          git
          ninja
        ];

        llvmPackage = pkgs.llvmPackages_21;

        # Override the existing gtest package to use clang
        clangGtest = pkgs.gtest.override {
          stdenv = llvmPackage.stdenv;
        };

        commonAttrs = {
          pname = "libmahjong";
          version = "0.1.0";
          src = ./.;

          cmakeFlags = [
            "-DBUILD_SHARED_LIBS=ON" # Explicitly build shared libraries
            "-Dlibmahjong_build_tests=OFF"
            "-Dlibmahjong_build_tools=OFF"
            "-DCMAKE_INSTALL_LIBDIR=lib"
            "-DCMAKE_INSTALL_INCLUDEDIR=include"
          ];

          # Let CMake handle installation
          dontUseCmakeBuildDir = false;

          meta = with pkgs.lib; {
            description = "Riichi Mahjong Game Engine Library";
          };
        };

        clangToolsWithLibcxx = llvmPackage.clang-tools.override {
          enableLibcxx = true;
        };

        clangNativeBuildInputs =
          buildPackages
          ++ (with llvmPackage; [
            libcxx
            libcxx.dev
            libcxxClang
            lldb
          ])
          ++ [
            clangToolsWithLibcxx
          ]
          ++ (with pkgs; [
            clangGtest # Use our clang-built GTest instead of pkgs.gtest
            clangGtest.dev # Include the development headers
          ]);
      in
      {
        devShells.default = pkgs.mkShell.override { stdenv = llvmPackage.libcxxStdenv; } {
          nativeBuildInputs = clangNativeBuildInputs;

          # Disable all hardening
          hardeningDisable = [ "all" ];

          NIX_CFLAGS_COMPILE = "-stdlib=libc++";
          NIX_CFLAGS_LINK = "-stdlib=libc++ -L${llvmPackage.libcxx}/lib -lc++abi";

          shellHook = ''
            mkdir -p .vscode
            echo "{" > .vscode/settings.json
            echo '  "nixEnvSelector.nixFile": "''${workspaceFolder}/shell.nix",' >> .vscode/settings.json
            echo "  \"cmake.cmakePath\": \"${pkgs.cmake}/bin/cmake\"," >> .vscode/settings.json
            echo "  \"cmake.ctestPath\": \"${pkgs.cmake}/bin/ctest\"," >> .vscode/settings.json
            echo "  \"cmake.skipConfigureIfCachePresent\": true," >> .vscode/settings.json
            echo "  \"cmake.configureOnOpen\": false," >> .vscode/settings.json
            echo "  \"cmake.configureOnEdit\": false," >> .vscode/settings.json
            echo "}" >> .vscode/settings.json

            ${pkgs.cmake}/bin/cmake -S . -B build -G Ninja \
              -Dlibmahjong_build_tests=ON \
              -DCMAKE_CXX_FLAGS="-O1 -g -I${clangGtest.dev}/include"
          '';
        };

        packages = rec {
          clang = llvmPackage.stdenv.mkDerivation (
            commonAttrs
            // {
              nativeBuildInputs = clangNativeBuildInputs ++ [ llvmPackage.libcxx ];

              buildInputs = [ llvmPackage.libcxx ];

              hardeningDisable = [ "all" ];

              preConfigure = ''
                # Create wrapper for clang++ to force libc++
                mkdir -p $TMPDIR/wrappers
                cat > $TMPDIR/wrappers/clang++ << EOF
                #!/bin/sh
                exec ${llvmPackage.libcxxClang}/bin/clang++ \
                  -stdlib=libc++ \
                  -Wno-unused-command-line-argument \
                  -isystem ${llvmPackage.libcxx.dev}/include/c++/v1 \
                  "\$@"
                EOF
                chmod +x $TMPDIR/wrappers/clang++

                # Pass through wrapper for clang
                ln -s ${llvmPackage.libcxxClang}/bin/clang $TMPDIR/wrappers/clang

                # Create wrapper for clang-tidy for libc++ headers
                cat > $TMPDIR/wrappers/clang-tidy << EOF
                #!/bin/sh
                exec ${clangToolsWithLibcxx}/bin/clang-tidy \
                  --extra-arg=-stdlib=libc++ \
                  --extra-arg=-isystem${llvmPackage.libcxx.dev}/include/c++/v1 \
                  "\$@"
                EOF
                chmod +x $TMPDIR/wrappers/clang-tidy

                ln -s ${clangToolsWithLibcxx}/bin/clang-scan-deps $TMPDIR/wrappers/clang-scan-deps

                export PATH="$TMPDIR/wrappers:$PATH"
              '';

              setupHook = pkgs.writeText "setup-hook.sh" ''
                addLibmahjongLibs() {
                  addToSearchPath LD_LIBRARY_PATH $1/lib
                }
                addEnvHooks "$targetOffset" addLibmahjongLibs
              '';
            }
          );

          tests =
            pkgs.runCommand "libmahjong-tests"
              {
                nativeBuildInputs = clangNativeBuildInputs;
                src = ./.;
                hardeningDisable = [ "all" ];
              }
              ''
                # Create output directory
                mkdir -p $out

                # Create build directory and configure with tests enabled
                cmake -S $src \
                      -B build \
                      -G Ninja \
                      -Dlibmahjong_build_tools=OFF \
                      -Dlibmahjong_build_tests=ON

                # Build the project with tests
                cmake --build build

                # Run tests with JUnit output
                (ctest --test-dir build --output-on-failure --output-junit $out/test.xml || 
                  (echo "Tests failed but continuing build" && cp -r build/Testing $out/test-details))
              '';

          coverage =
            pkgs.runCommand "libmahjong-coverage"
              {
                nativeBuildInputs = clangNativeBuildInputs;
                src = ./.;
                hardeningDisable = [ "all" ];
              }
              ''
                mkdir -p $out

                cmake -S $src \
                      -B build \
                      -G Ninja \
                      -DCMAKE_BUILD_TYPE=Debug \
                      -Dlibmahjong_build_tools=OFF \
                      -Dlibmahjong_build_tests=ON \
                      -Dlibmahjong_enable_coverage=ON

                cmake --build build

                mkdir -p build/coverage
                LLVM_PROFILE_FILE="$PWD/build/coverage/coverage-%p.profraw" ctest --test-dir build --output-on-failure

                # Find all test executables
                TEST_BINARIES=$(find build/tests -type f -executable ! -name "*.so" 2>/dev/null | grep -v CMakeFiles | tr '\n' ' ')
                # Find the main library
                MAIN_LIB=$(find build -name "libmahjong.so" -type f | head -1)

                ${llvmPackage.llvm}/bin/llvm-profdata merge \
                  -sparse build/coverage/*.profraw \
                  -o $out/coverage.profdata

                # We need to provide some sort of binary for the reports/exports,
                # but all the cov data got generated when we ran ctest. Just use the first one
                FIRST_BINARY=$(echo $TEST_BINARIES | cut -d' ' -f1)

                ${llvmPackage.llvm}/bin/llvm-cov report \
                  $FIRST_BINARY \
                  -object=$MAIN_LIB \
                  -instr-profile=$out/coverage.profdata \
                  $src/src \
                  > $out/coverage-summary.txt

                ${llvmPackage.llvm}/bin/llvm-cov show \
                  $FIRST_BINARY \
                  -object=$MAIN_LIB \
                  -instr-profile=$out/coverage.profdata \
                  -format=html \
                  -output-dir=$out/html \
                  $src/src

                ${llvmPackage.llvm}/bin/llvm-cov export \
                  $FIRST_BINARY \
                  -object=$MAIN_LIB \
                  -instr-profile=$out/coverage.profdata \
                  -format=lcov \
                  $src/src \
                  > $out/coverage.lcov

                ${llvmPackage.llvm}/bin/llvm-cov export \
                  $FIRST_BINARY \
                  -object=$MAIN_LIB \
                  -instr-profile=$out/coverage.profdata \
                  $src/src \
                  > $out/coverage.json
              '';

          default = clang;
        };

        lib = {
          libmahjong = self.packages.${system}.default;
        };
      }
    );
}
