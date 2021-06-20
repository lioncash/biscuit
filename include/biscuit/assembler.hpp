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

    void AUIPC(uint32_t imm, GPR rd) noexcept {
        EmitUType(imm, rd, 0b0010111);
    }

    void J(uint32_t imm) noexcept {
        JAL(imm, x0);
    }

    void JAL(uint32_t imm, GPR rd) noexcept {
        EmitJType(imm, rd, 0b1101111);
    }

    void LUI(uint32_t imm, GPR rd) noexcept {
        EmitUType(imm, rd, 0b0110111);
    }

private:
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