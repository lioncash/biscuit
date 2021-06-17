#pragma once

#include <cstdint>
#include <biscuit/assert.hpp>

namespace biscuit {

/**
 * Generic abstraction around a register.
 * 
 * This is less bug-prone than using raw primitive sizes
 * in opcode emitter functions, since it provides stronger typing.
 */
class Register {
public:
    constexpr Register() noexcept = default;

    /// Gets the index for this register.
    [[nodiscard]] constexpr uint32_t Index() const noexcept {
        return m_index;
    }

    /// Determines whether or not this register is a general-purpose register.
    [[nodiscard]] constexpr bool IsGPR() const noexcept {
        return m_type == Type::GPR;
    }

    /// Determines whether or not this register is a floating-point register.
    [[nodiscard]] constexpr bool IsFPR() const noexcept {
        return m_type == Type::FPR;
    }

    /// Ensures that this register represents a valid GPR.
    void EnforceValidGPR() const noexcept {
        // TODO: Handle the RISC-V compressed extension, which only has 16 GPRs.
        BISCUIT_ASSERT(IsGPR() && m_index < 32);
    }

    /// Ensures that this register represents a valid FPR.
    void EnforceValidFPR() const noexcept {
        BISCUIT_ASSERT(IsFPR() && m_index < 32);
    }

protected:
    enum class Type {
        GPR, // General purpose register
        FPR, // Floating-point register
    };

    constexpr Register(uint32_t index, Type type) noexcept
        : m_index{index}, m_type{type} {}

private:
    uint32_t m_index{};
    Type m_type{};
};

/// General purpose register.
class GPR : public Register {
public:
    constexpr GPR() : Register{0, Type::GPR} {}
    constexpr explicit GPR(uint32_t index) : Register{index, Type::GPR} {}
};

/// Floating point register.
class FPR : public Register {
public:
    constexpr FPR() : Register{0, Type::FPR} {}
    constexpr explicit FPR(uint32_t index) : Register{index, Type::FPR} {}
};

constexpr GPR x0{0};
constexpr GPR x1{1};
constexpr GPR x2{2};
constexpr GPR x3{3};
constexpr GPR x4{4};
constexpr GPR x5{5};
constexpr GPR x6{6};
constexpr GPR x7{7};
constexpr GPR x8{8};
constexpr GPR x9{9};
constexpr GPR x10{10};
constexpr GPR x11{11};
constexpr GPR x12{12};
constexpr GPR x13{13};
constexpr GPR x14{14};
constexpr GPR x15{15};
constexpr GPR x16{16};
constexpr GPR x17{17};
constexpr GPR x18{18};
constexpr GPR x19{19};
constexpr GPR x20{20};
constexpr GPR x21{21};
constexpr GPR x22{22};
constexpr GPR x23{23};
constexpr GPR x24{24};
constexpr GPR x25{25};
constexpr GPR x26{26};
constexpr GPR x27{27};
constexpr GPR x28{28};
constexpr GPR x29{29};
constexpr GPR x30{30};
constexpr GPR x31{31};

constexpr FPR f0{0};
constexpr FPR f1{1};
constexpr FPR f2{2};
constexpr FPR f3{3};
constexpr FPR f4{4};
constexpr FPR f5{5};
constexpr FPR f6{6};
constexpr FPR f7{7};
constexpr FPR f8{8};
constexpr FPR f9{9};
constexpr FPR f10{10};
constexpr FPR f11{11};
constexpr FPR f12{12};
constexpr FPR f13{13};
constexpr FPR f14{14};
constexpr FPR f15{15};
constexpr FPR f16{16};
constexpr FPR f17{17};
constexpr FPR f18{18};
constexpr FPR f19{19};
constexpr FPR f20{20};
constexpr FPR f21{21};
constexpr FPR f22{22};
constexpr FPR f23{23};
constexpr FPR f24{24};
constexpr FPR f25{25};
constexpr FPR f26{26};
constexpr FPR f27{27};
constexpr FPR f28{28};
constexpr FPR f29{29};
constexpr FPR f30{30};
constexpr FPR f31{31};

} // namespace biscuit
