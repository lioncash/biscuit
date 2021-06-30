#include <catch/catch.hpp>

#include <biscuit/assembler.hpp>

using namespace biscuit;

TEST_CASE("MRET", "[rvpriv]") {
    uint32_t value = 0;
    Assembler as(reinterpret_cast<uint8_t*>(&value), sizeof(value));

    as.MRET();
    REQUIRE(value == 0x30200073);
}

TEST_CASE("SFENCE.VMA", "[rvpriv]") {
    uint32_t value = 0;
    Assembler as(reinterpret_cast<uint8_t*>(&value), sizeof(value));

    as.SFENCE_VMA(x0, x0);
    REQUIRE(value == 0x12000073U);

    as.RewindBuffer();

    as.SFENCE_VMA(x15, x15);
    REQUIRE(value == 0x12F78073);
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
