#include <catch/catch.hpp>

#include <biscuit/assembler.hpp>

using namespace biscuit;

TEST_CASE("WRS.NTO", "[Zawrs]") {
    uint32_t value = 0;
    Assembler as(reinterpret_cast<uint8_t*>(&value), sizeof(value));

    as.WRS_NTO();
    REQUIRE(value == 0x00D00073);
}

TEST_CASE("WRS.STO", "[Zawrs]") {
    uint32_t value = 0;
    Assembler as(reinterpret_cast<uint8_t*>(&value), sizeof(value));

    as.WRS_STO();
    REQUIRE(value == 0x01D00073);
}
