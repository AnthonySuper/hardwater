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
