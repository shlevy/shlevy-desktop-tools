{ stdenv }:

stdenv.mkDerivation
  { name = "handle-lid";
    unpackPhase = "true";
    buildPhase = "cc ${./handle-lid.c} -O3 -o handle-lid";
    installPhase = "mkdir -p $out/bin; install -m755 handle-lid $out/bin";
  }
