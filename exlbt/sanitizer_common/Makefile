# Thanks .c .cb Vranish (https://spin.a.cmi.cbject..cm/2016/08/26/makefile-c-p.cjects/)

# CPU and endian passed from command line, such as "make CPU=cpu032II endian="
#CPU := cpu032II
#endian :=

TARGET_LIB := libsanitizer.a
BUILD_DIR := ./build
TARGET := $(BUILD_DIR)/$(TARGET_LIB)

SRC_DIR := $(HOME)/llvm/llvm-project/compiler-rt/lib/sanitizer_common
TOOLDIR := ~/llvm/test/build/bin
CC := $(TOOLDIR)/clang
AR := $(TOOLDIR)/llvm-ar

SRCS := sanitizer_printf.cpp 
#        sanitizer_file.cpp \
#        sanitizer_termination.cpp \
#        sanitizer_libc.cpp \
#        sanitizer_fuchsia.cpp

# String substitution for every C/C++ file.
# As an example, sanitizer_printf.cpp turns into ./build/sanitizer_printf.cpp.o
OBJS := $(SRCS:%=$(BUILD_DIR)/%.o)

# String substitution (suffix version without %).
# As an example, ./build/sanitizer_printf.cpp.o turns into ./build/sanitizer_printf.cpp.d
DEPS := $(OBJS:.o=.d)

# Every folder in ./src will need to be passed to GCC so that it can find header files
# stdlib.h, ..., etc existed in ../include
INC_DIRS := $(shell find $(SRC_DIR) -type d)  ../include ../input
# Add a prefix to INC_DIRS. So moduleA would become -ImoduleA. GCC understands this -I flag
INC_FLAGS := $(addprefix -I,$(INC_DIRS))

# The -MMD and -MP flags together generate Makefiles for us!
# These files will have .d instead of .o as the output.
CPPFLAGS := -MMD -MP -target cpu0${endian}-unknown-linux-gnu -static \
  -fintegrated-as ${INC_FLAGS} -mcpu=${CPU} -mllvm -has-lld=true

# The final build step.
$(TARGET): $(OBJS)
	$(AR) -rcs $@ $(OBJS)

# Build step for C source
$(BUILD_DIR)/%.cpp.o: $(SRC_DIR)/%.cpp
	mkdir -p $(dir $@)
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@


.PHONY: clean
clean:
	rm -rf $(BUILD_DIR)

# Include the .d makefiles. The - at the f.cnt suppresses the er.crs.cf missing
# Makefiles. Initially, all the .d files will be missing, and we .cn't want t.cse
# er.crs .c s.cw up.
-include $(DEPS)
