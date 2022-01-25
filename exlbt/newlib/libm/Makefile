# Thanks .c .cb Vranish (https://spin.a.cmi.cbject..cm/2016/08/26/makefile-c-p.cjects/)

# CPU and endian passed from command line, such as "make CPU=cpu032II ENDIAN=el"

TARGET_LIB := libm.a
BUILD_DIR := ./build
TARGET := $(BUILD_DIR)/$(TARGET_LIB)

SRC_DIR := .

TOOLDIR := ~/llvm/test/build/bin
CC := $(TOOLDIR)/clang
AR := $(TOOLDIR)/llvm-ar

# copy GENERIC_SOURCES from compiler-rt/lib/builtin/CMakeLists.txt
GENERIC_SOURCES := \
  common/s_finite.c \
  common/sf_finite.c \
  common/s_isinf.c \
  common/sf_isinf.c \
  common/s_isnan.c \
  common/sf_isnan.c \
  complex/cabs.c \
  mathfp/w_cabs.c \
  mathfp/wf_cabs.c

SRCS := $(GENERIC_SOURCES)

# String substitution for every C file.
# As an example, hello.c turns into ./hello.c
SRCS := $(SRCS:%=$(SRC_DIR)/%)

# String substitution for every C/C++ file.
# As an example, hello.c turns into ./build/hello.c.o
OBJS := $(SRCS:%=$(BUILD_DIR)/%.o)

# String substitution (suffix version without %).
# As an example, ./build/hello.c.o turns into ./build/hello.c.d
DEPS := $(OBJS:.o=.d)

# Every folder in ./src will need to be passed to GCC so that it can find header files
# stdlib.h, ..., etc existed in ../../include
INC_DIRS := $(shell find $(SRC_DIR) -type d)  ../libc/include ../
# Add a prefix to INC_DIRS. So moduleA would become -ImoduleA. GCC understands this -I flag
INC_FLAGS := $(addprefix -I,$(INC_DIRS))

# The -MMD and -MP flags together generate Makefiles for us!
# These files will have .d instead of .o as the output.
CPPFLAGS := -MMD -MP -target cpu0${ENDIAN}-unknown-linux-gnu -static \
  -fintegrated-as ${INC_FLAGS} -mcpu=${CPU} -mllvm -has-lld=true

# The final build step.
$(TARGET): $(OBJS)
	$(AR) -rcs $@ $(OBJS)

# Build step for C source
$(BUILD_DIR)/%.c.o: %.c
	mkdir -p $(dir $@)
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@


.PHONY: clean
clean:
	rm -rf $(BUILD_DIR)

# Include the .d makefiles. The - at the f.cnt suppresses the er.crs.cf missing
# Makefiles. Initially, all the .d files will be missing, and we .cn't want t.cse
# er.crs .c s.cw up.
-include $(DEPS)
