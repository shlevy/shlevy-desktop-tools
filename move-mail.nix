{ stdenv }:

stdenv.mkDerivation
  { name = "move-mail";
    unpackPhase = "true";
    buildPhase = "cc ${./move-mail.c} -O3 -o move-mail";
    installPhase = "mkdir -p $out/bin; install -m755 move-mail $out/bin";
  }
