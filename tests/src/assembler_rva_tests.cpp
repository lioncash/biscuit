#include <catch/catch.hpp>

#include <biscuit/assembler.hpp>

using namespace biscuit;

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
