//===- lib/ReaderWriter/ELF/Cpu0/Cpu0DynamicLibraryWriter.h ---------------===//
//
//                             The LLVM Linker
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#ifndef Cpu0_DYNAMIC_LIBRARY_WRITER_H
#define Cpu0_DYNAMIC_LIBRARY_WRITER_H

#include "DynamicLibraryWriter.h"
#include "Cpu0LinkingContext.h"

namespace lld {
namespace elf {

template <class ELFT>
class Cpu0DynamicLibraryWriter : public DynamicLibraryWriter<ELFT> {
public:
  Cpu0DynamicLibraryWriter(Cpu0LinkingContext &context,
                             Cpu0TargetLayout<ELFT> &layout);

protected:
  // Add any runtime files and their atoms to the output
  virtual bool createImplicitFiles(std::vector<std::unique_ptr<File>> &);

  virtual void finalizeDefaultAtomValues() {
    return DynamicLibraryWriter<ELFT>::finalizeDefaultAtomValues();
  }

  virtual void addDefaultAtoms() {
    return DynamicLibraryWriter<ELFT>::addDefaultAtoms();
  }

private:
  class GOTFile : public SimpleFile {
  public:
    GOTFile(const ELFLinkingContext &eti) : SimpleFile("GOTFile") {}
    llvm::BumpPtrAllocator _alloc;
  };

  std::unique_ptr<GOTFile> _gotFile;
  Cpu0LinkingContext &_context;
  Cpu0TargetLayout<ELFT> &_cpu0Layout;
};

template <class ELFT>
Cpu0DynamicLibraryWriter<ELFT>::Cpu0DynamicLibraryWriter(
    Cpu0LinkingContext &context, Cpu0TargetLayout<ELFT> &layout)
    : DynamicLibraryWriter<ELFT>(context, layout),
      _gotFile(new GOTFile(context)), _context(context), _cpu0Layout(layout) {
}

template <class ELFT>
bool Cpu0DynamicLibraryWriter<ELFT>::createImplicitFiles(
    std::vector<std::unique_ptr<File>> &result) {
  DynamicLibraryWriter<ELFT>::createImplicitFiles(result);
  _gotFile->addAtom(*new (_gotFile->_alloc) GLOBAL_OFFSET_TABLEAtom(*_gotFile));
  _gotFile->addAtom(*new (_gotFile->_alloc) DYNAMICAtom(*_gotFile));
  result.push_back(std::move(_gotFile));
  return true;
}

} // namespace elf
} // namespace lld

#endif
