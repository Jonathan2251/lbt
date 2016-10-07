//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_TOOLS_ELF2HEX_ELF2HEX_H
#define LLVM_TOOLS_ELF2HEX_ELF2HEX_H

#include "llvm/DebugInfo/DIContext.h"
#include "llvm/Support/CommandLine.h"
#include "llvm/Support/Compiler.h"
#include "llvm/Support/DataTypes.h"
#include "llvm/Object/Archive.h"

#include <stdio.h>
#include "llvm/Support/raw_ostream.h"

#define BOOT_SIZE 16

#define DLINK
//#define ELF2HEX_DEBUG

namespace llvm {
class StringRef;

namespace object {
  class COFFObjectFile;
  class MachOObjectFile;
  class ObjectFile;
  class Archive;
  class RelocationRef;
}

extern cl::opt<std::string> TripleName;
extern cl::opt<std::string> ArchName;
extern cl::list<std::string> MAttrs;
extern cl::list<std::string> FilterSections;
extern cl::opt<bool> NoShowRawInsn;

// Various helper functions.
void error(std::error_code EC);
bool RelocAddressLess(object::RelocationRef a, object::RelocationRef b);
LLVM_ATTRIBUTE_NORETURN void error(Twine Message);
LLVM_ATTRIBUTE_NORETURN void report_error(StringRef File, std::error_code EC);
LLVM_ATTRIBUTE_NORETURN void report_error(StringRef File, llvm::Error E);
LLVM_ATTRIBUTE_NORETURN void report_error(StringRef FileName,
                                          StringRef ArchiveName,
                                          llvm::Error E,
                                          StringRef ArchitectureName
                                                    = StringRef());
LLVM_ATTRIBUTE_NORETURN void report_error(StringRef ArchiveName,
                                          const object::Archive::Child &C,
                                          llvm::Error E,
                                          StringRef ArchitectureName
                                                    = StringRef());
} // end namespace llvm

#endif
