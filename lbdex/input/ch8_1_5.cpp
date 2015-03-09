// clang -target mips-unknown-linux-gnu -c ch8_1_5.cpp -emit-llvm -o ch8_1_5.bc
// /usr/local/llvm/test/cmake_debug_build/bin/llc -march=cpu0 -relocation-model=pic -filetype=asm ch8_1_5.bc -o -
// /usr/local/llvm/test/cmake_debug_build/bin/llc -march=cpu0 -relocation-model=static -filetype=asm ch8_1_5.bc -o -
// /usr/local/llvm/test/cmake_debug_build/bin/llc -march=cpu0 -relocation-model=pic -filetype=obj ch8_1_5.bc -o ch8_1_5.cpu0.o
// /usr/local/llvm/test/cmake_debug_build/bin/llc -march=cpu0 -relocation-model=static -filetype=obj ch8_1_5.bc -o ch8_1_5.cpu0.o

// /Users/Jonathan/llvm/test/cmake_debug_build/Debug/bin/llc -march=cpu0 -relocation-model=pic -filetype=asm ch8_1_5.bc -o -

/// start
extern "C" int printf(const char *format, ...);
extern "C" int sprintf(char *out, const char *format, ...);

typedef unsigned char Byte;
typedef unsigned char *Address;
typedef enum {FALSE=0, TRUE=1} Boolean;

unsigned char sBuffer[4] = {0x35, 0x35, 0x35, 0x00};

Boolean test_ctrl2()
{
    Boolean Result = FALSE;
    Byte Comparator = sBuffer[1];
    Byte ByteToCompare = sBuffer[0];

    switch ( sBuffer[0] )
    {
        case 0x30:
            if ( ByteToCompare == Comparator )
                Result = TRUE;
            break;
        case 0x31:
            if ( ByteToCompare != Comparator )
                Result = TRUE;
            break;
        case 0x32:
            if ( ByteToCompare > Comparator )
                Result = TRUE;
            break;
        case 0x33:
            if ( ByteToCompare < Comparator )
                Result = TRUE;
            break;
        case 0x34:
            if ( ByteToCompare >= Comparator )
                Result = TRUE;
            break;
        case 0x35:
            if ( ByteToCompare <= Comparator )
                Result = TRUE;
            break;
    }
    return Result;
}

