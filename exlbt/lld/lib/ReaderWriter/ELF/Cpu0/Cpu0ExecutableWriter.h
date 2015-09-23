//===- lib/ReaderWriter/ELF/Cpu0/Cpu0ExecutableWriter.h ------------------===//
//
//                             The LLVM Linker
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#ifndef CPU0_EXECUTABLE_WRITER_H
#define CPU0_EXECUTABLE_WRITER_H

#include "ExecutableWriter.h"
#include "Cpu0LinkingContext.h"
#include "Cpu0TargetHandler.h"

namespace lld {
namespace elf {

template <class ELFT>
class Cpu0ExecutableWriter : public ExecutableWriter<ELFT> {
public:
  Cpu0ExecutableWriter(Cpu0LinkingContext &ctx, Cpu0TargetLayout<ELFT> &layout);

protected:
  // Add any runtime files and their atoms to the output
  void
  createImplicitFiles(std::vector<std::unique_ptr<File>> &result) override {
    ExecutableWriter<ELFT>::createImplicitFiles(result);
    auto gotFile = llvm::make_unique<SimpleFile>("GOTFile");
    gotFile->addAtom(*new (gotFile->allocator())
                         GlobalOffsetTableAtom(*gotFile));
    if (this->_ctx.isDynamic())
      gotFile->addAtom(*new (gotFile->allocator()) DynamicAtom(*gotFile));
    result.push_back(std::move(gotFile));
  }

  void buildDynamicSymbolTable(const File &file) override {
    for (auto sec : this->_layout.sections()) {
      if (auto section = dyn_cast<AtomSection<ELFT>>(sec)) {
        for (const auto &atom : section->atoms()) {
          if (_targetLayout.getGOTSection().hasGlobalGOTEntry(atom->_atom)) {
            this->_dynamicSymbolTable->addSymbol(atom->_atom, section->ordinal(),
                                                 atom->_virtualAddr, atom);
          }
        }
      }
    }

    ExecutableWriter<ELFT>::buildDynamicSymbolTable(file);
  }

  Cpu0TargetLayout<ELFT> &_targetLayout;
};

} // namespace elf
} // namespace lld

#endif
