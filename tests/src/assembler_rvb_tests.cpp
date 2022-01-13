#include <catch/catch.hpp>

#include <biscuit/assembler.hpp>

using namespace biscuit;

TEST_CASE("ADD.UW", "[rvb]") {
    uint32_t value = 0;
    Assembler as(reinterpret_cast<uint8_t*>(&value), sizeof(value));

    as.ADDUW(x31, x7, x15);
    REQUIRE(value == 0x08F38FBB);

    as.RewindBuffer();

    // Pseudo instruction

    as.ZEXTW(x31, x7);
    REQUIRE(value == 0x08038FBB);
}

TEST_CASE("ANDN", "[rvb]") {
    uint32_t value = 0;
    Assembler as(reinterpret_cast<uint8_t*>(&value), sizeof(value));

    as.ANDN(x31, x7, x15);
    REQUIRE(value == 0x40F3FFB3);
}

TEST_CASE("BCLR", "[rvb]") {
    uint32_t value = 0;
    Assembler as(reinterpret_cast<uint8_t*>(&value), sizeof(value));

    as.BCLR(x31, x7, x15);
    REQUIRE(value == 0x48F39FB3);
}

TEST_CASE("BCLRI", "[rvb]") {
    uint32_t value = 0;
    Assembler as(reinterpret_cast<uint8_t*>(&value), sizeof(value));

    as.BCLRI(x31, x7, 0);
    REQUIRE(value == 0x48039F93);

    as.RewindBuffer();

    as.BCLRI(x31, x7, 63);
    REQUIRE(value == 0x4BF39F93);
}

TEST_CASE("BEXT", "[rvb]") {
    uint32_t value = 0;
    Assembler as(reinterpret_cast<uint8_t*>(&value), sizeof(value));

    as.BEXT(x31, x7, x15);
    REQUIRE(value == 0x48F3DFB3);
}

TEST_CASE("BEXTI", "[rvb]") {
    uint32_t value = 0;
    Assembler as(reinterpret_cast<uint8_t*>(&value), sizeof(value));

    as.BEXTI(x31, x7, 0);
    REQUIRE(value == 0x4803DF93);

    as.RewindBuffer();

    as.BEXTI(x31, x7, 63);
    REQUIRE(value == 0x4BF3DF93);
}

TEST_CASE("BINV", "[rvb]") {
    uint32_t value = 0;
    Assembler as(reinterpret_cast<uint8_t*>(&value), sizeof(value));

    as.BINV(x31, x7, x15);
    REQUIRE(value == 0x68F39FB3);
}

TEST_CASE("BINVI", "[rvb]") {
    uint32_t value = 0;
    Assembler as(reinterpret_cast<uint8_t*>(&value), sizeof(value));

    as.BINVI(x31, x7, 0);
    REQUIRE(value == 0x68039F93);

    as.RewindBuffer();

    as.BINVI(x31, x7, 63);
    REQUIRE(value == 0x6BF39F93);
}

TEST_CASE("BSET", "[rvb]") {
    uint32_t value = 0;
    Assembler as(reinterpret_cast<uint8_t*>(&value), sizeof(value));

    as.BSET(x31, x7, x15);
    REQUIRE(value == 0x28F39FB3);
}

TEST_CASE("BSETI", "[rvb]") {
    uint32_t value = 0;
    Assembler as(reinterpret_cast<uint8_t*>(&value), sizeof(value));

    as.BSETI(x31, x7, 0);
    REQUIRE(value == 0x28039FB3);

    as.RewindBuffer();

    as.BSETI(x31, x7, 63);
    REQUIRE(value == 0x2BF39FB3);
}

TEST_CASE("CLMUL", "[rvb]") {
    uint32_t value = 0;
    Assembler as(reinterpret_cast<uint8_t*>(&value), sizeof(value));

    as.CLMUL(x31, x7, x15);
    REQUIRE(value == 0x0AF39FB3);
}

TEST_CASE("CLMULH", "[rvb]") {
    uint32_t value = 0;
    Assembler as(reinterpret_cast<uint8_t*>(&value), sizeof(value));

    as.CLMULH(x31, x7, x15);
    REQUIRE(value == 0x0AF3BFB3);
}

TEST_CASE("CLMULR", "[rvb]") {
    uint32_t value = 0;
    Assembler as(reinterpret_cast<uint8_t*>(&value), sizeof(value));

    as.CLMULR(x31, x7, x15);
    REQUIRE(value == 0x0AF3AFB3);
}

TEST_CASE("CLZ", "[rvb]") {
    uint32_t value = 0;
    Assembler as(reinterpret_cast<uint8_t*>(&value), sizeof(value));

    as.CLZ(x31, x7);
    REQUIRE(value == 0x60039F93);
}

TEST_CASE("CLZW", "[rvb]") {
    uint32_t value = 0;
    Assembler as(reinterpret_cast<uint8_t*>(&value), sizeof(value));

    as.CLZW(x31, x7);
    REQUIRE(value == 0x60039F9B);
}

TEST_CASE("CPOP", "[rvb]") {
    uint32_t value = 0;
    Assembler as(reinterpret_cast<uint8_t*>(&value), sizeof(value));

    as.CPOP(x31, x7);
    REQUIRE(value == 0x60239F93);
}

TEST_CASE("CPOPW", "[rvb]") {
    uint32_t value = 0;
    Assembler as(reinterpret_cast<uint8_t*>(&value), sizeof(value));

    as.CPOPW(x31, x7);
    REQUIRE(value == 0x60239F9B);
}

TEST_CASE("CTZ", "[rvb]") {
    uint32_t value = 0;
    Assembler as(reinterpret_cast<uint8_t*>(&value), sizeof(value));

    as.CTZ(x31, x7);
    REQUIRE(value == 0x60139F93);
}

TEST_CASE("CTZW", "[rvb]") {
    uint32_t value = 0;
    Assembler as(reinterpret_cast<uint8_t*>(&value), sizeof(value));

    as.CTZW(x31, x7);
    REQUIRE(value == 0x60139F9B);
}

TEST_CASE("MAX", "[rvb]") {
    uint32_t value = 0;
    Assembler as(reinterpret_cast<uint8_t*>(&value), sizeof(value));

    as.MAX(x31, x7, x15);
    REQUIRE(value == 0x0AF3EFB3);
}

TEST_CASE("MAXU", "[rvb]") {
    uint32_t value = 0;
    Assembler as(reinterpret_cast<uint8_t*>(&value), sizeof(value));

    as.MAXU(x31, x7, x15);
    REQUIRE(value == 0x0AF3FFB3);
}

TEST_CASE("MIN", "[rvb]") {
    uint32_t value = 0;
    Assembler as(reinterpret_cast<uint8_t*>(&value), sizeof(value));

    as.MIN(x31, x7, x15);
    REQUIRE(value == 0x0AF3CFB3);
}

TEST_CASE("MINU", "[rvb]") {
    uint32_t value = 0;
    Assembler as(reinterpret_cast<uint8_t*>(&value), sizeof(value));

    as.MINU(x31, x7, x15);
    REQUIRE(value == 0x0AF3DFB3);
}

TEST_CASE("ORC.B", "[rvb]") {
    uint32_t value = 0;
    Assembler as(reinterpret_cast<uint8_t*>(&value), sizeof(value));

    as.ORCB(x31, x7);
    REQUIRE(value == 0x2873DF93);
}

TEST_CASE("ORN", "[rvb]") {
    uint32_t value = 0;
    Assembler as(reinterpret_cast<uint8_t*>(&value), sizeof(value));

    as.ORN(x31, x7, x15);
    REQUIRE(value == 0x40F3EFB3);
}

TEST_CASE("PACK", "[rvb]") {
    uint32_t value = 0;
    Assembler as(reinterpret_cast<uint8_t*>(&value), sizeof(value));

    as.PACK(x31, x7, x2);
    REQUIRE(value == 0x0823CFB3);
}

TEST_CASE("REV8", "[rvb]") {
    uint32_t value = 0;
    Assembler as(reinterpret_cast<uint8_t*>(&value), sizeof(value));

    as.REV8_32(x31, x7);
    REQUIRE(value == 0x6983DF93);

    as.RewindBuffer();

    as.REV8_64(x31, x7);
    REQUIRE(value == 0x6B83DF93);
}

TEST_CASE("ROL", "[rvb]") {
    uint32_t value = 0;
    Assembler as(reinterpret_cast<uint8_t*>(&value), sizeof(value));

    as.ROL(x31, x7, x15);
    REQUIRE(value == 0x60F39FB3);
}

TEST_CASE("ROLW", "[rvb]") {
    uint32_t value = 0;
    Assembler as(reinterpret_cast<uint8_t*>(&value), sizeof(value));

    as.ROLW(x31, x7, x15);
    REQUIRE(value == 0x60F39FBB);
}

TEST_CASE("ROR", "[rvb]") {
    uint32_t value = 0;
    Assembler as(reinterpret_cast<uint8_t*>(&value), sizeof(value));

    as.ROR(x31, x7, x15);
    REQUIRE(value == 0x60F3DFB3);
}

TEST_CASE("RORW", "[rvb]") {
    uint32_t value = 0;
    Assembler as(reinterpret_cast<uint8_t*>(&value), sizeof(value));

    as.RORW(x31, x7, x15);
    REQUIRE(value == 0x60F3DFBB);
}

TEST_CASE("RORI", "[rvb]") {
    uint32_t value = 0;
    Assembler as(reinterpret_cast<uint8_t*>(&value), sizeof(value));

    as.RORI(x31, x7, 0);
    REQUIRE(value == 0x6003DF93);

    as.RewindBuffer();

    as.RORI(x31, x7, 63);
    REQUIRE(value == 0x63F3DF93);
}

TEST_CASE("RORIW", "[rvb]") {
    uint32_t value = 0;
    Assembler as(reinterpret_cast<uint8_t*>(&value), sizeof(value));

    as.RORIW(x31, x7, 0);
    REQUIRE(value == 0x6003DF9B);

    as.RewindBuffer();

    as.RORIW(x31, x7, 63);
    REQUIRE(value == 0x63F3DF9B);
}

TEST_CASE("SEXT.B", "[rvb]") {
    uint32_t value = 0;
    Assembler as(reinterpret_cast<uint8_t*>(&value), sizeof(value));

    as.SEXTB(x31, x7);
    REQUIRE(value == 0x60439F93);
}

TEST_CASE("SEXT.H", "[rvb]") {
    uint32_t value = 0;
    Assembler as(reinterpret_cast<uint8_t*>(&value), sizeof(value));

    as.SEXTH(x31, x7);
    REQUIRE(value == 0x60539F93);
}

TEST_CASE("SH1ADD", "[rvb]") {
    uint32_t value = 0;
    Assembler as(reinterpret_cast<uint8_t*>(&value), sizeof(value));

    as.SH1ADD(x31, x7, x15);
    REQUIRE(value == 0x20F3AFB3);
}

TEST_CASE("SH1ADD.UW", "[rvb]") {
    uint32_t value = 0;
    Assembler as(reinterpret_cast<uint8_t*>(&value), sizeof(value));

    as.SH1ADDUW(x31, x7, x15);
    REQUIRE(value == 0x20F3AFBB);
}

TEST_CASE("SH2ADD", "[rvb]") {
    uint32_t value = 0;
    Assembler as(reinterpret_cast<uint8_t*>(&value), sizeof(value));

    as.SH2ADD(x31, x7, x15);
    REQUIRE(value == 0x20F3CFB3);
}

TEST_CASE("SH2ADD.UW", "[rvb]") {
    uint32_t value = 0;
    Assembler as(reinterpret_cast<uint8_t*>(&value), sizeof(value));

    as.SH2ADDUW(x31, x7, x15);
    REQUIRE(value == 0x20F3CFBB);
}

TEST_CASE("SH3ADD", "[rvb]") {
    uint32_t value = 0;
    Assembler as(reinterpret_cast<uint8_t*>(&value), sizeof(value));

    as.SH3ADD(x31, x7, x15);
    REQUIRE(value == 0x20F3EFB3);
}

TEST_CASE("SH3ADD.UW", "[rvb]") {
    uint32_t value = 0;
    Assembler as(reinterpret_cast<uint8_t*>(&value), sizeof(value));

    as.SH3ADDUW(x31, x7, x15);
    REQUIRE(value == 0x20F3EFBB);
}

TEST_CASE("SLLI.UW", "[rvb]") {
    uint32_t value = 0;
    Assembler as(reinterpret_cast<uint8_t*>(&value), sizeof(value));

    as.SLLIUW(x31, x7, 0);
    REQUIRE(value == 0x08039F9B);

    as.RewindBuffer();

    as.SLLIUW(x31, x7, 63);
    REQUIRE(value == 0x0BF39F9B);
}

TEST_CASE("XNOR", "[rvb]") {
    uint32_t value = 0;
    Assembler as(reinterpret_cast<uint8_t*>(&value), sizeof(value));

    as.XNOR(x31, x7, x15);
    REQUIRE(value == 0x40F3CFB3);
}

TEST_CASE("ZEXT.H", "[rvb]") {
    uint32_t value = 0;
    Assembler as(reinterpret_cast<uint8_t*>(&value), sizeof(value));

    as.ZEXTH_32(x31, x7);
    REQUIRE(value == 0x0803CFB3);

    as.RewindBuffer();

    as.ZEXTH_64(x31, x7);
    REQUIRE(value == 0x0803CFBB);
}
