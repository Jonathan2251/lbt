//===-- llvm-objdump.cpp - Object file dumping utility for llvm -----------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This program is a utility that works like binutils "objdump", that is, it
// dumps out a plethora of information about an object file depending on the
// flags.
//
// The flags and output of this program should be near identical to those of
// binutils objdump.
//
//===----------------------------------------------------------------------===//

#include "elf2hex.h"
#include "llvm/ADT/Optional.h"
#include "llvm/ADT/STLExtras.h"
#include "llvm/ADT/StringExtras.h"
#include "llvm/ADT/Triple.h"
#include "llvm/CodeGen/FaultMaps.h"
#include "llvm/DebugInfo/DWARF/DWARFContext.h"
#include "llvm/MC/MCAsmInfo.h"
#include "llvm/MC/MCContext.h"
#include "llvm/MC/MCDisassembler/MCDisassembler.h"
#include "llvm/MC/MCInst.h"
#include "llvm/MC/MCInstPrinter.h"
#include "llvm/MC/MCInstrAnalysis.h"
#include "llvm/MC/MCInstrInfo.h"
#include "llvm/MC/MCObjectFileInfo.h"
#include "llvm/MC/MCRegisterInfo.h"
#include "llvm/MC/MCDisassembler/MCRelocationInfo.h"
#include "llvm/MC/MCSubtargetInfo.h"
#include "llvm/Object/Archive.h"
#include "llvm/Object/ELFObjectFile.h"
#include "llvm/Object/COFF.h"
#include "llvm/Object/MachO.h"
#include "llvm/Object/ObjectFile.h"
#include "llvm/Support/Casting.h"
#include "llvm/Support/CommandLine.h"
#include "llvm/Support/Debug.h"
#include "llvm/Support/Errc.h"
#include "llvm/Support/FileSystem.h"
#include "llvm/Support/Format.h"
#include "llvm/Support/GraphWriter.h"
#include "llvm/Support/Host.h"
#include "llvm/Support/ManagedStatic.h"
#include "llvm/Support/MemoryBuffer.h"
#include "llvm/Support/PrettyStackTrace.h"
#include "llvm/Support/Signals.h"
#include "llvm/Support/SourceMgr.h"
#include "llvm/Support/TargetRegistry.h"
#include "llvm/Support/TargetSelect.h"
#include "llvm/Support/raw_ostream.h"
#include <algorithm>
#include <cctype>
#include <cstring>
#include <system_error>
#include <utility>

using namespace llvm;
using namespace object;

static cl::list<std::string>
InputFilenames(cl::Positional, cl::desc("<input object files>"),cl::ZeroOrMore);

cl::opt<std::string>
llvm::TripleName("triple", cl::desc("Target triple to disassemble for, "
                                    "see -version for available targets"));

cl::opt<std::string>
llvm::ArchName("arch-name", cl::desc("Target arch to disassemble for, "
                                "see -version for available targets"));

cl::list<std::string>
llvm::FilterSections("section", cl::desc("Operate on the specified sections only. "
                                         "With -macho dump segment,section"));
cl::alias
static FilterSectionsj("j", cl::desc("Alias for --section"),
                 cl::aliasopt(llvm::FilterSections));

cl::list<std::string>
llvm::MAttrs("mattr",
  cl::CommaSeparated,
  cl::desc("Target specific attributes"),
  cl::value_desc("a1,+a2,-a3,..."));

cl::opt<bool>
llvm::NoShowRawInsn("no-show-raw-insn", cl::desc("When disassembling "
                                                 "instructions, do not print "
                                                 "the instruction bytes."));

static StringRef ToolName;
namespace {
typedef std::function<bool(llvm::object::SectionRef const &)> FilterPredicate;

class SectionFilterIterator {
public:
  SectionFilterIterator(FilterPredicate P,
                        llvm::object::section_iterator const &I,
                        llvm::object::section_iterator const &E)
      : Predicate(std::move(P)), Iterator(I), End(E) {
    ScanPredicate();
  }
  const llvm::object::SectionRef &operator*() const { return *Iterator; }
  SectionFilterIterator &operator++() {
    ++Iterator;
    ScanPredicate();
    return *this;
  }
  bool operator!=(SectionFilterIterator const &Other) const {
    return Iterator != Other.Iterator;
  }

private:
  void ScanPredicate() {
    while (Iterator != End && !Predicate(*Iterator)) {
      ++Iterator;
    }
  }
  FilterPredicate Predicate;
  llvm::object::section_iterator Iterator;
  llvm::object::section_iterator End;
};

class SectionFilter {
public:
  SectionFilter(FilterPredicate P, llvm::object::ObjectFile const &O)
      : Predicate(std::move(P)), Object(O) {}
  SectionFilterIterator begin() {
    return SectionFilterIterator(Predicate, Object.section_begin(),
                                 Object.section_end());
  }
  SectionFilterIterator end() {
    return SectionFilterIterator(Predicate, Object.section_end(),
                                 Object.section_end());
  }

private:
  FilterPredicate Predicate;
  llvm::object::ObjectFile const &Object;
};

SectionFilter ToolSectionFilter(llvm::object::ObjectFile const &O) {
  return SectionFilter([](llvm::object::SectionRef const &S) {
                         if(FilterSections.empty())
                           return true;
                         llvm::StringRef String;
                         std::error_code error = S.getName(String);
                         if (error)
                           return false;
                         return std::find(FilterSections.begin(),
                                          FilterSections.end(),
                                          String) != FilterSections.end();
                       },
                       O);
}
}

void llvm::error(std::error_code EC) {
  if (!EC)
    return;

  errs() << ToolName << ": error reading file: " << EC.message() << ".\n";
  errs().flush();
  exit(1);
}

LLVM_ATTRIBUTE_NORETURN void llvm::error(Twine Message) {
  errs() << ToolName << ": " << Message << ".\n";
  errs().flush();
  exit(1);
}

LLVM_ATTRIBUTE_NORETURN void llvm::report_error(StringRef File,
                                                std::error_code EC) {
  assert(EC);
  errs() << ToolName << ": '" << File << "': " << EC.message() << ".\n";
  exit(1);
}

LLVM_ATTRIBUTE_NORETURN void llvm::report_error(StringRef File,
                                                llvm::Error E) {
  assert(E);
  std::string Buf;
  raw_string_ostream OS(Buf);
  logAllUnhandledErrors(std::move(E), OS, "");
  OS.flush();
  errs() << ToolName << ": '" << File << "': " << Buf;
  exit(1);
}

LLVM_ATTRIBUTE_NORETURN void llvm::report_error(StringRef ArchiveName,
                                                StringRef FileName,
                                                llvm::Error E,
                                                StringRef ArchitectureName) {
  assert(E);
  errs() << ToolName << ": ";
  if (ArchiveName != "")
    errs() << ArchiveName << "(" << FileName << ")";
  else
    errs() << FileName;
  if (!ArchitectureName.empty())
    errs() << " (for architecture " << ArchitectureName << ")";
  std::string Buf;
  raw_string_ostream OS(Buf);
  logAllUnhandledErrors(std::move(E), OS, "");
  OS.flush();
  errs() << " " << Buf;
  exit(1);
}

LLVM_ATTRIBUTE_NORETURN void llvm::report_error(StringRef ArchiveName,
                                                const object::Archive::Child &C,
                                                llvm::Error E,
                                                StringRef ArchitectureName) {
  ErrorOr<StringRef> NameOrErr = C.getName();
  // TODO: if we have a error getting the name then it would be nice to print
  // the index of which archive member this is and or its offset in the
  // archive instead of "???" as the name.
  if (NameOrErr.getError())
    llvm::report_error(ArchiveName, "???", std::move(E), ArchitectureName);
  else
    llvm::report_error(ArchiveName, NameOrErr.get(), std::move(E),
                       ArchitectureName);
}

const Target *getTarget(const ObjectFile *Obj = nullptr) {
  // Figure out the target triple.
  llvm::Triple TheTriple("unknown-unknown-unknown");
  if (TripleName.empty()) {
    if (Obj) {
      TheTriple.setArch(Triple::ArchType(Obj->getArch()));
      // TheTriple defaults to ELF, and COFF doesn't have an environment:
      // the best we can do here is indicate that it is mach-o.
      if (Obj->isMachO())
        TheTriple.setObjectFormat(Triple::MachO);

      if (Obj->isCOFF()) {
        const auto COFFObj = dyn_cast<COFFObjectFile>(Obj);
        if (COFFObj->getArch() == Triple::thumb)
          TheTriple.setTriple("thumbv7-windows");
      }
    }
  } else
    TheTriple.setTriple(Triple::normalize(TripleName));

  // Get the target specific parser.
  std::string Error;
  const Target *TheTarget = TargetRegistry::lookupTarget(ArchName, TheTriple,
                                                         Error);
  if (!TheTarget)
    report_fatal_error("can't find target: " + Error);

  // Update the triple name and return the found target.
  TripleName = TheTriple.getTriple();
  return TheTarget;
}

bool llvm::RelocAddressLess(RelocationRef a, RelocationRef b) {
  return a.getOffset() < b.getOffset();
}


template <class ELFT>
static std::error_code getRelocationValueString(const ELFObjectFile<ELFT> *Obj,
                                                const RelocationRef &RelRef,
                                                SmallVectorImpl<char> &Result) {
  DataRefImpl Rel = RelRef.getRawDataRefImpl();

  typedef typename ELFObjectFile<ELFT>::Elf_Sym Elf_Sym;
  typedef typename ELFObjectFile<ELFT>::Elf_Shdr Elf_Shdr;
  typedef typename ELFObjectFile<ELFT>::Elf_Rela Elf_Rela;

  const ELFFile<ELFT> &EF = *Obj->getELFFile();

  ErrorOr<const Elf_Shdr *> SecOrErr = EF.getSection(Rel.d.a);
  if (std::error_code EC = SecOrErr.getError())
    return EC;
  const Elf_Shdr *Sec = *SecOrErr;
  ErrorOr<const Elf_Shdr *> SymTabOrErr = EF.getSection(Sec->sh_link);
  if (std::error_code EC = SymTabOrErr.getError())
    return EC;
  const Elf_Shdr *SymTab = *SymTabOrErr;
  assert(SymTab->sh_type == ELF::SHT_SYMTAB ||
         SymTab->sh_type == ELF::SHT_DYNSYM);
  ErrorOr<const Elf_Shdr *> StrTabSec = EF.getSection(SymTab->sh_link);
  if (std::error_code EC = StrTabSec.getError())
    return EC;
  ErrorOr<StringRef> StrTabOrErr = EF.getStringTable(*StrTabSec);
  if (std::error_code EC = StrTabOrErr.getError())
    return EC;
  StringRef StrTab = *StrTabOrErr;
  uint8_t type = RelRef.getType();
  StringRef res;
  int64_t addend = 0;
  switch (Sec->sh_type) {
  default:
    return object_error::parse_failed;
  case ELF::SHT_REL: {
    // TODO: Read implicit addend from section data.
    break;
  }
  case ELF::SHT_RELA: {
    const Elf_Rela *ERela = Obj->getRela(Rel);
    addend = ERela->r_addend;
    break;
  }
  }
  symbol_iterator SI = RelRef.getSymbol();
  const Elf_Sym *symb = Obj->getSymbol(SI->getRawDataRefImpl());
  StringRef Target;
  if (symb->getType() == ELF::STT_SECTION) {
    Expected<section_iterator> SymSI = SI->getSection();
    if (!SymSI)
      return errorToErrorCode(SymSI.takeError());
    const Elf_Shdr *SymSec = Obj->getSection((*SymSI)->getRawDataRefImpl());
    ErrorOr<StringRef> SecName = EF.getSectionName(SymSec);
    if (std::error_code EC = SecName.getError())
      return EC;
    Target = *SecName;
  } else {
    Expected<StringRef> SymName = symb->getName(StrTab);
    if (!SymName)
      return errorToErrorCode(SymName.takeError());
    Target = *SymName;
  }
  switch (EF.getHeader()->e_machine) {
  case ELF::EM_X86_64:
    switch (type) {
    case ELF::R_X86_64_PC8:
    case ELF::R_X86_64_PC16:
    case ELF::R_X86_64_PC32: {
      std::string fmtbuf;
      raw_string_ostream fmt(fmtbuf);
      fmt << Target << (addend < 0 ? "" : "+") << addend << "-P";
      fmt.flush();
      Result.append(fmtbuf.begin(), fmtbuf.end());
    } break;
    case ELF::R_X86_64_8:
    case ELF::R_X86_64_16:
    case ELF::R_X86_64_32:
    case ELF::R_X86_64_32S:
    case ELF::R_X86_64_64: {
      std::string fmtbuf;
      raw_string_ostream fmt(fmtbuf);
      fmt << Target << (addend < 0 ? "" : "+") << addend;
      fmt.flush();
      Result.append(fmtbuf.begin(), fmtbuf.end());
    } break;
    default:
      res = "Unknown";
    }
    break;
  case ELF::EM_LANAI:
  case ELF::EM_AARCH64: {
    std::string fmtbuf;
    raw_string_ostream fmt(fmtbuf);
    fmt << Target;
    if (addend != 0)
      fmt << (addend < 0 ? "" : "+") << addend;
    fmt.flush();
    Result.append(fmtbuf.begin(), fmtbuf.end());
    break;
  }
  case ELF::EM_386:
  case ELF::EM_IAMCU:
  case ELF::EM_ARM:
  case ELF::EM_HEXAGON:
  case ELF::EM_MIPS:
  case ELF::EM_BPF:
    res = Target;
    break;
  case ELF::EM_WEBASSEMBLY:
    switch (type) {
    case ELF::R_WEBASSEMBLY_DATA: {
      std::string fmtbuf;
      raw_string_ostream fmt(fmtbuf);
      fmt << Target << (addend < 0 ? "" : "+") << addend;
      fmt.flush();
      Result.append(fmtbuf.begin(), fmtbuf.end());
      break;
    }
    case ELF::R_WEBASSEMBLY_FUNCTION:
      res = Target;
      break;
    default:
      res = "Unknown";
    }
    break;
  default:
    res = "Unknown";
  }
  if (Result.empty())
    Result.append(res.begin(), res.end());
  return std::error_code();
}

static std::error_code getRelocationValueString(const ELFObjectFileBase *Obj,
                                                const RelocationRef &Rel,
                                                SmallVectorImpl<char> &Result) {
  if (auto *ELF32LE = dyn_cast<ELF32LEObjectFile>(Obj))
    return getRelocationValueString(ELF32LE, Rel, Result);
  if (auto *ELF64LE = dyn_cast<ELF64LEObjectFile>(Obj))
    return getRelocationValueString(ELF64LE, Rel, Result);
  if (auto *ELF32BE = dyn_cast<ELF32BEObjectFile>(Obj))
    return getRelocationValueString(ELF32BE, Rel, Result);
  auto *ELF64BE = cast<ELF64BEObjectFile>(Obj);
  return getRelocationValueString(ELF64BE, Rel, Result);
}

static std::error_code getRelocationValueString(const COFFObjectFile *Obj,
                                                const RelocationRef &Rel,
                                                SmallVectorImpl<char> &Result) {
  symbol_iterator SymI = Rel.getSymbol();
  Expected<StringRef> SymNameOrErr = SymI->getName();
  if (!SymNameOrErr)
    return errorToErrorCode(SymNameOrErr.takeError());
  StringRef SymName = *SymNameOrErr;
  Result.append(SymName.begin(), SymName.end());
  return std::error_code();
}

static void printRelocationTargetName(const MachOObjectFile *O,
                                      const MachO::any_relocation_info &RE,
                                      raw_string_ostream &fmt) {
  bool IsScattered = O->isRelocationScattered(RE);

  // Target of a scattered relocation is an address.  In the interest of
  // generating pretty output, scan through the symbol table looking for a
  // symbol that aligns with that address.  If we find one, print it.
  // Otherwise, we just print the hex address of the target.
  if (IsScattered) {
    uint32_t Val = O->getPlainRelocationSymbolNum(RE);

    for (const SymbolRef &Symbol : O->symbols()) {
      std::error_code ec;
      Expected<uint64_t> Addr = Symbol.getAddress();
      if (!Addr) {
        std::string Buf;
        raw_string_ostream OS(Buf);
        logAllUnhandledErrors(Addr.takeError(), OS, "");
        OS.flush();
        report_fatal_error(Buf);
      }
      if (*Addr != Val)
        continue;
      Expected<StringRef> Name = Symbol.getName();
      if (!Name) {
        std::string Buf;
        raw_string_ostream OS(Buf);
        logAllUnhandledErrors(Name.takeError(), OS, "");
        OS.flush();
        report_fatal_error(Buf);
      }
      fmt << *Name;
      return;
    }

    // If we couldn't find a symbol that this relocation refers to, try
    // to find a section beginning instead.
    for (const SectionRef &Section : ToolSectionFilter(*O)) {
      std::error_code ec;

      StringRef Name;
      uint64_t Addr = Section.getAddress();
      if (Addr != Val)
        continue;
      if ((ec = Section.getName(Name)))
        report_fatal_error(ec.message());
      fmt << Name;
      return;
    }

    fmt << format("0x%x", Val);
    return;
  }

  StringRef S;
  bool isExtern = O->getPlainRelocationExternal(RE);
  uint64_t Val = O->getPlainRelocationSymbolNum(RE);

  if (isExtern) {
    symbol_iterator SI = O->symbol_begin();
    advance(SI, Val);
    Expected<StringRef> SOrErr = SI->getName();
    error(errorToErrorCode(SOrErr.takeError()));
    S = *SOrErr;
  } else {
    section_iterator SI = O->section_begin();
    // Adjust for the fact that sections are 1-indexed.
    advance(SI, Val - 1);
    SI->getName(S);
  }

  fmt << S;
}

static std::error_code getRelocationValueString(const MachOObjectFile *Obj,
                                                const RelocationRef &RelRef,
                                                SmallVectorImpl<char> &Result) {
  DataRefImpl Rel = RelRef.getRawDataRefImpl();
  MachO::any_relocation_info RE = Obj->getRelocation(Rel);

  unsigned Arch = Obj->getArch();

  std::string fmtbuf;
  raw_string_ostream fmt(fmtbuf);
  unsigned Type = Obj->getAnyRelocationType(RE);
  bool IsPCRel = Obj->getAnyRelocationPCRel(RE);

  // Determine any addends that should be displayed with the relocation.
  // These require decoding the relocation type, which is triple-specific.

  // X86_64 has entirely custom relocation types.
  if (Arch == Triple::x86_64) {
    bool isPCRel = Obj->getAnyRelocationPCRel(RE);

    switch (Type) {
    case MachO::X86_64_RELOC_GOT_LOAD:
    case MachO::X86_64_RELOC_GOT: {
      printRelocationTargetName(Obj, RE, fmt);
      fmt << "@GOT";
      if (isPCRel)
        fmt << "PCREL";
      break;
    }
    case MachO::X86_64_RELOC_SUBTRACTOR: {
      DataRefImpl RelNext = Rel;
      Obj->moveRelocationNext(RelNext);
      MachO::any_relocation_info RENext = Obj->getRelocation(RelNext);

      // X86_64_RELOC_SUBTRACTOR must be followed by a relocation of type
      // X86_64_RELOC_UNSIGNED.
      // NOTE: Scattered relocations don't exist on x86_64.
      unsigned RType = Obj->getAnyRelocationType(RENext);
      if (RType != MachO::X86_64_RELOC_UNSIGNED)
        report_fatal_error("Expected X86_64_RELOC_UNSIGNED after "
                           "X86_64_RELOC_SUBTRACTOR.");

      // The X86_64_RELOC_UNSIGNED contains the minuend symbol;
      // X86_64_RELOC_SUBTRACTOR contains the subtrahend.
      printRelocationTargetName(Obj, RENext, fmt);
      fmt << "-";
      printRelocationTargetName(Obj, RE, fmt);
      break;
    }
    case MachO::X86_64_RELOC_TLV:
      printRelocationTargetName(Obj, RE, fmt);
      fmt << "@TLV";
      if (isPCRel)
        fmt << "P";
      break;
    case MachO::X86_64_RELOC_SIGNED_1:
      printRelocationTargetName(Obj, RE, fmt);
      fmt << "-1";
      break;
    case MachO::X86_64_RELOC_SIGNED_2:
      printRelocationTargetName(Obj, RE, fmt);
      fmt << "-2";
      break;
    case MachO::X86_64_RELOC_SIGNED_4:
      printRelocationTargetName(Obj, RE, fmt);
      fmt << "-4";
      break;
    default:
      printRelocationTargetName(Obj, RE, fmt);
      break;
    }
    // X86 and ARM share some relocation types in common.
  } else if (Arch == Triple::x86 || Arch == Triple::arm ||
             Arch == Triple::ppc) {
    // Generic relocation types...
    switch (Type) {
    case MachO::GENERIC_RELOC_PAIR: // prints no info
      return std::error_code();
    case MachO::GENERIC_RELOC_SECTDIFF: {
      DataRefImpl RelNext = Rel;
      Obj->moveRelocationNext(RelNext);
      MachO::any_relocation_info RENext = Obj->getRelocation(RelNext);

      // X86 sect diff's must be followed by a relocation of type
      // GENERIC_RELOC_PAIR.
      unsigned RType = Obj->getAnyRelocationType(RENext);

      if (RType != MachO::GENERIC_RELOC_PAIR)
        report_fatal_error("Expected GENERIC_RELOC_PAIR after "
                           "GENERIC_RELOC_SECTDIFF.");

      printRelocationTargetName(Obj, RE, fmt);
      fmt << "-";
      printRelocationTargetName(Obj, RENext, fmt);
      break;
    }
    }

    if (Arch == Triple::x86 || Arch == Triple::ppc) {
      switch (Type) {
      case MachO::GENERIC_RELOC_LOCAL_SECTDIFF: {
        DataRefImpl RelNext = Rel;
        Obj->moveRelocationNext(RelNext);
        MachO::any_relocation_info RENext = Obj->getRelocation(RelNext);

        // X86 sect diff's must be followed by a relocation of type
        // GENERIC_RELOC_PAIR.
        unsigned RType = Obj->getAnyRelocationType(RENext);
        if (RType != MachO::GENERIC_RELOC_PAIR)
          report_fatal_error("Expected GENERIC_RELOC_PAIR after "
                             "GENERIC_RELOC_LOCAL_SECTDIFF.");

        printRelocationTargetName(Obj, RE, fmt);
        fmt << "-";
        printRelocationTargetName(Obj, RENext, fmt);
        break;
      }
      case MachO::GENERIC_RELOC_TLV: {
        printRelocationTargetName(Obj, RE, fmt);
        fmt << "@TLV";
        if (IsPCRel)
          fmt << "P";
        break;
      }
      default:
        printRelocationTargetName(Obj, RE, fmt);
      }
    } else { // ARM-specific relocations
      switch (Type) {
      case MachO::ARM_RELOC_HALF:
      case MachO::ARM_RELOC_HALF_SECTDIFF: {
        // Half relocations steal a bit from the length field to encode
        // whether this is an upper16 or a lower16 relocation.
        bool isUpper = Obj->getAnyRelocationLength(RE) >> 1;

        if (isUpper)
          fmt << ":upper16:(";
        else
          fmt << ":lower16:(";
        printRelocationTargetName(Obj, RE, fmt);

        DataRefImpl RelNext = Rel;
        Obj->moveRelocationNext(RelNext);
        MachO::any_relocation_info RENext = Obj->getRelocation(RelNext);

        // ARM half relocs must be followed by a relocation of type
        // ARM_RELOC_PAIR.
        unsigned RType = Obj->getAnyRelocationType(RENext);
        if (RType != MachO::ARM_RELOC_PAIR)
          report_fatal_error("Expected ARM_RELOC_PAIR after "
                             "ARM_RELOC_HALF");

        // NOTE: The half of the target virtual address is stashed in the
        // address field of the secondary relocation, but we can't reverse
        // engineer the constant offset from it without decoding the movw/movt
        // instruction to find the other half in its immediate field.

        // ARM_RELOC_HALF_SECTDIFF encodes the second section in the
        // symbol/section pointer of the follow-on relocation.
        if (Type == MachO::ARM_RELOC_HALF_SECTDIFF) {
          fmt << "-";
          printRelocationTargetName(Obj, RENext, fmt);
        }

        fmt << ")";
        break;
      }
      default: { printRelocationTargetName(Obj, RE, fmt); }
      }
    }
  } else
    printRelocationTargetName(Obj, RE, fmt);

  fmt.flush();
  Result.append(fmtbuf.begin(), fmtbuf.end());
  return std::error_code();
}

std::error_code getRelocationValueString(const RelocationRef &Rel,
                                                SmallVectorImpl<char> &Result) {
  const ObjectFile *Obj = Rel.getObject();
  if (auto *ELF = dyn_cast<ELFObjectFileBase>(Obj))
    return getRelocationValueString(ELF, Rel, Result);
  if (auto *COFF = dyn_cast<COFFObjectFile>(Obj))
    return getRelocationValueString(COFF, Rel, Result);
  auto *MachO = cast<MachOObjectFile>(Obj);
  return getRelocationValueString(MachO, Rel, Result);
}

/// @brief Indicates whether this relocation should hidden when listing
/// relocations, usually because it is the trailing part of a multipart
/// relocation that will be printed as part of the leading relocation.
bool getHidden(RelocationRef RelRef) {
  const ObjectFile *Obj = RelRef.getObject();
  auto *MachO = dyn_cast<MachOObjectFile>(Obj);
  if (!MachO)
    return false;

  unsigned Arch = MachO->getArch();
  DataRefImpl Rel = RelRef.getRawDataRefImpl();
  uint64_t Type = MachO->getRelocationType(Rel);

  // On arches that use the generic relocations, GENERIC_RELOC_PAIR
  // is always hidden.
  if (Arch == Triple::x86 || Arch == Triple::arm || Arch == Triple::ppc) {
    if (Type == MachO::GENERIC_RELOC_PAIR)
      return true;
  } else if (Arch == Triple::x86_64) {
    // On x86_64, X86_64_RELOC_UNSIGNED is hidden only when it follows
    // an X86_64_RELOC_SUBTRACTOR.
    if (Type == MachO::X86_64_RELOC_UNSIGNED && Rel.d.a > 0) {
      DataRefImpl RelPrev = Rel;
      RelPrev.d.a--;
      uint64_t PrevType = MachO->getRelocationType(RelPrev);
      if (PrevType == MachO::X86_64_RELOC_SUBTRACTOR)
        return true;
    }
  }

  return false;
}


static cl::opt<bool>
LittleEndian("le", 
cl::desc("Little endian format"));

#ifdef ELF2HEX_DEBUG
// Modified from PrintSectionHeaders()
uint64_t GetSectionHeaderStartAddress(const ObjectFile *Obj, 
  StringRef sectionName) {
//  outs() << "Sections:\n"
//            "Idx Name          Size      Address          Type\n";
  std::error_code ec;
  unsigned i = 0;
  for (const SectionRef &Section : Obj->sections()) {
    error(ec);
    StringRef Name;
    error(Section.getName(Name));
    uint64_t Address;
    Address = Section.getAddress();
    uint64_t Size;
    Size = Section.getSize();
    bool Text;
    Text = Section.isText();
    if (Name == sectionName)
      return Address;
    else
      return 0;
    ++i;
  }
  return 0;
}
#endif

// Reference from llvm::PrintSymbolTable of llvm-objdump.cpp
uint64_t GetSymbolAddress(const ObjectFile *o, StringRef SymbolName) {
  for (const SymbolRef &Symbol : o->symbols()) {
    Expected<uint64_t> AddressOrError = Symbol.getAddress();
    if (!AddressOrError)
      report_error(SymbolName, o->getFileName(), AddressOrError.takeError());
    uint64_t Address = *AddressOrError;
    Expected<SymbolRef::Type> TypeOrError = Symbol.getType();
    if (!TypeOrError)
      report_error(SymbolName, o->getFileName(), TypeOrError.takeError());
    SymbolRef::Type Type = *TypeOrError;
    Expected<section_iterator> SectionOrErr = Symbol.getSection();
    error(errorToErrorCode(SectionOrErr.takeError()));
    section_iterator Section = *SectionOrErr;
    StringRef Name;
    if (Type == SymbolRef::ST_Debug && Section != o->section_end()) {
      Section->getName(Name);
    } else {
      Expected<StringRef> NameOrErr = Symbol.getName();
      if (!NameOrErr)
        report_error(SymbolName, o->getFileName(), NameOrErr.takeError(),
                     SymbolName);
      Name = *NameOrErr;
    }
    if (Name == SymbolName)
      return Address;
  }
  return 0;
}

// Fill /*address*/ 00 00 00 00 [startAddr..endAddr] from startAddr to endAddr. 
// Include startAddr and endAddr.
static void Fill0s(uint64_t startAddr, uint64_t endAddr) {
  std::size_t addr;

  assert((startAddr <= endAddr) && "startAddr must <= BaseAddr");
  // Fill /*address*/ 00 00 00 00 for 4 bytes alignment (1 Cpu0 word size)
  for (addr = startAddr; addr < endAddr; addr += 4) {
    outs() << format("/*%8" PRIx64 " */", addr);
    outs() << format("%02" PRIx64 " ", 0) << format("%02" PRIx64 " ", 0) \
    << format("%02" PRIx64 " ", 0) << format("%02" PRIx64 " ", 0) << '\n';
  }

  return;
}

static void PrintDataSection(const ObjectFile *o, uint64_t& lastDumpAddr, 
  SectionRef Section) {
  std::string Error;
  StringRef Name;
  StringRef Contents;
  uint64_t BaseAddr;
  bool BSS;
  uint64_t size;
  error(Section.getName(Name));
  error(Section.getContents(Contents));
  BaseAddr = Section.getAddress();
  BSS = Section.isBSS();

  size = (Contents.size()+3)/4*4;
  if (Contents.size() <= 0) {
    return;
  }

  outs() << "/*Contents of section " << Name << ":*/\n";
  // Dump out the content as hex and printable ascii characters.
  for (std::size_t addr = 0, end = Contents.size(); addr < end; addr += 16) {
    outs() << format("/*%8" PRIx64 " */", BaseAddr + addr);
    // Dump line of hex.
    for (std::size_t i = 0; i < 16; ++i) {
      if (i != 0 && i % 4 == 0)
        outs() << ' ';
      if (addr + i < end)
        outs() << hexdigit((Contents[addr + i] >> 4) & 0xF, true)
               << hexdigit(Contents[addr + i] & 0xF, true) << " ";
    }
    // Print ascii.
    outs() << "/*" << "  ";
    for (std::size_t i = 0; i < 16 && addr + i < end; ++i) {
      if (std::isprint(static_cast<unsigned char>(Contents[addr + i]) & 0xFF))
        outs() << Contents[addr + i];
      else
        outs() << ".";
    }
    outs() << "*/" << "\n";
  }
  for (std::size_t i = Contents.size(); i < size; i++) {
    outs() << "00 ";
  }
  outs() << "\n";
#ifdef ELF2HEX_DEBUG
  errs() << "Name " << Name << "  BaseAddr ";
  errs() << format("%8" PRIx64 " Contents.size() ", BaseAddr);
  errs() << format("%8" PRIx64 " size ", Contents.size());
  errs() << format("%8" PRIx64 " \n", size);
#endif
  // save the end address of this section to lastDumpAddr
  lastDumpAddr = BaseAddr + size;
}

// Modified from DisassembleObject()
static void DisassembleObjectInHexFormat(const ObjectFile *Obj
/*, bool InlineRelocs*/  , std::unique_ptr<MCDisassembler>& DisAsm, 
  std::unique_ptr<MCInstPrinter>& IP, uint64_t& lastDumpAddr,
  std::unique_ptr<const MCSubtargetInfo>& STI) {

#ifdef ELF2HEX_DEBUG
  errs() << __FUNCTION__<< "\t" << format("!lastDumpAddr %8" PRIx64 "\n", lastDumpAddr);
#endif
  std::error_code ec;
  for (const SectionRef &Section : Obj->sections()) {
    error(ec);
    StringRef Name;
    StringRef Contents;
    uint64_t BaseAddr;
    error(Section.getName(Name));
    error(Section.getContents(Contents));
    BaseAddr = Section.getAddress();
    if (BaseAddr < 0x100)
      continue;
  #ifdef ELF2HEX_DEBUG
    errs() << "Name " << Name << format("  BaseAddr %8" PRIx64 "\n", BaseAddr);
    errs() << format("!!lastDumpAddr %8" PRIx64 "\n", lastDumpAddr);
  #endif
    bool text;
    text = Section.isText();
    if (!text) {
    #ifdef ELF2HEX_DEBUG
      errs() << "!text\n";
    #endif
      if (lastDumpAddr < BaseAddr) {
        Fill0s(lastDumpAddr, BaseAddr - 1);
        lastDumpAddr = BaseAddr;
      }
      if (Name == ".got.plt") {
        uint64_t BaseAddr;
        BaseAddr = Section.getAddress();
        PrintDataSection(Obj, lastDumpAddr, Section);
      }
      else if ((Name == ".bss" || Name == ".sbss") && Contents.size() > 0) {
        uint64_t size = (Contents.size() + 3)/4*4;
        Fill0s(BaseAddr, BaseAddr + size - 1);
        lastDumpAddr = BaseAddr + size;
        continue;
      }
      else {
        PrintDataSection(Obj, lastDumpAddr, Section);
      }
      continue;
    }
    else {
      if (lastDumpAddr < BaseAddr) {
        Fill0s(lastDumpAddr, BaseAddr - 1);
        lastDumpAddr = BaseAddr;
      }
    }
    // It's .text section
    uint64_t SectionAddr;
    SectionAddr = Section.getAddress();
    uint64_t SectSize = Section.getSize();
    if (!SectSize)
      continue;

    // Make a list of all the symbols in this section.
    std::vector<std::pair<uint64_t, StringRef> > Symbols;
    for (const SymbolRef &Symbol : Obj->symbols()) {
      if (Section.containsSymbol(Symbol)) {
        Expected<uint64_t> AddressOrErr = Symbol.getAddress();
        error(errorToErrorCode(AddressOrErr.takeError()));
        uint64_t Address = *AddressOrErr;
        Address -= SectionAddr;
        if (Address >= SectSize)
          continue;

        Expected<StringRef> Name = Symbol.getName();
        error(errorToErrorCode(Name.takeError()));
        Symbols.push_back(std::make_pair(Address, *Name));
      }
    }

    // Sort the symbols by address, just in case they didn't come in that way.
    array_pod_sort(Symbols.begin(), Symbols.end());
  #ifdef ELF2HEX_DEBUG
    for (unsigned si = 0, se = Symbols.size(); si != se; ++si) {
        errs() << '\n' << "/*" << Symbols[si].first << "  " << Symbols[si].second << ":*/\n";
    }
  #endif

    // Make a list of all the relocations for this section.
    std::vector<RelocationRef> Rels;

    // Sort relocations by address.
    std::sort(Rels.begin(), Rels.end(), RelocAddressLess);

    StringRef SegmentName = "";
    if (const MachOObjectFile *MachO =
        dyn_cast<const MachOObjectFile>(Obj)) {
      DataRefImpl DR = Section.getRawDataRefImpl();
      SegmentName = MachO->getSectionFinalSegmentName(DR);
    }
    StringRef name;
    error(Section.getName(name));
    outs() << "/*" << "Disassembly of section ";
    if (!SegmentName.empty())
      outs() << SegmentName << ",";
    outs() << name << ':' << "*/";

    // If the section has no symbols just insert a dummy one and disassemble
    // the whole section.
    if (Symbols.empty())
      Symbols.push_back(std::make_pair(0, name));

    SmallString<40> Comments;
    raw_svector_ostream CommentStream(Comments);

    StringRef BytesStr;
    error(Section.getContents(BytesStr));
    ArrayRef<uint8_t> Bytes(reinterpret_cast<const uint8_t *>(BytesStr.data()),
                            BytesStr.size());
    uint64_t Size;
    uint64_t Index;
    SectSize = Section.getSize();

    std::vector<RelocationRef>::const_iterator rel_cur = Rels.begin();
    std::vector<RelocationRef>::const_iterator rel_end = Rels.end();
    // Disassemble symbol by symbol.
    for (unsigned si = 0, se = Symbols.size(); si != se; ++si) {
      uint64_t Start = Symbols[si].first;
      uint64_t End;
      // The end is either the size of the section or the beginning of the next
      // symbol.
      if (si == se - 1)
        End = SectSize;
      // Make sure this symbol takes up space.
      else if (Symbols[si + 1].first != Start)
        End = Symbols[si + 1].first - 1;
      else {
        outs() << '\n' << "/*" << Symbols[si].second << ":*/\n";
        continue;
      }

      outs() << '\n' << "/*" << Symbols[si].second << ":*/\n";

#ifndef NDEBUG
        raw_ostream &DebugOut = DebugFlag ? dbgs() : nulls();
#else
        raw_ostream &DebugOut = nulls();
#endif

      for (Index = Start; Index < End; Index += Size) {
        MCInst Inst;
        if (DisAsm->getInstruction(Inst, Size, Bytes.slice(Index),
                                   SectionAddr + Index, DebugOut,
                                   CommentStream)) {
          outs() << format("/*%8" PRIx64 ":*/", SectionAddr + Index);
          if (!NoShowRawInsn) {
            outs() << "\t";
            dumpBytes(Bytes.slice(Index, Size), outs());
          }
          outs() << "/*";
          IP->printInst(&Inst, outs(), "", *STI);
          outs() << CommentStream.str();
          outs() << "*/";
          Comments.clear();
          outs() << "\n";
        } else {
          errs() << ToolName << ": warning: invalid instruction encoding\n";
          if (Size == 0)
            Size = 1; // skip illegible bytes
        }

        //  outs() << "Size = " << Size <<  "Index = " << Index << "lastDumpAddr = "
        //         << lastDumpAddr << "\n"; // debug
        // Print relocation for instruction.
        while (rel_cur != rel_end) {
          bool hidden = getHidden(*rel_cur);
          uint64_t addr = rel_cur->getOffset();
          SmallString<16> name;
          SmallString<32> val;

          // If this relocation is hidden, skip it.
          if (hidden) goto skip_print_rel;

          // Stop when rel_cur's address is past the current instruction.
          if (addr >= Index + Size) break;
          rel_cur->getTypeName(name);
          error(getRelocationValueString(*rel_cur, val));

          outs() << format("\t\t\t/*%8" PRIx64 ": ", SectionAddr + addr) << name
                 << "\t" << val << "*/\n";

        skip_print_rel:
          ++rel_cur;
        }
      }
    #ifdef ELF2HEX_DEBUG
      errs() << format("SectionAddr + Index = %8" PRIx64 "\n", SectionAddr + Index);
      errs() << format("lastDumpAddr %8" PRIx64 "\n", lastDumpAddr);
    #endif
    }
    // In section .plt or .text, the Contents.size() maybe < (SectionAddr + Index)
    if (Contents.size() < (SectionAddr + Index))
      lastDumpAddr = SectionAddr + Index;
    else
      lastDumpAddr = SectionAddr + Contents.size();
  }
}

uint64_t SectionOffset(const ObjectFile *o, StringRef secName) {
  std::error_code ec;

  for (const SectionRef &Section : o->sections()) {
    error(ec);
    StringRef Name;
    StringRef Contents;
    uint64_t BaseAddr;
    bool BSS;
    error(Section.getName(Name));
    error(Section.getContents(Contents));
    BaseAddr = Section.getAddress();
    BSS = Section.isBSS();

    if (Name == secName)
      return BaseAddr;
  }
  return 0;
}

static void PrintBootSection(uint64_t textOffset, uint64_t isrAddr, bool isLittleEndian) {
  uint64_t offset = textOffset - 4;

  // isr instruction at 0x8 and PC counter point to next instruction
  uint64_t isrOffset = isrAddr - 8 - 4;
  if (isLittleEndian) {
    outs() << "/*       0:*/	";
    outs() << format("%02" PRIx64 " ", (offset & 0xff));
    outs() << format("%02" PRIx64 " ", (offset & 0xff00) >> 8);
    outs() << format("%02" PRIx64 "", (offset & 0xff0000) >> 16);
    outs() << " 36";
    outs() << "                                  /*	jmp	0x";
    outs() << format("%02" PRIx64 "%02" PRIx64 "%02" PRIx64 " */\n",
      (offset & 0xff0000) >> 16, (offset & 0xff00) >> 8, (offset & 0xff));
    outs() <<
      "/*       4:*/	04 00 00 36                                  /*	jmp	4 */\n";
    offset -= 8;
    outs() << "/*       8:*/	";
    outs() << format("%02" PRIx64 " ", (isrOffset & 0xff));
    outs() << format("%02" PRIx64 " ", (isrOffset & 0xff00) >> 8);
    outs() << format("%02" PRIx64 "", (isrOffset & 0xff0000) >> 16);
    outs() << " 36";
    outs() << "                                  /*	jmp	0x";
    outs() << format("%02" PRIx64 "%02" PRIx64 "%02" PRIx64 " */\n",
      (isrOffset & 0xff0000) >> 16, (isrOffset & 0xff00) >> 8, (isrOffset & 0xff));
    outs() <<
      "/*       c:*/	fc ff ff 36                                  /*	jmp	-4 */\n";
  }
  else {
    outs() << "/*       0:*/	36 ";
    outs() << format("%02" PRIx64 " ", (offset & 0xff0000) >> 16);
    outs() << format("%02" PRIx64 " ", (offset & 0xff00) >> 8);
    outs() << format("%02" PRIx64 "", (offset & 0xff));
    outs() << "                                  /*	jmp	0x";
    outs() << format("%02" PRIx64 "%02" PRIx64 "%02" PRIx64 " */\n",
      (offset & 0xff0000) >> 16, (offset & 0xff00) >> 8, (offset & 0xff));
    outs() <<
      "/*       4:*/	36 00 00 04                                  /*	jmp	4 */\n";
    offset -= 8;
    outs() << "/*       8:*/	36 ";
    outs() << format("%02" PRIx64 " ", (isrOffset & 0xff0000) >> 16);
    outs() << format("%02" PRIx64 " ", (isrOffset & 0xff00) >> 8);
    outs() << format("%02" PRIx64 "", (isrOffset & 0xff));
    outs() << "                                  /*	jmp	0x";
    outs() << format("%02" PRIx64 "%02" PRIx64 "%02" PRIx64 " */\n",
      (isrOffset & 0xff0000) >> 16, (isrOffset & 0xff00) >> 8, (isrOffset & 0xff));
    outs() <<
      "/*       c:*/	36 ff ff fc                                  /*	jmp	-4 */\n";
  }
}

#if 0
// Create by ref PrintSymbolTable()
static void FillJTI(const ObjectFile *o) {
  for (const SymbolRef &Symbol : o->symbols()) {
    StringRef JTIBlockName;
    uint64_t BBAddr[0x10000];
    StringRef Name;
    uint64_t Address;
    SymbolRef::Type Type;
    uint64_t Size;
    uint32_t Flags = Symbol.getFlags();
    section_iterator Section = o->section_end();
    if (error(Section->getName(SectionName)))
      SectionName = "";
    if (SectionName != ".rodata") continue;
    if (error(Symbol.getName(Name)))
      continue;
  // For example: Name=JTI8_0 => JTIBlockName=BB8 (rule hit until _)
    if (strncmp(Name.c_str(), "JTI", strlen("JTI") == 0) {
      int i = 0;
      for (i = strlen("JTI"); Name[i] != '_'; i++);
      if (i > strlen("JTI")) 
        JTIBlockName = "BB" + Name.substr(strlen("JTI"), i-strlen("JTI"));
    }
  // Then get all BB8_* address.
    Address = Section.getAddress();
    if (error(Symbol.getSection(Section)))
      continue;
    if (Section != ".text")
      continue;
}
#endif

// Modified from DisassembleObject()
static void Elf2Hex(const ObjectFile *Obj) {
  uint64_t lastDumpAddr = 0;

  const Target *TheTarget = getTarget(Obj);

  // Package up features to be passed to target/subtarget
  SubtargetFeatures Features = Obj->getFeatures();
  if (MAttrs.size()) {
    for (unsigned i = 0; i != MAttrs.size(); ++i)
      Features.AddFeature(MAttrs[i]);
  }

  std::unique_ptr<const MCRegisterInfo> MRI(TheTarget->createMCRegInfo(TripleName));
  if (!MRI)
    report_fatal_error("error: no register info for target " + TripleName);

  // Set up disassembler.
  std::unique_ptr<const MCAsmInfo> AsmInfo(
    TheTarget->createMCAsmInfo(*MRI, TripleName));
  if (!AsmInfo)
    report_fatal_error("error: no assembly info for target " + TripleName);

  std::unique_ptr<const MCSubtargetInfo> STI(
    TheTarget->createMCSubtargetInfo(TripleName, "", Features.getString()));
  if (!STI)
    report_fatal_error("error: no subtarget info for target " + TripleName);

  std::unique_ptr<const MCInstrInfo> MII(TheTarget->createMCInstrInfo());
  if (!MII)
    report_fatal_error("error: no instruction info for target " + TripleName);

  std::unique_ptr<const MCObjectFileInfo> MOFI(new MCObjectFileInfo);
  MCContext Ctx(AsmInfo.get(), MRI.get(), MOFI.get());

  std::unique_ptr<MCDisassembler> DisAsm(
    TheTarget->createMCDisassembler(*STI, Ctx));
  if (!DisAsm)
    report_fatal_error("error: no disassembler for target " + TripleName);

  std::unique_ptr<const MCInstrAnalysis> MIA(
      TheTarget->createMCInstrAnalysis(MII.get()));

  int AsmPrinterVariant = AsmInfo->getAssemblerDialect();
  std::unique_ptr<MCInstPrinter> IP(TheTarget->createMCInstPrinter(
      Triple(TripleName), AsmPrinterVariant, *AsmInfo, *MII, *MRI));
  if (!IP)
    report_fatal_error("error: no instruction printer for target " +
                       TripleName);

#ifdef ELF2HEX_DEBUG
  uint64_t startAddr = GetSectionHeaderStartAddress(Obj, "_start");
  errs() << format("_start address:%08" PRIx64 "\n", startAddr);
#endif
  uint64_t isrAddr = GetSymbolAddress(Obj, "ISR");
  errs() << format("ISR address:%08" PRIx64 "\n", isrAddr);
  {
    std::error_code EC;
    //uint64_t pltOffset = SectionOffset(Obj, ".plt");
    uint64_t textOffset = SectionOffset(Obj, ".text");
    PrintBootSection(textOffset, isrAddr, LittleEndian);
    lastDumpAddr = BOOT_SIZE;
    Fill0s(lastDumpAddr, 0x100);
    lastDumpAddr = 0x100;
    DisassembleObjectInHexFormat(Obj, DisAsm, IP, lastDumpAddr, STI);
  }
}

static void DumpObject(const ObjectFile *o) {
  outs() << "/*";
  outs() << o->getFileName()
         << ":\tfile format " << o->getFileFormatName() << "*/";
  outs() << "\n\n";

  Elf2Hex(o);
}

/// @brief Open file and figure out how to dump it.
static void DumpInput(StringRef file) {
  // Attempt to open the binary.
  Expected<OwningBinary<Binary>> BinaryOrErr = createBinary(file);
  if (!BinaryOrErr)
    report_error(file, BinaryOrErr.takeError());

  Binary &Binary = *BinaryOrErr.get().getBinary();

  if (ObjectFile *o = dyn_cast<ObjectFile>(&Binary))
    DumpObject(o);
  else
    report_error(file, object_error::invalid_file_type);
}

int main(int argc, char **argv) {
  // Print a stack trace if we signal out.
  sys::PrintStackTraceOnErrorSignal(argv[0]);
  PrettyStackTraceProgram X(argc, argv);
  llvm_shutdown_obj Y;  // Call llvm_shutdown() on exit.

  // Initialize targets and assembly printers/parsers.
  llvm::InitializeAllTargetInfos();
  llvm::InitializeAllTargetMCs();
  llvm::InitializeAllDisassemblers();

  // Register the target printer for --version.
  cl::AddExtraVersionPrinter(TargetRegistry::printRegisteredTargetsForVersion);

  cl::ParseCommandLineOptions(argc, argv, "llvm object file dumper\n");
  TripleName = Triple::normalize(TripleName);

  ToolName = argv[0];

  // Defaults to a.out if no filenames specified.
  if (InputFilenames.size() == 0)
    InputFilenames.push_back("a.out");

  std::for_each(InputFilenames.begin(), InputFilenames.end(),
                DumpInput);

  return EXIT_SUCCESS;
}

