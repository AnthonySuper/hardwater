#ifndef _FILE_CHUNK_HPP
#define _FILE_CHUNK_HPP
#include "mapped_file.hpp"
#include "openssl/sha.h"
#include <string>
#include <array>
#include <sstream>
#include <cstdint>
#include <iomanip>
#include <vector>
#include <iterator>

namespace Hardwater {
    class FragmentHash {
    public:
        using HashArray = std::array<unsigned char, SHA256_DIGEST_LENGTH>;
        
        /**
         * Slurp an existing fragment hash.
         * Used primarily to read Ions.
         */
        template<typename Itr>
        FragmentHash(Itr &in,
                     typename std::enable_if<sizeof(*in) == 1, bool>::type t = true)
        {
            for(int i = 0; i < hash.size(); ++i) {
                hash[i] = *in;
                in++;
            }
        }
        
        
        template<typename Itr>
        FragmentHash(Itr begin, Itr end)
        {
            static_assert(sizeof(decltype(*begin)) == 1);
            SHA256_CTX ctx;
            SHA256_Init(&ctx);
            SHA256_Update(&ctx, begin, end - begin);
            SHA256_Final(hash.data(), &ctx);
        }
        
        template<typename T>
        FragmentHash(T *ptr, size_t size)
        : FragmentHash(ptr, ptr + size)
        {
            
        }
        
        template<typename T>
        FragmentHash(std::vector<T> vec,
                     typename std::enable_if<sizeof(T) == 1, int>::type t = 0)
        : FragmentHash(vec.data(), vec.size())
        {
        }
        
        FragmentHash(std::vector<uint8_t>::iterator begin,
                     std::vector<uint8_t>::iterator end);
        
        
        std::string getDigest();
        
        HashArray getHash() {
            return hash;
        }
        
        const HashArray& getHash() const {
            return hash;
        }
        
        void appendToBuffer(std::vector<uint8_t> &buff) const;
        
        bool operator==(const FragmentHash& o);
        
        template<typename Itr>
        bool verify(Itr begin, Itr end)
        {
            return *this == FragmentHash(begin, end);
        }
        
    protected:
        HashArray hash;
    };
}
#endif
