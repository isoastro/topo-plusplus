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