//===- lib/ReaderWriter/ELF/Cpu0/Cpu0LinkingContext.h -----------------===//
//
//                             The LLVM Linker
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

#ifndef LLD_READER_WRITER_ELF_CPU0EL_CPU0EL_LINKING_CONTEXT_H
#define LLD_READER_WRITER_ELF_CPU0EL_CPU0EL_LINKING_CONTEXT_H

#include "Cpu0TargetHandler.h"

#include "lld/ReaderWriter/ELFLinkingContext.h"

#include "llvm/Object/ELF.h"
#include "llvm/Support/ELF.h"

namespace lld {
namespace elf {

/// \brief Cpu0 internal references.
enum {
  /// \brief The 32 bit index of the relocation in the got this reference refers
  /// to.
  LLD_R_CPU0EL_GOTRELINDEX = 1025,
};

class Cpu0elLinkingContext final : public ELFLinkingContext {
public:
  Cpu0elLinkingContext(llvm::Triple triple)
      : ELFLinkingContext(triple, std::unique_ptr<TargetHandlerBase>(
                                      new Cpu0elTargetHandler(*this))) {}

//  bool isLittleEndian() const override { return false; }

  void addPasses(PassManager &) override;

  uint64_t getBaseAddress() const override {
    if (_baseAddress == 0)
      return 0x000000;
    return _baseAddress;
  }

  bool isDynamicRelocation(const DefinedAtom &,
                           const Reference &r) const override {
    if (r.kindNamespace() != Reference::KindNamespace::ELF)
      return false;
    assert(r.kindArch() == Reference::KindArch::Cpu0);
    switch (r.kindValue()) {
    case llvm::ELF::R_CPU0_GLOB_DAT:
      return true;
    default:
      return false;
    }
  }

  virtual bool isPLTRelocation(const DefinedAtom &,
                               const Reference &r) const override {
    if (r.kindNamespace() != Reference::KindNamespace::ELF)
      return false;
    assert(r.kindArch() == Reference::KindArch::Cpu0);
    switch (r.kindValue()) {
    case llvm::ELF::R_CPU0_JUMP_SLOT:
    case llvm::ELF::R_CPU0_RELGOT:
      return true;
    default:
      return false;
    }
  }

  /// \brief Cpu0 has two relative relocations
  /// a) for supporting relative relocs - R_CPU0_RELGOT
  bool isRelativeReloc(const Reference &r) const override {
    if (r.kindNamespace() != Reference::KindNamespace::ELF)
      return false;
    assert(r.kindArch() == Reference::KindArch::Cpu0);
    switch (r.kindValue()) {
    case llvm::ELF::R_CPU0_RELGOT:
      return true;
    default:
      return false;
    }
  }

  bool isStaticExecutable() const { return _isStaticExecutable; }
};
} // end namespace elf
} // end namespace lld

#endif
