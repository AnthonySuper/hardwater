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
        
        struct EncryptedIndex {
            EncryptedIndex(size_t idx, IonKey& k);
            std::vector<uint8_t> encrypted;
        };
        
    public:
        static Chunk generate(MappedFile::iterator begin,
                              MappedFile::iterator end,
                              IonKey& key,
                              size_t index);
        
        static Chunk read(std::vector<uint8_t>::iterator &it);
        
        Chunk(size_t size, EncryptedIndex index, FragmentHash hash);
        
        void writeTo(std::vector<uint8_t> &buff);
        
    protected:
        size_t size;
        FragmentHash hash;
        EncryptedIndex index;
    };
}

#endif
