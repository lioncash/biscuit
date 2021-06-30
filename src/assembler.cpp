#include <biscuit/assert.hpp>
#include <biscuit/assembler.hpp>

#include <cstring>

namespace biscuit {

// B-type immediates only provide ~4KiB range branches.
[[nodiscard]] static bool IsValidBTypeImm(ptrdiff_t value) noexcept {
    return value >= -2048 && value <= 2047;
}

// J-type immediates only provide ~1MiB range branches.
[[nodiscard]] static bool IsValidJTypeImm(ptrdiff_t value) noexcept {
    return value >= -0x80000 && value <= 0x7FFFF;
}

// CB-type immediates only provide ~256B range branches.
[[nodiscard]] static bool IsValidCBTypeImm(ptrdiff_t value) noexcept {
    return value >= -256 && value <= 255;
}

// CJ-type immediates only provide ~2KiB range branches.
[[nodiscard]] static bool IsValidCJTypeImm(ptrdiff_t value) noexcept {
    return value >= -1024 && value <= 1023;
}

// Determines whether or not the register fits in 3-bit compressed encoding.
[[nodiscard]] static bool IsValid3BitCompressedReg(Register reg) noexcept {
    const auto index = reg.Index();
    return index >= 8 && index <= 15;
}

// Turns a compressed register into its encoding.
[[nodiscard]] static uint32_t CompressedRegTo3BitEncoding(Register reg) noexcept {
    return reg.Index() - 8;
}

// Transforms a regular value into an immediate encoded in a B-type instruction.
[[nodiscard]] static uint32_t TransformToBTypeImm(uint32_t imm) noexcept {
    // clang-format off
    return ((imm & 0x07E0) << 20) |
           ((imm & 0x1000) << 19) |
           ((imm & 0x001E) << 7) |
           ((imm & 0x0800) >> 4);
    // clang-format on
}

// Transforms a regular value into an immediate encoded in a J-type instruction.
[[nodiscard]] static uint32_t TransformToJTypeImm(uint32_t imm) noexcept {
    // clang-format off
    return ((imm & 0x0FF000) >> 0) |
           ((imm & 0x000800) << 9) |
           ((imm & 0x0007FE) << 20) |
           ((imm & 0x100000) << 11);
    // clang-format on
}

// Transforms a regular value into an immediate encoded in a CB-type instruction.
[[nodiscard]] static uint32_t TransformToCBTypeImm(uint32_t imm) noexcept {
    // clang-format off
    return ((imm & 0x0C0) >> 1) |
           ((imm & 0x006) << 2) |
           ((imm & 0x020) >> 3) |
           ((imm & 0x018) << 7) |
           ((imm & 0x100) << 4);
    // clang-format on
}

// Transforms a regular value into an immediate encoded in a CJ-type instruction.
[[nodiscard]] static uint32_t TransformToCJTypeImm(uint32_t imm) noexcept {
    // clang-format off
    return ((imm & 0x800) << 1) |
           ((imm & 0x010) << 7) |
           ((imm & 0x300) << 1) |
           ((imm & 0x400) >> 2) |
           ((imm & 0x040) << 1) |
           ((imm & 0x080) >> 1) |
           ((imm & 0x00E) << 4) |
           ((imm & 0x020) >> 3);
    // clang-format on
}

// Determines if a value lies within the range of a 6-bit immediate.
[[nodiscard]] static bool IsValid6BitSignedImm(int32_t value) noexcept {
    return value >= -32 && value <= 31;
}

void Assembler::Bind(Label* label) {
    BindToOffset(label, m_buffer.GetCursorOffset());
}

void Assembler::ADD(GPR rd, GPR lhs, GPR rhs) noexcept {
    EmitRType(0b0000000, rhs, lhs, 0b000, rd, 0b0110011);
}

void Assembler::ADDI(GPR rd, GPR rs, int32_t imm) noexcept {
    EmitIType(static_cast<uint32_t>(imm), rs, 0b000, rd, 0b0010011);
}

void Assembler::AND(GPR rd, GPR lhs, GPR rhs) noexcept {
    EmitRType(0b0000000, rhs, lhs, 0b111, rd, 0b0110011);
}

void Assembler::ANDI(GPR rd, GPR rs, uint32_t imm) noexcept {
    EmitIType(imm, rs, 0b111, rd, 0b0010011);
}

void Assembler::AUIPC(GPR rd, uint32_t imm) noexcept {
    EmitUType(imm, rd, 0b0010111);
}

void Assembler::BEQ(GPR rs1, GPR rs2, Label* label) noexcept {
    const auto address = LinkAndGetOffset(label);
    BEQ(rs1, rs2, static_cast<int32_t>(address));
}

void Assembler::BEQZ(GPR rs, Label* label) noexcept {
    const auto address = LinkAndGetOffset(label);
    BEQZ(rs, static_cast<int32_t>(address));
}

void Assembler::BGE(GPR rs1, GPR rs2, Label* label) noexcept {
    const auto address = LinkAndGetOffset(label);
    BGE(rs1, rs2, static_cast<int32_t>(address));
}

void Assembler::BGEU(GPR rs1, GPR rs2, Label* label) noexcept {
    const auto address = LinkAndGetOffset(label);
    BGEU(rs1, rs2, static_cast<int32_t>(address));
}

void Assembler::BGEZ(GPR rs, Label* label) noexcept {
    const auto address = LinkAndGetOffset(label);
    BGEZ(rs, static_cast<int32_t>(address));
}

void Assembler::BGT(GPR rs, GPR rt, Label* label) noexcept {
    const auto address = LinkAndGetOffset(label);
    BGT(rs, rt, static_cast<int32_t>(address));
}

void Assembler::BGTU(GPR rs, GPR rt, Label* label) noexcept {
    const auto address = LinkAndGetOffset(label);
    BGTU(rs, rt, static_cast<int32_t>(address));
}

void Assembler::BGTZ(GPR rs, Label* label) noexcept {
    const auto address = LinkAndGetOffset(label);
    BGTZ(rs, static_cast<int32_t>(address));
}

void Assembler::BLE(GPR rs, GPR rt, Label* label) noexcept {
    const auto address = LinkAndGetOffset(label);
    BLE(rs, rt, static_cast<int32_t>(address));
}

void Assembler::BLEU(GPR rs, GPR rt, Label* label) noexcept {
    const auto address = LinkAndGetOffset(label);
    BLEU(rs, rt, static_cast<int32_t>(address));
}

void Assembler::BLEZ(GPR rs, Label* label) noexcept {
    const auto address = LinkAndGetOffset(label);
    BLEZ(rs, static_cast<int32_t>(address));
}

void Assembler::BLT(GPR rs1, GPR rs2, Label* label) noexcept {
    const auto address = LinkAndGetOffset(label);
    BLT(rs1, rs2, static_cast<int32_t>(address));
}

void Assembler::BLTU(GPR rs1, GPR rs2, Label* label) noexcept {
    const auto address = LinkAndGetOffset(label);
    BLTU(rs1, rs2, static_cast<int32_t>(address));
}

void Assembler::BLTZ(GPR rs, Label* label) noexcept {
    const auto address = LinkAndGetOffset(label);
    BLTZ(rs, static_cast<int32_t>(address));
}

void Assembler::BNE(GPR rs1, GPR rs2, Label* label) noexcept {
    const auto address = LinkAndGetOffset(label);
    BNE(rs1, rs2, static_cast<int32_t>(address));
}

void Assembler::BNEZ(GPR rs, Label* label) noexcept {
    const auto address = LinkAndGetOffset(label);
    BNEZ(rs, static_cast<int32_t>(address));
}

void Assembler::BEQ(GPR rs1, GPR rs2, int32_t imm) noexcept {
    BISCUIT_ASSERT(IsValidBTypeImm(imm));
    EmitBType(static_cast<uint32_t>(imm), rs2, rs1, 0b000, 0b1100011);
}

void Assembler::BEQZ(GPR rs, int32_t imm) noexcept {
    BEQ(rs, x0, imm);
}

void Assembler::BGE(GPR rs1, GPR rs2, int32_t imm) noexcept {
    BISCUIT_ASSERT(IsValidBTypeImm(imm));
    EmitBType(static_cast<uint32_t>(imm), rs2, rs1, 0b101, 0b1100011);
}

void Assembler::BGEU(GPR rs1, GPR rs2, int32_t imm) noexcept {
    BISCUIT_ASSERT(IsValidBTypeImm(imm));
    EmitBType(static_cast<uint32_t>(imm), rs2, rs1, 0b111, 0b1100011);
}

void Assembler::BGEZ(GPR rs, int32_t imm) noexcept {
    BGE(rs, x0, imm);
}

void Assembler::BGT(GPR rs, GPR rt, int32_t imm) noexcept {
    BLT(rt, rs, imm);
}

void Assembler::BGTU(GPR rs, GPR rt, int32_t imm) noexcept {
    BLTU(rt, rs, imm);
}

void Assembler::BGTZ(GPR rs, int32_t imm) noexcept {
    BLT(x0, rs, imm);
}

void Assembler::BLE(GPR rs, GPR rt, int32_t imm) noexcept {
    BGE(rt, rs, imm);
}

void Assembler::BLEU(GPR rs, GPR rt, int32_t imm) noexcept {
    BGEU(rt, rs, imm);
}

void Assembler::BLEZ(GPR rs, int32_t imm) noexcept {
    BGE(x0, rs, imm);
}

void Assembler::BLT(GPR rs1, GPR rs2, int32_t imm) noexcept {
    BISCUIT_ASSERT(IsValidBTypeImm(imm));
    EmitBType(static_cast<uint32_t>(imm), rs2, rs1, 0b100, 0b1100011);
}

void Assembler::BLTU(GPR rs1, GPR rs2, int32_t imm) noexcept {
    BISCUIT_ASSERT(IsValidBTypeImm(imm));
    EmitBType(static_cast<uint32_t>(imm), rs2, rs1, 0b110, 0b1100011);
}

void Assembler::BLTZ(GPR rs, int32_t imm) noexcept {
    BLT(rs, x0, imm);
}

void Assembler::BNE(GPR rs1, GPR rs2, int32_t imm) noexcept {
    BISCUIT_ASSERT(IsValidBTypeImm(imm));
    EmitBType(static_cast<uint32_t>(imm), rs2, rs1, 0b001, 0b1100011);
}

void Assembler::BNEZ(GPR rs, int32_t imm) noexcept {
    BNE(x0, rs, imm);
}

void Assembler::EBREAK() noexcept {
    m_buffer.Emit32(0x00100073);
}

void Assembler::ECALL() noexcept {
    m_buffer.Emit32(0x00000073);
}

void Assembler::FENCE() noexcept {
    FENCE(FenceOrder::IORW, FenceOrder::IORW);
}

void Assembler::FENCE(FenceOrder pred, FenceOrder succ) noexcept {
    EmitFENCE(0b0000, pred, succ, x0, 0b000, x0, 0b0001111);
}

void Assembler::FENCEI(GPR rd, GPR rs, uint32_t imm) noexcept {
    m_buffer.Emit32(((imm & 0xFFF) << 20) | (rs.Index() << 15) | 0x1000U | (rd.Index() << 7) | 0b0001111);
}

void Assembler::FENCETSO() noexcept {
    EmitFENCE(0b1000, FenceOrder::RW, FenceOrder::RW, x0, 0b000, x0, 0b0001111);
}

void Assembler::J(Label* label) noexcept {
    const auto address = LinkAndGetOffset(label);
    BISCUIT_ASSERT(IsValidJTypeImm(address));
    J(static_cast<uint32_t>(address));
}

void Assembler::JAL(Label* label) noexcept {
    const auto address = LinkAndGetOffset(label);
    BISCUIT_ASSERT(IsValidJTypeImm(address));
    JAL(static_cast<uint32_t>(address));
}

void Assembler::JAL(GPR rd, Label* label) noexcept {
    const auto address = LinkAndGetOffset(label);
    BISCUIT_ASSERT(IsValidJTypeImm(address));
    JAL(rd, static_cast<uint32_t>(address));
}

void Assembler::J(int32_t imm) noexcept {
    BISCUIT_ASSERT(IsValidJTypeImm(imm));
    JAL(x0, imm);
}

void Assembler::JAL(int32_t imm) noexcept {
    BISCUIT_ASSERT(IsValidJTypeImm(imm));
    EmitJType(imm, x1, 0b1101111);
}

void Assembler::JAL(GPR rd, int32_t imm) noexcept {
    BISCUIT_ASSERT(IsValidJTypeImm(imm));
    EmitJType(static_cast<uint32_t>(imm), rd, 0b1101111);
}

void Assembler::JALR(GPR rs) noexcept {
    JALR(x1, 0, rs);
}

void Assembler::JALR(GPR rd, int32_t imm, GPR rs1) noexcept {
    BISCUIT_ASSERT(IsValidJTypeImm(imm));
    EmitIType(static_cast<uint32_t>(imm), rs1, 0b000, rd, 0b1100111);
}

void Assembler::JR(GPR rs) noexcept {
    JALR(x0, 0, rs);
}

void Assembler::LB(GPR rd, uint32_t imm, GPR rs) noexcept {
    EmitIType(imm, rs, 0b000, rd, 0b0000011);
}

void Assembler::LBU(GPR rd, uint32_t imm, GPR rs) noexcept {
    EmitIType(imm, rs, 0b100, rd, 0b0000011);
}

void Assembler::LH(GPR rd, uint32_t imm, GPR rs) noexcept {
    EmitIType(imm, rs, 0b001, rd, 0b0000011);
}

void Assembler::LHU(GPR rd, uint32_t imm, GPR rs) noexcept {
    EmitIType(imm, rs, 0b101, rd, 0b0000011);
}

void Assembler::LUI(GPR rd, uint32_t imm) noexcept {
    EmitUType(imm, rd, 0b0110111);
}

void Assembler::LW(GPR rd, uint32_t imm, GPR rs) noexcept {
    EmitIType(imm, rs, 0b010, rd, 0b0000011);
}

void Assembler::MV(GPR rd, GPR rs) noexcept {
    ADDI(rd, rs, 0);
}

void Assembler::NEG(GPR rd, GPR rs) noexcept {
    SUB(rd, x0, rs);
}

void Assembler::NOP() noexcept {
    ADDI(x0, x0, 0);
}

void Assembler::NOT(GPR rd, GPR rs) noexcept {
    XORI(rd, rs, UINT32_MAX);
}

void Assembler::OR(GPR rd, GPR lhs, GPR rhs) noexcept {
    EmitRType(0b0000000, rhs, lhs, 0b110, rd, 0b0110011);
}

void Assembler::ORI(GPR rd, GPR rs, uint32_t imm) noexcept {
    EmitIType(imm, rs, 0b110, rd, 0b0010011);
}

void Assembler::PAUSE() noexcept {
    m_buffer.Emit32(0x0100000F);
}

void Assembler::RET() noexcept {
    JALR(x0, 0, x1);
}

void Assembler::SB(GPR rs2, uint32_t imm, GPR rs1) noexcept {
    EmitSType(imm, rs2, rs1, 0b000, 0b0100011);
}

void Assembler::SEQZ(GPR rd, GPR rs) noexcept {
    SLTIU(rd, rs, 1);
}

void Assembler::SGTZ(GPR rd, GPR rs) noexcept {
    SLT(rd, x0, rs);
}

void Assembler::SH(GPR rs2, uint32_t imm, GPR rs1) noexcept {
    EmitSType(imm, rs2, rs1, 0b001, 0b0100011);
}

void Assembler::SLL(GPR rd, GPR lhs, GPR rhs) noexcept {
    EmitRType(0b0000000, rhs, lhs, 0b001, rd, 0b0110011);
}

void Assembler::SLLI(GPR rd, GPR rs, uint32_t shift) noexcept {
    EmitIType(shift & 0x1F, rs, 0b001, rd, 0b0010011);
}

void Assembler::SLT(GPR rd, GPR lhs, GPR rhs) noexcept {
    EmitRType(0b0000000, rhs, lhs, 0b010, rd, 0b0110011);
}

void Assembler::SLTI(GPR rd, GPR rs, uint32_t imm) noexcept {
    EmitIType(imm, rs, 0b010, rd, 0b0010011);
}

void Assembler::SLTIU(GPR rd, GPR rs, uint32_t imm) noexcept {
    EmitIType(imm, rs, 0b011, rd, 0b0010011);
}

void Assembler::SLTU(GPR rd, GPR lhs, GPR rhs) noexcept {
    EmitRType(0b0000000, rhs, lhs, 0b011, rd, 0b0110011);
}

void Assembler::SLTZ(GPR rd, GPR rs) noexcept {
    SLT(rd, rs, x0);
}

void Assembler::SNEZ(GPR rd, GPR rs) noexcept {
    SLTU(rd, x0, rs);
}

void Assembler::SRA(GPR rd, GPR lhs, GPR rhs) noexcept {
    EmitRType(0b0100000, rhs, lhs, 0b101, rd, 0b0110011);
}

void Assembler::SRAI(GPR rd, GPR rs, uint32_t shift) noexcept {
    EmitIType((0b0100000 << 5) | (shift & 0x1F), rs, 0b101, rd, 0b0010011);
}

void Assembler::SRL(GPR rd, GPR lhs, GPR rhs) noexcept {
    EmitRType(0b0000000, rhs, lhs, 0b101, rd, 0b0110011);
}

void Assembler::SRLI(GPR rd, GPR rs, uint32_t shift) noexcept {
    EmitIType(shift & 0x1F, rs, 0b101, rd, 0b0010011);
}

void Assembler::SUB(GPR rd, GPR lhs, GPR rhs) noexcept {
    EmitRType(0b0100000, rhs, lhs, 0b000, rd, 0b0110011);
}

void Assembler::SW(GPR rs2, uint32_t imm, GPR rs1) noexcept {
    EmitSType(imm, rs2, rs1, 0b010, 0b0100011);
}

void Assembler::XOR(GPR rd, GPR lhs, GPR rhs) noexcept {
    EmitRType(0b0000000, rhs, lhs, 0b100, rd, 0b0110011);
}

void Assembler::XORI(GPR rd, GPR rs, uint32_t imm) noexcept {
    EmitIType(imm, rs, 0b100, rd, 0b0010011);
}

// RV64I Instructions

void Assembler::ADDIW(GPR rd, GPR rs, int32_t imm) noexcept {
    EmitIType(static_cast<uint32_t>(imm), rs, 0b000, rd, 0b0011011);
}

void Assembler::ADDW(GPR rd, GPR lhs, GPR rhs) noexcept {
    EmitRType(0b0000000, rhs, lhs, 0b000, rd, 0b0111011);
}

void Assembler::LD(GPR rd, uint32_t imm, GPR rs) noexcept {
    EmitIType(imm, rs, 0b011, rd, 0b0000011);
}

void Assembler::LWU(GPR rd, GPR rs, uint32_t imm) noexcept {
    EmitIType(imm, rs, 0b110, rd, 0b0000011);
}

void Assembler::SD(GPR rs2, uint32_t imm, GPR rs1) noexcept {
    EmitSType(imm, rs2, rs1, 0b011, 0b0100011);
}

void Assembler::SRAI64(GPR rd, GPR rs, uint32_t shift) noexcept {
    EmitIType((0b0100000 << 5) | (shift & 0x3F), rs, 0b101, rd, 0b0010011);
}
void Assembler::SLLI64(GPR rd, GPR rs, uint32_t shift) noexcept {
    EmitIType(shift & 0x3F, rs, 0b001, rd, 0b0010011);
}
void Assembler::SRLI64(GPR rd, GPR rs, uint32_t shift) noexcept {
    EmitIType(shift & 0x3F, rs, 0b101, rd, 0b0010011);
}

void Assembler::SLLIW(GPR rd, GPR rs, uint32_t shift) noexcept {
    EmitIType(shift & 0x1F, rs, 0b001, rd, 0b0011011);
}
void Assembler::SRAIW(GPR rd, GPR rs, uint32_t shift) noexcept {
    EmitIType((0b0100000 << 5) | (shift & 0x1F), rs, 0b101, rd, 0b0011011);
}
void Assembler::SRLIW(GPR rd, GPR rs, uint32_t shift) noexcept {
    EmitIType(shift & 0x1F, rs, 0b101, rd, 0b0011011);
}

void Assembler::SLLW(GPR rd, GPR lhs, GPR rhs) noexcept {
    EmitRType(0b0000000, rhs, lhs, 0b001, rd, 0b0111011);
}
void Assembler::SRAW(GPR rd, GPR lhs, GPR rhs) noexcept {
    EmitRType(0b0100000, rhs, lhs, 0b101, rd, 0b0111011);
}
void Assembler::SRLW(GPR rd, GPR lhs, GPR rhs) noexcept {
    EmitRType(0b0000000, rhs, lhs, 0b101, rd, 0b0111011);
}

void Assembler::SUBW(GPR rd, GPR lhs, GPR rhs) noexcept {
    EmitRType(0b0100000, rhs, lhs, 0b000, rd, 0b0111011);
}

// Zicsr Extension Instructions

void Assembler::CSRRC(GPR rd, CSR csr, GPR rs) noexcept {
    EmitIType(static_cast<uint32_t>(csr), rs, 0b011, rd, 0b1110011);
}
void Assembler::CSRRCI(GPR rd, CSR csr, uint32_t imm) noexcept {
    EmitIType(static_cast<uint32_t>(csr), GPR{imm & 0x1F}, 0b111, rd, 0b1110011);
}
void Assembler::CSRRS(GPR rd, CSR csr, GPR rs) noexcept {
    EmitIType(static_cast<uint32_t>(csr), rs, 0b010, rd, 0b1110011);
}
void Assembler::CSRRSI(GPR rd, CSR csr, uint32_t imm) noexcept {
    EmitIType(static_cast<uint32_t>(csr), GPR{imm & 0x1F}, 0b110, rd, 0b1110011);
}
void Assembler::CSRRW(GPR rd, CSR csr, GPR rs) noexcept {
    EmitIType(static_cast<uint32_t>(csr), rs, 0b001, rd, 0b1110011);
}
void Assembler::CSRRWI(GPR rd, CSR csr, uint32_t imm) noexcept {
    EmitIType(static_cast<uint32_t>(csr), GPR{imm & 0x1F}, 0b101, rd, 0b1110011);
}

void Assembler::CSRR(GPR rd, CSR csr) noexcept {
    CSRRS(rd, csr, x0);
}
void Assembler::CSWR(CSR csr, GPR rs) noexcept {
    CSRRW(x0, csr, rs);
}

void Assembler::CSRS(CSR csr, GPR rs) noexcept {
    CSRRS(x0, csr, rs);
}
void Assembler::CSRC(CSR csr, GPR rs) noexcept {
    CSRRC(x0, csr, rs);
}

void Assembler::CSRCI(CSR csr, uint32_t imm) noexcept {
    CSRRCI(x0, csr, imm);
}
void Assembler::CSRSI(CSR csr, uint32_t imm) noexcept {
    CSRRSI(x0, csr, imm);
}
void Assembler::CSRWI(CSR csr, uint32_t imm) noexcept {
    CSRRWI(x0, csr, imm);
}

void Assembler::FRCSR(GPR rd) noexcept {
    CSRRS(rd, CSR::FCSR, x0);
}
void Assembler::FSCSR(GPR rd, GPR rs) noexcept {
    CSRRW(rd, CSR::FCSR, rs);
}
void Assembler::FSCSR(GPR rs) noexcept {
    CSRRW(x0, CSR::FCSR, rs);
}

void Assembler::FRRM(GPR rd) noexcept {
    CSRRS(rd, CSR::FRM, x0);
}
void Assembler::FSRM(GPR rd, GPR rs) noexcept {
    CSRRW(rd, CSR::FRM, rs);
}
void Assembler::FSRM(GPR rs) noexcept {
    CSRRW(x0, CSR::FRM, rs);
}

void Assembler::FSRMI(GPR rd, uint32_t imm) noexcept {
    CSRRWI(rd, CSR::FRM, imm);
}
void Assembler::FSRMI(uint32_t imm) noexcept {
    CSRRWI(x0, CSR::FRM, imm);
}

void Assembler::FRFLAGS(GPR rd) noexcept {
    CSRRS(rd, CSR::FFlags, x0);
}
void Assembler::FSFLAGS(GPR rd, GPR rs) noexcept {
    CSRRW(rd, CSR::FFlags, rs);
}
void Assembler::FSFLAGS(GPR rs) noexcept {
    CSRRW(x0, CSR::FFlags, rs);
}

void Assembler::FSFLAGSI(GPR rd, uint32_t imm) noexcept {
    CSRRWI(rd, CSR::FFlags, imm);
}
void Assembler::FSFLAGSI(uint32_t imm) noexcept {
    CSRRWI(x0, CSR::FFlags, imm);
}

void Assembler::RDCYCLE(GPR rd) noexcept {
    CSRRS(rd, CSR::Cycle, x0);
}
void Assembler::RDCYCLEH(GPR rd) noexcept {
    CSRRS(rd, CSR::CycleH, x0);
}

void Assembler::RDINSTRET(GPR rd) noexcept {
    CSRRS(rd, CSR::InstRet, x0);
}
void Assembler::RDINSTRETH(GPR rd) noexcept {
    CSRRS(rd, CSR::InstRetH, x0);
}

void Assembler::RDTIME(GPR rd) noexcept {
    CSRRS(rd, CSR::Time, x0);
}
void Assembler::RDTIMEH(GPR rd) noexcept {
    CSRRS(rd, CSR::TimeH, x0);
}

// RV32M Extension Instructions

void Assembler::DIV(GPR rd, GPR rs1, GPR rs2) noexcept {
    EmitRType(0b0000001, rs2, rs1, 0b100, rd, 0b0110011);
}
void Assembler::DIVU(GPR rd, GPR rs1, GPR rs2) noexcept {
    EmitRType(0b0000001, rs2, rs1, 0b101, rd, 0b0110011);
}
void Assembler::MUL(GPR rd, GPR rs1, GPR rs2) noexcept {
    EmitRType(0b0000001, rs2, rs1, 0b000, rd, 0b0110011);
}
void Assembler::MULH(GPR rd, GPR rs1, GPR rs2) noexcept {
    EmitRType(0b0000001, rs2, rs1, 0b001, rd, 0b0110011);
}
void Assembler::MULHSU(GPR rd, GPR rs1, GPR rs2) noexcept {
    EmitRType(0b0000001, rs2, rs1, 0b010, rd, 0b0110011);
}
void Assembler::MULHU(GPR rd, GPR rs1, GPR rs2) noexcept {
    EmitRType(0b0000001, rs2, rs1, 0b011, rd, 0b0110011);
}
void Assembler::REM(GPR rd, GPR rs1, GPR rs2) noexcept {
    EmitRType(0b0000001, rs2, rs1, 0b110, rd, 0b0110011);
}
void Assembler::REMU(GPR rd, GPR rs1, GPR rs2) noexcept {
    EmitRType(0b0000001, rs2, rs1, 0b111, rd, 0b0110011);
}

// RV64M Extension Instructions

void Assembler::DIVW(GPR rd, GPR rs1, GPR rs2) noexcept {
    EmitRType(0b0000001, rs2, rs1, 0b100, rd, 0b0111011);
}
void Assembler::DIVUW(GPR rd, GPR rs1, GPR rs2) noexcept {
    EmitRType(0b0000001, rs2, rs1, 0b101, rd, 0b0111011);
}
void Assembler::MULW(GPR rd, GPR rs1, GPR rs2) noexcept {
    EmitRType(0b0000001, rs2, rs1, 0b000, rd, 0b0111011);
}
void Assembler::REMW(GPR rd, GPR rs1, GPR rs2) noexcept {
    EmitRType(0b0000001, rs2, rs1, 0b110, rd, 0b0111011);
}
void Assembler::REMUW(GPR rd, GPR rs1, GPR rs2) noexcept {
    EmitRType(0b0000001, rs2, rs1, 0b111, rd, 0b0111011);
}

// RV32A Extension Instructions

void Assembler::AMOADD_W(Ordering ordering, GPR rd, GPR rs2, GPR rs1) noexcept {
    EmitAtomic(0b00000, ordering, rs2, rs1, 0b010, rd, 0b0101111);
}
void Assembler::AMOAND_W(Ordering ordering, GPR rd, GPR rs2, GPR rs1) noexcept {
    EmitAtomic(0b01100, ordering, rs2, rs1, 0b010, rd, 0b0101111);
}
void Assembler::AMOMAX_W(Ordering ordering, GPR rd, GPR rs2, GPR rs1) noexcept {
    EmitAtomic(0b10100, ordering, rs2, rs1, 0b010, rd, 0b0101111);
}
void Assembler::AMOMAXU_W(Ordering ordering, GPR rd, GPR rs2, GPR rs1) noexcept {
    EmitAtomic(0b11100, ordering, rs2, rs1, 0b010, rd, 0b0101111);
}
void Assembler::AMOMIN_W(Ordering ordering, GPR rd, GPR rs2, GPR rs1) noexcept {
    EmitAtomic(0b10000, ordering, rs2, rs1, 0b010, rd, 0b0101111);
}
void Assembler::AMOMINU_W(Ordering ordering, GPR rd, GPR rs2, GPR rs1) noexcept {
    EmitAtomic(0b11000, ordering, rs2, rs1, 0b010, rd, 0b0101111);
}
void Assembler::AMOOR_W(Ordering ordering, GPR rd, GPR rs2, GPR rs1) noexcept {
    EmitAtomic(0b01000, ordering, rs2, rs1, 0b010, rd, 0b0101111);
}
void Assembler::AMOSWAP_W(Ordering ordering, GPR rd, GPR rs2, GPR rs1) noexcept {
    EmitAtomic(0b00001, ordering, rs2, rs1, 0b010, rd, 0b0101111);
}
void Assembler::AMOXOR_W(Ordering ordering, GPR rd, GPR rs2, GPR rs1) noexcept {
    EmitAtomic(0b00100, ordering, rs2, rs1, 0b010, rd, 0b0101111);
}
void Assembler::LR_W(Ordering ordering, GPR rd, GPR rs) noexcept {
    EmitAtomic(0b00010, ordering, x0, rs, 0b010, rd, 0b0101111);
}
void Assembler::SC_W(Ordering ordering, GPR rd, GPR rs1, GPR rs2) noexcept {
    EmitAtomic(0b00011, ordering, rs2, rs1, 0b010, rd, 0b0101111);
}

// RV64A Extension Instructions

void Assembler::AMOADD_D(Ordering ordering, GPR rd, GPR rs2, GPR rs1) noexcept {
    EmitAtomic(0b00000, ordering, rs2, rs1, 0b011, rd, 0b0101111);
}
void Assembler::AMOAND_D(Ordering ordering, GPR rd, GPR rs2, GPR rs1) noexcept {
    EmitAtomic(0b01100, ordering, rs2, rs1, 0b011, rd, 0b0101111);
}
void Assembler::AMOMAX_D(Ordering ordering, GPR rd, GPR rs2, GPR rs1) noexcept {
    EmitAtomic(0b10100, ordering, rs2, rs1, 0b011, rd, 0b0101111);
}
void Assembler::AMOMAXU_D(Ordering ordering, GPR rd, GPR rs2, GPR rs1) noexcept {
    EmitAtomic(0b11100, ordering, rs2, rs1, 0b011, rd, 0b0101111);
}
void Assembler::AMOMIN_D(Ordering ordering, GPR rd, GPR rs2, GPR rs1) noexcept {
    EmitAtomic(0b10000, ordering, rs2, rs1, 0b011, rd, 0b0101111);
}
void Assembler::AMOMINU_D(Ordering ordering, GPR rd, GPR rs2, GPR rs1) noexcept {
    EmitAtomic(0b11000, ordering, rs2, rs1, 0b011, rd, 0b0101111);
}
void Assembler::AMOOR_D(Ordering ordering, GPR rd, GPR rs2, GPR rs1) noexcept {
    EmitAtomic(0b01000, ordering, rs2, rs1, 0b011, rd, 0b0101111);
}
void Assembler::AMOSWAP_D(Ordering ordering, GPR rd, GPR rs2, GPR rs1) noexcept {
    EmitAtomic(0b00001, ordering, rs2, rs1, 0b011, rd, 0b0101111);
}
void Assembler::AMOXOR_D(Ordering ordering, GPR rd, GPR rs2, GPR rs1) noexcept {
    EmitAtomic(0b00100, ordering, rs2, rs1, 0b011, rd, 0b0101111);
}
void Assembler::LR_D(Ordering ordering, GPR rd, GPR rs) noexcept {
    EmitAtomic(0b00010, ordering, x0, rs, 0b011, rd, 0b0101111);
}
void Assembler::SC_D(Ordering ordering, GPR rd, GPR rs1, GPR rs2) noexcept {
    EmitAtomic(0b00011, ordering, rs2, rs1, 0b011, rd, 0b0101111);
}

// RV32F Extension Instructions

void Assembler::FADD_S(FPR rd, FPR rs1, FPR rs2, RMode rmode) noexcept {
    EmitRType(0b0000000, rs2, rs1, rmode, rd, 0b1010011);
}
void Assembler::FCLASS_S(GPR rd, FPR rs1) noexcept {
    EmitRType(0b1110000, f0, rs1, 0b0001, rd, 0b1010011);
}
void Assembler::FCVT_S_W(FPR rd, GPR rs1, RMode rmode) noexcept {
    EmitRType(0b1101000, f0, rs1, static_cast<uint32_t>(rmode), rd, 0b1010011);
}
void Assembler::FCVT_S_WU(FPR rd, GPR rs1, RMode rmode) noexcept {
    EmitRType(0b1101000, f1, rs1, static_cast<uint32_t>(rmode), rd, 0b1010011);
}
void Assembler::FCVT_W_S(GPR rd, FPR rs1, RMode rmode) noexcept {
    EmitRType(0b1100000, f0, rs1, static_cast<uint32_t>(rmode), rd, 0b1010011);
}
void Assembler::FCVT_WU_S(GPR rd, FPR rs1, RMode rmode) noexcept {
    EmitRType(0b1100000, f1, rs1, static_cast<uint32_t>(rmode), rd, 0b1010011);
}
void Assembler::FDIV_S(FPR rd, FPR rs1, FPR rs2, RMode rmode) noexcept {
    EmitRType(0b0001100, rs2, rs1, rmode, rd, 0b1010011);
}
void Assembler::FEQ_S(GPR rd, FPR rs1, FPR rs2) noexcept {
    EmitRType(0b1010000, rs2, rs1, 0b010, rd, 0b1010011);
}
void Assembler::FLE_S(GPR rd, FPR rs1, FPR rs2) noexcept {
    EmitRType(0b1010000, rs2, rs1, 0b000, rd, 0b1010011);
}
void Assembler::FLT_S(GPR rd, FPR rs1, FPR rs2) noexcept {
    EmitRType(0b1010000, rs2, rs1, 0b001, rd, 0b1010011);
}
void Assembler::FLW(FPR rd, uint32_t offset, GPR rs) noexcept {
    EmitIType(offset, rs, 0b010, rd, 0b0000111);
}
void Assembler::FMADD_S(FPR rd, FPR rs1, FPR rs2, FPR rs3, RMode rmode) noexcept {
    EmitR4Type(rs3, 0b00, rs2, rs1, rmode, rd, 0b1000011);
}
void Assembler::FMAX_S(FPR rd, FPR rs1, FPR rs2) noexcept {
    EmitRType(0b0010100, rs2, rs1, 0b001, rd, 0b1010011);
}
void Assembler::FMIN_S(FPR rd, FPR rs1, FPR rs2) noexcept {
    EmitRType(0b0010100, rs2, rs1, 0b000, rd, 0b1010011);
}
void Assembler::FMSUB_S(FPR rd, FPR rs1, FPR rs2, FPR rs3, RMode rmode) noexcept {
    EmitR4Type(rs3, 0b00, rs2, rs1, rmode, rd, 0b1000111);
}
void Assembler::FMUL_S(FPR rd, FPR rs1, FPR rs2, RMode rmode) noexcept {
    EmitRType(0b0001000, rs2, rs1, rmode, rd, 0b1010011);
}
void Assembler::FMV_W_X(FPR rd, GPR rs1) noexcept {
    EmitRType(0b1111000, f0, rs1, 0b000, rd, 0b1010011);
}
void Assembler::FMV_X_W(GPR rd, FPR rs1) noexcept {
    EmitRType(0b1110000, f0, rs1, 0b000, rd, 0b1010011);
}
void Assembler::FNMADD_S(FPR rd, FPR rs1, FPR rs2, FPR rs3, RMode rmode) noexcept {
    EmitR4Type(rs3, 0b00, rs2, rs1, rmode, rd, 0b1001111);
}
void Assembler::FNMSUB_S(FPR rd, FPR rs1, FPR rs2, FPR rs3, RMode rmode) noexcept {
    EmitR4Type(rs3, 0b00, rs2, rs1, rmode, rd, 0b1001011);
}
void Assembler::FSGNJ_S(FPR rd, FPR rs1, FPR rs2) noexcept {
    EmitRType(0b0010000, rs2, rs1, 0b000, rd, 0b1010011);
}
void Assembler::FSGNJN_S(FPR rd, FPR rs1, FPR rs2) noexcept {
    EmitRType(0b0010000, rs2, rs1, 0b001, rd, 0b1010011);
}
void Assembler::FSGNJX_S(FPR rd, FPR rs1, FPR rs2) noexcept {
    EmitRType(0b0010000, rs2, rs1, 0b010, rd, 0b1010011);
}
void Assembler::FSQRT_S(FPR rd, FPR rs1, RMode rmode) noexcept {
    EmitRType(0b0101100, f0, rs1, rmode, rd, 0b1010011);
}
void Assembler::FSUB_S(FPR rd, FPR rs1, FPR rs2, RMode rmode) noexcept {
    EmitRType(0b0000100, rs2, rs1, rmode, rd, 0b1010011);
}
void Assembler::FSW(FPR rs2, uint32_t offset, GPR rs1) noexcept {
    EmitSType(offset, rs2, rs1, 0b010, 0b0100111);
}

void Assembler::FABS_S(FPR rd, FPR rs) noexcept {
    FSGNJX_S(rd, rs, rs);
}
void Assembler::FMV_S(FPR rd, FPR rs) noexcept {
    FSGNJ_S(rd, rs, rs);
}
void Assembler::FNEG_S(FPR rd, FPR rs) noexcept {
    FSGNJN_S(rd, rs, rs);
}

// RV64F Extension Instructions

void Assembler::FCVT_L_S(GPR rd, FPR rs1, RMode rmode) noexcept {
    EmitRType(0b1100000, f2, rs1, static_cast<uint32_t>(rmode), rd, 0b1010011);
}
void Assembler::FCVT_LU_S(GPR rd, FPR rs1, RMode rmode) noexcept {
    EmitRType(0b1100000, f3, rs1, static_cast<uint32_t>(rmode), rd, 0b1010011);
}
void Assembler::FCVT_S_L(FPR rd, GPR rs1, RMode rmode) noexcept {
    EmitRType(0b1101000, f2, rs1, static_cast<uint32_t>(rmode), rd, 0b1010011);
}
void Assembler::FCVT_S_LU(FPR rd, GPR rs1, RMode rmode) noexcept {
    EmitRType(0b1101000, f3, rs1, static_cast<uint32_t>(rmode), rd, 0b1010011);
}

// RV32D Extension Instructions

void Assembler::FADD_D(FPR rd, FPR rs1, FPR rs2, RMode rmode) noexcept {
    EmitRType(0b0000001, rs2, rs1, rmode, rd, 0b1010011);
}
void Assembler::FCLASS_D(GPR rd, FPR rs1) noexcept {
    EmitRType(0b1110001, f0, rs1, 0b0001, rd, 0b1010011);
}
void Assembler::FCVT_D_W(FPR rd, GPR rs1, RMode rmode) noexcept {
    EmitRType(0b1101001, f0, rs1, static_cast<uint32_t>(rmode), rd, 0b1010011);
}
void Assembler::FCVT_D_WU(FPR rd, GPR rs1, RMode rmode) noexcept {
    EmitRType(0b1101001, f1, rs1, static_cast<uint32_t>(rmode), rd, 0b1010011);
}
void Assembler::FCVT_W_D(GPR rd, FPR rs1, RMode rmode) noexcept {
    EmitRType(0b1100001, f0, rs1, static_cast<uint32_t>(rmode), rd, 0b1010011);
}
void Assembler::FCVT_WU_D(GPR rd, FPR rs1, RMode rmode) noexcept {
    EmitRType(0b1100001, f1, rs1, static_cast<uint32_t>(rmode), rd, 0b1010011);
}
void Assembler::FCVT_D_S(FPR rd, FPR rs1, RMode rmode) noexcept {
    EmitRType(0b0100001, f0, rs1, static_cast<uint32_t>(rmode), rd, 0b1010011);
}
void Assembler::FCVT_S_D(FPR rd, FPR rs1, RMode rmode) noexcept {
    EmitRType(0b0100000, f1, rs1, static_cast<uint32_t>(rmode), rd, 0b1010011);
}
void Assembler::FDIV_D(FPR rd, FPR rs1, FPR rs2, RMode rmode) noexcept {
    EmitRType(0b0001101, rs2, rs1, rmode, rd, 0b1010011);
}
void Assembler::FEQ_D(GPR rd, FPR rs1, FPR rs2) noexcept {
    EmitRType(0b1010001, rs2, rs1, 0b010, rd, 0b1010011);
}
void Assembler::FLE_D(GPR rd, FPR rs1, FPR rs2) noexcept {
    EmitRType(0b1010001, rs2, rs1, 0b000, rd, 0b1010011);
}
void Assembler::FLT_D(GPR rd, FPR rs1, FPR rs2) noexcept {
    EmitRType(0b1010001, rs2, rs1, 0b001, rd, 0b1010011);
}
void Assembler::FLD(FPR rd, uint32_t offset, GPR rs) noexcept {
    EmitIType(offset, rs, 0b011, rd, 0b0000111);
}
void Assembler::FMADD_D(FPR rd, FPR rs1, FPR rs2, FPR rs3, RMode rmode) noexcept {
    EmitR4Type(rs3, 0b01, rs2, rs1, rmode, rd, 0b1000011);
}
void Assembler::FMAX_D(FPR rd, FPR rs1, FPR rs2) noexcept {
    EmitRType(0b0010101, rs2, rs1, 0b001, rd, 0b1010011);
}
void Assembler::FMIN_D(FPR rd, FPR rs1, FPR rs2) noexcept {
    EmitRType(0b0010101, rs2, rs1, 0b000, rd, 0b1010011);
}
void Assembler::FMSUB_D(FPR rd, FPR rs1, FPR rs2, FPR rs3, RMode rmode) noexcept {
    EmitR4Type(rs3, 0b01, rs2, rs1, rmode, rd, 0b1000111);
}
void Assembler::FMUL_D(FPR rd, FPR rs1, FPR rs2, RMode rmode) noexcept {
    EmitRType(0b0001001, rs2, rs1, rmode, rd, 0b1010011);
}
void Assembler::FNMADD_D(FPR rd, FPR rs1, FPR rs2, FPR rs3, RMode rmode) noexcept {
    EmitR4Type(rs3, 0b01, rs2, rs1, rmode, rd, 0b1001111);
}
void Assembler::FNMSUB_D(FPR rd, FPR rs1, FPR rs2, FPR rs3, RMode rmode) noexcept {
    EmitR4Type(rs3, 0b01, rs2, rs1, rmode, rd, 0b1001011);
}
void Assembler::FSGNJ_D(FPR rd, FPR rs1, FPR rs2) noexcept {
    EmitRType(0b0010001, rs2, rs1, 0b000, rd, 0b1010011);
}
void Assembler::FSGNJN_D(FPR rd, FPR rs1, FPR rs2) noexcept {
    EmitRType(0b0010001, rs2, rs1, 0b001, rd, 0b1010011);
}
void Assembler::FSGNJX_D(FPR rd, FPR rs1, FPR rs2) noexcept {
    EmitRType(0b0010001, rs2, rs1, 0b010, rd, 0b1010011);
}
void Assembler::FSQRT_D(FPR rd, FPR rs1, RMode rmode) noexcept {
    EmitRType(0b0101101, f0, rs1, rmode, rd, 0b1010011);
}
void Assembler::FSUB_D(FPR rd, FPR rs1, FPR rs2, RMode rmode) noexcept {
    EmitRType(0b0000101, rs2, rs1, rmode, rd, 0b1010011);
}
void Assembler::FSD(FPR rs2, uint32_t offset, GPR rs1) noexcept {
    EmitSType(offset, rs2, rs1, 0b011, 0b0100111);
}

void Assembler::FABS_D(FPR rd, FPR rs) noexcept {
    FSGNJX_D(rd, rs, rs);
}
void Assembler::FMV_D(FPR rd, FPR rs) noexcept {
    FSGNJ_D(rd, rs, rs);
}
void Assembler::FNEG_D(FPR rd, FPR rs) noexcept {
    FSGNJN_D(rd, rs, rs);
}

// RV64D Extension Instructions

void Assembler::FCVT_L_D(GPR rd, FPR rs1, RMode rmode) noexcept {
    EmitRType(0b1100001, f2, rs1, static_cast<uint32_t>(rmode), rd, 0b1010011);
}
void Assembler::FCVT_LU_D(GPR rd, FPR rs1, RMode rmode) noexcept {
    EmitRType(0b1100001, f3, rs1, static_cast<uint32_t>(rmode), rd, 0b1010011);
}
void Assembler::FCVT_D_L(FPR rd, GPR rs1, RMode rmode) noexcept {
    EmitRType(0b1101001, f2, rs1, static_cast<uint32_t>(rmode), rd, 0b1010011);
}
void Assembler::FCVT_D_LU(FPR rd, GPR rs1, RMode rmode) noexcept {
    EmitRType(0b1101001, f3, rs1, static_cast<uint32_t>(rmode), rd, 0b1010011);
}
void Assembler::FMV_D_X(FPR rd, GPR rs1) noexcept {
    EmitRType(0b1111001, f0, rs1, 0b000, rd, 0b1010011);
}
void Assembler::FMV_X_D(GPR rd, FPR rs1) noexcept {
    EmitRType(0b1110001, f0, rs1, 0b000, rd, 0b1010011);
}

// RV32Q Extension Instructions

void Assembler::FADD_Q(FPR rd, FPR rs1, FPR rs2, RMode rmode) noexcept {
    EmitRType(0b0000011, rs2, rs1, rmode, rd, 0b1010011);
}
void Assembler::FCLASS_Q(GPR rd, FPR rs1) noexcept {
    EmitRType(0b1110011, f0, rs1, 0b0001, rd, 0b1010011);
}
void Assembler::FCVT_Q_W(FPR rd, GPR rs1, RMode rmode) noexcept {
    EmitRType(0b1101011, f0, rs1, static_cast<uint32_t>(rmode), rd, 0b1010011);
}
void Assembler::FCVT_Q_WU(FPR rd, GPR rs1, RMode rmode) noexcept {
    EmitRType(0b1101011, f1, rs1, static_cast<uint32_t>(rmode), rd, 0b1010011);
}
void Assembler::FCVT_W_Q(GPR rd, FPR rs1, RMode rmode) noexcept {
    EmitRType(0b1100011, f0, rs1, static_cast<uint32_t>(rmode), rd, 0b1010011);
}
void Assembler::FCVT_WU_Q(GPR rd, FPR rs1, RMode rmode) noexcept {
    EmitRType(0b1100011, f1, rs1, static_cast<uint32_t>(rmode), rd, 0b1010011);
}
void Assembler::FCVT_Q_D(FPR rd, FPR rs1, RMode rmode) noexcept {
    EmitRType(0b0100011, f1, rs1, static_cast<uint32_t>(rmode), rd, 0b1010011);
}
void Assembler::FCVT_D_Q(FPR rd, FPR rs1, RMode rmode) noexcept {
    EmitRType(0b0100001, f3, rs1, static_cast<uint32_t>(rmode), rd, 0b1010011);
}
void Assembler::FCVT_Q_S(FPR rd, FPR rs1, RMode rmode) noexcept {
    EmitRType(0b0100011, f0, rs1, static_cast<uint32_t>(rmode), rd, 0b1010011);
}
void Assembler::FCVT_S_Q(FPR rd, FPR rs1, RMode rmode) noexcept {
    EmitRType(0b0100000, f3, rs1, static_cast<uint32_t>(rmode), rd, 0b1010011);
}
void Assembler::FDIV_Q(FPR rd, FPR rs1, FPR rs2, RMode rmode) noexcept {
    EmitRType(0b0001111, rs2, rs1, rmode, rd, 0b1010011);
}
void Assembler::FEQ_Q(GPR rd, FPR rs1, FPR rs2) noexcept {
    EmitRType(0b1010011, rs2, rs1, 0b010, rd, 0b1010011);
}
void Assembler::FLE_Q(GPR rd, FPR rs1, FPR rs2) noexcept {
    EmitRType(0b1010011, rs2, rs1, 0b000, rd, 0b1010011);
}
void Assembler::FLT_Q(GPR rd, FPR rs1, FPR rs2) noexcept {
    EmitRType(0b1010011, rs2, rs1, 0b001, rd, 0b1010011);
}
void Assembler::FLQ(FPR rd, uint32_t offset, GPR rs) noexcept {
    EmitIType(offset, rs, 0b100, rd, 0b0000111);
}
void Assembler::FMADD_Q(FPR rd, FPR rs1, FPR rs2, FPR rs3, RMode rmode) noexcept {
    EmitR4Type(rs3, 0b11, rs2, rs1, rmode, rd, 0b1000011);
}
void Assembler::FMAX_Q(FPR rd, FPR rs1, FPR rs2) noexcept {
    EmitRType(0b0010111, rs2, rs1, 0b001, rd, 0b1010011);
}
void Assembler::FMIN_Q(FPR rd, FPR rs1, FPR rs2) noexcept {
    EmitRType(0b0010111, rs2, rs1, 0b000, rd, 0b1010011);
}
void Assembler::FMSUB_Q(FPR rd, FPR rs1, FPR rs2, FPR rs3, RMode rmode) noexcept {
    EmitR4Type(rs3, 0b11, rs2, rs1, rmode, rd, 0b1000111);
}
void Assembler::FMUL_Q(FPR rd, FPR rs1, FPR rs2, RMode rmode) noexcept {
    EmitRType(0b0001011, rs2, rs1, rmode, rd, 0b1010011);
}
void Assembler::FNMADD_Q(FPR rd, FPR rs1, FPR rs2, FPR rs3, RMode rmode) noexcept {
    EmitR4Type(rs3, 0b11, rs2, rs1, rmode, rd, 0b1001111);
}
void Assembler::FNMSUB_Q(FPR rd, FPR rs1, FPR rs2, FPR rs3, RMode rmode) noexcept {
    EmitR4Type(rs3, 0b11, rs2, rs1, rmode, rd, 0b1001011);
}
void Assembler::FSGNJ_Q(FPR rd, FPR rs1, FPR rs2) noexcept {
    EmitRType(0b0010011, rs2, rs1, 0b000, rd, 0b1010011);
}
void Assembler::FSGNJN_Q(FPR rd, FPR rs1, FPR rs2) noexcept {
    EmitRType(0b0010011, rs2, rs1, 0b001, rd, 0b1010011);
}
void Assembler::FSGNJX_Q(FPR rd, FPR rs1, FPR rs2) noexcept {
    EmitRType(0b0010011, rs2, rs1, 0b010, rd, 0b1010011);
}
void Assembler::FSQRT_Q(FPR rd, FPR rs1, RMode rmode) noexcept {
    EmitRType(0b0101111, f0, rs1, rmode, rd, 0b1010011);
}
void Assembler::FSUB_Q(FPR rd, FPR rs1, FPR rs2, RMode rmode) noexcept {
    EmitRType(0b0000111, rs2, rs1, rmode, rd, 0b1010011);
}
void Assembler::FSQ(FPR rs2, uint32_t offset, GPR rs1) noexcept {
    EmitSType(offset, rs2, rs1, 0b100, 0b0100111);
}

void Assembler::FABS_Q(FPR rd, FPR rs) noexcept {
    FSGNJX_Q(rd, rs, rs);
}
void Assembler::FMV_Q(FPR rd, FPR rs) noexcept {
    FSGNJ_Q(rd, rs, rs);
}
void Assembler::FNEG_Q(FPR rd, FPR rs) noexcept {
    FSGNJN_Q(rd, rs, rs);
}

// RV64Q Extension Instructions

void Assembler::FCVT_L_Q(GPR rd, FPR rs1, RMode rmode) noexcept {
    EmitRType(0b1100011, f2, rs1, static_cast<uint32_t>(rmode), rd, 0b1010011);
}
void Assembler::FCVT_LU_Q(GPR rd, FPR rs1, RMode rmode) noexcept {
    EmitRType(0b1100011, f3, rs1, static_cast<uint32_t>(rmode), rd, 0b1010011);
}
void Assembler::FCVT_Q_L(FPR rd, GPR rs1, RMode rmode) noexcept {
    EmitRType(0b1101011, f2, rs1, static_cast<uint32_t>(rmode), rd, 0b1010011);
}
void Assembler::FCVT_Q_LU(FPR rd, GPR rs1, RMode rmode) noexcept {
    EmitRType(0b1101011, f3, rs1, static_cast<uint32_t>(rmode), rd, 0b1010011);
}

// RVC Extension Instructions

void Assembler::C_ADD(GPR rd, GPR rs) noexcept {
    BISCUIT_ASSERT(rd != x0);
    BISCUIT_ASSERT(rs != x0);
    m_buffer.Emit16(0x9002 | (rd.Index() << 7) | (rs.Index() << 2));
}

void Assembler::C_ADDI(GPR rd, int32_t imm) noexcept {
    BISCUIT_ASSERT(imm != 0);
    BISCUIT_ASSERT(IsValid6BitSignedImm(imm));
    EmitCompressedImmediate(0b000, imm, rd, 0b01);
}

void Assembler::C_ADDIW(GPR rd, int32_t imm) noexcept {
    BISCUIT_ASSERT(IsValid6BitSignedImm(imm));
    EmitCompressedImmediate(0b001, imm, rd, 0b01);
}

void Assembler::C_ADDI4SPN(GPR rd, uint32_t imm) noexcept {
    BISCUIT_ASSERT(imm != 0);
    EmitCompressedWideImmediate(0b000, imm, rd, 0b00);
}

void Assembler::C_ADDW(GPR rd, GPR rs) noexcept {
    EmitCompressedRegArith(0b100111, rd, 0b01, rs, 0b01);
}

void Assembler::C_ADDI16SP(int32_t imm) noexcept {
    BISCUIT_ASSERT(imm != 0);

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
    EmitCompressedRegArith(0b100011, rd, 0b11, rs, 0b01);
}

void Assembler::C_ANDI(GPR rd, uint32_t imm) noexcept {
    BISCUIT_ASSERT(IsValid3BitCompressedReg(rd));

    constexpr auto base = 0x8801U;
    const auto shift_enc = ((imm & 0b11111) << 2) | ((imm & 0b100000) << 7);
    const auto reg = CompressedRegTo3BitEncoding(rd);

    m_buffer.Emit16(base | shift_enc | (reg << 7));
}

void Assembler::C_BEQZ(GPR rs, int32_t offset) noexcept {
    EmitCompressedBranch(0b110, offset, rs, 0b01);
}

void Assembler::C_BEQZ(GPR rs, Label* label) noexcept {
    const auto address = LinkAndGetOffset(label);
    C_BEQZ(rs, static_cast<int32_t>(address));
}

void Assembler::C_BNEZ(GPR rs, int32_t offset) noexcept {
    EmitCompressedBranch(0b111, offset, rs, 0b01);
}

void Assembler::C_BNEZ(GPR rs, Label* label) noexcept {
    const auto address = LinkAndGetOffset(label);
    C_BNEZ(rs, static_cast<int32_t>(address));
}

void Assembler::C_EBREAK() noexcept {
    m_buffer.Emit16(0x9002);
}

void Assembler::C_FLD(FPR rd, uint32_t imm, GPR rs) noexcept {
    EmitCompressedLoad(0b001, imm, rs, rd, 0b00);
}

void Assembler::C_FLW(FPR rd, uint32_t imm, GPR rs) noexcept {
    imm &= 0x7C;
    const auto new_imm = ((imm & 0b0100) << 5) | (imm & 0x78);
    EmitCompressedLoad(0b011, new_imm, rs, rd, 0b00);
}

void Assembler::C_FSD(FPR rs2, uint32_t imm, GPR rs1) noexcept {
    EmitCompressedStore(0b101, imm, rs1, rs2, 0b00);
}

void Assembler::C_J(Label* label) noexcept {
    const auto address = LinkAndGetOffset(label);
    BISCUIT_ASSERT(IsValidCJTypeImm(address));
    C_J(static_cast<int32_t>(address));
}

void Assembler::C_J(int32_t offset) noexcept {
    EmitCompressedJump(0b101, offset, 0b01);
}

void Assembler::C_JAL(Label* label) noexcept {
    const auto address = LinkAndGetOffset(label);
    BISCUIT_ASSERT(IsValidCJTypeImm(address));
    C_JAL(static_cast<int32_t>(address));
}

void Assembler::C_JAL(int32_t offset) noexcept {
    EmitCompressedJump(0b001, offset, 0b01);
}

void Assembler::C_FSW(FPR rs2, uint32_t imm, GPR rs1) noexcept {
    imm &= 0x7C;
    const auto new_imm = ((imm & 0b0100) << 5) | (imm & 0x78);
    EmitCompressedStore(0b111, new_imm, rs1, rs2, 0b00);
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
    EmitCompressedLoad(0b011, imm, rs, rd, 0b00);
}

void Assembler::C_LI(GPR rd, int32_t imm) noexcept {
    BISCUIT_ASSERT(IsValid6BitSignedImm(imm));
    EmitCompressedImmediate(0b010, imm, rd, 0b01);
}

void Assembler::C_LQ(GPR rd, uint32_t imm, GPR rs) noexcept {
    imm &= 0x1F0;
    const auto new_imm = ((imm & 0x100) >> 5) | (imm & 0xF0);
    EmitCompressedLoad(0b001, new_imm, rs, rd, 0b00);
}

void Assembler::C_LUI(GPR rd, uint32_t imm) noexcept {
    BISCUIT_ASSERT(imm != 0);
    BISCUIT_ASSERT(rd != x0 && rd != x2);

    const auto new_imm = (imm & 0x3F000) >> 12;
    EmitCompressedImmediate(0b011, new_imm, rd, 0b01);
}

void Assembler::C_LW(GPR rd, uint32_t imm, GPR rs) noexcept {
    imm &= 0x7C;
    const auto new_imm = ((imm & 0b0100) << 5) | (imm & 0x78);
    EmitCompressedLoad(0b010, new_imm, rs, rd, 0b00);
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
    EmitCompressedRegArith(0b100011, rd, 0b10, rs, 0b01);
}

void Assembler::C_SD(GPR rs2, uint32_t imm, GPR rs1) noexcept {
    EmitCompressedLoad(0b111, imm, rs1, rs2, 0b00);
}

void Assembler::C_SLLI(GPR rd, uint32_t shift) noexcept {
    BISCUIT_ASSERT(rd != x0);
    BISCUIT_ASSERT(shift != 0);

    const auto shift_enc = ((shift & 0b11111) << 2) | ((shift & 0b100000) << 7);
    m_buffer.Emit16(0x0002U | shift_enc | (rd.Index() << 7));
}

void Assembler::C_SQ(GPR rs2, uint32_t imm, GPR rs1) noexcept {
    imm &= 0x1F0;
    const auto new_imm = ((imm & 0x100) >> 5) | (imm & 0xF0);
    EmitCompressedStore(0b101, new_imm, rs1, rs2, 0b00);
}

void Assembler::C_SRAI(GPR rd, uint32_t shift) noexcept {
    BISCUIT_ASSERT(IsValid3BitCompressedReg(rd));
    BISCUIT_ASSERT(shift != 0);

    constexpr auto base = 0x8401U;
    const auto shift_enc = ((shift & 0b11111) << 2) | ((shift & 0b100000) << 7);
    const auto reg = CompressedRegTo3BitEncoding(rd);

    m_buffer.Emit16(base | shift_enc | (reg << 7));
}

void Assembler::C_SRLI(GPR rd, uint32_t shift) noexcept {
    BISCUIT_ASSERT(IsValid3BitCompressedReg(rd));
    BISCUIT_ASSERT(shift != 0);

    constexpr auto base = 0x8001U;
    const auto shift_enc = ((shift & 0b11111) << 2) | ((shift & 0b100000) << 7);
    const auto reg = CompressedRegTo3BitEncoding(rd);

    m_buffer.Emit16(base | shift_enc | (reg << 7));
}

void Assembler::C_SUB(GPR rd, GPR rs) noexcept {
    EmitCompressedRegArith(0b100011, rd, 0b00, rs, 0b01);
}

void Assembler::C_SUBW(GPR rd, GPR rs) noexcept {
    EmitCompressedRegArith(0b100111, rd, 0b00, rs, 0b01);
}

void Assembler::C_SW(GPR rs2, uint32_t imm, GPR rs1) noexcept {
    imm &= 0x7C;
    const auto new_imm = ((imm & 0b0100) << 5) | (imm & 0x78);
    EmitCompressedStore(0b110, new_imm, rs1, rs2, 0b00);
}

void Assembler::C_UNDEF() noexcept {
    m_buffer.Emit16(0);
}

void Assembler::C_XOR(GPR rd, GPR rs) noexcept {
    EmitCompressedRegArith(0b100011, rd, 0b01, rs, 0b01);
}

void Assembler::EmitAtomic(uint32_t funct5, Ordering ordering, GPR rs2, GPR rs1, uint32_t funct3, GPR rd, uint32_t opcode) noexcept {
    const auto funct7 = (funct5 << 2) | static_cast<uint32_t>(ordering);
    EmitRType(funct7, rs2, rs1, funct3, rd, opcode);
}

void Assembler::EmitBType(uint32_t imm, GPR rs2, GPR rs1, uint32_t funct3, uint32_t opcode) noexcept {
    imm &= 0x1FFE;

    m_buffer.Emit32(TransformToBTypeImm(imm) | (rs2.Index() << 20) | (rs1.Index() << 15) | ((funct3 & 0b111) << 12) | (opcode & 0x7F));
}

void Assembler::EmitIType(uint32_t imm, Register rs1, uint32_t funct3, Register rd, uint32_t opcode) noexcept {
    imm &= 0xFFF;

    m_buffer.Emit32((imm << 20) | (rs1.Index() << 15) | ((funct3 & 0b111) << 12) | (rd.Index() << 7) | (opcode & 0x7F));
}

void Assembler::EmitJType(uint32_t imm, GPR rd, uint32_t opcode) noexcept {
    imm &= 0x1FFFFE;

    m_buffer.Emit32(TransformToJTypeImm(imm) | rd.Index() << 7 | (opcode & 0x7F));
}

void Assembler::EmitRType(uint32_t funct7, Register rs2, Register rs1, uint32_t funct3, Register rd, uint32_t opcode) noexcept {
    m_buffer.Emit32(((funct7 & 0xFF) << 25) | (rs2.Index() << 20) | (rs1.Index() << 15) |
                            ((funct3 & 0b111) << 12) | (rd.Index() << 7) | (opcode & 0x7F));
}

void Assembler::EmitRType(uint32_t funct7, FPR rs2, FPR rs1, RMode funct3, FPR rd, uint32_t opcode) noexcept {
    m_buffer.Emit32(((funct7 & 0xFF) << 25) | (rs2.Index() << 20) | (rs1.Index() << 15) |
                    (static_cast<uint32_t>(funct3) << 12) | (rd.Index() << 7) | (opcode & 0x7F));
}

void Assembler::EmitR4Type(FPR rs3, uint32_t funct2, FPR rs2, FPR rs1, RMode funct3, FPR rd, uint32_t opcode) noexcept {
    const auto reg_bits = (rs3.Index() << 27) | (rs2.Index() << 20) | (rs1.Index() << 15) | (rd.Index() << 7);
    const auto funct_bits = ((funct2 & 0b11) << 25) | (static_cast<uint32_t>(funct3) << 12);
    m_buffer.Emit32(reg_bits | funct_bits | (opcode & 0x7F));
}

void Assembler::EmitSType(uint32_t imm, Register rs2, GPR rs1, uint32_t funct3, uint32_t opcode) noexcept {
    imm &= 0xFFF;

    // clang-format off
    const auto new_imm = ((imm & 0x01F) << 7) |
                         ((imm & 0xFE0) << 20);
    // clang-format on

    m_buffer.Emit32(new_imm | (rs2.Index() << 20) | (rs1.Index() << 15) | ((funct3 & 0b111) << 12) | (opcode & 0x7F));
}

void Assembler::EmitUType(uint32_t imm, GPR rd, uint32_t opcode) noexcept {
    m_buffer.Emit32((imm & 0xFFFFF000) | rd.Index() << 7 | (opcode & 0x7F));
}

void Assembler::EmitFENCE(uint32_t fm, FenceOrder pred, FenceOrder succ, GPR rs, uint32_t funct3, GPR rd, uint32_t opcode) noexcept {
    // clang-format off
    m_buffer.Emit32(((fm & 0b1111) << 28) |
                    (static_cast<uint32_t>(pred) << 24) |
                    (static_cast<uint32_t>(succ) << 20) |
                    (rs.Index() << 15) |
                    ((funct3 & 0b111) << 12) |
                    (rd.Index() << 7) |
                    (opcode & 0x7F));
    // clang-format on
}

void Assembler::EmitCompressedBranch(uint32_t funct3, int32_t offset, GPR rs, uint32_t op) noexcept {
    BISCUIT_ASSERT(IsValidCBTypeImm(offset));
    BISCUIT_ASSERT(IsValid3BitCompressedReg(rs));

    const auto transformed_imm = TransformToCBTypeImm(static_cast<uint32_t>(offset));
    const auto rs_san = CompressedRegTo3BitEncoding(rs);
    m_buffer.Emit16(((funct3 & 0b111) << 13) | transformed_imm | (rs_san << 7) | (op & 0b11));
}

void Assembler::EmitCompressedJump(uint32_t funct3, int32_t offset, uint32_t op) noexcept {
    BISCUIT_ASSERT(IsValidCJTypeImm(offset));
    m_buffer.Emit16(TransformToCJTypeImm(static_cast<uint32_t>(offset)) | ((funct3 & 0b111) << 13) | (op & 0b11));
}

void Assembler::EmitCompressedImmediate(uint32_t funct3, uint32_t imm, GPR rd, uint32_t op) noexcept {
    BISCUIT_ASSERT(rd != x0);
    const auto new_imm = ((imm & 0b11111) << 2) | ((imm & 0b100000) << 7);
    m_buffer.Emit16(((funct3 & 0b111) << 13) | new_imm | (rd.Index() << 7) | (op & 0b11));
}

void Assembler::EmitCompressedLoad(uint32_t funct3, uint32_t imm, GPR rs, Register rd, uint32_t op) noexcept {
    BISCUIT_ASSERT(IsValid3BitCompressedReg(rs));
    BISCUIT_ASSERT(IsValid3BitCompressedReg(rd));

    imm &= 0xF8;

    const auto imm_enc = ((imm & 0x38) << 7) | ((imm & 0xC0) >> 1);
    const auto rd_san = CompressedRegTo3BitEncoding(rd);
    const auto rs_san = CompressedRegTo3BitEncoding(rs);
    m_buffer.Emit16(((funct3 & 0b111) << 13) | imm_enc | (rs_san << 7) | (rd_san << 2) | (op & 0b11));
}

void Assembler::EmitCompressedRegArith(uint32_t funct6, GPR rd, uint32_t funct2, GPR rs, uint32_t op) noexcept {
    BISCUIT_ASSERT(IsValid3BitCompressedReg(rs));
    BISCUIT_ASSERT(IsValid3BitCompressedReg(rd));

    const auto rd_san = CompressedRegTo3BitEncoding(rd);
    const auto rs_san = CompressedRegTo3BitEncoding(rs);
    m_buffer.Emit16(((funct6 & 0b111111) << 10) | (rd_san << 7) | ((funct2 & 0b11) << 5) | (rs_san << 2) | (op & 0b11));
}

void Assembler::EmitCompressedStore(uint32_t funct3, uint32_t imm, GPR rs1, Register rs2, uint32_t op) noexcept {
    // This has the same format as a compressed load, with rs2 taking the place of rd.
    // We can reuse the code we've already written to handle this.
    EmitCompressedLoad(funct3, imm, rs1, rs2, op);
}

void Assembler::EmitCompressedWideImmediate(uint32_t funct3, uint32_t imm, GPR rd, uint32_t op) noexcept {
    BISCUIT_ASSERT(IsValid3BitCompressedReg(rd));
    const auto rd_sanitized = CompressedRegTo3BitEncoding(rd);
    m_buffer.Emit16(((funct3 & 0b111) << 13) | ((imm & 0xFF) << 5) | (rd_sanitized << 2) | (op & 0b11));
}

void Assembler::BindToOffset(Label* label, Label::LocationOffset offset) {
    BISCUIT_ASSERT(label != nullptr);
    BISCUIT_ASSERT(offset >= 0 && offset <= m_buffer.GetCursorOffset());

    label->Bind(offset);
    ResolveLabelOffsets(label);
    label->ClearOffsets();
}

ptrdiff_t Assembler::LinkAndGetOffset(Label* label) {
    BISCUIT_ASSERT(label != nullptr);

    // If we have a bound label, then it's straightforward to calculate
    // the offsets.
    if (label->IsBound()) {
        const auto cursor_address = m_buffer.GetCursorAddress();
        const auto label_offset = m_buffer.GetOffsetAddress(static_cast<uintptr_t>(*label->GetLocation()));
        return static_cast<ptrdiff_t>(label_offset - cursor_address);
    }

    // If we don't have a bound location, we return an offset of zero.
    // While the emitter will emit a bogus branch instruction initially,
    // the offset will be patched over once the label has been properly
    // bound to a location.
    label->AddOffset(m_buffer.GetCursorOffset());
    return 0;
}

void Assembler::ResolveLabelOffsets(Label* label) {
    // Conditional branch instructions make use of the B-type immediate encoding for offsets.
    const auto is_b_type = [](uint32_t instruction) {
        return (instruction & 0x7F) == 0b1100011;
    };
    // JAL makes use of the J-type immediate encoding for offsets.
    const auto is_j_type = [](uint32_t instruction) {
        return (instruction & 0x7F) == 0b1101111;
    };
    // C.BEQZ and C.BNEZ make use of this encoding type.
    const auto is_cb_type = [](uint32_t instruction) {
        const auto op = instruction & 0b11;
        const auto funct3 = instruction & 0xE000;
        return op == 0b01 && funct3 >= 0xC000;
    };
    // C.JAL and C.J make use of this encoding type.
    const auto is_cj_type = [](uint32_t instruction) {
        const auto op = instruction & 0b11;
        const auto funct3 = instruction & 0xE000;
        return op == 0b01 && (funct3 == 0x2000 || funct3 == 0xA000);
    };
    // If we know an instruction is a compressed branch, then it's a 16-bit instruction
    // Otherwise it's a regular-sized 32-bit instruction.
    const auto determine_inst_size = [&](uint32_t instruction) -> size_t {
        if (is_cj_type(instruction) || is_cb_type(instruction)) {
            return 2;
        } else {
            return 4;
        }
    };

    const auto label_location = *label->GetLocation();

    for (const auto offset : label->m_offsets) {
        const auto address = m_buffer.GetOffsetAddress(offset);
        auto* const ptr = reinterpret_cast<uint8_t*>(address);
        const auto inst_size = determine_inst_size(uint32_t{*ptr} | (uint32_t{*(ptr + 1)} << 8));

        uint32_t instruction = 0;
        std::memcpy(&instruction, ptr, inst_size);

        // Given all branch instructions we need to patch have 0 encoded as
        // their branch offset, we don't need to worry about any masking work.
        //
        // It's enough to verify that the immediate is going to be valid
        // and then OR it into the instruction.

        const auto encoded_offset = label_location - offset;

        if (inst_size == sizeof(uint32_t)) {
            if (is_b_type(instruction)) {
                BISCUIT_ASSERT(IsValidBTypeImm(encoded_offset));
                instruction |= TransformToBTypeImm(static_cast<uint32_t>(encoded_offset));
            } else if (is_j_type(instruction)) {
                BISCUIT_ASSERT(IsValidJTypeImm(encoded_offset));
                instruction |= TransformToJTypeImm(static_cast<uint32_t>(encoded_offset));
            }
        } else {
            if (is_cb_type(instruction)) {
                BISCUIT_ASSERT(IsValidCBTypeImm(encoded_offset));
                instruction |= TransformToCBTypeImm(static_cast<uint32_t>(encoded_offset));
            } else if (is_cj_type(instruction)) {
                BISCUIT_ASSERT(IsValidCJTypeImm(encoded_offset));
                instruction |= TransformToCJTypeImm(static_cast<uint32_t>(encoded_offset));
            }
        }

        std::memcpy(ptr, &instruction, inst_size);
    }
}

} // namespace biscuit
