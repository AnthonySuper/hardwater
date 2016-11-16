#include "file_chunk.hpp"

namespace Hardwater {
    FileChunk::FileChunk(MappedFile::const_iterator begin,
            MappedFile::const_iterator end)
    {
        SHA256_CTX ctx;
        SHA256_Init(&ctx);
        SHA256_Update(&ctx, begin, begin - end);
        SHA256_Final(hash, &ctx);
    }
}
