//===- lib/ReaderWriter/ELF/Cpu0/Cpu0SectionChunks.cpp --------------===//
//
//                             The LLVM Linker
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

#include "Cpu0SectionChunks.h"
#include "TargetLayout.h"

namespace lld {
namespace elf {

template <class ELFT> Cpu0GOTSection<ELFT>::Cpu0GOTSection(const ELFLinkingContext &ctx)
  : AtomSection<ELFT>(ctx, ".got", DefinedAtom::typeGOT, DefinedAtom::permRW_,
    TargetLayout<ELFT>::ORDER_GOT) {
  this->_alignment = 16;
}

template <class ELFT> const AtomLayout *Cpu0GOTSection<ELFT>::appendAtom(const Atom *atom) {
  return AtomSection<ELFT>::appendAtom(atom);
}

template class Cpu0GOTSection<ELF32BE>;
template class Cpu0GOTSection<ELF32LE>;
} // elf
} // lld
