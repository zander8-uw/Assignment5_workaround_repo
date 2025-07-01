#include <TestHarness.h>

#include "Arrow.h"

#include "TestHelperTestEnvironment.h"

namespace TestHuntTheWumpus
{
    TEST(ArrowSuite, Arrow_HasProperAttributes)
    {
        TestEnvironment env;

        const HuntTheWumpus::Arrow arrow(0, env.m_context);

        const auto &properties = arrow.Properties();

        CHECK(!properties.m_carryableByBats);
        CHECK(!properties.m_isEdible);
        CHECK(!properties.m_reportMovement);
        CHECK(properties.m_fatalToHunter);
        CHECK(properties.m_fatalToWumpus);

        CHECK_EQUAL(2, arrow.GetPriority());

        const auto& id = arrow.GetIdentifier();

        CHECK_EQUAL(HuntTheWumpus::Category::Arrow, id.m_category);
    }
}
