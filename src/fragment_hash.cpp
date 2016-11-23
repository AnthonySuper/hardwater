#include "fragment_hash.hpp"

namespace Hardwater {
    std::string FragmentHash::getDigest() {
        std::stringstream ss;
        ss << std::hex << std::setfill('0');
        for(auto c: hash) {
            ss << std::setw(2) << static_cast<int>(c);
        }
        return ss.str();
    }
    
    bool FragmentHash::operator==(const Hardwater::FragmentHash &o)
    {
        return hash == o.hash;
    }
}
