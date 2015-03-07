//===- lib/ReaderWriter/ELF/Cpu0/Cpu0LinkingContext.cpp ---------------===//
//
//                             The LLVM Linker
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

#include "Cpu0LinkingContext.h"
#include "Cpu0RelocationPass.h"

using namespace lld;

void elf::Cpu0LinkingContext::addPasses(PassManager &pm) {
  auto pass = createCpu0RelocationPass(*this);
  if (pass)
    pm.add(std::move(pass));
  ELFLinkingContext::addPasses(pm);
}

