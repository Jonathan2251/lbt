# Copyright (c) 2004,  Theodore A. Roth
# Copyright (c) 2005, 2008, 2009  Anatoly Sokolov
# All rights reserved.
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions are met:
#
# * Redistributions of source code must retain the above copyright
#   notice, this list of conditions and the following disclaimer.
# * Redistributions in binary form must reproduce the above copyright
#   notice, this list of conditions and the following disclaimer in
#   the documentation and/or other materials provided with the
#   distribution.
# * Neither the name of the copyright holders nor the names of
#   contributors may be used to endorse or promote products derived
#   from this software without specific prior written permission.
#
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
# AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
# IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
# ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
# LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
# CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
# SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
# INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
# CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
# ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
# POSSIBILITY OF SUCH DAMAGE.
#

SUBDIRS =  at90can32 at90can64 at90can128 at90pwm216 at90pwm316 at90pwm161 at90scr100 at90usb646 at90usb647 at90usb1286 at90usb1287 at94k ata5790 ata5795 atmega16 atmega16a atmega161 atmega162 atmega163 atmega164a atmega164p atmega164pa atmega165 atmega165a atmega165p atmega165pa atmega168 atmega168a atmega168p atmega168pa atmega169 atmega169a atmega169p atmega169pa atmega16hva atmega16hva2 atmega16hvb atmega16hvbrevb atmega16m1 atmega16u4 atmega32 atmega32a atmega323 atmega324a atmega324p atmega324pa atmega325 atmega325a atmega325p atmega325pa atmega3250 atmega3250a atmega3250p atmega3250pa atmega328 atmega328p atmega329 atmega329a atmega329p atmega329pa atmega3290 atmega3290a atmega3290p atmega3290pa atmega32c1 atmega32hvb atmega32hvbrevb atmega32m1 atmega32u4 atmega32u6 atmega406 atmega644rfr2 atmega64rfr2 atmega64 atmega64a atmega640 atmega644 atmega644a atmega644p atmega644pa atmega645 atmega645a atmega645p atmega6450 atmega6450a atmega6450p atmega649 atmega649a atmega649p atmega6490 atmega6490a atmega6490p atmega64c1 atmega64hve atmega64m1 atmega128 atmega1280 atmega1281 atmega1284p atmega128rfa1 m3000

AVR_TARGET          = avr5
AVR_TARGET_DEFS     = -D__COMPILING_AVR_LIBC__
AVR_TARGET_CFLAGS   = -mcall-prologues -Os
AVR_TARGET_ASFLAGS  = 
AVR_INSTALL_DIR     = avr5

VPATH = $(top_srcdir)/libc/stdlib:$(top_srcdir)/libc/pmstring:$(top_srcdir)/libc/string:$(top_srcdir)/libc/misc:$(top_srcdir)/libc/stdio:$(top_srcdir)/libm/fplib:$(top_srcdir)/libc/time

if HAS_avr5

AVRLIB_DEVLIST =
if HAS_at90can32
  AVRLIB_DEVLIST += at90can32
endif	# at90can32
if HAS_at90can64
  AVRLIB_DEVLIST += at90can64
endif	# at90can64
if HAS_at90can128
  AVRLIB_DEVLIST += at90can128
endif	# at90can128
if HAS_at90pwm216
  AVRLIB_DEVLIST += at90pwm216
endif	# at90pwm216
if HAS_at90pwm316
  AVRLIB_DEVLIST += at90pwm316
endif	# at90pwm316
if HAS_at90pwm161
  AVRLIB_DEVLIST += at90pwm161
endif	# at90pwm161
if HAS_at90scr100
  AVRLIB_DEVLIST += at90scr100
endif	# at90scr100
if HAS_at90usb646
  AVRLIB_DEVLIST += at90usb646
endif	# at90usb646
if HAS_at90usb647
  AVRLIB_DEVLIST += at90usb647
endif	# at90usb647
if HAS_at90usb1286
  AVRLIB_DEVLIST += at90usb1286
endif	# at90usb1286
if HAS_at90usb1287
  AVRLIB_DEVLIST += at90usb1287
endif	# at90usb1287
if HAS_at94k
  AVRLIB_DEVLIST += at94k
endif	# at94k
if HAS_ata5790
  AVRLIB_DEVLIST += ata5790
endif	# ata5790
if HAS_ata5795
  AVRLIB_DEVLIST += ata5795
endif	# ata5795
if HAS_atmega16
  AVRLIB_DEVLIST += atmega16
endif	# atmega16
if HAS_atmega16a
  AVRLIB_DEVLIST += atmega16a
endif	# atmega16a
if HAS_atmega161
  AVRLIB_DEVLIST += atmega161
endif	# atmega161
if HAS_atmega162
  AVRLIB_DEVLIST += atmega162
endif	# atmega162
if HAS_atmega163
  AVRLIB_DEVLIST += atmega163
endif	# atmega163
if HAS_atmega164a
  AVRLIB_DEVLIST += atmega164a
endif	# atmega164a
if HAS_atmega164p
  AVRLIB_DEVLIST += atmega164p
endif	# atmega164p
if HAS_atmega164pa
  AVRLIB_DEVLIST += atmega164pa
endif	# atmega164pa
if HAS_atmega165
  AVRLIB_DEVLIST += atmega165
endif	# atmega165
if HAS_atmega165a
  AVRLIB_DEVLIST += atmega165a
endif	# atmega165a
if HAS_atmega165p
  AVRLIB_DEVLIST += atmega165p
endif	# atmega165p
if HAS_atmega165pa
  AVRLIB_DEVLIST += atmega165pa
endif	# atmega165pa
if HAS_atmega168
  AVRLIB_DEVLIST += atmega168
endif	# atmega168
if HAS_atmega168a
  AVRLIB_DEVLIST += atmega168a
endif	# atmega168a
if HAS_atmega168p
  AVRLIB_DEVLIST += atmega168p
endif	# atmega168p
if HAS_atmega168pa
  AVRLIB_DEVLIST += atmega168pa
endif	# atmega168pa
if HAS_atmega169
  AVRLIB_DEVLIST += atmega169
endif	# atmega169
if HAS_atmega169a
  AVRLIB_DEVLIST += atmega169a
endif	# atmega169a
if HAS_atmega169p
  AVRLIB_DEVLIST += atmega169p
endif	# atmega169p
if HAS_atmega169pa
  AVRLIB_DEVLIST += atmega169pa
endif	# atmega169pa
if HAS_atmega16hva
  AVRLIB_DEVLIST += atmega16hva
endif	# atmega16hva
if HAS_atmega16hva2
  AVRLIB_DEVLIST += atmega16hva2
endif	# atmega16hva2
if HAS_atmega16hvb
  AVRLIB_DEVLIST += atmega16hvb
endif	# atmega16hvb
if HAS_atmega16hvbrevb
  AVRLIB_DEVLIST += atmega16hvbrevb
endif	# atmega16hvbrevb
if HAS_atmega16m1
  AVRLIB_DEVLIST += atmega16m1
endif	# atmega16m1
if HAS_atmega16u4
  AVRLIB_DEVLIST += atmega16u4
endif	# atmega16u4
if HAS_atmega32
  AVRLIB_DEVLIST += atmega32
endif	# atmega32
if HAS_atmega32a
  AVRLIB_DEVLIST += atmega32a
endif	# atmega32a
if HAS_atmega323
  AVRLIB_DEVLIST += atmega323
endif	# atmega323
if HAS_atmega324a
  AVRLIB_DEVLIST += atmega324a
endif	# atmega324a
if HAS_atmega324p
  AVRLIB_DEVLIST += atmega324p
endif	# atmega324p
if HAS_atmega324pa
  AVRLIB_DEVLIST += atmega324pa
endif	# atmega324pa
if HAS_atmega325
  AVRLIB_DEVLIST += atmega325
endif	# atmega325
if HAS_atmega325a
  AVRLIB_DEVLIST += atmega325a
endif	# atmega325a
if HAS_atmega325p
  AVRLIB_DEVLIST += atmega325p
endif	# atmega325p
if HAS_atmega325pa
  AVRLIB_DEVLIST += atmega325pa
endif	# atmega325pa
if HAS_atmega3250
  AVRLIB_DEVLIST += atmega3250
endif	# atmega3250
if HAS_atmega3250a
  AVRLIB_DEVLIST += atmega3250a
endif	# atmega3250a
if HAS_atmega3250p
  AVRLIB_DEVLIST += atmega3250p
endif	# atmega3250p
if HAS_atmega3250pa
  AVRLIB_DEVLIST += atmega3250pa
endif	# atmega3250pa
if HAS_atmega328
  AVRLIB_DEVLIST += atmega328
endif	# atmega328
if HAS_atmega328p
  AVRLIB_DEVLIST += atmega328p
endif	# atmega328p
if HAS_atmega329
  AVRLIB_DEVLIST += atmega329
endif	# atmega329
if HAS_atmega329a
  AVRLIB_DEVLIST += atmega329a
endif	# atmega329a
if HAS_atmega329p
  AVRLIB_DEVLIST += atmega329p
endif	# atmega329p
if HAS_atmega329pa
  AVRLIB_DEVLIST += atmega329pa
endif	# atmega329pa
if HAS_atmega3290
  AVRLIB_DEVLIST += atmega3290
endif	# atmega3290
if HAS_atmega3290a
  AVRLIB_DEVLIST += atmega3290a
endif	# atmega3290a
if HAS_atmega3290p
  AVRLIB_DEVLIST += atmega3290p
endif	# atmega3290p
if HAS_atmega3290pa
  AVRLIB_DEVLIST += atmega3290pa
endif	# atmega3290pa
if HAS_atmega32c1
  AVRLIB_DEVLIST += atmega32c1
endif	# atmega32c1
if HAS_atmega32hvb
  AVRLIB_DEVLIST += atmega32hvb
endif	# atmega32hvb
if HAS_atmega32hvbrevb
  AVRLIB_DEVLIST += atmega32hvbrevb
endif	# atmega32hvbrevb
if HAS_atmega32m1
  AVRLIB_DEVLIST += atmega32m1
endif	# atmega32m1
if HAS_atmega32u4
  AVRLIB_DEVLIST += atmega32u4
endif	# atmega32u4
if HAS_atmega32u6
  AVRLIB_DEVLIST += atmega32u6
endif	# atmega32u6
if HAS_atmega406
  AVRLIB_DEVLIST += atmega406
endif	# atmega406
if HAS_atmega644rfr2
  AVRLIB_DEVLIST += atmega644rfr2
endif	# atmega644rfr2
if HAS_atmega64rfr2
  AVRLIB_DEVLIST += atmega64rfr2
endif	# atmega64rfr2
if HAS_atmega64
  AVRLIB_DEVLIST += atmega64
endif	# atmega64
if HAS_atmega64a
  AVRLIB_DEVLIST += atmega64a
endif	# atmega64a
if HAS_atmega640
  AVRLIB_DEVLIST += atmega640
endif	# atmega640
if HAS_atmega644
  AVRLIB_DEVLIST += atmega644
endif	# atmega644
if HAS_atmega644a
  AVRLIB_DEVLIST += atmega644a
endif	# atmega644a
if HAS_atmega644p
  AVRLIB_DEVLIST += atmega644p
endif	# atmega644p
if HAS_atmega644pa
  AVRLIB_DEVLIST += atmega644pa
endif	# atmega644pa
if HAS_atmega645
  AVRLIB_DEVLIST += atmega645
endif	# atmega645
if HAS_atmega645a
  AVRLIB_DEVLIST += atmega645a
endif	# atmega645a
if HAS_atmega645p
  AVRLIB_DEVLIST += atmega645p
endif	# atmega645p
if HAS_atmega6450
  AVRLIB_DEVLIST += atmega6450
endif	# atmega6450
if HAS_atmega6450a
  AVRLIB_DEVLIST += atmega6450a
endif	# atmega6450a
if HAS_atmega6450p
  AVRLIB_DEVLIST += atmega6450p
endif	# atmega6450p
if HAS_atmega649
  AVRLIB_DEVLIST += atmega649
endif	# atmega649
if HAS_atmega649a
  AVRLIB_DEVLIST += atmega649a
endif	# atmega649a
if HAS_atmega649p
  AVRLIB_DEVLIST += atmega649p
endif	# atmega649p
if HAS_atmega6490
  AVRLIB_DEVLIST += atmega6490
endif	# atmega6490
if HAS_atmega6490a
  AVRLIB_DEVLIST += atmega6490a
endif	# atmega6490a
if HAS_atmega6490p
  AVRLIB_DEVLIST += atmega6490p
endif	# atmega6490p
if HAS_atmega64c1
  AVRLIB_DEVLIST += atmega64c1
endif	# atmega64c1
if HAS_atmega64hve
  AVRLIB_DEVLIST += atmega64hve
endif	# atmega64hve
if HAS_atmega64m1
  AVRLIB_DEVLIST += atmega64m1
endif	# atmega64m1
if HAS_atmega128
  AVRLIB_DEVLIST += atmega128
endif	# atmega128
if HAS_atmega1280
  AVRLIB_DEVLIST += atmega1280
endif	# atmega1280
if HAS_atmega1281
  AVRLIB_DEVLIST += atmega1281
endif	# atmega1281
if HAS_atmega1284p
  AVRLIB_DEVLIST += atmega1284p
endif	# atmega1284p
if HAS_atmega128rfa1
  AVRLIB_DEVLIST += atmega128rfa1
endif	# atmega128rfa1
if HAS_m3000
  AVRLIB_DEVLIST += m3000
endif	# m3000

AM_CPPFLAGS = -I$(top_srcdir)/common -I$(top_srcdir)/include -I$(top_builddir)/include

AVRLIB_CFLAGS = @CDEBUG@ -Wall -W -Wstrict-prototypes -mmcu=$(AVR_TARGET) $(AVR_TARGET_DEFS) $(AVR_TARGET_CFLAGS)

AVRLIB_ASFLAGS = -x assembler-with-cpp @ASDEBUG@ -mmcu=$(AVR_TARGET) $(AVR_TARGET_DEFS) $(AVR_TARGET_ASFLAGS)

AM_CFLAGS = $(AVRLIB_CFLAGS)
AM_CCASFLAGS = $(AM_CPPFLAGS) $(AVRLIB_ASFLAGS)

__install_dir = $(prefix)/avr/lib/$(AVR_INSTALL_DIR)

avrdir = $(__install_dir)
avrlibdir = $(__install_dir)

avr_LIBRARIES = \
	libc.a \
	libprintf_min.a \
	libprintf_flt.a \
	libscanf_min.a \
	libscanf_flt.a \
	libm.a

include $(top_srcdir)/libc/stdlib/Rules.am
include $(top_srcdir)/libc/pmstring/Rules.am
include $(top_srcdir)/libc/string/Rules.am
include $(top_srcdir)/libc/misc/Rules.am
include $(top_srcdir)/libc/stdio/Rules.am
include $(top_srcdir)/libm/fplib/Rules.am
include $(top_srcdir)/libc/time/Rules.am

nodist_libc_a_SOURCES = \
	$(stdlib_a_c_sources) \
	$(stdlib_a_asm_sources) \
	$(pmstring_a_c_sources) \
	$(pmstring_a_asm_sources) \
	$(string_a_c_sources) \
	$(string_a_asm_sources) \
	$(misc_a_c_sources) \
	$(misc_a_asm_sources) \
	$(stdio_a_c_sources) \
	$(stdio_a_asm_sources) \
	$(libm_a_c_sources) \
	$(libm_a_asm_sources) \
	$(time_a_c_sources) \
	$(time_a_asm_sources)

nodist_libm_a_SOURCES = \
	$(libm_a_c_sources) \
	$(libm_a_asm_sources)

libc_a_LIBADD = \
	$(stdio_a_libadd) \
	$(misc_a_libadd)

libc_a_DEPENDENCIES = \
	$(stdio_a_libadd) \
	$(misc_a_libadd)

else

echo all distdir install installdirs clean distclean uninstall check:

endif
