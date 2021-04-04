#include "catch2/catch.hpp"
#include "testCommon.h"
#include "geometry.h"

using namespace geometry;

TEST_CASE("Vec3 construction", "[geometry]") {
    SECTION("3 args") {
        Vec3 p = {1.2, -2.3, 3.4};
        CHECK(p.x == 1.2);
        CHECK(p.y == -2.3);
        CHECK(p.z == 3.4);
    }

    SECTION("Copy") {
        Vec3 p = {1, 2, 3};
        Vec3 q = {p};
        p.x += 1;
        CHECK(p.x == 2.0); // changed, but q didn't
        CHECK(p.y == 2.0);
        CHECK(p.z == 3.0);
        CHECK(q.x == 1.0);
        CHECK(q.y == 2.0);
        CHECK(q.z == 3.0);
    }

    SECTION("Assign") {
        Vec3 p = {1, 2, 3};
        Vec3 q = p;
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

TEST_CASE("Vec3 const-methods", "[geometry]") {
    SECTION("Dot product") {
        SECTION("Zero lhs") {
            Vec3 p = {0, 0, 0};
            Vec3 q = {-5, -6, 7};
            double dot = p.dot(q);
            CHECK(dot == 0.0); // Anything dot 0 is zero
        }

        SECTION("Zero rhs") {
            Vec3 p = {1, -2, 3};
            Vec3 q = {0, 0, 0};
            double dot = p.dot(q);
            CHECK(dot == 0.0); // Anything dot 0 is zero
        }

        SECTION("Non zero") {
            Vec3 p = {1, -2, 3};
            Vec3 q = {-9, -6, 11};
            double dot = p.dot(q);
            compareReals(dot, 36.0); // -9 + 12 + 33 = 36
        }

        SECTION("Self") {
            Vec3 p = {1, -2, 3};
            double dot = p.dot(p);
            compareReals(dot, 14.0); // 1 + 4 + 9 = 14
        }
    }

    SECTION("Cross product") {
        SECTION("Zero lhs") {
            Vec3 p = {0, 0, 0};
            Vec3 q = {-5, -6, 7};
            Vec3 r = p.cross(q);
            CHECK(r.x == 0.0);
            CHECK(r.y == 0.0);
            CHECK(r.z == 0.0);
        }

        SECTION("Zero rhs") {
            Vec3 p = {1, -2, 3};
            Vec3 q = {0, 0, 0};
            Vec3 r = p.cross(q);
            CHECK(r.x == 0.0);
            CHECK(r.y == 0.0);
            CHECK(r.z == 0.0);
        }

        SECTION("Non zero") {
            Vec3 p = {1, -2, 3};
            Vec3 q = {-9, -6, 11};
            Vec3 r = p.cross(q);
            compareReals(r.x, -4.0);
            compareReals(r.y, -38.0);
            compareReals(r.z, -24.0);
        }
    }

    SECTION("2-norm") {
        SECTION("Zero") {
            Vec3 p = {0, 0, 0};
            double norm = p.norm();
            CHECK(norm == 0.0);
        }

        SECTION("Unit vector") {
            Vec3 p = {0, 0, 0};
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
            Vec3 p = {-9, -6, 11};
            double norm = p.norm();
            compareReals(norm, 15.427248620541512);
        }
    }
}

TEST_CASE("Vec3 math operations", "[geometry]") {
    SECTION("Negation") {
        Vec3 lhs = {1, -2, 3};
        Vec3 res = -lhs;
        CHECK(res.x == -lhs.x);
        CHECK(res.y == -lhs.y);
        CHECK(res.z == -lhs.z);
    }

    SECTION("Addition") {
        Vec3 lhs = {1, -2, 3};
        Vec3 rhs;
        SECTION("Zero") {
            rhs = {0, 0, 0};
            Vec3 result = lhs + rhs;
            CHECK(result.x == lhs.x);
            CHECK(result.y == lhs.y);
            CHECK(result.z == lhs.z);
        }

        SECTION("Negative") {
            rhs = {-2, -2, -2};
            Vec3 result = lhs + rhs;
            CHECK(result.x == lhs.x - 2);
            CHECK(result.y == lhs.y - 2);
            CHECK(result.z == lhs.z - 2);
        }

        SECTION("Positive") {
            rhs = {2, 2, 2};
            Vec3 result = lhs + rhs;
            CHECK(result.x == lhs.x + 2);
            CHECK(result.y == lhs.y + 2);
            CHECK(result.z == lhs.z + 2);
        }

        SECTION("Self") {
            Vec3 result = lhs + lhs;
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
        Vec3 lhs = {1, -2, 3};
        Vec3 rhs;
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
        Vec3 lhs = {1, -2, 3};
        Vec3 rhs;
        SECTION("Zero") {
            rhs = {0, 0, 0};
            Vec3 result = lhs - rhs;
            CHECK(result.x == lhs.x);
            CHECK(result.y == lhs.y);
            CHECK(result.z == lhs.z);
        }

        SECTION("Negative") {
            rhs = {-2, -2, -2};
            Vec3 result = lhs - rhs;
            CHECK(result.x == lhs.x + 2);
            CHECK(result.y == lhs.y + 2);
            CHECK(result.z == lhs.z + 2);
        }

        SECTION("Positive") {
            rhs = {2, 2, 2};
            Vec3 result = lhs - rhs;
            CHECK(result.x == lhs.x - 2);
            CHECK(result.y == lhs.y - 2);
            CHECK(result.z == lhs.z - 2);
        }

        SECTION("Self") {
            Vec3 result = lhs - lhs;
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
        Vec3 lhs = {1, -2, 3};
        Vec3 rhs;
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
        Vec3 lhs = {1, -2, 3};
        double rhs;
        SECTION("Zero") {
            rhs = 0;
            Vec3 result = lhs * rhs;
            CHECK(result.x == 0);
            CHECK(result.y == 0);
            CHECK(result.z == 0);
        }

        SECTION("One") {
            rhs = 1;
            Vec3 result = lhs * rhs;
            CHECK(result.x == lhs.x);
            CHECK(result.y == lhs.y);
            CHECK(result.z == lhs.z);
        }

        SECTION("Negative") {
            rhs = -2;
            Vec3 result = lhs * rhs;
            CHECK(result.x == lhs.x * -2);
            CHECK(result.y == lhs.y * -2);
            CHECK(result.z == lhs.z * -2);
        }

        SECTION("Positive") {
            rhs = 2;
            Vec3 result = lhs * rhs;
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
        Vec3 lhs = {1, -2, 3};
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
        Vec3 lhs = {1, -2, 3};
        double rhs;
        SECTION("Zero") {
            rhs = 0;
            Vec3 result = lhs / rhs;
            CHECK(std::isinf(result.x) == true);
            CHECK(std::isinf(result.y) == true);
            CHECK(std::isinf(result.z) == true);
        }

        SECTION("One") {
            rhs = 1;
            Vec3 result = lhs / rhs;
            CHECK(result.x == 1);
            CHECK(result.y == -2);
            CHECK(result.z == 3);
        }

        SECTION("Negative one") {
            rhs = -1;
            Vec3 result = lhs / rhs;
            CHECK(result.x == -1);
            CHECK(result.y == 2);
            CHECK(result.z == -3);
        }

        SECTION("Negative") {
            rhs = -2;
            Vec3 result = lhs / rhs;
            CHECK(result.x == lhs.x / -2);
            CHECK(result.y == lhs.y / -2);
            CHECK(result.z == lhs.z / -2);
        }

        SECTION("Positive") {
            rhs = 2;
            Vec3 result = lhs / rhs;
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
        Vec3 lhs = {1, -2, 3};
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
        Vec3 a = {0, 0, 0};
        Vec3 b = {1, 1, 0};
        Vec3 c = {2, 0, 0};
        SECTION("Left to right") {
            Vec3 norm = normal(a, b, c);
            CHECK(norm.x == 0);
            CHECK(norm.y == 0);
            CHECK(norm.z == -1);
        }
        SECTION("Right to left") {
            Vec3 norm = normal(c, b, a);
            CHECK(norm.x == 0);
            CHECK(norm.y == 0);
            CHECK(norm.z == 1);
        }
    }

    SECTION("270 degrees, flat z") { // Opposite result of 90 degrees
        Vec3 a = {0, 0, 0};
        Vec3 b = {-1, -1, 0};
        Vec3 c = {2, 0, 0};
        Vec3 norm;
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
        Vec3 a = {3, -2, 8};
        Vec3 b = {-4, 11, 0};
        Vec3 c = {9, -5, -5};
        Vec3 norm = normal(a, b, c);
        compareReals(norm.x, -0.78911034102321065);
        compareReals(norm.y, -0.56832299172137968);
        compareReals(norm.z, -0.23305331315193267);
        compareReals(norm.norm(), 1.0);
    }

    SECTION("Equal") {
        Vec3 a = {3, -2, 8};
        Vec3 norm = normal(a, a, a);
        CHECK(norm.x == 0);
        CHECK(norm.y == 0);
        CHECK(norm.z == 1);
    }
}