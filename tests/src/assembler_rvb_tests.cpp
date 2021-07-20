#include <catch/catch.hpp>

#include <biscuit/assembler.hpp>

using namespace biscuit;

TEST_CASE("ADD.UW", "[rvb]") {
    uint32_t value = 0;
    Assembler as(reinterpret_cast<uint8_t*>(&value), sizeof(value));

    as.ADDUW(x31, x7, x15);
    REQUIRE(value == 0x08F38FBB);

    as.RewindBuffer();

    // Pseudo instruction

    as.ZEXTW(x31, x7);
    REQUIRE(value == 0x08038FBB);
}
