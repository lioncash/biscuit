#pragma once

#include <cstdint>

namespace biscuit {

// Control and Status Register
enum class CSR : uint32_t {
    // clang-format off

    // User-level CSRs

    UStatus        = 0x000, // User status register
    UIE            = 0x004, // User interrupt-enable register
    UTVEC          = 0x005, // User trap handler base address
    UScratch       = 0x040, // Scratch register for user trap handlers
    UEPC           = 0x041, // User exception program counter
    UCause         = 0x042, // User trap cause
    UTVal          = 0x043, // User bad address or instruction
    UIP            = 0x044, // User interrupt pending

    FFlags         = 0x001, // Floating-point Accrued Exceptions
    FRM            = 0x002, // Floating-point Dynamic Rounding Mode
    FCSR           = 0x003, // Floating-point Control and Status Register (frm + fflags)

    Cycle          = 0xC00, // Cycle counter for RDCYCLE instruction.
    Time           = 0xC01, // Timer for RDTIME instruction.
    InstRet        = 0xC02, // Instructions retired counter for RDINSTRET instruction.
    CycleH         = 0xC80, // Upper 32 bits of cycle, RV32I only.
    TimeH          = 0xC81, // Upper 32 bits of time, RV32I only.
    InstRetH       = 0xC82, // Upper 32 bits of instret, RV32I only.

    // Supervisor-level CSRs

    SStatus        = 0x100, // Supervisor status register
    SEDeleg        = 0x102, // Supervisor exeception delegation register
    SIDeleg        = 0x103, // Supervisor interrupt delegation register
    SIE            = 0x104, // Supervisor interrupt-enable register
    STVec          = 0x105, // Supervisor trap handler base address
    SCounterEn     = 0x106, // Supervisor counter enable

    SScratch       = 0x140, // Scratch register for supervisor trap handlers
    SEPC           = 0x141, // Supervisor exception program counter
    SCause         = 0x142, // Supervisor trap cause
    STVal          = 0x143, // Supervisor bad address or instruction
    SIP            = 0x144, // Supervisor interrupt pending.

    SATP           = 0x180, // Supervisor address translation and protection

    // Machine-level CSRs

    MVendorID      = 0xF11, // Vendor ID
    MArchID        = 0xF12, // Architecture ID
    MImpID         = 0xF13, // Implementation ID
    MHartID        = 0xF14, // Hardware Thread ID

    MStatus        = 0x300, // Machine status register
    MISA           = 0x301, // ISA and extensions
    MEDeleg        = 0x302, // Machine exception delegation register
    MIDeleg        = 0x303, // Machine interrupt delegation register
    MIE            = 0x304, // Machine interrupt-enable register
    MRVec          = 0x305, // Machine trap-handler base address
    MCounterEn     = 0x306, // Machine counter enable

    MScratch       = 0x340, // Scratch register for machine trap handlers
    MEPC           = 0x341, // Machine exception program counter
    MCause         = 0x342, // Machine trap cause
    MTVal          = 0x343, // Machine bad address or instruction
    MIP            = 0x344, // Machine interrupt pending

    PMPCfg0        = 0x3A0, // Physical memory protection configuration
    PMPCfg1        = 0x3A1, // Physical memory protection configuration, RV32 only.
    PMPCfg2        = 0x3A2, // Physical memory protection configuration
    PMPCfg3        = 0x3A3, // Physical memory protection configuration, RV32 only.
    PMPAddr0       = 0x3B0, // Physical memory protection address register
    PMPAddr1       = 0x3B1, // Physical memory protection address register
    PMPAddr2       = 0x3B2, // Physical memory protection address register
    PMPAddr3       = 0x3B3, // Physical memory protection address register
    PMPAddr4       = 0x3B4, // Physical memory protection address register
    PMPAddr5       = 0x3B5, // Physical memory protection address register
    PMPAddr6       = 0x3B6, // Physical memory protection address register
    PMPAddr7       = 0x3B7, // Physical memory protection address register
    PMPAddr8       = 0x3B8, // Physical memory protection address register
    PMPAddr9       = 0x3B9, // Physical memory protection address register
    PMPAddr10      = 0x3BA, // Physical memory protection address register
    PMPAddr11      = 0x3BB, // Physical memory protection address register
    PMPAddr12      = 0x3BC, // Physical memory protection address register
    PMPAddr13      = 0x3BD, // Physical memory protection address register
    PMPAddr14      = 0x3BE, // Physical memory protection address register
    PMPAddr15      = 0x3BF, // Physical memory protection address register

    MCycle         = 0xB00, // Machine cycle counter
    MInstRet       = 0xB02, // Machine instructions-retired counter
    MHPMCounter3   = 0xB03, // Machine performance-monitoring counter
    MHPMCounter4   = 0xB04, // Machine performance-monitoring counter
    MHPMCounter5   = 0xB05, // Machine performance-monitoring counter
    MHPMCounter6   = 0xB06, // Machine performance-monitoring counter
    MHPMCounter7   = 0xB07, // Machine performance-monitoring counter
    MHPMCounter8   = 0xB08, // Machine performance-monitoring counter
    MHPMCounter9   = 0xB09, // Machine performance-monitoring counter
    MHPMCounter10  = 0xB0A, // Machine performance-monitoring counter
    MHPMCounter11  = 0xB0B, // Machine performance-monitoring counter
    MHPMCounter12  = 0xB0C, // Machine performance-monitoring counter
    MHPMCounter13  = 0xB0D, // Machine performance-monitoring counter
    MHPMCounter14  = 0xB0E, // Machine performance-monitoring counter
    MHPMCounter15  = 0xB0F, // Machine performance-monitoring counter
    MHPMCounter16  = 0xB10, // Machine performance-monitoring counter
    MHPMCounter17  = 0xB11, // Machine performance-monitoring counter
    MHPMCounter18  = 0xB12, // Machine performance-monitoring counter
    MHPMCounter19  = 0xB13, // Machine performance-monitoring counter
    MHPMCounter20  = 0xB14, // Machine performance-monitoring counter
    MHPMCounter21  = 0xB15, // Machine performance-monitoring counter
    MHPMCounter22  = 0xB16, // Machine performance-monitoring counter
    MHPMCounter23  = 0xB17, // Machine performance-monitoring counter
    MHPMCounter24  = 0xB18, // Machine performance-monitoring counter
    MHPMCounter25  = 0xB19, // Machine performance-monitoring counter
    MHPMCounter26  = 0xB1A, // Machine performance-monitoring counter
    MHPMCounter27  = 0xB1B, // Machine performance-monitoring counter
    MHPMCounter28  = 0xB1C, // Machine performance-monitoring counter
    MHPMCounter29  = 0xB1D, // Machine performance-monitoring counter
    MHPMCounter30  = 0xB1E, // Machine performance-monitoring counter
    MHPMCounter31  = 0xB1F, // Machine performance-monitoring counter

    MCycleH        = 0xB80, // Upper 32 bits ofmcycle, RV32I only
    MInstRetH      = 0xB82, // Upper 32 bits ofminstret, RV32I only

    MHPMCounter3H  = 0xB83, // Upper 32 bits of MHPMCounter3, RV32I only
    MHPMCounter4H  = 0xB84, // Upper 32 bits of MHPMCounter4, RV32I only
    MHPMCounter5H  = 0xB85, // Upper 32 bits of MHPMCounter5, RV32I only
    MHPMCounter6H  = 0xB86, // Upper 32 bits of MHPMCounter6, RV32I only
    MHPMCounter7H  = 0xB87, // Upper 32 bits of MHPMCounter7, RV32I only
    MHPMCounter8H  = 0xB88, // Upper 32 bits of MHPMCounter8, RV32I only
    MHPMCounter9H  = 0xB89, // Upper 32 bits of MHPMCounter9, RV32I only
    MHPMCounter10H = 0xB8A, // Upper 32 bits of MHPMCounter10, RV32I only
    MHPMCounter11H = 0xB8B, // Upper 32 bits of MHPMCounter11, RV32I only
    MHPMCounter12H = 0xB8C, // Upper 32 bits of MHPMCounter12, RV32I only
    MHPMCounter13H = 0xB8D, // Upper 32 bits of MHPMCounter13, RV32I only
    MHPMCounter14H = 0xB8E, // Upper 32 bits of MHPMCounter14, RV32I only
    MHPMCounter15H = 0xB8F, // Upper 32 bits of MHPMCounter15, RV32I only
    MHPMCounter16H = 0xB90, // Upper 32 bits of MHPMCounter16, RV32I only
    MHPMCounter17H = 0xB91, // Upper 32 bits of MHPMCounter17, RV32I only
    MHPMCounter18H = 0xB92, // Upper 32 bits of MHPMCounter18, RV32I only
    MHPMCounter19H = 0xB93, // Upper 32 bits of MHPMCounter19, RV32I only
    MHPMCounter20H = 0xB94, // Upper 32 bits of MHPMCounter20, RV32I only
    MHPMCounter21H = 0xB95, // Upper 32 bits of MHPMCounter21, RV32I only
    MHPMCounter22H = 0xB96, // Upper 32 bits of MHPMCounter22, RV32I only
    MHPMCounter23H = 0xB97, // Upper 32 bits of MHPMCounter23, RV32I only
    MHPMCounter24H = 0xB98, // Upper 32 bits of MHPMCounter24, RV32I only
    MHPMCounter25H = 0xB99, // Upper 32 bits of MHPMCounter25, RV32I only
    MHPMCounter26H = 0xB9A, // Upper 32 bits of MHPMCounter26, RV32I only
    MHPMCounter27H = 0xB9B, // Upper 32 bits of MHPMCounter27, RV32I only
    MHPMCounter28H = 0xB9C, // Upper 32 bits of MHPMCounter28, RV32I only
    MHPMCounter29H = 0xB9D, // Upper 32 bits of MHPMCounter29, RV32I only
    MHPMCounter30H = 0xB9E, // Upper 32 bits of MHPMCounter30, RV32I only
    MHPMCounter31H = 0xB9F, // Upper 32 bits of MHPMCounter31, RV32I only

    MCountInhibit  = 0x320, // Machine counter-inhibit register

    MHPMEvent3     = 0x323, // Machine performance-monitoring event selector
    MHPMEvent4     = 0x324, // Machine performance-monitoring event selector
    MHPMEvent5     = 0x325, // Machine performance-monitoring event selector
    MHPMEvent6     = 0x326, // Machine performance-monitoring event selector
    MHPMEvent7     = 0x327, // Machine performance-monitoring event selector
    MHPMEvent8     = 0x328, // Machine performance-monitoring event selector
    MHPMEvent9     = 0x329, // Machine performance-monitoring event selector
    MHPMEvent10    = 0x32A, // Machine performance-monitoring event selector
    MHPMEvent11    = 0x32B, // Machine performance-monitoring event selector
    MHPMEvent12    = 0x32C, // Machine performance-monitoring event selector
    MHPMEvent13    = 0x32D, // Machine performance-monitoring event selector
    MHPMEvent14    = 0x32E, // Machine performance-monitoring event selector
    MHPMEvent15    = 0x32F, // Machine performance-monitoring event selector
    MHPMEvent16    = 0x330, // Machine performance-monitoring event selector
    MHPMEvent17    = 0x331, // Machine performance-monitoring event selector
    MHPMEvent18    = 0x332, // Machine performance-monitoring event selector
    MHPMEvent19    = 0x333, // Machine performance-monitoring event selector
    MHPMEvent20    = 0x334, // Machine performance-monitoring event selector
    MHPMEvent21    = 0x335, // Machine performance-monitoring event selector
    MHPMEvent22    = 0x336, // Machine performance-monitoring event selector
    MHPMEvent23    = 0x337, // Machine performance-monitoring event selector
    MHPMEvent24    = 0x338, // Machine performance-monitoring event selector
    MHPMEvent25    = 0x339, // Machine performance-monitoring event selector
    MHPMEvent26    = 0x33A, // Machine performance-monitoring event selector
    MHPMEvent27    = 0x33B, // Machine performance-monitoring event selector
    MHPMEvent28    = 0x33C, // Machine performance-monitoring event selector
    MHPMEvent29    = 0x33D, // Machine performance-monitoring event selector
    MHPMEvent30    = 0x33E, // Machine performance-monitoring event selector
    MHPMEvent31    = 0x33F, // Machine performance-monitoring event selector

    TSelect        = 0x7A0, // Debug/Trace trigger register select
    TData1         = 0x7A1, // First Debug/Trace trigger data register
    TData2         = 0x7A2, // Second Debug/Trace trigger data register
    TData3         = 0x7A3, // Third Debug/Trace trigger data register

    DCSR           = 0x7B0, // Debug control and status register
    DPC            = 0x7B1, // Debug PC
    DScratch0      = 0x7B2, // Debug scratch register 0
    DScratch1      = 0x7B3, // Debug scratch register 0

    // clang-format on
};

} // namespace biscuit
