#ifndef _HARDWATER_CHUNK_HPP
#define _HARDWATER_CHUNK_HPP
#include "fragment_hash.hpp"
#include "mapped_file.hpp"
#include "ion_key.hpp"
#include <vector>
#include <cstdint>
#include <memory>

namespace Hardwater {
    class Chunk {
    public:
        /**
         * Create a chunk from a given slice of a plaintext file.
         */
        Chunk(MappedFile::iterator begin,
              MappedFile::iterator end,
              IonKey k,
              uint16_t idx);
    protected:
        uint16_t index;
        
    };
}

#endif
