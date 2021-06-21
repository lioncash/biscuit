#include <catch/catch.hpp>

#include <biscuit/assembler.hpp>

TEST_CASE("LWU", "[rv64i]") {
    uint32_t value = 0;
    biscuit::Assembler as(reinterpret_cast<uint8_t*>(&value), sizeof(value));

    as.LWU(biscuit::x15, biscuit::x31, 1024);
    REQUIRE(value == 0x400FE783);

    as.RewindBuffer();

    as.LWU(biscuit::x15, biscuit::x31, 2048);
    REQUIRE(value == 0x800FE783);

    as.RewindBuffer();

    as.LWU(biscuit::x15, biscuit::x31, 4095);
    REQUIRE(value == 0xFFFFE783);
}

TEST_CASE("LD", "[rv64i]") {
    uint32_t value = 0;
    biscuit::Assembler as(reinterpret_cast<uint8_t*>(&value), sizeof(value));

    as.LD(biscuit::x15, 1024, biscuit::x31);
    REQUIRE(value == 0x400FB783);

    as.RewindBuffer();

    as.LD(biscuit::x15, 2048, biscuit::x31);
    REQUIRE(value == 0x800FB783);

    as.RewindBuffer();

    as.LD(biscuit::x15, 4095, biscuit::x31);
    REQUIRE(value == 0xFFFFB783);
}

TEST_CASE("SD", "[rv64i]") {
    uint32_t value = 0;
    biscuit::Assembler as(reinterpret_cast<uint8_t*>(&value), sizeof(value));

    as.SD(biscuit::x15, 1024, biscuit::x31);
    REQUIRE(value == 0x40FFB023);

    as.RewindBuffer();

    as.SD(biscuit::x15, 2048, biscuit::x31);
    REQUIRE(value == 0x80FFB023);

    as.RewindBuffer();

    as.SD(biscuit::x15, 4095, biscuit::x31);
    REQUIRE(value == 0xFEFFBFA3);
}

TEST_CASE("SLLI64", "[rv64i]") {
    uint32_t value = 0;
    biscuit::Assembler as(reinterpret_cast<uint8_t*>(&value), sizeof(value));

    as.SLLI64(biscuit::x31, biscuit::x15, 10);
    REQUIRE(value == 0x00A79F93);

    as.RewindBuffer();

    as.SLLI64(biscuit::x31, biscuit::x15, 20);
    REQUIRE(value == 0x01479F93);

    as.RewindBuffer();

    as.SLLI64(biscuit::x31, biscuit::x15, 31);
    REQUIRE(value == 0x01F79F93);

    as.RewindBuffer();

    as.SLLI64(biscuit::x31, biscuit::x15, 63);
    REQUIRE(value == 0x03F79F93);
}

TEST_CASE("SRAI64", "[rv64i]") {
    uint32_t value = 0;
    biscuit::Assembler as(reinterpret_cast<uint8_t*>(&value), sizeof(value));

    as.SRAI64(biscuit::x31, biscuit::x15, 10);
    REQUIRE(value == 0x40A7DF93);

    as.RewindBuffer();

    as.SRAI64(biscuit::x31, biscuit::x15, 20);
    REQUIRE(value == 0x4147DF93);

    as.RewindBuffer();

    as.SRAI64(biscuit::x31, biscuit::x15, 31);
    REQUIRE(value == 0x41F7DF93);

    as.RewindBuffer();

    as.SRAI64(biscuit::x31, biscuit::x15, 63);
    REQUIRE(value == 0x43F7DF93);
}

TEST_CASE("SRLI64", "[rv64i]") {
    uint32_t value = 0;
    biscuit::Assembler as(reinterpret_cast<uint8_t*>(&value), sizeof(value));

    as.SRLI64(biscuit::x31, biscuit::x15, 10);
    REQUIRE(value == 0x00A7DF93);

    as.RewindBuffer();

    as.SRLI64(biscuit::x31, biscuit::x15, 20);
    REQUIRE(value == 0x0147DF93);

    as.RewindBuffer();

    as.SRLI64(biscuit::x31, biscuit::x15, 31);
    REQUIRE(value == 0x01F7DF93);

    as.RewindBuffer();

    as.SRLI64(biscuit::x31, biscuit::x15, 63);
    REQUIRE(value == 0x03F7DF93);
}
