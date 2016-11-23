#include "key_helpers.hpp"

namespace TestHelpers {
    Hardwater::Key getDefaultPublicKey()
    {
        OpenSSL_add_all_ciphers();
        OpenSSL_add_all_digests();
        return {"test_files/keys/generic_public.pem",
            Hardwater::Key::KeyType::Public,
            nullptr};
    }
    
    Hardwater::Key getDefaultPrivateKey()
    {
        OpenSSL_add_all_ciphers();
        OpenSSL_add_all_digests();
        return {"test_files/keys/generic.pem",
            Hardwater::Key::KeyType::Private,
            "passphrase"};
    }
}
