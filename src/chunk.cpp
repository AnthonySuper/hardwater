#include "chunk.hpp"

namespace Hardwater {

    Chunk::EncryptedIndex::EncryptedIndex(size_t idx, IonKey& k)
    {
        uint32_t ind = idx;
        std::array<uint8_t, 4> bytes;
        *(reinterpret_cast<uint32_t*>(bytes.data())) = ind;
        encrypted = k.encrypt(bytes.begin(), bytes.end());
    }
    
    
    Chunk Chunk::generate(MappedFile::iterator begin,
                          MappedFile::iterator end,
                          Hardwater::IonKey& key,
                          size_t index)
    {
        auto enc = key.encrypt(begin, end);
        return {enc.size(),
            {index, key},
            Hardwater::FragmentHash{enc}};
    }
    
    Chunk::Chunk(size_t s, EncryptedIndex id, FragmentHash hsh) :
        size(s), hash(hsh), index(id)
    {
    }
    
    
    static std::array<uint8_t, 4> writeLen(size_t in)
    {
        if(in > std::numeric_limits<uint32_t>::max()) {
            throw std::runtime_error("Chunk is too large");
        }
        std::array<uint8_t, 4> tmp;
        *(reinterpret_cast<uint32_t*>(tmp.data())) = in;
        return tmp;
    }
    
    void Chunk::writeTo(std::vector<uint8_t> &buff)
    {
        if(index.encrypted.size() != 4) {
            throw std::runtime_error("Houstin, we have an issue");
        }
        // Index of the chunk, encrypted with the Ion key
        buff.insert(buff.end(),
                    index.encrypted.begin(),
                    index.encrypted.end());
        // Length of chunk when encrypted
        auto wl = writeLen(size);
        buff.insert(buff.end(),
                    wl.begin(),
                    wl.end());
        // Hash of the chunk (when data is encrypted)
        hash.appendToBuffer(buff);
    }
}


