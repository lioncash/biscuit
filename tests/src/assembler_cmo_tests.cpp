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

TEST_CASE("CBO.FLUSH", "[cmo]") {
    uint32_t value = 0;
    Assembler as(reinterpret_cast<uint8_t*>(&value), sizeof(value));

    as.CBO_FLUSH(x0);
    REQUIRE(value == 0x0020200F);

    as.RewindBuffer();

    as.CBO_FLUSH(x31);
    REQUIRE(value == 0x002FA00F);
}
