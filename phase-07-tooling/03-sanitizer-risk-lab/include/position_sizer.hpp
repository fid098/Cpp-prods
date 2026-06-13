#pragma once

// ---------------------------------------------------------------------------
// BUG 3: Signed integer overflow in size_position()
//   When notional * multiplier > INT_MAX, the result wraps to a negative number.
//   Detected by: -fsanitize=undefined (signed-integer-overflow)
//   Fix: use long long for notional, multiplier, and the return type.
// ---------------------------------------------------------------------------

// Returns the raw position size in number of contracts.
// BUG 3: int overflow when notional * multiplier > 2^31-1
inline int size_position(int notional, int multiplier) {
    return notional * multiplier;
}

// Fixed version — TODO: implement using long long
inline long long size_position_safe(long long notional, long long multiplier) {
    // TODO: return notional * multiplier
    return 0;
}
