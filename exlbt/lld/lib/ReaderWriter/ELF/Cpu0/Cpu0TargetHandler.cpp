//===- lib/ReaderWriter/ELF/Cpu0/Cpu0TargetHandler.cpp ----------------===//
//
//                             The LLVM Linker
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

#include "Atoms.h"
#include "Cpu0DynamicLibraryWriter.h"
#include "Cpu0ExecutableWriter.h"
#include "Cpu0LinkingContext.h"
#include "Cpu0TargetHandler.h"

endianness gEndian = native;

namespace lld {
namespace elf {

#ifdef DLINKER
template <class ELFT>
void Cpu0SoPlt<ELFT>::createFunAddr(Cpu0TargetLayout<ELFT> &cpu0Layout,
                              llvm::FileOutputBuffer &buf) {
  auto dynsymSection = cpu0Layout.
                       findOutputSection(".dynsym");
  uint64_t dynsymFileOffset, dynsymSize;
  if (dynsymSection) {
    dynsymFileOffset = dynsymSection->fileOffset();
    dynsymSize = dynsymSection->memSize();
    uint8_t *atomContent = buf.getBufferStart() + dynsymFileOffset;
    for (uint64_t i = 4; i < dynsymSize; i += 16) {
      funAddr[funAddrSize] = 
        *reinterpret_cast<llvm::support::ubig32_t*>((uint32_t*)
        (atomContent + i));
      funAddrSize++;
    }
  }
  return;
}

template <class ELFT>
int Cpu0SoPlt<ELFT>::getDynFunIndexByTargetAddr(uint64_t fAddr) {
  for (int i = 0; i < funAddrSize; i++) {
    // Below statement fix the issue that both __tls_get_addr and first 
    // function has the same file offset 0 issue.
    if (i < (funAddrSize-1) && funAddr[i] == funAddr[i+1])
      continue;

    if (fAddr == funAddr[i]) {
      return i;
    }
  }
  return -1;
}

template class Cpu0SoPlt<ELF32BE>;
template class Cpu0SoPlt<ELF32LE>;
#endif // DLINKER

template <class ELFT>
Cpu0TargetHandler<ELFT>::Cpu0TargetHandler(Cpu0LinkingContext &ctx, 
                                           endianness endian)
    : _ctx(ctx), _targetLayout(new Cpu0TargetLayout<ELFT>(ctx)),
      _relocationHandler(new Cpu0TargetRelocationHandler<ELFT>
                         (ctx, *_targetLayout, *this, endian)) {
  gEndian = endian;
}

template <class ELFT>
std::unique_ptr<Writer> Cpu0TargetHandler<ELFT>::getWriter() {
  switch (this->_ctx.getOutputELFType()) {
  case llvm::ELF::ET_EXEC:
    return llvm::make_unique<Cpu0ExecutableWriter<ELFT>>(_ctx, *_targetLayout);
  case llvm::ELF::ET_DYN:
    return llvm::make_unique<Cpu0DynamicLibraryWriter<ELFT>>(_ctx, *_targetLayout);
  case llvm::ELF::ET_REL:
    llvm_unreachable("TODO: support -r mode");
  default:
    llvm_unreachable("unsupported output type");
  }
}

template class Cpu0TargetHandler<ELF32BE>;
template class Cpu0TargetHandler<ELF32LE>;

}
}
