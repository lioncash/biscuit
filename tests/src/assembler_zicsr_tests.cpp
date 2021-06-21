#include <catch/catch.hpp>

#include <biscuit/assembler.hpp>

TEST_CASE("CSRRC", "[Zicsr]") {
    using namespace biscuit;

    uint32_t value = 0;
    Assembler as(reinterpret_cast<uint8_t*>(&value), sizeof(value));

    as.CSRRC(x31, Assembler::CSR::Cycle, x15);
    REQUIRE(value == 0xC007BFF3);

    as.RewindBuffer();

    as.CSRRC(x31, Assembler::CSR::FFlags, x15);
    REQUIRE(value == 0x0017BFF3);

    as.RewindBuffer();

    as.CSRRC(x31, Assembler::CSR::FRM, x15);
    REQUIRE(value == 0x0027BFF3);

    as.RewindBuffer();

    as.CSRRC(x31, Assembler::CSR::FCSR, x15);
    REQUIRE(value == 0x0037BFF3);
}

TEST_CASE("CSRRS", "[Zicsr]") {
    using namespace biscuit;

    uint32_t value = 0;
    Assembler as(reinterpret_cast<uint8_t*>(&value), sizeof(value));

    as.CSRRS(x31, Assembler::CSR::Cycle, x15);
    REQUIRE(value == 0xC007AFF3);

    as.RewindBuffer();

    as.CSRRS(x31, Assembler::CSR::FFlags, x15);
    REQUIRE(value == 0x0017AFF3);

    as.RewindBuffer();

    as.CSRRS(x31, Assembler::CSR::FRM, x15);
    REQUIRE(value == 0x0027AFF3);

    as.RewindBuffer();

    as.CSRRS(x31, Assembler::CSR::FCSR, x15);
    REQUIRE(value == 0x0037AFF3);
}

TEST_CASE("CSRRW", "[Zicsr]") {
    using namespace biscuit;

    uint32_t value = 0;
    Assembler as(reinterpret_cast<uint8_t*>(&value), sizeof(value));

    as.CSRRW(x31, Assembler::CSR::Cycle, x15);
    REQUIRE(value == 0xC0079FF3);

    as.RewindBuffer();

    as.CSRRW(x31, Assembler::CSR::FFlags, x15);
    REQUIRE(value == 0x00179FF3);

    as.RewindBuffer();

    as.CSRRW(x31, Assembler::CSR::FRM, x15);
    REQUIRE(value == 0x00279FF3);

    as.RewindBuffer();

    as.CSRRW(x31, Assembler::CSR::FCSR, x15);
    REQUIRE(value == 0x00379FF3);
}
