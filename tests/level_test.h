#include <cxxtest/TestSuite.h>

#include "Level.h"

class LevelValueAtTest : public CxxTest::TestSuite {

    Level *level;

public:
    void setUp()
    {
        level  = new Level(4, 4);

        // TODO: abstract this into a helper:
        //       levelWithPlatforms(platformArray[])
        int platforms[] = {
            0, 1, 2, 3,
            4, 5, 6, 7,
            8, 9, 10, 11,
            12, 13, 14, 15
        };

        // TODO: this is a lot of setup, maybe make a test map
        Layer layer;
        layer.tiles.assign(platforms, platforms + 16);
        layer.tileCount = 16;

        level->layers["platforms"] = layer;
        level->mapWidth = 4;
        level->mapHeight = 4;

        //level->printPlatforms();
    }

    void tearDown()
    {
        delete level;
    }

    void test_valueAt_evens(void)
    {
        // 0,  1,  2,  3,
        // 4,  5,  6,  7,
        // 8,  9,  10, 11,
        // 12, 13, 14, 15

        //TS_ASSERT(level->valueAt(-100,100) == 0);
        TS_ASSERT(level->valueAt(-50, 100) == 1);
        TS_ASSERT(level->valueAt(0, 100) == 2);
        TS_ASSERT(level->valueAt(50, 100) == 3);

        TS_ASSERT(level->valueAt(-100, 50) == 0);
        TS_ASSERT(level->valueAt(-50, 50) == 1);
        TS_ASSERT(level->valueAt(0, 50) == 2);
        TS_ASSERT(level->valueAt(50, 50) == 3);

        TS_ASSERT(level->valueAt(-100, 0) == 4);
        TS_ASSERT(level->valueAt(-50, 0) == 5);
        TS_ASSERT(level->valueAt(0, 0) == 6);
        TS_ASSERT(level->valueAt(50, 0) == 7);

        TS_ASSERT(level->valueAt(-100, -50) == 8);
        TS_ASSERT(level->valueAt(-50, -50) == 9);
        TS_ASSERT(level->valueAt(0, -50) == 10);
        TS_ASSERT(level->valueAt(50, -50) == 11);

        TS_ASSERT(level->valueAt(-100, -100) == 12);
    }

    void test_valueAt_x_odds(void)
    {
        // 0,  1,  2,  3,
        // 4,  5,  6,  7,
        // 8,  9,  10, 11,
        // 12, 13, 14, 15
        // TODO: should we even both checking 100? It's technically off the map
        TS_ASSERT(level->valueAt(-75, 100) == 0);
        TS_ASSERT(level->valueAt(-25, 100) == 1);
        TS_ASSERT(level->valueAt(25, 100) == 2);
        TS_ASSERT(level->valueAt(75, 100) == 3);

        TS_ASSERT(level->valueAt(-75, 50) == 0);
        TS_ASSERT(level->valueAt(-25, 50) == 1);
        TS_ASSERT(level->valueAt(25, 50) == 2);
        TS_ASSERT(level->valueAt(75, 50) == 3);

        TS_ASSERT(level->valueAt(-75, 0) == 4);
        TS_ASSERT(level->valueAt(-25, 0) == 5);
        TS_ASSERT(level->valueAt(25, 0) == 6);
        TS_ASSERT(level->valueAt(75, 0) == 7);

        TS_ASSERT(level->valueAt(-75, -50) == 8);
        TS_ASSERT(level->valueAt(-25, -50) == 9);
        TS_ASSERT(level->valueAt(25, -50) == 10);
        TS_ASSERT(level->valueAt(75, -50) == 11);
    }

    void test_valueAt_y_odds(void)
    {
        // 0,  1,  2,  3,
        // 4,  5,  6,  7,
        // 8,  9,  10, 11,
        // 12, 13, 14, 15
        TS_ASSERT(level->valueAt(-100, 75) == 0);
        TS_ASSERT(level->valueAt(-50, 75) == 1);
        TS_ASSERT(level->valueAt(0, 75) == 2);
        TS_ASSERT(level->valueAt(50, 75) == 3);

        TS_ASSERT(level->valueAt(-100, 25) == 4);
        TS_ASSERT(level->valueAt(-50, 25) == 5);
        TS_ASSERT(level->valueAt(0, 25) == 6);
        TS_ASSERT(level->valueAt(50, 25) == 7);

        TS_ASSERT(level->valueAt(-100, -25) == 8);
        TS_ASSERT(level->valueAt(-50, -25) == 9);
        TS_ASSERT(level->valueAt(0, -25) == 10);
        TS_ASSERT(level->valueAt(50, -25) == 11);

        TS_ASSERT(level->valueAt(-100, -75) == 12);
        TS_ASSERT(level->valueAt(-50, -75) == 13);
        TS_ASSERT(level->valueAt(0, -75) == 14);
        TS_ASSERT(level->valueAt(50, -75) == 15);
    }

    void test_valueAt_xy_odds(void)
    {
        // 0,  1,  2,  3,
        // 4,  5,  6,  7,
        // 8,  9,  10, 11,
        // 12, 13, 14, 15
        TS_ASSERT(level->valueAt(-75, 75) == 0);
        TS_ASSERT(level->valueAt(-25, 75) == 1);
        TS_ASSERT(level->valueAt(25, 75) == 2);
        TS_ASSERT(level->valueAt(75, 75) == 3);

        TS_ASSERT(level->valueAt(-75, 25) == 4);
        TS_ASSERT(level->valueAt(-25, 25) == 5);
        TS_ASSERT(level->valueAt(25, 25) == 6);
        TS_ASSERT(level->valueAt(75, 25) == 7);

        TS_ASSERT(level->valueAt(-75, -25) == 8);
        TS_ASSERT(level->valueAt(-25, -25) == 9);
        TS_ASSERT(level->valueAt(25, -25) == 10);
        TS_ASSERT(level->valueAt(75, -25) == 11);

        TS_ASSERT(level->valueAt(-75, -75) == 12);
        TS_ASSERT(level->valueAt(-25, -75) == 13);
        TS_ASSERT(level->valueAt(25, -75) == 14);
        TS_ASSERT(level->valueAt(75, -75) == 15);
    }
};


class LevelIsBlockedTest : public CxxTest::TestSuite {

    Level *level;

public:
    void setUp()
    {
        level  = new Level(4, 4);

        // TODO: abstract this into a helper:
        //       levelWithPlatforms(platformArray[])
        int platforms[] = {
            0, 0, 0, 0,
            0, 0, 0, 0,
            0, 0, 0, 0,
            1, 0, 0, 0
        };

        // TODO: this is a lot of setup, maybe make a test map
        Layer layer;
        layer.tiles.assign(platforms, platforms + 16);
        layer.tileCount = 16;

        level->layers["platforms"] = layer;
        level->mapWidth = 4;
        level->mapHeight = 4;

        level->printPlatforms();
    }

    void tearDown()
    {
        delete level;
    }

    void test_isBlocked(void)
    {
        // 0, 0, 0, 0,
        // 0, 0, 0, 0,
        // 0, 0, 0, 0,
        // 1, 0, 0, 0
        TS_TRACE("for a 1x1 character");
        Vector2D size = {1, 1};
        TS_ASSERT(level->isBlocked(-99, -99, size) == true);
        TS_ASSERT(level->isBlocked(-51, -100, size) == true);
        TS_ASSERT(level->isBlocked(-51, -51, size) == true);
        TS_ASSERT_EQUALS(level->isBlocked(-50, -100, size), false);
        TS_ASSERT_EQUALS(level->isBlocked(-50, -50, size), false);

        //printf("-0 -100");
        //TS_ASSERT_EQUALS(level->isBlocked(0, -100, size), false);

        //printf("-100 -50");
        //TS_ASSERT_EQUALS(level->isBlocked(-100, -50, size), true);

        //printf("75 75");
        //TS_ASSERT_EQUALS(level->isBlocked(75, 75, size), true);
    }
};


class LevelCanFallTest : public CxxTest::TestSuite {

    Level *level;

public:
    void setUp()
    {
        level  = new Level(4, 4);

        // TODO: abstract this into a helper:
        //       levelWithPlatforms(platformArray[])
        int platforms[] = {
            0, 0, 0, 0,
            0, 0, 0, 0,
            0, 0, 0, 0,
            1, 1, 1, 1
        };

        Layer layer;
        layer.tiles.assign(platforms, platforms + 16);
        layer.tileCount = 16;

        level->layers["platforms"] = layer;
        level->mapWidth = 4;
        level->mapHeight = 4;

        //level->printPlatforms();
    }

    void tearDown()
    {
        delete level;
    }

    void test_canFall(void)
    {

        // 0, 0, 0, 0,
        // 0, 0, 0, 0,
        // 0, 0, 0, 0,
        // 1, 1, 1, 1
        //TS_TRACE("for a 1x1 character");
        //Vector2D size = {1, 1};
        //TS_ASSERT(level->isBlocked(-99, -99, size) == true);
        //TS_ASSERT(level->isBlocked(-51, -100, size) == true);
        //TS_ASSERT(level->isBlocked(-51, -51, size) == true);
        //TS_ASSERT_EQUALS(level->isBlocked(-50, -100, size), false);
        //TS_ASSERT_EQUALS(level->isBlocked(-50, -50, size), false);

        //printf("-0 -100");
        //TS_ASSERT_EQUALS(level->isBlocked(0, -100, size), false);

        //printf("-100 -50");
        //TS_ASSERT_EQUALS(level->isBlocked(-100, -50, size), true);

        //printf("75 75");
        //TS_ASSERT_EQUALS(level->isBlocked(75, 75, size), true);
    }
};
