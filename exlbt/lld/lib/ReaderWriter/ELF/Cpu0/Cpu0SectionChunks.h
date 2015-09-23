//===- lib/ReaderWriter/ELF/Cpu0/Cpu0SectionChunks.h ----------------===//
//
//                             The LLVM Linker
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

#ifndef LLD_READER_WRITER_ELF_CPU0_CPU0_SECTION_CHUNKS_H
#define LLD_READER_WRITER_ELF_CPU0_CPU0_SECTION_CHUNKS_H

#include "TargetLayout.h"

namespace lld {
namespace elf {

template <class ELFT> class Cpu0GOTSection : public AtomSection<ELFT> {
public:
  Cpu0GOTSection(const ELFLinkingContext &ctx);

  bool hasGlobalGOTEntry(const Atom *a) const {
    return _tlsMap.count(a);
  }

  const AtomLayout *appendAtom(const Atom *atom) override;

private:
  /// \brief Map TLS Atoms to their GOT entry index.
  llvm::DenseMap<const Atom *, std::size_t> _tlsMap;
};

} // elf
} // lld

#endif
