#include <catch/catch.hpp>

#include <array>
#include <vector>

#include <biscuit/assembler.hpp>

using namespace biscuit;

TEST_CASE("Branch to Self", "[branch]") {
    uint32_t data;
    biscuit::Assembler as(reinterpret_cast<uint8_t*>(&data), sizeof(data));

    // Simple branch to self with a jump instruction.
    {
        Label label;
        as.Bind(&label);
        as.J(&label);
        REQUIRE(data == 0x0000006F);
    }

    as.RewindBuffer();

    // Simple branch to self with a conditional branch instruction.
    {
        Label label;
        as.Bind(&label);
        as.BNE(x3, x4, &label);
        REQUIRE(data == 0x00419063);
    }
}

TEST_CASE("Branch with Instructions Between", "[branch]") {
    std::array<uint32_t, 20> data{};
    biscuit::Assembler as(reinterpret_cast<uint8_t*>(&data), sizeof(data));

    // Simple branch backward
    {
        Label label;
        as.Bind(&label);
        as.ADD(x1, x2, x3);
        as.SUB(x2, x4, x3);
        as.J(&label);
        REQUIRE(data[2] == 0xFF9FF06F);
    }

    as.RewindBuffer();

    // Simple branch forward
    {
        Label label;
        as.J(&label);
        as.ADD(x1, x2, x3);
        as.SUB(x2, x4, x3);
        as.Bind(&label);
        REQUIRE(data[0] == 0x00C0006F);
    }
}