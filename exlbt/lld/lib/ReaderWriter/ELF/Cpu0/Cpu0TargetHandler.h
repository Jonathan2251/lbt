//===- lib/ReaderWriter/ELF/Cpu0/Cpu0TargetHandler.h ------------------===//
//
//                             The LLVM Linker
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

#ifndef LLD_READER_WRITER_ELF_CPU0_CPU0_TARGET_HANDLER_H
#define LLD_READER_WRITER_ELF_CPU0_CPU0_TARGET_HANDLER_H

#include "ELFReader.h"
#include "TargetLayout.h"
#include "Cpu0LinkingContext.h"
#include "Cpu0RelocationHandler.h"
#include "Cpu0SectionChunks.h"
#include "lld/Core/Simple.h"
#include "llvm/Support/Endian.h"

using namespace llvm::support;

#define DLINKER

namespace lld {
namespace elf {

#ifdef DLINKER
template <class ELFT> 
class Cpu0SoPlt {
private:
  uint32_t funAddr[100];
  int funAddrSize = 0;
public:
  void createFunAddr(Cpu0TargetLayout<ELFT> &cpu0Layout,
                     llvm::FileOutputBuffer &buf);
  // Return function index, 1: 1st function appear on section .text of .so.
  //   2: 2nd function ...
  // For example: 3 functions _Z2laii, _Z3fooii and _Z3barv. 1: is _Z2laii 
  //   2 is _Z3fooii, 3: is _Z3barv.
  int getDynFunIndexByTargetAddr(uint64_t fAddr);
};
#endif

template <class ELFT> class Cpu0TargetLayout : public TargetLayout<ELFT> {
public:
  Cpu0TargetLayout(Cpu0LinkingContext &ctx) : TargetLayout<ELFT>(ctx),
    _gotSection(new (this->_allocator) Cpu0GOTSection<ELFT>(ctx)) {}

  AtomSection<ELFT> *
  createSection(StringRef name, int32_t type,
                DefinedAtom::ContentPermissions permissions,
                typename TargetLayout<ELFT>::SectionOrder order) override {
    if (type == DefinedAtom::typeGOT && name == ".got")
      return _gotSection;
    return TargetLayout<ELFT>::createSection(name, type, permissions, order);
  }

  void finalizeOutputSectionLayout() override {
    sortOutputSectionByPriority<ELFT>(".init_array");
    sortOutputSectionByPriority<ELFT>(".fini_array");
  }

 const Cpu0GOTSection<ELFT> &getGOTSection() const { return *_gotSection; }

private:
  uint32_t getPriority(StringRef sectionName) const {
    StringRef priority = sectionName.drop_front().rsplit('.').second;
    uint32_t prio;
    if (priority.getAsInteger(10, prio))
      return std::numeric_limits<uint32_t>::max();
    return prio;
  }

  template <typename T> void sortOutputSectionByPriority(StringRef prefix) {
    OutputSection<T> *section = Cpu0TargetLayout<ELFT>::findOutputSection(prefix);
    if (!section)
      return;
    auto sections = section->sections();
    std::sort(sections.begin(), sections.end(),
              [&](Chunk<T> *lhs, Chunk<T> *rhs) {
                Section<T> *lhsSection = dyn_cast<Section<T>>(lhs);
                Section<T> *rhsSection = dyn_cast<Section<T>>(rhs);
                if (!lhsSection || !rhsSection)
                  return false;
                StringRef lhsName = lhsSection->inputSectionName();
                StringRef rhsName = rhsSection->inputSectionName();
                if (!lhsName.startswith(prefix) || !rhsName.startswith(prefix))
                  return false;
                return getPriority(lhsName) < getPriority(rhsName);
              });
  }

private:
  Cpu0GOTSection<ELFT> *_gotSection;
};

template <class ELFT> class Cpu0TargetHandler : public TargetHandler {
public:
  Cpu0TargetHandler(Cpu0LinkingContext &ctx, endianness endian);

  const TargetRelocationHandler &getRelocationHandler() const override {
    return *_relocationHandler;
  }

  std::unique_ptr<Reader> getObjReader() override {
    return llvm::make_unique<ELFReader<ELFFile<ELFT>>>(_ctx);
  }

  std::unique_ptr<Reader> getDSOReader() override {
    return llvm::make_unique<ELFReader<DynamicFile<ELFT>>>(_ctx);
  }

  std::unique_ptr<Writer> getWriter() override;

#ifdef DLINKER
  Cpu0SoPlt<ELFT> soPlt;
#endif
protected:
  Cpu0LinkingContext &_ctx;
  std::unique_ptr<Cpu0TargetLayout<ELFT>> _targetLayout;
  std::unique_ptr<Cpu0TargetRelocationHandler<ELFT>> _relocationHandler;
};

} // end namespace elf
} // end namespace lld

#endif
