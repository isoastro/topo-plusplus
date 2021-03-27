#include "catch2/catch.hpp"
#include "testCommon.h"
#include "geometry.h"

using namespace geometry;

TEST_CASE("Point construction", "[geometry]") {
    SECTION("No args") {
        Point p;
        CHECK(p.x == 0.0);
        CHECK(p.y == 0.0);
        CHECK(p.z == 0.0);
    }

    SECTION("3 args") {
        Point p(1.2, -2.3, 3.4);
        CHECK(p.x == 1.2);
        CHECK(p.y == -2.3);
        CHECK(p.z == 3.4);
    }

    SECTION("Copy") {
        Point p(1, 2, 3);
        Point q(p);
        p.x += 1;
        CHECK(p.x == 2.0); // changed, but q didn't
        CHECK(p.y == 2.0);
        CHECK(p.z == 3.0);
        CHECK(q.x == 1.0);
        CHECK(q.y == 2.0);
        CHECK(q.z == 3.0);
    }

    SECTION("Assign") {
        Point p(1, 2, 3);
        Point q = p;
        p.x += 1;
        CHECK(p.x == 2.0); // changed, but q didn't
        CHECK(p.y == 2.0);
        CHECK(p.z == 3.0);
        CHECK(q.x == 1.0);
        CHECK(q.y == 2.0);
        CHECK(q.z == 3.0);
    }

    SECTION("From geodetic") {
        // TODO
    }
}

TEST_CASE("Point const-methods", "[geometry]") {
    SECTION("Dot product") {
        SECTION("Zero lhs") {
            Point p(0, 0, 0);
            Point q(-5, -6, 7);
            double dot = p.dot(q);
            CHECK(dot == 0.0); // Anything dot 0 is zero
        }

        SECTION("Zero rhs") {
            Point p(1, -2, 3);
            Point q(0, 0, 0);
            double dot = p.dot(q);
            CHECK(dot == 0.0); // Anything dot 0 is zero
        }

        SECTION("Non zero") {
            Point p(1, -2, 3);
            Point q(-9, -6, 11);
            double dot = p.dot(q);
            compareReals(dot, 36.0); // -9 + 12 + 33 = 36
        }

        SECTION("Self") {
            Point p(1, -2, 3);
            double dot = p.dot(p);
            compareReals(dot, 14.0); // 1 + 4 + 9 = 14
        }
    }

    SECTION("Cross product") {
        SECTION("Zero lhs") {
            Point p(0, 0, 0);
            Point q(-5, -6, 7);
            Point r = p.cross(q);
            CHECK(r.x == 0.0);
            CHECK(r.y == 0.0);
            CHECK(r.z == 0.0);
        }

        SECTION("Zero rhs") {
            Point p(1, -2, 3);
            Point q(0, 0, 0);
            Point r = p.cross(q);
            CHECK(r.x == 0.0);
            CHECK(r.y == 0.0);
            CHECK(r.z == 0.0);
        }

        SECTION("Non zero") {
            Point p(1, -2, 3);
            Point q(-9, -6, 11);
            Point r = p.cross(q);
            compareReals(r.x, -4.0);
            compareReals(r.y, -38.0);
            compareReals(r.z, -24.0);
        }
    }

    SECTION("2-norm") {
        SECTION("Zero") {
            Point p(0, 0, 0);
            double norm = p.norm();
            CHECK(norm == 0.0);
        }

        SECTION("Unit vector") {
            Point p(0, 0, 0);
            SECTION("x") {
                p.x = 1;
            }
            SECTION("y") {
                p.y = 1;
            }
            SECTION("z") {
                p.z = 1;
            }
            SECTION("xy") {
                p.x = std::sqrt(2.0) / 2.0;
                p.y = std::sqrt(2.0) / 2.0;
            }
            SECTION("yz") {
                p.y = std::sqrt(2.0) / 2.0;
                p.z = std::sqrt(2.0) / 2.0;
            }
            SECTION("xz") {
                p.x = std::sqrt(2.0) / 2.0;
                p.z = std::sqrt(2.0) / 2.0;
            }
            double norm = p.norm();
            compareReals(norm, 1.0);
        }

        SECTION("Non zero") {
            Point p(-9, -6, 11);
            double norm = p.norm();
            compareReals(norm, 15.427248620541512);
        }
    }
}

TEST_CASE("Point math operations", "[geometry]") {
    SECTION("Negation") {
        Point lhs = {1, -2, 3};
        Point res = -lhs;
        CHECK(res.x == -lhs.x);
        CHECK(res.y == -lhs.y);
        CHECK(res.z == -lhs.z);
    }

    SECTION("Addition") {
        Point lhs(1, -2, 3);
        Point rhs;
        SECTION("Zero") {
            rhs = {0, 0, 0};
            Point result = lhs + rhs;
            CHECK(result.x == lhs.x);
            CHECK(result.y == lhs.y);
            CHECK(result.z == lhs.z);
        }

        SECTION("Negative") {
            rhs = {-2, -2, -2};
            Point result = lhs + rhs;
            CHECK(result.x == lhs.x - 2);
            CHECK(result.y == lhs.y - 2);
            CHECK(result.z == lhs.z - 2);
        }

        SECTION("Positive") {
            rhs = {2, 2, 2};
            Point result = lhs + rhs;
            CHECK(result.x == lhs.x + 2);
            CHECK(result.y == lhs.y + 2);
            CHECK(result.z == lhs.z + 2);
        }

        SECTION("Self") {
            Point result = lhs + lhs;
            CHECK(result.x == lhs.x + lhs.x);
            CHECK(result.y == lhs.y + lhs.y);
            CHECK(result.z == lhs.z + lhs.z);
        }
        // lhs did not change, all these methods are const
        REQUIRE(lhs.x == 1);
        REQUIRE(lhs.y == -2);
        REQUIRE(lhs.z == 3);
    }

    SECTION("Auto-addition") {
        Point lhs(1, -2, 3);
        Point rhs;
        SECTION("Zero") {
            rhs = {0, 0, 0};
            lhs += rhs;
            CHECK(lhs.x == 1);
            CHECK(lhs.y == -2);
            CHECK(lhs.z == 3);
        }

        SECTION("Negative") {
            rhs = {-2, -2, -2};
            lhs += rhs;
            CHECK(lhs.x == 1 - 2);
            CHECK(lhs.y == -2 - 2);
            CHECK(lhs.z == 3 - 2);
        }

        SECTION("Positive") {
            rhs = {2, 2, 2};
            lhs += rhs;
            CHECK(lhs.x == 1 + 2);
            CHECK(lhs.y == -2 + 2);
            CHECK(lhs.z == 3 + 2);
        }

        SECTION("Self") {
            lhs += lhs;
            CHECK(lhs.x == 1 + 1);
            CHECK(lhs.y == -2 - 2);
            CHECK(lhs.z == 3 + 3);
        }
    }

    SECTION("Subtraction") {
        Point lhs(1, -2, 3);
        Point rhs;
        SECTION("Zero") {
            rhs = {0, 0, 0};
            Point result = lhs - rhs;
            CHECK(result.x == lhs.x);
            CHECK(result.y == lhs.y);
            CHECK(result.z == lhs.z);
        }

        SECTION("Negative") {
            rhs = {-2, -2, -2};
            Point result = lhs - rhs;
            CHECK(result.x == lhs.x + 2);
            CHECK(result.y == lhs.y + 2);
            CHECK(result.z == lhs.z + 2);
        }

        SECTION("Positive") {
            rhs = {2, 2, 2};
            Point result = lhs - rhs;
            CHECK(result.x == lhs.x - 2);
            CHECK(result.y == lhs.y - 2);
            CHECK(result.z == lhs.z - 2);
        }

        SECTION("Self") {
            Point result = lhs - lhs;
            CHECK(result.x == 0);
            CHECK(result.y == 0);
            CHECK(result.z == 0);
        }
        // lhs did not change, all these methods are const
        REQUIRE(lhs.x == 1);
        REQUIRE(lhs.y == -2);
        REQUIRE(lhs.z == 3);
    }

    SECTION("Auto-subtraction") {
        Point lhs(1, -2, 3);
        Point rhs;
        SECTION("Zero") {
            rhs = {0, 0, 0};
            lhs -= rhs;
            CHECK(lhs.x == 1);
            CHECK(lhs.y == -2);
            CHECK(lhs.z == 3);
        }

        SECTION("Negative") {
            rhs = {-2, -2, -2};
            lhs -= rhs;
            CHECK(lhs.x == 1 + 2);
            CHECK(lhs.y == -2 + 2);
            CHECK(lhs.z == 3 + 2);
        }

        SECTION("Positive") {
            rhs = {2, 2, 2};
            lhs -= rhs;
            CHECK(lhs.x == 1 - 2);
            CHECK(lhs.y == -2 - 2);
            CHECK(lhs.z == 3 - 2);
        }

        SECTION("Self") {
            lhs -= lhs;
            CHECK(lhs.x == 0);
            CHECK(lhs.y == 0);
            CHECK(lhs.z == 0);
        }
    }

    SECTION("Multiplication") {
        Point lhs(1, -2, 3);
        double rhs;
        SECTION("Zero") {
            rhs = 0;
            Point result = lhs * rhs;
            CHECK(result.x == 0);
            CHECK(result.y == 0);
            CHECK(result.z == 0);
        }

        SECTION("One") {
            rhs = 1;
            Point result = lhs * rhs;
            CHECK(result.x == lhs.x);
            CHECK(result.y == lhs.y);
            CHECK(result.z == lhs.z);
        }

        SECTION("Negative") {
            rhs = -2;
            Point result = lhs * rhs;
            CHECK(result.x == lhs.x * -2);
            CHECK(result.y == lhs.y * -2);
            CHECK(result.z == lhs.z * -2);
        }

        SECTION("Positive") {
            rhs = 2;
            Point result = lhs * rhs;
            CHECK(result.x == lhs.x * 2);
            CHECK(result.y == lhs.y * 2);
            CHECK(result.z == lhs.z * 2);
        }

        // lhs did not change, all these methods are const
        REQUIRE(lhs.x == 1);
        REQUIRE(lhs.y == -2);
        REQUIRE(lhs.z == 3);
    }

    SECTION("Auto-multiplication") {
        Point lhs(1, -2, 3);
        double rhs;
        SECTION("Zero") {
            rhs = 0;
            lhs *= rhs;
            CHECK(lhs.x == 0);
            CHECK(lhs.y == 0);
            CHECK(lhs.z == 0);
        }

        SECTION("One") {
            rhs = 1;
            lhs *= rhs;
            CHECK(lhs.x == 1);
            CHECK(lhs.y == -2);
            CHECK(lhs.z == 3);
        }

        SECTION("Negative") {
            rhs = -2;
            lhs *= rhs;
            CHECK(lhs.x == 1 * -2);
            CHECK(lhs.y == -2 * -2);
            CHECK(lhs.z == 3 * -2);
        }

        SECTION("Positive") {
            rhs = 2;
            lhs *= rhs;
            CHECK(lhs.x == 1 * 2);
            CHECK(lhs.y == -2 * 2);
            CHECK(lhs.z == 3 * 2);
        }
    }

    SECTION("Division") {
        Point lhs(1, -2, 3);
        double rhs;
        SECTION("Zero") {
            rhs = 0;
            Point result = lhs / rhs;
            CHECK(std::isinf(result.x) == true);
            CHECK(std::isinf(result.y) == true);
            CHECK(std::isinf(result.z) == true);
        }

        SECTION("One") {
            rhs = 1;
            Point result = lhs / rhs;
            CHECK(result.x == 1);
            CHECK(result.y == -2);
            CHECK(result.z == 3);
        }

        SECTION("Negative one") {
            rhs = -1;
            Point result = lhs / rhs;
            CHECK(result.x == -1);
            CHECK(result.y == 2);
            CHECK(result.z == -3);
        }

        SECTION("Negative") {
            rhs = -2;
            Point result = lhs / rhs;
            CHECK(result.x == lhs.x / -2);
            CHECK(result.y == lhs.y / -2);
            CHECK(result.z == lhs.z / -2);
        }

        SECTION("Positive") {
            rhs = 2;
            Point result = lhs / rhs;
            CHECK(result.x == lhs.x / 2);
            CHECK(result.y == lhs.y / 2);
            CHECK(result.z == lhs.z / 2);
        }

        // lhs did not change, all these methods are const
        REQUIRE(lhs.x == 1);
        REQUIRE(lhs.y == -2);
        REQUIRE(lhs.z == 3);
    }

    SECTION("Auto-division") {
        Point lhs(1, -2, 3);
        double rhs;
        SECTION("Zero") {
            rhs = 0;
            lhs /= rhs;
            CHECK(std::isinf(lhs.x) == true);
            CHECK(std::isinf(lhs.y) == true);
            CHECK(std::isinf(lhs.z) == true);
        }

        SECTION("One") {
            rhs = 1;
            lhs /= rhs;
            CHECK(lhs.x == 1);
            CHECK(lhs.y == -2);
            CHECK(lhs.z == 3);
        }

        SECTION("Negative one") {
            rhs = -1;
            lhs /= rhs;
            CHECK(lhs.x == -1);
            CHECK(lhs.y == 2);
            CHECK(lhs.z == -3);
        }

        SECTION("Negative") {
            rhs = -2;
            lhs /= rhs;
            CHECK(lhs.x == 1.0 / -2.0);
            CHECK(lhs.y == -2.0 / -2.0);
            CHECK(lhs.z == 3.0 / -2.0);
        }

        SECTION("Positive") {
            rhs = 2;
            lhs /= rhs;
            CHECK(lhs.x == 1.0 / 2.0);
            CHECK(lhs.y == -2.0 / 2.0);
            CHECK(lhs.z == 3.0 / 2.0);
        }
    }
}

TEST_CASE("Normal vector", "[geometry]") {
    SECTION("90 degrees, flat z") {
        Point a = {0, 0, 0};
        Point b = {1, 1, 0};
        Point c = {2, 0, 0};
        SECTION("Left to right") {
            Point norm = normal(a, b, c);
            CHECK(norm.x == 0);
            CHECK(norm.y == 0);
            CHECK(norm.z == -1);
        }
        SECTION("Right to left") {
            Point norm = normal(c, b, a);
            CHECK(norm.x == 0);
            CHECK(norm.y == 0);
            CHECK(norm.z == 1);
        }
    }

    SECTION("270 degrees, flat z") { // Opposite result of 90 degrees
        Point a = {0, 0, 0};
        Point b = {-1, -1, 0};
        Point c = {2, 0, 0};
        Point norm;
        SECTION("Left to right") {
            norm = normal(a, b, c);
            CHECK(norm.x == 0);
            CHECK(norm.y == 0);
            CHECK(norm.z == 1);
        }
        SECTION("Right to left") {
            norm = normal(c, b, a);
            CHECK(norm.x == 0);
            CHECK(norm.y == 0);
            CHECK(norm.z == -1);
        }
        CHECK(norm.norm() == 1);
    }

    SECTION("Arbitrary") {
        Point a = {3, -2, 8};
        Point b = {-4, 11, 0};
        Point c = {9, -5, -5};
        Point norm = normal(a, b, c);
        compareReals(norm.x, -0.78911034102321065);
        compareReals(norm.y, -0.56832299172137968);
        compareReals(norm.z, -0.23305331315193267);
        compareReals(norm.norm(), 1.0);
    }

    SECTION("Equal") {
        Point a = {3, -2, 8};
        Point norm = normal(a, a, a);
        CHECK(norm.x == 0);
        CHECK(norm.y == 0);
        CHECK(norm.z == 1);
    }
}