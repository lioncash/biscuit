#include <catch/catch.hpp>

#include <biscuit/assembler.hpp>

using namespace biscuit;

TEST_CASE("ADD.UW", "[rvb]") {
    uint32_t value = 0;
    Assembler as(reinterpret_cast<uint8_t*>(&value), sizeof(value));

    as.ADDUW(x31, x7, x15);
    REQUIRE(value == 0x08F38FBB);

    as.RewindBuffer();

    // Pseudo instruction

    as.ZEXTW(x31, x7);
    REQUIRE(value == 0x08038FBB);
}

TEST_CASE("ANDN", "[rvb]") {
    uint32_t value = 0;
    Assembler as(reinterpret_cast<uint8_t*>(&value), sizeof(value));

    as.ANDN(x31, x7, x15);
    REQUIRE(value == 0x40F3FFB3);
}

TEST_CASE("BCLR", "[rvb]") {
    uint32_t value = 0;
    Assembler as(reinterpret_cast<uint8_t*>(&value), sizeof(value));

    as.BCLR(x31, x7, x15);
    REQUIRE(value == 0x48F39FB3);
}

TEST_CASE("BCLRI", "[rvb]") {
    uint32_t value = 0;
    Assembler as(reinterpret_cast<uint8_t*>(&value), sizeof(value));

    as.BCLRI(x31, x7, 0);
    REQUIRE(value == 0x48039F93);

    as.RewindBuffer();

    as.BCLRI(x31, x7, 63);
    REQUIRE(value == 0x4BF39F93);
}

TEST_CASE("BEXT", "[rvb]") {
    uint32_t value = 0;
    Assembler as(reinterpret_cast<uint8_t*>(&value), sizeof(value));

    as.BEXT(x31, x7, x15);
    REQUIRE(value == 0x48F3DFB3);
}

TEST_CASE("BEXTI", "[rvb]") {
    uint32_t value = 0;
    Assembler as(reinterpret_cast<uint8_t*>(&value), sizeof(value));

    as.BEXTI(x31, x7, 0);
    REQUIRE(value == 0x4803DF93);

    as.RewindBuffer();

    as.BEXTI(x31, x7, 63);
    REQUIRE(value == 0x4BF3DF93);
}

TEST_CASE("BINV", "[rvb]") {
    uint32_t value = 0;
    Assembler as(reinterpret_cast<uint8_t*>(&value), sizeof(value));

    as.BINV(x31, x7, x15);
    REQUIRE(value == 0x68F39FB3);
}

TEST_CASE("BINVI", "[rvb]") {
    uint32_t value = 0;
    Assembler as(reinterpret_cast<uint8_t*>(&value), sizeof(value));

    as.BINVI(x31, x7, 0);
    REQUIRE(value == 0x68039F93);

    as.RewindBuffer();

    as.BINVI(x31, x7, 63);
    REQUIRE(value == 0x6BF39F93);
}

TEST_CASE("BSET", "[rvb]") {
    uint32_t value = 0;
    Assembler as(reinterpret_cast<uint8_t*>(&value), sizeof(value));

    as.BSET(x31, x7, x15);
    REQUIRE(value == 0x28F39FB3);
}

TEST_CASE("BSETI", "[rvb]") {
    uint32_t value = 0;
    Assembler as(reinterpret_cast<uint8_t*>(&value), sizeof(value));

    as.BSETI(x31, x7, 0);
    REQUIRE(value == 0x28039FB3);

    as.RewindBuffer();

    as.BSETI(x31, x7, 63);
    REQUIRE(value == 0x2BF39FB3);
}
