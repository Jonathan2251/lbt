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

SUBDIRS =  at90s1200 attiny11 attiny12 attiny13 attiny15 attiny2313 attiny28 at90s2313 at90s2323 at90s2333 at90s2343 at90s4433 at90s4414 at90s4434 at90s8515 at90s8535 at90c8534 attiny22 attiny26 at86rf401 attiny25 attiny45 attiny85 attiny24 attiny44 attiny84 attiny261 attiny461 attiny861

AVR_TARGET          = avr2
AVR_TARGET_DEFS     = -D__COMPILING_AVR_LIBC__
AVR_TARGET_CFLAGS   = -mcall-prologues -Os
AVR_TARGET_ASFLAGS  = 
AVR_INSTALL_DIR     = 

VPATH = $(top_srcdir)/libc/stdlib:$(top_srcdir)/libc/pmstring:$(top_srcdir)/libc/string:$(top_srcdir)/libc/misc:$(top_srcdir)/libc/stdio:$(top_srcdir)/libm/fplib:$(top_srcdir)/libc/time

if HAS_avr2

AVRLIB_DEVLIST =
if HAS_at90s1200
  AVRLIB_DEVLIST += at90s1200
endif	# at90s1200
if HAS_attiny11
  AVRLIB_DEVLIST += attiny11
endif	# attiny11
if HAS_attiny12
  AVRLIB_DEVLIST += attiny12
endif	# attiny12
if HAS_attiny13
  AVRLIB_DEVLIST += attiny13
endif	# attiny13
if HAS_attiny15
  AVRLIB_DEVLIST += attiny15
endif	# attiny15
if HAS_attiny2313
  AVRLIB_DEVLIST += attiny2313
endif	# attiny2313
if HAS_attiny28
  AVRLIB_DEVLIST += attiny28
endif	# attiny28
if HAS_at90s2313
  AVRLIB_DEVLIST += at90s2313
endif	# at90s2313
if HAS_at90s2323
  AVRLIB_DEVLIST += at90s2323
endif	# at90s2323
if HAS_at90s2333
  AVRLIB_DEVLIST += at90s2333
endif	# at90s2333
if HAS_at90s2343
  AVRLIB_DEVLIST += at90s2343
endif	# at90s2343
if HAS_at90s4433
  AVRLIB_DEVLIST += at90s4433
endif	# at90s4433
if HAS_at90s4414
  AVRLIB_DEVLIST += at90s4414
endif	# at90s4414
if HAS_at90s4434
  AVRLIB_DEVLIST += at90s4434
endif	# at90s4434
if HAS_at90s8515
  AVRLIB_DEVLIST += at90s8515
endif	# at90s8515
if HAS_at90s8535
  AVRLIB_DEVLIST += at90s8535
endif	# at90s8535
if HAS_at90c8534
  AVRLIB_DEVLIST += at90c8534
endif	# at90c8534
if HAS_attiny22
  AVRLIB_DEVLIST += attiny22
endif	# attiny22
if HAS_attiny26
  AVRLIB_DEVLIST += attiny26
endif	# attiny26
if HAS_at86rf401
  AVRLIB_DEVLIST += at86rf401
endif	# at86rf401
if HAS_attiny25
  AVRLIB_DEVLIST += attiny25
endif	# attiny25
if HAS_attiny45
  AVRLIB_DEVLIST += attiny45
endif	# attiny45
if HAS_attiny85
  AVRLIB_DEVLIST += attiny85
endif	# attiny85
if HAS_attiny24
  AVRLIB_DEVLIST += attiny24
endif	# attiny24
if HAS_attiny44
  AVRLIB_DEVLIST += attiny44
endif	# attiny44
if HAS_attiny84
  AVRLIB_DEVLIST += attiny84
endif	# attiny84
if HAS_attiny261
  AVRLIB_DEVLIST += attiny261
endif	# attiny261
if HAS_attiny461
  AVRLIB_DEVLIST += attiny461
endif	# attiny461
if HAS_attiny861
  AVRLIB_DEVLIST += attiny861
endif	# attiny861

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
