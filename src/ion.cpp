#include "ion.hpp"

namespace Hardwater {
    
    Ion::Ion() :
    file(nullptr), overallHash(nullptr)
    {
        
    }
    
    Ion::Ion(std::unique_ptr<MappedFile>&& f) :
    file(std::move(f))
    {
        
    }
    
    void Ion::generate() {
        if(file == nullptr || chunks.size() > 0) {
            throw std::runtime_error("Cannot generate from a null file or a file that is already chunked");
        }
        determineChunkSize();
        auto begin = file->begin();
        size_t numChunks = getNumChunks();
        overallHash = std::make_unique<FragmentHash>(file->begin(), file->end());
        for(size_t i = 0; i < (numChunks - 1); ++i) {
            auto start = begin + i * chunkSize;
            auto end = start + chunkSize;
            chunks.emplace_back(start, end);
        }
        chunks.emplace_back(begin + (numChunks  - 1) * chunkSize, file->end());
    }
    
    void Ion::determineChunkSize() {
        constexpr size_t kilobyte = 1024;
        constexpr size_t megabyte = kilobyte * kilobyte;
        if(file->getSize() < kilobyte) {
            chunkSize = kilobyte;
        }
        else if(file->getSize() < megabyte) {
            chunkSize = 16 * kilobyte;
        }
        else {
            constexpr size_t multiple = 16 * kilobyte;
            chunkSize = file->getSize() / 2048;
            // clamp to nearest multiple of 16 kb
            chunkSize = ((chunkSize + multiple - 1) / multiple) * multiple;
        }
    }
    
    void Ion::createChunks(const std::vector<FragmentHash> &hashes)
    {
        if(chunks.size() > 0) {
            throw std::runtime_error("We already have chunks");
        }
        if(hashes.size() != getNumChunks()) {
            throw std::runtime_error("Mismatch on chunk numbers");
        }
        for(int i = 0; i < hashes.size(); ++i) {
            chunks.emplace_back(hashes.at(i));
            auto b = getChunkBoundary(i);
            chunks.at(i).addFile(b.first, b.second);
        }
    }
    
    
    std::pair<MappedFile::iterator, MappedFile::iterator>
    Ion::getChunkBoundary(int chunkNum)
    {
        checkFile();
        auto b = file->begin() + (chunkNum * chunkSize);
        if(chunkNum == getNumChunks() - 1) {
            auto e = file->end();
            return std::make_pair(b, e);
        }
        auto e = b + chunkSize;
        return std::make_pair(b, e);
    }
    
    bool Ion::checkValidity() {
        checkFile();
        return FragmentHash(file->begin(), file->end()) == *overallHash;
    }
    
    void Ion::checkFile() {
        if(file == nullptr) {
            throw std::runtime_error("File is a null pointer");
        }
    }
    
    
    
}
