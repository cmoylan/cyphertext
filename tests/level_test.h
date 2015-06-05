#include <cxxtest/TestSuite.h>

#include "Level.h"

class LevelTest : public CxxTest::TestSuite {

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

        level->printPlatforms();
    }

    void tearDown()
    {
        delete level;
    }

    void test_valueAt_evens(void)
    {
        TS_ASSERT(level->valueAt(-100, 100) == 0);
        TS_ASSERT(level->valueAt(-50, 100) == 1);
        TS_ASSERT(level->valueAt(0, 100) == 2);
        TS_ASSERT(level->valueAt(50, 100) == 3);

        TS_ASSERT(level->valueAt(-100, 50) == 4);
        TS_ASSERT(level->valueAt(-50, 50) == 5);
        TS_ASSERT(level->valueAt(0, 50) == 6);
        TS_ASSERT(level->valueAt(50, 50) == 7);

        TS_ASSERT(level->valueAt(-100, 0) == 8);
        TS_ASSERT(level->valueAt(-50, 0) == 9);
        TS_ASSERT(level->valueAt(0, 0) == 10);
        TS_ASSERT(level->valueAt(50, 0) == 11);

        TS_ASSERT(level->valueAt(-100, -50) == 12);
        TS_ASSERT(level->valueAt(-50, -50) == 13);
        TS_ASSERT(level->valueAt(0, -50) == 14);
        TS_ASSERT(level->valueAt(50, -50) == 15);
    }

    void test_valueAt_odds(void)
    {
        // TODO: left off here
    }


    //void test_isBlocked(void)
    //{
    //    TS_TRACE("for a 1x1 character");
    //    Vector2D size = {1, 1};

    //    printf("\n-100 -100 ");
    //    TS_ASSERT(level->isBlocked(-100, -100, size) == true);
    //    printf("\n-51 -100 ");
    //    TS_ASSERT(level->isBlocked(-51, -100, size) == true);
    //    printf("\n-51 -51 ");
    //    //TS_ASSERT(level->isBlocked(-51, -51, size) == true);
    //    level->isBlocked(-51, -51, size);
    //    //printf("-50 -100 ");
    //    //TS_ASSERT(level->isBlocked(-50, -100, size) == false);

    //    //TS_ASSERT_EQUALS(level->isBlocked(-50, -50, size), false);

    //    //printf("-0 -100");
    //    //TS_ASSERT_EQUALS(level->isBlocked(0, -100, size), false);

    //    //printf("-100 -50");
    //    //TS_ASSERT_EQUALS(level->isBlocked(-100, -50, size), true);

    //    //printf("75 75");
    //    //TS_ASSERT_EQUALS(level->isBlocked(75, 75, size), true);
    //}

};
