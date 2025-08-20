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

        llvmPackage = pkgs.llvmPackages_20;

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

        clangNativeBuildInputs =
          buildPackages
          ++ (with llvmPackage; [
            clang-tools # Add clang-tools which includes clang-tidy
            libcxx
            clang
            lldb # Use LLDB for debugging instead of GDB
          ])
          ++ (with pkgs; [
            clangGtest # Use our clang-built GTest instead of pkgs.gtest
            clangGtest.dev # Include the development headers
          ]);
      in
      {
        devShells.default = pkgs.mkShell.override { stdenv = llvmPackage.stdenv; } {
          nativeBuildInputs = clangNativeBuildInputs;

          # Disable all hardening
          hardeningDisable = [ "all" ];

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
              -DCMAKE_CXX_FLAGS="-O1 -g -I${clangGtest.dev}/include -I${llvmPackage.libcxx}/include/c++/v1"
          '';
        };

        packages = rec {
          gcc = pkgs.stdenv.mkDerivation (
            commonAttrs
            // {
              nativeBuildInputs = buildPackages;
              # Add any runtime dependencies your library needs
              propagatedBuildInputs = with pkgs; [
                # Add dependencies that users of your library will need
              ];

              # This ensures dependent packages can find your library
              setupHook = pkgs.writeText "setup-hook.sh" ''
                addLibmahjongLibs() {
                  addToSearchPath LD_LIBRARY_PATH $1/lib
                }
                addEnvHooks "$targetOffset" addLibmahjongLibs
              '';
            }
          );

          clang = llvmPackage.stdenv.mkDerivation (
            commonAttrs
            // {
              nativeBuildInputs = clangNativeBuildInputs;

              hardeningDisable = [ "all" ];

              # This ensures dependent packages can find your library
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
                nativeBuildInputs = clangNativeBuildInputs ++ [ pkgs.lcov ];
                src = ./.;
                hardeningDisable = [ "all" ];
              }
              ''
                # Create output directory
                mkdir -p $out

                # Create build directory and configure with tests and coverage enabled
                cmake -S $src \
                      -B build \
                      -G Ninja \
                      -DCMAKE_BUILD_TYPE=Debug \
                      -Dlibmahjong_build_tools=OFF \
                      -Dlibmahjong_build_tests=ON \
                      -Dlibmahjong_enable_coverage=ON

                # Build the project with tests
                cmake --build build

                # Run tests to generate coverage data
                ctest --test-dir build --output-on-failure

                # Capture coverage data
                lcov --directory build --capture --output-file $out/coverage.info

                # Filter out system files and test files
                lcov --remove $out/coverage.info \
                     '/usr/*' \
                     '*/nix/store/*' \
                     '*/tests/*' \
                     '*/gtest/*' \
                     --output-file $out/coverage.info

                # Generate HTML report
                genhtml $out/coverage.info --output-directory $out/html

                # Debug coverage summary
                lcov --list $out/coverage.info > $out/coverage.txt
              '';

          default = clang;
        };

        lib = {
          libmahjong = self.packages.${system}.default;
        };
      }
    );
}
