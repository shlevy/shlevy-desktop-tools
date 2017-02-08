pkgs:

(pkgs.callPackages ./scripts.nix {}) //
(pkgs.callPackages ./elisp.nix { inherit (pkgs.emacsPackagesNg) org-plus-contrib; }) //
  { title-loop = pkgs.callPackage ./title-loop.nix {};
    move-mail = pkgs.callPackage ./move-mail.nix {};
  }
