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

using namespace lld;
using namespace elf;

Cpu0elTargetHandler::Cpu0elTargetHandler(Cpu0elLinkingContext &context)
    : DefaultTargetHandler(context), _context(context),
      _cpu0TargetLayout(new Cpu0elTargetLayout<Cpu0elELFType>(context)),
      _cpu0RelocationHandler(
          new Cpu0elTargetRelocationHandler(*_cpu0TargetLayout.get(), context, 
                                            context)) {}

void Cpu0elTargetHandler::registerRelocationNames(Registry &registry) {
  registry.addKindTable(Reference::KindNamespace::ELF,
                        Reference::KindArch::Cpu0, kindStrings);
}

std::unique_ptr<Writer> Cpu0elTargetHandler::getWriter() {
  switch (this->_context.getOutputELFType()) {
  case llvm::ELF::ET_EXEC:
    return std::unique_ptr<Writer>(new Cpu0ExecutableWriter<Cpu0elELFType>(
        _context, *_cpu0TargetLayout.get()));
  case llvm::ELF::ET_DYN:
    return std::unique_ptr<Writer>(
        new Cpu0DynamicLibraryWriter<Cpu0elELFType>(
            _context, *_cpu0TargetLayout.get()));
  case llvm::ELF::ET_REL:
    llvm_unreachable("TODO: support -r mode");
  default:
    llvm_unreachable("unsupported output type");
  }
}

#define ELF_RELOC(name, value) LLD_KIND_STRING_ENTRY(name),

const Registry::KindStrings Cpu0elTargetHandler::kindStrings[] = {
#include "llvm/Support/ELFRelocs/Cpu0.def"
  LLD_KIND_STRING_ENTRY(LLD_R_CPU0EL_GOTRELINDEX),
  LLD_KIND_STRING_END
};

#undef ELF_RELOC

