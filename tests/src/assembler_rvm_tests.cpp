#include <catch/catch.hpp>

#include <biscuit/assembler.hpp>

TEST_CASE("MUL", "[rv32m]") {
    uint32_t value = 0;
    biscuit::Assembler as(reinterpret_cast<uint8_t*>(&value), sizeof(value));

    as.MUL(biscuit::x31, biscuit::x15, biscuit::x20);
    REQUIRE(value == 0x03478FB3);

    as.RewindBuffer();

    as.MUL(biscuit::x31, biscuit::x20, biscuit::x15);
    REQUIRE(value == 0x02FA0FB3);

    as.RewindBuffer();

    as.MUL(biscuit::x20, biscuit::x31, biscuit::x15);
    REQUIRE(value == 0x02FF8A33);
}

TEST_CASE("MULH", "[rv32m]") {
    uint32_t value = 0;
    biscuit::Assembler as(reinterpret_cast<uint8_t*>(&value), sizeof(value));

    as.MULH(biscuit::x31, biscuit::x15, biscuit::x20);
    REQUIRE(value == 0x03479FB3);

    as.RewindBuffer();

    as.MULH(biscuit::x31, biscuit::x20, biscuit::x15);
    REQUIRE(value == 0x02FA1FB3);

    as.RewindBuffer();

    as.MULH(biscuit::x20, biscuit::x31, biscuit::x15);
    REQUIRE(value == 0x02FF9A33);
}

TEST_CASE("MULHSU", "[rv32m]") {
    uint32_t value = 0;
    biscuit::Assembler as(reinterpret_cast<uint8_t*>(&value), sizeof(value));

    as.MULHSU(biscuit::x31, biscuit::x15, biscuit::x20);
    REQUIRE(value == 0x0347AFB3);

    as.RewindBuffer();

    as.MULHSU(biscuit::x31, biscuit::x20, biscuit::x15);
    REQUIRE(value == 0x02FA2FB3);

    as.RewindBuffer();

    as.MULHSU(biscuit::x20, biscuit::x31, biscuit::x15);
    REQUIRE(value == 0x02FFAA33);
}
