#pragma once

namespace HuntTheWumpus
{
    class UserNotification;
    class IRandomProvider;
    class IGameStateChange;

    struct Context
    {
        UserNotification &m_notification; // NOLINT(cppcoreguidelines-avoid-const-or-ref-data-members)
        IRandomProvider &m_random; // NOLINT(cppcoreguidelines-avoid-const-or-ref-data-members)
        IGameStateChange &m_change; // NOLINT(cppcoreguidelines-avoid-const-or-ref-data-members)
    };
}
