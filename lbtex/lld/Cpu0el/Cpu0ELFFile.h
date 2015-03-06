//===- lib/ReaderWriter/ELF/Cpu0/Cpu0ELFFile.h ------------------------===//
//
//                             The LLVM Linker
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

#ifndef LLD_READER_WRITER_ELF_CPU0EL_ELF_FILE_H
#define LLD_READER_WRITER_ELF_CPU0EL_ELF_FILE_H

#include "ELFReader.h"

namespace lld {
namespace elf {

class Cpu0elLinkingContext;

template <class ELFT> class Cpu0elELFFile : public ELFFile<ELFT> {
public:
  Cpu0elELFFile(std::unique_ptr<MemoryBuffer> mb, bool atomizeStrings)
      : ELFFile<ELFT>(std::move(mb), atomizeStrings) {}

  static ErrorOr<std::unique_ptr<Cpu0elELFFile>>
  create(std::unique_ptr<MemoryBuffer> mb, bool atomizeStrings) {
    return std::unique_ptr<Cpu0elELFFile<ELFT>>(
        new Cpu0elELFFile<ELFT>(std::move(mb), atomizeStrings));
  }
};

template <class ELFT> class Cpu0elDynamicFile : public DynamicFile<ELFT> {
public:
  Cpu0elDynamicFile(const Cpu0elLinkingContext &context, StringRef name)
      : DynamicFile<ELFT>(context, name) {}
};

} // elf
} // lld

#endif // LLD_READER_WRITER_ELF_CPU0EL_ELF_FILE_H
