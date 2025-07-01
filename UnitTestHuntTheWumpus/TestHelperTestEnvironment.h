#pragma once

#include "TestHelperGameState.h"
#include "TestHelperRandomProvider.h"
#include "TestHelperDungeon.h"
#include "UserNotification.h"

#include "Context.h"

namespace TestHuntTheWumpus
{
    struct TestEnvironment
    {
        TestEnvironment();

        TestRandomProvider m_provider;
        HuntTheWumpus::UserNotification m_userNotifier;
        TestGameState m_state;

        HuntTheWumpus::Context m_context{ .m_notification = m_userNotifier, .m_random = m_provider, .m_change = m_state };

        TestDungeon m_dungeon;
    };
}
