#include <catch/catch.hpp>

#include <biscuit/assembler.hpp>

TEST_CASE("ADD", "[rv32i]") {
    uint32_t value = 0;
    biscuit::Assembler as(reinterpret_cast<uint8_t*>(&value), sizeof(value));

    as.ADD(biscuit::x7, biscuit::x15, biscuit::x31);
    REQUIRE(value == 0x01F783B3);

    as.RewindBuffer();

    as.ADD(biscuit::x31, biscuit::x31, biscuit::x31);
    REQUIRE(value == 0x01FF8FB3);

    as.RewindBuffer();

    as.ADD(biscuit::x0, biscuit::x0, biscuit::x0);
    REQUIRE(value == 0x00000033);
}

TEST_CASE("ADDI", "[rv32i]") {
    uint32_t value = 0;
    biscuit::Assembler as(reinterpret_cast<uint8_t*>(&value), sizeof(value));

    as.ADDI(biscuit::x15, biscuit::x31, 1024);
    REQUIRE(value == 0x400F8793);

    as.RewindBuffer();

    as.ADDI(biscuit::x15, biscuit::x31, 2048);
    REQUIRE(value == 0x800F8793);

    as.RewindBuffer();

    as.ADDI(biscuit::x15, biscuit::x31, 4095);
    REQUIRE(value == 0xFFFF8793);
}

TEST_CASE("ANDI", "[rv32i]") {
    uint32_t value = 0;
    biscuit::Assembler as(reinterpret_cast<uint8_t*>(&value), sizeof(value));

    as.ANDI(biscuit::x15, biscuit::x31, 1024);
    REQUIRE(value == 0x400FF793);

    as.RewindBuffer();

    as.ANDI(biscuit::x15, biscuit::x31, 2048);
    REQUIRE(value == 0x800FF793);

    as.RewindBuffer();

    as.ANDI(biscuit::x15, biscuit::x31, 4095);
    REQUIRE(value == 0xFFFFF793);
}

TEST_CASE("AUIPC", "[rv32i]") {
    uint32_t value = 0;
    biscuit::Assembler as(reinterpret_cast<uint8_t*>(&value), sizeof(value));

    as.AUIPC(biscuit::x31, 0xFFFFFFFF);
    REQUIRE(value == 0xFFFFFF97);

    as.RewindBuffer();

    as.AUIPC(biscuit::x31, 0);
    REQUIRE(value == 0x00000F97);

    as.RewindBuffer();

    as.AUIPC(biscuit::x31, 0x00FF00FF);
    REQUIRE(value == 0x00FF0F97);
}

TEST_CASE("BEQ", "[rv32i]") {
    uint32_t value = 0;
    biscuit::Assembler as(reinterpret_cast<uint8_t*>(&value), sizeof(value));

    as.BEQ(biscuit::x15, biscuit::x31, 2000);
    REQUIRE(value == 0x7DF78863);

    as.RewindBuffer();

    as.BEQ(biscuit::x15, biscuit::x31, 4096);
    REQUIRE(value == 0x81F78063);

    as.RewindBuffer();

    as.BEQ(biscuit::x15, biscuit::x31, 8191);
    REQUIRE(value == 0xFFF78FE3);
}

TEST_CASE("BGE", "[rv32i]") {
    uint32_t value = 0;
    biscuit::Assembler as(reinterpret_cast<uint8_t*>(&value), sizeof(value));

    as.BGE(biscuit::x15, biscuit::x31, 2000);
    REQUIRE(value == 0x7DF7D863);

    as.RewindBuffer();

    as.BGE(biscuit::x15, biscuit::x31, 4096);
    REQUIRE(value == 0x81F7D063);

    as.RewindBuffer();

    as.BGE(biscuit::x15, biscuit::x31, 8191);
    REQUIRE(value == 0xFFF7DFE3);
}

TEST_CASE("BGEU", "[rv32i]") {
    uint32_t value = 0;
    biscuit::Assembler as(reinterpret_cast<uint8_t*>(&value), sizeof(value));

    as.BGEU(biscuit::x15, biscuit::x31, 2000);
    REQUIRE(value == 0x7DF7F863);

    as.RewindBuffer();

    as.BGEU(biscuit::x15, biscuit::x31, 4096);
    REQUIRE(value == 0x81F7F063);

    as.RewindBuffer();

    as.BGEU(biscuit::x15, biscuit::x31, 8191);
    REQUIRE(value == 0xFFF7FFE3);
}

TEST_CASE("BNE", "[rv32i]") {
    uint32_t value = 0;
    biscuit::Assembler as(reinterpret_cast<uint8_t*>(&value), sizeof(value));

    as.BNE(biscuit::x15, biscuit::x31, 2000);
    REQUIRE(value == 0x7DF79863);

    as.RewindBuffer();

    as.BNE(biscuit::x15, biscuit::x31, 4096);
    REQUIRE(value == 0x81F79063);

    as.RewindBuffer();

    as.BNE(biscuit::x15, biscuit::x31, 8191);
    REQUIRE(value == 0xFFF79FE3);
}

TEST_CASE("BLT", "[rv32i]") {
    uint32_t value = 0;
    biscuit::Assembler as(reinterpret_cast<uint8_t*>(&value), sizeof(value));

    as.BLT(biscuit::x15, biscuit::x31, 2000);
    REQUIRE(value == 0x7DF7C863);

    as.RewindBuffer();

    as.BLT(biscuit::x15, biscuit::x31, 4096);
    REQUIRE(value == 0x81F7C063);

    as.RewindBuffer();

    as.BLT(biscuit::x15, biscuit::x31, 8191);
    REQUIRE(value == 0xFFF7CFE3);
}

TEST_CASE("BLTU", "[rv32i]") {
    uint32_t value = 0;
    biscuit::Assembler as(reinterpret_cast<uint8_t*>(&value), sizeof(value));

    as.BLTU(biscuit::x15, biscuit::x31, 2000);
    REQUIRE(value == 0x7DF7E863);

    as.RewindBuffer();

    as.BLTU(biscuit::x15, biscuit::x31, 4096);
    REQUIRE(value == 0x81F7E063);

    as.RewindBuffer();

    as.BLTU(biscuit::x15, biscuit::x31, 8191);
    REQUIRE(value == 0xFFF7EFE3);
}

TEST_CASE("JAL", "[rv32i]") {
    uint32_t value = 0;
    biscuit::Assembler as(reinterpret_cast<uint8_t*>(&value), sizeof(value));

    as.JAL(biscuit::x31, 0xFFFFFFFF);
    REQUIRE(value == 0xFFFFFFEF);

    as.RewindBuffer();

    as.JAL(biscuit::x31, 2000);
    REQUIRE(value == 0x7D000FEF);

    as.RewindBuffer();

    as.JAL(biscuit::x31, 100000);
    REQUIRE(value == 0x6A018FEF);
}

TEST_CASE("JALR", "[rv32i]") {
    uint32_t value = 0;
    biscuit::Assembler as(reinterpret_cast<uint8_t*>(&value), sizeof(value));

    as.JALR(biscuit::x15, 1024, biscuit::x31);
    REQUIRE(value == 0x400F87E7);

    as.RewindBuffer();

    as.JALR(biscuit::x15, 2048, biscuit::x31);
    REQUIRE(value == 0x800F87E7);

    as.RewindBuffer();

    as.JALR(biscuit::x15, 4095, biscuit::x31);
    REQUIRE(value == 0xFFFF87E7);
}

TEST_CASE("LB", "[rv32i]") {
    uint32_t value = 0;
    biscuit::Assembler as(reinterpret_cast<uint8_t*>(&value), sizeof(value));

    as.LB(biscuit::x15, 1024, biscuit::x31);
    REQUIRE(value == 0x400F8783);

    as.RewindBuffer();

    as.LB(biscuit::x15, 2048, biscuit::x31);
    REQUIRE(value == 0x800F8783);

    as.RewindBuffer();

    as.LB(biscuit::x15, 4095, biscuit::x31);
    REQUIRE(value == 0xFFFF8783);
}

TEST_CASE("LBU", "[rv32i]") {
    uint32_t value = 0;
    biscuit::Assembler as(reinterpret_cast<uint8_t*>(&value), sizeof(value));

    as.LBU(biscuit::x15, 1024, biscuit::x31);
    REQUIRE(value == 0x400FC783);

    as.RewindBuffer();

    as.LBU(biscuit::x15, 2048, biscuit::x31);
    REQUIRE(value == 0x800FC783);

    as.RewindBuffer();

    as.LBU(biscuit::x15, 4095, biscuit::x31);
    REQUIRE(value == 0xFFFFC783);
}

TEST_CASE("LH", "[rv32i]") {
    uint32_t value = 0;
    biscuit::Assembler as(reinterpret_cast<uint8_t*>(&value), sizeof(value));

    as.LH(biscuit::x15, 1024, biscuit::x31);
    REQUIRE(value == 0x400F9783);

    as.RewindBuffer();

    as.LH(biscuit::x15, 2048, biscuit::x31);
    REQUIRE(value == 0x800F9783);

    as.RewindBuffer();

    as.LH(biscuit::x15, 4095, biscuit::x31);
    REQUIRE(value == 0xFFFF9783);
}

TEST_CASE("LHU", "[rv32i]") {
    uint32_t value = 0;
    biscuit::Assembler as(reinterpret_cast<uint8_t*>(&value), sizeof(value));

    as.LHU(biscuit::x15, 1024, biscuit::x31);
    REQUIRE(value == 0x400FD783);

    as.RewindBuffer();

    as.LHU(biscuit::x15, 2048, biscuit::x31);
    REQUIRE(value == 0x800FD783);

    as.RewindBuffer();

    as.LHU(biscuit::x15, 4095, biscuit::x31);
    REQUIRE(value == 0xFFFFD783);
}

TEST_CASE("LUI", "[rv32i]") {
    uint32_t value = 0;
    biscuit::Assembler as(reinterpret_cast<uint8_t*>(&value), sizeof(value));

    as.LUI(biscuit::x10, 0xFFFFFFFF);
    REQUIRE(value == 0xFFFFF537);

    as.RewindBuffer();

    as.LUI(biscuit::x10, 0x7FFFF000);
    REQUIRE(value == 0x7FFFF537);

    as.RewindBuffer();

    as.LUI(biscuit::x31, 0xFFFFFFFF);
    REQUIRE(value == 0xFFFFFFB7);
}

TEST_CASE("LW", "[rv32i]") {
    uint32_t value = 0;
    biscuit::Assembler as(reinterpret_cast<uint8_t*>(&value), sizeof(value));

    as.LW(biscuit::x15, 1024, biscuit::x31);
    REQUIRE(value == 0x400FA783);

    as.RewindBuffer();

    as.LW(biscuit::x15, 2048, biscuit::x31);
    REQUIRE(value == 0x800FA783);

    as.RewindBuffer();

    as.LW(biscuit::x15, 4095, biscuit::x31);
    REQUIRE(value == 0xFFFFA783);
}

TEST_CASE("ORI", "[rv32i]") {
    uint32_t value = 0;
    biscuit::Assembler as(reinterpret_cast<uint8_t*>(&value), sizeof(value));

    as.ORI(biscuit::x15, biscuit::x31, 1024);
    REQUIRE(value == 0x400FE793);

    as.RewindBuffer();

    as.ORI(biscuit::x15, biscuit::x31, 2048);
    REQUIRE(value == 0x800FE793);

    as.RewindBuffer();

    as.ORI(biscuit::x15, biscuit::x31, 4095);
    REQUIRE(value == 0xFFFFE793);
}

TEST_CASE("SB", "[rv32i]") {
    uint32_t value = 0;
    biscuit::Assembler as(reinterpret_cast<uint8_t*>(&value), sizeof(value));

    as.SB(biscuit::x31, 1024, biscuit::x15);
    REQUIRE(value == 0x41F78023);

    as.RewindBuffer();

    as.SB(biscuit::x31, 2048, biscuit::x15);
    REQUIRE(value == 0x81F78023);

    as.RewindBuffer();

    as.SB(biscuit::x31, 4095, biscuit::x15);
    REQUIRE(value == 0xFFF78FA3);
}

TEST_CASE("SH", "[rv32i]") {
    uint32_t value = 0;
    biscuit::Assembler as(reinterpret_cast<uint8_t*>(&value), sizeof(value));

    as.SH(biscuit::x31, 1024, biscuit::x15);
    REQUIRE(value == 0x41F79023);

    as.RewindBuffer();

    as.SH(biscuit::x31, 2048, biscuit::x15);
    REQUIRE(value == 0x81F79023);

    as.RewindBuffer();

    as.SH(biscuit::x31, 4095, biscuit::x15);
    REQUIRE(value == 0xFFF79FA3);
}

TEST_CASE("SLL", "[rv32i]") {
    uint32_t value = 0;
    biscuit::Assembler as(reinterpret_cast<uint8_t*>(&value), sizeof(value));

    as.SLL(biscuit::x7, biscuit::x15, biscuit::x31);
    REQUIRE(value == 0x01F793B3);

    as.RewindBuffer();

    as.SLL(biscuit::x31, biscuit::x31, biscuit::x31);
    REQUIRE(value == 0x01FF9FB3);

    as.RewindBuffer();

    as.SLL(biscuit::x0, biscuit::x0, biscuit::x0);
    REQUIRE(value == 0x00001033);
}

TEST_CASE("SLLI", "[rv32i]") {
    uint32_t value = 0;
    biscuit::Assembler as(reinterpret_cast<uint8_t*>(&value), sizeof(value));

    as.SLLI(biscuit::x31, biscuit::x15, 10);
    REQUIRE(value == 0x00A79F93);

    as.RewindBuffer();

    as.SLLI(biscuit::x31, biscuit::x15, 20);
    REQUIRE(value == 0x01479F93);

    as.RewindBuffer();

    as.SLLI(biscuit::x31, biscuit::x15, 31);
    REQUIRE(value == 0x01F79F93);
}

TEST_CASE("SLT", "[rv32i]") {
    uint32_t value = 0;
    biscuit::Assembler as(reinterpret_cast<uint8_t*>(&value), sizeof(value));

    as.SLT(biscuit::x7, biscuit::x15, biscuit::x31);
    REQUIRE(value == 0x01F7A3B3);

    as.RewindBuffer();

    as.SLT(biscuit::x31, biscuit::x31, biscuit::x31);
    REQUIRE(value == 0x01FFAFB3);

    as.RewindBuffer();

    as.SLT(biscuit::x0, biscuit::x0, biscuit::x0);
    REQUIRE(value == 0x00002033);
}

TEST_CASE("SLTI", "[rv32i]") {
    uint32_t value = 0;
    biscuit::Assembler as(reinterpret_cast<uint8_t*>(&value), sizeof(value));

    as.SLTI(biscuit::x15, biscuit::x31, 1024);
    REQUIRE(value == 0x400FA793);

    as.RewindBuffer();

    as.SLTI(biscuit::x15, biscuit::x31, 2048);
    REQUIRE(value == 0x800FA793);

    as.RewindBuffer();

    as.SLTI(biscuit::x15, biscuit::x31, 4095);
    REQUIRE(value == 0xFFFFA793);
}

TEST_CASE("SLTIU", "[rv32i]") {
    uint32_t value = 0;
    biscuit::Assembler as(reinterpret_cast<uint8_t*>(&value), sizeof(value));

    as.SLTIU(biscuit::x15, biscuit::x31, 1024);
    REQUIRE(value == 0x400FB793);

    as.RewindBuffer();

    as.SLTIU(biscuit::x15, biscuit::x31, 2048);
    REQUIRE(value == 0x800FB793);

    as.RewindBuffer();

    as.SLTIU(biscuit::x15, biscuit::x31, 4095);
    REQUIRE(value == 0xFFFFB793);
}

TEST_CASE("SLTU", "[rv32i]") {
    uint32_t value = 0;
    biscuit::Assembler as(reinterpret_cast<uint8_t*>(&value), sizeof(value));

    as.SLTU(biscuit::x7, biscuit::x15, biscuit::x31);
    REQUIRE(value == 0x01F7B3B3);

    as.RewindBuffer();

    as.SLTU(biscuit::x31, biscuit::x31, biscuit::x31);
    REQUIRE(value == 0x01FFBFB3);

    as.RewindBuffer();

    as.SLTU(biscuit::x0, biscuit::x0, biscuit::x0);
    REQUIRE(value == 0x00003033);
}

TEST_CASE("SRAI", "[rv32i]") {
    uint32_t value = 0;
    biscuit::Assembler as(reinterpret_cast<uint8_t*>(&value), sizeof(value));

    as.SRAI(biscuit::x31, biscuit::x15, 10);
    REQUIRE(value == 0x40A7DF93);

    as.RewindBuffer();

    as.SRAI(biscuit::x31, biscuit::x15, 20);
    REQUIRE(value == 0x4147DF93);

    as.RewindBuffer();

    as.SRAI(biscuit::x31, biscuit::x15, 31);
    REQUIRE(value == 0x41F7DF93);
}

TEST_CASE("SRLI", "[rv32i]") {
    uint32_t value = 0;
    biscuit::Assembler as(reinterpret_cast<uint8_t*>(&value), sizeof(value));

    as.SRLI(biscuit::x31, biscuit::x15, 10);
    REQUIRE(value == 0x00A7DF93);

    as.RewindBuffer();

    as.SRLI(biscuit::x31, biscuit::x15, 20);
    REQUIRE(value == 0x0147DF93);

    as.RewindBuffer();

    as.SRLI(biscuit::x31, biscuit::x15, 31);
    REQUIRE(value == 0x01F7DF93);
}

TEST_CASE("SUB", "[rv32i]") {
    uint32_t value = 0;
    biscuit::Assembler as(reinterpret_cast<uint8_t*>(&value), sizeof(value));

    as.SUB(biscuit::x7, biscuit::x15, biscuit::x31);
    REQUIRE(value == 0x41F783B3);

    as.RewindBuffer();

    as.SUB(biscuit::x31, biscuit::x31, biscuit::x31);
    REQUIRE(value == 0x41FF8FB3);

    as.RewindBuffer();

    as.SUB(biscuit::x0, biscuit::x0, biscuit::x0);
    REQUIRE(value == 0x40000033);
}

TEST_CASE("SW", "[rv32i]") {
    uint32_t value = 0;
    biscuit::Assembler as(reinterpret_cast<uint8_t*>(&value), sizeof(value));

    as.SW(biscuit::x31, 1024, biscuit::x15);
    REQUIRE(value == 0x41F7A023);

    as.RewindBuffer();

    as.SW(biscuit::x31, 2048, biscuit::x15);
    REQUIRE(value == 0x81F7A023);

    as.RewindBuffer();

    as.SW(biscuit::x31, 4095, biscuit::x15);
    REQUIRE(value == 0xFFF7AFA3);
}

TEST_CASE("XOR", "[rv32i]") {
    uint32_t value = 0;
    biscuit::Assembler as(reinterpret_cast<uint8_t*>(&value), sizeof(value));

    as.XOR(biscuit::x7, biscuit::x15, biscuit::x31);
    REQUIRE(value == 0x01F7C3B3);

    as.RewindBuffer();

    as.XOR(biscuit::x31, biscuit::x31, biscuit::x31);
    REQUIRE(value == 0x01FFCFB3);

    as.RewindBuffer();

    as.XOR(biscuit::x0, biscuit::x0, biscuit::x0);
    REQUIRE(value == 0x00004033);
}

TEST_CASE("XORI", "[rv32i]") {
    uint32_t value = 0;
    biscuit::Assembler as(reinterpret_cast<uint8_t*>(&value), sizeof(value));

    as.XORI(biscuit::x15, biscuit::x31, 1024);
    REQUIRE(value == 0x400FC793);

    as.RewindBuffer();

    as.XORI(biscuit::x15, biscuit::x31, 2048);
    REQUIRE(value == 0x800FC793);

    as.RewindBuffer();

    as.XORI(biscuit::x15, biscuit::x31, 4095);
    REQUIRE(value == 0xFFFFC793);
}
