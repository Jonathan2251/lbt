//===--- Cpu0.cpp - Implement Cpu0 target feature support -----------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// This file implements Cpu0 TargetInfo objects.
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

static constexpr llvm::StringLiteral ValidCPUNames[] = {
    {"cpu032I"},  {"cpu032II"}};

bool Cpu0TargetInfo::isValidCPUName(StringRef Name) const {
  return llvm::find(ValidCPUNames, Name) != std::end(ValidCPUNames);
}

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

  Builder.defineMacro("__REGISTER_PREFIX__", "");

  switch (FloatABI) {
  case HardFloat:
    llvm_unreachable("HardFloat is not support in Cpu0");
    break;
  case SoftFloat:
    Builder.defineMacro("__cpu0_soft_float", Twine(1));
    break;
  }
}

bool Cpu0TargetInfo::hasFeature(StringRef Feature) const {
  return llvm::StringSwitch<bool>(Feature)
      .Case("cpu0", true)
      .Default(false);
}

ArrayRef<Builtin::Info> Cpu0TargetInfo::getTargetBuiltins() const {
  return llvm::makeArrayRef(BuiltinInfo, clang::Cpu0::LastTSBuiltin -
                                             Builtin::FirstTSBuiltin);
}

unsigned Cpu0TargetInfo::getUnwindWordWidth() const {
  return llvm::StringSwitch<unsigned>(ABI)
      .Cases("o32", "s32", 32)
      .Default(getPointerWidth(0));
}

bool Cpu0TargetInfo::validateTarget(DiagnosticsEngine &Diags) const {
  if (CPU != "cpu032I" && CPU != "cpu032II") {
    Diags.Report(diag::err_target_unknown_cpu) << ABI << CPU;
    return false;
  }

  return true;
}
