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
#include "lld/ReaderWriter/ELFLinkingContext.h"
#include "llvm/Support/Endian.h"

using namespace llvm::support;

namespace lld {
namespace elf {
template<typename ELFT> class Cpu0TargetLayout;
template<typename ELFT> class Cpu0TargetHandler;

template <class ELFT>
class Cpu0TargetRelocationHandler final : public TargetRelocationHandler {
public:
  Cpu0TargetRelocationHandler(Cpu0LinkingContext &ctx, 
                              Cpu0TargetLayout<ELFT> &layout, 
                              Cpu0TargetHandler<ELFT> &targetHandler,
                              endianness endian)
      : _tlsSize(0), _ctx(ctx), _layout(layout), _targetHandler(targetHandler),
        _endian(endian)
  {}

  std::error_code applyRelocation(ELFWriter &, llvm::FileOutputBuffer &,
                                  const AtomLayout &,
                                  const Reference &) const override;

  endianness endian() const { return _endian; };

private:
  // Cached size of the TLS segment.
  mutable uint64_t _tlsSize;
  Cpu0LinkingContext &_ctx;
  Cpu0TargetLayout<ELFT> &_layout;
  Cpu0TargetHandler<ELFT> &_targetHandler;
  endianness _endian;
};

} // end namespace elf
} // end namespace lld

#endif // Cpu0_RELOCATION_HANDLER_H
