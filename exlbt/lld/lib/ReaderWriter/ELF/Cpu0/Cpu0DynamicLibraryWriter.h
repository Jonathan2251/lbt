//===- lib/ReaderWriter/ELF/Cpu0/Cpu0DynamicLibraryWriter.h ---------------===//
//
//                             The LLVM Linker
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#ifndef CPU0_DYNAMIC_LIBRARY_WRITER_H
#define CPU0_DYNAMIC_LIBRARY_WRITER_H

#include "DynamicLibraryWriter.h"
#include "Cpu0LinkingContext.h"
#include "Cpu0TargetHandler.h"

namespace lld {
namespace elf {

template <class ELFT>
class Cpu0DynamicLibraryWriter : public DynamicLibraryWriter<ELFT> {
public:
  Cpu0DynamicLibraryWriter(Cpu0LinkingContext &ctx,
                             Cpu0TargetLayout<ELFT> &layout);

protected:
  // Add any runtime files and their atoms to the output
  void createImplicitFiles(std::vector<std::unique_ptr<File>> &) override;
};

} // namespace elf
} // namespace lld

using namespace lld::elf;

template <class ELFT> Cpu0DynamicLibraryWriter<ELFT>::Cpu0DynamicLibraryWriter(
    Cpu0LinkingContext &ctx, Cpu0TargetLayout<ELFT> &layout)
    : DynamicLibraryWriter<ELFT>(ctx, layout) {}

template <class ELFT> void Cpu0DynamicLibraryWriter<ELFT>::createImplicitFiles(
    std::vector<std::unique_ptr<File>> &result) {
  DynamicLibraryWriter<ELFT>::createImplicitFiles(result);
  auto gotFile = llvm::make_unique<SimpleFile>("GOTFile");
  gotFile->addAtom(*new (gotFile->allocator()) GlobalOffsetTableAtom(*gotFile));
  gotFile->addAtom(*new (gotFile->allocator()) DynamicAtom(*gotFile));
  result.push_back(std::move(gotFile));
}

#endif
