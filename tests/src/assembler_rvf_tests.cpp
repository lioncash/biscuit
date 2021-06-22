#include <catch/catch.hpp>

#include <biscuit/assembler.hpp>

using namespace biscuit;

TEST_CASE("FLW", "[rv32f]") {
    uint32_t value = 0;
    Assembler as(reinterpret_cast<uint8_t*>(&value), sizeof(value));

    as.FLW(f15, 1024, x31);
    REQUIRE(value == 0x400FA787);

    as.RewindBuffer();

    as.FLW(f15, 2048, x31);
    REQUIRE(value == 0x800FA787);

    as.RewindBuffer();

    as.FLW(f15, 4095, x31);
    REQUIRE(value == 0xFFFFA787);
}

TEST_CASE("FSW", "[rv32f]") {
    uint32_t value = 0;
    Assembler as(reinterpret_cast<uint8_t*>(&value), sizeof(value));

    as.FSW(f31, 1024, x15);
    REQUIRE(value == 0x41F7A027);

    as.RewindBuffer();

    as.FSW(f31, 2048, x15);
    REQUIRE(value == 0x81F7A027);

    as.RewindBuffer();

    as.FSW(f31, 4095, x15);
    REQUIRE(value == 0xFFF7AFA7);
}
