#ifndef _HARDWATER_ENCRYPTED_ION_KEY_HPP
#define _HARDWATER_ENCRYPTED_ION_KEY_HPP

#include "ion_key.hpp"
#include "key.hpp"
#include <openssl/err.h>

namespace Hardwater {
    /**
     * Hardwater Ions are encrypted with an Ion Key, which is essentially just an AES Key.
     * These keys are written, encrypted to the solution key, into the Ion file.
     * This class represents an Ion key which is encrypted.
     */
    class EncryptedIonKey {
        
    public:
        /**
         * Create an EncryptedIonKey by encrypting it for a solution key
         */
        EncryptedIonKey(IonKey &ik, Key &k);
        
        /**
         * Create an encryped Ion Key from a size and buffered data.
         * Only really used when reading Ion files.
         */
        EncryptedIonKey(std::vector<uint8_t>::const_iterator begin,
                        std::vector<uint8_t>::const_iterator end);
        
        /**
         * Get a decrypted IonKey given a solution key.
         */
        IonKey decrypt(Key &k);
        
        constexpr static int Padding = RSA_PKCS1_OAEP_PADDING;
        
        const std::vector<IonKey::ByteType>& getEncrypted() const {
            return encrypted;
        }
        
    private:
        std::vector<IonKey::ByteType> encrypted;
        
        void handleSSLError();
    };
}
#endif
