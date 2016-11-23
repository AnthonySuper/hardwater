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
}


