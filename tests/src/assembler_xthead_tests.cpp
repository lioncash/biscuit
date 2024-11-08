#include <catch/catch.hpp>

#include <biscuit/assembler.hpp>

#include "assembler_test_utils.hpp"

using namespace biscuit;

TEST_CASE("TH.MVEQZ", "[XThead]") {
    uint32_t value = 0;
    auto as = MakeAssembler64(value);

    as.TH_MVEQZ(x31, x30, x29);
    REQUIRE(value == 0x41DF1F8B);

    as.RewindBuffer();

    as.TH_MVEQZ(x1, x2, x3);
    REQUIRE(value == 0x4031108B);
}

TEST_CASE("TH.MVNEZ", "[XThead]") {
    uint32_t value = 0;
    auto as = MakeAssembler64(value);

    as.TH_MVNEZ(x31, x30, x29);
    REQUIRE(value == 0x43DF1F8B);

    as.RewindBuffer();

    as.TH_MVNEZ(x1, x2, x3);
    REQUIRE(value == 0x4231108B);
}
