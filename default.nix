{ pkgs ? import <nixpkgs> {} }:
  pkgs.mkShell {
    # nativeBuildInputs is usually what you want -- tools you need to run
    nativeBuildInputs = with pkgs.buildPackages; 
    [   
        xorg.libX11
        xorg.libXrandr
        xorg.libXi
        glfw
        pkg-config 
    ];
}
