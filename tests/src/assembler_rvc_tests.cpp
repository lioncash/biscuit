#include <catch/catch.hpp>

#include <biscuit/assembler.hpp>

using namespace biscuit;

TEST_CASE("C.ADDI4SPN", "[rvc]") {
    uint32_t value = 0;
    Assembler as(reinterpret_cast<uint8_t*>(&value), sizeof(value));

    as.C_ADDI4SPN(x15, 0xFF);
    REQUIRE(value == 0x1FFC);

    as.RewindBuffer();

    as.C_ADDI4SPN(x8, 0xFF);
    REQUIRE(value == 0x1FE0);
}

TEST_CASE("C.FLD", "[rvc]") {
    uint32_t value = 0;
    Assembler as(reinterpret_cast<uint8_t*>(&value), sizeof(value));

    as.C_FLD(f15, 8, x15);
    REQUIRE(value == 0x279C);

    as.RewindBuffer();

    as.C_FLD(f15, 24, x15);
    REQUIRE(value == 0x2F9C);
}

TEST_CASE("C.FLW", "[rvc]") {
    uint32_t value = 0;
    Assembler as(reinterpret_cast<uint8_t*>(&value), sizeof(value));

    as.C_FLW(f15, 16, x15);
    REQUIRE(value == 0x6B9C);

    as.RewindBuffer();

    as.C_FLW(f15, 24, x15);
    REQUIRE(value == 0x6F9C);
}

TEST_CASE("C.FSD", "[rvc]") {
    uint32_t value = 0;
    Assembler as(reinterpret_cast<uint8_t*>(&value), sizeof(value));

    as.C_FSD(f15, 8, x15);
    REQUIRE(value == 0xA79C);

    as.RewindBuffer();

    as.C_FSD(f15, 24, x15);
    REQUIRE(value == 0xAF9C);
}

TEST_CASE("C.LD", "[rvc]") {
    uint32_t value = 0;
    Assembler as(reinterpret_cast<uint8_t*>(&value), sizeof(value));

    as.C_LD(x15, 8, x15);
    REQUIRE(value == 0x679C);

    as.RewindBuffer();

    as.C_LD(x15, 24, x15);
    REQUIRE(value == 0x6F9C);
}

TEST_CASE("C.LQ", "[rvc]") {
    uint32_t value = 0;
    Assembler as(reinterpret_cast<uint8_t*>(&value), sizeof(value));

    as.C_LQ(x15, 16, x15);
    REQUIRE(value == 0x2B9C);

    as.RewindBuffer();

    as.C_LQ(x15, 256, x15);
    REQUIRE(value == 0x279C);
}

TEST_CASE("C.LW", "[rvc]") {
    uint32_t value = 0;
    Assembler as(reinterpret_cast<uint8_t*>(&value), sizeof(value));

    as.C_LW(x15, 16, x15);
    REQUIRE(value == 0x4B9C);

    as.RewindBuffer();

    as.C_LW(x15, 24, x15);
    REQUIRE(value == 0x4F9C);
}

TEST_CASE("C.SQ", "[rvc]") {
    uint32_t value = 0;
    Assembler as(reinterpret_cast<uint8_t*>(&value), sizeof(value));

    as.C_SQ(x15, 16, x15);
    REQUIRE(value == 0xAB9C);

    as.RewindBuffer();

    as.C_SQ(x15, 256, x15);
    REQUIRE(value == 0xA79C);
}

TEST_CASE("C.UNDEF", "[rvc]") {
    uint32_t value = 0;
    Assembler as(reinterpret_cast<uint8_t*>(&value), sizeof(value));

    as.C_UNDEF();
    REQUIRE(value == 0);
}
