#include "chunk.hpp"

namespace Hardwater {
    
    Chunk::Chunk(MappedFile::iterator begin,
                 MappedFile::iterator end,
                 IonKey k,
                 uint16_t idx) :
    index(idx)
    {
        
    }
}
