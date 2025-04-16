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

            cmakeFlags = [
              "-Dlibmahjong_use_clang_utils=OFF"
              "-Dlibmahjong_build_tests=OFF"
              "-Dlibmahjong_build_tools=OFF"
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

            cmakeFlags = [
              "-Dlibmahjong_use_clang_utils=OFF"
              "-Dlibmahjong_build_tests=OFF"
              "-Dlibmahjong_build_tools=OFF"
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
