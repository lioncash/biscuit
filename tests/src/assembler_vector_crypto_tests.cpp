#include <catch/catch.hpp>

#include <biscuit/assembler.hpp>

using namespace biscuit;

TEST_CASE("VANDN.VV", "[Zvbb]") {
    uint32_t value = 0;
    Assembler as(reinterpret_cast<uint8_t*>(&value), sizeof(value));

    as.VANDN(v20, v12, v10, VecMask::Yes);
    REQUIRE(value == 0x04C50A57);

    as.RewindBuffer();

    as.VANDN(v20, v12, v10, VecMask::No);
    REQUIRE(value == 0x06C50A57);
}

TEST_CASE("VANDN.VX", "[Zvbb]") {
    uint32_t value = 0;
    Assembler as(reinterpret_cast<uint8_t*>(&value), sizeof(value));

    as.VANDN(v20, v12, x10, VecMask::Yes);
    REQUIRE(value == 0x04C54A57);

    as.RewindBuffer();

    as.VANDN(v20, v12, x10, VecMask::No);
    REQUIRE(value == 0x06C54A57);
}
