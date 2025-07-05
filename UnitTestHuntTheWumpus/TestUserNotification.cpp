#include <TestHarness.h>

#include "Dungeon.h"

#include "Cave.h"

#include "Bat.h"
#include "Pit.h"
#include "Wumpus.h"

#include "UserNotification.h"

#include "TestHelperTestEnvironment.h"

namespace TestHuntTheWumpus
{
    TEST(TestEnvironmentSuite, Test_AddCallback_overload_variant_1)
    {
        TestEnvironment env;
        std::string errorMessage = "This a test for callbacks.";
        std::function<void()> emptyFunction;

        try
        {
            env.m_userNotifier.AddCallback(HuntTheWumpus::UserNotification::Notification::TEST_ERROR, std::move(emptyFunction));
        }
        catch (const std::invalid_argument& e)
        {
            errorMessage = e.what();
        }

        CHECK_EQUAL("callback is null, bad input.", errorMessage);
    }


    TEST(TestEnvironmentSuite, Test_AddCallback_overload_variant_2)
    {
        TestEnvironment env;
        std::string errorMessage = "This a test for callbacks.";
        std::function<void(int)> emptyFunction;

        try
        {
            env.m_userNotifier.AddCallback(HuntTheWumpus::UserNotification::Notification::TEST_ERROR, std::move(emptyFunction));
        }
        catch (const std::invalid_argument& e)
        {
            errorMessage = e.what();
        }

        CHECK_EQUAL("callback is null, bad input.", errorMessage);
    }


    TEST(TestEnvironmentSuite, Test_AddCallback_overload_variant_3)
    {
        TestEnvironment env;
        std::string errorMessage = "This a test for callbacks.";
        std::function<void(const std::vector<int>&)> emptyFunction;

        try
        {
            env.m_userNotifier.AddCallback(HuntTheWumpus::UserNotification::Notification::TEST_ERROR, std::move(emptyFunction));
        }
        catch (const std::invalid_argument& e)
        {
            errorMessage = e.what();
        }

        CHECK_EQUAL("callback is null, bad input.", errorMessage);
    }

}