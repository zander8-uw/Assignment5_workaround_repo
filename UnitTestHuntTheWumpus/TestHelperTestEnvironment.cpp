#include "TestHelperTestEnvironment.h"

namespace TestHuntTheWumpus
{
    TestEnvironment::TestEnvironment()
    {
        // Add in default notifications
        m_userNotifier.AddCallback(HuntTheWumpus::UserNotification::Notification::CaveEntered, [](const int) {});
        m_userNotifier.AddCallback(HuntTheWumpus::UserNotification::Notification::NeighboringCaves, [](const std::vector<int>&) {});
        m_userNotifier.AddCallback(HuntTheWumpus::UserNotification::Notification::ObserveBat, [&] {});
        m_userNotifier.AddCallback(HuntTheWumpus::UserNotification::Notification::ObservePit, [&] {});
        m_userNotifier.AddCallback(HuntTheWumpus::UserNotification::Notification::ObserveWumpus, [&] {});
        m_userNotifier.AddCallback(HuntTheWumpus::UserNotification::Notification::ObserveMiss, [&] { });
        m_userNotifier.AddCallback(HuntTheWumpus::UserNotification::Notification::WumpusAwoken, [&] { });
    }
}