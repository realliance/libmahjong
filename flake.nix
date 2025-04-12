{
  description = "Libmahjong Riichi Majong Game Engine";

  inputs = {
    nixpkgs.url = "github:NixOS/nixpkgs";
    flake-utils.url = "github:numtide/flake-utils";
  };

  outputs = { self, nixpkgs, flake-utils }:
    flake-utils.lib.eachDefaultSystem (system:
      let
        pkgs = import nixpkgs { inherit system; };

        packages = with pkgs; [
          cmake
        ];

        googletest = pkgs.fetchFromGitHub {
          owner = "google";
          repo = "googletest";
          rev = "e90fe2485641bab0d6af4500192dc503384950d1";
          sha256 = "ddvZed7VynQkorrBGPK3jIhf2orzb5eiZZlUrH+bYoQ=";
        };

        flags = [
          "-DFETCHCONTENT_SOURCE_DIR_GOOGLETEST=${googletest}"
        ];
      in
      {
        devShells = {
          default = pkgs.mkShell.override { stdenv = pkgs.clangStdenv; } {
            packages = packages ++ [ pkgs.clang ];
          };
        };

        packages = rec {
          libmahjong-gcc = pkgs.stdenv.mkDerivation rec {
            name = "libmahjong-gcc";

            src = ./.;

            cmakeFlags = flags ++ [
              "-Dlibmahjong_clang_tidy=OFF"
            ];
            
            nativeBuildInputs = packages;
          };
          
          libmahjong-clang = pkgs.clangStdenv.mkDerivation rec {
            name = "libmahjong-clang";

            src = ./.;

            cmakeFlags = flags;
            
            nativeBuildInputs = packages ++ [ pkgs.clang ];
          };

          default = libmahjong-clang;
        };
      }
    );
}
