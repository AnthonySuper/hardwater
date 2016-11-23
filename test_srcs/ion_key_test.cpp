#include "catch.hpp"
#include "ion_key.hpp"

TEST_CASE("File encryption works without throwing") {
    Hardwater::IonKey k;
    std::vector<uint8_t> ints{65, 78, 84, 72, 79, 78, 89};
    REQUIRE_NOTHROW(k.encrypt(ints.begin(), ints.end()));
}

TEST_CASE("Data encryption roundtrip") {
    Hardwater::IonKey k;
    std::vector<uint8_t> ints{65, 78, 84, 72, 79, 78, 89};
    auto crypt = k.encrypt(ints.begin(), ints.end());
    auto decrypt = k.decrypt(crypt.begin(), crypt.end());
    REQUIRE(ints == decrypt);
}

