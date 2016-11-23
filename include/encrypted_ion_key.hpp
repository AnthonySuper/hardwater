#ifndef _HARDWATER_ENCRYPTED_ION_KEY_HPP
#define _HARDWATER_ENCRYPTED_ION_KEY_HPP

#include "ion_key.hpp"
#include "key.hpp"
#include <openssl/err.h>

namespace Hardwater {
    class EncryptedIonKey {
        
    public:
        EncryptedIonKey(IonKey &ik, Key &k);
        IonKey decrypt(Key &k);
        constexpr static int Padding = RSA_PKCS1_OAEP_PADDING;
    private:
        std::vector<IonKey::ByteType> encrypted;
        void handleSSLError();
    };
}
#endif
