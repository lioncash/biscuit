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

TEST_CASE("FCLASS.S", "[rv32f]") {
    uint32_t value = 0;
    Assembler as(reinterpret_cast<uint8_t*>(&value), sizeof(value));

    as.FCLASS_S(x31, f7);
    REQUIRE(value == 0xE0039FD3);

    as.RewindBuffer();

    as.FCLASS_S(x7, f31);
    REQUIRE(value == 0xE00F93D3);
}

TEST_CASE("FCVT.S.W", "[rv32f]") {
    uint32_t value = 0;
    Assembler as(reinterpret_cast<uint8_t*>(&value), sizeof(value));

    as.FCVT_S_W(f31, x7, Assembler::RMode::RNE);
    REQUIRE(value == 0xD0038FD3);

    as.RewindBuffer();

    as.FCVT_S_W(f31, x7, Assembler::RMode::RMM);
    REQUIRE(value == 0xD003CFD3);

    as.RewindBuffer();

    as.FCVT_S_W(f31, x7, Assembler::RMode::DYN);
    REQUIRE(value == 0xD003FFD3);
}

TEST_CASE("FCVT.S.WU", "[rv32f]") {
    uint32_t value = 0;
    Assembler as(reinterpret_cast<uint8_t*>(&value), sizeof(value));

    as.FCVT_S_WU(f31, x7, Assembler::RMode::RNE);
    REQUIRE(value == 0xD0138FD3);

    as.RewindBuffer();

    as.FCVT_S_WU(f31, x7, Assembler::RMode::RMM);
    REQUIRE(value == 0xD013CFD3);

    as.RewindBuffer();

    as.FCVT_S_WU(f31, x7, Assembler::RMode::DYN);
    REQUIRE(value == 0xD013FFD3);
}

TEST_CASE("FCVT.L.S", "[rv64f]") {
    uint32_t value = 0;
    Assembler as(reinterpret_cast<uint8_t*>(&value), sizeof(value));

    as.FCVT_L_S(x31, f7, Assembler::RMode::RNE);
    REQUIRE(value == 0xC0238FD3);

    as.RewindBuffer();

    as.FCVT_L_S(x31, f7, Assembler::RMode::RMM);
    REQUIRE(value == 0xC023CFD3);

    as.RewindBuffer();

    as.FCVT_L_S(x31, f7, Assembler::RMode::DYN);
    REQUIRE(value == 0xC023FFD3);
}

TEST_CASE("FCVT.LU.S", "[rv64f]") {
    uint32_t value = 0;
    Assembler as(reinterpret_cast<uint8_t*>(&value), sizeof(value));

    as.FCVT_LU_S(x31, f7, Assembler::RMode::RNE);
    REQUIRE(value == 0xC0338FD3);

    as.RewindBuffer();

    as.FCVT_LU_S(x31, f7, Assembler::RMode::RMM);
    REQUIRE(value == 0xC033CFD3);

    as.RewindBuffer();

    as.FCVT_LU_S(x31, f7, Assembler::RMode::DYN);
    REQUIRE(value == 0xC033FFD3);
}

TEST_CASE("FCVT.S.L", "[rv64f]") {
    uint32_t value = 0;
    Assembler as(reinterpret_cast<uint8_t*>(&value), sizeof(value));

    as.FCVT_S_L(f31, x7, Assembler::RMode::RNE);
    REQUIRE(value == 0xD0238FD3);

    as.RewindBuffer();

    as.FCVT_S_L(f31, x7, Assembler::RMode::RMM);
    REQUIRE(value == 0xD023CFD3);

    as.RewindBuffer();

    as.FCVT_S_L(f31, x7, Assembler::RMode::DYN);
    REQUIRE(value == 0xD023FFD3);
}

TEST_CASE("FCVT.S.LU", "[rv64f]") {
    uint32_t value = 0;
    Assembler as(reinterpret_cast<uint8_t*>(&value), sizeof(value));

    as.FCVT_S_LU(f31, x7, Assembler::RMode::RNE);
    REQUIRE(value == 0xD0338FD3);

    as.RewindBuffer();

    as.FCVT_S_LU(f31, x7, Assembler::RMode::RMM);
    REQUIRE(value == 0xD033CFD3);

    as.RewindBuffer();

    as.FCVT_S_LU(f31, x7, Assembler::RMode::DYN);
    REQUIRE(value == 0xD033FFD3);
}

TEST_CASE("FCVT.W.S", "[rv32f]") {
    uint32_t value = 0;
    Assembler as(reinterpret_cast<uint8_t*>(&value), sizeof(value));

    as.FCVT_W_S(x31, f7, Assembler::RMode::RNE);
    REQUIRE(value == 0xC0038FD3);

    as.RewindBuffer();

    as.FCVT_W_S(x31, f7, Assembler::RMode::RMM);
    REQUIRE(value == 0xC003CFD3);

    as.RewindBuffer();

    as.FCVT_W_S(x31, f7, Assembler::RMode::DYN);
    REQUIRE(value == 0xC003FFD3);
}

TEST_CASE("FCVT.WU.S", "[rv32f]") {
    uint32_t value = 0;
    Assembler as(reinterpret_cast<uint8_t*>(&value), sizeof(value));

    as.FCVT_WU_S(x31, f7, Assembler::RMode::RNE);
    REQUIRE(value == 0xC0138FD3);

    as.RewindBuffer();

    as.FCVT_WU_S(x31, f7, Assembler::RMode::RMM);
    REQUIRE(value == 0xC013CFD3);

    as.RewindBuffer();

    as.FCVT_WU_S(x31, f7, Assembler::RMode::DYN);
    REQUIRE(value == 0xC013FFD3);
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

TEST_CASE("FEQ.S", "[rv32f]") {
    uint32_t value = 0;
    Assembler as(reinterpret_cast<uint8_t*>(&value), sizeof(value));

    as.FEQ_S(x31, f7, f26);
    REQUIRE(value == 0xA1A3AFD3);

    as.RewindBuffer();

    as.FEQ_S(x31, f26, f7);
    REQUIRE(value == 0xA07D2FD3);
}

TEST_CASE("FLE.S", "[rv32f]") {
    uint32_t value = 0;
    Assembler as(reinterpret_cast<uint8_t*>(&value), sizeof(value));

    as.FLE_S(x31, f7, f26);
    REQUIRE(value == 0xA1A38FD3);

    as.RewindBuffer();

    as.FLE_S(x31, f26, f7);
    REQUIRE(value == 0xA07D0FD3);
}

TEST_CASE("FLT.S", "[rv32f]") {
    uint32_t value = 0;
    Assembler as(reinterpret_cast<uint8_t*>(&value), sizeof(value));

    as.FLT_S(x31, f7, f26);
    REQUIRE(value == 0xA1A39FD3);

    as.RewindBuffer();

    as.FLT_S(x31, f26, f7);
    REQUIRE(value == 0xA07D1FD3);
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

TEST_CASE("FMV.W.X", "[rv32f]") {
    uint32_t value = 0;
    Assembler as(reinterpret_cast<uint8_t*>(&value), sizeof(value));

    as.FMV_W_X(f31, x7);
    REQUIRE(value == 0xF0038FD3);

    as.RewindBuffer();

    as.FMV_W_X(f7, x31);
    REQUIRE(value == 0xF00F83D3);
}

TEST_CASE("FMV.X.W", "[rv32f]") {
    uint32_t value = 0;
    Assembler as(reinterpret_cast<uint8_t*>(&value), sizeof(value));

    as.FMV_X_W(x31, f7);
    REQUIRE(value == 0xE0038FD3);

    as.RewindBuffer();

    as.FMV_X_W(x7, f31);
    REQUIRE(value == 0xE00F83D3);
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
