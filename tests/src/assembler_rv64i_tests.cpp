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
