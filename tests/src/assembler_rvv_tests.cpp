#include <catch/catch.hpp>

#include <biscuit/assembler.hpp>

using namespace biscuit;

TEST_CASE("VADD.VV", "[rvv]") {
    uint32_t value = 0;
    Assembler as(reinterpret_cast<uint8_t*>(&value), sizeof(value));

    as.VADD(v4, v8, v12, VecMask::No);
    REQUIRE(value == 0x02860257);

    as.RewindBuffer();

    as.VADD(v4, v8, v12, VecMask::Yes);
    REQUIRE(value == 0x00860257);
}

TEST_CASE("VADD.VX", "[rvv]") {
    uint32_t value = 0;
    Assembler as(reinterpret_cast<uint8_t*>(&value), sizeof(value));

    as.VADD(v4, v8, x11, VecMask::No);
    REQUIRE(value == 0x0285C257);

    as.RewindBuffer();

    as.VADD(v4, v8, x11, VecMask::Yes);
    REQUIRE(value == 0x0085C257);
}

TEST_CASE("VADD.VI", "[rvv]") {
    uint32_t value = 0;
    Assembler as(reinterpret_cast<uint8_t*>(&value), sizeof(value));

    as.VADD(v4, v8, 15, VecMask::No);
    REQUIRE(value == 0x0287B257);

    as.RewindBuffer();

    as.VADD(v4, v8, -16, VecMask::No);
    REQUIRE(value == 0x02883257);

    as.RewindBuffer();

    as.VADD(v4, v8, 15, VecMask::Yes);
    REQUIRE(value == 0x0087B257);

    as.RewindBuffer();

    as.VADD(v4, v8, -16, VecMask::Yes);
    REQUIRE(value == 0x00883257);
}

TEST_CASE("VAND.VV", "[rvv]") {
    uint32_t value = 0;
    Assembler as(reinterpret_cast<uint8_t*>(&value), sizeof(value));

    as.VAND(v4, v8, v12, VecMask::No);
    REQUIRE(value == 0x26860257);

    as.RewindBuffer();

    as.VAND(v4, v8, v12, VecMask::Yes);
    REQUIRE(value == 0x24860257);
}

TEST_CASE("VAND.VX", "[rvv]") {
    uint32_t value = 0;
    Assembler as(reinterpret_cast<uint8_t*>(&value), sizeof(value));

    as.VAND(v4, v8, x11, VecMask::No);
    REQUIRE(value == 0x2685C257);

    as.RewindBuffer();

    as.VAND(v4, v8, x11, VecMask::Yes);
    REQUIRE(value == 0x2485C257);
}

TEST_CASE("VAND.VI", "[rvv]") {
    uint32_t value = 0;
    Assembler as(reinterpret_cast<uint8_t*>(&value), sizeof(value));

    as.VAND(v4, v8, 15, VecMask::No);
    REQUIRE(value == 0x2687B257);

    as.RewindBuffer();

    as.VAND(v4, v8, -16, VecMask::No);
    REQUIRE(value == 0x26883257);

    as.RewindBuffer();

    as.VAND(v4, v8, 15, VecMask::Yes);
    REQUIRE(value == 0x2487B257);

    as.RewindBuffer();

    as.VAND(v4, v8, -16, VecMask::Yes);
    REQUIRE(value == 0x24883257);
}

TEST_CASE("VMAX.VV", "[rvv]") {
    uint32_t value = 0;
    Assembler as(reinterpret_cast<uint8_t*>(&value), sizeof(value));

    as.VMAX(v4, v8, v12, VecMask::No);
    REQUIRE(value == 0x1E860257);

    as.RewindBuffer();

    as.VMAX(v4, v8, v12, VecMask::Yes);
    REQUIRE(value == 0x1C860257);
}

TEST_CASE("VMAX.VX", "[rvv]") {
    uint32_t value = 0;
    Assembler as(reinterpret_cast<uint8_t*>(&value), sizeof(value));

    as.VMAX(v4, v8, x11, VecMask::No);
    REQUIRE(value == 0x1E85C257);

    as.RewindBuffer();

    as.VMAX(v4, v8, x11, VecMask::Yes);
    REQUIRE(value == 0x1C85C257);
}

TEST_CASE("VMAXU.VV", "[rvv]") {
    uint32_t value = 0;
    Assembler as(reinterpret_cast<uint8_t*>(&value), sizeof(value));

    as.VMAXU(v4, v8, v12, VecMask::No);
    REQUIRE(value == 0x1A860257);

    as.RewindBuffer();

    as.VMAXU(v4, v8, v12, VecMask::Yes);
    REQUIRE(value == 0x18860257);
}

TEST_CASE("VMAXU.VX", "[rvv]") {
    uint32_t value = 0;
    Assembler as(reinterpret_cast<uint8_t*>(&value), sizeof(value));

    as.VMAXU(v4, v8, x11, VecMask::No);
    REQUIRE(value == 0x1A85C257);

    as.RewindBuffer();

    as.VMAXU(v4, v8, x11, VecMask::Yes);
    REQUIRE(value == 0x1885C257);
}

TEST_CASE("VMIN.VV", "[rvv]") {
    uint32_t value = 0;
    Assembler as(reinterpret_cast<uint8_t*>(&value), sizeof(value));

    as.VMIN(v4, v8, v12, VecMask::No);
    REQUIRE(value == 0x16860257);

    as.RewindBuffer();

    as.VMIN(v4, v8, v12, VecMask::Yes);
    REQUIRE(value == 0x14860257);
}

TEST_CASE("VMIN.VX", "[rvv]") {
    uint32_t value = 0;
    Assembler as(reinterpret_cast<uint8_t*>(&value), sizeof(value));

    as.VMIN(v4, v8, x11, VecMask::No);
    REQUIRE(value == 0x1685C257);

    as.RewindBuffer();

    as.VMIN(v4, v8, x11, VecMask::Yes);
    REQUIRE(value == 0x1485C257);
}

TEST_CASE("VMINU.VV", "[rvv]") {
    uint32_t value = 0;
    Assembler as(reinterpret_cast<uint8_t*>(&value), sizeof(value));

    as.VMINU(v4, v8, v12, VecMask::No);
    REQUIRE(value == 0x12860257);

    as.RewindBuffer();

    as.VMINU(v4, v8, v12, VecMask::Yes);
    REQUIRE(value == 0x10860257);
}

TEST_CASE("VMINU.VX", "[rvv]") {
    uint32_t value = 0;
    Assembler as(reinterpret_cast<uint8_t*>(&value), sizeof(value));

    as.VMINU(v4, v8, x11, VecMask::No);
    REQUIRE(value == 0x1285C257);

    as.RewindBuffer();

    as.VMINU(v4, v8, x11, VecMask::Yes);
    REQUIRE(value == 0x1085C257);
}

TEST_CASE("VOR.VV", "[rvv]") {
    uint32_t value = 0;
    Assembler as(reinterpret_cast<uint8_t*>(&value), sizeof(value));

    as.VOR(v4, v8, v12, VecMask::No);
    REQUIRE(value == 0x2A860257);

    as.RewindBuffer();

    as.VOR(v4, v8, v12, VecMask::Yes);
    REQUIRE(value == 0x28860257);
}

TEST_CASE("VOR.VX", "[rvv]") {
    uint32_t value = 0;
    Assembler as(reinterpret_cast<uint8_t*>(&value), sizeof(value));

    as.VOR(v4, v8, x11, VecMask::No);
    REQUIRE(value == 0x2A85C257);

    as.RewindBuffer();

    as.VOR(v4, v8, x11, VecMask::Yes);
    REQUIRE(value == 0x2885C257);
}

TEST_CASE("VOR.VI", "[rvv]") {
    uint32_t value = 0;
    Assembler as(reinterpret_cast<uint8_t*>(&value), sizeof(value));

    as.VOR(v4, v8, 15, VecMask::No);
    REQUIRE(value == 0x2A87B257);

    as.RewindBuffer();

    as.VOR(v4, v8, -16, VecMask::No);
    REQUIRE(value == 0x2A883257);

    as.RewindBuffer();

    as.VOR(v4, v8, 15, VecMask::Yes);
    REQUIRE(value == 0x2887B257);

    as.RewindBuffer();

    as.VOR(v4, v8, -16, VecMask::Yes);
    REQUIRE(value == 0x28883257);
}

TEST_CASE("VRGATHER.VV", "[rvv]") {
    uint32_t value = 0;
    Assembler as(reinterpret_cast<uint8_t*>(&value), sizeof(value));

    as.VRGATHER(v4, v8, v12, VecMask::No);
    REQUIRE(value == 0x32860257);

    as.RewindBuffer();

    as.VRGATHER(v4, v8, v12, VecMask::Yes);
    REQUIRE(value == 0x30860257);
}

TEST_CASE("VRGATHER.VX", "[rvv]") {
    uint32_t value = 0;
    Assembler as(reinterpret_cast<uint8_t*>(&value), sizeof(value));

    as.VRGATHER(v4, v8, x11, VecMask::No);
    REQUIRE(value == 0x3285C257);

    as.RewindBuffer();

    as.VRGATHER(v4, v8, x11, VecMask::Yes);
    REQUIRE(value == 0x3085C257);
}

TEST_CASE("VRGATHER.VI", "[rvv]") {
    uint32_t value = 0;
    Assembler as(reinterpret_cast<uint8_t*>(&value), sizeof(value));

    as.VRGATHER(v4, v8, 0, VecMask::No);
    REQUIRE(value == 0x32803257);

    as.RewindBuffer();

    as.VRGATHER(v4, v8, 31, VecMask::No);
    REQUIRE(value == 0x328FB257);

    as.RewindBuffer();

    as.VRGATHER(v4, v8, 0, VecMask::Yes);
    REQUIRE(value == 0x30803257);

    as.RewindBuffer();

    as.VRGATHER(v4, v8, 31, VecMask::Yes);
    REQUIRE(value == 0x308FB257);
}

TEST_CASE("VRSUB.VX", "[rvv]") {
    uint32_t value = 0;
    Assembler as(reinterpret_cast<uint8_t*>(&value), sizeof(value));

    as.VRSUB(v4, v8, x11, VecMask::No);
    REQUIRE(value == 0x0E85C257);

    as.RewindBuffer();

    as.VRSUB(v4, v8, x11, VecMask::Yes);
    REQUIRE(value == 0x0C85C257);
}

TEST_CASE("VRSUB.VI", "[rvv]") {
    uint32_t value = 0;
    Assembler as(reinterpret_cast<uint8_t*>(&value), sizeof(value));

    as.VRSUB(v4, v8, 15, VecMask::No);
    REQUIRE(value == 0x0E87B257);

    as.RewindBuffer();

    as.VRSUB(v4, v8, -16, VecMask::No);
    REQUIRE(value == 0x0E883257);

    as.RewindBuffer();

    as.VRSUB(v4, v8, 15, VecMask::Yes);
    REQUIRE(value == 0x0C87B257);

    as.RewindBuffer();

    as.VRSUB(v4, v8, -16, VecMask::Yes);
    REQUIRE(value == 0x0C883257);
}

TEST_CASE("VSLIDEUP.VX", "[rvv]") {
    uint32_t value = 0;
    Assembler as(reinterpret_cast<uint8_t*>(&value), sizeof(value));

    as.VSLIDEUP(v4, v8, x11, VecMask::No);
    REQUIRE(value == 0x3A85C257);

    as.RewindBuffer();

    as.VSLIDEUP(v4, v8, x11, VecMask::Yes);
    REQUIRE(value == 0x3885C257);
}

TEST_CASE("VSLIDEUP.VI", "[rvv]") {
    uint32_t value = 0;
    Assembler as(reinterpret_cast<uint8_t*>(&value), sizeof(value));

    as.VSLIDEUP(v4, v8, 0, VecMask::No);
    REQUIRE(value == 0x3A803257);

    as.RewindBuffer();

    as.VSLIDEUP(v4, v8, 31, VecMask::No);
    REQUIRE(value == 0x3A8FB257);

    as.RewindBuffer();

    as.VSLIDEUP(v4, v8, 0, VecMask::Yes);
    REQUIRE(value == 0x38803257);

    as.RewindBuffer();

    as.VSLIDEUP(v4, v8, 31, VecMask::Yes);
    REQUIRE(value == 0x388FB257);
}

TEST_CASE("VSUB.VV", "[rvv]") {
    uint32_t value = 0;
    Assembler as(reinterpret_cast<uint8_t*>(&value), sizeof(value));

    as.VSUB(v4, v8, v12, VecMask::No);
    REQUIRE(value == 0x0A860257);

    as.RewindBuffer();

    as.VSUB(v4, v8, v12, VecMask::Yes);
    REQUIRE(value == 0x08860257);
}

TEST_CASE("VSUB.VX", "[rvv]") {
    uint32_t value = 0;
    Assembler as(reinterpret_cast<uint8_t*>(&value), sizeof(value));

    as.VSUB(v4, v8, x11, VecMask::No);
    REQUIRE(value == 0x0A85C257);

    as.RewindBuffer();

    as.VSUB(v4, v8, x11, VecMask::Yes);
    REQUIRE(value == 0x0885C257);
}

TEST_CASE("VXOR.VV", "[rvv]") {
    uint32_t value = 0;
    Assembler as(reinterpret_cast<uint8_t*>(&value), sizeof(value));

    as.VXOR(v4, v8, v12, VecMask::No);
    REQUIRE(value == 0x2E860257);

    as.RewindBuffer();

    as.VXOR(v4, v8, v12, VecMask::Yes);
    REQUIRE(value == 0x2C860257);
}

TEST_CASE("VXOR.VX", "[rvv]") {
    uint32_t value = 0;
    Assembler as(reinterpret_cast<uint8_t*>(&value), sizeof(value));

    as.VXOR(v4, v8, x11, VecMask::No);
    REQUIRE(value == 0x2E85C257);

    as.RewindBuffer();

    as.VXOR(v4, v8, x11, VecMask::Yes);
    REQUIRE(value == 0x2C85C257);
}

TEST_CASE("VXOR.VI", "[rvv]") {
    uint32_t value = 0;
    Assembler as(reinterpret_cast<uint8_t*>(&value), sizeof(value));

    as.VXOR(v4, v8, 15, VecMask::No);
    REQUIRE(value == 0x2E87B257);

    as.RewindBuffer();

    as.VXOR(v4, v8, -16, VecMask::No);
    REQUIRE(value == 0x2E883257);

    as.RewindBuffer();

    as.VXOR(v4, v8, 15, VecMask::Yes);
    REQUIRE(value == 0x2C87B257);

    as.RewindBuffer();

    as.VXOR(v4, v8, -16, VecMask::Yes);
    REQUIRE(value == 0x2C883257);
}

TEST_CASE("VLE8.V", "[rvv]") {
    uint32_t value = 0;
    Assembler as(reinterpret_cast<uint8_t*>(&value), sizeof(value));

    as.VLE8(v0, x11, VecMask::No);
    REQUIRE(value == 0x02058007);

    as.RewindBuffer();

    as.VLE8(v0, x11, VecMask::Yes);
    REQUIRE(value == 0x00058007);
}

TEST_CASE("VLE16.V", "[rvv]") {
    uint32_t value = 0;
    Assembler as(reinterpret_cast<uint8_t*>(&value), sizeof(value));

    as.VLE16(v0, x11, VecMask::No);
    REQUIRE(value == 0x0205D007);

    as.RewindBuffer();

    as.VLE16(v0, x11, VecMask::Yes);
    REQUIRE(value == 0x0005D007);
}

TEST_CASE("VLE32.V", "[rvv]") {
    uint32_t value = 0;
    Assembler as(reinterpret_cast<uint8_t*>(&value), sizeof(value));

    as.VLE32(v0, x11, VecMask::No);
    REQUIRE(value == 0x0205E007);

    as.RewindBuffer();

    as.VLE32(v0, x11, VecMask::Yes);
    REQUIRE(value == 0x0005E007);
}

TEST_CASE("VLE64.V", "[rvv]") {
    uint32_t value = 0;
    Assembler as(reinterpret_cast<uint8_t*>(&value), sizeof(value));

    as.VLE64(v0, x11, VecMask::No);
    REQUIRE(value == 0x0205F007);

    as.RewindBuffer();

    as.VLE64(v0, x11, VecMask::Yes);
    REQUIRE(value == 0x0005F007);
}

TEST_CASE("VLM.V", "[rvv]") {
    uint32_t value = 0;
    Assembler as(reinterpret_cast<uint8_t*>(&value), sizeof(value));

    as.VLM(v0, x11);
    REQUIRE(value == 0x02B58007);
}

TEST_CASE("VLSE8.V", "[rvv]") {
    uint32_t value = 0;
    Assembler as(reinterpret_cast<uint8_t*>(&value), sizeof(value));

    as.VLSE8(v4, x10, x11, VecMask::No);
    REQUIRE(value == 0x0AB50207);

    as.RewindBuffer();

    as.VLSE8(v4, x10, x11, VecMask::Yes);
    REQUIRE(value == 0x08B50207);
}

TEST_CASE("VLSE16.V", "[rvv]") {
    uint32_t value = 0;
    Assembler as(reinterpret_cast<uint8_t*>(&value), sizeof(value));

    as.VLSE16(v4, x10, x11, VecMask::No);
    REQUIRE(value == 0x0AB55207);

    as.RewindBuffer();

    as.VLSE16(v4, x10, x11, VecMask::Yes);
    REQUIRE(value == 0x08B55207);
}

TEST_CASE("VLSE32.V", "[rvv]") {
    uint32_t value = 0;
    Assembler as(reinterpret_cast<uint8_t*>(&value), sizeof(value));

    as.VLSE32(v4, x10, x11, VecMask::No);
    REQUIRE(value == 0x0AB56207);

    as.RewindBuffer();

    as.VLSE32(v4, x10, x11, VecMask::Yes);
    REQUIRE(value == 0x08B56207);
}

TEST_CASE("VLSE64.V", "[rvv]") {
    uint32_t value = 0;
    Assembler as(reinterpret_cast<uint8_t*>(&value), sizeof(value));

    as.VLSE64(v4, x10, x11, VecMask::No);
    REQUIRE(value == 0x0AB57207);

    as.RewindBuffer();

    as.VLSE64(v4, x10, x11, VecMask::Yes);
    REQUIRE(value == 0x08B57207);
}

TEST_CASE("VLOXEI8.V", "[rvv]") {
    uint32_t value = 0;
    Assembler as(reinterpret_cast<uint8_t*>(&value), sizeof(value));

    as.VLOXEI8(v4, x10, v12, VecMask::No);
    REQUIRE(value == 0x0EC50207);

    as.RewindBuffer();

    as.VLOXEI8(v4, x10, v12, VecMask::Yes);
    REQUIRE(value == 0x0CC50207);
}

TEST_CASE("VLOXEI16.V", "[rvv]") {
    uint32_t value = 0;
    Assembler as(reinterpret_cast<uint8_t*>(&value), sizeof(value));

    as.VLOXEI16(v4, x10, v12, VecMask::No);
    REQUIRE(value == 0x0EC55207);

    as.RewindBuffer();

    as.VLOXEI16(v4, x10, v12, VecMask::Yes);
    REQUIRE(value == 0x0CC55207);
}

TEST_CASE("VLOXEI32.V", "[rvv]") {
    uint32_t value = 0;
    Assembler as(reinterpret_cast<uint8_t*>(&value), sizeof(value));

    as.VLOXEI32(v4, x10, v12, VecMask::No);
    REQUIRE(value == 0x0EC56207);

    as.RewindBuffer();

    as.VLOXEI32(v4, x10, v12, VecMask::Yes);
    REQUIRE(value == 0x0CC56207);
}

TEST_CASE("VLOXEI64.V", "[rvv]") {
    uint32_t value = 0;
    Assembler as(reinterpret_cast<uint8_t*>(&value), sizeof(value));

    as.VLOXEI64(v4, x10, v12, VecMask::No);
    REQUIRE(value == 0x0EC57207);

    as.RewindBuffer();

    as.VLOXEI64(v4, x10, v12, VecMask::Yes);
    REQUIRE(value == 0x0CC57207);
}

TEST_CASE("VLUXEI8.V", "[rvv]") {
    uint32_t value = 0;
    Assembler as(reinterpret_cast<uint8_t*>(&value), sizeof(value));

    as.VLUXEI8(v4, x10, v12, VecMask::No);
    REQUIRE(value == 0x06C50207);

    as.RewindBuffer();

    as.VLUXEI8(v4, x10, v12, VecMask::Yes);
    REQUIRE(value == 0x04C50207);
}

TEST_CASE("VLUXEI16.V", "[rvv]") {
    uint32_t value = 0;
    Assembler as(reinterpret_cast<uint8_t*>(&value), sizeof(value));

    as.VLUXEI16(v4, x10, v12, VecMask::No);
    REQUIRE(value == 0x06C55207);

    as.RewindBuffer();

    as.VLUXEI16(v4, x10, v12, VecMask::Yes);
    REQUIRE(value == 0x04C55207);
}

TEST_CASE("VLUXEI32.V", "[rvv]") {
    uint32_t value = 0;
    Assembler as(reinterpret_cast<uint8_t*>(&value), sizeof(value));

    as.VLUXEI32(v4, x10, v12, VecMask::No);
    REQUIRE(value == 0x06C56207);

    as.RewindBuffer();

    as.VLUXEI32(v4, x10, v12, VecMask::Yes);
    REQUIRE(value == 0x04C56207);
}

TEST_CASE("VLUXEI64.V", "[rvv]") {
    uint32_t value = 0;
    Assembler as(reinterpret_cast<uint8_t*>(&value), sizeof(value));

    as.VLUXEI64(v4, x10, v12, VecMask::No);
    REQUIRE(value == 0x06C57207);

    as.RewindBuffer();

    as.VLUXEI64(v4, x10, v12, VecMask::Yes);
    REQUIRE(value == 0x04C57207);
}

TEST_CASE("VLE8FF.V", "[rvv]") {
    uint32_t value = 0;
    Assembler as(reinterpret_cast<uint8_t*>(&value), sizeof(value));

    as.VLE8FF(v4, x10, VecMask::No);
    REQUIRE(value == 0x03050207);

    as.RewindBuffer();

    as.VLE8FF(v4, x10, VecMask::Yes);
    REQUIRE(value == 0x01050207);
}

TEST_CASE("VLE16FF.V", "[rvv]") {
    uint32_t value = 0;
    Assembler as(reinterpret_cast<uint8_t*>(&value), sizeof(value));

    as.VLE16FF(v4, x10, VecMask::No);
    REQUIRE(value == 0x03055207);

    as.RewindBuffer();

    as.VLE16FF(v4, x10, VecMask::Yes);
    REQUIRE(value == 0x01055207);
}

TEST_CASE("VLE32FF.V", "[rvv]") {
    uint32_t value = 0;
    Assembler as(reinterpret_cast<uint8_t*>(&value), sizeof(value));

    as.VLE32FF(v4, x10, VecMask::No);
    REQUIRE(value == 0x03056207);

    as.RewindBuffer();

    as.VLE32FF(v4, x10, VecMask::Yes);
    REQUIRE(value == 0x01056207);
}

TEST_CASE("VLE64FF.V", "[rvv]") {
    uint32_t value = 0;
    Assembler as(reinterpret_cast<uint8_t*>(&value), sizeof(value));

    as.VLE64FF(v4, x10, VecMask::No);
    REQUIRE(value == 0x03057207);

    as.RewindBuffer();

    as.VLE64FF(v4, x10, VecMask::Yes);
    REQUIRE(value == 0x01057207);
}

TEST_CASE("8-bit segmented unit-stride loads", "[rvv]") {
    uint32_t value = 0;
    Assembler as(reinterpret_cast<uint8_t*>(&value), sizeof(value));

    for (uint32_t i = 2; i <= 8; i++) {
        as.VLSEGE8(i, v4, x10, VecMask::No);
        REQUIRE(value == (0x02050207 | ((i - 1) << 29)));

        as.RewindBuffer();

        as.VLSEGE8(i, v4, x10, VecMask::Yes);
        REQUIRE(value == (0x00050207 | ((i - 1) << 29)));

        as.RewindBuffer();
    }
}

TEST_CASE("16-bit segmented unit-stride loads", "[rvv]") {
    uint32_t value = 0;
    Assembler as(reinterpret_cast<uint8_t*>(&value), sizeof(value));

    for (uint32_t i = 2; i <= 8; i++) {
        as.VLSEGE16(i, v4, x10, VecMask::No);
        REQUIRE(value == (0x02055207 | ((i - 1) << 29)));

        as.RewindBuffer();

        as.VLSEGE16(i, v4, x10, VecMask::Yes);
        REQUIRE(value == (0x00055207 | ((i - 1) << 29)));

        as.RewindBuffer();
    }
}

TEST_CASE("32-bit segmented unit-stride loads", "[rvv]") {
    uint32_t value = 0;
    Assembler as(reinterpret_cast<uint8_t*>(&value), sizeof(value));

    for (uint32_t i = 2; i <= 8; i++) {
        as.VLSEGE32(i, v4, x10, VecMask::No);
        REQUIRE(value == (0x02056207 | ((i - 1) << 29)));

        as.RewindBuffer();

        as.VLSEGE32(i, v4, x10, VecMask::Yes);
        REQUIRE(value == (0x00056207 | ((i - 1) << 29)));

        as.RewindBuffer();
    }
}

TEST_CASE("64-bit segmented unit-stride loads", "[rvv]") {
    uint32_t value = 0;
    Assembler as(reinterpret_cast<uint8_t*>(&value), sizeof(value));

    for (uint32_t i = 2; i <= 8; i++) {
        as.VLSEGE64(i, v4, x10, VecMask::No);
        REQUIRE(value == (0x02057207 | ((i - 1) << 29)));

        as.RewindBuffer();

        as.VLSEGE64(i, v4, x10, VecMask::Yes);
        REQUIRE(value == (0x00057207 | ((i - 1) << 29)));

        as.RewindBuffer();
    }
}

TEST_CASE("8-bit segmented strided loads", "[rvv]") {
    uint32_t value = 0;
    Assembler as(reinterpret_cast<uint8_t*>(&value), sizeof(value));

    for (uint32_t i = 2; i <= 8; i++) {
        as.VLSSEGE8(i, v4, x10, x11, VecMask::No);
        REQUIRE(value == (0x0AB50207 | ((i - 1) << 29)));

        as.RewindBuffer();

        as.VLSSEGE8(i, v4, x10, x11, VecMask::Yes);
        REQUIRE(value == (0x08B50207 | ((i - 1) << 29)));

        as.RewindBuffer();
    }
}

TEST_CASE("16-bit segmented strided loads", "[rvv]") {
    uint32_t value = 0;
    Assembler as(reinterpret_cast<uint8_t*>(&value), sizeof(value));

    for (uint32_t i = 2; i <= 8; i++) {
        as.VLSSEGE16(i, v4, x10, x11, VecMask::No);
        REQUIRE(value == (0x0AB55207 | ((i - 1) << 29)));

        as.RewindBuffer();

        as.VLSSEGE16(i, v4, x10, x11, VecMask::Yes);
        REQUIRE(value == (0x08B55207 | ((i - 1) << 29)));

        as.RewindBuffer();
    }
}

TEST_CASE("32-bit segmented strided loads", "[rvv]") {
    uint32_t value = 0;
    Assembler as(reinterpret_cast<uint8_t*>(&value), sizeof(value));

    for (uint32_t i = 2; i <= 8; i++) {
        as.VLSSEGE32(i, v4, x10, x11, VecMask::No);
        REQUIRE(value == (0x0AB56207 | ((i - 1) << 29)));

        as.RewindBuffer();

        as.VLSSEGE32(i, v4, x10, x11, VecMask::Yes);
        REQUIRE(value == (0x08B56207 | ((i - 1) << 29)));

        as.RewindBuffer();
    }
}

TEST_CASE("64-bit segmented strided loads", "[rvv]") {
    uint32_t value = 0;
    Assembler as(reinterpret_cast<uint8_t*>(&value), sizeof(value));

    for (uint32_t i = 2; i <= 8; i++) {
        as.VLSSEGE64(i, v4, x10, x11, VecMask::No);
        REQUIRE(value == (0x0AB57207 | ((i - 1) << 29)));

        as.RewindBuffer();

        as.VLSSEGE64(i, v4, x10, x11, VecMask::Yes);
        REQUIRE(value == (0x08B57207 | ((i - 1) << 29)));

        as.RewindBuffer();
    }
}

TEST_CASE("8-bit vector indexed-ordered segment loads", "[rvv]") {
    uint32_t value = 0;
    Assembler as(reinterpret_cast<uint8_t*>(&value), sizeof(value));

    for (uint32_t i = 2; i <= 8; i++) {
        as.VLOXSEGEI8(i, v4, x10, v12, VecMask::No);
        REQUIRE(value == (0x0EC50207 | ((i - 1) << 29)));

        as.RewindBuffer();

        as.VLOXSEGEI8(i, v4, x10, v12, VecMask::Yes);
        REQUIRE(value == (0x0CC50207 | ((i - 1) << 29)));

        as.RewindBuffer();
    }
}

TEST_CASE("16-bit vector indexed-ordered segment loads", "[rvv]") {
    uint32_t value = 0;
    Assembler as(reinterpret_cast<uint8_t*>(&value), sizeof(value));

    for (uint32_t i = 2; i <= 8; i++) {
        as.VLOXSEGEI16(i, v4, x10, v12, VecMask::No);
        REQUIRE(value == (0x0EC55207 | ((i - 1) << 29)));

        as.RewindBuffer();

        as.VLOXSEGEI16(i, v4, x10, v12, VecMask::Yes);
        REQUIRE(value == (0x0CC55207 | ((i - 1) << 29)));

        as.RewindBuffer();
    }
}

TEST_CASE("32-bit vector indexed-ordered segment loads", "[rvv]") {
    uint32_t value = 0;
    Assembler as(reinterpret_cast<uint8_t*>(&value), sizeof(value));

    for (uint32_t i = 2; i <= 8; i++) {
        as.VLOXSEGEI32(i, v4, x10, v12, VecMask::No);
        REQUIRE(value == (0x0EC56207 | ((i - 1) << 29)));

        as.RewindBuffer();

        as.VLOXSEGEI32(i, v4, x10, v12, VecMask::Yes);
        REQUIRE(value == (0x0CC56207 | ((i - 1) << 29)));

        as.RewindBuffer();
    }
}

TEST_CASE("64-bit vector indexed-ordered segment loads", "[rvv]") {
    uint32_t value = 0;
    Assembler as(reinterpret_cast<uint8_t*>(&value), sizeof(value));

    for (uint32_t i = 2; i <= 8; i++) {
        as.VLOXSEGEI64(i, v4, x10, v12, VecMask::No);
        REQUIRE(value == (0x0EC57207 | ((i - 1) << 29)));

        as.RewindBuffer();

        as.VLOXSEGEI64(i, v4, x10, v12, VecMask::Yes);
        REQUIRE(value == (0x0CC57207 | ((i - 1) << 29)));

        as.RewindBuffer();
    }
}

TEST_CASE("8-bit vector indexed-unordered segment loads", "[rvv]") {
    uint32_t value = 0;
    Assembler as(reinterpret_cast<uint8_t*>(&value), sizeof(value));

    for (uint32_t i = 2; i <= 8; i++) {
        as.VLUXSEGEI8(i, v4, x10, v12, VecMask::No);
        REQUIRE(value == (0x06C50207 | ((i - 1) << 29)));

        as.RewindBuffer();

        as.VLUXSEGEI8(i, v4, x10, v12, VecMask::Yes);
        REQUIRE(value == (0x04C50207 | ((i - 1) << 29)));

        as.RewindBuffer();
    }
}

TEST_CASE("16-bit vector indexed-unordered segment loads", "[rvv]") {
    uint32_t value = 0;
    Assembler as(reinterpret_cast<uint8_t*>(&value), sizeof(value));

    for (uint32_t i = 2; i <= 8; i++) {
        as.VLUXSEGEI16(i, v4, x10, v12, VecMask::No);
        REQUIRE(value == (0x06C55207 | ((i - 1) << 29)));

        as.RewindBuffer();

        as.VLUXSEGEI16(i, v4, x10, v12, VecMask::Yes);
        REQUIRE(value == (0x04C55207 | ((i - 1) << 29)));

        as.RewindBuffer();
    }
}

TEST_CASE("32-bit vector indexed-unordered segment loads", "[rvv]") {
    uint32_t value = 0;
    Assembler as(reinterpret_cast<uint8_t*>(&value), sizeof(value));

    for (uint32_t i = 2; i <= 8; i++) {
        as.VLUXSEGEI32(i, v4, x10, v12, VecMask::No);
        REQUIRE(value == (0x06C56207 | ((i - 1) << 29)));

        as.RewindBuffer();

        as.VLUXSEGEI32(i, v4, x10, v12, VecMask::Yes);
        REQUIRE(value == (0x04C56207 | ((i - 1) << 29)));

        as.RewindBuffer();
    }
}

TEST_CASE("64-bit vector indexed-unordered segment loads", "[rvv]") {
    uint32_t value = 0;
    Assembler as(reinterpret_cast<uint8_t*>(&value), sizeof(value));

    for (uint32_t i = 2; i <= 8; i++) {
        as.VLUXSEGEI64(i, v4, x10, v12, VecMask::No);
        REQUIRE(value == (0x06C57207 | ((i - 1) << 29)));

        as.RewindBuffer();

        as.VLUXSEGEI64(i, v4, x10, v12, VecMask::Yes);
        REQUIRE(value == (0x04C57207 | ((i - 1) << 29)));

        as.RewindBuffer();
    }
}

TEST_CASE("8-bit vector whole register loads", "[rvv]") {
    uint32_t value = 0;
    Assembler as(reinterpret_cast<uint8_t*>(&value), sizeof(value));

    as.VLRE8(1, v3, x10);
    REQUIRE(value == 0x02850187);
    as.RewindBuffer();

    as.VLRE8(2, v2, x10);
    REQUIRE(value == 0x22850107);
    as.RewindBuffer();

    as.VLRE8(4, v4, x10);
    REQUIRE(value == 0x62850207);
    as.RewindBuffer();

    as.VLRE8(8, v8, x10);
    REQUIRE(value == 0xE2850407);
}

TEST_CASE("16-bit vector whole register loads", "[rvv]") {
    uint32_t value = 0;
    Assembler as(reinterpret_cast<uint8_t*>(&value), sizeof(value));

    as.VLRE16(1, v3, x10);
    REQUIRE(value == 0x02855187);
    as.RewindBuffer();

    as.VLRE16(2, v2, x10);
    REQUIRE(value == 0x22855107);
    as.RewindBuffer();

    as.VLRE16(4, v4, x10);
    REQUIRE(value == 0x62855207);
    as.RewindBuffer();

    as.VLRE16(8, v8, x10);
    REQUIRE(value == 0xE2855407);
}

TEST_CASE("32-bit vector whole register loads", "[rvv]") {
    uint32_t value = 0;
    Assembler as(reinterpret_cast<uint8_t*>(&value), sizeof(value));

    as.VLRE32(1, v3, x10);
    REQUIRE(value == 0x02856187);
    as.RewindBuffer();

    as.VLRE32(2, v2, x10);
    REQUIRE(value == 0x22856107);
    as.RewindBuffer();

    as.VLRE32(4, v4, x10);
    REQUIRE(value == 0x62856207);
    as.RewindBuffer();

    as.VLRE32(8, v8, x10);
    REQUIRE(value == 0xE2856407);
}

TEST_CASE("64-bit vector whole register loads", "[rvv]") {
    uint32_t value = 0;
    Assembler as(reinterpret_cast<uint8_t*>(&value), sizeof(value));

    as.VLRE64(1, v3, x10);
    REQUIRE(value == 0x02857187);
    as.RewindBuffer();

    as.VLRE64(2, v2, x10);
    REQUIRE(value == 0x22857107);
    as.RewindBuffer();

    as.VLRE64(4, v4, x10);
    REQUIRE(value == 0x62857207);
    as.RewindBuffer();

    as.VLRE64(8, v8, x10);
    REQUIRE(value == 0xE2857407);
}

TEST_CASE("VSE8.V", "[rvv]") {
    uint32_t value = 0;
    Assembler as(reinterpret_cast<uint8_t*>(&value), sizeof(value));

    as.VSE8(v0, x13, VecMask::No);
    REQUIRE(value == 0x02068027);

    as.RewindBuffer();

    as.VSE8(v0, x13, VecMask::Yes);
    REQUIRE(value == 0x00068027);
}

TEST_CASE("VSE16.V", "[rvv]") {
    uint32_t value = 0;
    Assembler as(reinterpret_cast<uint8_t*>(&value), sizeof(value));

    as.VSE16(v0, x13, VecMask::No);
    REQUIRE(value == 0x0206D027);

    as.RewindBuffer();

    as.VSE16(v0, x13, VecMask::Yes);
    REQUIRE(value == 0x0006D027);
}

TEST_CASE("VSE32.V", "[rvv]") {
    uint32_t value = 0;
    Assembler as(reinterpret_cast<uint8_t*>(&value), sizeof(value));

    as.VSE32(v0, x13, VecMask::No);
    REQUIRE(value == 0x0206E027);

    as.RewindBuffer();

    as.VSE32(v0, x13, VecMask::Yes);
    REQUIRE(value == 0x0006E027);
}

TEST_CASE("VSE64.V", "[rvv]") {
    uint32_t value = 0;
    Assembler as(reinterpret_cast<uint8_t*>(&value), sizeof(value));

    as.VSE64(v0, x13, VecMask::No);
    REQUIRE(value == 0x0206F027);

    as.RewindBuffer();

    as.VSE64(v0, x13, VecMask::Yes);
    REQUIRE(value == 0x0006F027);
}

TEST_CASE("VSM.V", "[rvv]") {
    uint32_t value = 0;
    Assembler as(reinterpret_cast<uint8_t*>(&value), sizeof(value));

    as.VSM(v0, x13);
    REQUIRE(value == 0x02B68027);
}

TEST_CASE("VSSE8.V", "[rvv]") {
    uint32_t value = 0;
    Assembler as(reinterpret_cast<uint8_t*>(&value), sizeof(value));

    as.VSSE8(v4, x10, x11, VecMask::No);
    REQUIRE(value == 0x0AB50227);

    as.RewindBuffer();

    as.VSSE8(v4, x10, x11, VecMask::Yes);
    REQUIRE(value == 0x08B50227);
}

TEST_CASE("VSSE16.V", "[rvv]") {
    uint32_t value = 0;
    Assembler as(reinterpret_cast<uint8_t*>(&value), sizeof(value));

    as.VSSE16(v4, x10, x11, VecMask::No);
    REQUIRE(value == 0x0AB55227);

    as.RewindBuffer();

    as.VSSE16(v4, x10, x11, VecMask::Yes);
    REQUIRE(value == 0x08B55227);
}

TEST_CASE("VSSE32.V", "[rvv]") {
    uint32_t value = 0;
    Assembler as(reinterpret_cast<uint8_t*>(&value), sizeof(value));

    as.VSSE32(v4, x10, x11, VecMask::No);
    REQUIRE(value == 0x0AB56227);

    as.RewindBuffer();

    as.VSSE32(v4, x10, x11, VecMask::Yes);
    REQUIRE(value == 0x08B56227);
}

TEST_CASE("VSSE64.V", "[rvv]") {
    uint32_t value = 0;
    Assembler as(reinterpret_cast<uint8_t*>(&value), sizeof(value));

    as.VSSE64(v4, x10, x11, VecMask::No);
    REQUIRE(value == 0x0AB57227);

    as.RewindBuffer();

    as.VSSE64(v4, x10, x11, VecMask::Yes);
    REQUIRE(value == 0x08B57227);
}

TEST_CASE("VSOXEI8.V", "[rvv]") {
    uint32_t value = 0;
    Assembler as(reinterpret_cast<uint8_t*>(&value), sizeof(value));

    as.VSOXEI8(v4, x10, v12, VecMask::No);
    REQUIRE(value == 0x0EC50227);

    as.RewindBuffer();

    as.VSOXEI8(v4, x10, v12, VecMask::Yes);
    REQUIRE(value == 0x0CC50227);
}

TEST_CASE("VSOXEI16.V", "[rvv]") {
    uint32_t value = 0;
    Assembler as(reinterpret_cast<uint8_t*>(&value), sizeof(value));

    as.VSOXEI16(v4, x10, v12, VecMask::No);
    REQUIRE(value == 0x0EC55227);

    as.RewindBuffer();

    as.VSOXEI16(v4, x10, v12, VecMask::Yes);
    REQUIRE(value == 0x0CC55227);
}

TEST_CASE("VSOXEI32.V", "[rvv]") {
    uint32_t value = 0;
    Assembler as(reinterpret_cast<uint8_t*>(&value), sizeof(value));

    as.VSOXEI32(v4, x10, v12, VecMask::No);
    REQUIRE(value == 0x0EC56227);

    as.RewindBuffer();

    as.VSOXEI32(v4, x10, v12, VecMask::Yes);
    REQUIRE(value == 0x0CC56227);
}

TEST_CASE("VSOXEI64.V", "[rvv]") {
    uint32_t value = 0;
    Assembler as(reinterpret_cast<uint8_t*>(&value), sizeof(value));

    as.VSOXEI64(v4, x10, v12, VecMask::No);
    REQUIRE(value == 0x0EC57227);

    as.RewindBuffer();

    as.VSOXEI64(v4, x10, v12, VecMask::Yes);
    REQUIRE(value == 0x0CC57227);
}

TEST_CASE("VSUXEI8.V", "[rvv]") {
    uint32_t value = 0;
    Assembler as(reinterpret_cast<uint8_t*>(&value), sizeof(value));

    as.VSUXEI8(v4, x10, v12, VecMask::No);
    REQUIRE(value == 0x06C50227);

    as.RewindBuffer();

    as.VSUXEI8(v4, x10, v12, VecMask::Yes);
    REQUIRE(value == 0x04C50227);
}

TEST_CASE("VSUXEI16.V", "[rvv]") {
    uint32_t value = 0;
    Assembler as(reinterpret_cast<uint8_t*>(&value), sizeof(value));

    as.VSUXEI16(v4, x10, v12, VecMask::No);
    REQUIRE(value == 0x06C55227);

    as.RewindBuffer();

    as.VSUXEI16(v4, x10, v12, VecMask::Yes);
    REQUIRE(value == 0x04C55227);
}

TEST_CASE("VSUXEI32.V", "[rvv]") {
    uint32_t value = 0;
    Assembler as(reinterpret_cast<uint8_t*>(&value), sizeof(value));

    as.VSUXEI32(v4, x10, v12, VecMask::No);
    REQUIRE(value == 0x06C56227);

    as.RewindBuffer();

    as.VSUXEI32(v4, x10, v12, VecMask::Yes);
    REQUIRE(value == 0x04C56227);
}

TEST_CASE("VSUXEI64.V", "[rvv]") {
    uint32_t value = 0;
    Assembler as(reinterpret_cast<uint8_t*>(&value), sizeof(value));

    as.VSUXEI64(v4, x10, v12, VecMask::No);
    REQUIRE(value == 0x06C57227);

    as.RewindBuffer();

    as.VSUXEI64(v4, x10, v12, VecMask::Yes);
    REQUIRE(value == 0x04C57227);
}

TEST_CASE("8-bit segmented unit-stride stores", "[rvv]") {
    uint32_t value = 0;
    Assembler as(reinterpret_cast<uint8_t*>(&value), sizeof(value));

    for (uint32_t i = 2; i <= 8; i++) {
        as.VSSEGE8(i, v4, x10, VecMask::No);
        REQUIRE(value == (0x02050227 | ((i - 1) << 29)));

        as.RewindBuffer();

        as.VSSEGE8(i, v4, x10, VecMask::Yes);
        REQUIRE(value == (0x00050227 | ((i - 1) << 29)));

        as.RewindBuffer();
    }
}

TEST_CASE("16-bit segmented unit-stride stores", "[rvv]") {
    uint32_t value = 0;
    Assembler as(reinterpret_cast<uint8_t*>(&value), sizeof(value));

    for (uint32_t i = 2; i <= 8; i++) {
        as.VSSEGE16(i, v4, x10, VecMask::No);
        REQUIRE(value == (0x02055227 | ((i - 1) << 29)));

        as.RewindBuffer();

        as.VSSEGE16(i, v4, x10, VecMask::Yes);
        REQUIRE(value == (0x00055227 | ((i - 1) << 29)));

        as.RewindBuffer();
    }
}

TEST_CASE("32-bit segmented unit-stride stores", "[rvv]") {
    uint32_t value = 0;
    Assembler as(reinterpret_cast<uint8_t*>(&value), sizeof(value));

    for (uint32_t i = 2; i <= 8; i++) {
        as.VSSEGE32(i, v4, x10, VecMask::No);
        REQUIRE(value == (0x02056227 | ((i - 1) << 29)));

        as.RewindBuffer();

        as.VSSEGE32(i, v4, x10, VecMask::Yes);
        REQUIRE(value == (0x00056227 | ((i - 1) << 29)));

        as.RewindBuffer();
    }
}

TEST_CASE("64-bit segmented unit-stride stores", "[rvv]") {
    uint32_t value = 0;
    Assembler as(reinterpret_cast<uint8_t*>(&value), sizeof(value));

    for (uint32_t i = 2; i <= 8; i++) {
        as.VSSEGE64(i, v4, x10, VecMask::No);
        REQUIRE(value == (0x02057227 | ((i - 1) << 29)));

        as.RewindBuffer();

        as.VSSEGE64(i, v4, x10, VecMask::Yes);
        REQUIRE(value == (0x00057227 | ((i - 1) << 29)));

        as.RewindBuffer();
    }
}

TEST_CASE("8-bit segmented strided stores", "[rvv]") {
    uint32_t value = 0;
    Assembler as(reinterpret_cast<uint8_t*>(&value), sizeof(value));

    for (uint32_t i = 2; i <= 8; i++) {
        as.VSSSEGE8(i, v4, x10, x11, VecMask::No);
        REQUIRE(value == (0x0AB50227 | ((i - 1) << 29)));

        as.RewindBuffer();

        as.VSSSEGE8(i, v4, x10, x11, VecMask::Yes);
        REQUIRE(value == (0x08B50227 | ((i - 1) << 29)));

        as.RewindBuffer();
    }
}

TEST_CASE("16-bit segmented strided stores", "[rvv]") {
    uint32_t value = 0;
    Assembler as(reinterpret_cast<uint8_t*>(&value), sizeof(value));

    for (uint32_t i = 2; i <= 8; i++) {
        as.VSSSEGE16(i, v4, x10, x11, VecMask::No);
        REQUIRE(value == (0x0AB55227 | ((i - 1) << 29)));

        as.RewindBuffer();

        as.VSSSEGE16(i, v4, x10, x11, VecMask::Yes);
        REQUIRE(value == (0x08B55227 | ((i - 1) << 29)));

        as.RewindBuffer();
    }
}

TEST_CASE("32-bit segmented strided stores", "[rvv]") {
    uint32_t value = 0;
    Assembler as(reinterpret_cast<uint8_t*>(&value), sizeof(value));

    for (uint32_t i = 2; i <= 8; i++) {
        as.VSSSEGE32(i, v4, x10, x11, VecMask::No);
        REQUIRE(value == (0x0AB56227 | ((i - 1) << 29)));

        as.RewindBuffer();

        as.VSSSEGE32(i, v4, x10, x11, VecMask::Yes);
        REQUIRE(value == (0x08B56227 | ((i - 1) << 29)));

        as.RewindBuffer();
    }
}

TEST_CASE("64-bit segmented strided stores", "[rvv]") {
    uint32_t value = 0;
    Assembler as(reinterpret_cast<uint8_t*>(&value), sizeof(value));

    for (uint32_t i = 2; i <= 8; i++) {
        as.VSSSEGE64(i, v4, x10, x11, VecMask::No);
        REQUIRE(value == (0x0AB57227 | ((i - 1) << 29)));

        as.RewindBuffer();

        as.VSSSEGE64(i, v4, x10, x11, VecMask::Yes);
        REQUIRE(value == (0x08B57227 | ((i - 1) << 29)));

        as.RewindBuffer();
    }
}

TEST_CASE("8-bit segmented vector indexed-ordered stores", "[rvv]") {
    uint32_t value = 0;
    Assembler as(reinterpret_cast<uint8_t*>(&value), sizeof(value));

    for (uint32_t i = 2; i <= 8; i++) {
        as.VSOXSEGEI8(i, v4, x10, v12, VecMask::No);
        REQUIRE(value == (0x0EC50227 | ((i - 1) << 29)));

        as.RewindBuffer();

        as.VSOXSEGEI8(i, v4, x10, v12, VecMask::Yes);
        REQUIRE(value == (0x0CC50227 | ((i - 1) << 29)));

        as.RewindBuffer();
    }
}

TEST_CASE("16-bit segmented vector indexed-ordered stores", "[rvv]") {
    uint32_t value = 0;
    Assembler as(reinterpret_cast<uint8_t*>(&value), sizeof(value));

    for (uint32_t i = 2; i <= 8; i++) {
        as.VSOXSEGEI16(i, v4, x10, v12, VecMask::No);
        REQUIRE(value == (0x0EC55227 | ((i - 1) << 29)));

        as.RewindBuffer();

        as.VSOXSEGEI16(i, v4, x10, v12, VecMask::Yes);
        REQUIRE(value == (0x0CC55227 | ((i - 1) << 29)));

        as.RewindBuffer();
    }
}

TEST_CASE("32-bit segmented vector indexed-ordered stores", "[rvv]") {
    uint32_t value = 0;
    Assembler as(reinterpret_cast<uint8_t*>(&value), sizeof(value));

    for (uint32_t i = 2; i <= 8; i++) {
        as.VSOXSEGEI32(i, v4, x10, v12, VecMask::No);
        REQUIRE(value == (0x0EC56227 | ((i - 1) << 29)));

        as.RewindBuffer();

        as.VSOXSEGEI32(i, v4, x10, v12, VecMask::Yes);
        REQUIRE(value == (0x0CC56227 | ((i - 1) << 29)));

        as.RewindBuffer();
    }
}

TEST_CASE("64-bit segmented vector indexed-ordered stores", "[rvv]") {
    uint32_t value = 0;
    Assembler as(reinterpret_cast<uint8_t*>(&value), sizeof(value));

    for (uint32_t i = 2; i <= 8; i++) {
        as.VSOXSEGEI64(i, v4, x10, v12, VecMask::No);
        REQUIRE(value == (0x0EC57227 | ((i - 1) << 29)));

        as.RewindBuffer();

        as.VSOXSEGEI64(i, v4, x10, v12, VecMask::Yes);
        REQUIRE(value == (0x0CC57227 | ((i - 1) << 29)));

        as.RewindBuffer();
    }
}

TEST_CASE("8-bit segmented vector indexed-unordered stores", "[rvv]") {
    uint32_t value = 0;
    Assembler as(reinterpret_cast<uint8_t*>(&value), sizeof(value));

    for (uint32_t i = 2; i <= 8; i++) {
        as.VSUXSEGEI8(i, v4, x10, v12, VecMask::No);
        REQUIRE(value == (0x06C50227 | ((i - 1) << 29)));

        as.RewindBuffer();

        as.VSUXSEGEI8(i, v4, x10, v12, VecMask::Yes);
        REQUIRE(value == (0x04C50227 | ((i - 1) << 29)));

        as.RewindBuffer();
    }
}

TEST_CASE("16-bit segmented vector indexed-unordered stores", "[rvv]") {
    uint32_t value = 0;
    Assembler as(reinterpret_cast<uint8_t*>(&value), sizeof(value));

    for (uint32_t i = 2; i <= 8; i++) {
        as.VSUXSEGEI16(i, v4, x10, v12, VecMask::No);
        REQUIRE(value == (0x06C55227 | ((i - 1) << 29)));

        as.RewindBuffer();

        as.VSUXSEGEI16(i, v4, x10, v12, VecMask::Yes);
        REQUIRE(value == (0x04C55227 | ((i - 1) << 29)));

        as.RewindBuffer();
    }
}

TEST_CASE("32-bit segmented vector indexed-unordered stores", "[rvv]") {
    uint32_t value = 0;
    Assembler as(reinterpret_cast<uint8_t*>(&value), sizeof(value));

    for (uint32_t i = 2; i <= 8; i++) {
        as.VSUXSEGEI32(i, v4, x10, v12, VecMask::No);
        REQUIRE(value == (0x06C56227 | ((i - 1) << 29)));

        as.RewindBuffer();

        as.VSUXSEGEI32(i, v4, x10, v12, VecMask::Yes);
        REQUIRE(value == (0x04C56227 | ((i - 1) << 29)));

        as.RewindBuffer();
    }
}

TEST_CASE("64-bit segmented vector indexed-unordered stores", "[rvv]") {
    uint32_t value = 0;
    Assembler as(reinterpret_cast<uint8_t*>(&value), sizeof(value));

    for (uint32_t i = 2; i <= 8; i++) {
        as.VSUXSEGEI64(i, v4, x10, v12, VecMask::No);
        REQUIRE(value == (0x06C57227 | ((i - 1) << 29)));

        as.RewindBuffer();

        as.VSUXSEGEI64(i, v4, x10, v12, VecMask::Yes);
        REQUIRE(value == (0x04C57227 | ((i - 1) << 29)));

        as.RewindBuffer();
    }
}

TEST_CASE("Vector whole register stores", "[rvv]") {
    uint32_t value = 0;
    Assembler as(reinterpret_cast<uint8_t*>(&value), sizeof(value));

    as.VSR(1, v3, x11);
    REQUIRE(value == 0x028581A7);

    as.RewindBuffer();

    as.VSR(2, v2, x11);
    REQUIRE(value == 0x22858127);

    as.RewindBuffer();

    as.VSR(4, v4, x11);
    REQUIRE(value == 0x62858227);

    as.RewindBuffer();

    as.VSR(8, v8, x11);
    REQUIRE(value == 0xE2858427);
}

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
