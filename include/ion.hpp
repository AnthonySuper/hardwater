#ifndef _HARDWATER_ION_HPP
#define _HARDWATER_ION_HPP
#include <string>
#include <vector>
#include <memory>
#include <future>
#include "fragment_hash.hpp"
#include "mapped_file.hpp"
#include "chunk.hpp"
#include "key.hpp"
#include "ion_key.hpp"
#include "encrypted_ion_key.hpp"
#include "util.hpp"


namespace Hardwater {
    class Ion {
    public:
        static Ion generate(MappedFile &mf, Key& pub);
        
        size_t chunkSize();
        
        Ion(size_t fileSize,
            size_t numChunks,
            FragmentHash hash,
            std::vector<Chunk>&& chunks);
        
        
    protected:
        FragmentHash hash;
        
        size_t fileSize;
        
        size_t numChunks;
        
        std::vector<Chunk> chunks;
        
        
    };
}
#endif
