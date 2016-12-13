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
    /**
     * Ions in harwater represent a single file in a solution at a given point in time.
     * They do this according to the Ion specification, which you can see in the README.
     */
    class Ion {
    public:
        
        /**
         * Generate an Ion for a solution using the Solution's public key.
         */
        static Ion generate(MappedFile &mf, Key& pub);
        
        /**
         * Read an Ion from a buffer.
         */
        static Ion fromBuff(std::vector<uint8_t> &buff, Key &pub);
        
        size_t chunkSize();
        
        Ion(EncryptedIonKey &ik,
            std::vector<uint8_t> encryptedFileName,
            size_t fileSize,
            size_t numChunks,
            FragmentHash hash,
            std::vector<Chunk>&& chunks);
        
        /**
         * Generate a dissolved ion by signing an existing Ion with a solution key.
         */
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
