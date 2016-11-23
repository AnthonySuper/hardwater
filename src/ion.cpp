#include "ion.hpp"

namespace Hardwater {
    
    static size_t determineChunkNumber(size_t size)
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
    
    Ion Ion::generate(MappedFile &mf,
                       Key &pub)
    {
        if(pub.isPrivate()) {
            throw std::runtime_error("Type mismatch, public key is private");
        }
        FragmentHash hash(mf.begin(), mf.end());
        IonKey ik;
        EncryptedIonKey ek(ik, pub);
        size_t numChunks = determineChunkNumber(mf.getSize());
        size_t chunkSize = mf.getSize() / numChunks;
        std::vector<Chunk> chunks;
        for(size_t i = 0; i < numChunks; ++i) {
            MappedFile::iterator begin = mf.begin() + (i * chunkSize);
            MappedFile::iterator end = (i == numChunks - 1) ?
                mf.end() : mf.begin() + ((i + 1) * chunkSize);
            auto c = Chunk::generate(begin,
                                     end,
                                     ik,
                                     i);
            chunks.push_back(c);
        }
        std::random_shuffle(chunks.begin(), chunks.end(), Util::RNG());
        return {mf.getSize(), numChunks, hash, std::move(chunks)};
    }
    
    Ion::Ion(size_t fs,
             size_t nc,
             FragmentHash overall,
             std::vector<Chunk>&& chunks)
    : hash(overall), fileSize(fs), numChunks(nc), chunks(chunks)
    {
        
    }
    
    size_t Ion::chunkSize()
    {
        if(fileSize == 0 || numChunks == 0) {
            throw std::runtime_error("Cannot determine chunk size!");
        }
        return fileSize / numChunks;
    }
}
