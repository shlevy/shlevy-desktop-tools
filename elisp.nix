{ emacsWithPackages, fetchFromGitHub, fetchFromBitbucket, stdenv, org-plus-contrib }:
let make-emacs = args: stdenv.mkDerivation (args //
      { buildInputs = [ (emacsWithPackages (args.emacsPackages or [])) ];
        buildPhase = "emacs -batch -f package-initialize -f batch-byte-compile *.el";
        installPhase = ''
          mkdir -p $out/share/emacs/site-lisp; install -m644 *.el *.elc $out/share/emacs/site-lisp
        '';
      });
in
  { org-drill = make-emacs
      { name = "org-drill-01b05cd";
        src = fetchFromBitbucket
          { owner = "eeeickythump";
            repo = "org-drill";
            rev = "01b05cd7561ad69e5ec9c1200414d4fa128c9a17";
            sha256 = "1w69hp2fbmq03xm10yalxrm8f7mhrbjc689k5wjnqgrqcwipi5gj";
          };
        emacsPackages = [ org-plus-contrib ];
      };
  }
