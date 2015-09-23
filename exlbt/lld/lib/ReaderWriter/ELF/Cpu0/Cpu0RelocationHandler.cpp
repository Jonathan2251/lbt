//===- lib/ReaderWriter/ELF/Cpu0/Cpu0RelocationHandler.cpp ------------===//
//
//                             The LLVM Linker
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

#include "Cpu0LinkingContext.h"
#include "Cpu0TargetHandler.h"
#include "llvm/Support/Endian.h"

using namespace lld;
using namespace lld::elf;
using namespace llvm::support;
using namespace llvm::support::endian;

//template <class ELFT>
/// \brief R_CPU0_HI16 - word64: (S + A) >> 16
void relocHI16(uint8_t *location, uint64_t P, uint64_t S, int64_t A, endianness endian) {
 // Don't know why A, ref.addend(), = 9
  uint32_t result = (uint32_t)(S >> 16);
//  endian::write<uint32_t, ELFT::TargetEndianness, unaligned>(location, result);
  if (endian == big) {
    write32be(location, result | read32be(location));
  }
  else if (endian == little) {
    write32le(location, result | read32le(location));
  }
  else {
    llvm_unreachable("Unexpected size");
  }
}

void relocLO16(uint8_t *location, uint64_t P, uint64_t S, uint64_t A, endianness endian) {
 // Don't know why A, ref.addend(), = 9
  uint32_t result = (uint32_t)(S & 0x0000ffff);
  if (endian == big) {
    write32be(location, result | read32be(location));
  }
  else if (endian == little) {
    write32le(location, result | read32le(location));
  }
  else {
    llvm_unreachable("Unexpected size");
  }
}

#if 0 // Not support yet
/// \brief R_CPU0_GOT16 - word32: S
void relocGOT16(uint8_t *location, uint64_t P, uint64_t S, int64_t A) {
  uint32_t result = (uint32_t)(S);
  write32be(location, result | read32be(location));
}
#endif

/// \brief R_CPU0_PC24 - word32: S + A - P
void relocPC24(uint8_t *location, uint64_t P, uint64_t S, int64_t A, endianness endian) {
  uint32_t result = (uint32_t)(S  - P);
  if (endian == big) {
    uint32_t machinecode = (uint32_t) * 
                           reinterpret_cast<llvm::support::ubig32_t *>(location);
    uint32_t opcode = (machinecode & 0xff000000);
    uint32_t offset = (machinecode & 0x00ffffff);
    write32be(location, ((result + offset) & 0x00ffffff) | opcode);
  }
  else if (endian == little) {
    uint32_t machinecode = (uint32_t) * 
                           reinterpret_cast<llvm::support::ulittle32_t *>(location);
    uint32_t opcode = (machinecode & 0xff000000);
    uint32_t offset = (machinecode & 0x00ffffff);
    write32le(location, ((result + offset) & 0x00ffffff) | opcode);
  }
  else {
    llvm_unreachable("Unexpected size");
  }
}

/// \brief R_CPU0_32 - word32:  S
void reloc32(uint8_t *location, uint64_t P, uint64_t S, int64_t A, endianness endian) {
  int32_t result = (uint32_t)(S);
  if (endian == big) {
    write32be(location, result | read32be(location));
  }
  else if (endian == little) {
    write32le(location, result | read32be(location));
  }
  else {
    llvm_unreachable("Unexpected size");
  }
  // TODO: Make sure that the result zero extends to the 64bit value.
}

template <class ELFT> std::error_code Cpu0TargetRelocationHandler<ELFT>::applyRelocation(
    ELFWriter &writer, llvm::FileOutputBuffer &buf, const AtomLayout &atom,
    const Reference &ref) const {
#ifdef DLINKER
  static bool firstTime = true;
  std::string soName("libfoobar.cpu0.so");
  int idx = 0;
  if (firstTime) {
    if (this->_ctx.getOutputELFType() == llvm::ELF::ET_DYN) {
      _targetHandler.soPlt.createFunAddr(_layout, buf);
    }
    else if (this->_ctx.getOutputELFType() == llvm::ELF::ET_EXEC && 
             !this->_ctx.isStaticExecutable()) {
      _targetHandler.soPlt.createFunAddr(_layout, buf);
    }
    firstTime = false;
  }
#endif // DLINKER
  uint8_t *atomContent = buf.getBufferStart() + atom._fileOffset;
  uint8_t *loc = atomContent + ref.offsetInAtom();
  uint64_t target = writer.addressOfAtom(ref.target());
  uint64_t reloc = atom._virtualAddr + ref.offsetInAtom();
#ifdef DLINKER
#if 1 // For case R_CPU0_GOT16:
//  auto gotAtomIter = _context.getTargetHandler<Cpu0ELFType>().targetLayout().
//                     findAbsoluteAtom("_GLOBAL_OFFSET_TABLE_");
//  uint64_t globalOffsetTableAddress = writer.addressOfAtom(*gotAtomIter);
// .got.plt start from _GLOBAL_OFFSET_TABLE_
  auto gotpltSection = _layout.findOutputSection(".got.plt");
  uint64_t gotPltFileOffset;
  if (gotpltSection)
    gotPltFileOffset = gotpltSection->fileOffset();
  else
    gotPltFileOffset = 0;
#endif
#endif // DLINKER

  if (ref.kindNamespace() != Reference::KindNamespace::ELF)
    return std::error_code();
  assert(ref.kindArch() == Reference::KindArch::Cpu0);
  switch (ref.kindValue()) {
  case R_CPU0_NONE:
    break;
  case R_CPU0_HI16:
    relocHI16(loc, reloc, target, ref.addend(), _endian);
    break;
  case R_CPU0_LO16:
    relocLO16(loc, reloc, target, ref.addend(), _endian);
    break;
#if 0 // Not support yet
  case R_CPU0_GOT16:
#if 1
    idx = _targetHandler.soPlt.getDynFunIndexByTargetAddr(targetVAddress);
    relocGOT16(location, reloc, idx, ref.addend(), _endian);
#else
    relocGOT16(location, reloc, (target - gotPltFileOffset),
               ref.addend());
#endif
    break;
#endif
  case R_CPU0_PC24:
    relocPC24(loc, reloc, target, ref.addend(), _endian);
    break;
#ifdef DLINKER
  case R_CPU0_CALL16:
  // offset at _GLOBAL_OFFSET_TABLE_ and $gp point to _GLOBAL_OFFSET_TABLE_.
    idx = _targetHandler.soPlt.getDynFunIndexByTargetAddr(target);
    reloc32(loc, reloc, idx*0x04+16, ref.addend(), _endian);
    break;
#endif // DLINKER
  case R_CPU0_32:
    reloc32(loc, reloc, target, ref.addend(), _endian);
    break;
  // Runtime only relocations. Ignore here.
  case R_CPU0_JUMP_SLOT:
  case R_CPU0_GLOB_DAT:
    break;
  default:
    return make_unhandled_reloc_error();
  }

  return std::error_code();
}

namespace lld {
namespace elf {

template class Cpu0TargetRelocationHandler<ELF32BE>;
template class Cpu0TargetRelocationHandler<ELF32LE>;

} // end namespace elf
} // end namespace lld

