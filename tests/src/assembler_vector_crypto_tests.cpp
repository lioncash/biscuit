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

TEST_CASE("VBREV.V", "[Zvbb]") {
    uint32_t value = 0;
    Assembler as(reinterpret_cast<uint8_t*>(&value), sizeof(value));

    as.VBREV(v20, v12, VecMask::Yes);
    REQUIRE(value == 0x48C52A57);

    as.RewindBuffer();

    as.VBREV(v20, v12, VecMask::No);
    REQUIRE(value == 0x4AC52A57);
}

TEST_CASE("VBREV8.V", "[Zvbb]") {
    uint32_t value = 0;
    Assembler as(reinterpret_cast<uint8_t*>(&value), sizeof(value));

    as.VBREV8(v20, v12, VecMask::Yes);
    REQUIRE(value == 0x48C42A57);

    as.RewindBuffer();

    as.VBREV8(v20, v12, VecMask::No);
    REQUIRE(value == 0x4AC42A57);
}

TEST_CASE("VREV8.V", "[Zvbb]") {
    uint32_t value = 0;
    Assembler as(reinterpret_cast<uint8_t*>(&value), sizeof(value));

    as.VREV8(v20, v12, VecMask::Yes);
    REQUIRE(value == 0x48C4AA57);

    as.RewindBuffer();

    as.VREV8(v20, v12, VecMask::No);
    REQUIRE(value == 0x4AC4AA57);
}

TEST_CASE("VCLZ.V", "[Zvbb]") {
    uint32_t value = 0;
    Assembler as(reinterpret_cast<uint8_t*>(&value), sizeof(value));

    as.VCLZ(v20, v12, VecMask::Yes);
    REQUIRE(value == 0x48C62A57);

    as.RewindBuffer();

    as.VCLZ(v20, v12, VecMask::No);
    REQUIRE(value == 0x4AC62A57);
}

TEST_CASE("VCTZ.V", "[Zvbb]") {
    uint32_t value = 0;
    Assembler as(reinterpret_cast<uint8_t*>(&value), sizeof(value));

    as.VCTZ(v20, v12, VecMask::Yes);
    REQUIRE(value == 0x48C6AA57);

    as.RewindBuffer();

    as.VCTZ(v20, v12, VecMask::No);
    REQUIRE(value == 0x4AC6AA57);
}
