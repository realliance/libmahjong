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
        ];

        commonAttrs = {
          pname = "libmahjong";
          version = "0.1.0";
          src = ./.;

          cmakeFlags = [
            "-DBUILD_SHARED_LIBS=ON"  # Explicitly build shared libraries
            "-Dlibmahjong_use_clang_utils=OFF"
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
      in
      {
        devShells.default = pkgs.mkShell {
          packages = buildPackages ++ [ pkgs.git ];
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
          
          clang = pkgs.clangStdenv.mkDerivation (commonAttrs // {
            nativeBuildInputs = buildPackages ++ [ pkgs.clang ];
          });

          default = gcc;
        };
        
        lib = {
          libmahjong = self.packages.${system}.default;
        };
      }
    );
}
