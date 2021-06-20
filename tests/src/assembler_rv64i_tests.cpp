#include <catch/catch.hpp>

#include <biscuit/assembler.hpp>

TEST_CASE("LWU", "[rv64i]") {
    uint32_t value = 0;
    biscuit::Assembler as(reinterpret_cast<uint8_t*>(&value), sizeof(value));

    as.LWU(biscuit::x15, biscuit::x31, 1024);
    REQUIRE(value == 0x400FE783);

    as.RewindBuffer();

    as.LWU(biscuit::x15, biscuit::x31, 2048);
    REQUIRE(value == 0x800FE783);

    as.RewindBuffer();

    as.LWU(biscuit::x15, biscuit::x31, 4095);
    REQUIRE(value == 0xFFFFE783);
}
