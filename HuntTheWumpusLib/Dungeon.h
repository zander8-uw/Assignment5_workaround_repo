#pragma once

#include "Denizen.h"

#include "Cave.h"

#include <memory>
#include <unordered_map>
#include <vector>

namespace HuntTheWumpus
{
    enum class DungeonMove : uint8_t
    {
        Move,
        Shoot
    };

    class IDungeon
    {
    public:
        
        IDungeon() = default;
        virtual ~IDungeon() = default;

        virtual const std::shared_ptr<Cave> &FindCave(int caveId) = 0;
        virtual void Move(const DenizenIdentifier &identifier, int destinationCave) = 0;
        virtual void MoveDenizenRandomly( const DenizenIdentifier &identifier ) = 0;

        IDungeon(const IDungeon &) = default;
        IDungeon(IDungeon &&) = default;
        IDungeon &operator=(const IDungeon &) = default;
        IDungeon &operator=(IDungeon &&) = default;
    };

    class Dungeon final : public IDungeon
    {
    public:

        explicit Dungeon(Context &providers);
        ~Dungeon() override = default;

        const std::shared_ptr<Cave> &FindCave(int caveId) override;
        void Move(const DenizenIdentifier &identifier, int destinationCave) override;
        void MoveDenizenRandomly( const DenizenIdentifier &identifier ) override;

        void MakeMove(DungeonMove operation, const std::vector<int> &destinationIds);
        static bool LegalMove(const std::shared_ptr<Denizen> &denizen, int destinationCave);

        Dungeon(const Dungeon &) = delete;
        Dungeon(Dungeon &&) = delete;
        Dungeon &operator=(const Dungeon &) = delete;
        Dungeon &operator=(Dungeon &&) = delete;

    private:

        void MoveDenizenRandomly( const std::shared_ptr<Denizen> &denizen );

        void Initialize(Context &providers);
        void AddDenizen(const std::shared_ptr<Denizen> &newDenizen);
        void MakeTunnels() const;
        void AddToCave(const std::shared_ptr<Denizen> &denizen, int caveId, bool observeEntrance);

        Context &m_providers;
        std::unordered_map<DenizenIdentifier, std::shared_ptr<Denizen>, DenizenIdentifierHasher> m_caveDenizens;
        std::unordered_map<int, std::shared_ptr<Cave>> m_caves;
    };
}
