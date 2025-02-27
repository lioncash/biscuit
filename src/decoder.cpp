#include <biscuit/decoder.hpp>
#include <cstring>

namespace {

using namespace biscuit;

struct Information {
    uint32_t data;
    ArchFeature host_arch;
    Ordering ordering = Ordering::None;
    RMode rmode = RMode::DYN; // also used for "no RMode"
    VecMask masked = VecMask::No;
};

uint32_t Rd(uint32_t instruction) {
    return (instruction >> 7) & 0x1F;
}

uint32_t Rs1(uint32_t instruction) {
    return (instruction >> 15) & 0x1F;
}

uint32_t Rs2(uint32_t instruction) {
    return (instruction >> 20) & 0x1F;
}

uint32_t Funct7(uint32_t instruction) {
    return (instruction >> 25) & 0x7F;
}

uint32_t Funct3(uint32_t instruction) {
    return (instruction >> 12) & 0x7;
}

int32_t JImm(uint32_t instruction) {
    uint32_t imm = 0;

    imm |= (instruction & 0x80000000) >> 11;  // Bit  31    -> Bit 20
    imm |= (instruction & 0x7FE00000) >> 20;  // Bits 30-21 -> Bits 10:1
    imm |= (instruction & 0x00100000) >> 9;   // Bit  20    -> Bit 11
    imm |= (instruction & 0x000FF000);        // Bits 19-12 -> Bits 19:12

    if (imm & 0x00100000) {
        imm |= 0xFFE00000;
    }

    return static_cast<int32_t>(imm);
}

int32_t BImm(uint32_t instruction) {
    uint32_t imm = 0;

    imm |= (instruction & 0x80000000) >> 19;  // Bit 31     -> Bit 12
    imm |= (instruction & 0x7E000000) >> 20;  // Bits 30-25 -> Bits 10:5
    imm |= (instruction & 0x00000F00) >> 7;   // Bits 11-8  -> Bits 4:1
    imm |= (instruction & 0x00000080) << 4;   // Bit 7      -> Bit 11

    if (imm & 0x00001000) {
        imm |= 0xFFFFE000;
    }

    return static_cast<int32_t>(imm);
}

int32_t IImm(uint32_t instruction) {
    return static_cast<int32_t>(instruction) >> 20;
}

int32_t UImm(uint32_t instruction) {
    // Don't sign extend
    return static_cast<int32_t>(instruction >> 12);
}

int32_t SImm(uint32_t instruction) {
    uint32_t imm = 0;

    imm |= (instruction & 0xFE000000) >> 20;  // Bits 31-25 -> Bits 11:5
    imm |= (instruction & 0x00000F80) >> 7;   // Bits 11-7  -> Bits 4:0

    if (imm & 0x00000800) { 
        imm |= 0xFFFFF000;
    }

    return static_cast<int32_t>(imm);
}

int32_t ShiftImm(uint32_t instruction) {
    return static_cast<int32_t>((instruction >> 20) & 0x3F);
}

uint32_t ArchToWidth(ArchFeature feature) {
    switch (feature) {
        case ArchFeature::RV32:
            return 32;
        case ArchFeature::RV64:
            return 64;
        case ArchFeature::RV128:
            return 128;
    }

    BISCUIT_ASSERT(false);
}

void SetAqrl(Information& info, bool aq, bool rl) {
    Ordering ordering;
    if (aq && rl) {
        ordering = Ordering::AQRL;
    } else if (aq) {
        ordering = Ordering::AQ;
    } else if (rl) {
        ordering = Ordering::RL;
    } else {
        ordering = Ordering::None;
    }

    info.ordering = ordering;
}

constexpr uint32_t GetInstructionLength(Extension extension) {
    switch (extension) {
        // TODO: add others
        case Extension::C: {
            return 2;
        }
        default: {
            return 4;
        }
    }
}

template<class Type_, uint32_t(*Func)(uint32_t), size_t Size_, bool Write_>
struct Register {
    constexpr static bool IsNone() {
        return Func == nullptr;
    }

    constexpr static Type_ Create(uint32_t instruction) {
        return Type_(Func(instruction));
    }

    constexpr static size_t Size = Size_;

    constexpr static bool Write = Write_;
};

using NoRegister = Register<bool, nullptr, 0, false>;

// This type is for when the width is not fixed but instead depends
// on the host architecture. For example for the ADD instruction which has
// 32-bit operands in 32-bit mode and 64-bit operands in 64-bit mode.
template<uint32_t(*Func)(uint32_t), bool Write>
using GPR0 = Register<GPR, Func, 0, Write>;

template<uint32_t(*Func)(uint32_t), bool Write = false>
using GPRAddress = GPR0<Func, Write>;

template<uint32_t(*Func)(uint32_t), bool Write>
using GPR8 = Register<GPR, Func, 8, Write>;

template<uint32_t(*Func)(uint32_t), bool Write>
using GPR16 = Register<GPR, Func, 16, Write>;

template<uint32_t(*Func)(uint32_t), bool Write>
using GPR32 = Register<GPR, Func, 32, Write>;

template<uint32_t(*Func)(uint32_t), bool Write>
using GPR64 = Register<GPR, Func, 64, Write>;

template<uint32_t(*Func)(uint32_t), bool Write>
using FPR32 = Register<FPR, Func, 32, Write>;

template<uint32_t(*Func)(uint32_t), bool Write>
using FPR64 = Register<FPR, Func, 64, Write>;

// Func is a function that extracts the immediate from the instruction
template<int32_t(*Func)(uint32_t)>
struct Immediate {
    constexpr static bool IsNone() {
        return Func == nullptr;
    }

    constexpr static int32_t Create(uint32_t instruction) {
        return Func(instruction);
    }

    constexpr static bool IsRegister = false;
};

using NoImmediate = Immediate<nullptr>;
using SImmediate = Immediate<SImm>;
using BImmediate = Immediate<BImm>;
using IImmediate = Immediate<IImm>;
using JImmediate = Immediate<JImm>;
using UImmediate = Immediate<UImm>;
using ShiftImmediate = Immediate<ShiftImm>;

template<class RegType>
constexpr DecodedOperand DecodeReg(uint32_t instruction, uint32_t host_size) {
    if (RegType::IsNone()) {
        BISCUIT_ASSERT(false);
    }

    // Construct an actual GPR/FPR/Vec from the instruction
    auto biscuit_reg = RegType::Create(instruction);
    return DecodedOperand(biscuit_reg, RegType::Size == 0 ? host_size : RegType::Size, RegType::Write);
}

template<Mnemonic mnemonic, Extension extension, ArchFeature arch, class RegType1 = NoRegister, class RegType2 = NoRegister, class RegType3 = NoRegister, class RegType4 = NoRegister, class ImmType = NoImmediate>
constexpr DecoderStatus DecodeGeneric(DecodedInstruction& instruction, DecodedOperand* operands, const Information& information) {
    instruction.mnemonic = mnemonic;
    instruction.extension = extension;
    instruction.arch = arch;
    instruction.length = GetInstructionLength(extension);
    instruction.masked = information.masked;
    instruction.rmode = information.rmode;
    instruction.ordering = information.ordering;
    memcpy(instruction.raw, &information.data, instruction.length);

    uint8_t count = 0;
    if constexpr (!RegType1::IsNone()) {
        count++;
        if constexpr (!RegType2::IsNone()) {
            count++;
            if constexpr (!RegType3::IsNone()) {
                count++;
                if constexpr (!RegType4::IsNone()) {
                    count++;
                }
            }
        }
    }

    if constexpr (!ImmType::IsNone()) {
        count++;
    }

    BISCUIT_ASSERT(count <= 4);
    instruction.operand_count = count;

    uint8_t index = 0;
    if (operands) {
        uint32_t host_size = ArchToWidth(information.host_arch);

        if constexpr (!RegType1::IsNone()) {
            operands[index++] = DecodeReg<RegType1>(information.data, host_size);
        }

        if constexpr (!RegType2::IsNone()) {
            operands[index++] = DecodeReg<RegType2>(information.data, host_size);
        }

        if constexpr (!RegType3::IsNone()) {
            operands[index++] = DecodeReg<RegType3>(information.data, host_size);
        }

        if constexpr (!RegType4::IsNone()) {
            operands[index++] = DecodeReg<RegType4>(information.data, host_size);
        }

        if constexpr (!ImmType::IsNone()) {
            BISCUIT_ASSERT(index < 4);
            operands[index++] = DecodedOperand(ImmType::Create(information.data));
        }
    }

    return DecoderStatus::Ok;
}

// Overloads with different register counts
template<Mnemonic mnemonic, Extension extension, ArchFeature arch, class RegType1, class RegType2, class RegType3, class ImmType>
constexpr DecoderStatus DecodeGeneric3(DecodedInstruction& instruction, DecodedOperand* operands, const Information& information) {
    return DecodeGeneric<mnemonic, extension, arch, RegType1, RegType2, RegType3, NoRegister, ImmType>(instruction, operands, information);
}

template<Mnemonic mnemonic, Extension extension, ArchFeature arch, class RegType1, class RegType2, class ImmType>
constexpr DecoderStatus DecodeGeneric2(DecodedInstruction& instruction, DecodedOperand* operands, const Information& information) {
    return DecodeGeneric<mnemonic, extension, arch, RegType1, RegType2, NoRegister, NoRegister, ImmType>(instruction, operands, information);
}

template<Mnemonic mnemonic, Extension extension, ArchFeature arch, class RegType1, class ImmType>
constexpr DecoderStatus DecodeGeneric1(DecodedInstruction& instruction, DecodedOperand* operands, const Information& information) {
    return DecodeGeneric<mnemonic, extension, arch, RegType1, NoRegister, NoRegister, NoRegister, ImmType>(instruction, operands, information);
}

template<Mnemonic mnemonic, Extension extension, ArchFeature arch, class ImmType>
constexpr DecoderStatus DecodeGeneric0(DecodedInstruction& instruction, DecodedOperand* operands, const Information& information) {
    return DecodeGeneric<mnemonic, extension, arch, NoRegister, NoRegister, NoRegister, NoRegister, ImmType>(instruction, operands, information);
}

template<Mnemonic mnemonic, Extension extension, ArchFeature arch>
constexpr DecoderStatus DecodeUType(DecodedInstruction& instruction, DecodedOperand* operands, const Information& info) {
    return DecodeGeneric1<mnemonic, extension, arch, GPR0<Rd, true>, UImmediate>(instruction, operands, info);
}

template<Mnemonic mnemonic, Extension extension, ArchFeature arch>
constexpr DecoderStatus DecodeJType(DecodedInstruction& instruction, DecodedOperand* operands, const Information& info) {
    return DecodeGeneric1<mnemonic, extension, arch, GPR0<Rd, true>, JImmediate>(instruction, operands, info);
}

template<Mnemonic mnemonic, Extension extension, ArchFeature arch, template<uint32_t (*Func)(uint32_t), bool Write> class DestReg = GPR0, template<uint32_t (*Func)(uint32_t), bool Write> class SourceReg = GPR0>
constexpr DecoderStatus DecodeIType(DecodedInstruction& instruction, DecodedOperand* operands, const Information& info) {
    return DecodeGeneric2<mnemonic, extension, arch, DestReg<Rd, true>, SourceReg<Rs1, false>, IImmediate>(instruction, operands, info);
}

template<Mnemonic mnemonic, Extension extension, ArchFeature arch, class SourceReg1 = GPR0<Rs1, false>, class SourceReg2 = GPR0<Rs2, false>>
constexpr DecoderStatus DecodeBType(DecodedInstruction& instruction, DecodedOperand* operands, const Information& info) {
    return DecodeGeneric2<mnemonic, extension, arch, SourceReg1, SourceReg2, BImmediate>(instruction, operands, info);
}

template<Mnemonic mnemonic, Extension extension, ArchFeature arch, template<uint32_t (*Func)(uint32_t), bool Write> class SourceReg>
constexpr DecoderStatus DecodeSType(DecodedInstruction& instruction, DecodedOperand* operands, const Information& info) {
    return DecodeGeneric2<mnemonic, extension, arch, GPRAddress<Rs1>, SourceReg<Rs2, false>, SImmediate>(instruction, operands, info);
}


template<Mnemonic mnemonic, Extension extension, ArchFeature arch, template<uint32_t (*Func)(uint32_t), bool Write> class DestReg = GPR0, template<uint32_t (*Func)(uint32_t), bool Write> class SourceReg1 = GPR0, template<uint32_t (*Func)(uint32_t), bool Write> class SourceReg2 = GPR0>
constexpr DecoderStatus DecodeRType(DecodedInstruction& instruction, DecodedOperand* operands, const Information& info) {
    return DecodeGeneric3<mnemonic, extension, arch, DestReg<Rd, true>, SourceReg1<Rs1, false>, SourceReg2<Rs2, false>, NoImmediate>(instruction, operands, info);
}

template<Mnemonic mnemonic, Extension extension, ArchFeature arch, template<uint32_t (*Func)(uint32_t), bool Write> class DestReg = GPR0, template<uint32_t (*Func)(uint32_t), bool Write> class SourceReg = GPR0>
constexpr DecoderStatus DecodeR2Type(DecodedInstruction& instruction, DecodedOperand* operands, const Information& info) {
    return DecodeGeneric2<mnemonic, extension, arch, DestReg<Rd, true>, SourceReg<Rs1, false>,  NoImmediate>(instruction, operands, info);
}

template<Mnemonic mnemonic, Extension extension, ArchFeature arch, template<uint32_t (*Func)(uint32_t), bool Write> class DestReg = GPR0, template<uint32_t (*Func)(uint32_t), bool Write> class SourceReg = GPR0>
constexpr DecoderStatus DecodeShiftType(DecodedInstruction& instruction, DecodedOperand* operands, const Information& info) {
    return DecodeGeneric2<mnemonic, extension, arch, DestReg<Rd, true>, SourceReg<Rs1, false>, ShiftImmediate>(instruction, operands, info);
}

template<Mnemonic mnemonic, Extension extension, ArchFeature arch>
constexpr DecoderStatus DecodeNoOperands(DecodedInstruction& instruction, DecodedOperand* operands, const Information& info) {
    return DecodeGeneric0<mnemonic, extension, arch, NoImmediate>(instruction, operands, info);
}

}

namespace biscuit {

Decoder::~Decoder() = default;

DecoderStatus Decoder::Decode(const void* buffer, size_t length, DecodedInstruction& instruction, DecodedOperand* operands) const {
    if (length < 2) {
        return DecoderStatus::OutOfLength;
    }

    if (!buffer) {
        return DecoderStatus::InvalidBuffer;
    }

    uint16_t storage16;
    memcpy(&storage16, buffer, sizeof(uint16_t));

    if ((storage16 & 0b11) != 0b11) {
        // return DecodeCompressed(storage16, instruction, operands);
        return DecoderStatus::UnknownInstruction;
    } else {
        if (length < 4) {
            return DecoderStatus::OutOfLength;
        }

        uint32_t storage32;
        memcpy(&storage32, buffer, sizeof(uint32_t));

        return DecodeRegular(storage32, instruction, operands);
    }
}

DecoderStatus Decoder::DecodeRegular(uint32_t data, DecodedInstruction& instruction, DecodedOperand* operands) const {
    uint32_t opcode = data & 0x7F;

    Information info;
    info.data = data;
    info.host_arch = m_features;

    switch (opcode) {
        case 0b0110111: {
            return DecodeUType<Mnemonic::LUI, Extension::I, ArchFeature::RV32>(instruction, operands, info);
        }
        case 0b0010111: {
            return DecodeUType<Mnemonic::AUIPC, Extension::I, ArchFeature::RV32>(instruction, operands, info);
        }
        case 0b1101111: {
            return DecodeJType<Mnemonic::JAL, Extension::I, ArchFeature::RV32>(instruction, operands, info);
        }
        case 0b1100111: {
            switch (Funct3(data)) {
                case 0b000: {
                    return DecodeIType<Mnemonic::JALR, Extension::I, ArchFeature::RV32>(instruction, operands, info);
                }
                default: {
                    return DecoderStatus::UnknownInstruction;
                }
            }
        }
        case 0b1100011: {
            switch (Funct3(data)) {
                case 0b000: {
                    return DecodeBType<Mnemonic::BEQ, Extension::I, ArchFeature::RV32>(instruction, operands, info);
                }
                case 0b001: {
                    return DecodeBType<Mnemonic::BNE, Extension::I, ArchFeature::RV32>(instruction, operands, info);
                }
                case 0b100: {
                    return DecodeBType<Mnemonic::BLT, Extension::I, ArchFeature::RV32>(instruction, operands, info);
                }
                case 0b101: {
                    return DecodeBType<Mnemonic::BGE, Extension::I, ArchFeature::RV32>(instruction, operands, info);
                }
                case 0b110: {
                    return DecodeBType<Mnemonic::BLTU, Extension::I, ArchFeature::RV32>(instruction, operands, info);
                }
                case 0b111: {
                    return DecodeBType<Mnemonic::BGEU, Extension::I, ArchFeature::RV32>(instruction, operands, info);
                }
                default: {
                    return DecoderStatus::UnknownInstruction;
                }
            }
        }
        case 0b0000011: {
            switch (Funct3(data)) {
                case 0b000: {
                    // Should the dest types be GPR0 (match host) as these sign/zero extend? same for all W instructions like ADDIW
                    return DecodeIType<Mnemonic::LB, Extension::I, ArchFeature::RV32, GPR8>(instruction, operands, info);
                }
                case 0b001: {
                    return DecodeIType<Mnemonic::LH, Extension::I, ArchFeature::RV32, GPR16>(instruction, operands, info);
                }
                case 0b010: {
                    return DecodeIType<Mnemonic::LW, Extension::I, ArchFeature::RV32, GPR32>(instruction, operands, info);
                }
                case 0b011: {
                    return DecodeIType<Mnemonic::LD, Extension::I, ArchFeature::RV64, GPR64>(instruction, operands, info);
                }
                case 0b100: {
                    return DecodeIType<Mnemonic::LBU, Extension::I, ArchFeature::RV32, GPR8>(instruction, operands, info);
                }
                case 0b101: {
                    return DecodeIType<Mnemonic::LHU, Extension::I, ArchFeature::RV32, GPR16>(instruction, operands, info);
                }
                case 0b110: {
                    return DecodeIType<Mnemonic::LWU, Extension::I, ArchFeature::RV64, GPR32>(instruction, operands, info);
                }
                default: {
                    return DecoderStatus::UnknownInstruction;
                }
            }
        }
        case 0b0000111: {
            switch (Funct3(data)) {
                case 0b010: {
                    return DecodeIType<Mnemonic::FLW, Extension::F, ArchFeature::RV32, FPR32, GPRAddress>(instruction, operands, info);
                }
                case 0b011: {
                    return DecodeIType<Mnemonic::FLD, Extension::D, ArchFeature::RV32, FPR64, GPRAddress>(instruction, operands, info);
                }
                default: {
                    return DecoderStatus::UnknownInstruction;
                }
            }

        }
        case 0b0100111: {
            switch (Funct3(data)) {
                case 0b010: {
                    return DecodeSType<Mnemonic::FSW, Extension::F, ArchFeature::RV32, FPR32>(instruction, operands, info);
                }
                case 0b011: {
                    return DecodeSType<Mnemonic::FSD, Extension::D, ArchFeature::RV32, FPR64>(instruction, operands, info);
                }
                default: {
                    return DecoderStatus::UnknownInstruction;
                }
            }
        }
        case 0b0100011: {
            switch (Funct3(data)) {
                case 0b000: {
                    return DecodeSType<Mnemonic::SB, Extension::I, ArchFeature::RV32, GPR8>(instruction, operands, info);
                }
                case 0b001: {
                    return DecodeSType<Mnemonic::SH, Extension::I, ArchFeature::RV32, GPR16>(instruction, operands, info);
                }
                case 0b010: {
                    return DecodeSType<Mnemonic::SW, Extension::I, ArchFeature::RV32, GPR32>(instruction, operands, info);
                }
                case 0b011: {
                    return DecodeSType<Mnemonic::SD, Extension::I, ArchFeature::RV64, GPR64>(instruction, operands, info);
                }
                default: {
                    return DecoderStatus::UnknownInstruction;
                }
            }
        }
        case 0b0010011: {
            switch (Funct3(data)) {
                case 0b000: {
                    return DecodeIType<Mnemonic::ADDI, Extension::I, ArchFeature::RV32>(instruction, operands, info);
                }
                case 0b001: {
                    switch (Funct7(data)) {
                        case 0b0000000: {
                            return DecodeShiftType<Mnemonic::SLLI, Extension::I, ArchFeature::RV32>(instruction, operands, info);
                        }
                        default: {
                            return DecoderStatus::UnknownInstruction;
                        }
                    }
                }
                case 0b010: {
                    return DecodeIType<Mnemonic::SLTI, Extension::I, ArchFeature::RV32>(instruction, operands, info);
                }
                case 0b011: {
                    return DecodeIType<Mnemonic::SLTIU, Extension::I, ArchFeature::RV32>(instruction, operands, info);
                }
                case 0b100: {
                    return DecodeIType<Mnemonic::XORI, Extension::I, ArchFeature::RV32>(instruction, operands, info);
                }
                case 0b101: {
                    switch (Funct7(data)) {
                        case 0b0000000: {
                            return DecodeShiftType<Mnemonic::SRLI, Extension::I, ArchFeature::RV32>(instruction, operands, info);
                        }
                        case 0b0100000: {
                            return DecodeShiftType<Mnemonic::SRAI, Extension::I, ArchFeature::RV32>(instruction, operands, info);
                        }
                        default: {
                            return DecoderStatus::UnknownInstruction;
                        }
                    }
                }
                case 0b110: {
                    return DecodeIType<Mnemonic::ORI, Extension::I, ArchFeature::RV32>(instruction, operands, info);
                }
                case 0b111: {
                    return DecodeIType<Mnemonic::ANDI, Extension::I, ArchFeature::RV32>(instruction, operands, info);
                }
                default: {
                    return DecoderStatus::UnknownInstruction;
                }
            }
        }
        case 0b0011011: {
            switch (Funct3(data)) {
                case 0b000: {
                    return DecodeIType<Mnemonic::ADDIW, Extension::I, ArchFeature::RV64, GPR32, GPR32>(instruction, operands, info);
                }
                case 0b001: {
                    switch (Funct7(data)) {
                        case 0b0000000: {
                            return DecodeShiftType<Mnemonic::SLLIW, Extension::I, ArchFeature::RV64, GPR32, GPR32>(instruction, operands, info);
                        }
                        default: {
                            return DecoderStatus::UnknownInstruction;
                        }
                    }
                }
                case 0b101: {
                    switch (Funct7(data)) {
                        case 0b0000000: {
                            return DecodeShiftType<Mnemonic::SRLIW, Extension::I, ArchFeature::RV64, GPR32, GPR32>(instruction, operands, info);
                        }
                        case 0b0100000: {
                            return DecodeShiftType<Mnemonic::SRAIW, Extension::I, ArchFeature::RV64, GPR32, GPR32>(instruction, operands, info);
                        }
                        default: {
                            return DecoderStatus::UnknownInstruction;
                        }
                    }
                }
                default: {
                    return DecoderStatus::UnknownInstruction;
                }
            }
        }
        case 0b0101111: {
            uint32_t funct5 = Funct7(data);
            bool aq = funct5 & 0b10;
            bool rl = funct5 & 0b01;
            funct5 = (funct5 >> 2) & 0x1F;
            SetAqrl(info, aq, rl);
            switch(Funct3(data)) {
                case 0b000: {
                    switch (funct5) {
                        case 0b00001: {
                            return DecodeRType<Mnemonic::AMOSWAP_B, Extension::Zabha, ArchFeature::RV32, GPR8, GPRAddress, GPR8>(instruction, operands, info);
                        }
                        case 0b00000: {
                            return DecodeRType<Mnemonic::AMOADD_B, Extension::Zabha, ArchFeature::RV32, GPR8, GPRAddress, GPR8>(instruction, operands, info);
                        }
                        case 0b00100: {
                            return DecodeRType<Mnemonic::AMOXOR_B, Extension::Zabha, ArchFeature::RV32, GPR8, GPRAddress, GPR8>(instruction, operands, info);
                        }
                        case 0b01100: {
                            return DecodeRType<Mnemonic::AMOAND_B, Extension::Zabha, ArchFeature::RV32, GPR8, GPRAddress, GPR8>(instruction, operands, info);
                        }
                        case 0b01000: {
                            return DecodeRType<Mnemonic::AMOOR_B, Extension::Zabha, ArchFeature::RV32, GPR8, GPRAddress, GPR8>(instruction, operands, info);
                        }
                        case 0b10000: {
                            return DecodeRType<Mnemonic::AMOMIN_B, Extension::Zabha, ArchFeature::RV32, GPR8, GPRAddress, GPR8>(instruction, operands, info);
                        }
                        case 0b10100: {
                            return DecodeRType<Mnemonic::AMOMAX_B, Extension::Zabha, ArchFeature::RV32, GPR8, GPRAddress, GPR8>(instruction, operands, info);
                        }
                        case 0b11000: {
                            return DecodeRType<Mnemonic::AMOMINU_B, Extension::Zabha, ArchFeature::RV32, GPR8, GPRAddress, GPR8>(instruction, operands, info);
                        }
                        case 0b11100: {
                            return DecodeRType<Mnemonic::AMOMAXU_B, Extension::Zabha, ArchFeature::RV32, GPR8, GPRAddress, GPR8>(instruction, operands, info);
                        }
                        default: {
                            return DecoderStatus::UnknownInstruction;
                        }
                    }
                }
                case 0b001: {
                    switch (funct5) {
                        case 0b00001: {
                            return DecodeRType<Mnemonic::AMOSWAP_H, Extension::Zabha, ArchFeature::RV32, GPR16, GPRAddress, GPR16>(instruction, operands, info);
                        }
                        case 0b00000: {
                            return DecodeRType<Mnemonic::AMOADD_H, Extension::Zabha, ArchFeature::RV32, GPR16, GPRAddress, GPR16>(instruction, operands, info);
                        }
                        case 0b00100: {
                            return DecodeRType<Mnemonic::AMOXOR_H, Extension::Zabha, ArchFeature::RV32, GPR16, GPRAddress, GPR16>(instruction, operands, info);
                        }
                        case 0b01100: {
                            return DecodeRType<Mnemonic::AMOAND_H, Extension::Zabha, ArchFeature::RV32, GPR16, GPRAddress, GPR16>(instruction, operands, info);
                        }
                        case 0b01000: {
                            return DecodeRType<Mnemonic::AMOOR_H, Extension::Zabha, ArchFeature::RV32, GPR16, GPRAddress, GPR16>(instruction, operands, info);
                        }
                        case 0b10000: {
                            return DecodeRType<Mnemonic::AMOMIN_H, Extension::Zabha, ArchFeature::RV32, GPR16, GPRAddress, GPR16>(instruction, operands, info);
                        }
                        case 0b10100: {
                            return DecodeRType<Mnemonic::AMOMAX_H, Extension::Zabha, ArchFeature::RV32, GPR16, GPRAddress, GPR16>(instruction, operands, info);
                        }
                        case 0b11000: {
                            return DecodeRType<Mnemonic::AMOMINU_H, Extension::Zabha, ArchFeature::RV32, GPR16, GPRAddress, GPR16>(instruction, operands, info);
                        }
                        case 0b11100: {
                            return DecodeRType<Mnemonic::AMOMAXU_H, Extension::Zabha, ArchFeature::RV32, GPR16, GPRAddress, GPR16>(instruction, operands, info);
                        }
                        default: {
                            return DecoderStatus::UnknownInstruction;
                        }
                    }
                }
                case 0b010: {
                    switch (funct5) {
                        case 0b00010: {
                            if (Rs2(data) != 0b00000) {
                                return DecoderStatus::UnknownInstruction;
                            }

                            return DecodeR2Type<Mnemonic::LR_W, Extension::A, ArchFeature::RV32, GPR32, GPRAddress>(instruction, operands, info);
                        }
                        case 0b00011: {
                            return DecodeRType<Mnemonic::SC_W, Extension::A, ArchFeature::RV32, GPR32, GPRAddress, GPR32>(instruction, operands, info);
                        }
                        case 0b00001: {
                            return DecodeRType<Mnemonic::AMOSWAP_W, Extension::A, ArchFeature::RV32, GPR32, GPRAddress, GPR32>(instruction, operands, info);
                        }
                        case 0b00000: {
                            return DecodeRType<Mnemonic::AMOADD_W, Extension::A, ArchFeature::RV32, GPR32, GPRAddress, GPR32>(instruction, operands, info);
                        }
                        case 0b00100: {
                            return DecodeRType<Mnemonic::AMOXOR_W, Extension::A, ArchFeature::RV32, GPR32, GPRAddress, GPR32>(instruction, operands, info);
                        }
                        case 0b01100: {
                            return DecodeRType<Mnemonic::AMOAND_W, Extension::A, ArchFeature::RV32, GPR32, GPRAddress, GPR32>(instruction, operands, info);
                        }
                        case 0b01000: {
                            return DecodeRType<Mnemonic::AMOOR_W, Extension::A, ArchFeature::RV32, GPR32, GPRAddress, GPR32>(instruction, operands, info);
                        }
                        case 0b10000: {
                            return DecodeRType<Mnemonic::AMOMIN_W, Extension::A, ArchFeature::RV32, GPR32, GPRAddress, GPR32>(instruction, operands, info);
                        }
                        case 0b10100: {
                            return DecodeRType<Mnemonic::AMOMAX_W, Extension::A, ArchFeature::RV32, GPR32, GPRAddress, GPR32>(instruction, operands, info);
                        }
                        case 0b11000: {
                            return DecodeRType<Mnemonic::AMOMINU_W, Extension::A, ArchFeature::RV32, GPR32, GPRAddress, GPR32>(instruction, operands, info);
                        }
                        case 0b11100: {
                            return DecodeRType<Mnemonic::AMOMAXU_W, Extension::A, ArchFeature::RV32, GPR32, GPRAddress, GPR32>(instruction, operands, info);
                        }
                        default: {
                            return DecoderStatus::UnknownInstruction;
                        }
                    }
                }
                case 0b011: {
                    switch (funct5) {
                        case 0b00010: {
                            if (Rs2(data) != 0b00000) {
                                return DecoderStatus::UnknownInstruction;
                            }

                            return DecodeR2Type<Mnemonic::LR_D, Extension::A, ArchFeature::RV64, GPR64, GPRAddress>(instruction, operands, info);
                        }
                        case 0b00011: {
                            return DecodeRType<Mnemonic::SC_D, Extension::A, ArchFeature::RV64, GPR64, GPRAddress, GPR64>(instruction, operands, info);
                        }
                        case 0b00001: {
                            return DecodeRType<Mnemonic::AMOSWAP_D, Extension::A, ArchFeature::RV64, GPR64, GPRAddress, GPR64>(instruction, operands, info);
                        }
                        case 0b00000: {
                            return DecodeRType<Mnemonic::AMOADD_D, Extension::A, ArchFeature::RV64, GPR64, GPRAddress, GPR64>(instruction, operands, info);
                        }
                        case 0b00100: {
                            return DecodeRType<Mnemonic::AMOXOR_D, Extension::A, ArchFeature::RV64, GPR64, GPRAddress, GPR64>(instruction, operands, info);
                        }
                        case 0b01100: {
                            return DecodeRType<Mnemonic::AMOAND_D, Extension::A, ArchFeature::RV64, GPR64, GPRAddress, GPR64>(instruction, operands, info);
                        }
                        case 0b01000: {
                            return DecodeRType<Mnemonic::AMOOR_D, Extension::A, ArchFeature::RV64, GPR64, GPRAddress, GPR64>(instruction, operands, info);
                        }
                        case 0b10000: {
                            return DecodeRType<Mnemonic::AMOMIN_D, Extension::A, ArchFeature::RV64, GPR64, GPRAddress, GPR64>(instruction, operands, info);
                        }
                        case 0b10100: {
                            return DecodeRType<Mnemonic::AMOMAX_D, Extension::A, ArchFeature::RV64, GPR64, GPRAddress, GPR64>(instruction, operands, info);
                        }
                        case 0b11000: {
                            return DecodeRType<Mnemonic::AMOMINU_D, Extension::A, ArchFeature::RV64, GPR64, GPRAddress, GPR64>(instruction, operands, info);
                        }
                        case 0b11100: {
                            return DecodeRType<Mnemonic::AMOMAXU_D, Extension::A, ArchFeature::RV64, GPR64, GPRAddress, GPR64>(instruction, operands, info);
                        }
                        default: {
                            return DecoderStatus::UnknownInstruction;
                        }
                    }
                }
                default: {
                    return DecoderStatus::UnknownInstruction;
                }
            }
        }
        case 0b0111011: {
            switch (Funct3(data)) {
                case 0b000: {
                    switch (Funct7(data)) {
                        case 0b0000000: {
                            return DecodeRType<Mnemonic::ADDW, Extension::I, ArchFeature::RV64, GPR32, GPR32, GPR32>(instruction, operands, info);
                        }
                        case 0b0000001: {
                            return DecodeRType<Mnemonic::MULW, Extension::M, ArchFeature::RV64, GPR32, GPR32, GPR32>(instruction, operands, info);
                        }
                        case 0b0100000: {
                            return DecodeRType<Mnemonic::SUBW, Extension::I, ArchFeature::RV64, GPR32, GPR32, GPR32>(instruction, operands, info);
                        }
                        default: {
                            return DecoderStatus::UnknownInstruction;
                        }
                    }
                }
                case 0b001: {
                    switch (Funct7(data)) {
                        case 0b0000000: {
                            return DecodeRType<Mnemonic::SLLW, Extension::I, ArchFeature::RV64, GPR32, GPR32, GPR32>(instruction, operands, info);
                        }
                        default: {
                            return DecoderStatus::UnknownInstruction;
                        }
                    }
                }
                case 0b100: {
                    switch(Funct7(data)) {
                        case 0b0000001: {
                            return DecodeRType<Mnemonic::DIVW, Extension::M, ArchFeature::RV64, GPR32, GPR32, GPR32>(instruction, operands, info);
                        }
                        default: {
                            return DecoderStatus::UnknownInstruction;
                        }
                    }
                }
                case 0b101: {
                    switch (Funct7(data)) {
                        case 0b0000000: {
                            return DecodeRType<Mnemonic::SRLW, Extension::I, ArchFeature::RV64, GPR32, GPR32, GPR32>(instruction, operands, info);
                        }
                        case 0b0000001: {
                            return DecodeRType<Mnemonic::DIVUW, Extension::M, ArchFeature::RV64, GPR32, GPR32, GPR32>(instruction, operands, info);
                        }
                        case 0b0100000: {
                            return DecodeRType<Mnemonic::SRAW, Extension::I, ArchFeature::RV64, GPR32, GPR32, GPR32>(instruction, operands, info);
                        }
                        default: {
                            return DecoderStatus::UnknownInstruction;
                        }
                    }
                }
                case 0b110: {
                    switch (Funct7(data)) {
                        case 0b0000001: {
                            return DecodeRType<Mnemonic::REMW, Extension::M, ArchFeature::RV64, GPR32, GPR32, GPR32>(instruction, operands, info);
                        }
                        default: {
                            return DecoderStatus::UnknownInstruction;
                        }
                    }
                }
                case 0b111: {
                    switch (Funct7(data)) {
                        case 0b0000001: {
                            return DecodeRType<Mnemonic::REMUW, Extension::M, ArchFeature::RV64, GPR32, GPR32, GPR32>(instruction, operands, info);
                        }
                        default: {
                            return DecoderStatus::UnknownInstruction;
                        }
                    }
                }
                default: {
                    return DecoderStatus::UnknownInstruction;
                }
            }
        }
        case 0b1010011: {
            info.rmode = static_cast<RMode>(Funct3(data));
            switch (Funct7(data)) {
                case 0b0000000: {
                    return DecodeRType<Mnemonic::FADD_S, Extension::F, ArchFeature::RV32, FPR32, FPR32, FPR32>(instruction, operands, info);
                }
                case 0b0000001: {
                    return DecodeRType<Mnemonic::FADD_D, Extension::D, ArchFeature::RV32, FPR64, FPR64, FPR64>(instruction, operands, info);
                }
                case 0b0000100: {
                    return DecodeRType<Mnemonic::FSUB_S, Extension::F, ArchFeature::RV32, FPR32, FPR32, FPR32>(instruction, operands, info);
                }
                case 0b0000101: {
                    return DecodeRType<Mnemonic::FSUB_D, Extension::D, ArchFeature::RV32, FPR64, FPR64, FPR64>(instruction, operands, info);
                }
                case 0b0001000: {
                    return DecodeRType<Mnemonic::FMUL_S, Extension::F, ArchFeature::RV32, FPR32, FPR32, FPR32>(instruction, operands, info);
                }
                case 0b0001001: {
                    return DecodeRType<Mnemonic::FMUL_D, Extension::D, ArchFeature::RV32, FPR64, FPR64, FPR64>(instruction, operands, info);
                }
                case 0b0001100: {
                    return DecodeRType<Mnemonic::FDIV_S, Extension::F, ArchFeature::RV32, FPR32, FPR32, FPR32>(instruction, operands, info);
                }
                case 0b0001101: {
                    return DecodeRType<Mnemonic::FDIV_D, Extension::D, ArchFeature::RV32, FPR64, FPR64, FPR64>(instruction, operands, info);
                }
                case 0b0101100: {
                    return DecodeR2Type<Mnemonic::FSQRT_S, Extension::F, ArchFeature::RV32, FPR32, FPR32>(instruction, operands, info);
                }
                case 0b0101101: {
                    return DecodeR2Type<Mnemonic::FSQRT_D, Extension::D, ArchFeature::RV32, FPR64, FPR64>(instruction, operands, info);
                }
                case 0b0010000: {
                    switch (Funct3(data)) {
                        case 0b000: {
                            return DecodeRType<Mnemonic::FSGNJ_S, Extension::F, ArchFeature::RV32, FPR32, FPR32, FPR32>(instruction, operands, info);
                        }
                        case 0b001: {
                            return DecodeRType<Mnemonic::FSGNJN_S, Extension::F, ArchFeature::RV32, FPR32, FPR32, FPR32>(instruction, operands, info);
                        }
                        case 0b010: {
                            return DecodeRType<Mnemonic::FSGNJX_S, Extension::F, ArchFeature::RV32, FPR32, FPR32, FPR32>(instruction, operands, info);
                        }
                        default: {
                            return DecoderStatus::UnknownInstruction;
                        }
                    }
                }
                case 0b0010001: {
                    switch (Funct3(data)) {
                        case 0b000: {
                            return DecodeRType<Mnemonic::FSGNJ_D, Extension::D, ArchFeature::RV32, FPR64, FPR64, FPR64>(instruction, operands, info);
                        }
                        case 0b001: {
                            return DecodeRType<Mnemonic::FSGNJN_D, Extension::D, ArchFeature::RV32, FPR64, FPR64, FPR64>(instruction, operands, info);
                        }
                        case 0b010: {
                            return DecodeRType<Mnemonic::FSGNJX_D, Extension::D, ArchFeature::RV32, FPR64, FPR64, FPR64>(instruction, operands, info);
                        }
                        default: {
                            return DecoderStatus::UnknownInstruction;
                        }
                    }
                }
                case 0b0010100: {
                    switch (Funct3(data)) {
                        case 0b000: {
                            return DecodeRType<Mnemonic::FMIN_S, Extension::F, ArchFeature::RV32, FPR32, FPR32, FPR32>(instruction, operands, info);
                        }
                        case 0b001: {
                            return DecodeRType<Mnemonic::FMAX_S, Extension::F, ArchFeature::RV32, FPR32, FPR32, FPR32>(instruction, operands, info);
                        }
                        default: {
                            return DecoderStatus::UnknownInstruction;
                        }
                    }
                }
                case 0b0010101: {
                    switch (Funct3(data)) {
                        case 0b000: {
                            return DecodeRType<Mnemonic::FMIN_D, Extension::F, ArchFeature::RV32, FPR64, FPR64, FPR64>(instruction, operands, info);
                        }
                        case 0b001: {
                            return DecodeRType<Mnemonic::FMAX_D, Extension::F, ArchFeature::RV32, FPR64, FPR64, FPR64>(instruction, operands, info);
                        }
                        default: {
                            return DecoderStatus::UnknownInstruction;
                        }
                    }
                }
                case 0b0100000: {
                    switch (Rs2(data)) {
                        case 0b00001: {
                            return DecodeR2Type<Mnemonic::FCVT_S_D, Extension::D, ArchFeature::RV32, FPR32, FPR64>(instruction, operands, info);
                        }
                        default: {
                            return DecoderStatus::UnknownInstruction;
                        }
                    }
                }
                case 0b0100001: {
                    switch (Rs2(data)) {
                        case 0b00000: {
                            return DecodeR2Type<Mnemonic::FCVT_D_S, Extension::D, ArchFeature::RV32, FPR64, FPR32>(instruction, operands, info);
                        }
                        default: {
                            return DecoderStatus::UnknownInstruction;
                        }
                    }
                }
                case 0b1100000: {
                    switch (Rs2(data)) {
                        case 0b00000: {
                            return DecodeR2Type<Mnemonic::FCVT_W_S, Extension::F, ArchFeature::RV32, GPR32, FPR32>(instruction, operands, info);
                        }
                        case 0b00001: {
                            return DecodeR2Type<Mnemonic::FCVT_WU_S, Extension::F, ArchFeature::RV32, GPR32, FPR32>(instruction, operands, info);
                        }
                        case 0b00010: {
                            return DecodeR2Type<Mnemonic::FCVT_L_S, Extension::F, ArchFeature::RV64, GPR64, FPR32>(instruction, operands, info);
                        }
                        case 0b00011: {
                            return DecodeR2Type<Mnemonic::FCVT_LU_S, Extension::F, ArchFeature::RV64, GPR64, FPR32>(instruction, operands, info);
                        }
                        default: {
                            return DecoderStatus::UnknownInstruction;
                        }
                    }
                }
                case 0b1100001: {
                    switch (Rs2(data)) {
                        case 0b00000: {
                            return DecodeR2Type<Mnemonic::FCVT_W_D, Extension::D, ArchFeature::RV32, GPR32, FPR64>(instruction, operands, info);
                        }
                        case 0b00001: {
                            return DecodeR2Type<Mnemonic::FCVT_WU_D, Extension::D, ArchFeature::RV32, GPR32, FPR64>(instruction, operands, info);
                        }
                        case 0b00010: {
                            return DecodeR2Type<Mnemonic::FCVT_L_D, Extension::D, ArchFeature::RV64, GPR64, FPR64>(instruction, operands, info);
                        }
                        case 0b00011: {
                            return DecodeR2Type<Mnemonic::FCVT_LU_D, Extension::D, ArchFeature::RV64, GPR64, FPR64>(instruction, operands, info);
                        }
                        default: {
                            return DecoderStatus::UnknownInstruction;
                        }
                    }
                }
                case 0b1110000: {
                    switch (Rs2(data)) {
                        case 0b00000: {
                            switch (Funct3(data)) {
                                case 0b000: {
                                    return DecodeR2Type<Mnemonic::FMV_X_W, Extension::F, ArchFeature::RV32, FPR32, GPR32>(instruction, operands, info);
                                }
                                case 0b001: {
                                    return DecodeR2Type<Mnemonic::FCLASS_S, Extension::F, ArchFeature::RV32, FPR32, FPR32>(instruction, operands, info);
                                }
                                default: {
                                    return DecoderStatus::UnknownInstruction;
                                }
                            }
                        }
                        default: {
                            return DecoderStatus::UnknownInstruction;
                        }
                    }
                }
                case 0b1110001: {
                    switch (Rs2(data)) {
                        case 0b00000: {
                            switch (Funct3(data)) {
                                case 0b000: {
                                    return DecodeR2Type<Mnemonic::FMV_X_D, Extension::D, ArchFeature::RV64, FPR64, GPR64>(instruction, operands, info);
                                }
                                case 0b001: {
                                    return DecodeR2Type<Mnemonic::FCLASS_D, Extension::D, ArchFeature::RV64, FPR64, FPR64>(instruction, operands, info);
                                }
                                default: {
                                    return DecoderStatus::UnknownInstruction;
                                }
                            }
                        }
                        default: {
                            return DecoderStatus::UnknownInstruction;
                        }
                    }
                }
                case 0b1010000: {
                    switch (Funct3(data)) {
                        case 0b000: {
                            return DecodeRType<Mnemonic::FLE_S, Extension::F, ArchFeature::RV32, GPR0, FPR32, FPR32>(instruction, operands, info);
                        }
                        case 0b001: {
                            return DecodeRType<Mnemonic::FLT_S, Extension::F, ArchFeature::RV32, GPR0, FPR32, FPR32>(instruction, operands, info);
                        }
                        case 0b010: {
                            return DecodeRType<Mnemonic::FEQ_S, Extension::F, ArchFeature::RV32, GPR0, FPR32, FPR32>(instruction, operands, info);
                        }
                        default: {
                            return DecoderStatus::UnknownInstruction;
                        }
                    }
                }
                case 0b1010001: {
                    switch (Funct3(data)) {
                        case 0b000: {
                            return DecodeRType<Mnemonic::FLE_D, Extension::D, ArchFeature::RV32, GPR0, FPR64, FPR64>(instruction, operands, info);
                        }
                        case 0b001: {
                            return DecodeRType<Mnemonic::FLT_D, Extension::D, ArchFeature::RV32, GPR0, FPR64, FPR64>(instruction, operands, info);
                        }
                        case 0b010: {
                            return DecodeRType<Mnemonic::FEQ_D, Extension::D, ArchFeature::RV32, GPR0, FPR64, FPR64>(instruction, operands, info);
                        }
                        default: {
                            return DecoderStatus::UnknownInstruction;
                        }
                    }
                }
                case 0b1101000: {
                    switch (Rs2(data)) {
                        case 0b00000: {
                            return DecodeR2Type<Mnemonic::FCVT_S_W, Extension::F, ArchFeature::RV32, FPR32, GPR32>(instruction, operands, info);
                        }
                        case 0b00001: {
                            return DecodeR2Type<Mnemonic::FCVT_S_WU, Extension::F, ArchFeature::RV32, FPR32, GPR32>(instruction, operands, info);
                        }
                        case 0b00010: {
                            return DecodeR2Type<Mnemonic::FCVT_S_L, Extension::F, ArchFeature::RV64, FPR32, GPR64>(instruction, operands, info);
                        }
                        case 0b00011: {
                            return DecodeR2Type<Mnemonic::FCVT_S_LU, Extension::F, ArchFeature::RV64, FPR32, GPR64>(instruction, operands, info);
                        }
                        default: {
                            return DecoderStatus::UnknownInstruction;
                        }
                    }
                }
                case 0b1101001: {
                    switch (Rs2(data)) {
                        case 0b00000: {
                            return DecodeR2Type<Mnemonic::FCVT_D_W, Extension::D, ArchFeature::RV32, FPR64, GPR32>(instruction, operands, info);
                        }
                        case 0b00001: {
                            return DecodeR2Type<Mnemonic::FCVT_D_WU, Extension::D, ArchFeature::RV32, FPR64, GPR32>(instruction, operands, info);
                        }
                        case 0b00010: {
                            return DecodeR2Type<Mnemonic::FCVT_D_L, Extension::D, ArchFeature::RV64, FPR64, GPR64>(instruction, operands, info);
                        }
                        case 0b00011: {
                            return DecodeR2Type<Mnemonic::FCVT_D_LU, Extension::D, ArchFeature::RV64, FPR64, GPR64>(instruction, operands, info);
                        }
                        default: {
                            return DecoderStatus::UnknownInstruction;
                        }
                    }
                }
                case 0b1111000: {
                    if (Rs2(data) != 0b00000 || Funct3(data) != 0b000) {
                        return DecoderStatus::UnknownInstruction;
                    }

                    return DecodeR2Type<Mnemonic::FMV_W_X, Extension::F, ArchFeature::RV32, FPR32, GPR32>(instruction, operands, info);
                }
                case 0b1111001: {
                    if (Rs2(data) != 0b00000 || Funct3(data) != 0b000) {
                        return DecoderStatus::UnknownInstruction;
                    }

                    return DecodeR2Type<Mnemonic::FMV_D_X, Extension::D, ArchFeature::RV32, FPR64, GPR0>(instruction, operands, info);
                }
                default: {
                    return DecoderStatus::UnknownInstruction;
                }
            }
        }
        case 0b0110011: {
            switch (Funct3(data)) {
                case 0b000: {
                    switch (Funct7(data)) {
                        case 0b0000000: {
                            return DecodeRType<Mnemonic::ADD, Extension::I, ArchFeature::RV32>(instruction, operands, info);
                        }
                        case 0b0000001: {
                            return DecodeRType<Mnemonic::MUL, Extension::M, ArchFeature::RV32>(instruction, operands, info);
                        }
                        case 0b0100000: {
                            return DecodeRType<Mnemonic::SUB, Extension::I, ArchFeature::RV32>(instruction, operands, info);
                        }
                        default: {
                            return DecoderStatus::UnknownInstruction;
                        }
                    }
                }
                case 0b001: {
                    switch (Funct7(data)) {
                        case 0b0000000: {
                            return DecodeRType<Mnemonic::SLL, Extension::I, ArchFeature::RV32>(instruction, operands, info);
                        }
                        case 0b0000001: {
                            return DecodeRType<Mnemonic::MULH, Extension::M, ArchFeature::RV32>(instruction, operands, info);
                        }
                        default: {
                            return DecoderStatus::UnknownInstruction;
                        }
                    }
                }
                case 0b010: {
                    switch (Funct7(data)) {
                        case 0b0000000: {
                            return DecodeRType<Mnemonic::SLT, Extension::I, ArchFeature::RV32>(instruction, operands, info);
                        }
                        case 0b0000001: {
                            return DecodeRType<Mnemonic::MULHSU, Extension::M, ArchFeature::RV32>(instruction, operands, info);
                        }
                        default: {
                            return DecoderStatus::UnknownInstruction;
                        }
                    }
                }
                case 0b011: {
                    switch (Funct7(data)) {
                        case 0b0000000: {
                            return DecodeRType<Mnemonic::SLTU, Extension::I, ArchFeature::RV32>(instruction, operands, info);
                        }
                        case 0b0000001: {
                            return DecodeRType<Mnemonic::MULHU, Extension::M, ArchFeature::RV32>(instruction, operands, info);
                        }
                        default: {
                            return DecoderStatus::UnknownInstruction;
                        }
                    }
                }
                case 0b100: {
                    switch (Funct7(data)) {
                        case 0b0000000: {
                            return DecodeRType<Mnemonic::XOR, Extension::I, ArchFeature::RV32>(instruction, operands, info);
                        }
                        case 0b0000001: {
                            return DecodeRType<Mnemonic::DIV, Extension::M, ArchFeature::RV32>(instruction, operands, info);
                        }
                        default: {
                            return DecoderStatus::UnknownInstruction;
                        }
                    }
                }
                case 0b101: {
                    switch (Funct7(data)) {
                        case 0b0000000: {
                            return DecodeRType<Mnemonic::SRL, Extension::I, ArchFeature::RV32>(instruction, operands, info);
                        }
                        case 0b0000001: {
                            return DecodeRType<Mnemonic::DIVU, Extension::M, ArchFeature::RV32>(instruction, operands, info);
                        }
                        case 0b0100000: {
                            return DecodeRType<Mnemonic::SRA, Extension::I, ArchFeature::RV32>(instruction, operands, info);
                        }
                        default: {
                            return DecoderStatus::UnknownInstruction;
                        }
                    }
                }
                case 0b110: {
                    switch (Funct7(data)) {
                        case 0b0000000: {
                            return DecodeRType<Mnemonic::OR, Extension::I, ArchFeature::RV32>(instruction, operands, info);
                        }
                        case 0b0000001: {
                            return DecodeRType<Mnemonic::REM, Extension::M, ArchFeature::RV32>(instruction, operands, info);
                        }
                        default: {
                            return DecoderStatus::UnknownInstruction;
                        }
                    }
                }
                case 0b111: {
                    switch (Funct7(data)) {
                        case 0b0000000: {
                            return DecodeRType<Mnemonic::AND, Extension::I, ArchFeature::RV32>(instruction, operands, info);
                        }
                        case 0b0000001: {
                            return DecodeRType<Mnemonic::REMU, Extension::M, ArchFeature::RV32>(instruction, operands, info);
                        }
                        default: {
                            return DecoderStatus::UnknownInstruction;
                        }
                    }
                }
                default: {
                    return DecoderStatus::UnknownInstruction;
                }
            }
        }
        case 0b0001111: {
            switch (Funct3(data)) {
                case 0b000: {
                    if (data == 0x0100000F) {
                        return DecodeNoOperands<Mnemonic::PAUSE, Extension::I, ArchFeature::RV32>(instruction, operands, info);
                    } else if (data == 0x8330000F) {
                        return DecodeNoOperands<Mnemonic::FENCETSO, Extension::I, ArchFeature::RV32>(instruction, operands, info);
                    } else {
                        return DecodeR2Type<Mnemonic::FENCE, Extension::I, ArchFeature::RV32>(instruction, operands, info);
                    }
                }
                case 0b001: {
                    return DecodeIType<Mnemonic::FENCEI, Extension::Zifencei, ArchFeature::RV32>(instruction, operands, info);
                }
                default: {
                    return DecoderStatus::UnknownInstruction;
                }
            }
        }
        case 0b1110011: {
            if (data == 0x00000073) {
                return DecodeNoOperands<Mnemonic::ECALL, Extension::I, ArchFeature::RV32>(instruction, operands, info);
            } else if (data == 0x00100073) {
                return DecodeNoOperands<Mnemonic::EBREAK, Extension::I, ArchFeature::RV32>(instruction, operands, info);
            } else {
                return DecoderStatus::UnknownInstruction;
            }
        }
        default: {
            return DecoderStatus::UnknownInstruction;
        }
    }
}

} // namespace biscuit
