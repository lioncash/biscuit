#include <catch/catch.hpp>

#include <biscuit/assembler.hpp>

using namespace biscuit;

TEST_CASE("AES32DSI", "[rvk]") {
    uint32_t value = 0;
    Assembler as(reinterpret_cast<uint8_t*>(&value), sizeof(value));

    as.AES32DSI(x31, x31, x31, 0b11);
    REQUIRE(value == 0xEBFF8FB3);

    as.RewindBuffer();

    as.AES32DSI(x1, x2, x3, 0b10);
    REQUIRE(value == 0xAA3100B3);
}

TEST_CASE("AES32DSMI", "[rvk]") {
    uint32_t value = 0;
    Assembler as(reinterpret_cast<uint8_t*>(&value), sizeof(value));

    as.AES32DSMI(x31, x31, x31, 0b11);
    REQUIRE(value == 0xEFFF8FB3);

    as.RewindBuffer();

    as.AES32DSMI(x1, x2, x3, 0b10);
    REQUIRE(value == 0xAE3100B3);
}

TEST_CASE("AES32ESI", "[rvk]") {
    uint32_t value = 0;
    Assembler as(reinterpret_cast<uint8_t*>(&value), sizeof(value));

    as.AES32ESI(x31, x31, x31, 0b11);
    REQUIRE(value == 0xE3FF8FB3);

    as.RewindBuffer();

    as.AES32ESI(x1, x2, x3, 0b10);
    REQUIRE(value == 0xA23100B3);
}

TEST_CASE("AES32ESMI", "[rvk]") {
    uint32_t value = 0;
    Assembler as(reinterpret_cast<uint8_t*>(&value), sizeof(value));

    as.AES32ESMI(x31, x31, x31, 0b11);
    REQUIRE(value == 0xE7FF8FB3);

    as.RewindBuffer();

    as.AES32ESMI(x1, x2, x3, 0b10);
    REQUIRE(value == 0xA63100B3);
}

TEST_CASE("AES64DS", "[rvk]") {
    uint32_t value = 0;
    Assembler as(reinterpret_cast<uint8_t*>(&value), sizeof(value));

    as.AES64DS(x31, x31, x31);
    REQUIRE(value == 0x3BFF8FB3);

    as.RewindBuffer();

    as.AES64DS(x1, x2, x3);
    REQUIRE(value == 0x3A3100B3);
}

TEST_CASE("AES64DSM", "[rvk]") {
    uint32_t value = 0;
    Assembler as(reinterpret_cast<uint8_t*>(&value), sizeof(value));

    as.AES64DSM(x31, x31, x31);
    REQUIRE(value == 0x3FFF8FB3);

    as.RewindBuffer();

    as.AES64DSM(x1, x2, x3);
    REQUIRE(value == 0x3E3100B3);
}

TEST_CASE("AES64ES", "[rvk]") {
    uint32_t value = 0;
    Assembler as(reinterpret_cast<uint8_t*>(&value), sizeof(value));

    as.AES64ES(x31, x31, x31);
    REQUIRE(value == 0x33FF8FB3);

    as.RewindBuffer();

    as.AES64ES(x1, x2, x3);
    REQUIRE(value == 0x323100B3);
}

TEST_CASE("AES64ESM", "[rvk]") {
    uint32_t value = 0;
    Assembler as(reinterpret_cast<uint8_t*>(&value), sizeof(value));

    as.AES64ESM(x31, x31, x31);
    REQUIRE(value == 0x37FF8FB3);

    as.RewindBuffer();

    as.AES64ESM(x1, x2, x3);
    REQUIRE(value == 0x363100B3);
}

TEST_CASE("AES64IM", "[rvk]") {
    uint32_t value = 0;
    Assembler as(reinterpret_cast<uint8_t*>(&value), sizeof(value));

    as.AES64IM(x31, x31);
    REQUIRE(value == 0x300F9F93);

    as.RewindBuffer();

    as.AES64IM(x1, x2);
    REQUIRE(value == 0x30011093);
}

TEST_CASE("AES64KS1I", "[rvk]") {
    uint32_t value = 0;
    Assembler as(reinterpret_cast<uint8_t*>(&value), sizeof(value));

    as.AES64KS1I(x31, x31, 0xA);
    REQUIRE(value == 0x31AF9F93);

    as.RewindBuffer();

    as.AES64KS1I(x1, x2, 0x5);
    REQUIRE(value == 0x31511093);
}
