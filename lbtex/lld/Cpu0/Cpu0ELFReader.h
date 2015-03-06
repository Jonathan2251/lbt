//===- lib/ReaderWriter/ELF/CPU0/CPU0ELFReader.h ----------------------===//
//
//                             The LLVM Linker
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

#ifndef LLD_READER_WRITER_CPU0_CPU0_ELF_READER_H
#define LLD_READER_WRITER_CPU0_CPU0_ELF_READER_H

#include "ELFReader.h"
#include "Cpu0ELFFile.h"

namespace lld {
namespace elf {

typedef llvm::object::ELFType<llvm::support::big, 2, false> Cpu0ELFType;

struct Cpu0DynamicFileCreateELFTraits {
  typedef llvm::ErrorOr<std::unique_ptr<lld::SharedLibraryFile>> result_type;

  template <class ELFT>
  static result_type create(std::unique_ptr<llvm::MemoryBuffer> mb,
                            bool useUndefines) {
    return lld::elf::Cpu0DynamicFile<ELFT>::create(std::move(mb),
                                                     useUndefines);
  }
};

struct Cpu0ELFFileCreateELFTraits {
  typedef llvm::ErrorOr<std::unique_ptr<lld::File>> result_type;

  template <class ELFT>
  static result_type create(std::unique_ptr<llvm::MemoryBuffer> mb,
                            bool atomizeStrings) {
    return lld::elf::Cpu0ELFFile<ELFT>::create(std::move(mb), atomizeStrings);
  }
};

class Cpu0ELFObjectReader
    : public ELFObjectReader<Cpu0ELFType, Cpu0ELFFileCreateELFTraits> {
public:
  Cpu0ELFObjectReader(bool atomizeStrings)
      : ELFObjectReader<Cpu0ELFType, Cpu0ELFFileCreateELFTraits>(
            atomizeStrings, llvm::ELF::EM_CPU0) {}
};

class Cpu0ELFDSOReader
    : public ELFDSOReader<Cpu0ELFType, Cpu0DynamicFileCreateELFTraits> {
public:
  Cpu0ELFDSOReader(bool useUndefines)
      : ELFDSOReader<Cpu0ELFType, Cpu0DynamicFileCreateELFTraits>(
            useUndefines, llvm::ELF::EM_CPU0) {}
};

} // namespace elf
} // namespace lld

#endif // LLD_READER_WRITER_ELF_CPU0_CPU0_READER_H
