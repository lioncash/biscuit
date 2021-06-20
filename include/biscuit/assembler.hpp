#pragma once

#include <biscuit/code_buffer.hpp>
#include <biscuit/registers.hpp>
#include <cstddef>
#include <cstdint>

namespace biscuit {

/**
 * Base class that all assembler instances will inherit from
 * to generate RISC-V code.
 */
class Assembler {
public:
    /**
     * Constructor
     *
     * Initializes the underlying code buffer to be able to hold `capacity` bytes.
     *
     * @param capacity The capacity for the underlying code buffer in bytes.
     *                 If no capacity is specified, then the underlying buffer
     *                 will be 4KB in size.
     */
    [[nodiscard]] explicit Assembler(size_t capacity = CodeBuffer::default_capacity)
        : m_buffer(capacity) {}

   /**
     * Constructor
     *
     * @param buffer   A non-null pointer to an allocated buffer of size `capacity`.
     * @param capacity The capacity of the memory pointed to by `buffer`.
     *
     * @pre The given memory buffer must not be null.
     * @pre The given memory buffer must be at minimum `capacity` bytes in size.
     *
     * @note The caller is responsible for managing the lifetime of the given memory.
     *       CodeBuffer will *not* free the memory once it goes out of scope.
     */
    [[nodiscard]] explicit Assembler(uint8_t* buffer, size_t capacity)
        : m_buffer(buffer, capacity) {}

    // Copy constructor and assignment.
    Assembler(const Assembler&) = delete;
    Assembler& operator=(const Assembler&) = delete;

    // Move constructor and assignment.
    Assembler(Assembler&&) = default;
    Assembler& operator=(Assembler&&) = default;

    // Destructor
    ~Assembler() = default;

    /**
     * Allows rewinding of the code buffer cursor.
     *
     * @param offset The offset to rewind the cursor by.
     *
     * @note If no offset is provided, then this function rewinds the
     *       cursor to the beginning of the buffer.
     *
     * @note The offset may not be larger than the current cursor offset
     *       and may not be less than the current buffer starting address.
     */
    void RewindBuffer(ptrdiff_t offset = 0) {
        m_buffer.RewindCursor(offset);
    }

    // RV32I Instructions

    void AUIPC(GPR rd, uint32_t imm) noexcept {
        EmitUType(imm, rd, 0b0010111);
    }

    void BEQ(GPR rs1, GPR rs2, uint32_t imm) noexcept {
        EmitBType(imm, rs2, rs1, 0b000, 0b1100011);
    }

    void BEQZ(GPR rs, uint32_t imm) noexcept {
        BEQ(rs, x0, imm);
    }

    void BGE(GPR rs1, GPR rs2, uint32_t imm) noexcept {
        EmitBType(imm, rs2, rs1, 0b101, 0b1100011);
    }

    void BGEU(GPR rs1, GPR rs2, uint32_t imm) noexcept {
        EmitBType(imm, rs2, rs1, 0b111, 0b1100011);
    }

    void BGEZ(GPR rs, uint32_t imm) noexcept {
        BGE(rs, x0, imm);
    }

    void BGT(GPR rs, GPR rt, uint32_t imm) noexcept {
        BLT(rt, rs, imm);
    }

    void BGTU(GPR rs, GPR rt, uint32_t imm) noexcept {
        BLTU(rt, rs, imm);
    }

    void BGTZ(GPR rs, uint32_t imm) noexcept {
        BLT(x0, rs, imm);
    }

    void BLE(GPR rs, GPR rt, uint32_t imm) noexcept {
        BGE(rt, rs, imm);
    }

    void BLEU(GPR rs, GPR rt, uint32_t imm) noexcept {
        BGEU(rt, rs, imm);
    }

    void BLEZ(GPR rs, uint32_t imm) noexcept {
        BGE(x0, rs, imm);
    }

    void BLT(GPR rs1, GPR rs2, uint32_t imm) noexcept {
        EmitBType(imm, rs2, rs1, 0b100, 0b1100011);
    }

    void BLTU(GPR rs1, GPR rs2, uint32_t imm) noexcept {
        EmitBType(imm, rs2, rs1, 0b110, 0b1100011);
    }

    void BLTZ(GPR rs, uint32_t imm) noexcept {
        BLT(rs, x0, imm);
    }

    void BNE(GPR rs1, GPR rs2, uint32_t imm) noexcept {
        EmitBType(imm, rs2, rs1, 0b001, 0b1100011);
    }

    void BNEZ(GPR rs, uint32_t imm) noexcept {
        BNE(x0, rs, imm);
    }

    void J(uint32_t imm) noexcept {
        JAL(x0, imm);
    }

    void JAL(uint32_t imm) noexcept {
        EmitJType(imm, x1, 0b1101111);
    }

    void JAL(GPR rd, uint32_t imm) noexcept {
        EmitJType(imm, rd, 0b1101111);
    }

    void JALR(GPR rs) noexcept {
        JALR(x1, 0, rs);
    }

    void JALR(GPR rd, uint32_t imm, GPR rs1) noexcept {
        EmitIType(imm, rs1, 0b000, rd, 0b1100111);
    }

    void JR(GPR rs) noexcept {
        JALR(x0, 0, rs);
    }

    void LB(GPR rd, uint32_t imm, GPR rs) noexcept {
        EmitIType(imm, rs, 0b000, rd, 0b0000011);
    }

    void LBU(GPR rd, uint32_t imm, GPR rs) noexcept {
        EmitIType(imm, rs, 0b100, rd, 0b0000011);
    }

    void LH(GPR rd, uint32_t imm, GPR rs) noexcept {
        EmitIType(imm, rs, 0b001, rd, 0b0000011);
    }

    void LHU(GPR rd, uint32_t imm, GPR rs) noexcept {
        EmitIType(imm, rs, 0b101, rd, 0b0000011);
    }

    void LUI(GPR rd, uint32_t imm) noexcept {
        EmitUType(imm, rd, 0b0110111);
    }

    void LW(GPR rd, uint32_t imm, GPR rs) noexcept {
        EmitIType(imm, rs, 0b010, rd, 0b0000011);
    }

    void RET() noexcept {
        JALR(x0, 0, x1);
    }

private:
    // Emits a B type RISC-V instruction. These consist of:
    // imm[12|10:5] | rs2 | rs1 | funct3 | imm[4:1] | imm[11] | opcode
    void EmitBType(uint32_t imm, GPR rs2, GPR rs1, uint32_t funct3, uint32_t opcode) noexcept {
        imm &= 0x1FFE;

        const auto new_imm = ((imm & 0x07E0) << 20) |
                             ((imm & 0x1000) << 19) |
                             ((imm & 0x001E) << 7) |
                             ((imm & 0x0800) >> 4);

        m_buffer.Emit32(new_imm | (rs2.Index() << 20) | (rs1.Index() << 15) | ((funct3 & 0b111) << 12) | (opcode & 0x7F));
    }

    // Emits a I type RISC-V instruction. These consist of:
    // imm[11:0] | rs1 | funct3 | rd | opcode
    void EmitIType(uint32_t imm, GPR rs1, uint32_t funct3, GPR rd, uint32_t opcode) noexcept {
        imm &= 0xFFF;

        m_buffer.Emit32((imm << 20) | (rs1.Index() << 15) | ((funct3 & 0b111) << 12) | (rd.Index() << 7) | (opcode & 0x7F));
    }

    // Emits a J type RISC-V instruction. These consist of:
    // imm[20|10:1|11|19:12] | rd | opcode
    void EmitJType(uint32_t imm, GPR rd, uint32_t opcode) noexcept {
        imm &= 0x1FFFFE;

        // Literally what the heck is this immediate format.
        const auto new_imm = ((imm & 0x0FF000) >> 0) |
                             ((imm & 0x000800) << 9) |
                             ((imm & 0x0007FE) << 20) |
                             ((imm & 0x100000) << 11);

        m_buffer.Emit32(new_imm | rd.Index() << 7 | (opcode & 0x7F));
    }

    // Emits a U type RISC-V instruction. These consist of:
    // imm[31:12] | rd | opcode
    void EmitUType(uint32_t imm, GPR rd, uint32_t opcode) noexcept {
        m_buffer.Emit32((imm & 0xFFFFF000) | rd.Index() << 7 | (opcode & 0x7F));
    }

    CodeBuffer m_buffer;
};

} // namespace biscuit