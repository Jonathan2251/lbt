//===- lib/ReaderWriter/ELF/Cpu0/Cpu0RelocationHandler.h --------------===//
//
//                             The LLVM Linker
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

#ifndef CPU0_RELOCATION_HANDLER_H
#define CPU0_RELOCATION_HANDLER_H

#include "Cpu0TargetHandler.h"

namespace lld {
namespace elf {
typedef llvm::object::ELFType<llvm::support::big, 2, false> Cpu0ELFType;
class Cpu0LinkingContext;

template <class ELFT> class Cpu0TargetLayout;

class Cpu0TargetRelocationHandler final
    : public TargetRelocationHandler<Cpu0ELFType> {
public:
  Cpu0TargetRelocationHandler(Cpu0TargetLayout<Cpu0ELFType> &layout, 
                              Cpu0LinkingContext &context, 
                              ELFLinkingContext &targetInfo)
      : TargetRelocationHandler<Cpu0ELFType>(targetInfo),
        _tlsSize(0), _cpu0Layout(layout), 
        _context(context) {}

  std::error_code applyRelocation(ELFWriter &, llvm::FileOutputBuffer &,
                                  const lld::AtomLayout &,
                                  const Reference &) const override;

  uint64_t tlsSize() { return _tlsSize; } // Just avoid compiler warning

private:
  // Cached size of the TLS segment.
  mutable uint64_t _tlsSize;
  Cpu0TargetLayout<Cpu0ELFType> &_cpu0Layout;
  Cpu0LinkingContext &_context;
};

} // end namespace elf
} // end namespace lld

#endif // Cpu0_RELOCATION_HANDLER_H
