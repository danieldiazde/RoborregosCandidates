extern "C" {
#include <unity.h>
}

void setUp() {}
void tearDown() {}

// Cell tests
void test_fresh_cell_is_unvisited();
void test_mark_visited_works();

// Maze tests
void test_shared_wall();
void test_shared_wall_vertical();
void test_boundary_is_blocked();
void test_interior_starts_unknown();
void test_unknown_is_not_passable();
void test_cannot_leave_the_maze();
void test_out_of_range_is_safe();
void test_contains();

int main(int, char**) {
    UNITY_BEGIN();

    RUN_TEST(test_shared_wall);
    RUN_TEST(test_shared_wall_vertical);
    RUN_TEST(test_boundary_is_blocked);
    RUN_TEST(test_interior_starts_unknown);
    RUN_TEST(test_unknown_is_not_passable);
    RUN_TEST(test_cannot_leave_the_maze);
    RUN_TEST(test_out_of_range_is_safe);
    RUN_TEST(test_contains);

    RUN_TEST(test_fresh_cell_is_unvisited);
    RUN_TEST(test_mark_visited_works);

    return UNITY_END();
}