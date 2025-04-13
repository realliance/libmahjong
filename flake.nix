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
          git
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
          default = pkgs.mkShell {
            packages = packages ++ [ pkgs.git ];
          };
        };

        packages = rec {
          gcc = pkgs.stdenv.mkDerivation rec {
            name = "libmahjong";
            pname = "libmahjong";
            version = "0.1.0";

            src = ./.;

            cmakeFlags = flags ++ [
              "-Dlibmahjong_clang_tidy=OFF"
              "-DCMAKE_INSTALL_LIBDIR=lib"
              "-DCMAKE_INSTALL_INCLUDEDIR=include"
            ];
            
            nativeBuildInputs = packages;
            
            # Ensure headers and library files are installed
            postInstall = ''
              mkdir -p $out/lib
              cp libmahjong.so $out/lib/
            '';
            
            meta = with pkgs.lib; {
              description = "Riichi Mahjong Game Engine Library";
            };
          };
          
          clang = pkgs.clangStdenv.mkDerivation rec {
            name = "libmahjong";
            pname = "libmahjong";
            version = "0.1.0";

            src = ./.;

            cmakeFlags = flags ++ [
              "-DCMAKE_INSTALL_LIBDIR=lib"
              "-DCMAKE_INSTALL_INCLUDEDIR=include" 
            ];
            
            nativeBuildInputs = packages ++ [ pkgs.clang ];
            
            # Ensure headers and library files are installed
            postInstall = ''
              mkdir -p $out/lib
              cp libmahjong.so $out/lib/
            '';
            
            meta = with pkgs.lib; {
              description = "Riichi Mahjong Game Engine Library";
            };
          };

          default = gcc;
        };
        
        # Add library output for consumers
        lib = {
          libmahjong = self.packages.${system}.default;
        };
      }
    );
}
