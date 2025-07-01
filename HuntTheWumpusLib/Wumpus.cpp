#include "Wumpus.h"

#include "Cave.h"
#include "Context.h"
#include "Dungeon.h"
#include "GameStateObservation.h"
#include "UserNotification.h"
#include "RandomProvider.h"

namespace HuntTheWumpus
{
    Wumpus::Wumpus(const int instance, Context& providers)
        : Denizen(
            { .m_category = Category::Wumpus, .m_instance = instance },
            { .m_carryableByBats = false, .m_fatalToWumpus = false, .m_fatalToHunter = true, .m_isEdible = false, .m_reportMovement = false },
            providers)
    {
    }

    bool Wumpus::ObserveCaveEntrance(const std::shared_ptr<Denizen>& trigger)
    {
        // Is this the player coming into the Wumpus' room?
        if (trigger->Properties().m_isEdible)
        {
            // The Wumpus wakes up when the Hunter enters its cave.
            m_providers.m_notification.Notify(UserNotification::Notification::WumpusTriggered);

            const auto moveProb = m_providers.m_random.MakeRandomNumber();

            if (moveProb >= 0.25f)
            {
                const auto curCave = m_cave.lock();
                curCave->GetDungeon().MoveDenizenRandomly(m_identifier);
                return true;
            }

            // Otherwise it stays put.
            m_providers.m_notification.Notify(UserNotification::Notification::HunterEaten);
            m_providers.m_change.GameOver(false);

            return true;
        }

        // Is this an arrow that shot us?
        if (trigger->Properties().m_fatalToWumpus)
        {
            m_providers.m_notification.Notify(UserNotification::Notification::WumpusShot);
            m_providers.m_change.GameOver(true);

            return true;
        }

        return false;
    }

    void Wumpus::ReportPresence() const
    {
        m_providers.m_notification.Notify(UserNotification::Notification::ObserveWumpus);
    }
}
