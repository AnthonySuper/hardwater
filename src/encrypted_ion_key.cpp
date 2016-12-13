#include "encrypted_ion_key.hpp"
#include <iostream>
namespace Hardwater {
    EncryptedIonKey::EncryptedIonKey(IonKey &ik, Key &k)
    {
        if(k.isPrivate()) {
            throw std::invalid_argument("Need a public key!");
        }
        
        auto rsa = k.getKey();
        encrypted.resize(RSA_size(rsa));
        std::vector<uint8_t> combined;
        auto& keyBuff = ik.getKey();
        auto& ivpBuff = ik.getIVP();
        static_assert(IonKey::keyLength + IonKey::IVPLength < 256,
                      "Cannot encrypt IVP and key together, too large");
        combined.insert(combined.end(), keyBuff.begin(), keyBuff.end());
        combined.insert(combined.end(), ivpBuff.begin(), ivpBuff.end());
        int sze = RSA_public_encrypt(combined.size(),
                                     combined.data(),
                                     encrypted.data(),
                                     rsa,
                                     Padding);
        if(sze == -1) {
            handleSSLError();
        }
    }
    
    IonKey EncryptedIonKey::decrypt(Hardwater::Key &k)
    {
        if(k.isPublic()) {
            throw std::invalid_argument("Need a private key!");
        }
        auto rsa = k.getKey();
        std::vector<uint8_t> buff;
        buff.resize(RSA_size(rsa));
        int res = RSA_private_decrypt(buff.size(),
                                      encrypted.data(),
                                      buff.data(),
                                      rsa,
                                      Padding);
        if(res == -1) {
            handleSSLError();
        }
        std::cout << "res is " << res << std::endl;
        buff.resize(res);
        IonKey::KeyBuffer keyBuff;
        IonKey::IVPBuffer ivpBuff;
        std::copy(buff.begin(),
                  buff.begin() + IonKey::keyLength,
                  keyBuff.begin());
        auto b2 = buff.begin() + IonKey::keyLength;
        std::copy(b2,
                  b2 + IonKey::IVPLength,
                  ivpBuff.begin());
        return IonKey(keyBuff, ivpBuff);
    }
    
    EncryptedIonKey::EncryptedIonKey(std::vector<uint8_t>::const_iterator begin,
                                     std::vector<uint8_t>::const_iterator end) :
    encrypted(begin, end) {
        
    }
    
    void EncryptedIonKey::handleSSLError()
    {
        ERR_load_crypto_strings();
        auto code = ERR_get_error();
        char buff[120];
        ERR_error_string(code, buff);
        std::string str(buff);
        throw std::runtime_error("SSL Error " + str + " : " + ERR_reason_error_string(code));
    }
}
