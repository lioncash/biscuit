#include <catch/catch.hpp>

#include <biscuit/assembler.hpp>

#include "assembler_test_utils.hpp"
#include "decoder_test_utils.hpp"

using namespace biscuit;

#define REQUIRE_GPR(operand, gpr, write, size) \
    REQUIRE(operand.GetType() == OperandType::GPR); \
    REQUIRE(operand.GPR() == gpr); \
    REQUIRE(operand.Width() == size); \
    REQUIRE(operand.IsWrite() == write)

#define REQUIRE_GPR64(operand, gpr, write) \
    REQUIRE_GPR(operand, gpr, write, 64)

#define REQUIRE_GPR32(operand, gpr, write) \
    REQUIRE_GPR(operand, gpr, write, 32)

#define REQUIRE_GPR16(operand, gpr, write) \
    REQUIRE_GPR(operand, gpr, write, 16)

#define REQUIRE_GPR8(operand, gpr, write) \
    REQUIRE_GPR(operand, gpr, write, 8)

#define REQUIRE_INST(mnem, ext) \
    REQUIRE(instruction.mnemonic == mnem); \
    REQUIRE(instruction.extension == ext); \
    REQUIRE(instruction.arch == ArchFeature::RV32)

#define REQUIRE_IMM(operand, imm) \
    REQUIRE(operand.GetType() == OperandType::Immediate); \
    REQUIRE(operand.Immediate() == imm)

TEST_CASE("Decoder LUI", "[rv32i-decoder]") {
    uint32_t value = 0;
    auto as = MakeAssembler32(value);

    as.LUI(x7, 0xABCDE);

    DecodedInstruction instruction;
    DecodedOperand operands[4];
    auto de = MakeDecoder32();
    DecoderStatus status = de.Decode(&value, 4, instruction, operands);

    REQUIRE(status == DecoderStatus::Ok);
    REQUIRE_INST(Mnemonic::LUI, Extension::I);
    REQUIRE_GPR32(operands[0], x7, true);
    REQUIRE_IMM(operands[1], 0xABCDE);
}

TEST_CASE("Decoder AUIPC", "[rv32i-decoder]") {
    uint32_t value = 0;
    auto as = MakeAssembler32(value);

    as.AUIPC(x7, 0xABCDE);

    DecodedInstruction instruction;
    DecodedOperand operands[4];
    auto de = MakeDecoder32();
    DecoderStatus status = de.Decode(&value, 4, instruction, operands);

    REQUIRE(status == DecoderStatus::Ok);
    REQUIRE_INST(Mnemonic::AUIPC, Extension::I);
    REQUIRE_GPR32(operands[0], x7, true);
    REQUIRE_IMM(operands[1], 0xABCDE);
}

TEST_CASE("Decoder JAL", "[rv32i-decoder]") {
    uint32_t value = 0;
    auto as = MakeAssembler32(value);

    as.JAL(x7, -1024);

    DecodedInstruction instruction;
    DecodedOperand operands[4];
    auto de = MakeDecoder32();
    DecoderStatus status = de.Decode(&value, 4, instruction, operands);

    REQUIRE(status == DecoderStatus::Ok);
    REQUIRE_INST(Mnemonic::JAL, Extension::I);
    REQUIRE_GPR32(operands[0], x7, true);
    REQUIRE_IMM(operands[1], -1024);
}

TEST_CASE("Decoder JALR", "[rv32i-decoder]") {
    uint32_t value = 0;
    auto as = MakeAssembler32(value);

    as.JALR(x7, -1024, x15);

    DecodedInstruction instruction;
    DecodedOperand operands[4];
    auto de = MakeDecoder32();
    DecoderStatus status = de.Decode(&value, 4, instruction, operands);

    REQUIRE(status == DecoderStatus::Ok);
    REQUIRE_INST(Mnemonic::JALR, Extension::I);
    REQUIRE_GPR32(operands[0], x7, true);
    REQUIRE_GPR32(operands[1], x15, false);
    REQUIRE_IMM(operands[2], -1024);
}

TEST_CASE("Decoder BEQ", "[rv32i-decoder]") {
    uint32_t value = 0;
    auto as = MakeAssembler32(value);

    as.BEQ(x7, x15, -1024);

    DecodedInstruction instruction;
    DecodedOperand operands[4];
    auto de = MakeDecoder32();
    DecoderStatus status = de.Decode(&value, 4, instruction, operands);

    REQUIRE(status == DecoderStatus::Ok);
    REQUIRE_INST(Mnemonic::BEQ, Extension::I);
    REQUIRE_GPR32(operands[0], x7, false);
    REQUIRE_GPR32(operands[1], x15, false);
    REQUIRE_IMM(operands[2], -1024);
}

TEST_CASE("Decoder BNE", "[rv32i-decoder]") {
    uint32_t value = 0;
    auto as = MakeAssembler32(value);

    as.BNE(x7, x15, -1024);

    DecodedInstruction instruction;
    DecodedOperand operands[4];
    auto de = MakeDecoder32();
    DecoderStatus status = de.Decode(&value, 4, instruction, operands);

    REQUIRE(status == DecoderStatus::Ok);
    REQUIRE_INST(Mnemonic::BNE, Extension::I);
    REQUIRE_GPR32(operands[0], x7, false);
    REQUIRE_GPR32(operands[1], x15, false);
    REQUIRE_IMM(operands[2], -1024);
}

TEST_CASE("Decoder BLT", "[rv32i-decoder]") {
    uint32_t value = 0;
    auto as = MakeAssembler32(value);

    as.BLT(x7, x15, -1024);

    DecodedInstruction instruction;
    DecodedOperand operands[4];
    auto de = MakeDecoder32();
    DecoderStatus status = de.Decode(&value, 4, instruction, operands);

    REQUIRE(status == DecoderStatus::Ok);
    REQUIRE_INST(Mnemonic::BLT, Extension::I);
    REQUIRE_GPR32(operands[0], x7, false);
    REQUIRE_GPR32(operands[1], x15, false);
    REQUIRE_IMM(operands[2], -1024);
}

TEST_CASE("Decoder BGE", "[rv32i-decoder]") {
    uint32_t value = 0;
    auto as = MakeAssembler32(value);

    as.BGE(x7, x15, -1024);

    DecodedInstruction instruction;
    DecodedOperand operands[4];
    auto de = MakeDecoder32();
    DecoderStatus status = de.Decode(&value, 4, instruction, operands);

    REQUIRE(status == DecoderStatus::Ok);
    REQUIRE_INST(Mnemonic::BGE, Extension::I);
    REQUIRE_GPR32(operands[0], x7, false);
    REQUIRE_GPR32(operands[1], x15, false);
    REQUIRE_IMM(operands[2], -1024);
}

TEST_CASE("Decoder BLTU", "[rv32i-decoder]") {
    uint32_t value = 0;
    auto as = MakeAssembler32(value);

    as.BLTU(x7, x15, -1024);

    DecodedInstruction instruction;
    DecodedOperand operands[4];
    auto de = MakeDecoder32();
    DecoderStatus status = de.Decode(&value, 4, instruction, operands);

    REQUIRE(status == DecoderStatus::Ok);
    REQUIRE_INST(Mnemonic::BLTU, Extension::I);
    REQUIRE_GPR32(operands[0], x7, false);
    REQUIRE_GPR32(operands[1], x15, false);
    REQUIRE_IMM(operands[2], -1024);
}

TEST_CASE("Decoder BGEU", "[rv32i-decoder]") {
    uint32_t value = 0;
    auto as = MakeAssembler32(value);

    as.BGEU(x7, x15, -1024);

    DecodedInstruction instruction;
    DecodedOperand operands[4];
    auto de = MakeDecoder32();
    DecoderStatus status = de.Decode(&value, 4, instruction, operands);

    REQUIRE(status == DecoderStatus::Ok);
    REQUIRE_INST(Mnemonic::BGEU, Extension::I);
    REQUIRE_GPR32(operands[0], x7, false);
    REQUIRE_GPR32(operands[1], x15, false);
    REQUIRE_IMM(operands[2], -1024);
}

TEST_CASE("Decoder LB", "[rv32i-decoder]") {
    uint32_t value = 0;
    auto as = MakeAssembler32(value);

    as.LB(x7, -1024, x15);

    DecodedInstruction instruction;
    DecodedOperand operands[4];
    auto de = MakeDecoder32();
    DecoderStatus status = de.Decode(&value, 4, instruction, operands);

    REQUIRE(status == DecoderStatus::Ok);
    REQUIRE_INST(Mnemonic::LB, Extension::I);
    REQUIRE_GPR8(operands[0], x7, true);
    REQUIRE_GPR32(operands[1], x15, false);
    REQUIRE_IMM(operands[2], -1024);
}

TEST_CASE("Decoder LBU", "[rv32i-decoder]") {
    uint32_t value = 0;
    auto as = MakeAssembler32(value);

    as.LBU(x7, -1024, x15);

    DecodedInstruction instruction;
    DecodedOperand operands[4];
    auto de = MakeDecoder32();
    DecoderStatus status = de.Decode(&value, 4, instruction, operands);

    REQUIRE(status == DecoderStatus::Ok);
    REQUIRE_INST(Mnemonic::LBU, Extension::I);
    REQUIRE_GPR8(operands[0], x7, true);
    REQUIRE_GPR32(operands[1], x15, false);
    REQUIRE_IMM(operands[2], -1024);
}

TEST_CASE("Decoder LH", "[rv32i-decoder]") {
    uint32_t value = 0;
    auto as = MakeAssembler32(value);

    as.LH(x7, -1024, x15);

    DecodedInstruction instruction;
    DecodedOperand operands[4];
    auto de = MakeDecoder32();
    DecoderStatus status = de.Decode(&value, 4, instruction, operands);

    REQUIRE(status == DecoderStatus::Ok);
    REQUIRE_INST(Mnemonic::LH, Extension::I);
    REQUIRE_GPR16(operands[0], x7, true);
    REQUIRE_GPR32(operands[1], x15, false);
    REQUIRE_IMM(operands[2], -1024);
}

TEST_CASE("Decoder LHU", "[rv32i-decoder]") {
    uint32_t value = 0;
    auto as = MakeAssembler32(value);

    as.LHU(x7, -1024, x15);

    DecodedInstruction instruction;
    DecodedOperand operands[4];
    auto de = MakeDecoder32();
    DecoderStatus status = de.Decode(&value, 4, instruction, operands);

    REQUIRE(status == DecoderStatus::Ok);
    REQUIRE_INST(Mnemonic::LHU, Extension::I);
    REQUIRE_GPR16(operands[0], x7, true);
    REQUIRE_GPR32(operands[1], x15, false);
    REQUIRE_IMM(operands[2], -1024);
}

TEST_CASE("Decoder LW", "[rv32i-decoder]") {
    uint32_t value = 0;
    auto as = MakeAssembler32(value);

    as.LW(x7, -1024, x15);

    DecodedInstruction instruction;
    DecodedOperand operands[4];
    auto de = MakeDecoder32();
    DecoderStatus status = de.Decode(&value, 4, instruction, operands);

    REQUIRE(status == DecoderStatus::Ok);
    REQUIRE_INST(Mnemonic::LW, Extension::I);
    REQUIRE_GPR32(operands[0], x7, true);
    REQUIRE_GPR32(operands[1], x15, false);
    REQUIRE_IMM(operands[2], -1024);
}

TEST_CASE("Decoder SB", "[rv32i-decoder]") {
    uint32_t value = 0;
    auto as = MakeAssembler32(value);

    as.SB(x15, -1024, x7);

    DecodedInstruction instruction;
    DecodedOperand operands[4];
    auto de = MakeDecoder32();
    DecoderStatus status = de.Decode(&value, 4, instruction, operands);

    REQUIRE(status == DecoderStatus::Ok);
    REQUIRE_INST(Mnemonic::SB, Extension::I);
    REQUIRE_GPR32(operands[0], x7, false);
    REQUIRE_GPR8(operands[1], x15, false);
    REQUIRE_IMM(operands[2], -1024);
}

TEST_CASE("Decoder SH", "[rv32i-decoder]") {
    uint32_t value = 0;
    auto as = MakeAssembler32(value);

    as.SH(x15, -1024, x7);

    DecodedInstruction instruction;
    DecodedOperand operands[4];
    auto de = MakeDecoder32();
    DecoderStatus status = de.Decode(&value, 4, instruction, operands);

    REQUIRE(status == DecoderStatus::Ok);
    REQUIRE_INST(Mnemonic::SH, Extension::I);
    REQUIRE_GPR32(operands[0], x7, false);
    REQUIRE_GPR16(operands[1], x15, false);
    REQUIRE_IMM(operands[2], -1024);
}

TEST_CASE("Decoder SW", "[rv32i-decoder]") {
    uint32_t value = 0;
    auto as = MakeAssembler32(value);

    as.SW(x15, -1024, x7);

    DecodedInstruction instruction;
    DecodedOperand operands[4];
    auto de = MakeDecoder32();
    DecoderStatus status = de.Decode(&value, 4, instruction, operands);

    REQUIRE(status == DecoderStatus::Ok);
    REQUIRE_INST(Mnemonic::SW, Extension::I);
    REQUIRE_GPR32(operands[0], x7, false);
    REQUIRE_GPR32(operands[1], x15, false);
    REQUIRE_IMM(operands[2], -1024);
}

TEST_CASE("Decoder ADD", "[rv32i-decoder]") {
    uint32_t value = 0;
    auto as = MakeAssembler32(value);

    as.ADD(x7, x15, x31);

    DecodedInstruction instruction;
    DecodedOperand operands[4];
    auto de = MakeDecoder32();
    DecoderStatus status = de.Decode(&value, 4, instruction, operands);

    REQUIRE(status == DecoderStatus::Ok);
    REQUIRE_INST(Mnemonic::ADD, Extension::I);
    REQUIRE_GPR32(operands[0], x7, true);
    REQUIRE_GPR32(operands[1], x15, false);
    REQUIRE_GPR32(operands[2], x31, false);
}

TEST_CASE("Decoder ADDI", "[rv32i-decoder]") {
    uint32_t value = 0;
    auto as = MakeAssembler32(value);

    as.ADDI(x7, x15, 1024);

    DecodedInstruction instruction;
    DecodedOperand operands[4];
    auto de = MakeDecoder32();
    DecoderStatus status = de.Decode(&value, 4, instruction, operands);

    REQUIRE(status == DecoderStatus::Ok);
    REQUIRE_INST(Mnemonic::ADDI, Extension::I);
    REQUIRE_GPR32(operands[0], x7, true);
    REQUIRE_GPR32(operands[1], x15, false);
    REQUIRE_IMM(operands[2], 1024);
}

TEST_CASE("Decoder SUB", "[rv32i-decoder]") {
    uint32_t value = 0;
    auto as = MakeAssembler32(value);

    as.SUB(x7, x15, x31);

    DecodedInstruction instruction;
    DecodedOperand operands[4];
    auto de = MakeDecoder32();
    DecoderStatus status = de.Decode(&value, 4, instruction, operands);

    REQUIRE(status == DecoderStatus::Ok);
    REQUIRE_INST(Mnemonic::SUB, Extension::I);
    REQUIRE_GPR32(operands[0], x7, true);
    REQUIRE_GPR32(operands[1], x15, false);
    REQUIRE_GPR32(operands[2], x31, false);
}

TEST_CASE("Decoder AND", "[rv32i-decoder]") {
    uint32_t value = 0;
    auto as = MakeAssembler32(value);

    as.AND(x7, x15, x31);

    DecodedInstruction instruction;
    DecodedOperand operands[4];
    auto de = MakeDecoder32();
    DecoderStatus status = de.Decode(&value, 4, instruction, operands);

    REQUIRE(status == DecoderStatus::Ok);
    REQUIRE_INST(Mnemonic::AND, Extension::I);
    REQUIRE_GPR32(operands[0], x7, true);
    REQUIRE_GPR32(operands[1], x15, false);
    REQUIRE_GPR32(operands[2], x31, false);
}

TEST_CASE("Decoder OR", "[rv32i-decoder]") {
    uint32_t value = 0;
    auto as = MakeAssembler32(value);

    as.OR(x7, x15, x31);

    DecodedInstruction instruction;
    DecodedOperand operands[4];
    auto de = MakeDecoder32();
    DecoderStatus status = de.Decode(&value, 4, instruction, operands);

    REQUIRE(status == DecoderStatus::Ok);
    REQUIRE_INST(Mnemonic::OR, Extension::I);
    REQUIRE_GPR32(operands[0], x7, true);
    REQUIRE_GPR32(operands[1], x15, false);
    REQUIRE_GPR32(operands[2], x31, false);
}

TEST_CASE("Decoder ORI", "[rv32i-decoder]") {
    uint32_t value = 0;
    auto as = MakeAssembler32(value);

    as.ORI(x7, x15, 1024);

    DecodedInstruction instruction;
    DecodedOperand operands[4];
    auto de = MakeDecoder32();
    DecoderStatus status = de.Decode(&value, 4, instruction, operands);

    REQUIRE(status == DecoderStatus::Ok);
    REQUIRE_INST(Mnemonic::ORI, Extension::I);
    REQUIRE_GPR32(operands[0], x7, true);
    REQUIRE_GPR32(operands[1], x15, false);
    REQUIRE_IMM(operands[2], 1024);
}

TEST_CASE("Decoder XOR", "[rv32i-decoder]") {
    uint32_t value = 0;
    auto as = MakeAssembler32(value);

    as.XOR(x7, x15, x31);

    DecodedInstruction instruction;
    DecodedOperand operands[4];
    auto de = MakeDecoder32();
    DecoderStatus status = de.Decode(&value, 4, instruction, operands);

    REQUIRE(status == DecoderStatus::Ok);
    REQUIRE_INST(Mnemonic::XOR, Extension::I);
    REQUIRE_GPR32(operands[0], x7, true);
    REQUIRE_GPR32(operands[1], x15, false);
    REQUIRE_GPR32(operands[2], x31, false);
}

TEST_CASE("Decoder XORI", "[rv32i-decoder]") {
    uint32_t value = 0;
    auto as = MakeAssembler32(value);

    as.XORI(x7, x15, 1024);

    DecodedInstruction instruction;
    DecodedOperand operands[4];
    auto de = MakeDecoder32();
    DecoderStatus status = de.Decode(&value, 4, instruction, operands);

    REQUIRE(status == DecoderStatus::Ok);
    REQUIRE_INST(Mnemonic::XORI, Extension::I);
    REQUIRE_GPR32(operands[0], x7, true);
    REQUIRE_GPR32(operands[1], x15, false);
    REQUIRE_IMM(operands[2], 1024);
}

TEST_CASE("Decoder SLL", "[rv32i-decoder]") {
    uint32_t value = 0;
    auto as = MakeAssembler32(value);

    as.SLL(x7, x15, x31);

    DecodedInstruction instruction;
    DecodedOperand operands[4];
    auto de = MakeDecoder32();
    DecoderStatus status = de.Decode(&value, 4, instruction, operands);

    REQUIRE(status == DecoderStatus::Ok);
    REQUIRE_INST(Mnemonic::SLL, Extension::I);
    REQUIRE_GPR32(operands[0], x7, true);
    REQUIRE_GPR32(operands[1], x15, false);
    REQUIRE_GPR32(operands[2], x31, false);
}

TEST_CASE("Decoder SLLI", "[rv32i-decoder]") {
    uint32_t value = 0;
    auto as = MakeAssembler32(value);

    as.SLLI(x7, x15, 30);

    DecodedInstruction instruction;
    DecodedOperand operands[4];
    auto de = MakeDecoder32();
    DecoderStatus status = de.Decode(&value, 4, instruction, operands);

    REQUIRE(status == DecoderStatus::Ok);
    REQUIRE_INST(Mnemonic::SLLI, Extension::I);
    REQUIRE_GPR32(operands[0], x7, true);
    REQUIRE_GPR32(operands[1], x15, false);
    REQUIRE_IMM(operands[2], 30);
}

TEST_CASE("Decoder SRL", "[rv32i-decoder]") {
    uint32_t value = 0;
    auto as = MakeAssembler32(value);

    as.SRL(x7, x15, x31);

    DecodedInstruction instruction;
    DecodedOperand operands[4];
    auto de = MakeDecoder32();
    DecoderStatus status = de.Decode(&value, 4, instruction, operands);

    REQUIRE(status == DecoderStatus::Ok);
    REQUIRE_INST(Mnemonic::SRL, Extension::I);
    REQUIRE_GPR32(operands[0], x7, true);
    REQUIRE_GPR32(operands[1], x15, false);
    REQUIRE_GPR32(operands[2], x31, false);
}

TEST_CASE("Decoder SRLI", "[rv32i-decoder]") {
    uint32_t value = 0;
    auto as = MakeAssembler32(value);

    as.SRLI(x7, x15, 30);

    DecodedInstruction instruction;
    DecodedOperand operands[4];
    auto de = MakeDecoder32();
    DecoderStatus status = de.Decode(&value, 4, instruction, operands);

    REQUIRE(status == DecoderStatus::Ok);
    REQUIRE_INST(Mnemonic::SRLI, Extension::I);
    REQUIRE_GPR32(operands[0], x7, true);
    REQUIRE_GPR32(operands[1], x15, false);
    REQUIRE_IMM(operands[2], 30);
}

TEST_CASE("Decoder SRA", "[rv32i-decoder]") {
    uint32_t value = 0;
    auto as = MakeAssembler32(value);

    as.SRA(x7, x15, x31);

    DecodedInstruction instruction;
    DecodedOperand operands[4];
    auto de = MakeDecoder32();
    DecoderStatus status = de.Decode(&value, 4, instruction, operands);

    REQUIRE(status == DecoderStatus::Ok);
    REQUIRE_INST(Mnemonic::SRA, Extension::I);
    REQUIRE_GPR32(operands[0], x7, true);
    REQUIRE_GPR32(operands[1], x15, false);
    REQUIRE_GPR32(operands[2], x31, false);
}

TEST_CASE("Decoder SRAI", "[rv32i-decoder]") {
    uint32_t value = 0;
    auto as = MakeAssembler32(value);

    as.SRAI(x7, x15, 30);

    DecodedInstruction instruction;
    DecodedOperand operands[4];
    auto de = MakeDecoder32();
    DecoderStatus status = de.Decode(&value, 4, instruction, operands);

    REQUIRE(status == DecoderStatus::Ok);
    REQUIRE_INST(Mnemonic::SRAI, Extension::I);
    REQUIRE_GPR32(operands[0], x7, true);
    REQUIRE_GPR32(operands[1], x15, false);
    REQUIRE_IMM(operands[2], 30);
}

TEST_CASE("Decoder SLT", "[rv32i-decoder]") {
    uint32_t value = 0;
    auto as = MakeAssembler32(value);

    as.SLT(x7, x15, x31);

    DecodedInstruction instruction;
    DecodedOperand operands[4];
    auto de = MakeDecoder32();
    DecoderStatus status = de.Decode(&value, 4, instruction, operands);

    REQUIRE(status == DecoderStatus::Ok);
    REQUIRE_INST(Mnemonic::SLT, Extension::I);
    REQUIRE_GPR32(operands[0], x7, true);
    REQUIRE_GPR32(operands[1], x15, false);
    REQUIRE_GPR32(operands[2], x31, false);
}

TEST_CASE("Decoder SLTI", "[rv32i-decoder]") {
    uint32_t value = 0;
    auto as = MakeAssembler32(value);

    as.SLTI(x7, x15, 1024);

    DecodedInstruction instruction;
    DecodedOperand operands[4];
    auto de = MakeDecoder32();
    DecoderStatus status = de.Decode(&value, 4, instruction, operands);

    REQUIRE(status == DecoderStatus::Ok);
    REQUIRE_INST(Mnemonic::SLTI, Extension::I);
    REQUIRE_GPR32(operands[0], x7, true);
    REQUIRE_GPR32(operands[1], x15, false);
    REQUIRE_IMM(operands[2], 1024);
}

TEST_CASE("Decoder SLTU", "[rv32i-decoder]") {
    uint32_t value = 0;
    auto as = MakeAssembler32(value);

    as.SLTU(x7, x15, x31);

    DecodedInstruction instruction;
    DecodedOperand operands[4];
    auto de = MakeDecoder32();
    DecoderStatus status = de.Decode(&value, 4, instruction, operands);

    REQUIRE(status == DecoderStatus::Ok);
    REQUIRE_INST(Mnemonic::SLTU, Extension::I);
    REQUIRE_GPR32(operands[0], x7, true);
    REQUIRE_GPR32(operands[1], x15, false);
    REQUIRE_GPR32(operands[2], x31, false);
}

TEST_CASE("Decoder SLTIU", "[rv32i-decoder]") {
    uint32_t value = 0;
    auto as = MakeAssembler32(value);

    as.SLTIU(x7, x15, 1024);

    DecodedInstruction instruction;
    DecodedOperand operands[4];
    auto de = MakeDecoder32();
    DecoderStatus status = de.Decode(&value, 4, instruction, operands);

    REQUIRE(status == DecoderStatus::Ok);
    REQUIRE_INST(Mnemonic::SLTIU, Extension::I);
    REQUIRE_GPR32(operands[0], x7, true);
    REQUIRE_GPR32(operands[1], x15, false);
    REQUIRE_IMM(operands[2], 1024);
}

TEST_CASE("Decoder FENCE", "[rv32i-decoder]") {
    uint32_t value = 0;
    auto as = MakeAssembler32(value);

    // TODO: pred/succ
    as.FENCE();

    DecodedInstruction instruction;
    DecodedOperand operands[4];
    auto de = MakeDecoder32();
    DecoderStatus status = de.Decode(&value, 4, instruction, operands);

    REQUIRE(status == DecoderStatus::Ok);
    REQUIRE_INST(Mnemonic::FENCE, Extension::I);
}

TEST_CASE("Decoder FENCETSO", "[rv32i-decoder]") {
    uint32_t value = 0;
    auto as = MakeAssembler32(value);

    as.FENCETSO();

    DecodedInstruction instruction;
    DecodedOperand operands[4];
    auto de = MakeDecoder32();
    DecoderStatus status = de.Decode(&value, 4, instruction, operands);

    REQUIRE(status == DecoderStatus::Ok);
    REQUIRE_INST(Mnemonic::FENCETSO, Extension::I);
}

TEST_CASE("Decoder PAUSE", "[rv32i-decoder]") {
    uint32_t value = 0;
    auto as = MakeAssembler32(value);

    as.PAUSE();

    DecodedInstruction instruction;
    DecodedOperand operands[4];
    auto de = MakeDecoder32();
    DecoderStatus status = de.Decode(&value, 4, instruction, operands);

    REQUIRE(status == DecoderStatus::Ok);
    REQUIRE_INST(Mnemonic::PAUSE, Extension::I);
}

TEST_CASE("Decoder ECALL", "[rv32i-decoder]") {
    uint32_t value = 0;
    auto as = MakeAssembler32(value);

    as.ECALL();

    DecodedInstruction instruction;
    DecodedOperand operands[4];
    auto de = MakeDecoder32();
    DecoderStatus status = de.Decode(&value, 4, instruction, operands);

    REQUIRE(status == DecoderStatus::Ok);
    REQUIRE_INST(Mnemonic::ECALL, Extension::I);
}

TEST_CASE("Decoder EBREAK", "[rv32i-decoder]") {
    uint32_t value = 0;
    auto as = MakeAssembler32(value);

    as.EBREAK();

    DecodedInstruction instruction;
    DecodedOperand operands[4];
    auto de = MakeDecoder32();
    DecoderStatus status = de.Decode(&value, 4, instruction, operands);

    REQUIRE(status == DecoderStatus::Ok);
    REQUIRE_INST(Mnemonic::EBREAK, Extension::I);
}
