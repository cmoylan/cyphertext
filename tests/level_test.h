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
        TS_TRACE("for a 1x1 character");
        Vector2D size = {1, 1};

        printf("-100 -100 ");
        TS_ASSERT_EQUALS(level->isBlocked(-100, -100, size), true);

        //printf("-0 -100");
        //TS_ASSERT_EQUALS(level->isBlocked(0, -100, size), false);

        //printf("-100 -50");
        //TS_ASSERT_EQUALS(level->isBlocked(-100, -50, size), true);

        //printf("75 75");
        //TS_ASSERT_EQUALS(level->isBlocked(75, 75, size), true);
    }

};
