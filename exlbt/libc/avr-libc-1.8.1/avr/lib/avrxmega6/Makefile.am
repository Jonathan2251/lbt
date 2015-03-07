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

SUBDIRS =  atxmega128a3 atxmega128a3u atxmega128b1 atxmega128b3 atxmega128c3 atxmega128d3 atxmega128d4 atxmega192a3 atxmega192a3u atxmega192c3 atxmega192d3 atxmega256a3 atxmega256a3u atxmega256a3b atxmega256a3bu atxmega256c3 atxmega256d3 atxmega384c3 atxmega384d3

AVR_TARGET          = avrxmega6
AVR_TARGET_DEFS     = -D__COMPILING_AVR_LIBC__
AVR_TARGET_CFLAGS   = -Os $(FNO_JUMP_TABLES)
AVR_TARGET_ASFLAGS  = 
AVR_INSTALL_DIR     = avrxmega6

VPATH = $(top_srcdir)/libc/stdlib:$(top_srcdir)/libc/pmstring:$(top_srcdir)/libc/string:$(top_srcdir)/libc/misc:$(top_srcdir)/libc/stdio:$(top_srcdir)/libm/fplib:$(top_srcdir)/libc/time

if HAS_avrxmega6

AVRLIB_DEVLIST =
if HAS_atxmega128a3
  AVRLIB_DEVLIST += atxmega128a3
endif	# atxmega128a3
if HAS_atxmega128a3u
  AVRLIB_DEVLIST += atxmega128a3u
endif	# atxmega128a3u
if HAS_atxmega128b1
  AVRLIB_DEVLIST += atxmega128b1
endif	# atxmega128b1
if HAS_atxmega128b3
  AVRLIB_DEVLIST += atxmega128b3
endif	# atxmega128b3
if HAS_atxmega128c3
  AVRLIB_DEVLIST += atxmega128c3
endif	# atxmega128c3
if HAS_atxmega128d3
  AVRLIB_DEVLIST += atxmega128d3
endif	# atxmega128d3
if HAS_atxmega128d4
  AVRLIB_DEVLIST += atxmega128d4
endif	# atxmega128d4
if HAS_atxmega192a3
  AVRLIB_DEVLIST += atxmega192a3
endif	# atxmega192a3
if HAS_atxmega192a3u
  AVRLIB_DEVLIST += atxmega192a3u
endif	# atxmega192a3u
if HAS_atxmega192c3
  AVRLIB_DEVLIST += atxmega192c3
endif	# atxmega192c3
if HAS_atxmega192d3
  AVRLIB_DEVLIST += atxmega192d3
endif	# atxmega192d3
if HAS_atxmega256a3
  AVRLIB_DEVLIST += atxmega256a3
endif	# atxmega256a3
if HAS_atxmega256a3u
  AVRLIB_DEVLIST += atxmega256a3u
endif	# atxmega256a3u
if HAS_atxmega256a3b
  AVRLIB_DEVLIST += atxmega256a3b
endif	# atxmega256a3b
if HAS_atxmega256a3bu
  AVRLIB_DEVLIST += atxmega256a3bu
endif	# atxmega256a3bu
if HAS_atxmega256c3
  AVRLIB_DEVLIST += atxmega256c3
endif	# atxmega256c3
if HAS_atxmega256d3
  AVRLIB_DEVLIST += atxmega256d3
endif	# atxmega256d3
if HAS_atxmega384c3
  AVRLIB_DEVLIST += atxmega384c3
endif	# atxmega384c3
if HAS_atxmega384d3
  AVRLIB_DEVLIST += atxmega384d3
endif	# atxmega384d3

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
