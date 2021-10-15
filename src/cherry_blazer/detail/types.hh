#pragma once

#include <cstdint>

namespace cherry_blazer {

namespace detail {

using i8 = int_fast8_t;
using i16 = int_fast16_t;
using i32 = int_fast32_t;
using i64 = int_fast64_t;

using u8 = uint_fast8_t;
using u16 = uint_fast16_t;
using u32 = uint_fast32_t;
using u64 = uint_fast64_t;

} // namespace detail

using detail::i16;
using detail::i32;
using detail::i64;
using detail::i8;

using detail::u16;
using detail::u32;
using detail::u64;
using detail::u8;

} // namespace cherry_blazer
