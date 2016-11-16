#ifndef _FILE_CHUNK_HPP
#define _FILE_CHUNK_HPP
#include "mapped_file.hpp"
#include "openssl/sha.h"
#include <string>
#include <array>
#include <sstream>
#include <iomanip>

namespace Hardwater {
    class FragmentHash {
    public:
        FragmentHash(MappedFile::const_iterator begin, 
                MappedFile::const_iterator end);
        std::string getDigest();
    protected:
        std::array<unsigned char, SHA256_DIGEST_LENGTH> hash;
    };
}
#endif
