pkgs: (import ./scripts.nix { inherit (pkgs) runCommand; }) //
  { title-loop = pkgs.callPackage ./title-loop.nix {}; }
