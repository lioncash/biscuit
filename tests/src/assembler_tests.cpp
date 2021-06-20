#include <catch/catch.hpp>

#include <biscuit/assembler.hpp>

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
