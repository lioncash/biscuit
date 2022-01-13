#include <biscuit/assert.hpp>
#include <biscuit/assembler.hpp>

namespace biscuit {
namespace {
void EmitAES32Instruction(CodeBuffer& buffer, uint32_t op, GPR rd, GPR rs1, GPR rs2, uint32_t bs) noexcept {
    BISCUIT_ASSERT(bs <= 0b11);
    buffer.Emit32(op | (bs << 30) | (rs2.Index() << 20) |
                  (rs1.Index() << 15) | (rd.Index() << 7));
}
} // Anonymous namespace

void Assembler::AES32DSI(GPR rd, GPR rs1, GPR rs2, uint32_t bs) noexcept {
    EmitAES32Instruction(m_buffer, 0x2A000033U, rd, rs1, rs2, bs);
}

void Assembler::AES32DSMI(GPR rd, GPR rs1, GPR rs2, uint32_t bs) noexcept {
    EmitAES32Instruction(m_buffer, 0x2E000033U, rd, rs1, rs2, bs);
}

void Assembler::AES32ESI(GPR rd, GPR rs1, GPR rs2, uint32_t bs) noexcept {
    EmitAES32Instruction(m_buffer, 0x22000033U, rd, rs1, rs2, bs);
}

void Assembler::AES32ESMI(GPR rd, GPR rs1, GPR rs2, uint32_t bs) noexcept {
    EmitAES32Instruction(m_buffer, 0x26000033U, rd, rs1, rs2, bs);
}
} // namespace biscuit
