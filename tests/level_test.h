#include <cxxtest/TestSuite.h>

#include "Level.h"

class LevelTest : public CxxTest::TestSuite {

    Level *level;

public:
    void setUp()
    {
        level  = new Level();

        int platforms[] = {
            0, 0, 0, 1,
            0, 1, 0, 0,
            1, 0, 1, 0,
            0, 0, 0, 0
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
        Vector2D size = {1, 1};

        //TS_TRACE();
        TS_ASSERT_EQUALS(level->isBlocked(-100, -100, size), false);

        //TS_TRACE();
        TS_ASSERT(level->isBlocked(100, -100, size));

        //
        TS_ASSERT(level->isBlocked(-25, 25, size));

	TS_ASSERT_EQUALS(level->isBlocked(10,10,size), false);
    }

};
