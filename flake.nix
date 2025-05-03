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

        llvmPackage = pkgs.llvmPackages_20;

        clangNativeBuildInputs = buildPackages ++ (with llvmPackage; [
          clang-tools  # Add clang-tools which includes clang-tidy
          libcxx
          clang
        ]) ++ (with pkgs; [
          gtest
        ]);
      in
      {
        devShells.default = pkgs.mkShell.override { stdenv = llvmPackage.stdenv; } {
          nativeBuildInputs = clangNativeBuildInputs;

          hardeningDisable = [ "all" ];
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
