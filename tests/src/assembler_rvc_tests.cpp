#include <catch/catch.hpp>

#include <array>
#include <biscuit/assembler.hpp>

using namespace biscuit;

TEST_CASE("C.ADDI", "[rvc]") {
    uint32_t value = 0;
    Assembler as(reinterpret_cast<uint8_t*>(&value), sizeof(value));

    as.C_ADDI(x15, -1);
    REQUIRE(value == 0x17FD);

    as.RewindBuffer();

    as.C_ADDI(x15, -32);
    REQUIRE(value == 0x1781);

    as.RewindBuffer();

    as.C_ADDI(x15, 31);
    REQUIRE(value == 0x07FD);
}

TEST_CASE("C.ADDIW", "[rvc]") {
    uint32_t value = 0;
    Assembler as(reinterpret_cast<uint8_t*>(&value), sizeof(value));

    as.C_ADDIW(x15, -1);
    REQUIRE(value == 0x37FD);

    as.RewindBuffer();

    as.C_ADDIW(x15, -32);
    REQUIRE(value == 0x3781);

    as.RewindBuffer();

    as.C_ADDIW(x15, 31);
    REQUIRE(value == 0x27FD);
}

TEST_CASE("C.ADDI4SPN", "[rvc]") {
    uint32_t value = 0;
    Assembler as(reinterpret_cast<uint8_t*>(&value), sizeof(value));

    as.C_ADDI4SPN(x15, 0xFF);
    REQUIRE(value == 0x1FFC);

    as.RewindBuffer();

    as.C_ADDI4SPN(x8, 0xFF);
    REQUIRE(value == 0x1FE0);
}

TEST_CASE("C.ADDI16SP", "[rvc]") {
    uint32_t value = 0;
    Assembler as(reinterpret_cast<uint8_t*>(&value), sizeof(value));

    as.C_ADDI16SP(16);
    REQUIRE(value == 0x6141);

    as.RewindBuffer();

    as.C_ADDI16SP(64);
    REQUIRE(value == 0x6121);
}

TEST_CASE("C.ANDI", "[rvc]") {
    uint32_t value = 0;
    Assembler as(reinterpret_cast<uint8_t*>(&value), sizeof(value));

    as.C_ANDI(x15, 16);
    REQUIRE(value == 0x8BC1);

    as.RewindBuffer();

    as.C_ANDI(x15, 31);
    REQUIRE(value == 0x8BFD);
}

TEST_CASE("C.FLD", "[rvc]") {
    uint32_t value = 0;
    Assembler as(reinterpret_cast<uint8_t*>(&value), sizeof(value));

    as.C_FLD(f15, 8, x15);
    REQUIRE(value == 0x279C);

    as.RewindBuffer();

    as.C_FLD(f15, 24, x15);
    REQUIRE(value == 0x2F9C);
}

TEST_CASE("C.FLW", "[rvc]") {
    uint32_t value = 0;
    Assembler as(reinterpret_cast<uint8_t*>(&value), sizeof(value));

    as.C_FLW(f15, 16, x15);
    REQUIRE(value == 0x6B9C);

    as.RewindBuffer();

    as.C_FLW(f15, 24, x15);
    REQUIRE(value == 0x6F9C);
}

TEST_CASE("C.FSD", "[rvc]") {
    uint32_t value = 0;
    Assembler as(reinterpret_cast<uint8_t*>(&value), sizeof(value));

    as.C_FSD(f15, 8, x15);
    REQUIRE(value == 0xA79C);

    as.RewindBuffer();

    as.C_FSD(f15, 24, x15);
    REQUIRE(value == 0xAF9C);
}

TEST_CASE("C.FSW", "[rvc]") {
    uint32_t value = 0;
    Assembler as(reinterpret_cast<uint8_t*>(&value), sizeof(value));

    as.C_FSW(f15, 16, x15);
    REQUIRE(value == 0xEB9C);

    as.RewindBuffer();

    as.C_FSW(f15, 24, x15);
    REQUIRE(value == 0xEF9C);
}

TEST_CASE("C.LD", "[rvc]") {
    uint32_t value = 0;
    Assembler as(reinterpret_cast<uint8_t*>(&value), sizeof(value));

    as.C_LD(x15, 8, x15);
    REQUIRE(value == 0x679C);

    as.RewindBuffer();

    as.C_LD(x15, 24, x15);
    REQUIRE(value == 0x6F9C);
}

TEST_CASE("C.LI", "[rvc]") {
    uint32_t value = 0;
    Assembler as(reinterpret_cast<uint8_t*>(&value), sizeof(value));

    as.C_LI(x15, -1);
    REQUIRE(value == 0x57FD);

    as.RewindBuffer();

    as.C_LI(x15, -32);
    REQUIRE(value == 0x5781);

    as.RewindBuffer();

    as.C_LI(x15, 31);
    REQUIRE(value == 0x47FD);
}

TEST_CASE("C.LQ", "[rvc]") {
    uint32_t value = 0;
    Assembler as(reinterpret_cast<uint8_t*>(&value), sizeof(value));

    as.C_LQ(x15, 16, x15);
    REQUIRE(value == 0x2B9C);

    as.RewindBuffer();

    as.C_LQ(x15, 256, x15);
    REQUIRE(value == 0x279C);
}

TEST_CASE("C.LUI", "[rvc]") {
    uint32_t value = 0;
    Assembler as(reinterpret_cast<uint8_t*>(&value), sizeof(value));

    as.C_LUI(x15, 0x3F000);
    REQUIRE(value == 0x77FD);

    as.RewindBuffer();

    as.C_LUI(x15, 0x0F000);
    REQUIRE(value == 0x67BD);
}

TEST_CASE("C.LW", "[rvc]") {
    uint32_t value = 0;
    Assembler as(reinterpret_cast<uint8_t*>(&value), sizeof(value));

    as.C_LW(x15, 16, x15);
    REQUIRE(value == 0x4B9C);

    as.RewindBuffer();

    as.C_LW(x15, 24, x15);
    REQUIRE(value == 0x4F9C);
}

TEST_CASE("C.NOP", "[rvc]") {
    uint32_t value = 0;
    Assembler as(reinterpret_cast<uint8_t*>(&value), sizeof(value));

    as.C_NOP();
    REQUIRE(value == 0x0001);
}

TEST_CASE("C.SD", "[rvc]") {
    uint32_t value = 0;
    Assembler as(reinterpret_cast<uint8_t*>(&value), sizeof(value));

    as.C_SD(x15, 8, x15);
    REQUIRE(value == 0xE79C);

    as.RewindBuffer();

    as.C_SD(x15, 24, x15);
    REQUIRE(value == 0xEF9C);
}

TEST_CASE("C.SQ", "[rvc]") {
    uint32_t value = 0;
    Assembler as(reinterpret_cast<uint8_t*>(&value), sizeof(value));

    as.C_SQ(x15, 16, x15);
    REQUIRE(value == 0xAB9C);

    as.RewindBuffer();

    as.C_SQ(x15, 256, x15);
    REQUIRE(value == 0xA79C);
}

TEST_CASE("C.SRAI", "[rvc]") {
    uint32_t value = 0;
    Assembler as(reinterpret_cast<uint8_t*>(&value), sizeof(value));

    as.C_SRAI(x15, 16);
    REQUIRE(value == 0x87C1);

    as.RewindBuffer();

    as.C_SRAI(x15, 31);
    REQUIRE(value == 0x87FD);
}

TEST_CASE("C.SRLI", "[rvc]") {
    uint32_t value = 0;
    Assembler as(reinterpret_cast<uint8_t*>(&value), sizeof(value));

    as.C_SRLI(x15, 16);
    REQUIRE(value == 0x83C1);

    as.RewindBuffer();

    as.C_SRLI(x15, 31);
    REQUIRE(value == 0x83FD);
}

TEST_CASE("C.SUB", "[rvc]") {
    uint32_t value = 0;
    Assembler as(reinterpret_cast<uint8_t*>(&value), sizeof(value));

    as.C_SUB(x15, x15);
    REQUIRE(value == 0x8F9D);

    as.RewindBuffer();

    as.C_SUB(x15, x8);
    REQUIRE(value == 0x8F81);
}

TEST_CASE("C.SW", "[rvc]") {
    uint32_t value = 0;
    Assembler as(reinterpret_cast<uint8_t*>(&value), sizeof(value));

    as.C_SW(x15, 16, x15);
    REQUIRE(value == 0xCB9C);

    as.RewindBuffer();

    as.C_SW(x15, 24, x15);
    REQUIRE(value == 0xCF9C);
}

TEST_CASE("C.UNDEF", "[rvc]") {
    uint32_t value = 0;
    Assembler as(reinterpret_cast<uint8_t*>(&value), sizeof(value));

    as.C_UNDEF();
    REQUIRE(value == 0);
}

TEST_CASE("C.XOR", "[rvc]") {
    uint32_t value = 0;
    Assembler as(reinterpret_cast<uint8_t*>(&value), sizeof(value));

    as.C_XOR(x15, x15);
    REQUIRE(value == 0x8FBD);

    as.RewindBuffer();

    as.C_XOR(x15, x8);
    REQUIRE(value == 0x8FA1);
}
