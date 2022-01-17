#include <catch/catch.hpp>

#include <biscuit/assembler.hpp>

TEST_CASE("NTL.P1", "[Zihintntl]") {
    using namespace biscuit;

    uint32_t value = 0;
    Assembler as(reinterpret_cast<uint8_t*>(&value), sizeof(value));

    as.NTL_P1();
    REQUIRE(value == 0x00200033);
}
