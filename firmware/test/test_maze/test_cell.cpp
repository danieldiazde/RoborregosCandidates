extern "C" {
#include <unity.h>
}

#include "Cell.hpp"

using namespace maze;

void test_fresh_cell_is_unvisited() {
    Cell c;

    TEST_ASSERT_FALSE(c.isVisited());
}

void test_mark_visited_works() {
    Cell c;

    c.markVisited();

    TEST_ASSERT_TRUE(c.isVisited());
}