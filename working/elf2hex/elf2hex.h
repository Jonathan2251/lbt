//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_TOOLS_ELF2HEX_ELF2HEX_H
#define LLVM_TOOLS_ELF2HEX_ELF2HEX_H

#include "llvm/ADT/StringRef.h"
#include "llvm/Support/CommandLine.h"
#include "llvm/Support/DataTypes.h"

#include <stdio.h>
#include "llvm/Support/raw_ostream.h"

#define BOOT_SIZE 16

#define DLINK
//#define ELF2HEX_DEBUG

namespace llvm {
namespace object {
  class COFFObjectFile;
  class MachOObjectFile;
  class ObjectFile;
  class RelocationRef;
}

extern cl::opt<std::string> TripleName;
extern cl::opt<std::string> ArchName;
extern cl::list<std::string> MAttrs;
extern cl::opt<bool> NoShowRawInsn;

// Various helper functions.
bool error(std::error_code ec);
bool RelocAddressLess(object::RelocationRef a, object::RelocationRef b);

} // end namespace llvm

#endif
