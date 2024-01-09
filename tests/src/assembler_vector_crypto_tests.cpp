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

TEST_CASE("VCPOP.V", "[Zvbb]") {
    uint32_t value = 0;
    Assembler as(reinterpret_cast<uint8_t*>(&value), sizeof(value));

    as.VCPOP(v20, v12, VecMask::Yes);
    REQUIRE(value == 0x48C72A57);

    as.RewindBuffer();

    as.VCPOP(v20, v12, VecMask::No);
    REQUIRE(value == 0x4AC72A57);
}

TEST_CASE("VROL.VV", "[Zvbb]") {
    uint32_t value = 0;
    Assembler as(reinterpret_cast<uint8_t*>(&value), sizeof(value));

    as.VROL(v20, v12, v10, VecMask::Yes);
    REQUIRE(value == 0x54C50A57);

    as.RewindBuffer();

    as.VROL(v20, v12, v10, VecMask::No);
    REQUIRE(value == 0x56C50A57);
}

TEST_CASE("VROL.VX", "[Zvbb]") {
    uint32_t value = 0;
    Assembler as(reinterpret_cast<uint8_t*>(&value), sizeof(value));

    as.VROL(v20, v12, x10, VecMask::Yes);
    REQUIRE(value == 0x54C54A57);

    as.RewindBuffer();

    as.VROL(v20, v12, x10, VecMask::No);
    REQUIRE(value == 0x56C54A57);
}

TEST_CASE("VROR.VV", "[Zvbb]") {
    uint32_t value = 0;
    Assembler as(reinterpret_cast<uint8_t*>(&value), sizeof(value));

    as.VROR(v20, v12, v10, VecMask::Yes);
    REQUIRE(value == 0x50C50A57);

    as.RewindBuffer();

    as.VROR(v20, v12, v10, VecMask::No);
    REQUIRE(value == 0x52C50A57);
}

TEST_CASE("VROR.VX", "[Zvbb]") {
    uint32_t value = 0;
    Assembler as(reinterpret_cast<uint8_t*>(&value), sizeof(value));

    as.VROR(v20, v12, x10, VecMask::Yes);
    REQUIRE(value == 0x50C54A57);

    as.RewindBuffer();

    as.VROR(v20, v12, x10, VecMask::No);
    REQUIRE(value == 0x52C54A57);
}

TEST_CASE("VROR.VI", "[Zvbb]") {
    uint32_t value = 0;
    Assembler as(reinterpret_cast<uint8_t*>(&value), sizeof(value));

    as.VROR(v20, v12, 63, VecMask::Yes);
    REQUIRE(value == 0x54CFBA57);

    as.RewindBuffer();

    as.VROR(v20, v12, 31, VecMask::Yes);
    REQUIRE(value == 0x50CFBA57);

    as.RewindBuffer();

    as.VROR(v20, v12, 63, VecMask::No);
    REQUIRE(value == 0x56CFBA57);

    as.RewindBuffer();

    as.VROR(v20, v12, 31, VecMask::No);
    REQUIRE(value == 0x52CFBA57);
}
