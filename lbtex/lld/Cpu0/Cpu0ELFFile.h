//===- lib/ReaderWriter/ELF/Cpu0/Cpu0ELFFile.h ------------------------===//
//
//                             The LLVM Linker
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

#ifndef LLD_READER_WRITER_ELF_CPU0_ELF_FILE_H
#define LLD_READER_WRITER_ELF_CPU0_ELF_FILE_H

#include "ELFReader.h"

namespace lld {
namespace elf {

class Cpu0LinkingContext;

template <class ELFT> class Cpu0ELFFile : public ELFFile<ELFT> {
public:
  Cpu0ELFFile(std::unique_ptr<MemoryBuffer> mb, bool atomizeStrings)
      : ELFFile<ELFT>(std::move(mb), atomizeStrings) {}

  static ErrorOr<std::unique_ptr<Cpu0ELFFile>>
  create(std::unique_ptr<MemoryBuffer> mb, bool atomizeStrings) {
    return std::unique_ptr<Cpu0ELFFile<ELFT>>(
        new Cpu0ELFFile<ELFT>(std::move(mb), atomizeStrings));
  }
};

template <class ELFT> class Cpu0DynamicFile : public DynamicFile<ELFT> {
public:
  Cpu0DynamicFile(const Cpu0LinkingContext &context, StringRef name)
      : DynamicFile<ELFT>(context, name) {}
};

} // elf
} // lld

#endif // LLD_READER_WRITER_ELF_CPU0_ELF_FILE_H
