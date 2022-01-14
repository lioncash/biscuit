#include <catch/catch.hpp>

#include <biscuit/assembler.hpp>

using namespace biscuit;

TEST_CASE("CBO.CLEAN", "[cmo]") {
    uint32_t value = 0;
    Assembler as(reinterpret_cast<uint8_t*>(&value), sizeof(value));

    as.CBO_CLEAN(x0);
    REQUIRE(value == 0x0010200F);

    as.RewindBuffer();

    as.CBO_CLEAN(x31);
    REQUIRE(value == 0x001FA00F);
}
