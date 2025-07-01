#include "Pit.h"

#include "Context.h"
#include "UserNotification.h"
#include "GameStateObservation.h"

namespace HuntTheWumpus
{
    Pit::Pit(const int pitInstance, Context& providers)
        : Denizen(
            { .m_category = Category::Pit, .m_instance = pitInstance },
            { .m_carryableByBats = false, .m_fatalToWumpus = false, .m_fatalToHunter = true, .m_isEdible = false, .m_reportMovement = false },
            providers)
    {
    }

    bool Pit::ObserveCaveEntrance(const std::shared_ptr<Denizen>& trigger)
    {
        if (trigger->Properties().m_isEdible)
        {
            m_providers.m_notification.Notify(UserNotification::Notification::PitTriggered);
            m_providers.m_change.GameOver(false);

            return true;
        }

        return false;
    }

    void Pit::ReportPresence() const
    {
        m_providers.m_notification.Notify(UserNotification::Notification::ObservePit);
    }
}
