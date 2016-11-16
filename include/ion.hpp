#ifndef _HARDWATER_ION_HPP
#define _HARDWATER_ION_HPP
#include <string>
#include <vector>
#include "fragment_hash.hpp"
#include "mapped_file.hpp"

namespace Hardwater {
    class Ion {
    public:
        Ion(std::unique_ptr<MappedFile>&& f);
        inline const MappedFile& getFile() { return *file; }
        
        inline ssize_t getChunkSize() const { return chunkSize; }
        
        inline size_t getNumChunks() const {
            return (file->getSize() + chunkSize - 1) / chunkSize;
        }
        
        inline FragmentHash getOverallHash() const {
            return *overallHash;
        }
        
        inline const std::vector<FragmentHash>& getChunks() const { return chunks; }
        
        
        void generate();
        
    protected:
        std::unique_ptr<MappedFile> file;
        ssize_t chunkSize = -1;
    private:
        void determineChunkSize();
        std::vector<FragmentHash> chunks;
        std::unique_ptr<FragmentHash> overallHash;
    };
}
#endif
