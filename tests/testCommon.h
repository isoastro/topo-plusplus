#ifndef _testCommon_H_
#define _testCommon_H_

#include <limits>
#include "catch2/catch.hpp"

// Common functions that may be useful across multiple test suites

template <typename T>
void compareReals(const T & lhs, const T & rhs, const T & eps = std::numeric_limits<T>::epsilon()) {
    CHECK(std::abs(lhs - rhs) <= eps);
}

#endif // _testCommon_H_
