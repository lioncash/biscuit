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
    // Control and Status Register
    enum class CSR : uint32_t {
        FFlags = 0x001,   // Floating-point Accrued Exceptions
        FRM = 0x002,      // Floating-point Dynamic Rounding Mode
        FCSR = 0x003,     // Floating-point Control and Status Register (frm + fflags)

        Cycle = 0xC00,    // Cycle counter for RDCYCLE instruction.
        Time = 0xC01,     // Timer for RDTIME instruction.
        InstRet = 0xC02,  // Instructions retired counter for RDINSTRET instruction.
        CycleH = 0xC80,   // Upper 32 bits of cycle, RV32I only.
        TimeH = 0xC81,    // Upper 32 bits of time, RV32I only.
        InstRetH = 0xC82, // Upper 32 bits of instret, RV32I only.
    };

    enum class FenceOrder : uint32_t {
        W = 1, // Write
        R = 2, // Read
        O = 4, // Device Output
        I = 8, // Device Input

        RW = R | W,

        IR = I | R,
        IW = I | W,
        IRW = I | R | W,

        OI = O | I,
        OW = O | W,
        ORW = O | R | W,

        IORW = I | O | R | W,
    };

    // Atomic ordering
    enum class Ordering : uint32_t {
        None = 0,       // None
        RL = 1,         // Release
        AQ = 2,         // Acquire
        AQRL = AQ | RL, // Acquire-Release
    };

    // Floating-point Rounding Mode
    enum class RMode : uint32_t {
        RNE = 0b000, // Round to Nearest, ties to Even
        RTZ = 0b001, // Round towards Zero
        RDN = 0b010, // Round Down (towards negative infinity)
        RUP = 0b011, // Round Up (towards positive infinity)
        RMM = 0b100, // Round to Nearest, ties to Max Magnitude
        DYN = 0b111, // Dynamic Rounding Mode
    };

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
    virtual ~Assembler() = default;

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

    void ADD(GPR rd, GPR lhs, GPR rhs) noexcept {
        EmitRType(0b0000000, rhs, lhs, 0b000, rd, 0b0110011);
    }

    void ADDI(GPR rd, GPR rs, uint32_t imm) noexcept {
        EmitIType(imm, rs, 0b000, rd, 0b0010011);
    }

    void AND(GPR rd, GPR lhs, GPR rhs) noexcept {
        EmitRType(0b0000000, rhs, lhs, 0b111, rd, 0b0110011);
    }

    void ANDI(GPR rd, GPR rs, uint32_t imm) noexcept {
        EmitIType(imm, rs, 0b111, rd, 0b0010011);
    }

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

    void EBREAK() noexcept {
        m_buffer.Emit32(0x00100073);
    }

    void ECALL() noexcept {
        m_buffer.Emit32(0x00000073);
    }

    void FENCE() noexcept {
        FENCE(FenceOrder::IORW, FenceOrder::IORW);
    }

    void FENCE(FenceOrder pred, FenceOrder succ) noexcept {
        EmitFENCE(0b0000, pred, succ, x0, 0b000, x0, 0b0001111);
    }

    void FENCEI(GPR rd = x0, GPR rs = x0, uint32_t imm = 0) noexcept {
        m_buffer.Emit32(((imm & 0xFFF) << 20) | (rs.Index() << 15) | 0x1000U | (rd.Index() << 7) | 0b0001111);
    }

    void FENCETSO() noexcept {
        EmitFENCE(0b1000, FenceOrder::RW, FenceOrder::RW, x0, 0b000, x0, 0b0001111);
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

    void MV(GPR rd, GPR rs) noexcept {
        ADDI(rd, rs, 0);
    }

    void NEG(GPR rd, GPR rs) noexcept {
        SUB(rd, x0, rs);
    }

    void NOP() noexcept {
        ADDI(x0, x0, 0);
    }

    void NOT(GPR rd, GPR rs) noexcept {
        XORI(rd, rs, UINT32_MAX);
    }

    void OR(GPR rd, GPR lhs, GPR rhs) noexcept {
        EmitRType(0b0000000, rhs, lhs, 0b110, rd, 0b0110011);
    }

    void ORI(GPR rd, GPR rs, uint32_t imm) noexcept {
        EmitIType(imm, rs, 0b110, rd, 0b0010011);
    }

    void PAUSE() noexcept {
        m_buffer.Emit32(0x0100000F);
    }

    void RET() noexcept {
        JALR(x0, 0, x1);
    }

    void SB(GPR rs2, uint32_t imm, GPR rs1) noexcept {
        EmitSType(imm, rs2, rs1, 0b000, 0b0100011);
    }

    void SEQZ(GPR rd, GPR rs) noexcept {
        SLTIU(rd, rs, 1);
    }

    void SGTZ(GPR rd, GPR rs) noexcept {
        SLT(rd, x0, rs);
    }

    void SH(GPR rs2, uint32_t imm, GPR rs1) noexcept {
        EmitSType(imm, rs2, rs1, 0b001, 0b0100011);
    }

    void SLL(GPR rd, GPR lhs, GPR rhs) noexcept {
        EmitRType(0b0000000, rhs, lhs, 0b001, rd, 0b0110011);
    }

    void SLLI(GPR rd, GPR rs, uint32_t shift) noexcept {
        EmitIType(shift & 0x1F, rs, 0b001, rd, 0b0010011);
    }

    void SLT(GPR rd, GPR lhs, GPR rhs) noexcept {
        EmitRType(0b0000000, rhs, lhs, 0b010, rd, 0b0110011);
    }

    void SLTI(GPR rd, GPR rs, uint32_t imm) noexcept {
        EmitIType(imm, rs, 0b010, rd, 0b0010011);
    }

    void SLTIU(GPR rd, GPR rs, uint32_t imm) noexcept {
        EmitIType(imm, rs, 0b011, rd, 0b0010011);
    }

    void SLTU(GPR rd, GPR lhs, GPR rhs) noexcept {
        EmitRType(0b0000000, rhs, lhs, 0b011, rd, 0b0110011);
    }

    void SLTZ(GPR rd, GPR rs) noexcept {
        SLT(rd, rs, x0);
    }

    void SNEZ(GPR rd, GPR rs) noexcept {
        SLTU(rd, x0, rs);
    }

    void SRA(GPR rd, GPR lhs, GPR rhs) noexcept {
        EmitRType(0b0100000, rhs, lhs, 0b101, rd, 0b0110011);
    }

    void SRAI(GPR rd, GPR rs, uint32_t shift) noexcept {
        EmitIType((0b0100000 << 5) | (shift & 0x1F), rs, 0b101, rd, 0b0010011);
    }

    void SRL(GPR rd, GPR lhs, GPR rhs) noexcept {
        EmitRType(0b0000000, rhs, lhs, 0b101, rd, 0b0110011);
    }

    void SRLI(GPR rd, GPR rs, uint32_t shift) noexcept {
        EmitIType(shift & 0x1F, rs, 0b101, rd, 0b0010011);
    }

    void SUB(GPR rd, GPR lhs, GPR rhs) noexcept {
        EmitRType(0b0100000, rhs, lhs, 0b000, rd, 0b0110011);
    }

    void SW(GPR rs2, uint32_t imm, GPR rs1) noexcept {
        EmitSType(imm, rs2, rs1, 0b010, 0b0100011);
    }

    void XOR(GPR rd, GPR lhs, GPR rhs) noexcept {
        EmitRType(0b0000000, rhs, lhs, 0b100, rd, 0b0110011);
    }

    void XORI(GPR rd, GPR rs, uint32_t imm) noexcept {
        EmitIType(imm, rs, 0b100, rd, 0b0010011);
    }

    // RV64I Base Instruction Set

    void ADDIW(GPR rd, GPR rs, uint32_t imm) noexcept {
        EmitIType(imm, rs, 0b000, rd, 0b0011011);
    }

    void ADDW(GPR rd, GPR lhs, GPR rhs) noexcept {
        EmitRType(0b0000000, rhs, lhs, 0b000, rd, 0b0111011);
    }

    void LD(GPR rd, uint32_t imm, GPR rs) noexcept {
        EmitIType(imm, rs, 0b011, rd, 0b0000011);
    }

    void LWU(GPR rd, GPR rs, uint32_t imm) noexcept {
        EmitIType(imm, rs, 0b110, rd, 0b0000011);
    }

    void SD(GPR rs2, uint32_t imm, GPR rs1) noexcept {
        EmitSType(imm, rs2, rs1, 0b011, 0b0100011);
    }

    // NOTE: Perhaps we should coalesce this into the 32-bit variant?
    //       Keeping them separated would allow asserts for catching
    //       out of range shifts in the future.
    void SRAI64(GPR rd, GPR rs, uint32_t shift) noexcept {
        EmitIType((0b0100000 << 5) | (shift & 0x3F), rs, 0b101, rd, 0b0010011);
    }
    void SLLI64(GPR rd, GPR rs, uint32_t shift) noexcept {
        EmitIType(shift & 0x3F, rs, 0b001, rd, 0b0010011);
    }
    void SRLI64(GPR rd, GPR rs, uint32_t shift) noexcept {
        EmitIType(shift & 0x3F, rs, 0b101, rd, 0b0010011);
    }

    void SLLIW(GPR rd, GPR rs, uint32_t shift) noexcept {
        EmitIType(shift & 0x1F, rs, 0b001, rd, 0b0011011);
    }
    void SRAIW(GPR rd, GPR rs, uint32_t shift) noexcept {
        EmitIType((0b0100000 << 5) | (shift & 0x1F), rs, 0b101, rd, 0b0011011);
    }
    void SRLIW(GPR rd, GPR rs, uint32_t shift) noexcept {
        EmitIType(shift & 0x1F, rs, 0b101, rd, 0b0011011);
    }

    void SLLW(GPR rd, GPR lhs, GPR rhs) noexcept {
        EmitRType(0b0000000, rhs, lhs, 0b001, rd, 0b0111011);
    }
    void SRAW(GPR rd, GPR lhs, GPR rhs) noexcept {
        EmitRType(0b0100000, rhs, lhs, 0b101, rd, 0b0111011);
    }
    void SRLW(GPR rd, GPR lhs, GPR rhs) noexcept {
        EmitRType(0b0000000, rhs, lhs, 0b101, rd, 0b0111011);
    }

    void SUBW(GPR rd, GPR lhs, GPR rhs) noexcept {
        EmitRType(0b0100000, rhs, lhs, 0b000, rd, 0b0111011);
    }

    // Zicsr Extension Instructions

    void CSRRC(GPR rd, CSR csr, GPR rs) noexcept {
        EmitIType(static_cast<uint32_t>(csr), rs, 0b011, rd, 0b1110011);
    }
    void CSRRCI(GPR rd, CSR csr, uint32_t imm) noexcept {
        EmitIType(static_cast<uint32_t>(csr), GPR{imm & 0x1F}, 0b111, rd, 0b1110011);
    }
    void CSRRS(GPR rd, CSR csr, GPR rs) noexcept {
        EmitIType(static_cast<uint32_t>(csr), rs, 0b010, rd, 0b1110011);
    }
    void CSRRSI(GPR rd, CSR csr, uint32_t imm) noexcept {
        EmitIType(static_cast<uint32_t>(csr), GPR{imm & 0x1F}, 0b110, rd, 0b1110011);
    }
    void CSRRW(GPR rd, CSR csr, GPR rs) noexcept {
        EmitIType(static_cast<uint32_t>(csr), rs, 0b001, rd, 0b1110011);
    }
    void CSRRWI(GPR rd, CSR csr, uint32_t imm) noexcept {
        EmitIType(static_cast<uint32_t>(csr), GPR{imm & 0x1F}, 0b101, rd, 0b1110011);
    }

    void CSRR(GPR rd, CSR csr) noexcept {
        CSRRS(rd, csr, x0);
    }
    void CSWR(CSR csr, GPR rs) noexcept {
        CSRRW(x0, csr, rs);
    }

    void CSRS(CSR csr, GPR rs) noexcept {
        CSRRS(x0, csr, rs);
    }
    void CSRC(CSR csr, GPR rs) noexcept {
        CSRRC(x0, csr, rs);
    }

    void CSRCI(CSR csr, uint32_t imm) noexcept {
        CSRRCI(x0, csr, imm);
    }
    void CSRSI(CSR csr, uint32_t imm) noexcept {
        CSRRSI(x0, csr, imm);
    }
    void CSRWI(CSR csr, uint32_t imm) noexcept {
        CSRRWI(x0, csr, imm);
    }

    void FRCSR(GPR rd) noexcept {
        CSRRS(rd, CSR::FCSR, x0);
    }
    void FSCSR(GPR rd, GPR rs) noexcept {
        CSRRW(rd, CSR::FCSR, rs);
    }
    void FSCSR(GPR rs) noexcept {
        CSRRW(x0, CSR::FCSR, rs);
    }

    void FRRM(GPR rd) noexcept {
        CSRRS(rd, CSR::FRM, x0);
    }
    void FSRM(GPR rd, GPR rs) noexcept {
        CSRRW(rd, CSR::FRM, rs);
    }
    void FSRM(GPR rs) noexcept {
        CSRRW(x0, CSR::FRM, rs);
    }

    void FSRMI(GPR rd, uint32_t imm) noexcept {
        CSRRWI(rd, CSR::FRM, imm);
    }
    void FSRMI(uint32_t imm) noexcept {
        CSRRWI(x0, CSR::FRM, imm);
    }

    void FRFLAGS(GPR rd) noexcept {
        CSRRS(rd, CSR::FFlags, x0);
    }
    void FSFLAGS(GPR rd, GPR rs) noexcept {
        CSRRW(rd, CSR::FFlags, rs);
    }
    void FSFLAGS(GPR rs) noexcept {
        CSRRW(x0, CSR::FFlags, rs);
    }

    void FSFLAGSI(GPR rd, uint32_t imm) noexcept {
        CSRRWI(rd, CSR::FFlags, imm);
    }
    void FSFLAGSI(uint32_t imm) noexcept {
        CSRRWI(x0, CSR::FFlags, imm);
    }

    void RDCYCLE(GPR rd) noexcept {
        CSRRS(rd, CSR::Cycle, x0);
    }
    void RDCYCLEH(GPR rd) noexcept {
        CSRRS(rd, CSR::CycleH, x0);
    }

    void RDINSTRET(GPR rd) noexcept {
        CSRRS(rd, CSR::InstRet, x0);
    }
    void RDINSTRETH(GPR rd) noexcept {
        CSRRS(rd, CSR::InstRetH, x0);
    }

    void RDTIME(GPR rd) noexcept {
        CSRRS(rd, CSR::Time, x0);
    }
    void RDTIMEH(GPR rd) noexcept {
        CSRRS(rd, CSR::TimeH, x0);
    }

    // RV32M Extension Instructions

    void DIV(GPR rd, GPR rs1, GPR rs2) noexcept {
        EmitRType(0b0000001, rs2, rs1, 0b100, rd, 0b0110011);
    }
    void DIVU(GPR rd, GPR rs1, GPR rs2) noexcept {
        EmitRType(0b0000001, rs2, rs1, 0b101, rd, 0b0110011);
    }
    void MUL(GPR rd, GPR rs1, GPR rs2) noexcept {
        EmitRType(0b0000001, rs2, rs1, 0b000, rd, 0b0110011);
    }
    void MULH(GPR rd, GPR rs1, GPR rs2) noexcept {
        EmitRType(0b0000001, rs2, rs1, 0b001, rd, 0b0110011);
    }
    void MULHSU(GPR rd, GPR rs1, GPR rs2) noexcept {
        EmitRType(0b0000001, rs2, rs1, 0b010, rd, 0b0110011);
    }
    void MULHU(GPR rd, GPR rs1, GPR rs2) noexcept {
        EmitRType(0b0000001, rs2, rs1, 0b011, rd, 0b0110011);
    }
    void REM(GPR rd, GPR rs1, GPR rs2) noexcept {
        EmitRType(0b0000001, rs2, rs1, 0b110, rd, 0b0110011);
    }
    void REMU(GPR rd, GPR rs1, GPR rs2) noexcept {
        EmitRType(0b0000001, rs2, rs1, 0b111, rd, 0b0110011);
    }

    // RV64M Extension Instructions

    void DIVW(GPR rd, GPR rs1, GPR rs2) noexcept {
        EmitRType(0b0000001, rs2, rs1, 0b100, rd, 0b0111011);
    }
    void DIVUW(GPR rd, GPR rs1, GPR rs2) noexcept {
        EmitRType(0b0000001, rs2, rs1, 0b101, rd, 0b0111011);
    }
    void MULW(GPR rd, GPR rs1, GPR rs2) noexcept {
        EmitRType(0b0000001, rs2, rs1, 0b000, rd, 0b0111011);
    }
    void REMW(GPR rd, GPR rs1, GPR rs2) noexcept {
        EmitRType(0b0000001, rs2, rs1, 0b110, rd, 0b0111011);
    }
    void REMUW(GPR rd, GPR rs1, GPR rs2) noexcept {
        EmitRType(0b0000001, rs2, rs1, 0b111, rd, 0b0111011);
    }

    // RV32A Extension Instructions

    void AMOADD_W(Ordering ordering, GPR rd, GPR rs2, GPR rs1) noexcept {
        EmitAtomic(0b00000, ordering, rs2, rs1, 0b010, rd, 0b0101111);
    }
    void AMOAND_W(Ordering ordering, GPR rd, GPR rs2, GPR rs1) noexcept {
        EmitAtomic(0b01100, ordering, rs2, rs1, 0b010, rd, 0b0101111);
    }
    void AMOMAX_W(Ordering ordering, GPR rd, GPR rs2, GPR rs1) noexcept {
        EmitAtomic(0b10100, ordering, rs2, rs1, 0b010, rd, 0b0101111);
    }
    void AMOMAXU_W(Ordering ordering, GPR rd, GPR rs2, GPR rs1) noexcept {
        EmitAtomic(0b11100, ordering, rs2, rs1, 0b010, rd, 0b0101111);
    }
    void AMOMIN_W(Ordering ordering, GPR rd, GPR rs2, GPR rs1) noexcept {
        EmitAtomic(0b10000, ordering, rs2, rs1, 0b010, rd, 0b0101111);
    }
    void AMOMINU_W(Ordering ordering, GPR rd, GPR rs2, GPR rs1) noexcept {
        EmitAtomic(0b11000, ordering, rs2, rs1, 0b010, rd, 0b0101111);
    }
    void AMOOR_W(Ordering ordering, GPR rd, GPR rs2, GPR rs1) noexcept {
        EmitAtomic(0b01000, ordering, rs2, rs1, 0b010, rd, 0b0101111);
    }
    void AMOSWAP_W(Ordering ordering, GPR rd, GPR rs2, GPR rs1) noexcept {
        EmitAtomic(0b00001, ordering, rs2, rs1, 0b010, rd, 0b0101111);
    }
    void AMOXOR_W(Ordering ordering, GPR rd, GPR rs2, GPR rs1) noexcept {
        EmitAtomic(0b00100, ordering, rs2, rs1, 0b010, rd, 0b0101111);
    }
    void LR_W(Ordering ordering, GPR rd, GPR rs) noexcept {
        EmitAtomic(0b00010, ordering, x0, rs, 0b010, rd, 0b0101111);
    }
    void SC_W(Ordering ordering, GPR rd, GPR rs1, GPR rs2) noexcept {
        EmitAtomic(0b00011, ordering, rs2, rs1, 0b010, rd, 0b0101111);
    }

    // RV64A Extension Instructions

    void AMOADD_D(Ordering ordering, GPR rd, GPR rs2, GPR rs1) noexcept {
        EmitAtomic(0b00000, ordering, rs2, rs1, 0b011, rd, 0b0101111);
    }
    void AMOAND_D(Ordering ordering, GPR rd, GPR rs2, GPR rs1) noexcept {
        EmitAtomic(0b01100, ordering, rs2, rs1, 0b011, rd, 0b0101111);
    }
    void AMOMAX_D(Ordering ordering, GPR rd, GPR rs2, GPR rs1) noexcept {
        EmitAtomic(0b10100, ordering, rs2, rs1, 0b011, rd, 0b0101111);
    }
    void AMOMAXU_D(Ordering ordering, GPR rd, GPR rs2, GPR rs1) noexcept {
        EmitAtomic(0b11100, ordering, rs2, rs1, 0b011, rd, 0b0101111);
    }
    void AMOMIN_D(Ordering ordering, GPR rd, GPR rs2, GPR rs1) noexcept {
        EmitAtomic(0b10000, ordering, rs2, rs1, 0b011, rd, 0b0101111);
    }
    void AMOMINU_D(Ordering ordering, GPR rd, GPR rs2, GPR rs1) noexcept {
        EmitAtomic(0b11000, ordering, rs2, rs1, 0b011, rd, 0b0101111);
    }
    void AMOOR_D(Ordering ordering, GPR rd, GPR rs2, GPR rs1) noexcept {
        EmitAtomic(0b01000, ordering, rs2, rs1, 0b011, rd, 0b0101111);
    }
    void AMOSWAP_D(Ordering ordering, GPR rd, GPR rs2, GPR rs1) noexcept {
        EmitAtomic(0b00001, ordering, rs2, rs1, 0b011, rd, 0b0101111);
    }
    void AMOXOR_D(Ordering ordering, GPR rd, GPR rs2, GPR rs1) noexcept {
        EmitAtomic(0b00100, ordering, rs2, rs1, 0b011, rd, 0b0101111);
    }
    void LR_D(Ordering ordering, GPR rd, GPR rs) noexcept {
        EmitAtomic(0b00010, ordering, x0, rs, 0b011, rd, 0b0101111);
    }
    void SC_D(Ordering ordering, GPR rd, GPR rs1, GPR rs2) noexcept {
        EmitAtomic(0b00011, ordering, rs2, rs1, 0b011, rd, 0b0101111);
    }

    // RV32F Extension Instructions

    void FADD_S(FPR rd, FPR rs1, FPR rs2, RMode rmode = RMode::DYN) noexcept {
        EmitRType(0b0000000, rs2, rs1, rmode, rd, 0b1010011);
    }
    void FCLASS_S(GPR rd, FPR rs1) noexcept {
        EmitRType(0b1110000, f0, rs1, 0b0001, rd, 0b1010011);
    }
    void FCVT_S_W(FPR rd, GPR rs1, RMode rmode = RMode::DYN) noexcept {
        EmitRType(0b1101000, f0, rs1, static_cast<uint32_t>(rmode), rd, 0b1010011);
    }
    void FCVT_S_WU(FPR rd, GPR rs1, RMode rmode = RMode::DYN) noexcept {
        EmitRType(0b1101000, f1, rs1, static_cast<uint32_t>(rmode), rd, 0b1010011);
    }
    void FCVT_W_S(GPR rd, FPR rs1, RMode rmode = RMode::DYN) noexcept {
        EmitRType(0b1100000, f0, rs1, static_cast<uint32_t>(rmode), rd, 0b1010011);
    }
    void FCVT_WU_S(GPR rd, FPR rs1, RMode rmode = RMode::DYN) noexcept {
        EmitRType(0b1100000, f1, rs1, static_cast<uint32_t>(rmode), rd, 0b1010011);
    }
    void FDIV_S(FPR rd, FPR rs1, FPR rs2, RMode rmode = RMode::DYN) noexcept {
        EmitRType(0b0001100, rs2, rs1, rmode, rd, 0b1010011);
    }
    void FEQ_S(GPR rd, FPR rs1, FPR rs2) noexcept {
        EmitRType(0b1010000, rs2, rs1, 0b010, rd, 0b1010011);
    }
    void FLE_S(GPR rd, FPR rs1, FPR rs2) noexcept {
        EmitRType(0b1010000, rs2, rs1, 0b000, rd, 0b1010011);
    }
    void FLT_S(GPR rd, FPR rs1, FPR rs2) noexcept {
        EmitRType(0b1010000, rs2, rs1, 0b001, rd, 0b1010011);
    }
    void FLW(FPR rd, uint32_t offset, GPR rs) noexcept {
        EmitIType(offset, rs, 0b010, rd, 0b0000111);
    }
    void FMADD_S(FPR rd, FPR rs1, FPR rs2, FPR rs3, RMode rmode = RMode::DYN) noexcept {
        EmitR4Type(rs3, 0b00, rs2, rs1, rmode, rd, 0b1000011);
    }
    void FMAX_S(FPR rd, FPR rs1, FPR rs2) noexcept {
        EmitRType(0b0010100, rs2, rs1, 0b001, rd, 0b1010011);
    }
    void FMIN_S(FPR rd, FPR rs1, FPR rs2) noexcept {
        EmitRType(0b0010100, rs2, rs1, 0b000, rd, 0b1010011);
    }
    void FMSUB_S(FPR rd, FPR rs1, FPR rs2, FPR rs3, RMode rmode = RMode::DYN) noexcept {
        EmitR4Type(rs3, 0b00, rs2, rs1, rmode, rd, 0b1000111);
    }
    void FMUL_S(FPR rd, FPR rs1, FPR rs2, RMode rmode = RMode::DYN) noexcept {
        EmitRType(0b0001000, rs2, rs1, rmode, rd, 0b1010011);
    }
    void FMV_W_X(FPR rd, GPR rs1) noexcept {
        EmitRType(0b1111000, f0, rs1, 0b000, rd, 0b1010011);
    }
    void FMV_X_W(GPR rd, FPR rs1) noexcept {
        EmitRType(0b1110000, f0, rs1, 0b000, rd, 0b1010011);
    }
    void FNMADD_S(FPR rd, FPR rs1, FPR rs2, FPR rs3, RMode rmode = RMode::DYN) noexcept {
        EmitR4Type(rs3, 0b00, rs2, rs1, rmode, rd, 0b1001111);
    }
    void FNMSUB_S(FPR rd, FPR rs1, FPR rs2, FPR rs3, RMode rmode = RMode::DYN) noexcept {
        EmitR4Type(rs3, 0b00, rs2, rs1, rmode, rd, 0b1001011);
    }
    void FSGNJ_S(FPR rd, FPR rs1, FPR rs2) noexcept {
        EmitRType(0b0010000, rs2, rs1, 0b000, rd, 0b1010011);
    }
    void FSGNJN_S(FPR rd, FPR rs1, FPR rs2) noexcept {
        EmitRType(0b0010000, rs2, rs1, 0b001, rd, 0b1010011);
    }
    void FSGNJX_S(FPR rd, FPR rs1, FPR rs2) noexcept {
        EmitRType(0b0010000, rs2, rs1, 0b010, rd, 0b1010011);
    }
    void FSQRT_S(FPR rd, FPR rs1, RMode rmode = RMode::DYN) noexcept {
        EmitRType(0b0101100, f0, rs1, rmode, rd, 0b1010011);
    }
    void FSUB_S(FPR rd, FPR rs1, FPR rs2, RMode rmode = RMode::DYN) noexcept {
        EmitRType(0b0000100, rs2, rs1, rmode, rd, 0b1010011);
    }
    void FSW(FPR rs2, uint32_t offset, GPR rs1) noexcept {
        EmitSType(offset, rs2, rs1, 0b010, 0b0100111);
    }

    // RV64F Extension Instructions

    void FCVT_L_S(GPR rd, FPR rs1, RMode rmode = RMode::DYN) noexcept {
        EmitRType(0b1100000, f2, rs1, static_cast<uint32_t>(rmode), rd, 0b1010011);
    }

private:
    void EmitAtomic(uint32_t funct5, Ordering ordering, GPR rs2, GPR rs1, uint32_t funct3, GPR rd, uint32_t opcode) noexcept {
        const auto funct7 = (funct5 << 2) | static_cast<uint32_t>(ordering);
        EmitRType(funct7, rs2, rs1, funct3, rd, opcode);
    }

    // Emits a B type RISC-V instruction. These consist of:
    // imm[12|10:5] | rs2 | rs1 | funct3 | imm[4:1] | imm[11] | opcode
    void EmitBType(uint32_t imm, GPR rs2, GPR rs1, uint32_t funct3, uint32_t opcode) noexcept {
        imm &= 0x1FFE;

        // clang-format off
        const auto new_imm = ((imm & 0x07E0) << 20) |
                             ((imm & 0x1000) << 19) |
                             ((imm & 0x001E) << 7) |
                             ((imm & 0x0800) >> 4);
        // clang-format on

        m_buffer.Emit32(new_imm | (rs2.Index() << 20) | (rs1.Index() << 15) | ((funct3 & 0b111) << 12) | (opcode & 0x7F));
    }

    // Emits a I type RISC-V instruction. These consist of:
    // imm[11:0] | rs1 | funct3 | rd | opcode
    void EmitIType(uint32_t imm, Register rs1, uint32_t funct3, Register rd, uint32_t opcode) noexcept {
        imm &= 0xFFF;

        m_buffer.Emit32((imm << 20) | (rs1.Index() << 15) | ((funct3 & 0b111) << 12) | (rd.Index() << 7) | (opcode & 0x7F));
    }

    // Emits a J type RISC-V instruction. These consist of:
    // imm[20|10:1|11|19:12] | rd | opcode
    void EmitJType(uint32_t imm, GPR rd, uint32_t opcode) noexcept {
        imm &= 0x1FFFFE;

        // clang-format off
        const auto new_imm = ((imm & 0x0FF000) >> 0) |
                             ((imm & 0x000800) << 9) |
                             ((imm & 0x0007FE) << 20) |
                             ((imm & 0x100000) << 11);
        // clang-format on

        m_buffer.Emit32(new_imm | rd.Index() << 7 | (opcode & 0x7F));
    }

    // Emits a R type RISC instruction. These consist of:
    // funct7 | rs2 | rs1 | funct3 | rd | opcode
    void EmitRType(uint32_t funct7, Register rs2, Register rs1, uint32_t funct3, Register rd, uint32_t opcode) noexcept {
        m_buffer.Emit32(((funct7 & 0xFF) << 25) | (rs2.Index() << 20) | (rs1.Index() << 15) |
                             ((funct3 & 0b111) << 12) | (rd.Index() << 7) | (opcode & 0x7F));
    }

    // Emits a R type RISC instruction. These consist of:
    // funct7 | rs2 | rs1 | funct3 | rd | opcode
    void EmitRType(uint32_t funct7, FPR rs2, FPR rs1, RMode funct3, FPR rd, uint32_t opcode) noexcept {
        m_buffer.Emit32(((funct7 & 0xFF) << 25) | (rs2.Index() << 20) | (rs1.Index() << 15) |
                        (static_cast<uint32_t>(funct3) << 12) | (rd.Index() << 7) | (opcode & 0x7F));
    }

    // Emits a R4 type RISC instruction. These consist of:
    // rs3 | funct2 | rs2 | rs1 | funct3 | rd | opcode
    void EmitR4Type(FPR rs3, uint32_t funct2, FPR rs2, FPR rs1, RMode funct3, FPR rd, uint32_t opcode) noexcept {
        const auto reg_bits = (rs3.Index() << 27) | (rs2.Index() << 20) | (rs1.Index() << 15) | (rd.Index() << 7);
        const auto funct_bits = ((funct2 & 0b11) << 25) | (static_cast<uint32_t>(funct3) << 12);
        m_buffer.Emit32(reg_bits | funct_bits | (opcode & 0x7F));
    }

    // Emits a S type RISC-V instruction. These consist of:
    // imm[11:5] | rs2 | rs1 | funct3 | imm[4:0] | opcode
    void EmitSType(uint32_t imm, Register rs2, GPR rs1, uint32_t funct3, uint32_t opcode) noexcept {
        imm &= 0xFFF;

        // clang-format off
        const auto new_imm = ((imm & 0x01F) << 7) |
                             ((imm & 0xFE0) << 20);
        // clang-format on

        m_buffer.Emit32(new_imm | (rs2.Index() << 20) | (rs1.Index() << 15) | ((funct3 & 0b111) << 12) | (opcode & 0x7F));
    }

    // Emits a U type RISC-V instruction. These consist of:
    // imm[31:12] | rd | opcode
    void EmitUType(uint32_t imm, GPR rd, uint32_t opcode) noexcept {
        m_buffer.Emit32((imm & 0xFFFFF000) | rd.Index() << 7 | (opcode & 0x7F));
    }

    // Emits a fence instruction
    void EmitFENCE(uint32_t fm, FenceOrder pred, FenceOrder succ, GPR rs, uint32_t funct3, GPR rd, uint32_t opcode) noexcept {
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

    CodeBuffer m_buffer;
};

} // namespace biscuit