#ifndef _HARDWATER_ION_HPP
#define _HARDWATER_ION_HPP
#include <string>
#include <vector>
#include <memory>
#include "fragment_hash.hpp"
#include "mapped_file.hpp"
#include "chunk.hpp"
#include "key.hpp"
#include "ion_key.hpp"


namespace Hardwater {
    class Ion {
    public:
        Ion(MappedFile &mf, Key& k);
        
        size_t chunkSize();
        
    protected:
        
        std::tuple<MappedFile::iterator, MappedFile::iterator>
        chunkBoundaries(MappedFile& f, int num);
        
        
        size_t fileSize;
        
        size_t determineChunkNumber(size_t size);
        
        size_t numChunks;
        
        std::vector<Chunk> chunks;
    };
}
#endif
