//===--- Mips.cpp - Implement Mips target feature support -----------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// This file implements Mips TargetInfo objects.
//
//===----------------------------------------------------------------------===//

#include "Cpu0.h"
#include "Targets.h"
#include "clang/Basic/Diagnostic.h"
#include "clang/Basic/MacroBuilder.h"
#include "clang/Basic/TargetBuiltins.h"
#include "llvm/ADT/StringSwitch.h"

using namespace clang;
using namespace clang::targets;

const Builtin::Info Cpu0TargetInfo::BuiltinInfo[] = {
#define BUILTIN(ID, TYPE, ATTRS)                                               \
  {#ID, TYPE, ATTRS, nullptr, ALL_LANGUAGES, nullptr},
#define LIBBUILTIN(ID, TYPE, ATTRS, HEADER)                                    \
  {#ID, TYPE, ATTRS, HEADER, ALL_LANGUAGES, nullptr},
#include "clang/Basic/BuiltinsCpu0.def"
};

/*
bool MipsTargetInfo::processorSupportsGPR64() const {
  return llvm::StringSwitch<bool>(CPU)
      .Case("cpu032", true)
      .Case("mips4", true)
      .Case("mips5", true)
      .Case("mips64", true)
      .Case("mips64r2", true)
      .Case("mips64r3", true)
      .Case("mips64r5", true)
      .Case("mips64r6", true)
      .Case("octeon", true)
      .Case("octeon+", true)
      .Default(false);
}

static constexpr llvm::StringLiteral ValidCPUNames[] = {
    {"mips1"},  {"mips2"},    {"mips3"},    {"mips4"},    {"mips5"},
    {"mips32"}, {"mips32r2"}, {"mips32r3"}, {"mips32r5"}, {"mips32r6"},
    {"mips64"}, {"mips64r2"}, {"mips64r3"}, {"mips64r5"}, {"mips64r6"},
    {"octeon"}, {"octeon+"}, {"p5600"}};

bool MipsTargetInfo::isValidCPUName(StringRef Name) const {
  return llvm::find(ValidCPUNames, Name) != std::end(ValidCPUNames);
}

void MipsTargetInfo::fillValidCPUList(
    SmallVectorImpl<StringRef> &Values) const {
  Values.append(std::begin(ValidCPUNames), std::end(ValidCPUNames));
}*/

unsigned Cpu0TargetInfo::getISARev() const {
  return llvm::StringSwitch<unsigned>(getCPU())
             .Case("cpu032I", 1)
             .Case("cpu032II", 2)
             .Default(0);
}

void Cpu0TargetInfo::getTargetDefines(const LangOptions &Opts,
                                      MacroBuilder &Builder) const {
  if (BigEndian) {
    DefineStd(Builder, "CPU0EB", Opts);
    Builder.defineMacro("_CPU0EB");
  } else {
    DefineStd(Builder, "CPU0EL", Opts);
    Builder.defineMacro("_CPU0EL");
  }

  Builder.defineMacro("__cpu0__");
  Builder.defineMacro("_cpu0");
  if (Opts.GNUMode)
    Builder.defineMacro("cpu0");

  if (ABI == "o32" || ABI == "s32") {
    Builder.defineMacro("__cpu0", "32");
    Builder.defineMacro("_CPU0_ISA", "_CPU0_ISA_CPU032");
  } else {
    llvm_unreachable("Invalid ABI.");
  }

  const std::string ISARev = std::to_string(getISARev());

  if (!ISARev.empty())
    Builder.defineMacro("__cpu0_isa_rev", ISARev);

  if (ABI == "o32") {
    Builder.defineMacro("__cpu0_o32");
    Builder.defineMacro("_ABIO32", "1");
    Builder.defineMacro("_CPU0_SIM", "_ABIO32");
  } else if (ABI == "s32") {
    Builder.defineMacro("__cpu0_n32");
    Builder.defineMacro("_ABIS32", "2");
    Builder.defineMacro("_CPU0_SIM", "_ABIN32");
  } else
    llvm_unreachable("Invalid ABI.");

  if (!IsNoABICalls) {
    Builder.defineMacro("__cpu0_abicalls");
    if (CanUseBSDABICalls)
      Builder.defineMacro("__ABICALLS__");
  }

  Builder.defineMacro("__REGISTER_PREFIX__", "");

  switch (FloatABI) {
  case HardFloat:
    //Builder.defineMacro("__cpu0_hard_float", Twine(1));
    llvm_unreachable("HardFloat is not support in Cpu0");
    break;
  case SoftFloat:
    Builder.defineMacro("__cpu0_soft_float", Twine(1));
    break;
  }
}

/*bool MipsTargetInfo::hasFeature(StringRef Feature) const {
  return llvm::StringSwitch<bool>(Feature)
      .Case("mips", true)
      .Case("dsp", DspRev >= DSP1)
      .Case("dspr2", DspRev >= DSP2)
      .Case("fp64", FPMode == FP64)
      .Case("msa", HasMSA)
      .Default(false);
}*/

ArrayRef<Builtin::Info> Cpu0TargetInfo::getTargetBuiltins() const {
  return llvm::makeArrayRef(BuiltinInfo, clang::Cpu0::LastTSBuiltin -
                                             Builtin::FirstTSBuiltin);
}
/*
unsigned MipsTargetInfo::getUnwindWordWidth() const {
  return llvm::StringSwitch<unsigned>(ABI)
      .Case("o32", 32)
      .Case("n32", 64)
      .Case("n64", 64)
      .Default(getPointerWidth(0));
}*/

bool Cpu0TargetInfo::validateTarget(DiagnosticsEngine &Diags) const {
  if (CPU != "cpu032I" && CPU != "cpu032II") {
    Diags.Report(diag::err_target_unsupported_cpu) << ABI << CPU;
    return false;
  }

  return true;
}
