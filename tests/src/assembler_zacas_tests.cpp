#include <catch/catch.hpp>

#include <biscuit/assembler.hpp>

using namespace biscuit;

TEST_CASE("AMOCAS.D", "[Zacas]") {
    uint32_t value = 0;
    Assembler as(reinterpret_cast<uint8_t*>(&value), sizeof(value));

    as.AMOCAS_D(Ordering::None, x31, x7, x15);
    REQUIRE(value == 0x2877BFAF);

    as.RewindBuffer();

    as.AMOCAS_D(Ordering::AQ, x31, x7, x15);
    REQUIRE(value == 0x2C77BFAF);

    as.RewindBuffer();

    as.AMOCAS_D(Ordering::RL, x31, x7, x15);
    REQUIRE(value == 0x2A77BFAF);

    as.RewindBuffer();

    as.AMOCAS_D(Ordering::AQRL, x31, x7, x15);
    REQUIRE(value == 0x2E77BFAF);
}

TEST_CASE("AMOCAS.Q", "[Zacas]") {
    uint32_t value = 0;
    Assembler as(reinterpret_cast<uint8_t*>(&value), sizeof(value));

    as.AMOCAS_Q(Ordering::None, x30, x6, x15);
    REQUIRE(value == 0x2867CF2F);

    as.RewindBuffer();

    as.AMOCAS_Q(Ordering::AQ, x30, x6, x15);
    REQUIRE(value == 0x2C67CF2F);

    as.RewindBuffer();

    as.AMOCAS_Q(Ordering::RL, x30, x6, x15);
    REQUIRE(value == 0x2A67CF2F);

    as.RewindBuffer();

    as.AMOCAS_Q(Ordering::AQRL, x30, x6, x15);
    REQUIRE(value == 0x2E67CF2F);
}

TEST_CASE("AMOCAS.W", "[Zacas]") {
    uint32_t value = 0;
    Assembler as(reinterpret_cast<uint8_t*>(&value), sizeof(value));

    as.AMOCAS_W(Ordering::None, x31, x7, x15);
    REQUIRE(value == 0x2877AFAF);

    as.RewindBuffer();

    as.AMOCAS_W(Ordering::AQ, x31, x7, x15);
    REQUIRE(value == 0x2C77AFAF);

    as.RewindBuffer();

    as.AMOCAS_W(Ordering::RL, x31, x7, x15);
    REQUIRE(value == 0x2A77AFAF);

    as.RewindBuffer();

    as.AMOCAS_W(Ordering::AQRL, x31, x7, x15);
    REQUIRE(value == 0x2E77AFAF);
}
