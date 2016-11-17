#include "catch.hpp"
#include "ion.hpp"

TEST_CASE("Files hash properly") {
    auto q = std::make_unique<Hardwater::MappedFile>("test_files/text/short_snippet.txt");
    Hardwater::Ion ion(std::move(q));
    ion.generate();
    REQUIRE(ion.getOverallHash().getDigest() == "6ef6baf229ed350b6239ecade2e9b89a275ded50939e8892edeb9acdbab13ba6");
    
}
