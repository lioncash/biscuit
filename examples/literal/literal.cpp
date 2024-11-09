#include <biscuit/assert.hpp>
#include <biscuit/assembler.hpp>

#include <iostream>

using namespace biscuit;

constexpr uint64_t literal1_value = 0x1234567890ABCDEF;
constexpr uint64_t literal2_value = 0x1122334455667788;
constexpr uint64_t literal3_value = 0xFEDCBA0987654321;
constexpr uint64_t literal4_value = 0xAABBCCDDEEFF0011;

void print_literals(uint64_t literal1, uint64_t literal2, uint64_t literal3, uint64_t literal4) {
    std::cout << "Literal 1: " << std::hex << literal1 << std::endl;
    std::cout << "Literal 2: " << std::hex << literal2 << std::endl;
    std::cout << "Literal 3: " << std::hex << literal3 << std::endl;
    std::cout << "Literal 4: " << std::hex << literal4 << std::endl;

    BISCUIT_ASSERT(literal1 == literal1_value);
    BISCUIT_ASSERT(literal2 == literal2_value);
    BISCUIT_ASSERT(literal3 == literal3_value);
    BISCUIT_ASSERT(literal4 == literal4_value);
}

int main() {
    Assembler as(0x5000);

    Literal64 literal1(literal1_value);
    Literal64 literal2(literal2_value);
    Literal64 literal3(literal3_value);
    Literal64 literal4(literal4_value);

    // Literal placed before the code, more than 0x1000 bytes away
    as.Place(&literal1);

    as.AdvanceBuffer(as.GetCodeBuffer().GetCursorOffset() + 0x1000);

    // Literal placed before the code, less than 0x1000 bytes away
    as.Place(&literal2);

    void (*code)() = reinterpret_cast<void(*)()>(as.GetCursorPointer());
    as.ADDI(sp, sp, -8);
    as.SD(ra, 0, sp);

    as.LD(a0, &literal1);

    as.LD(a1, &literal2);

    as.LD(a2, &literal3);

    as.LD(a3, &literal4);

    as.LI(t0, (uint64_t)print_literals);
    as.JALR(t0);

    as.LD(ra, 0, sp);
    as.ADDI(sp, sp, 8);
    as.RET();

    // Literal placed after the code, less than 0x1000 bytes away
    as.Place(&literal3);

    as.AdvanceBuffer(as.GetCodeBuffer().GetCursorOffset() + 0x1000);

    // Literal placed after the code, more than 0x1000 bytes away
    as.Place(&literal4);

    as.GetCodeBuffer().SetExecutable();

    code();

    return 0;
}
