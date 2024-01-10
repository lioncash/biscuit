#include <biscuit/assert.hpp>
#include <biscuit/assembler.hpp>

#include "assembler_util.hpp"

// RVC Extension Instructions

namespace biscuit {
namespace {
// Emits a compressed branch instruction. These consist of:
// funct3 | imm[8|4:3] | rs | imm[7:6|2:1|5] | op
void EmitCompressedBranch(CodeBuffer& buffer, uint32_t funct3, int32_t offset, GPR rs, uint32_t op) {
    BISCUIT_ASSERT(IsValidCBTypeImm(offset));
    BISCUIT_ASSERT(IsValid3BitCompressedReg(rs));

    const auto transformed_imm = TransformToCBTypeImm(static_cast<uint32_t>(offset));
    const auto rs_san = CompressedRegTo3BitEncoding(rs);
    buffer.Emit16(((funct3 & 0b111) << 13) | transformed_imm | (rs_san << 7) | (op & 0b11));
}

// Emits a compressed jump instruction. These consist of:
// funct3 | imm | op
void EmitCompressedJump(CodeBuffer& buffer, uint32_t funct3, int32_t offset, uint32_t op) {
    BISCUIT_ASSERT(IsValidCJTypeImm(offset));

    buffer.Emit16(TransformToCJTypeImm(static_cast<uint32_t>(offset)) |
                  ((funct3 & 0b111) << 13) | (op & 0b11));
}

// Emits a compress immediate instruction. These consist of:
// funct3 | imm | rd | imm | op
void EmitCompressedImmediate(CodeBuffer& buffer, uint32_t funct3, uint32_t imm, GPR rd, uint32_t op) {
    BISCUIT_ASSERT(rd != x0);

    const auto new_imm = ((imm & 0b11111) << 2) | ((imm & 0b100000) << 7);
    buffer.Emit16(((funct3 & 0b111) << 13) | new_imm | (rd.Index() << 7) | (op & 0b11));
}

// Emits a compressed load instruction. These consist of:
// funct3 | imm | rs1 | imm | rd | op
void EmitCompressedLoad(CodeBuffer& buffer, uint32_t funct3, uint32_t imm, GPR rs,
                        Register rd, uint32_t op) {
    BISCUIT_ASSERT(IsValid3BitCompressedReg(rs));
    BISCUIT_ASSERT(IsValid3BitCompressedReg(rd));

    imm &= 0xF8;

    const auto imm_enc = ((imm & 0x38) << 7) | ((imm & 0xC0) >> 1);
    const auto rd_san = CompressedRegTo3BitEncoding(rd);
    const auto rs_san = CompressedRegTo3BitEncoding(rs);
    buffer.Emit16(((funct3 & 0b111) << 13) | imm_enc | (rs_san << 7) | (rd_san << 2) | (op & 0b11));
}

// Emits a compressed register arithmetic instruction. These consist of:
// funct6 | rd | funct2 | rs | op
void EmitCompressedRegArith(CodeBuffer& buffer, uint32_t funct6, GPR rd, uint32_t funct2,
                            GPR rs, uint32_t op) {
    BISCUIT_ASSERT(IsValid3BitCompressedReg(rs));
    BISCUIT_ASSERT(IsValid3BitCompressedReg(rd));

    const auto rd_san = CompressedRegTo3BitEncoding(rd);
    const auto rs_san = CompressedRegTo3BitEncoding(rs);
    buffer.Emit16(((funct6 & 0b111111) << 10) | (rd_san << 7) | ((funct2 & 0b11) << 5) |
                  (rs_san << 2) | (op & 0b11));
}

// Emits a compressed store instruction. These consist of:
// funct3 | imm | rs1 | imm | rs2 | op
void EmitCompressedStore(CodeBuffer& buffer, uint32_t funct3, uint32_t imm, GPR rs1,
                         Register rs2, uint32_t op) {
    // This has the same format as a compressed load, with rs2 taking the place of rd.
    // We can reuse the code we've already written to handle this.
    EmitCompressedLoad(buffer, funct3, imm, rs1, rs2, op);
}

// Emits a compressed wide immediate instruction. These consist of:
// funct3 | imm | rd | opcode
void EmitCompressedWideImmediate(CodeBuffer& buffer, uint32_t funct3, uint32_t imm,
                                 GPR rd, uint32_t op) {
    BISCUIT_ASSERT(IsValid3BitCompressedReg(rd));

    const auto rd_sanitized = CompressedRegTo3BitEncoding(rd);
    buffer.Emit16(((funct3 & 0b111) << 13) | ((imm & 0xFF) << 5) |
                  (rd_sanitized << 2) | (op & 0b11));
}

void EmitCLBType(CodeBuffer& buffer, uint32_t funct6, GPR rs, uint32_t uimm, GPR rd,
                 uint32_t op, uint32_t b6) {
    BISCUIT_ASSERT(IsValid3BitCompressedReg(rs));
    BISCUIT_ASSERT(IsValid3BitCompressedReg(rd));
    BISCUIT_ASSERT(uimm <= 3);

    const auto rd_san = CompressedRegTo3BitEncoding(rd);
    const auto rs_san = CompressedRegTo3BitEncoding(rs);

    buffer.Emit16((funct6 << 10) |  (rs_san << 7) | (b6 << 6) | (uimm << 5) | (rd_san << 2) | op);
}

void EmitCLHType(CodeBuffer& buffer, uint32_t funct6, GPR rs, uint32_t uimm, GPR rd,
                 uint32_t op, uint32_t b6) {
    BISCUIT_ASSERT((uimm % 2) == 0);
    BISCUIT_ASSERT(uimm <= 2);

    // Only have 1 bit of encoding space for the immediate.
    const uint32_t uimm_fixed = uimm >> 1;
    EmitCLBType(buffer, funct6, rs, uimm_fixed, rd, op, b6);
}

// These have the same layout as the equivalent loads, we just essentially alias
// the name of those to provide better intent at the call site.
void EmitCSBType(CodeBuffer& buffer, uint32_t funct6, GPR rs, uint32_t uimm, GPR rd, uint32_t op) {
    EmitCLBType(buffer, funct6, rs, uimm, rd, op, 0);
}
void EmitCSHType(CodeBuffer& buffer, uint32_t funct6, GPR rs, uint32_t uimm, GPR rd, uint32_t op) {
    EmitCLHType(buffer, funct6, rs, uimm, rd, op, 0);
}
} // Anonymous namespace

void Assembler::C_ADD(GPR rd, GPR rs) noexcept {
    BISCUIT_ASSERT(rs != x0);
    m_buffer.Emit16(0x9002 | (rd.Index() << 7) | (rs.Index() << 2));
}

void Assembler::C_ADDI(GPR rd, int32_t imm) noexcept {
    BISCUIT_ASSERT(imm != 0);
    BISCUIT_ASSERT(IsValidSigned6BitImm(imm));
    EmitCompressedImmediate(m_buffer, 0b000, static_cast<uint32_t>(imm), rd, 0b01);
}

void Assembler::C_ADDIW(GPR rd, int32_t imm) noexcept {
    BISCUIT_ASSERT(IsValidSigned6BitImm(imm));
    EmitCompressedImmediate(m_buffer, 0b001, static_cast<uint32_t>(imm), rd, 0b01);
}

void Assembler::C_ADDI4SPN(GPR rd, uint32_t imm) noexcept {
    BISCUIT_ASSERT(imm != 0);
    BISCUIT_ASSERT(imm <= 1020);
    BISCUIT_ASSERT(imm % 4 == 0);

    // clang-format off
    const auto new_imm = ((imm & 0x030) << 2) |
                         ((imm & 0x3C0) >> 4) |
                         ((imm & 0x004) >> 1) |
                         ((imm & 0x008) >> 3);
    // clang-format on

    EmitCompressedWideImmediate(m_buffer, 0b000, new_imm, rd, 0b00);
}

void Assembler::C_ADDW(GPR rd, GPR rs) noexcept {
    EmitCompressedRegArith(m_buffer, 0b100111, rd, 0b01, rs, 0b01);
}

void Assembler::C_ADDI16SP(int32_t imm) noexcept {
    BISCUIT_ASSERT(imm != 0);
    BISCUIT_ASSERT(imm >= -512 && imm <= 496);
    BISCUIT_ASSERT(imm % 16 == 0);

    // clang-format off
    const auto uimm = static_cast<uint32_t>(imm);
    const auto new_imm = ((uimm & 0x020) >> 3) |
                         ((uimm & 0x180) >> 4) |
                         ((uimm & 0x040) >> 1) |
                         ((uimm & 0x010) << 2) |
                         ((uimm & 0x200) << 3);
    // clang-format on

    m_buffer.Emit16(0x6000U | new_imm | (x2.Index() << 7) | 0b01U);
}

void Assembler::C_AND(GPR rd, GPR rs) noexcept {
    EmitCompressedRegArith(m_buffer, 0b100011, rd, 0b11, rs, 0b01);
}

void Assembler::C_ANDI(GPR rd, uint32_t imm) noexcept {
    BISCUIT_ASSERT(IsValid3BitCompressedReg(rd));

    constexpr auto base = 0x8801U;
    const auto shift_enc = ((imm & 0b11111) << 2) | ((imm & 0b100000) << 7);
    const auto reg = CompressedRegTo3BitEncoding(rd);

    m_buffer.Emit16(base | shift_enc | (reg << 7));
}

void Assembler::C_BEQZ(GPR rs, int32_t offset) noexcept {
    EmitCompressedBranch(m_buffer, 0b110, offset, rs, 0b01);
}

void Assembler::C_BEQZ(GPR rs, Label* label) noexcept {
    const auto address = LinkAndGetOffset(label);
    C_BEQZ(rs, static_cast<int32_t>(address));
}

void Assembler::C_BNEZ(GPR rs, int32_t offset) noexcept {
    EmitCompressedBranch(m_buffer, 0b111, offset, rs, 0b01);
}

void Assembler::C_BNEZ(GPR rs, Label* label) noexcept {
    const auto address = LinkAndGetOffset(label);
    C_BNEZ(rs, static_cast<int32_t>(address));
}

void Assembler::C_EBREAK() noexcept {
    m_buffer.Emit16(0x9002);
}

void Assembler::C_FLD(FPR rd, uint32_t imm, GPR rs) noexcept {
    BISCUIT_ASSERT(imm <= 248);
    BISCUIT_ASSERT(imm % 8 == 0);

    EmitCompressedLoad(m_buffer, 0b001, imm, rs, rd, 0b00);
}

void Assembler::C_FLDSP(FPR rd, uint32_t imm) noexcept {
    BISCUIT_ASSERT(imm <= 504);
    BISCUIT_ASSERT(imm % 8 == 0);

    // clang-format off
    const auto new_imm = ((imm & 0x018) << 2) |
                         ((imm & 0x1C0) >> 4) |
                         ((imm & 0x020) << 7);
    // clang-format on

    m_buffer.Emit16(0x2002U | (rd.Index() << 7) | new_imm);
}

void Assembler::C_FLW(FPR rd, uint32_t imm, GPR rs) noexcept {
    BISCUIT_ASSERT(imm <= 124);
    BISCUIT_ASSERT(imm % 4 == 0);

    imm &= 0x7C;
    const auto new_imm = ((imm & 0b0100) << 5) | (imm & 0x78);
    EmitCompressedLoad(m_buffer, 0b011, new_imm, rs, rd, 0b00);
}

void Assembler::C_FLWSP(FPR rd, uint32_t imm) noexcept {
    BISCUIT_ASSERT(imm <= 252);
    BISCUIT_ASSERT(imm % 4 == 0);

    // clang-format off
    const auto new_imm = ((imm & 0x020) << 7) |
                         ((imm & 0x0C0) >> 4) |
                         ((imm & 0x01C) << 2);
    // clang-format on

    m_buffer.Emit16(0x6002U | (rd.Index() << 7) | new_imm);
}

void Assembler::C_FSD(FPR rs2, uint32_t imm, GPR rs1) noexcept {
    BISCUIT_ASSERT(imm <= 248);
    BISCUIT_ASSERT(imm % 8 == 0);

    EmitCompressedStore(m_buffer, 0b101, imm, rs1, rs2, 0b00);
}

void Assembler::C_FSDSP(FPR rs, uint32_t imm) noexcept {
    BISCUIT_ASSERT(imm <= 504);
    BISCUIT_ASSERT(imm % 8 == 0);

    // clang-format off
    const auto new_imm = ((imm & 0x038) << 7) |
                         ((imm & 0x1C0) << 1);
    // clang-format on

    m_buffer.Emit16(0xA002U | (rs.Index() << 2) | new_imm);
}

void Assembler::C_J(Label* label) noexcept {
    const auto address = LinkAndGetOffset(label);
    C_J(static_cast<int32_t>(address));
}

void Assembler::C_J(int32_t offset) noexcept {
    EmitCompressedJump(m_buffer, 0b101, offset, 0b01);
}

void Assembler::C_JAL(Label* label) noexcept {
    const auto address = LinkAndGetOffset(label);
    C_JAL(static_cast<int32_t>(address));
}

void Assembler::C_JAL(int32_t offset) noexcept {
    EmitCompressedJump(m_buffer, 0b001, offset, 0b01);
}

void Assembler::C_FSW(FPR rs2, uint32_t imm, GPR rs1) noexcept {
    imm &= 0x7C;
    const auto new_imm = ((imm & 0b0100) << 5) | (imm & 0x78);
    EmitCompressedStore(m_buffer, 0b111, new_imm, rs1, rs2, 0b00);
}

void Assembler::C_FSWSP(FPR rs, uint32_t imm) noexcept {
    BISCUIT_ASSERT(imm <= 252);
    BISCUIT_ASSERT(imm % 4 == 0);

    // clang-format off
    const auto new_imm = ((imm & 0x0C0) << 1) |
                         ((imm & 0x03C) << 7);
    // clang-format on

    m_buffer.Emit16(0xE002U | (rs.Index() << 2) | new_imm);
}

void Assembler::C_JALR(GPR rs) noexcept {
    BISCUIT_ASSERT(rs != x0);
    m_buffer.Emit16(0x9002 | (rs.Index() << 7));
}

void Assembler::C_JR(GPR rs) noexcept {
    BISCUIT_ASSERT(rs != x0);
    m_buffer.Emit16(0x8002 | (rs.Index() << 7));
}

void Assembler::C_LD(GPR rd, uint32_t imm, GPR rs) noexcept {
    BISCUIT_ASSERT(imm <= 248);
    BISCUIT_ASSERT(imm % 8 == 0);

    EmitCompressedLoad(m_buffer, 0b011, imm, rs, rd, 0b00);
}

void Assembler::C_LDSP(GPR rd, uint32_t imm) noexcept {
    BISCUIT_ASSERT(rd != x0);
    BISCUIT_ASSERT(imm <= 504);
    BISCUIT_ASSERT(imm % 8 == 0);

    // clang-format off
    const auto new_imm = ((imm & 0x018) << 2) |
                         ((imm & 0x1C0) >> 4) |
                         ((imm & 0x020) << 7);
    // clang-format on

    m_buffer.Emit16(0x6002U | (rd.Index() << 7) | new_imm);
}

void Assembler::C_LI(GPR rd, int32_t imm) noexcept {
    BISCUIT_ASSERT(IsValidSigned6BitImm(imm));
    EmitCompressedImmediate(m_buffer, 0b010, static_cast<uint32_t>(imm), rd, 0b01);
}

void Assembler::C_LQ(GPR rd, uint32_t imm, GPR rs) noexcept {
    BISCUIT_ASSERT(imm <= 496);
    BISCUIT_ASSERT(imm % 16 == 0);

    imm &= 0x1F0;
    const auto new_imm = ((imm & 0x100) >> 5) | (imm & 0xF0);
    EmitCompressedLoad(m_buffer, 0b001, new_imm, rs, rd, 0b00);
}

void Assembler::C_LQSP(GPR rd, uint32_t imm) noexcept {
    BISCUIT_ASSERT(rd != x0);
    BISCUIT_ASSERT(imm <= 1008);
    BISCUIT_ASSERT(imm % 16 == 0);

    // clang-format off
    const auto new_imm = ((imm & 0x020) << 7) |
                         ((imm & 0x010) << 2) |
                         ((imm & 0x3C0) >> 4);
    // clang-format on

    m_buffer.Emit16(0x2002U | (rd.Index() << 7) | new_imm);
}

void Assembler::C_LUI(GPR rd, uint32_t imm) noexcept {
    BISCUIT_ASSERT(imm != 0);
    BISCUIT_ASSERT(rd != x0 && rd != x2);

    const auto new_imm = (imm & 0x3F000) >> 12;
    EmitCompressedImmediate(m_buffer, 0b011, new_imm, rd, 0b01);
}

void Assembler::C_LW(GPR rd, uint32_t imm, GPR rs) noexcept {
    BISCUIT_ASSERT(imm <= 124);
    BISCUIT_ASSERT(imm % 4 == 0);

    imm &= 0x7C;
    const auto new_imm = ((imm & 0b0100) << 5) | (imm & 0x78);
    EmitCompressedLoad(m_buffer, 0b010, new_imm, rs, rd, 0b00);
}

void Assembler::C_LWSP(GPR rd, uint32_t imm) noexcept {
    BISCUIT_ASSERT(rd != x0);
    BISCUIT_ASSERT(imm <= 252);
    BISCUIT_ASSERT(imm % 4 == 0);

    // clang-format off
    const auto new_imm = ((imm & 0x020) << 7) |
                         ((imm & 0x0C0) >> 4) |
                         ((imm & 0x01C) << 2);
    // clang-format on

    m_buffer.Emit16(0x4002U | (rd.Index() << 7) | new_imm);
}

void Assembler::C_MV(GPR rd, GPR rs) noexcept {
    BISCUIT_ASSERT(rd != x0);
    BISCUIT_ASSERT(rs != x0);
    m_buffer.Emit16(0x8002 | (rd.Index() << 7) | (rs.Index() << 2));
}

void Assembler::C_NOP() noexcept {
    m_buffer.Emit16(1);
}

void Assembler::C_OR(GPR rd, GPR rs) noexcept {
    EmitCompressedRegArith(m_buffer, 0b100011, rd, 0b10, rs, 0b01);
}

void Assembler::C_SD(GPR rs2, uint32_t imm, GPR rs1) noexcept {
    BISCUIT_ASSERT(imm <= 248);
    BISCUIT_ASSERT(imm % 8 == 0);

    EmitCompressedLoad(m_buffer, 0b111, imm, rs1, rs2, 0b00);
}

void Assembler::C_SDSP(GPR rs, uint32_t imm) noexcept {
    BISCUIT_ASSERT(imm <= 504);
    BISCUIT_ASSERT(imm % 8 == 0);

    // clang-format off
    const auto new_imm = ((imm & 0x038) << 7) |
                         ((imm & 0x1C0) << 1);
    // clang-format on

    m_buffer.Emit16(0xE002U | (rs.Index() << 2) | new_imm);
}

void Assembler::C_SLLI(GPR rd, uint32_t shift) noexcept {
    BISCUIT_ASSERT(rd != x0);
    BISCUIT_ASSERT(IsValidCompressedShiftAmount(shift));

    // RV128C encodes a 64-bit shift with an encoding of 0.
    if (shift == 64) {
        shift = 0;
    }

    const auto shift_enc = ((shift & 0b11111) << 2) | ((shift & 0b100000) << 7);
    m_buffer.Emit16(0x0002U | shift_enc | (rd.Index() << 7));
}

void Assembler::C_SQ(GPR rs2, uint32_t imm, GPR rs1) noexcept {
    BISCUIT_ASSERT(imm <= 496);
    BISCUIT_ASSERT(imm % 16 == 0);

    imm &= 0x1F0;
    const auto new_imm = ((imm & 0x100) >> 5) | (imm & 0xF0);
    EmitCompressedStore(m_buffer, 0b101, new_imm, rs1, rs2, 0b00);
}

void Assembler::C_SQSP(GPR rs, uint32_t imm) noexcept {
    BISCUIT_ASSERT(imm <= 1008);
    BISCUIT_ASSERT(imm % 16 == 0);

    // clang-format off
    const auto new_imm = ((imm & 0x3C0) << 1) |
                         ((imm & 0x030) << 7);
    // clang-format on

    m_buffer.Emit16(0xA002U | (rs.Index() << 2) | new_imm);
}

void Assembler::C_SRAI(GPR rd, uint32_t shift) noexcept {
    BISCUIT_ASSERT(IsValid3BitCompressedReg(rd));
    BISCUIT_ASSERT(IsValidCompressedShiftAmount(shift));

    // RV128C encodes a 64-bit shift with an encoding of 0.
    if (shift == 64) {
        shift = 0;
    }

    constexpr auto base = 0x8401U;
    const auto shift_enc = ((shift & 0b11111) << 2) | ((shift & 0b100000) << 7);
    const auto reg = CompressedRegTo3BitEncoding(rd);

    m_buffer.Emit16(base | shift_enc | (reg << 7));
}

void Assembler::C_SRLI(GPR rd, uint32_t shift) noexcept {
    BISCUIT_ASSERT(IsValid3BitCompressedReg(rd));
    BISCUIT_ASSERT(IsValidCompressedShiftAmount(shift));

    // RV128C encodes a 64-bit shift with an encoding of 0.
    if (shift == 64) {
        shift = 0;
    }

    constexpr auto base = 0x8001U;
    const auto shift_enc = ((shift & 0b11111) << 2) | ((shift & 0b100000) << 7);
    const auto reg = CompressedRegTo3BitEncoding(rd);

    m_buffer.Emit16(base | shift_enc | (reg << 7));
}

void Assembler::C_SUB(GPR rd, GPR rs) noexcept {
    EmitCompressedRegArith(m_buffer, 0b100011, rd, 0b00, rs, 0b01);
}

void Assembler::C_SUBW(GPR rd, GPR rs) noexcept {
    EmitCompressedRegArith(m_buffer, 0b100111, rd, 0b00, rs, 0b01);
}

void Assembler::C_SW(GPR rs2, uint32_t imm, GPR rs1) noexcept {
    BISCUIT_ASSERT(imm <= 124);
    BISCUIT_ASSERT(imm % 4 == 0);

    imm &= 0x7C;
    const auto new_imm = ((imm & 0b0100) << 5) | (imm & 0x78);
    EmitCompressedStore(m_buffer, 0b110, new_imm, rs1, rs2, 0b00);
}

void Assembler::C_SWSP(GPR rs, uint32_t imm) noexcept {
    BISCUIT_ASSERT(imm <= 252);
    BISCUIT_ASSERT(imm % 4 == 0);

    // clang-format off
    const auto new_imm = ((imm & 0x0C0) << 1) |
                         ((imm & 0x03C) << 7);
    // clang-format on

    m_buffer.Emit16(0xC002U | (rs.Index() << 2) | new_imm);
}

void Assembler::C_UNDEF() noexcept {
    m_buffer.Emit16(0);
}

void Assembler::C_XOR(GPR rd, GPR rs) noexcept {
    EmitCompressedRegArith(m_buffer, 0b100011, rd, 0b01, rs, 0b01);
}

// Zc Extension Instructions

void Assembler::C_LBU(GPR rd, uint32_t uimm, GPR rs) noexcept {
    // C.LBU swaps the ordering of the immediate.
    const auto uimm_fixed = ((uimm & 0b01) << 1) | ((uimm & 0b10) >> 1);

    EmitCLBType(m_buffer, 0b100000, rs, uimm_fixed, rd, 0b00, 0);
}
void Assembler::C_LH(GPR rd, uint32_t uimm, GPR rs) noexcept {
    EmitCLHType(m_buffer, 0b100001, rs, uimm, rd, 0b00, 1);
}
void Assembler::C_LHU(GPR rd, uint32_t uimm, GPR rs) noexcept {
    EmitCLHType(m_buffer, 0b100001, rs, uimm, rd, 0b00, 0);
}
void Assembler::C_SB(GPR rs2, uint32_t uimm, GPR rs1) noexcept {
    // C.SB swaps the ordering of the immediate.
    const auto uimm_fixed = ((uimm & 0b01) << 1) | ((uimm & 0b10) >> 1);

    EmitCSBType(m_buffer, 0b100010, rs1, uimm_fixed, rs2, 0b00);
}
void Assembler::C_SH(GPR rs2, uint32_t uimm, GPR rs1) noexcept {
    EmitCSHType(m_buffer, 0b100011, rs1, uimm, rs2, 0b00);
}

} // namespace biscuit
