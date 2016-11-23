#include "ion.hpp"

namespace Hardwater {
    Ion::Ion(MappedFile &mf, Key& k)
    {
        numChunks = determineChunkNumber(mf.getSize());
        fileSize = mf.getSize();
        
    }
    
    size_t Ion::chunkSize()
    {
        if(fileSize == 0 || numChunks == 0) {
            throw std::runtime_error("Cannot determine chunk size!");
        }
        return fileSize / numChunks;
    }
    
    size_t Ion::determineChunkNumber(size_t size)
    {
        constexpr size_t kilobyte = 1024;
        constexpr size_t megabyte = 1024 * kilobyte;
        constexpr size_t gigabyte = 1024 * megabyte;
        if(size < kilobyte) {
            return 1;
        }
        else if(size < megabyte) {
            return 128;
        }
        else if(size < gigabyte) {
            return 1024;
        }
        else {
            return 2048;
        }
    }
    
    std::tuple<MappedFile::iterator, MappedFile::iterator>
    Ion::chunkBoundaries(MappedFile& f, int num)
    {
        auto b = f.begin() + (num * chunkSize());
        if(num == numChunks - 1) {
            return std::make_tuple(b, f.end());
        }
        else {
            return std::make_tuple(b, b + chunkSize());
        }
    }
}
