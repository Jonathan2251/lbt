# Thanks .c .cb Vranish (https://spin.a.cmi.cbject..cm/2016/08/26/makefile-c-p.cjects/)

# CPU and endian passed from command line, such as "make CPU=cpu032II ENDIAN=el"

TARGET_LIB := libsanitizer.a
BUILD_DIR := ./build
TARGET := $(BUILD_DIR)/$(TARGET_LIB)

SRC_DIR := $(HOME)/llvm/llvm-project/compiler-rt/lib/sanitizer_common

PWD := $(shell pwd)

TOOLDIR := ~/llvm/test/build/bin
CC := $(TOOLDIR)/clang
AR := $(TOOLDIR)/llvm-ar

# copy SANITIZER_SOURCES from compiler-rt/lib/sanitizer_common/CMakeLists.txt
GENERIC_SOURCES := \
  sanitizer_allocator.cpp \
  sanitizer_common.cpp \
  sanitizer_deadlock_detector1.cpp \
  sanitizer_deadlock_detector2.cpp \
  sanitizer_errno.cpp \
  sanitizer_file.cpp \
  sanitizer_flags.cpp \
  sanitizer_flag_parser.cpp \
  sanitizer_fuchsia.cpp \
  sanitizer_libc.cpp \
  sanitizer_libignore.cpp \
  sanitizer_netbsd.cpp \
  sanitizer_persistent_allocator.cpp \
  sanitizer_platform_limits_freebsd.cpp \
  sanitizer_platform_limits_linux.cpp \
  sanitizer_platform_limits_netbsd.cpp \
  sanitizer_platform_limits_posix.cpp \
  sanitizer_platform_limits_solaris.cpp \
  sanitizer_posix.cpp \
  sanitizer_printf.cpp \
  sanitizer_procmaps_common.cpp \
  sanitizer_procmaps_bsd.cpp \
  sanitizer_procmaps_fuchsia.cpp \
  sanitizer_procmaps_linux.cpp \
  sanitizer_procmaps_mac.cpp \
  sanitizer_procmaps_solaris.cpp \
  sanitizer_rtems.cpp \
  sanitizer_solaris.cpp \
  sanitizer_stoptheworld_fuchsia.cpp \
  sanitizer_stoptheworld_mac.cpp \
  sanitizer_suppressions.cpp \
  sanitizer_tls_get_addr.cpp \
  sanitizer_thread_registry.cpp \
  sanitizer_type_traits.cpp \
  sanitizer_win.cpp

SRCS := $(SANITIZER_SOURCES)

# String substitution for every C file.
# As an example, sanitizer_common.cpp turns into ./builtins/sanitizer_common.cpp
SRCS := $(SRCS:%=$(SRC_DIR)/%) $(PWD)/../cpu0/abort.c

# String substitution for every C/C++ file.
# As an example, sanitizer_common.cpp turns into ./build/sanitizer_common.cpp.o
OBJS := $(SRCS:%=$(BUILD_DIR)/%.o)

# String substitution (suffix version without %).
# As an example, ./build/sanitizer_common.cpp.o turns into ./build/sanitizer_common.cpp.d
DEPS := $(OBJS:.o=.d)

# Every folder in ./src will need to be passed to GCC so that it can find header files
# stdlib.h, ..., etc existed in ../../include
INC_DIRS := $(shell find $(SRC_DIR) -type d)  ../../include ../../newlib/libc/include  ../../newlib/libc/include/sys ../../newlib/
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

# Build step for CPP source
$(BUILD_DIR)/%.cpp.o: %.cpp
	mkdir -p $(dir $@)
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@


.PHONY: clean
clean:
	rm -rf $(BUILD_DIR)

# Include the .d makefiles. The - at the f.cnt suppresses the er.crs.cf missing
# Makefiles. Initially, all the .d files will be missing, and we .cn't want t.cse
# er.crs .c s.cw up.
-include $(DEPS)
