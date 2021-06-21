#include <catch/catch.hpp>

#include <biscuit/assembler.hpp>

TEST_CASE("DIV", "[rv32m]") {
    uint32_t value = 0;
    biscuit::Assembler as(reinterpret_cast<uint8_t*>(&value), sizeof(value));

    as.DIV(biscuit::x31, biscuit::x15, biscuit::x20);
    REQUIRE(value == 0x0347CFB3);

    as.RewindBuffer();

    as.DIV(biscuit::x31, biscuit::x20, biscuit::x15);
    REQUIRE(value == 0x02FA4FB3);

    as.RewindBuffer();

    as.DIV(biscuit::x20, biscuit::x31, biscuit::x15);
    REQUIRE(value == 0x02FFCA33);
}

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

TEST_CASE("MULHU", "[rv32m]") {
    uint32_t value = 0;
    biscuit::Assembler as(reinterpret_cast<uint8_t*>(&value), sizeof(value));

    as.MULHU(biscuit::x31, biscuit::x15, biscuit::x20);
    REQUIRE(value == 0x0347BFB3);

    as.RewindBuffer();

    as.MULHU(biscuit::x31, biscuit::x20, biscuit::x15);
    REQUIRE(value == 0x02FA3FB3);

    as.RewindBuffer();

    as.MULHU(biscuit::x20, biscuit::x31, biscuit::x15);
    REQUIRE(value == 0x02FFBA33);
}
