#ifndef _HARDWATER_UTIL_HPP
#define _HARDWATER_UTIL_HPP
#include <type_traits>
#include <openssl/rand.h>
#include <limits>
namespace Hardwater { namespace Util {
    
    template<typename T>
    typename std::enable_if<std::is_integral<T>::value, T>::type
    roundUp(T in, T multiple)
    {
        auto isPositive = multiple >= 0;
        return ((in + isPositive * (multiple - 1)) / multiple) * multiple;
    }
    
    
    struct RNG {
        inline int operator()(int m) {
            unsigned int tmp;
            RAND_bytes(reinterpret_cast<unsigned char*>(&tmp), sizeof(tmp));
            unsigned int max = std::numeric_limits<unsigned int>::max();
            return (tmp / (1.0 * max)) * m;
        }
    };
    
}}
#endif
