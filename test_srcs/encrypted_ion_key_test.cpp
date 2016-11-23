#include "catch.hpp"
#include "encrypted_ion_key.hpp"
#include "ion_key.hpp"
#include "key_helpers.hpp"

TEST_CASE("EncryptedIonKey roundtrip") {
    auto pri = TestHelpers::getDefaultPrivateKey();
    auto pub = TestHelpers::getDefaultPublicKey();
    Hardwater::IonKey init;
    Hardwater::EncryptedIonKey enc(init, pub);
    REQUIRE(enc.decrypt(pri) == init);
    
}
