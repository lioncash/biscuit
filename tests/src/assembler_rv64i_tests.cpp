#include <catch/catch.hpp>

#include <biscuit/assembler.hpp>

using namespace biscuit;

TEST_CASE("ADDIW", "[rv64i]") {
    uint32_t value = 0;
    Assembler as(reinterpret_cast<uint8_t*>(&value), sizeof(value));

    as.ADDIW(x31, x15, 1024);
    REQUIRE(value == 0x40078F9B);

    as.RewindBuffer();

    as.ADDIW(x31, x15, 2048);
    REQUIRE(value == 0x80078F9B);

    as.RewindBuffer();

    as.ADDIW(x31, x15, 4095);
    REQUIRE(value == 0xFFF78F9B);
}

TEST_CASE("ADDW", "[rv64i]") {
    uint32_t value = 0;
    Assembler as(reinterpret_cast<uint8_t*>(&value), sizeof(value));

    as.ADDW(x7, x15, x31);
    REQUIRE(value == 0x01F783BB);

    as.RewindBuffer();

    as.ADDW(x31, x31, x31);
    REQUIRE(value == 0x01FF8FBB);

    as.RewindBuffer();

    as.ADDW(x0, x0, x0);
    REQUIRE(value == 0x0000003B);
}

TEST_CASE("LWU", "[rv64i]") {
    uint32_t value = 0;
    Assembler as(reinterpret_cast<uint8_t*>(&value), sizeof(value));

    as.LWU(x15, 1024, x31);
    REQUIRE(value == 0x400FE783);

    as.RewindBuffer();

    as.LWU(x15, 1536, x31);
    REQUIRE(value == 0x600FE783);

    as.RewindBuffer();

    as.LWU(x15, -1, x31);
    REQUIRE(value == 0xFFFFE783);
}

TEST_CASE("LD", "[rv64i]") {
    uint32_t value = 0;
    Assembler as(reinterpret_cast<uint8_t*>(&value), sizeof(value));

    as.LD(x15, 1024, x31);
    REQUIRE(value == 0x400FB783);

    as.RewindBuffer();

    as.LD(x15, 1536, x31);
    REQUIRE(value == 0x600FB783);

    as.RewindBuffer();

    as.LD(x15, -1, x31);
    REQUIRE(value == 0xFFFFB783);
}

TEST_CASE("SD", "[rv64i]") {
    uint32_t value = 0;
    Assembler as(reinterpret_cast<uint8_t*>(&value), sizeof(value));

    as.SD(x15, 1024, x31);
    REQUIRE(value == 0x40FFB023);

    as.RewindBuffer();

    as.SD(x15, 1536, x31);
    REQUIRE(value == 0x60FFB023);

    as.RewindBuffer();

    as.SD(x15, -1, x31);
    REQUIRE(value == 0xFEFFBFA3);
}

TEST_CASE("SLLI64", "[rv64i]") {
    uint32_t value = 0;
    Assembler as(reinterpret_cast<uint8_t*>(&value), sizeof(value));

    as.SLLI64(x31, x15, 10);
    REQUIRE(value == 0x00A79F93);

    as.RewindBuffer();

    as.SLLI64(x31, x15, 20);
    REQUIRE(value == 0x01479F93);

    as.RewindBuffer();

    as.SLLI64(x31, x15, 31);
    REQUIRE(value == 0x01F79F93);

    as.RewindBuffer();

    as.SLLI64(x31, x15, 63);
    REQUIRE(value == 0x03F79F93);
}

TEST_CASE("SLLIW", "[rv64i]") {
    uint32_t value = 0;
    Assembler as(reinterpret_cast<uint8_t*>(&value), sizeof(value));

    as.SLLIW(x31, x15, 10);
    REQUIRE(value == 0x00A79F9B);

    as.RewindBuffer();

    as.SLLIW(x31, x15, 20);
    REQUIRE(value == 0x01479F9B);

    as.RewindBuffer();

    as.SLLIW(x31, x15, 31);
    REQUIRE(value == 0x01F79F9B);
}

TEST_CASE("SLLW", "[rv64i]") {
    uint32_t value = 0;
    Assembler as(reinterpret_cast<uint8_t*>(&value), sizeof(value));

    as.SLLW(x7, x15, x31);
    REQUIRE(value == 0x01F793BB);

    as.RewindBuffer();

    as.SLLW(x31, x31, x31);
    REQUIRE(value == 0x01FF9FBB);

    as.RewindBuffer();

    as.SLLW(x0, x0, x0);
    REQUIRE(value == 0x0000103B);
}

TEST_CASE("SRAI64", "[rv64i]") {
    uint32_t value = 0;
    Assembler as(reinterpret_cast<uint8_t*>(&value), sizeof(value));

    as.SRAI64(x31, x15, 10);
    REQUIRE(value == 0x40A7DF93);

    as.RewindBuffer();

    as.SRAI64(x31, x15, 20);
    REQUIRE(value == 0x4147DF93);

    as.RewindBuffer();

    as.SRAI64(x31, x15, 31);
    REQUIRE(value == 0x41F7DF93);

    as.RewindBuffer();

    as.SRAI64(x31, x15, 63);
    REQUIRE(value == 0x43F7DF93);
}

TEST_CASE("SRAIW", "[rv64i]") {
    uint32_t value = 0;
    Assembler as(reinterpret_cast<uint8_t*>(&value), sizeof(value));

    as.SRAIW(x31, x15, 10);
    REQUIRE(value == 0x40A7DF9B);

    as.RewindBuffer();

    as.SRAIW(x31, x15, 20);
    REQUIRE(value == 0x4147DF9B);

    as.RewindBuffer();

    as.SRAIW(x31, x15, 31);
    REQUIRE(value == 0x41F7DF9B);
}

TEST_CASE("SRAW", "[rv64i]") {
    uint32_t value = 0;
    Assembler as(reinterpret_cast<uint8_t*>(&value), sizeof(value));

    as.SRAW(x7, x15, x31);
    REQUIRE(value == 0x41F7D3BB);

    as.RewindBuffer();

    as.SRAW(x31, x31, x31);
    REQUIRE(value == 0x41FFDFBB);

    as.RewindBuffer();

    as.SRAW(x0, x0, x0);
    REQUIRE(value == 0x4000503B);
}

TEST_CASE("SRLI64", "[rv64i]") {
    uint32_t value = 0;
    Assembler as(reinterpret_cast<uint8_t*>(&value), sizeof(value));

    as.SRLI64(x31, x15, 10);
    REQUIRE(value == 0x00A7DF93);

    as.RewindBuffer();

    as.SRLI64(x31, x15, 20);
    REQUIRE(value == 0x0147DF93);

    as.RewindBuffer();

    as.SRLI64(x31, x15, 31);
    REQUIRE(value == 0x01F7DF93);

    as.RewindBuffer();

    as.SRLI64(x31, x15, 63);
    REQUIRE(value == 0x03F7DF93);
}

TEST_CASE("SRLIW", "[rv64i]") {
    uint32_t value = 0;
    Assembler as(reinterpret_cast<uint8_t*>(&value), sizeof(value));

    as.SRLIW(x31, x15, 10);
    REQUIRE(value == 0x00A7DF9B);

    as.RewindBuffer();

    as.SRLIW(x31, x15, 20);
    REQUIRE(value == 0x0147DF9B);

    as.RewindBuffer();

    as.SRLIW(x31, x15, 31);
    REQUIRE(value == 0x01F7DF9B);
}

TEST_CASE("SRLW", "[rv64i]") {
    uint32_t value = 0;
    Assembler as(reinterpret_cast<uint8_t*>(&value), sizeof(value));

    as.SRLW(x7, x15, x31);
    REQUIRE(value == 0x01F7D3BB);

    as.RewindBuffer();

    as.SRLW(x31, x31, x31);
    REQUIRE(value == 0x01FFDFBB);

    as.RewindBuffer();

    as.SRLW(x0, x0, x0);
    REQUIRE(value == 0x0000503B);
}

TEST_CASE("SUBW", "[rv64i]") {
    uint32_t value = 0;
    Assembler as(reinterpret_cast<uint8_t*>(&value), sizeof(value));

    as.SUBW(x7, x15, x31);
    REQUIRE(value == 0x41F783BB);

    as.RewindBuffer();

    as.SUBW(x31, x31, x31);
    REQUIRE(value == 0x41FF8FBB);

    as.RewindBuffer();

    as.SUBW(x0, x0, x0);
    REQUIRE(value == 0x4000003B);
}
