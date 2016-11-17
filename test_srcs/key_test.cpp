#include "catch.hpp"
#include "key.hpp"

TEST_CASE("Basic extraction") {
    OpenSSL_add_all_ciphers();
    OpenSSL_add_all_digests();
    SECTION("Extraction from a file with a private key") {
        auto pp = "passphrase";
        auto path = "test_files/keys/generic.pem";
        SECTION("It cannot extract the public key"){
            auto keyType = Hardwater::Key::KeyType::Public;
            REQUIRE_THROWS_AS(Hardwater::Key(path, keyType, pp),
                              Hardwater::Key::ReadError);
        }
        
        SECTION("It can the private key") {
            auto keyType = Hardwater::Key::KeyType::Private;
            REQUIRE_NOTHROW(Hardwater::Key(path, keyType, pp));
        }
         
        
    }
    
    SECTION("Extraction from a public-only file") {
        auto path = "test_files/keys/generic_public.pem";
        SECTION("Works when requesting publc keys") {
            auto keyType = Hardwater::Key::KeyType::Public;
            REQUIRE_NOTHROW(Hardwater::Key(path, keyType, nullptr));
        }
        
        SECTION("Does not work when requesting a private key") {
            auto keyType = Hardwater::Key::KeyType::Private;
            REQUIRE_THROWS_AS(Hardwater::Key(path, keyType, ""),
                              Hardwater::Key::ReadError);
        }
    }
}
