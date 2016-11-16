#ifndef _FILE_CHUNK_HPP
#define _FILE_CHUNK_HPP
#include "mapped_file.hpp"
#include "openssl/sha.h"

namespace Hardwater {
    class FileChunk {
        FileChunk(MappedFile::const_iterator begin, 
                MappedFile::const_iterator end);
    protected:
        unsigned char hash[SHA256_DIGEST_LENGTH];
    };
}
#endif
