#include "Dungeon.h"

#include "Cave.h"
#include "Bat.h"
#include "Wumpus.h"
#include "Hunter.h"
#include "Pit.h"
// ReSharper disable once CppUnusedIncludeDirective
#include "Arrow.h"

#include "Context.h"
#include "RandomProvider.h"
#include "GameStateObservation.h"
#include "UserNotification.h"

// ReSharper disable once CppUnusedIncludeDirective
#include <algorithm>

namespace HuntTheWumpus
{
    constexpr auto DungeonExtent = 20;

    Dungeon::Dungeon(Context& providers)
        : m_providers(providers)
    {
        Initialize(providers);
    }

    void Dungeon::Initialize(Context& providers)
    {
        for (auto idx = 1; idx <= DungeonExtent; ++idx)
        {
            m_caves.emplace(idx, std::make_shared<Cave>(idx, *this));
        }

        MakeTunnels();

        AddDenizen( std::make_shared<Bat>(0, m_providers) );
        AddDenizen( std::make_shared<Bat>(1, m_providers) );
        AddDenizen( std::make_shared<Wumpus>(0, m_providers) );
        AddDenizen( std::make_shared<Pit>(0, m_providers) );
        AddDenizen( std::make_shared<Pit>(1, m_providers) );

        // Put the hunter in a random  empty cave.
        auto hunterPlaced = false;

        while(!hunterPlaced)
        {
            const auto hunterCave = providers.m_random.MakeRandomCave();

            if(m_caves[hunterCave]->HasDenizens())
            {
                continue;
            }

            AddToCave(std::make_shared<Hunter>(providers), hunterCave, true);

            hunterPlaced = true;
        }
    }

    void Dungeon::AddDenizen(const std::shared_ptr<Denizen>& newDenizen)
    {
        auto denizenPlaced = false;

        while (!denizenPlaced)
        {
            const auto denizenCave = m_providers.m_random.MakeRandomCave();

            // Verify that cave does not have a like Denizen in it.
            if (m_caves[denizenCave]->HasDenizen(newDenizen->GetIdentifier()))
            {
                continue;
            }

            AddToCave(newDenizen, denizenCave, false);

            denizenPlaced = true;
        }
    }

    void Dungeon::MakeTunnels() const
    {
        const std::vector<std::pair<int, std::vector<int> > > tunnelPairs = {
            { 1, {2, 5, 8} },
            { 2, {1, 3, 10} },
            { 3, {2, 4, 12} },
            { 4, {3, 5, 14} },
            { 5, {1, 4, 6} },
            { 6, { 5, 7, 15} },
            { 7, {6, 8, 17} },
            { 8, {1, 7, 9} },
            { 9, {8, 10, 18} },
            { 10, {2, 9, 11} },
            { 11, {10, 12, 19} },
            { 12, {3, 11, 13} },
            { 13, {12, 14, 20 }  },
            { 14, {4, 13, 15} },
            { 15, {6, 14, 16} },
            { 16, {15, 17, 20} },
            { 17, {7, 16, 18} },
            { 18, {9, 17, 19} },
            { 19, {11, 18, 20} },
            { 20, {13, 16, 19} }
        };

        for (auto&& [caveId, neighborIds] : tunnelPairs)
        {
            const auto cave = m_caves.at(caveId);

            for (auto&& neighborId : neighborIds)
            {
                const auto neighbor = m_caves.at(neighborId);
                cave->ConnectTo(neighbor);
            }
        }
    }

    const std::shared_ptr<Cave>& Dungeon::FindCave(const int caveId)
    {
        const auto caveItr = m_caves.find(caveId);
        return caveItr->second;
    }

    void Dungeon::AddToCave(const std::shared_ptr<Denizen>& denizen, const int caveId, const bool observeEntrance)
    {
        m_caveDenizens.emplace(denizen->GetIdentifier(), denizen);

        const auto cave = m_caves.at(caveId);

        denizen->EnterCave(cave);
        cave->AddDenizen(denizen, observeEntrance);
    }

    void Dungeon::Move(const DenizenIdentifier& identifier, const int destinationCave)
    {
        const auto thing = m_caveDenizens[identifier];

        const auto startCave = thing->GetCurrentCave().lock();

        thing->RemoveFromCave();
        startCave->RemoveDenizen(identifier);

        const auto destCave = m_caves[destinationCave];

        thing->EnterCave(destCave);
        destCave->AddDenizen(thing, true);
    }

    bool Dungeon::LegalMove(const std::shared_ptr<Denizen>& denizen, const int destinationCave)
    {
        const auto denizenCave = denizen->GetCurrentCave().lock();
        const auto tunnelIds = denizenCave->GetConnectedIds();

        return std::ranges::find(tunnelIds, destinationCave) != tunnelIds.end();
    }

    void Dungeon::MakeMove(const DungeonMove operation, const std::vector<int>& destinationIds)
    {
        // First, find the hunter.
        const auto hunter = std::dynamic_pointer_cast<Hunter>(m_caveDenizens.at({ Category::Hunter, 0 }));

        const auto currentCave = hunter->GetCurrentCave().lock();

        if (operation == DungeonMove::Move)
        {
            // Check move validity.
            if (LegalMove(hunter, destinationIds.front()))
            {
                Move(hunter->GetIdentifier(), destinationIds.front());
            }
            else
            {
                m_providers.m_notification.Notify(UserNotification::Notification::ReportIllegalMove, destinationIds.front());
            }
        }

        if (operation == DungeonMove::Shoot)
        {
            // Retrieve an arrow.
            const auto arrow = hunter->GetArrow();

            const auto startCave = hunter->GetCurrentCave().lock();

            AddToCave(arrow, startCave->GetCaveId(), false);

            for (const auto caveId : destinationIds)
            {
                if (!LegalMove(arrow, caveId))
                {
                    MoveDenizenRandomly(arrow);
                }
                else
                {
                    Move(arrow->GetIdentifier(), caveId);
                }

                if (!m_providers.m_change.IsPlaying())
                {
                    break;
                }
            }

            // Once the arrow is done, remove it.
            const auto currentArrowCave = arrow->GetCurrentCave().lock();

            arrow->RemoveFromCave();
            currentArrowCave->RemoveDenizen(arrow->GetIdentifier());
            m_caveDenizens.erase(arrow->GetIdentifier());

            if (m_providers.m_change.IsPlaying())
            {
                // We missed the Wumpus, and we're not dead.
                m_providers.m_notification.Notify(UserNotification::Notification::ObserveMiss);

                // Move the wumpus.
                m_providers.m_notification.Notify(UserNotification::Notification::WumpusAwoken);

                MoveDenizenRandomly(m_caveDenizens.at({ Category::Wumpus, 0 }));
            }

            // The wumpus move could have ended the game, so check again.
            if (m_providers.m_change.IsPlaying() && hunter->OutOfArrows())
            {
                m_providers.m_notification.Notify(UserNotification::Notification::ObserveOutOfArrows);
                m_providers.m_change.GameOver(false);
            }
        }
    }

    void Dungeon::MoveDenizenRandomly(const DenizenIdentifier& identifier)
    {
        MoveDenizenRandomly(m_caveDenizens.at(identifier));
    }

    void Dungeon::MoveDenizenRandomly(const std::shared_ptr<Denizen>& denizen)
    {
        const auto denizenCave = denizen->GetCurrentCave().lock();
        const auto caveTunnels = denizenCave->GetConnectedIds();

        const auto newRandomId = m_providers.m_random.MakeRandomTunnel();

        Move(denizen->GetIdentifier(), caveTunnels[newRandomId]);
    }
}
