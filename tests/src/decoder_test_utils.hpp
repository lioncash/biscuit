#pragma once

#include <biscuit/decoder.hpp>

namespace biscuit {

inline Decoder MakeDecoder32() {
    return Decoder(ArchFeature::RV32);
}

}