#include "UserNotification.h"
#include <stdexcept>

namespace
{
    template<typename Callback, typename CallbackArg> void DoCallback(const std::unordered_map<HuntTheWumpus::UserNotification::Notification, HuntTheWumpus::UserNotification::CallbackData>& callbacks, const HuntTheWumpus::UserNotification::Notification callbackId, const CallbackArg& arg)
    {
        const auto callback = callbacks.find(callbackId);

        const auto callbackFunc = std::get_if<Callback>(&callback->second);
        (*callbackFunc)(arg);
    }
}

namespace HuntTheWumpus
{
    void UserNotification::AddCallback(const Notification category, std::function<void()>&& callback)
    {
        if (callback == nullptr)
        {
            throw std::invalid_argument("callback is null, bad input.");
        }

        m_callbacks.insert_or_assign(category, std::move(callback));
    }

    void UserNotification::AddCallback(const Notification category, std::function<void(int)>&& callback)
    {
        if (callback == nullptr)
        {
            throw std::invalid_argument("callback is null, bad input.");
        }

        m_callbacks.insert_or_assign(category, std::move(callback));
    }

    void UserNotification::AddCallback(const Notification category, std::function<void(const std::vector<int>&)>&& callback)
    {
        if (callback == nullptr)
        {
            throw std::invalid_argument("callback is null, bad input.");
        }

        m_callbacks.insert_or_assign(category, std::move(callback));
    }

    void UserNotification::Notify(const Notification category) const
    {
        const auto callback = m_callbacks.find(category);

        const auto callbackFunc = std::get_if<std::function<void()>>(&callback->second);
        (*callbackFunc)();
    }

    void UserNotification::Notify(const Notification category, const int arg) const
    {
        DoCallback<std::function<void(int)>, int>(m_callbacks, category, arg);
    }

    void UserNotification::Notify(const Notification category, const std::vector<int>& arg) const
    {
        DoCallback<std::function<void(const std::vector<int> &)>, std::vector<int>>(m_callbacks, category, arg);
    }
}
