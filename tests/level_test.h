#include <cxxtest/TestSuite.h>

#include "Level.h"

class LevelTest : public CxxTest::TestSuite {

    Level *level;

 public:
    void setUp()
    {
	level  = new Level();
    }

    void tearDown()
    {
        delete level;
    }

    void test_it(void)
    {
        TS_ASSERT_EQUALS(1, 1);
    }

};
