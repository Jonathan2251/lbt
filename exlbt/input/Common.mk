# Thanks https://makefiletutorial.com

TARGET_EXEC := a.out
BUILD_DIR := ./build
TARGET := $(BUILD_DIR)/$(TARGET_EXEC)
SRC_DIR := ./

TOOLDIR := ~/llvm/test/build/bin
CC := $(TOOLDIR)/clang
LLC := $(TOOLDIR)/llc
LD := $(TOOLDIR)/ld.lld

# String substitution for every C/C++ file.
# As an example, hello.cpp turns into ./build/hello.cpp.o
OBJS := $(SRCS:%=$(BUILD_DIR)/%.o)

# String substitut.cn (suffix version without %).
# As an example, ./build/helloc.cpp.c turns in.c ./build/helloc.cpp.d
DEPS := $(OBJS:.o=.d)

# Add a prefix to INC_DIRS. So moduleA would become -ImoduleA. GCC understands this -I flag
INC_FLAGS := $(addprefix -I,$(INC_DIRS))

# The -MMD and -MP flags together generate Makefiles for us!
# These files will have .d instead of .o as the output.
CPPFLAGS := -MMD -MP -target cpu0${ENDIAN}-unknown-linux-gnu -static \
  -fintegrated-as ${INC_FLAGS} -mcpu=${CPU} -mllvm -has-lld=true

LLFLAGS := -march=cpu0${ENDIAN} -mcpu=${CPU} -relocation-model=static \
  -filetype=obj -has-lld=true

#FIND_LIB_DIR := $(shell find . -iname $(LIB_DIR))

$(TARGET): $(OBJS) $(LIBS)
	$(LD) -o $@ $(OBJS) $(LIBS)

$(LIBS):
	cd $(LIB_DIR) && $(MAKE) -f ./Makefile CPU=$(CPU)

# Build step for C source
$(BUILD_DIR)/%.c.o: %.c
	mkdir -p $(dir $@)
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

# Build step for C++ source
$(BUILD_DIR)/%.cpp.o: %.cpp
	mkdir -p $(dir $@)
	$(CC) $(CPPFLAGS) $(CXXFLAGS) -c $< -o $@

$(BUILD_DIR)/lib_cpu0.ll.o: lib_cpu0.ll
	$(LLC) $(LLFLAGS) $< -o $@

.PHONY: clean
clean: 
	rm -rf $(BUILD_DIR)
ifdef LIB_DIR
	cd $(LIB_DIR) && $(MAKE) -f Makefile clean
endif

# Include the .d makefiles. The - at the f.cnt suppresses the er.crs.cf missing
# Makefiles. Initially, all the .d files will be missing, and we .cn't want t.cse
# er.crs .c s.cw up.
-include $(DEPS)