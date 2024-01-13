#include <catch/catch.hpp>

#include <biscuit/assembler.hpp>

using namespace biscuit;

TEST_CASE("FCVT.BF16.S", "[Zfbfmin]") {
    uint32_t value = 0;
    Assembler as(reinterpret_cast<uint8_t*>(&value), sizeof(value));

    as.FCVT_BF16_S(f31, f7, RMode::RNE);
    REQUIRE(value == 0x44838FD3);

    as.RewindBuffer();

    as.FCVT_BF16_S(f31, f7, RMode::RMM);
    REQUIRE(value == 0x4483CFD3);

    as.RewindBuffer();

    as.FCVT_BF16_S(f31, f7, RMode::DYN);
    REQUIRE(value == 0x4483FFD3);
}

TEST_CASE("FCVT.S.BF16", "[Zfbfmin]") {
    uint32_t value = 0;
    Assembler as(reinterpret_cast<uint8_t*>(&value), sizeof(value));

    as.FCVT_S_BF16(f31, f7, RMode::RNE);
    REQUIRE(value == 0x40638FD3);

    as.RewindBuffer();

    as.FCVT_S_BF16(f31, f7, RMode::RMM);
    REQUIRE(value == 0x4063CFD3);

    as.RewindBuffer();

    as.FCVT_S_BF16(f31, f7, RMode::DYN);
    REQUIRE(value == 0x4063FFD3);
}

TEST_CASE("VFNCVTBF16.F.F.W", "[Zvfbfmin]") {
    uint32_t value = 0;
    Assembler as(reinterpret_cast<uint8_t*>(&value), sizeof(value));

    as.VFNCVTBF16_F_F_W(v31, v7, VecMask::Yes);
    REQUIRE(value == 0x487E9FD7);

    as.RewindBuffer();

    as.VFNCVTBF16_F_F_W(v31, v7, VecMask::No);
    REQUIRE(value == 0x4A7E9FD7);
}

TEST_CASE("VFWCVTBF16.F.F.V", "[Zvfbfmin]") {
    uint32_t value = 0;
    Assembler as(reinterpret_cast<uint8_t*>(&value), sizeof(value));

    as.VFWCVTBF16_F_F_V(v31, v7, VecMask::Yes);
    REQUIRE(value == 0x48769FD7);

    as.RewindBuffer();

    as.VFWCVTBF16_F_F_V(v31, v7, VecMask::No);
    REQUIRE(value == 0x4A769FD7);
}