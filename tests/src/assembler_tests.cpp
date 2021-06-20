#include <catch/catch.hpp>

#include <biscuit/assembler.hpp>

TEST_CASE("AUIPC", "[rv32i]") {
    uint32_t value = 0;
    biscuit::Assembler as(reinterpret_cast<uint8_t*>(&value), sizeof(value));

    as.AUIPC(0xFFFFFFFF, biscuit::x31);
    REQUIRE(value == 0xFFFFFF97);

    as.RewindBuffer();

    as.AUIPC(0, biscuit::x31);
    REQUIRE(value == 0x00000F97);

    as.RewindBuffer();

    as.AUIPC(0x00FF00FF, biscuit::x31);
    REQUIRE(value == 0x00FF0F97);
}

TEST_CASE("JAL", "[rv32i]") {
    uint32_t value = 0;
    biscuit::Assembler as(reinterpret_cast<uint8_t*>(&value), sizeof(value));

    as.JAL(0xFFFFFFFF, biscuit::x31);
    REQUIRE(value == 0xFFFFFFEF);

    as.RewindBuffer();

    as.JAL(2000, biscuit::x31);
    REQUIRE(value == 0x7D000FEF);

    as.RewindBuffer();

    as.JAL(100000, biscuit::x31);
    REQUIRE(value == 0x6A018FEF);
}

TEST_CASE("LUI", "[rv32i]") {
    uint32_t value = 0;
    biscuit::Assembler as(reinterpret_cast<uint8_t*>(&value), sizeof(value));

    as.LUI(0xFFFFFFFF, biscuit::x10);
    REQUIRE(value == 0xFFFFF537);

    as.RewindBuffer();

    as.LUI(0x7FFFF000, biscuit::x10);
    REQUIRE(value == 0x7FFFF537);

    as.RewindBuffer();

    as.LUI(0xFFFFFFFF, biscuit::x31);
    REQUIRE(value == 0xFFFFFFB7);
}
