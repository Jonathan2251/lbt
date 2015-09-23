
#include "Cpu0ExecutableWriter.h"

namespace lld {
namespace elf {

template <class ELFT>
Cpu0ExecutableWriter<ELFT>::
Cpu0ExecutableWriter(Cpu0LinkingContext &ctx, Cpu0TargetLayout<ELFT> &layout)
    : ExecutableWriter<ELFT>(ctx, layout), _targetLayout(layout) {}

template class Cpu0ExecutableWriter<ELF32BE>;
template class Cpu0ExecutableWriter<ELF32LE>;

}
}
