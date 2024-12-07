{
  description = "A simple C program using jansson built with Nix flakes";

  inputs = {
    nixpkgs.url = "github:NixOS/nixpkgs/nixpkgs-unstable";
  };

  outputs = { self, nixpkgs }: {
    packages = nixpkgs.lib.genAttrs [ "aarch64-darwin" "x86_64-linux" ] (system:
    let pkgs = import nixpkgs { inherit system; };
    in rec {
      wideriver = pkgs.stdenv.mkDerivation {
        pname = "wideriver";
        version = "1.0";

        src = ./.;

        nativeBuildInputs = with pkgs; [
          wayland-scanner
        ];
        buildInputs = with pkgs; [
          clang
          wayland
          wayland-scanner
          wlr-protocols
          wlroots
          pkg-config
        ];

        buildPhase = ''
          make
        '';

        installFlags = [ "PREFIX=$(out)" ];

        installPhase = ''
          mkdir -p $out/bin
          cp wideriver $out/bin/
        '';

        meta = with pkgs.lib; {
          description = "A simple program using jansson";
          license = licenses.mit;
          maintainers = [ maintainers.yourname ];
          platforms = platforms.unix;
        };
      };
    });

    devShell = nixpkgs.lib.genAttrs [ "aarch64-darwin" "x86_64-linux" ] (system:
    let pkgs = import nixpkgs { inherit system; };
    in pkgs.mkShell {
      nativeBuildInputs = with pkgs; [
        wayland-scanner
      ];
      buildInputs = with pkgs; [
        clang
        clang-tools  # Includes clangd
		cppcheck
		cmocka
		include-what-you-use
		valgrind
        wayland
        wayland-scanner
        wlr-protocols
        wlroots
        pkg-config
      ];

      shellHook = ''
        echo "Development environment loaded."
      '';
    });

    defaultPackage = {
      aarch64-darwin = self.packages.aarch64-darwin.wideriver;
      x86_64-linux = self.packages.x86_64-linux.wideriver;
    };

    defaultApp = {
      forAllSystems = nixpkgs.lib.mapAttrs' (system: pkg: {
        inherit system;
        defaultApp = {
          type = "app";
          program = "${pkg.wideriver}/bin/wideriver";
        };
      }) self.packages;
    };
  };
}
