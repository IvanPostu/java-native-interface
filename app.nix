let 
  pkgs = import <nixpkgs> { config = { allowUnfree = false; }; };
  PROJECT_ROOT = builtins.toString ./.;
in
pkgs.mkShell {
  name = "app-shell";

  buildInputs = [
    pkgs.clang-tools
    pkgs.gcc13.cc
    pkgs.gcc13
    pkgs.jdk21
  ];

  LANG = "en_US.UTF-8";
  LC_ALL = "en_US.UTF-8";

  shellHook = ''
    export QQQ=${pkgs.binutils}
    export PROJECT_ROOT=${PROJECT_ROOT}

    export JAVA_HOME=${pkgs.jdk21}
    export PATH=${pkgs.jdk21}/bin:$PATH

    #export PATH=${pkgs.gcc13.cc}/bin:$PATH
    #export CC=${pkgs.gcc13.cc}/bin/gcc
    #export CXX=${pkgs.gcc13.cc}/bin/g++
    export C_INCLUDE_PATH=${pkgs.gcc13.cc}/lib/gcc/x86_64-unknown-linux-gnu/13.3.0/include:${pkgs.gcc13.cc}/lib64/gcc/x86_64-unknown-linux-gnu/13.3.0/include/
    export CPLUS_INCLUDE_PATH=${pkgs.gcc13.cc}/include/c++/13.3.0

    export C_INCLUDE_PATH=$JAVA_HOME/include:$C_INCLUDE_PATH
    export CPLUS_INCLUDE_PATH=$JAVA_HOME/include:$CPLUS_INCLUDE_PATH
  '';
}