LBDEXDIR=../../lbdex
CC=clang
LLC=~/llvm/test/cmake_debug_build/bin/llc
LINK=~/llvm/test/cmake_debug_build/bin/lld
CFLAGS=-emit-llvm -c
CFLAGS-MIPS=-target mips-unknown-linux-gnu -emit-llvm -c
LLCFLAGS=-march=cpu0 -relocation-model=static -filetype=obj
LDFLAGS=-flavor gnu -target cpu0-unknown-linux-gnu 

SLINK_SRC1 := start.cpp debug.cpp printf-stdarg-def.c \
  ${LBDEXDIR}/input/ch4_1_2.cpp ${LBDEXDIR}/input/ch8_1_5.cpp \
  ${LBDEXDIR}/input/ch8_3.cpp ${LBDEXDIR}/input/ch8_5.cpp \
  

OUTPUT_OBJ_DIR=obj
#IRS := $(SOURCES:.c=.l)
#OBJECTS=$(IRS:.l=.o)
SOURCES=start.cpp debug.cpp printf-stdarg-def.c printf-stdarg.c ch4_1_2.cpp ch8_1_5.cpp ch8_3.cpp ch8_5.cpp ch9_2_3_tailcall.cpp ch9_4.cpp ch_slinker.cpp
IRS=start.bc debug.bc printf-stdarg-def.bc printf-stdarg.bc ch4_1_2.bc ch8_1_5.bc ch8_3.bc ch8_5.bc ch9_2_3_tailcall.bc ch9_4.bc ch_slinker.bc lib_cpu0.ll
OBJECTS=start.o debug.o printf-stdarg-def.o printf-stdarg.o ch4_1_2.o ch8_1_5.o ch8_3.o ch8_5.o ch9_2_3_tailcall.o ch9_4.o ch_slinker.o lib_cpu0.o
EXECUTABLE=compiler-rt.bc

a.out: ${OBJECTS}
	${LINK} ${LDFLAGS} ${OBJECTS} -o $@

start.o: start.bc
	${LLC} ${LLCFLAGS} $< -o $@

debug.o: debug.bc
	${LLC} ${LLCFLAGS} $< -o $@

printf-stdarg-def.o: printf-stdarg-def.bc
	${LLC} ${LLCFLAGS} $< -o $@

printf-stdarg.o: printf-stdarg.bc
	${LLC} ${LLCFLAGS} $< -o $@

ch4_1_2.o: ch4_1_2.bc
	${LLC} ${LLCFLAGS} $< -o $@

ch8_1_5.o: ch8_1_5.bc
	${LLC} ${LLCFLAGS} $< -o $@

ch8_3.o: ch8_3.bc
	${LLC} ${LLCFLAGS} $< -o $@

ch8_5.o: ch8_5.bc
	${LLC} ${LLCFLAGS} $< -o $@

ch9_2_3_tailcall.o: ch9_2_3_tailcall.bc
	${LLC} ${LLCFLAGS} $< -o $@

ch9_4.o: ch9_4.bc
	${LLC} ${LLCFLAGS} $< -o $@

ch_slinker.o: ch_slinker.bc
	${LLC} ${LLCFLAGS} $< -o $@

lib_cpu0.o: lib_cpu0.ll
	${LLC} ${LLCFLAGS} $< -o $@



start.bc: start.cpp
	${CC} ${CFLAGS-MIPS} $< -o $@

debug.bc: debug.cpp
	${CC} ${CFLAGS-MIPS} $< -o $@

printf-stdarg-def.bc: printf-stdarg-def.c
	${CC} ${CFLAGS-MIPS} $< -o $@

printf-stdarg.bc: printf-stdarg.c
	${CC} ${CFLAGS-MIPS} $< -o $@

ch4_1_2.bc: ${LBDEXDIR}/input/ch4_1_2.cpp
	${CC} ${CFLAGS} ${LBDEXDIR}/input/$< -o $@

ch8_1_5.bc: ${LBDEXDIR}/input/ch8_1_5.cpp
	${CC} -O1 ${CFLAGS} ${LBDEXDIR}/input/$< -o $@

ch8_3.bc: ${LBDEXDIR}/input/ch8_3.cpp
	${CC} -O1 ${CFLAGS} ${LBDEXDIR}/input/$< -o $@

ch8_5.bc: ${LBDEXDIR}/input/ch8_5.cpp
	${CC} -O1 ${CFLAGS} ${LBDEXDIR}/input/$< -o $@

ch9_2_3_tailcall.bc: ${LBDEXDIR}/input/ch9_2_3_tailcall.cpp
	${CC} -O1 -enable-cpu0-tail-calls ${CFLAGS} ${LBDEXDIR}/input/$< -o $@

ch9_4.bc: ${LBDEXDIR}/input/ch9_4.cpp
	${CC} ${CFLAGS} ${LBDEXDIR}/input/$< -o $@

ch_slinker.bc: ch_slinker.cpp ${LBDEXDIR}/input/ch_nolld.h
	${CC} -I${LBDEXDIR}/input/ ${CFLAGS-MIPS} $< -o $@


.PHONY: clean
clean:
	rm -rf ${OUTPUT_OBJ_DIR} *.bc *.o

