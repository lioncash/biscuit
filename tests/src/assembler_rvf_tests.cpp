#include <catch/catch.hpp>

#include <biscuit/assembler.hpp>

using namespace biscuit;

TEST_CASE("FADD.S", "[rv32f]") {
    uint32_t value = 0;
    Assembler as(reinterpret_cast<uint8_t*>(&value), sizeof(value));

    as.FADD_S(f31, f7, f26, Assembler::RMode::RNE);
    REQUIRE(value == 0x01A38FD3);

    as.RewindBuffer();

    as.FADD_S(f31, f7, f26, Assembler::RMode::RMM);
    REQUIRE(value == 0x01A3CFD3);

    as.RewindBuffer();

    as.FADD_S(f31, f7, f26, Assembler::RMode::DYN);
    REQUIRE(value == 0x01A3FFD3);
}

TEST_CASE("FDIV.S", "[rv32f]") {
    uint32_t value = 0;
    Assembler as(reinterpret_cast<uint8_t*>(&value), sizeof(value));

    as.FDIV_S(f31, f7, f26, Assembler::RMode::RNE);
    REQUIRE(value == 0x19A38FD3);

    as.RewindBuffer();

    as.FDIV_S(f31, f7, f26, Assembler::RMode::RMM);
    REQUIRE(value == 0x19A3CFD3);

    as.RewindBuffer();

    as.FDIV_S(f31, f7, f26, Assembler::RMode::DYN);
    REQUIRE(value == 0x19A3FFD3);
}

TEST_CASE("FLW", "[rv32f]") {
    uint32_t value = 0;
    Assembler as(reinterpret_cast<uint8_t*>(&value), sizeof(value));

    as.FLW(f15, 1024, x31);
    REQUIRE(value == 0x400FA787);

    as.RewindBuffer();

    as.FLW(f15, 2048, x31);
    REQUIRE(value == 0x800FA787);

    as.RewindBuffer();

    as.FLW(f15, 4095, x31);
    REQUIRE(value == 0xFFFFA787);
}

TEST_CASE("FMADD.S", "[rv32f]") {
    uint32_t value = 0;
    Assembler as(reinterpret_cast<uint8_t*>(&value), sizeof(value));

    as.FMADD_S(f15, f31, f7, f26, Assembler::RMode::RNE);
    REQUIRE(value == 0xD07F87C3);

    as.RewindBuffer();

    as.FMADD_S(f15, f31, f7, f26, Assembler::RMode::RMM);
    REQUIRE(value == 0xD07FC7C3);

    as.RewindBuffer();

    as.FMADD_S(f15, f31, f7, f26, Assembler::RMode::DYN);
    REQUIRE(value == 0xD07FF7C3);
}

TEST_CASE("FMAX.S", "[rv32f]") {
    uint32_t value = 0;
    Assembler as(reinterpret_cast<uint8_t*>(&value), sizeof(value));

    as.FMAX_S(f31, f7, f26);
    REQUIRE(value == 0x29A39FD3);

    as.RewindBuffer();

    as.FMAX_S(f31, f31, f31);
    REQUIRE(value == 0x29FF9FD3);
}

TEST_CASE("FMIN.S", "[rv32f]") {
    uint32_t value = 0;
    Assembler as(reinterpret_cast<uint8_t*>(&value), sizeof(value));

    as.FMIN_S(f31, f7, f26);
    REQUIRE(value == 0x29A38FD3);

    as.RewindBuffer();

    as.FMIN_S(f31, f31, f31);
    REQUIRE(value == 0x29FF8FD3);
}

TEST_CASE("FMSUB.S", "[rv32f]") {
    uint32_t value = 0;
    Assembler as(reinterpret_cast<uint8_t*>(&value), sizeof(value));

    as.FMSUB_S(f15, f31, f7, f26, Assembler::RMode::RNE);
    REQUIRE(value == 0xD07F87C7);

    as.RewindBuffer();

    as.FMSUB_S(f15, f31, f7, f26, Assembler::RMode::RMM);
    REQUIRE(value == 0xD07FC7C7);

    as.RewindBuffer();

    as.FMSUB_S(f15, f31, f7, f26, Assembler::RMode::DYN);
    REQUIRE(value == 0xD07FF7C7);
}

TEST_CASE("FMUL.S", "[rv32f]") {
    uint32_t value = 0;
    Assembler as(reinterpret_cast<uint8_t*>(&value), sizeof(value));

    as.FMUL_S(f31, f7, f26, Assembler::RMode::RNE);
    REQUIRE(value == 0x11A38FD3);

    as.RewindBuffer();

    as.FMUL_S(f31, f7, f26, Assembler::RMode::RMM);
    REQUIRE(value == 0x11A3CFD3);

    as.RewindBuffer();

    as.FMUL_S(f31, f7, f26, Assembler::RMode::DYN);
    REQUIRE(value == 0x11A3FFD3);
}

TEST_CASE("FNMADD.S", "[rv32f]") {
    uint32_t value = 0;
    Assembler as(reinterpret_cast<uint8_t*>(&value), sizeof(value));

    as.FNMADD_S(f15, f31, f7, f26, Assembler::RMode::RNE);
    REQUIRE(value == 0xD07F87CF);

    as.RewindBuffer();

    as.FNMADD_S(f15, f31, f7, f26, Assembler::RMode::RMM);
    REQUIRE(value == 0xD07FC7CF);

    as.RewindBuffer();

    as.FNMADD_S(f15, f31, f7, f26, Assembler::RMode::DYN);
    REQUIRE(value == 0xD07FF7CF);
}

TEST_CASE("FNMSUB.S", "[rv32f]") {
    uint32_t value = 0;
    Assembler as(reinterpret_cast<uint8_t*>(&value), sizeof(value));

    as.FNMSUB_S(f15, f31, f7, f26, Assembler::RMode::RNE);
    REQUIRE(value == 0xD07F87CB);

    as.RewindBuffer();

    as.FNMSUB_S(f15, f31, f7, f26, Assembler::RMode::RMM);
    REQUIRE(value == 0xD07FC7CB);

    as.RewindBuffer();

    as.FNMSUB_S(f15, f31, f7, f26, Assembler::RMode::DYN);
    REQUIRE(value == 0xD07FF7CB);
}

TEST_CASE("FSGNJ.S", "[rv32f]") {
    uint32_t value = 0;
    Assembler as(reinterpret_cast<uint8_t*>(&value), sizeof(value));

    as.FSGNJ_S(f31, f7, f26);
    REQUIRE(value == 0x21A38FD3);

    as.RewindBuffer();

    as.FSGNJ_S(f31, f31, f31);
    REQUIRE(value == 0x21FF8FD3);
}

TEST_CASE("FSGNJN.S", "[rv32f]") {
    uint32_t value = 0;
    Assembler as(reinterpret_cast<uint8_t*>(&value), sizeof(value));

    as.FSGNJN_S(f31, f7, f26);
    REQUIRE(value == 0x21A39FD3);

    as.RewindBuffer();

    as.FSGNJN_S(f31, f31, f31);
    REQUIRE(value == 0x21FF9FD3);
}

TEST_CASE("FSGNJX.S", "[rv32f]") {
    uint32_t value = 0;
    Assembler as(reinterpret_cast<uint8_t*>(&value), sizeof(value));

    as.FSGNJX_S(f31, f7, f26);
    REQUIRE(value == 0x21A3AFD3);

    as.RewindBuffer();

    as.FSGNJX_S(f31, f31, f31);
    REQUIRE(value == 0x21FFAFD3);
}

TEST_CASE("FSQRT.S", "[rv32f]") {
    uint32_t value = 0;
    Assembler as(reinterpret_cast<uint8_t*>(&value), sizeof(value));

    as.FSQRT_S(f31, f7, Assembler::RMode::RNE);
    REQUIRE(value == 0x58038FD3);

    as.RewindBuffer();

    as.FSQRT_S(f31, f7, Assembler::RMode::RMM);
    REQUIRE(value == 0x5803CFD3);

    as.RewindBuffer();

    as.FSQRT_S(f31, f7, Assembler::RMode::DYN);
    REQUIRE(value == 0x5803FFD3);
}

TEST_CASE("FSUB.S", "[rv32f]") {
    uint32_t value = 0;
    Assembler as(reinterpret_cast<uint8_t*>(&value), sizeof(value));

    as.FSUB_S(f31, f7, f26, Assembler::RMode::RNE);
    REQUIRE(value == 0x09A38FD3);

    as.RewindBuffer();

    as.FSUB_S(f31, f7, f26, Assembler::RMode::RMM);
    REQUIRE(value == 0x09A3CFD3);

    as.RewindBuffer();

    as.FSUB_S(f31, f7, f26, Assembler::RMode::DYN);
    REQUIRE(value == 0x09A3FFD3);
}

TEST_CASE("FSW", "[rv32f]") {
    uint32_t value = 0;
    Assembler as(reinterpret_cast<uint8_t*>(&value), sizeof(value));

    as.FSW(f31, 1024, x15);
    REQUIRE(value == 0x41F7A027);

    as.RewindBuffer();

    as.FSW(f31, 2048, x15);
    REQUIRE(value == 0x81F7A027);

    as.RewindBuffer();

    as.FSW(f31, 4095, x15);
    REQUIRE(value == 0xFFF7AFA7);
}
