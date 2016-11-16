#ifndef _HARDWATER_ION_HPP
#define _HARDWATER_ION_HPP
#include <string>
#include <vector>
#include "fragment_hash.hpp"
#include "mapped_file.hpp"
#include "chunk.hpp"

namespace Hardwater {
    class Ion {
    public:
        Ion(std::unique_ptr<MappedFile>&& f);
        Ion();
        inline const MappedFile& getFile() { return *file; }
        
        inline ssize_t getChunkSize() const { return chunkSize; }
        
        inline size_t getNumChunks() const {
            return (file->getSize() + chunkSize - 1) / chunkSize;
        }
        
        inline FragmentHash getOverallHash() const
        {
            return *overallHash;
        }
        
        inline const std::vector<Chunk>& getChunks() const
        {
            return chunks;
        }
        
        inline std::vector<Chunk> getChunks()
        {
            return chunks;
        }
        
        inline size_t setChunkSize(size_t chunkSize) {
            return this->chunkSize = chunkSize;
        }
        
        inline void setOverallHash(FragmentHash oh) {
            overallHash = std::make_unique<FragmentHash>(oh);
        }
        
        bool checkValidity();
        
        inline void setFile(std::unique_ptr<MappedFile> f) {
            file = std::move(f);
        }
        
        void createChunks(const std::vector<FragmentHash>& hash);
        
        void generate();
        
    protected:
        std::unique_ptr<MappedFile> file;
        ssize_t chunkSize = -1;
    private:
        void determineChunkSize();
        std::vector<Chunk> chunks;
        std::unique_ptr<FragmentHash> overallHash;
        std::pair<MappedFile::iterator, MappedFile::iterator> getChunkBoundary(int chunkNum);
        void checkFile();
        
    };
}
#endif
