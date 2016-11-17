#include "catch.hpp"
#include "key.hpp"

TEST_CASE("Basic extractio") {
    auto pp = "passphrase";
    SECTION("Extraction of public keys works") {
        auto keyType = Hardwater::Key::KeyType::Public;
        REQUIRE_NOTHROW(Hardwater::Key("test_files/keys/generic.pem", keyType, pp));
    }
    
    SECTION("Extraction of private keys works") {
        auto keyType = Hardwater::Key::KeyType::Private;
        REQUIRE_NOTHROW(Hardwater::Key("test_files/keys/generic.pem", keyType, pp));
    }
}
