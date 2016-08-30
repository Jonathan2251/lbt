#ifdef DLINK

#ifndef LLVM_TOOLS_ELF2HEX_DLINKER_ELF2HEX_DLINKER_H
#define LLVM_TOOLS_ELF2HEX_DLINKER_ELF2HEX_DLINKER_H

class Cpu0DynFunIndex {
private:
  char soStrtab[20][100];
  int soStrtabSize = 0;

  char exePltName[20][100];
  int exePltNameSize = 0;

  int findPltName(const char* pltName);
public:
  void createPltName(const ObjectFile *o);
  void createStrtab();
  uint16_t correctDynFunIndex(const char* pltName);
};

extern Cpu0DynFunIndex cpu0DynFunIndex;
extern cl::opt<bool> DumpSo;
extern cl::opt<bool> LinkSo;
extern void OutputDlinkerConfig(const ObjectFile *o, bool isLittleEndian, 
StringRef ToolName);

#endif

#endif // DLINK
