{
  description = "Libmahjong Riichi Mahjong Game Engine";

  inputs = {
    nixpkgs.url = "github:NixOS/nixpkgs";
    flake-utils.url = "github:numtide/flake-utils";
  };

  outputs = { self, nixpkgs, flake-utils }:
    flake-utils.lib.eachDefaultSystem (system:
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
            "-DBUILD_SHARED_LIBS=ON"  # Explicitly build shared libraries
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

        clangNativeBuildInputs = buildPackages ++ (with llvmPackage; [
          clang-tools  # Add clang-tools which includes clang-tidy
          libcxx
          clang
        ]) ++ [
          clangGtest      # Use our clang-built GTest instead of pkgs.gtest
          clangGtest.dev  # Include the development headers
        ];
      in
      {
        devShells.default = pkgs.mkShell.override { stdenv = llvmPackage.stdenv; } {
          nativeBuildInputs = clangNativeBuildInputs;

          hardeningDisable = [ "all" ];

          shellHook = ''
            mkdir -p .vscode
            echo "{" > .vscode/settings.json
            echo '  "nixEnvSelector.nixFile": "''${workspaceFolder}/shell.nix",' >> .vscode/settings.json
            echo "  \"cmake.cmakePath\": \"${pkgs.cmake}/bin/cmake\"," >> .vscode/settings.json
            echo "  \"cmake.configureArgs\": [\"-DGTEST_LINKED_AS_SHARED_LIBRARY=1\",\"-DGTEST_MAIN_LIBRARY=${clangGtest}/lib/libgtest_main.so\", \"-DGTEST_LIBRARY=${clangGtest}/lib/libgtest.so\", \"-DGTEST_INCLUDE_DIR=${clangGtest.dev}/include\"]," >> .vscode/settings.json
            echo "  \"cmake.configureEnvironment\": {\"CMAKE_MAKE_PROGRAM\": \"${pkgs.ninja}/bin/ninja\"}," >> .vscode/settings.json
            echo "  \"C_Cpp.default.includePath\": [\"${clangGtest.dev}/include\"]," >> .vscode/settings.json
            echo "  \"cmake.ctestPath\": \"${pkgs.cmake}/bin/ctest\"" >> .vscode/settings.json
            echo "}" >> .vscode/settings.json

            rm -f build/CMakeCache.txt && cmake -S . -B build -G Ninja
          '';
        };

        packages = rec {
          gcc = pkgs.stdenv.mkDerivation (commonAttrs // {
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
          });
          
          clang = llvmPackage.stdenv.mkDerivation (commonAttrs // {
            nativeBuildInputs = clangNativeBuildInputs;

            hardeningDisable = [ "all" ];

            # This ensures dependent packages can find your library
            setupHook = pkgs.writeText "setup-hook.sh" ''
              addLibmahjongLibs() {
                addToSearchPath LD_LIBRARY_PATH $1/lib
              }
              addEnvHooks "$targetOffset" addLibmahjongLibs
            '';
          });

          tests = pkgs.runCommand "libmahjong-tests" {
            nativeBuildInputs = clangNativeBuildInputs;
            src = ./.;
            hardeningDisable = [ "all" ];
          } ''
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

          default = clang;
        };
        
        lib = {
          libmahjong = self.packages.${system}.default;
        };
      }
    );
}
