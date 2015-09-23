//===- lib/ReaderWriter/ELF/Cpu0/Cpu0RelocationPass.h -----------------===//
//
//                             The LLVM Linker
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
///
/// \file
/// \brief Declares the relocation processing pass for x86-64. This includes
///   GOT and PLT entries, TLS, COPY, and ifunc.
///
//===----------------------------------------------------------------------===//

#ifndef LLD_READER_WRITER_ELF_CPU0_CPU0_RELOCATION_PASS_H
#define LLD_READER_WRITER_ELF_CPU0_CPU0_RELOCATION_PASS_H

#include <memory>
#include "Cpu0TargetHandler.h"

namespace lld {
class Pass;
namespace elf {
class Cpu0LinkingContext;

/// \brief Create x86-64 relocation pass for the given linking context.
std::unique_ptr<Pass>
createCpu0RelocationPass(const Cpu0LinkingContext &);
}
}

#endif
