//===- lib/ReaderWriter/ELF/Cpu0/Cpu0TargetHandler.h ------------------===//
//
//                             The LLVM Linker
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

#ifndef LLD_READER_WRITER_ELF_CPU0EL_CPU0EL_TARGET_HANDLER_H
#define LLD_READER_WRITER_ELF_CPU0EL_CPU0EL_TARGET_HANDLER_H

#include "Cpu0ELFFile.h"
#include "Cpu0ELFReader.h"
#include "Cpu0RelocationHandler.h"
#include "DefaultTargetHandler.h"
#include "TargetLayout.h"

#include "lld/Core/Simple.h"

#define DLINKER

namespace lld {
namespace elf {
typedef llvm::object::ELFType<llvm::support::little, 2, false> Cpu0elELFType;
class Cpu0elLinkingContext;

template <class ELFT> class Cpu0elTargetLayout : public TargetLayout<ELFT> {
public:
  Cpu0elTargetLayout(Cpu0elLinkingContext &context)
      : TargetLayout<ELFT>(context) {}
};

class Cpu0elTargetHandler final
    : public DefaultTargetHandler<Cpu0elELFType> {
public:
  Cpu0elTargetHandler(Cpu0elLinkingContext &context);

  Cpu0elTargetLayout<Cpu0elELFType> &getTargetLayout() override {
    return *(_cpu0TargetLayout.get());
  }

  void registerRelocationNames(Registry &registry) override;

  const Cpu0elTargetRelocationHandler &getRelocationHandler() const override {
    return *(_cpu0RelocationHandler.get());
  }

  std::unique_ptr<Reader> getObjReader(bool atomizeStrings) override {
    return std::unique_ptr<Reader>(new Cpu0elELFObjectReader(atomizeStrings));
  }

  std::unique_ptr<Reader> getDSOReader(bool useShlibUndefines) override {
    return std::unique_ptr<Reader>(new Cpu0elELFDSOReader(useShlibUndefines));
  }

  std::unique_ptr<Writer> getWriter() override;

private:
  static const Registry::KindStrings kindStrings[];
  Cpu0elLinkingContext &_context;
  std::unique_ptr<Cpu0elTargetLayout<Cpu0elELFType>> _cpu0TargetLayout;
  std::unique_ptr<Cpu0elTargetRelocationHandler> _cpu0RelocationHandler;
};

} // end namespace elf
} // end namespace lld

#endif
