#include "key.hpp"

namespace Hardwater {
    Key::Key(std::string pathname, KeyType type, std::string password) :
    type(type)
    {
        auto readFunc = (type == KeyType::Public) ? \
            PEM_read_RSAPublicKey : PEM_read_RSAPrivateKey;
        FILE *fp = fopen(pathname.c_str(), "r");
        if(fp == nullptr) {
            throw std::runtime_error("File does not exist");
        }
        RSA *rawPtr = nullptr;
        readFunc(fp,
                 &rawPtr,
                 nullptr,
                 const_cast<char*>(password.c_str()));
        if(rawPtr == nullptr) {
            
        }
        rsa = std::shared_ptr<RSA>(rawPtr, RSA_free);
    }
}
