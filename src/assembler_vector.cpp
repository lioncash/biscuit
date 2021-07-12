#include <biscuit/assert.hpp>
#include <biscuit/assembler.hpp>

namespace biscuit {

void Assembler::VSETIVLI(GPR rd, uint32_t imm, SEW sew, LMUL lmul, VTA vta, VMA vma) noexcept {
    imm &= 0b11111;

    // clang-format off
    const auto zimm = static_cast<uint32_t>(lmul) |
                      (static_cast<uint32_t>(sew) << 3) |
                      (static_cast<uint32_t>(vta) << 6) |
                      (static_cast<uint32_t>(vma) << 7);
    // clang-format on

    m_buffer.Emit32(0xC0007057U | (zimm << 20) | (imm << 15) | (rd.Index() << 7));
}

void Assembler::VSETVL(GPR rd, GPR rs1, GPR rs2) noexcept {
    m_buffer.Emit32(0x80007057U | (rs2.Index() << 20) | (rs1.Index() << 15) | (rd.Index() << 7));
}

void Assembler::VSETVLI(GPR rd, GPR rs, SEW sew, LMUL lmul, VTA vta, VMA vma) noexcept {
    // clang-format off
    const auto zimm = static_cast<uint32_t>(lmul) |
                      (static_cast<uint32_t>(sew) << 3) |
                      (static_cast<uint32_t>(vta) << 6) |
                      (static_cast<uint32_t>(vma) << 7);
    // clang-format on

    m_buffer.Emit32(0x00007057U | (zimm << 20) | (rs.Index() << 15) | (rd.Index() << 7));
}

} // namespace biscuit
