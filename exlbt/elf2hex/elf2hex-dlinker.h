#ifdef DLINK

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

int Cpu0DynFunIndex::findPltName(const char* pltName) {
  for (int i = 0; i < exePltNameSize; i++)
    if (strcmp(pltName, exePltName[i]) == 0)
      return i;
  return -1;
}

void Cpu0DynFunIndex::createPltName(const ObjectFile *Obj) {
  std::error_code ec;
  std::string Error;

  for (const SectionRef &Section : Obj->sections()) {
    if (error(ec)) return;
    StringRef Name;
    StringRef Contents;
    uint64_t BaseAddr;
    bool BSS;
    if (error(Section.getName(Name))) continue;
    if (error(Section.getContents(Contents))) continue;
    BaseAddr = Section.getAddress();
    BSS = Section.isBSS();

    if (Name == ".strtab") {
      int num_dyn_entry = 0;
      FILE *fd_num_dyn_entry;
      fd_num_dyn_entry = fopen("dlconfig/num_dyn_entry", "r");
      if (fd_num_dyn_entry != NULL) {
        fscanf(fd_num_dyn_entry, "%d", &num_dyn_entry);
      }
      fclose(fd_num_dyn_entry);

      for (std::size_t addr = 2+strlen(".PLT0"), end = Contents.size(); 
           addr < end; ) {
        if (Contents.substr(addr, strlen("__plt_")) != "__plt_")
          break;
        strcpy(exePltName[exePltNameSize], Contents.data()+addr);
        addr = addr + strlen(exePltName[exePltNameSize]) + 1;
        exePltNameSize++;
      }
      break;
    }
  }
}

void Cpu0DynFunIndex::createStrtab() {
  FILE *fd_dynstrAscii;

  fd_dynstrAscii = fopen("dlconfig/dynstrAscii", "r");
  if (fd_dynstrAscii == NULL)
    fclose(fd_dynstrAscii);
  assert(fd_dynstrAscii != NULL && "fd_dynstr == NULL");
  int i = 0;
  // function                  result on EOF or error                    
  // --------                  ----------------------
  // fgets()                   NULL
  // fscanf()                  number of succesful conversions
  //                             less than expected
  // fgetc()                   EOF
  // fread()                   number of elements read
  //                             less than expected
  int j = 0;
  for (i=0; 1; i++) {
    j=fscanf(fd_dynstrAscii, "%s", soStrtab[i]);
    if (j != 1)
      break;
  }
  soStrtabSize = i;
  fclose(fd_dynstrAscii);
}

uint16_t Cpu0DynFunIndex::correctDynFunIndex(const char* pltName) {
  int i = findPltName(pltName);
  if (i != -1) {
    int j = 0;
    for (j=0; j < soStrtabSize; j++)
      if (strcmp(soStrtab[j], (const char*)exePltName[i]+strlen("__plt_")) == 0)
        break;
    if (j == soStrtabSize) {
      outs() << "cannot find " << exePltName[i] << "\n";
      exit(1);
    }
    j++;
    return (uint16_t)(j & 0xffff);
  }
  return (uint16_t)0;
}

Cpu0DynFunIndex cpu0DynFunIndex;

static void DisassembleSoInHexFormat(const ObjectFile *Obj
/*, bool InlineRelocs*/  , std::unique_ptr<MCDisassembler>& DisAsm, 
  std::unique_ptr<MCInstPrinter>& IP, uint64_t& lastDumpAddr,
  std::unique_ptr<const MCSubtargetInfo>& STI) {
  std::string Error;
  uint64_t soLastPrintAddr = 0;
  FILE *fd_so_func_offset;
  int num_dyn_entry = 0;
  
  fd_so_func_offset = fopen("dlconfig/so_func_offset", "w");
  if (fd_so_func_offset == NULL)
    fclose(fd_so_func_offset);
  assert(fd_so_func_offset != NULL && "fd_so_func_offset == NULL");

#ifdef ELF2HEX_DEBUG
  errs() << format("!lastDumpAddr %8" PRIx64 "\n", lastDumpAddr);
#endif
  std::error_code ec;
  for (const SectionRef &Section : Obj->sections()) {
    if (error(ec)) break;
    StringRef Name;
    StringRef Contents;
    uint64_t BaseAddr;
    if (error(Section.getName(Name))) continue;
    if (error(Section.getContents(Contents))) continue;
    BaseAddr = Section.getAddress();
  #ifdef ELF2HEX_DEBUG
    errs() << "Name " << Name << format("  BaseAddr %8" PRIx64 "\n", BaseAddr);
    errs() << format("!!lastDumpAddr %8" PRIx64 "\n", lastDumpAddr);
  #endif
    bool text;
    text = Section.isText();
    if (!text) {
      continue;
    }
    // It's .text section
    uint64_t SectionAddr;
    SectionAddr = Section.getAddress();

    // Make a list of all the symbols in this section.
    std::vector<std::pair<uint64_t, StringRef> > Symbols;
    for (const SymbolRef &Symbol : Obj->symbols()) {
      if (Section.containsSymbol(Symbol)) {
        ErrorOr<uint64_t> AddressOrErr = Symbol.getAddress();
        if (error(AddressOrErr.getError()))
          break;
        uint64_t Address = *AddressOrErr;
        Address -= SectionAddr;

        ErrorOr<StringRef> Name = Symbol.getName();
        if (error(Name.getError()))
          break;
        Symbols.push_back(std::make_pair(Address, *Name));
      }
    }

    // Sort the symbols by address, just in case they didn't come in that way.
    array_pod_sort(Symbols.begin(), Symbols.end());

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
    if (error(Section.getName(name))) break;
    if (name == ".plt") continue;
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
    if (error(Section.getContents(BytesStr))) break;
    ArrayRef<uint8_t> Bytes(reinterpret_cast<const uint8_t *>(BytesStr.data()),
                            BytesStr.size());
    uint64_t Size;
    uint64_t Index;
    uint64_t SectSize;
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
        // This symbol has the same address as the next symbol. Skip it.
        fprintf(fd_so_func_offset, "%02x ", 
                (uint8_t)(Symbols[si].first >> 24));
        fprintf(fd_so_func_offset, "%02x ", 
                (uint8_t)((Symbols[si].first >> 16) & 0xFF));
        fprintf(fd_so_func_offset, "%02x ", 
                (uint8_t)((Symbols[si].first >> 8) & 0xFF));
        fprintf(fd_so_func_offset, "%02x    ", 
                (uint8_t)((Symbols[si].first) & 0xFF));
        std::string str = Symbols[si].second.str();
        std::size_t idx = 0;
        std::size_t strSize = 0;
        for (idx = 0, strSize = str.size(); idx < strSize; idx++) {
          fprintf(fd_so_func_offset, "%c%c ", 
                  hexdigit((str[idx] >> 4) & 0xF, true),
                  hexdigit(str[idx] & 0xF, true));
        }
        for (idx = strSize; idx < 48; idx++) {
          fprintf(fd_so_func_offset, "%02x ", 0);
        }
        fprintf(fd_so_func_offset, "/* %s */\n", Symbols[si].second.begin());
        num_dyn_entry++;

        outs() << '\n' << "/*" << Symbols[si].second << ":*/\n";
        continue;
      }

      soLastPrintAddr = Symbols[si].first;
      fprintf(fd_so_func_offset, "%02x ", (uint8_t)(Symbols[si].first >> 24));
      fprintf(fd_so_func_offset, "%02x ", 
              (uint8_t)((Symbols[si].first >> 16) & 0xFF));
      fprintf(fd_so_func_offset, "%02x ", 
              (uint8_t)((Symbols[si].first >> 8) & 0xFF));
      fprintf(fd_so_func_offset, "%02x    ", 
              (uint8_t)((Symbols[si].first) & 0xFF));
      std::string str = Symbols[si].second.str();
      std::size_t idx = 0;
      std::size_t strSize = 0;
      for (idx = 0, strSize = str.size(); idx < strSize; idx++) {
        fprintf(fd_so_func_offset, "%c%c ", 
                hexdigit((str[idx] >> 4) & 0xF, true), 
                hexdigit(str[idx] & 0xF, true));
      }
      for (idx = strSize; idx < 48; idx++) {
        fprintf(fd_so_func_offset, "%02x ", 0);
      }
      fprintf(fd_so_func_offset, "/* %s */\n", Symbols[si].second.begin());
      num_dyn_entry++;

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
          outs() << format("/*%8" PRIx64 ":*/", Index);
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

      #ifdef ELF2HEX_DEBUG
        errs() << "Size = " << Size <<  " Index = " << Index << " lastDumpAddr = "
               << lastDumpAddr << "\n"; // debug
      #endif
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
          if (error(getRelocationValueString(*rel_cur, val)))
            goto skip_print_rel;

          outs() << format("\t\t\t/*%8" PRIx64 ": ", SectionAddr + addr) << name
                 << "\t" << val << "*/\n";

        skip_print_rel:
          ++rel_cur;
        }
      }
      soLastPrintAddr = End;
      lastDumpAddr += Index;
    #ifdef ELF2HEX_DEBUG
      errs() << format("SectionAddr + Index = %8" PRIx64 "\n", SectionAddr + Index);
      errs() << format("lastDumpAddr %8" PRIx64 "\n", lastDumpAddr);
    #endif
    }
  }
// Dump share obj or lib
// Fix the issue that __tls_get_addr appear as file offset 0.
// Old lld version the __tls_get_addr appear at the last function name.
  std::pair<uint64_t, StringRef> dummy(soLastPrintAddr, "dummy");
  fprintf(fd_so_func_offset, "%02x ", (uint8_t)(dummy.first >> 24));
  fprintf(fd_so_func_offset, "%02x ", (uint8_t)((dummy.first >> 16) & 0xFF));
  fprintf(fd_so_func_offset, "%02x ", (uint8_t)((dummy.first >> 8) & 0xFF));
  fprintf(fd_so_func_offset, "%02x    ", (uint8_t)((dummy.first) & 0xFF));
  std::string str = dummy.second.str();
  std::size_t idx = 0;
  std::size_t strSize = 0;
  for (idx = 0, strSize = str.size(); idx < strSize; idx++) {
    fprintf(fd_so_func_offset, "%c%c ", hexdigit((str[idx] >> 4) & 0xF, true)
            , hexdigit(str[idx] & 0xF, true));
  }
  for (idx = strSize; idx < 48; idx++) {
    fprintf(fd_so_func_offset, "%02x ", 0);
  }
  fprintf(fd_so_func_offset, "/* %s */\n", dummy.second.begin());
  num_dyn_entry++;
  outs() << '\n' << "/*" << dummy.second << ":*/\n";
  FILE *fd_num_dyn_entry;
  fd_num_dyn_entry = fopen("dlconfig/num_dyn_entry", "w");
  if (fd_num_dyn_entry != NULL) {
    fprintf(fd_num_dyn_entry, "%d\n", num_dyn_entry);
  }
  fclose(fd_num_dyn_entry);
}

static void PrintSoDataSections(const ObjectFile *o, uint64_t& lastDumpAddr, 
                                bool isLittleEndian) {
  std::error_code ec;

  for (const SectionRef &Section : o->sections()) {
    if (error(ec)) return;
    StringRef Name;
    StringRef Contents;
    uint64_t BaseAddr;
    bool BSS;
    if (error(Section.getName(Name))) continue;
    if (error(Section.getContents(Contents))) continue;
    BaseAddr = Section.getAddress();
    BSS = Section.isBSS();
    if (Name == ".dynsym") {
      int num_dyn_entry = 0;
      FILE *fd_num_dyn_entry;
      fd_num_dyn_entry = fopen("dlconfig/num_dyn_entry", "r");
      if (fd_num_dyn_entry != NULL) {
        fscanf(fd_num_dyn_entry, "%d", &num_dyn_entry);
      }
      fclose(fd_num_dyn_entry);
      raw_fd_ostream fd_dynsym("dlconfig/dynsym", ec, sys::fs::F_Text);
      int count = 0;
      for (std::size_t addr = 0, end = Contents.size(); addr < end; addr += 16) {
        if (isLittleEndian) {
          fd_dynsym << hexdigit((Contents[addr+3] >> 4) & 0xF, true)
                     << hexdigit(Contents[addr+3] & 0xF, true) << " ";
          fd_dynsym << hexdigit((Contents[addr+2] >> 4) & 0xF, true)
                     << hexdigit(Contents[addr+2] & 0xF, true) << " ";
          fd_dynsym << hexdigit((Contents[addr+1] >> 4) & 0xF, true)
                     << hexdigit(Contents[addr+1] & 0xF, true) << " ";
          fd_dynsym << hexdigit((Contents[addr] >> 4) & 0xF, true)
                     << hexdigit(Contents[addr] & 0xF, true) << " ";
        }
        else {
          fd_dynsym << hexdigit((Contents[addr] >> 4) & 0xF, true)
                     << hexdigit(Contents[addr] & 0xF, true) << " ";
          fd_dynsym << hexdigit((Contents[addr+1] >> 4) & 0xF, true)
                     << hexdigit(Contents[addr+1] & 0xF, true) << " ";
          fd_dynsym << hexdigit((Contents[addr+2] >> 4) & 0xF, true)
                     << hexdigit(Contents[addr+2] & 0xF, true) << " ";
          fd_dynsym << hexdigit((Contents[addr+3] >> 4) & 0xF, true)
                     << hexdigit(Contents[addr+3] & 0xF, true) << " ";
        }
        count++;
      }
      for (int i = count; i < num_dyn_entry; i++) {
        fd_dynsym << "00 00 00 00 ";
      }
    }
    else if (Name == ".dynstr") {
      raw_fd_ostream fd_dynstr("dlconfig/dynstr", ec, sys::fs::F_Text);
      raw_fd_ostream fd_dynstrAscii("dlconfig/dynstrAscii", ec, 
                                    sys::fs::F_Text);
      for (std::size_t addr = 0, end = Contents.size(); addr < end; addr++) {
        fd_dynstr << hexdigit((Contents[addr] >> 4) & 0xF, true)
                   << hexdigit(Contents[addr] & 0xF, true) << " ";
        if (addr == 0)
          continue;
        if (Contents[addr] == '\0')
          fd_dynstrAscii << "\n";
        else
          fd_dynstrAscii << Contents[addr];
      }
    }
  }
}

#endif // DLINK
