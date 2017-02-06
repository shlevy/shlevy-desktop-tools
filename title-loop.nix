{ stdenv, libX11 }:

stdenv.mkDerivation
  { name = "title-loop";
    unpackPhase = "true";
    buildInputs = [ libX11 ];
    buildPhase = "cc ${./title-loop.c} -O3 -lX11 -o title-loop";
    installPhase = "mkdir -p $out/bin; install -m755 title-loop $out/bin";
  }
