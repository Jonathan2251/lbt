//===- lib/ReaderWriter/ELF/CPU0/CPU0ELFReader.h ----------------------===//
//
//                             The LLVM Linker
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

#ifndef LLD_READER_WRITER_CPU0EL_CPU0EL_ELF_READER_H
#define LLD_READER_WRITER_CPU0EL_CPU0EL_ELF_READER_H

#include "ELFReader.h"
#include "Cpu0ELFFile.h"

namespace lld {
namespace elf {

typedef llvm::object::ELFType<llvm::support::little, 2, false> Cpu0elELFType;

struct Cpu0elDynamicFileCreateELFTraits {
  typedef llvm::ErrorOr<std::unique_ptr<lld::SharedLibraryFile>> result_type;

  template <class ELFT>
  static result_type create(std::unique_ptr<llvm::MemoryBuffer> mb,
                            bool useUndefines) {
    return lld::elf::Cpu0elDynamicFile<ELFT>::create(std::move(mb),
                                                     useUndefines);
  }
};

struct Cpu0elELFFileCreateELFTraits {
  typedef llvm::ErrorOr<std::unique_ptr<lld::File>> result_type;

  template <class ELFT>
  static result_type create(std::unique_ptr<llvm::MemoryBuffer> mb,
                            bool atomizeStrings) {
    return lld::elf::Cpu0elELFFile<ELFT>::create(std::move(mb), atomizeStrings);
  }
};

class Cpu0elELFObjectReader
    : public ELFObjectReader<Cpu0elELFType, Cpu0elELFFileCreateELFTraits> {
public:
  Cpu0elELFObjectReader(bool atomizeStrings)
      : ELFObjectReader<Cpu0elELFType, Cpu0elELFFileCreateELFTraits>(
            atomizeStrings, llvm::ELF::EM_CPU0) {}
};

class Cpu0elELFDSOReader
    : public ELFDSOReader<Cpu0elELFType, Cpu0elDynamicFileCreateELFTraits> {
public:
  Cpu0elELFDSOReader(bool useUndefines)
      : ELFDSOReader<Cpu0elELFType, Cpu0elDynamicFileCreateELFTraits>(
            useUndefines, llvm::ELF::EM_CPU0) {}
};

} // namespace elf
} // namespace lld

#endif // LLD_READER_WRITER_ELF_CPU0EL_CPU0EL_READER_H
