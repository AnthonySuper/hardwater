#include "ion_key.hpp"

namespace Hardwater {
    
    
    IonKey::IonKey()
    {
        RAND_bytes(key.data(),
                   keyLength);
        RAND_bytes(ivp.data(),
                   IVPLength);
    }
    
    IonKey::IonKey(KeyBuffer &kb, IVPBuffer &ib) :
    key(kb), ivp(ib)
    {
    }
    
    
    bool IonKey::operator==(const IonKey &other)
    {
        return key == other.key &&
            ivp == other.ivp;
    }
    
    std::vector<IonKey::ByteType> IonKey::encrypt(std::string str)
    {
        std::vector<ByteType> ret;
        int rslen = (str.size() / 16 + 1)*16;
        ret.resize(rslen);
        int cipherLen = 0;
        EVP_CIPHER_CTX *ctx = getCTX();
        int res;
        res = EVP_EncryptInit_ex(ctx,
                                 CypherFunc(),
                                 NULL,
                                 key.data(),
                                 ivp.data());
        if(res != 1) {
            throw std::runtime_error("SSL Error");
        }
        auto ptr = reinterpret_cast<const unsigned char *>(str.c_str());
        res = EVP_EncryptUpdate(ctx,
                                ret.data(),
                                &cipherLen,
                                ptr,
                                str.length());
        if(res != 1) {
            throw std::runtime_error("SSL Error!");
        }
        int tempLen = 0;
        res = EVP_EncryptFinal(ctx,
                               (ret.data() + cipherLen),
                               &tempLen);
        if(res != 1) {
            throw std::runtime_error("SSL Error!");
        }
        cipherLen += tempLen;
        if(cipherLen != ret.size()) {
            throw std::runtime_error("That is not supposed to happen");
        }
        EVP_CIPHER_CTX_free(ctx);
        return ret;
    }
    
    
    EVP_CIPHER_CTX* IonKey::getCTX()
    {
        auto ctx = EVP_CIPHER_CTX_new();
        if(ctx == nullptr) {
            throw std::runtime_error("SSL Error");
        }
        return ctx;
    }
    
    std::vector<IonKey::ByteType>
    IonKey::encrypt(std::vector<ByteType>::iterator b,
                    std::vector<ByteType>::iterator e)
    {
        return encrypt(&(*b), &(*e));
    }
    
    std::vector<IonKey::ByteType>
    IonKey::decrypt(std::vector<ByteType>::iterator b,
                    std::vector<ByteType>::iterator e)
    {
        return decrypt(&(*b), &(*e));
    }
    
    
}
