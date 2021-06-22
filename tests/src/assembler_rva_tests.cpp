#include <catch/catch.hpp>

#include <biscuit/assembler.hpp>

using namespace biscuit;

TEST_CASE("AMOADD.D", "[rv64a]") {
    uint32_t value = 0;
    Assembler as(reinterpret_cast<uint8_t*>(&value), sizeof(value));

    as.AMOADD_D(Assembler::Ordering::None, x31, x7, x15);
    REQUIRE(value == 0x0077BFAF);

    as.RewindBuffer();

    as.AMOADD_D(Assembler::Ordering::AQ, x31, x7, x15);
    REQUIRE(value == 0x0477BFAF);

    as.RewindBuffer();

    as.AMOADD_D(Assembler::Ordering::RL, x31, x7, x15);
    REQUIRE(value == 0x0277BFAF);

    as.RewindBuffer();

    as.AMOADD_D(Assembler::Ordering::AQRL, x31, x7, x15);
    REQUIRE(value == 0x0677BFAF);
}

TEST_CASE("AMOADD.W", "[rv32a]") {
    uint32_t value = 0;
    Assembler as(reinterpret_cast<uint8_t*>(&value), sizeof(value));

    as.AMOADD_W(Assembler::Ordering::None, x31, x7, x15);
    REQUIRE(value == 0x0077AFAF);

    as.RewindBuffer();

    as.AMOADD_W(Assembler::Ordering::AQ, x31, x7, x15);
    REQUIRE(value == 0x0477AFAF);

    as.RewindBuffer();

    as.AMOADD_W(Assembler::Ordering::RL, x31, x7, x15);
    REQUIRE(value == 0x0277AFAF);

    as.RewindBuffer();

    as.AMOADD_W(Assembler::Ordering::AQRL, x31, x7, x15);
    REQUIRE(value == 0x0677AFAF);
}

TEST_CASE("AMOAND.D", "[rv64a]") {
    uint32_t value = 0;
    Assembler as(reinterpret_cast<uint8_t*>(&value), sizeof(value));

    as.AMOAND_D(Assembler::Ordering::None, x31, x7, x15);
    REQUIRE(value == 0x6077BFAF);

    as.RewindBuffer();

    as.AMOAND_D(Assembler::Ordering::AQ, x31, x7, x15);
    REQUIRE(value == 0x6477BFAF);

    as.RewindBuffer();

    as.AMOAND_D(Assembler::Ordering::RL, x31, x7, x15);
    REQUIRE(value == 0x6277BFAF);

    as.RewindBuffer();

    as.AMOAND_D(Assembler::Ordering::AQRL, x31, x7, x15);
    REQUIRE(value == 0x6677BFAF);
}

TEST_CASE("AMOAND.W", "[rv32a]") {
    uint32_t value = 0;
    Assembler as(reinterpret_cast<uint8_t*>(&value), sizeof(value));

    as.AMOAND_W(Assembler::Ordering::None, x31, x7, x15);
    REQUIRE(value == 0x6077AFAF);

    as.RewindBuffer();

    as.AMOAND_W(Assembler::Ordering::AQ, x31, x7, x15);
    REQUIRE(value == 0x6477AFAF);

    as.RewindBuffer();

    as.AMOAND_W(Assembler::Ordering::RL, x31, x7, x15);
    REQUIRE(value == 0x6277AFAF);

    as.RewindBuffer();

    as.AMOAND_W(Assembler::Ordering::AQRL, x31, x7, x15);
    REQUIRE(value == 0x6677AFAF);
}

TEST_CASE("AMOSWAP.D", "[rv64a]") {
    uint32_t value = 0;
    Assembler as(reinterpret_cast<uint8_t*>(&value), sizeof(value));

    as.AMOSWAP_D(Assembler::Ordering::None, x31, x7, x15);
    REQUIRE(value == 0x0877BFAF);

    as.RewindBuffer();

    as.AMOSWAP_D(Assembler::Ordering::AQ, x31, x7, x15);
    REQUIRE(value == 0x0C77BFAF);

    as.RewindBuffer();

    as.AMOSWAP_D(Assembler::Ordering::RL, x31, x7, x15);
    REQUIRE(value == 0x0A77BFAF);

    as.RewindBuffer();

    as.AMOSWAP_D(Assembler::Ordering::AQRL, x31, x7, x15);
    REQUIRE(value == 0x0E77BFAF);
}

TEST_CASE("AMOSWAP.W", "[rv32a]") {
    uint32_t value = 0;
    Assembler as(reinterpret_cast<uint8_t*>(&value), sizeof(value));

    as.AMOSWAP_W(Assembler::Ordering::None, x31, x7, x15);
    REQUIRE(value == 0x0877AFAF);

    as.RewindBuffer();

    as.AMOSWAP_W(Assembler::Ordering::AQ, x31, x7, x15);
    REQUIRE(value == 0x0C77AFAF);

    as.RewindBuffer();

    as.AMOSWAP_W(Assembler::Ordering::RL, x31, x7, x15);
    REQUIRE(value == 0x0A77AFAF);

    as.RewindBuffer();

    as.AMOSWAP_W(Assembler::Ordering::AQRL, x31, x7, x15);
    REQUIRE(value == 0x0E77AFAF);
}

TEST_CASE("AMOXOR.D", "[rv64a]") {
    uint32_t value = 0;
    Assembler as(reinterpret_cast<uint8_t*>(&value), sizeof(value));

    as.AMOXOR_D(Assembler::Ordering::None, x31, x7, x15);
    REQUIRE(value == 0x2077BFAF);

    as.RewindBuffer();

    as.AMOXOR_D(Assembler::Ordering::AQ, x31, x7, x15);
    REQUIRE(value == 0x2477BFAF);

    as.RewindBuffer();

    as.AMOXOR_D(Assembler::Ordering::RL, x31, x7, x15);
    REQUIRE(value == 0x2277BFAF);

    as.RewindBuffer();

    as.AMOXOR_D(Assembler::Ordering::AQRL, x31, x7, x15);
    REQUIRE(value == 0x2677BFAF);
}

TEST_CASE("AMOXOR.W", "[rv32a]") {
    uint32_t value = 0;
    Assembler as(reinterpret_cast<uint8_t*>(&value), sizeof(value));

    as.AMOXOR_W(Assembler::Ordering::None, x31, x7, x15);
    REQUIRE(value == 0x2077AFAF);

    as.RewindBuffer();

    as.AMOXOR_W(Assembler::Ordering::AQ, x31, x7, x15);
    REQUIRE(value == 0x2477AFAF);

    as.RewindBuffer();

    as.AMOXOR_W(Assembler::Ordering::RL, x31, x7, x15);
    REQUIRE(value == 0x2277AFAF);

    as.RewindBuffer();

    as.AMOXOR_W(Assembler::Ordering::AQRL, x31, x7, x15);
    REQUIRE(value == 0x2677AFAF);
}

TEST_CASE("LR.D", "[rv64a]") {
    uint32_t value = 0;
    Assembler as(reinterpret_cast<uint8_t*>(&value), sizeof(value));

    as.LR_D(Assembler::Ordering::None, x31, x15);
    REQUIRE(value == 0x1007BFAF);

    as.RewindBuffer();

    as.LR_D(Assembler::Ordering::AQ, x31, x15);
    REQUIRE(value == 0x1407BFAF);

    as.RewindBuffer();

    as.LR_D(Assembler::Ordering::RL, x31, x15);
    REQUIRE(value == 0x1207BFAF);

    as.RewindBuffer();

    as.LR_D(Assembler::Ordering::AQRL, x31, x15);
    REQUIRE(value == 0x1607BFAF);
}

TEST_CASE("LR.W", "[rv32a]") {
    uint32_t value = 0;
    Assembler as(reinterpret_cast<uint8_t*>(&value), sizeof(value));

    as.LR_W(Assembler::Ordering::None, x31, x15);
    REQUIRE(value == 0x1007AFAF);

    as.RewindBuffer();

    as.LR_W(Assembler::Ordering::AQ, x31, x15);
    REQUIRE(value == 0x1407AFAF);

    as.RewindBuffer();

    as.LR_W(Assembler::Ordering::RL, x31, x15);
    REQUIRE(value == 0x1207AFAF);

    as.RewindBuffer();

    as.LR_W(Assembler::Ordering::AQRL, x31, x15);
    REQUIRE(value == 0x1607AFAF);
}

TEST_CASE("SC.D", "[rv64a]") {
    uint32_t value = 0;
    Assembler as(reinterpret_cast<uint8_t*>(&value), sizeof(value));

    as.SC_D(Assembler::Ordering::None, x31, x15, x7);
    REQUIRE(value == 0x1877BFAF);

    as.RewindBuffer();

    as.SC_D(Assembler::Ordering::AQ, x31, x15, x7);
    REQUIRE(value == 0x1C77BFAF);

    as.RewindBuffer();

    as.SC_D(Assembler::Ordering::RL, x31, x15, x7);
    REQUIRE(value == 0x1A77BFAF);

    as.RewindBuffer();

    as.SC_D(Assembler::Ordering::AQRL, x31, x15, x7);
    REQUIRE(value == 0x1E77BFAF);
}

TEST_CASE("SC.W", "[rv32a]") {
    uint32_t value = 0;
    Assembler as(reinterpret_cast<uint8_t*>(&value), sizeof(value));

    as.SC_W(Assembler::Ordering::None, x31, x15, x7);
    REQUIRE(value == 0x1877AFAF);

    as.RewindBuffer();

    as.SC_W(Assembler::Ordering::AQ, x31, x15, x7);
    REQUIRE(value == 0x1C77AFAF);

    as.RewindBuffer();

    as.SC_W(Assembler::Ordering::RL, x31, x15, x7);
    REQUIRE(value == 0x1A77AFAF);

    as.RewindBuffer();

    as.SC_W(Assembler::Ordering::AQRL, x31, x15, x7);
    REQUIRE(value == 0x1E77AFAF);
}
