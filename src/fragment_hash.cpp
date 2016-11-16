#include "fragment_hash.hpp"

namespace Hardwater {
    FragmentHash::FragmentHash(MappedFile::const_iterator begin,
            MappedFile::const_iterator end)
    {
        SHA256_CTX ctx;
        SHA256_Init(&ctx);
        SHA256_Update(&ctx, begin, end - begin);
        SHA256_Final(hash.begin(), &ctx);
    }
    
    std::string FragmentHash::getDigest() {
        std::stringstream ss;
        ss << std::hex << std::fixed << std::setw(2);
        for(auto c: hash) {
            ss << static_cast<int>(c);
        }
        return ss.str();
    }
}
