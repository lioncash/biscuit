#include <catch/catch.hpp>

#include <biscuit/assembler.hpp>

using namespace biscuit;

TEST_CASE("CZERO.EQZ", "[Zicond]") {
    uint32_t value = 0;
    Assembler as(reinterpret_cast<uint8_t*>(&value), sizeof(value));

    as.CZERO_EQZ(x31, x30, x29);
    REQUIRE(value == 0x0FDF5FB3);

    as.RewindBuffer();

    as.CZERO_EQZ(x1, x2, x3);
    REQUIRE(value == 0x0E3150B3);
}

TEST_CASE("CZERO.NEZ", "[Zicond]") {
    uint32_t value = 0;
    Assembler as(reinterpret_cast<uint8_t*>(&value), sizeof(value));

    as.CZERO_NEZ(x31, x30, x29);
    REQUIRE(value == 0x0FDF7FB3);

    as.RewindBuffer();

    as.CZERO_NEZ(x1, x2, x3);
    REQUIRE(value == 0x0E3170B3);
}
