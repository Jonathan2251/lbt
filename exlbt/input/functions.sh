prologue() {
  LBDEXDIR=../../lbdex

  if [ $argNum == 0 ]; then
    echo "useage: bash $sh_name cpu_type endian"
    echo "  cpu_type: cpu032I or cpu032II"
    echo "  endian: be (big endian, default) or le (little endian)"
    echo "for example:"
    echo "  bash build-slinker.sh cpu032I be"
    exit 1;
  fi
  if [ $arg1 != cpu032I ] && [ $arg1 != cpu032II ]; then
    echo "1st argument is cpu032I or cpu032II"
    exit 1
  fi

  INCDIR=../../lbdex/input
  OS=`uname -s`
  echo "OS =" ${OS}

  if [ "$OS" == "Linux" ]; then
    CLANG=~/llvm/release/cmake_release_build/bin/clang
    TOOLDIR=~/llvm/test/cmake_debug_build/bin
  else
    CLANG=clang
    TOOLDIR=~/llvm/test/cmake_debug_build/Debug/bin
  fi

  CPU=$arg1
  echo "CPU =" "${CPU}"

  if [ "$arg2" != "" ] && [ $arg2 != le ] && [ $arg2 != be ]; then
    echo "2nd argument is be (big endian, default) or le (little endian)"
    exit 1
  fi
  if [ "$arg2" == "" ] || [ $arg2 == be ]; then
    endian=
  else
    endian=el
  fi
  echo "endian =" "${endian}"

  bash clean.sh
}

isLittleEndian() {
  echo "endian = " "$endian"
  if [ "$endian" == "LittleEndian" ] ; then
    le="true"
  elif [ "$endian" == "BigEndian" ] ; then
    le="false"
  else
    echo "!endian unknown"
    exit 1
  fi
}

elf2hex() {
  ${TOOLDIR}/elf2hex -le=${le} a.out > ${LBDEXDIR}/verilog/cpu0.hex
  if [ ${le} == "true" ] ; then
    echo "1   /* 0: big endian, 1: little endian */" > ${LBDEXDIR}/verilog/cpu0.config
  else
    echo "0   /* 0: big endian, 1: little endian */" > ${LBDEXDIR}/verilog/cpu0.config
  fi
  cat ${LBDEXDIR}/verilog/cpu0.config
}

epilogue() {
  endian=`${TOOLDIR}/llvm-readobj -h a.out|grep "DataEncoding"|awk '{print $2}'`
  isLittleEndian;
  elf2hex;
}
