//===- lib/ReaderWriter/ELF/Cpu0/Cpu0RelocationHandler.h --------------===//
//
//                             The LLVM Linker
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

#ifndef CPU0EL_RELOCATION_HANDLER_H
#define CPU0EL_RELOCATION_HANDLER_H

#include "Cpu0TargetHandler.h"

namespace lld {
namespace elf {
typedef llvm::object::ELFType<llvm::support::little, 2, false> Cpu0elELFType;
class Cpu0elLinkingContext;

template <class ELFT> class Cpu0elTargetLayout;

class Cpu0elTargetRelocationHandler final
    : public TargetRelocationHandler<Cpu0elELFType> {
public:
  Cpu0elTargetRelocationHandler(Cpu0elTargetLayout<Cpu0elELFType> &layout, 
                              Cpu0elLinkingContext &context, 
                              ELFLinkingContext &targetInfo)
      : TargetRelocationHandler<Cpu0elELFType>(targetInfo),
        _tlsSize(0), _cpu0Layout(layout), 
        _context(context) {}

  std::error_code applyRelocation(ELFWriter &, llvm::FileOutputBuffer &,
                                  const lld::AtomLayout &,
                                  const Reference &) const override;

private:
  // Cached size of the TLS segment.
  mutable uint64_t _tlsSize;
  Cpu0elTargetLayout<Cpu0elELFType> &_cpu0Layout;
  Cpu0elLinkingContext &_context;
};

} // end namespace elf
} // end namespace lld

#endif // CPU0EL_RELOCATION_HANDLER_H
