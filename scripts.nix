{ runCommand }:
let scripts = builtins.attrNames (builtins.readDir ./scripts);
in builtins.listToAttrs (map (script:
  { name = script;
    value = runCommand script {} ''
      mkdir -p $out/bin
      cp ${./scripts + "/${script}"} $out/bin/${script}
      patchShebangs $out
    '';
  }) scripts)
