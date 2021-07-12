#include <catch/catch.hpp>

#include <biscuit/assembler.hpp>

using namespace biscuit;

TEST_CASE("VSETIVLI", "[rvv]") {
    uint32_t value = 0;
    Assembler as(reinterpret_cast<uint8_t*>(&value), sizeof(value));

    as.VSETIVLI(x10, 11, SEW::E8, LMUL::M1, VTA::No, VMA::No);
    REQUIRE(value == 0xC005F557);

    as.RewindBuffer();

    as.VSETIVLI(x10, 11, SEW::E16, LMUL::M2, VTA::No, VMA::No);
    REQUIRE(value == 0xC095F557);

    as.RewindBuffer();

    as.VSETIVLI(x10, 11, SEW::E256, LMUL::M2, VTA::Yes, VMA::No);
    REQUIRE(value == 0xC695F557);

    as.RewindBuffer();

    as.VSETIVLI(x10, 11, SEW::E256, LMUL::M2, VTA::Yes, VMA::Yes);
    REQUIRE(value == 0xCE95F557);
}

TEST_CASE("VSETVL", "[rvv]") {
    uint32_t value = 0;
    Assembler as(reinterpret_cast<uint8_t*>(&value), sizeof(value));

    as.VSETVL(x10, x11, x12);
    REQUIRE(value == 0x80C5F557);
}

TEST_CASE("VSETVLI", "[rvv]") {
    uint32_t value = 0;
    Assembler as(reinterpret_cast<uint8_t*>(&value), sizeof(value));

    as.VSETVLI(x31, x6, SEW::E64, LMUL::MF2, VTA::Yes, VMA::Yes);
    REQUIRE(value == 0x0DF37FD7);

    as.RewindBuffer();

    as.VSETVLI(x31, x6, SEW::E64, LMUL::MF2, VTA::No, VMA::No);
    REQUIRE(value == 0x01F37FD7);

    as.RewindBuffer();

    as.VSETVLI(x12, x18, SEW::E8, LMUL::M1, VTA::No, VMA::No);
    REQUIRE(value == 0x00097657);

    as.RewindBuffer();

    as.VSETVLI(x15, x12, SEW::E32, LMUL::M4, VTA::No, VMA::No);
    REQUIRE(value == 0x012677D7);
}
