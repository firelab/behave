let
  nixpkgs = fetchTarball "https://github.com/NixOS/nixpkgs/tarball/nixos-24.11";
  pkgs = import nixpkgs { config = {}; overlays = []; };
in

pkgs.mkShellNoCC {
  packages = with pkgs; [
    # Docs
    doxygen

    # Building/Tests
    gnumake
    cmake
    llvm
    clang
  ];
}
