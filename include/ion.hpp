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
        using FileBuffer = std::vector<uint8_t>;
        
        static Ion generate(MappedFile &mf, Key& pub);
        
        static Ion fromBuff(FileBuffer &buff);
        
        size_t chunkSize();
        
        Ion(EncryptedIonKey &ik,
            std::vector<uint8_t> encryptedFileName,
            size_t fileSize,
            size_t numChunks,
            FragmentHash hash,
            std::vector<Chunk>&& chunks);
        
        std::vector<uint8_t> dissolve(Key& sign);
        
        static constexpr uint8_t protocolVersion = 1;
        
    protected:
        EncryptedIonKey ionKey;
        
        std::vector<uint8_t> encryptedFileName;
        
        FragmentHash hash;
        
        size_t fileSize;
        
        uint16_t numChunks;
        
        std::vector<Chunk> chunks;
    };
}
#endif
