#pragma once

#include <cstdint>
#include <functional>
#include <variant>
#include <vector>

namespace HuntTheWumpus
{
    // Inform the user about particular operations happening.
    class UserNotification final
    {
    public:

        enum class Notification : uint8_t
        {
            ObserveWumpus,
            ObservePit,
            ObserveBat,
            ObserveMiss,
            ObserveOutOfArrows,
            BatTriggered,
            PitTriggered,
            WumpusTriggered,
            WumpusAwoken,
            WumpusShot,
            HunterEaten,
            HunterShot,
            CaveEntered,
            NeighboringCaves,
            ReportIllegalMove
        };

        UserNotification() = default;
        ~UserNotification() = default;

        void AddCallback(Notification category, std::function<void()>&& callback);
        void AddCallback(Notification, std::function<void(int)>&& callback);
        void AddCallback(Notification, std::function<void(const std::vector<int>&)>&& callback);

        void Notify(Notification category) const;
        void Notify(Notification category, int arg) const;
        void Notify(Notification category, const std::vector<int>& arg) const;

        UserNotification(const UserNotification&) = delete;
        UserNotification(UserNotification&&) noexcept = default;
        UserNotification& operator=(const UserNotification&) = delete;
        UserNotification& operator=(UserNotification&&) = delete;

        using CallbackData = std::variant<std::function<void()>, std::function<void(int)>, std::function<void(const std::vector<int>&)> >;

    private:

        std::unordered_map<Notification, CallbackData> m_callbacks;
    };
}
