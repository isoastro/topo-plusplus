#ifndef _testCommon_H_
#define _testCommon_H_

#include <limits>
#include <iostream>
#include "catch2/catch.hpp"

// Common functions that may be useful across multiple test suites

template <typename T>
void compareReals(const T & lhs, const T & rhs, const T & eps = std::numeric_limits<T>::epsilon()) {
    CHECK(std::abs(lhs - rhs) <= eps);
}

template <typename T, int sigFigs>
void compareRealsSigFigs(const T & lhs, const T & rhs) {
    auto diff = std::abs(lhs - rhs);
    auto max = std::max(std::abs(lhs), std::abs(rhs));
    auto thresh = std::pow(0.1, static_cast<T>(sigFigs)) * max;
    CHECK(diff < thresh);
}

#endif // _testCommon_H_
