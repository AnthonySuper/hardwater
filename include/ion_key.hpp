#ifndef _HARDWATER_ION_KEY_HPP
#define _HARDWATER_ION_KEY_HPP
#include <openssl/aes.h>
#include <openssl/rand.h>
#include <openssl/evp.h>
#include <cstdint>
#include <array>
#include <vector>
#include "util.hpp"

namespace Hardwater {
    class IonKey {
    public:
        using ByteType = uint8_t;
        static constexpr size_t keyLength = 32;
        static constexpr size_t keySize = 256;
        static constexpr size_t IVPLength = 16;
        static constexpr size_t IVPSize = 128;
        using KeyBuffer = std::array<ByteType, keyLength>;
        using IVPBuffer = std::array<ByteType, IVPLength>;
        
        /**
         * Default constructor randomly generates a key
         */
        IonKey();
        
        IonKey(KeyBuffer &kb, IVPBuffer &ib);
        
        
        bool operator==(const IonKey &other);
        
        
        template<typename Itr>
        std::vector<ByteType> encrypt(Itr begin, Itr end)
        {
            static_assert(sizeof(*begin) == sizeof(ByteType));
            std::vector<ByteType> ret;
            size_t inSize = end - begin;
            int resizeLen = (inSize / 16 + 1)*16;
            ret.resize(resizeLen);
            int cipherLen = 0;
            EVP_CIPHER_CTX *ctx = getCTX();
            int res;
            if(ctx == nullptr)
            {
                throw std::runtime_error("Could not create EVP Cipher");
            }
            
            res = EVP_EncryptInit_ex(ctx,
                                     CypherFunc(),
                                     NULL,
                                     key.data(),
                                     ivp.data());
            if(res != 1) {
                throw std::runtime_error("SSL Error");
            }
            res = EVP_EncryptUpdate(ctx,
                                    ret.data(),
                                    &cipherLen,
                                    begin,
                                    inSize);
            if(res != 1) {
                throw std::runtime_error("SSL Error");
            }
            int tmpLen = 0;
            res = EVP_EncryptFinal(ctx,
                                   (ret.data() + cipherLen),
                                   &tmpLen);
            if(res != 1) {
                throw std::runtime_error("SSL Error");
            }
            cipherLen += tmpLen;
            if(cipherLen != ret.size()) {
                throw std::runtime_error("That's not supposed to happen");
            }
            EVP_CIPHER_CTX_free(ctx);
            return ret;
        }
        
        template<typename Itr>
        std::vector<ByteType> decrypt(Itr begin, Itr end)
        {
            static_assert(sizeof(*begin) == sizeof(ByteType));
            std::vector<IonKey::ByteType> ret;
            size_t inSize = end - begin;
            ret.resize(inSize);
            auto ctx = getCTX();
            int err = 0;
            err = EVP_DecryptInit_ex(ctx,
                                     CypherFunc(),
                                     NULL,
                                     key.data(),
                                     ivp.data());
            if(err != 1) {
                throw std::runtime_error("SSL Error");
            }
            int outl = 0;
            err = EVP_DecryptUpdate(ctx,
                                    ret.data(),
                                    &outl,
                                    begin,
                                    inSize);
            if(err != 1) {
                throw std::runtime_error("SSL Error");
            }
            int tempL = 0;
            err = EVP_DecryptFinal(ctx,
                                   (ret.data() + outl),
                                   &tempL);
            if(err != 1) {
                throw std::runtime_error("SSL Error");
            }
            EVP_CIPHER_CTX_free(ctx);
            ret.resize(tempL + outl);
            return ret;
        }
        
        using BTVecItr = std::vector<ByteType>::iterator;
        
        std::vector<ByteType> encrypt(BTVecItr begin, BTVecItr end);
        std::vector<ByteType> decrypt(BTVecItr begin, BTVecItr end);
        
        const std::array<ByteType, keyLength>& getKey() const {
            return key;
        }
        
        const std::array<ByteType, IVPLength>& getIVP() const {
            return ivp;
        }
        

        
    private:

        constexpr static const auto CypherFunc = EVP_aes_256_cbc;
        
        
        EVP_CIPHER_CTX* getCTX();
        
        KeyBuffer key;
        IVPBuffer ivp;
        
        static_assert(keyLength * sizeof(ByteType) == keySize / 8);
        static_assert(IVPLength * sizeof(ByteType) == IVPSize / 8);
    };
}
#endif
