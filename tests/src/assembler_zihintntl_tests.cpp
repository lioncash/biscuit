#include <catch/catch.hpp>

#include <biscuit/assembler.hpp>

using namespace biscuit;

TEST_CASE("NTL.S1", "[Zihintntl]") {
    uint32_t value = 0;
    Assembler as(reinterpret_cast<uint8_t*>(&value), sizeof(value));

    as.NTL_S1();
    REQUIRE(value == 0x00400033);
}

TEST_CASE("NTL.P1", "[Zihintntl]") {
    uint32_t value = 0;
    Assembler as(reinterpret_cast<uint8_t*>(&value), sizeof(value));

    as.NTL_P1();
    REQUIRE(value == 0x00200033);
}

TEST_CASE("NTL.PALL", "[Zihintntl]") {
    uint32_t value = 0;
    Assembler as(reinterpret_cast<uint8_t*>(&value), sizeof(value));

    as.NTL_PALL();
    REQUIRE(value == 0x00300033);
}
