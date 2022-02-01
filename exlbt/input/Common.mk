# Thanks https://makefiletutorial.com

TARGET_EXEC := a.out
BUILD_DIR := ./build
TARGET := $(BUILD_DIR)/$(TARGET_EXEC)
SRC_DIR := ./

TOOLDIR := ~/llvm/test/build/bin
CC := $(TOOLDIR)/clang
LLC := $(TOOLDIR)/llc
LD := $(TOOLDIR)/ld.lld
AS := $(TOOLDIR)/clang -static -fintegrated-as -c
RANLIB := $(TOOLDIR)/llvm-ranlib
READELF := $(TOOLDIR)/llvm-readelf

# String substitution for every C/C++ file.
# As an example, hello.cpp turns into ./build/hello.cpp.o
OBJS := $(SRCS:%=$(BUILD_DIR)/%.o)

# String substitution (suffix version without %).
# As an example, ./build/hello.cpp.o turns into ./build/hello.cpp.d
DEPS := $(OBJS:.o=.d)

# Add a prefix to INC_DIRS. So moduleA would become -ImoduleA. GCC understands this -I flag
INC_FLAGS := $(addprefix -I,$(INC_DIRS))

# The -MMD and -MP flags together generate Makefiles for us!
# These files will have .d instead of .o as the output.
# fintegrated-as: for asm code in C/C++
CPPFLAGS := -MMD -MP -target cpu0$(ENDIAN)-unknown-linux-gnu -static \
  -fintegrated-as $(INC_FLAGS) -mcpu=$(CPU) -mllvm -has-lld=true -DHAS_COMPLEX

LLFLAGS := -march=cpu0$(ENDIAN) -mcpu=$(CPU) -relocation-model=static \
  -filetype=obj -has-lld=true

CFLAGS := -target cpu0$(ENDIAN)-unknown-linux-gnu -static -mcpu=$(CPU) \
          -fintegrated-as -Wno-error=implicit-function-declaration
CONFIGURE := CC="$(CC)" CFLAGS="$(CFAGS)" AS="$(AS)" RANLIB="$(RANLIB)" READELF="$(READELF)" ../newlib/configure --host=cpu0

#FIND_LIBBUILTINS_DIR := $(shell find . -iname $(LIBBUILTINS_DIR))

$(TARGET): $(OBJS) $(LIBS)
	$(LD) -o $@ $(OBJS) $(LIBS)

$(LIBS):
ifdef LIBBUILTINS_DIR
	$(MAKE) -C $(LIBBUILTINS_DIR) 
endif

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
ifdef LIBBUILTINS_DIR
	cd $(LIBBUILTINS_DIR) && $(MAKE) -f Makefile clean
endif
ifdef NEWLIB_DIR
	cd $(NEWLIB_DIR) && rm -rf build/*
endif

# Include the .d makefiles. The - at the f.cnt suppresses the er.crs.cf missing
# Makefiles. Initially, all the .d files will be missing, and we .cn't want t.cse
# er.crs .c s.cw up.
-include $(DEPS)
