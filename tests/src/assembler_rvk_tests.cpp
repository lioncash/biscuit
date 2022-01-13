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
