CC=clang
LLC=/usr/local/llvm/test/cmake_debug_build/bin/llc
LINK=/usr/local/llvm/test/cmake_debug_build/bin/lld
INCFLAG=-I/home/cschen/test/open-src-libc/avr-libc-1.8.1/include -I/home/cschen/test/open-src-libc/avr-libc-1.8.1/common
CFLAGS=-target mips-unknown-linux-gnu -emit-llvm ${INCFLAG} -S
LLCFLAGS=-march=cpu0 -relocation-model=static -filetype=obj
LDFLAGS=-flavor gnu -target cpu0-unknown-linux-gnu 
#SOURCES := absvdi2.c absvsi2.c absvti2.c adddf3.c addsf3.c addtf3.c addvdi3.c \
#  addvsi3.c addvti3.c apple_versioning.c ashldi3.c ashlti3.c ashrdi3.c \
#  ashrti3.c clear_cache.c clzdi2.c clzsi2.c clzti2.c cmpdi2.c cmpti2.c \
#  comparedf2.c comparesf2.c ctzdi2.c ctzsi2.c ctzti2.c divdc3.c divdf3.c \
#  divdi3.c divmoddi4.c divmodsi4.c divsc3.c divsf3.c divsi3.c divti3.c divtf3.c \
#  divxc3.c

SOURCES := absvdi2.c absvsi2.c absvti2.c 

OUTPUT_OBJ_DIR=obj
#$(sources:.c=.d)
#IRS := $(addprefix , $(addsuffix .o, ${SOURCES}))
IRS := $(SOURCES:.c=.l)
#IRS=$(SOURCES:.c=.ll)
OBJECTS=$(IRS:.l=.o)
EXECUTABLE=compiler-rt.bc

all: $(SOURCES) $(EXECUTABLE)
	
$(EXECUTABLE): $(OBJECTS)
	${LINK} ${LDFLAGS} ${OBJECTS} -o $@

$(OBJECTS): %.o: %.ll
	${LLC} ${LLCFLAGS} $< -o $@

$(IRS): $(SOURCES)
	${CC} ${CFLAGS} $< -o $@

.PHONY: clean
clean:
	rm -rf ${OUTPUT_OBJ_DIR} *.bc *.ll *.o

