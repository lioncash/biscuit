#include <biscuit/assert.hpp>
#include <biscuit/assembler.hpp>

#include <cstring>

namespace biscuit {

// B-type immediates only provide ~4KB branches.
static bool IsValidBTypeImm(ptrdiff_t value) noexcept {
    return value >= -2048 && value <= 2047;
}

// J-type immediates only provide ~1MB length branches.
static bool IsValidJTypeImm(ptrdiff_t value) noexcept {
    return value >= -0x80000 && value <= 0x7FFFF;
}

// Transforms a regular value into an immediate encoded in a B-type instruction.
static uint32_t TransformToBTypeImm(uint32_t imm) noexcept {
    // clang-format off
    return ((imm & 0x07E0) << 20) |
           ((imm & 0x1000) << 19) |
           ((imm & 0x001E) << 7) |
           ((imm & 0x0800) >> 4);
    // clang-format on
}

// Transforms a regular value into an immediate encoded in a J-type instruction.
static uint32_t TransformToJTypeImm(uint32_t imm) noexcept {
    // clang-format off
    return ((imm & 0x0FF000) >> 0) |
           ((imm & 0x000800) << 9) |
           ((imm & 0x0007FE) << 20) |
           ((imm & 0x100000) << 11);
    // clang-format on
}

void Assembler::Bind(Label* label) {
    BindToOffset(label, m_buffer.GetCursorOffset());
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

    const auto label_location = *label->GetLocation();

    for (const auto offset : label->m_offsets) {
        const auto address = m_buffer.GetOffsetAddress(offset);
        auto* const ptr = reinterpret_cast<uint8_t*>(address);

        // TODO: Note that this needs to be adjusted to compensate for
        //       RISC-V compressed branch instructions, which are 16-bit,
        //       not 32-bit.
        uint32_t instruction = 0;
        std::memcpy(&instruction, ptr, sizeof(instruction));

        // Given all branch instructions we need to patch have 0 encoded as
        // their branch offset, we don't need to worry about any masking work.
        //
        // It's enough to verify that the immediate is going to be valid
        // and then OR it into the instruction.

        const auto encoded_offset = label_location - offset;

        if (is_b_type(instruction)) {
            BISCUIT_ASSERT(IsValidBTypeImm(encoded_offset));
            instruction |= TransformToBTypeImm(static_cast<uint32_t>(encoded_offset));
        } else if (is_j_type(instruction)) {
            BISCUIT_ASSERT(IsValidJTypeImm(encoded_offset));
            instruction |= TransformToJTypeImm(static_cast<uint32_t>(encoded_offset));
        }

        std::memcpy(ptr, &instruction, sizeof(uint32_t));
    }
}

void Assembler::BEQ(GPR rs1, GPR rs2, Label* label) noexcept {
    const auto address = LinkAndGetOffset(label);
    BISCUIT_ASSERT(IsValidBTypeImm(address));
    BEQ(rs1, rs2, static_cast<uint32_t>(address));
}

void Assembler::BEQZ(GPR rs, Label* label) noexcept {
    const auto address = LinkAndGetOffset(label);
    BISCUIT_ASSERT(IsValidBTypeImm(address));
    BEQZ(rs, static_cast<uint32_t>(address));
}

void Assembler::BGE(GPR rs1, GPR rs2, Label* label) noexcept {
    const auto address = LinkAndGetOffset(label);
    BISCUIT_ASSERT(IsValidBTypeImm(address));
    BGE(rs1, rs2, static_cast<uint32_t>(address));
}

void Assembler::BGEU(GPR rs1, GPR rs2, Label* label) noexcept {
    const auto address = LinkAndGetOffset(label);
    BISCUIT_ASSERT(IsValidBTypeImm(address));
    BGEU(rs1, rs2, static_cast<uint32_t>(address));
}

void Assembler::BGEZ(GPR rs, Label* label) noexcept {
    const auto address = LinkAndGetOffset(label);
    BISCUIT_ASSERT(IsValidBTypeImm(address));
    BGEZ(rs, static_cast<uint32_t>(address));
}

void Assembler::BGT(GPR rs, GPR rt, Label* label) noexcept {
    const auto address = LinkAndGetOffset(label);
    BISCUIT_ASSERT(IsValidBTypeImm(address));
    BGT(rs, rt, static_cast<uint32_t>(address));
}

void Assembler::BGTU(GPR rs, GPR rt, Label* label) noexcept {
    const auto address = LinkAndGetOffset(label);
    BISCUIT_ASSERT(IsValidBTypeImm(address));
    BGTU(rs, rt, static_cast<uint32_t>(address));
}

void Assembler::BGTZ(GPR rs, Label* label) noexcept {
    const auto address = LinkAndGetOffset(label);
    BISCUIT_ASSERT(IsValidBTypeImm(address));
    BGTZ(rs, static_cast<uint32_t>(address));
}

void Assembler::BLE(GPR rs, GPR rt, Label* label) noexcept {
    const auto address = LinkAndGetOffset(label);
    BISCUIT_ASSERT(IsValidBTypeImm(address));
    BLE(rs, rt, static_cast<uint32_t>(address));
}

void Assembler::BLEU(GPR rs, GPR rt, Label* label) noexcept {
    const auto address = LinkAndGetOffset(label);
    BISCUIT_ASSERT(IsValidBTypeImm(address));
    BLEU(rs, rt, static_cast<uint32_t>(address));
}

void Assembler::BLEZ(GPR rs, Label* label) noexcept {
    const auto address = LinkAndGetOffset(label);
    BISCUIT_ASSERT(IsValidBTypeImm(address));
    BLEZ(rs, static_cast<uint32_t>(address));
}

void Assembler::BLT(GPR rs1, GPR rs2, Label* label) noexcept {
    const auto address = LinkAndGetOffset(label);
    BISCUIT_ASSERT(IsValidBTypeImm(address));
    BLT(rs1, rs2, static_cast<uint32_t>(address));
}

void Assembler::BLTU(GPR rs1, GPR rs2, Label* label) noexcept {
    const auto address = LinkAndGetOffset(label);
    BISCUIT_ASSERT(IsValidBTypeImm(address));
    BLTU(rs1, rs2, static_cast<uint32_t>(address));
}

void Assembler::BLTZ(GPR rs, Label* label) noexcept {
    const auto address = LinkAndGetOffset(label);
    BISCUIT_ASSERT(IsValidBTypeImm(address));
    BLTZ(rs, static_cast<uint32_t>(address));
}

void Assembler::BNE(GPR rs1, GPR rs2, Label* label) noexcept {
    const auto address = LinkAndGetOffset(label);
    BISCUIT_ASSERT(IsValidBTypeImm(address));
    BNE(rs1, rs2, static_cast<uint32_t>(address));
}

void Assembler::BNEZ(GPR rs, Label* label) noexcept {
    const auto address = LinkAndGetOffset(label);
    BISCUIT_ASSERT(IsValidBTypeImm(address));
    BNEZ(rs, static_cast<uint32_t>(address));
}

void Assembler::BEQ(GPR rs1, GPR rs2, uint32_t imm) noexcept {
    EmitBType(imm, rs2, rs1, 0b000, 0b1100011);
}

void Assembler::BEQZ(GPR rs, uint32_t imm) noexcept {
    BEQ(rs, x0, imm);
}

void Assembler::BGE(GPR rs1, GPR rs2, uint32_t imm) noexcept {
    EmitBType(imm, rs2, rs1, 0b101, 0b1100011);
}

void Assembler::BGEU(GPR rs1, GPR rs2, uint32_t imm) noexcept {
    EmitBType(imm, rs2, rs1, 0b111, 0b1100011);
}

void Assembler::BGEZ(GPR rs, uint32_t imm) noexcept {
    BGE(rs, x0, imm);
}

void Assembler::BGT(GPR rs, GPR rt, uint32_t imm) noexcept {
    BLT(rt, rs, imm);
}

void Assembler::BGTU(GPR rs, GPR rt, uint32_t imm) noexcept {
    BLTU(rt, rs, imm);
}

void Assembler::BGTZ(GPR rs, uint32_t imm) noexcept {
    BLT(x0, rs, imm);
}

void Assembler::BLE(GPR rs, GPR rt, uint32_t imm) noexcept {
    BGE(rt, rs, imm);
}

void Assembler::BLEU(GPR rs, GPR rt, uint32_t imm) noexcept {
    BGEU(rt, rs, imm);
}

void Assembler::BLEZ(GPR rs, uint32_t imm) noexcept {
    BGE(x0, rs, imm);
}

void Assembler::BLT(GPR rs1, GPR rs2, uint32_t imm) noexcept {
    EmitBType(imm, rs2, rs1, 0b100, 0b1100011);
}

void Assembler::BLTU(GPR rs1, GPR rs2, uint32_t imm) noexcept {
    EmitBType(imm, rs2, rs1, 0b110, 0b1100011);
}

void Assembler::BLTZ(GPR rs, uint32_t imm) noexcept {
    BLT(rs, x0, imm);
}

void Assembler::BNE(GPR rs1, GPR rs2, uint32_t imm) noexcept {
    EmitBType(imm, rs2, rs1, 0b001, 0b1100011);
}

void Assembler::BNEZ(GPR rs, uint32_t imm) noexcept {
    BNE(x0, rs, imm);
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

void Assembler::J(uint32_t imm) noexcept {
    JAL(x0, imm);
}

void Assembler::JAL(uint32_t imm) noexcept {
    EmitJType(imm, x1, 0b1101111);
}

void Assembler::JAL(GPR rd, uint32_t imm) noexcept {
    EmitJType(imm, rd, 0b1101111);
}

void Assembler::JALR(GPR rs) noexcept {
    JALR(x1, 0, rs);
}

void Assembler::JALR(GPR rd, uint32_t imm, GPR rs1) noexcept {
    EmitIType(imm, rs1, 0b000, rd, 0b1100111);
}

void Assembler::JR(GPR rs) noexcept {
    JALR(x0, 0, rs);
}

} // namespace biscuit
