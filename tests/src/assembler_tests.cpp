#include <catch/catch.hpp>

#include <biscuit/assembler.hpp>

TEST_CASE("LUI", "[rv32i]") {
    uint32_t value = 0;
    biscuit::Assembler as(reinterpret_cast<uint8_t*>(&value), sizeof(value));

    as.LUI(0xFFFFFFFF, biscuit::x10);
    REQUIRE(value == 0xFFFFF537);

    as.RewindBuffer();

    as.LUI(0x7FFFF000, biscuit::x10);
    REQUIRE(value == 0x7FFFF537);
}
