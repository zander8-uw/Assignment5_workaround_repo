#include <TestHarness.h>

#include "TestHelperTestEnvironment.h"

#include "Denizen.h"

#include "Bat.h"
#include "Wumpus.h"
#include "Pit.h"
#include "Hunter.h"
#include "Arrow.h"

namespace TestHuntTheWumpus
{
    TEST(DenizenSuite, TestIdentifier_Comparison_ProperSorting)
    {
        constexpr HuntTheWumpus::DenizenIdentifier thing1{ .m_category = HuntTheWumpus::Category::Wumpus, .m_instance = 0 }; // Wumpus == 0 
        constexpr HuntTheWumpus::DenizenIdentifier thing2{ .m_category = HuntTheWumpus::Category::Arrow, .m_instance = 0 };
        // Arrow = 4
        constexpr HuntTheWumpus::DenizenIdentifier thing3{ .m_category = HuntTheWumpus::Category::Arrow, .m_instance = 1 };

        CHECK((thing1 <=> thing2) == std::strong_ordering::less);
        CHECK((thing2 <=> thing1) == std::strong_ordering::greater);
        CHECK((thing2 <=> thing3) == std::strong_ordering::less);
        CHECK((thing3 <=> thing2) == std::strong_ordering::greater);
        CHECK((thing3 <=> thing3) == std::strong_ordering::equal);
    }

    TEST(DenizenSuite, DenizenIdentifier_Hashing_UniqueHashes)
    {
        constexpr HuntTheWumpus::DenizenIdentifier thing1{ .m_category = HuntTheWumpus::Category::Arrow, .m_instance = 0 };
        constexpr HuntTheWumpus::DenizenIdentifier thing2{ .m_category = HuntTheWumpus::Category::Bat, .m_instance = 0 };
        constexpr HuntTheWumpus::DenizenIdentifier thing3{ .m_category = HuntTheWumpus::Category::Bat, .m_instance = 1 };
        constexpr HuntTheWumpus::DenizenIdentifier thing4{ .m_category = HuntTheWumpus::Category::Bat, .m_instance = 1 };

        constexpr HuntTheWumpus::DenizenIdentifierHasher hasher;

        const auto hash1 = hasher(thing1);
        const auto hash2 = hasher(thing2);
        const auto hash3 = hasher(thing3);
        const auto hash4 = hasher(thing4);

        CHECK(hash1 != hash2);
        CHECK(hash2 != hash3);
        CHECK_EQUAL(hash3, hash4);
    }

    TEST(DenizenSuite, DenizenPriorities_Proper)
    {
        // Make denizens, assert the priorities are properly ordered.
        TestEnvironment env;

        const HuntTheWumpus::Bat bat(0, env.m_context);
        const HuntTheWumpus::Pit pit(0, env.m_context);
        const HuntTheWumpus::Wumpus wumpus(0, env.m_context);
        const HuntTheWumpus::Arrow arrow(0, env.m_context);
        const HuntTheWumpus::Hunter hunter(env.m_context);

        // Requirement: Bat, then Pit, then Wumpus.
        CHECK(bat.GetPriority() > pit.GetPriority());
        CHECK(pit.GetPriority() > wumpus.GetPriority());
        CHECK(wumpus.GetPriority() > arrow.GetPriority());
        CHECK(wumpus.GetPriority() > hunter.GetPriority());
    }
}
