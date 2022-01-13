#include <biscuit/assert.hpp>
#include <biscuit/assembler.hpp>

namespace biscuit {
void Assembler::AES32DSI(GPR rd, GPR rs1, GPR rs2, uint32_t bs) noexcept {
    BISCUIT_ASSERT(bs <= 0b11);
    m_buffer.Emit32(0x2A000033U | (bs << 30) | (rs2.Index() << 20) |
                    (rs1.Index() << 15) | (rd.Index() << 7));
}

void Assembler::AES32DSMI(GPR rd, GPR rs1, GPR rs2, uint32_t bs) noexcept {
    BISCUIT_ASSERT(bs <= 0b11);
    m_buffer.Emit32(0x2E000033U | (bs << 30) | (rs2.Index() << 20) |
                    (rs1.Index() << 15) | (rd.Index() << 7));
}
} // namespace biscuit
