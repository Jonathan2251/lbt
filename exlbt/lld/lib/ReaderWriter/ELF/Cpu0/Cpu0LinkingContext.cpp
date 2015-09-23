//===- lib/ReaderWriter/ELF/Cpu0/Cpu0LinkingContext.cpp ---------------===//
//
//                             The LLVM Linker
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

#include "llvm/Support/Endian.h"
#include "Cpu0LinkingContext.h"
#include "Cpu0RelocationPass.h"
#include "Cpu0TargetHandler.h"

using namespace lld;
using namespace lld::elf;
using namespace llvm::support;

Cpu0LinkingContext::Cpu0LinkingContext(
    llvm::Triple triple, std::unique_ptr<TargetHandler> handler)
    : ELFLinkingContext(triple, std::move(handler)) {}

static std::unique_ptr<TargetHandler> createTarget(llvm::Triple triple,
                                                   Cpu0LinkingContext &ctx) {
  switch (triple.getArch()) {
  case llvm::Triple::cpu0:
    return llvm::make_unique<Cpu0TargetHandler<ELF32BE>>(ctx, big);
  case llvm::Triple::cpu0el:
    return llvm::make_unique<Cpu0TargetHandler<ELF32LE>>(ctx, little);
  default:
    llvm_unreachable("Unhandled arch");
  }
}

Cpu0LinkingContext::Cpu0LinkingContext(llvm::Triple triple)
    : Cpu0LinkingContext(triple, createTarget(triple, *this)) {}

void Cpu0LinkingContext::addPasses(PassManager &pm) {
  auto pass = createCpu0RelocationPass(*this);
  if (pass)
    pm.add(std::move(pass));
  ELFLinkingContext::addPasses(pm);
}

std::unique_ptr<ELFLinkingContext>
elf::createCpu0LinkingContext(llvm::Triple triple) {
  switch (triple.getArch()) {
  case llvm::Triple::cpu0:
    return llvm::make_unique<Cpu0LinkingContext>(triple);
  case llvm::Triple::cpu0el:
    return llvm::make_unique<Cpu0LinkingContext>(triple);
  }
  return nullptr;
}

static const Registry::KindStrings kindStrings[] = {
#define ELF_RELOC(name, value) LLD_KIND_STRING_ENTRY(name),
#include "llvm/Support/ELFRelocs/Cpu0.def"
#undef ELF_RELOC
  LLD_KIND_STRING_ENTRY(LLD_R_CPU0_GOTRELINDEX),
  LLD_KIND_STRING_END
};

void Cpu0LinkingContext::registerRelocationNames(Registry &registry) {
  registry.addKindTable(Reference::KindNamespace::ELF,
                        Reference::KindArch::Cpu0, kindStrings);
}
