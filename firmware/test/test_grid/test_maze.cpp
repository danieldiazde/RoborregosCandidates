extern "C" {
#include <unity.h>
}

#include "Maze.hpp"

using namespace maze;

void test_shared_wall() {
    Maze m;

    m.setWall(1, 1, Direction::East, WallState::Blocked);

    TEST_ASSERT_EQUAL(
        WallState::Blocked,
        m.wall(2, 1, Direction::West)
    );

    m.setWall(2, 1, Direction::West, WallState::Open);

    TEST_ASSERT_EQUAL(
        WallState::Open,
        m.wall(1, 1, Direction::East)
    );
}

void test_shared_wall_vertical() {
    Maze m;

    m.setWall(3, 2, Direction::North, WallState::Blocked);

    TEST_ASSERT_EQUAL(
        WallState::Blocked,
        m.wall(3, 3, Direction::South)
    );
}

void test_boundary_is_blocked() {
    Maze m;

    TEST_ASSERT_EQUAL(
        WallState::Blocked,
        m.wall(0, 0, Direction::West)
    );

    TEST_ASSERT_EQUAL(
        WallState::Blocked,
        m.wall(0, 0, Direction::South)
    );

    TEST_ASSERT_EQUAL(
        WallState::Blocked,
        m.wall(4, 4, Direction::North)
    );

    TEST_ASSERT_EQUAL(
        WallState::Blocked,
        m.wall(4, 4, Direction::East)
    );
}

void test_interior_starts_unknown() {
    Maze m;

    TEST_ASSERT_EQUAL(
        WallState::Unknown,
        m.wall(2, 2, Direction::North)
    );
}

void test_unknown_is_not_passable() {
    Maze m;

    TEST_ASSERT_FALSE(
        m.canMove(2, 2, Direction::North)
    );

    m.setWall(
        2,
        2,
        Direction::North,
        WallState::Open
    );

    TEST_ASSERT_TRUE(
        m.canMove(2, 2, Direction::North)
    );

    m.setWall(
        2,
        2,
        Direction::North,
        WallState::Blocked
    );

    TEST_ASSERT_FALSE(
        m.canMove(2, 2, Direction::North)
    );
}

void test_cannot_leave_the_maze() {
    Maze m;

    m.setWall(
        0,
        0,
        Direction::West,
        WallState::Open
    );

    TEST_ASSERT_FALSE(
        m.canMove(0, 0, Direction::West)
    );
}

void test_out_of_range_is_safe() {
    Maze m;

    TEST_ASSERT_EQUAL(
        WallState::Blocked,
        m.wall(9, 9, Direction::North)
    );

    TEST_ASSERT_EQUAL(
        WallState::Blocked,
        m.wall(-1, 0, Direction::East)
    );

    m.setWall(
        -1,
        0,
        Direction::East,
        WallState::Open
    );

    m.setWall(
        0,
        9,
        Direction::North,
        WallState::Open
    );

    TEST_ASSERT_EQUAL(
        WallState::Blocked,
        m.wall(0, 0, Direction::West)
    );
}

void test_contains() {
    Maze m;

    TEST_ASSERT_TRUE(m.contains(0, 0));
    TEST_ASSERT_TRUE(m.contains(4, 4));

    TEST_ASSERT_FALSE(m.contains(5, 0));
    TEST_ASSERT_FALSE(m.contains(-1, 0));
}