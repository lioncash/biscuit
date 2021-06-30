#include <catch/catch.hpp>

#include <biscuit/assembler.hpp>

using namespace biscuit;

TEST_CASE("MRET", "[rvpriv]") {
    uint32_t value = 0;
    Assembler as(reinterpret_cast<uint8_t*>(&value), sizeof(value));

    as.MRET();
    REQUIRE(value == 0x30200073);
}

TEST_CASE("SRET", "[rvpriv]") {
    uint32_t value = 0;
    Assembler as(reinterpret_cast<uint8_t*>(&value), sizeof(value));

    as.SRET();
    REQUIRE(value == 0x10200073);
}

TEST_CASE("URET", "[rvpriv]") {
    uint32_t value = 0;
    Assembler as(reinterpret_cast<uint8_t*>(&value), sizeof(value));

    as.URET();
    REQUIRE(value == 0x00200073);
}

TEST_CASE("WFI", "[rvpriv]") {
    uint32_t value = 0;
    Assembler as(reinterpret_cast<uint8_t*>(&value), sizeof(value));

    as.WFI();
    REQUIRE(value == 0x10500073);
}
