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

TEST_CASE("CBO.INVAL", "[cmo]") {
    uint32_t value = 0;
    Assembler as(reinterpret_cast<uint8_t*>(&value), sizeof(value));

    as.CBO_INVAL(x0);
    REQUIRE(value == 0x0000200F);

    as.RewindBuffer();

    as.CBO_INVAL(x31);
    REQUIRE(value == 0x000FA00F);
}

TEST_CASE("CBO.ZERO", "[cmo]") {
    uint32_t value = 0;
    Assembler as(reinterpret_cast<uint8_t*>(&value), sizeof(value));

    as.CBO_ZERO(x0);
    REQUIRE(value == 0x0040200F);

    as.RewindBuffer();

    as.CBO_ZERO(x31);
    REQUIRE(value == 0x004FA00F);
}

TEST_CASE("PREFETCH.I", "[cmo]") {
    uint32_t value = 0;
    Assembler as(reinterpret_cast<uint8_t*>(&value), sizeof(value));

    as.PREFETCH_I(x0);
    REQUIRE(value == 0x00006013);

    as.RewindBuffer();

    as.PREFETCH_I(x31, 2016);
    REQUIRE(value == 0x7E0FE013);

    as.RewindBuffer();

    as.PREFETCH_I(x31, -2016);
    REQUIRE(value == 0x820FE013);
}

TEST_CASE("PREFETCH.R", "[cmo]") {
    uint32_t value = 0;
    Assembler as(reinterpret_cast<uint8_t*>(&value), sizeof(value));

    as.PREFETCH_R(x0);
    REQUIRE(value == 0x00106013);

    as.RewindBuffer();

    as.PREFETCH_R(x31, 2016);
    REQUIRE(value == 0x7E1FE013);

    as.RewindBuffer();

    as.PREFETCH_R(x31, -2016);
    REQUIRE(value == 0x821FE013);
}
