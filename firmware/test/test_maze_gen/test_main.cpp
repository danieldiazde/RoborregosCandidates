// firmware/test/test_maze_gen/test_main.cpp
#include <unity.h>
#include "MazeGenerator.hpp"
#include "MazeValidator.hpp"

using namespace maze;

void setUp() {}
void tearDown() {}

static GeneratedMaze make(std::uint32_t seed, std::uint8_t extra) {
    GeneratorConfig c;
    c.seed = seed;
    c.extraOpenings = extra;
    return MazeGenerator(c).generate();
}

void test_validator_rejects_a_blank_maze() {
    GeneratedMaze bad{Maze{}, 0, 0, Direction::North};
    TEST_ASSERT_FALSE(validate(bad).ok);
}

void test_every_seed_is_valid() {
    for (std::uint32_t seed = 0; seed < 200; ++seed) {
        ValidationResult r = validate(make(seed, 8));
        TEST_ASSERT_TRUE_MESSAGE(r.ok, r.reason);
    }
}

void test_perfect_maze_is_a_spanning_tree() {
    for (std::uint32_t seed = 0; seed < 50; ++seed)
        TEST_ASSERT_EQUAL(24, countOpenInteriorWalls(make(seed, 0).maze));
}

void test_extra_openings_add_exactly_that_many() {
    for (std::uint32_t seed = 0; seed < 50; ++seed)
        TEST_ASSERT_EQUAL(24 + 5, countOpenInteriorWalls(make(seed, 5).maze));
}

void test_same_seed_same_maze() {
    GeneratedMaze a = make(42, 8), b = make(42, 8);
    for (std::int8_t x = 0; x < Maze::kSize; ++x)
        for (std::int8_t y = 0; y < Maze::kSize; ++y) {
            for (int i = 0; i < 4; ++i) {
                Direction d = static_cast<Direction>(i);
                TEST_ASSERT_EQUAL(static_cast<int>(a.maze.wall(x, y, d)),
                                  static_cast<int>(b.maze.wall(x, y, d)));
            }
            TEST_ASSERT_EQUAL(static_cast<int>(a.maze.cell(x, y).color()),
                              static_cast<int>(b.maze.cell(x, y).color()));
        }
}

void test_boundary_always_on_left() {
    for (std::uint32_t seed = 0; seed < 200; ++seed) {
        GeneratedMaze g = make(seed, 8);
        Direction left = turnLeft(g.startHeading);
        TEST_ASSERT_FALSE(g.maze.contains(g.startX + deltaX(left),
                                          g.startY + deltaY(left)));
    }
}

int main(int, char**) {
    UNITY_BEGIN();
    RUN_TEST(test_validator_rejects_a_blank_maze);
    RUN_TEST(test_every_seed_is_valid);
    RUN_TEST(test_perfect_maze_is_a_spanning_tree);
    RUN_TEST(test_extra_openings_add_exactly_that_many);
    RUN_TEST(test_same_seed_same_maze);
    RUN_TEST( test_boundary_always_on_left);
    return UNITY_END();
}