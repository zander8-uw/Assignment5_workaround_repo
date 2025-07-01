#pragma once

#include <compare>
#include <memory>

namespace HuntTheWumpus
{
    class Cave;
    class IRandomProvider;
    struct Context;

    enum class Category : uint8_t
    {
        Wumpus,
        Pit,
        Bat,
        Hunter,
        Arrow
    };

    struct DenizenIdentifier
    {
        Category m_category;
        int m_instance;

        bool operator==(const DenizenIdentifier &other) const = default;
        std::strong_ordering operator <=>(const DenizenIdentifier &other) const;
    };

    struct DenizenIdentifierHasher
    {
        size_t operator()(const DenizenIdentifier &id) const;
    };

    struct DenizenProperties
    {
        bool m_carryableByBats = false;
        bool m_fatalToWumpus = false;
        bool m_fatalToHunter = false;
        bool m_isEdible = false;
        bool m_reportMovement = false;
    };

    class Denizen
    {
    public:
        Denizen(const DenizenIdentifier& identifier, const DenizenProperties &properties, Context& providers);
        virtual ~Denizen() = default;

        // Returns true if the observation triggered a behavior.
        virtual bool ObserveCaveEntrance(const std::shared_ptr<Denizen>& trigger);
        virtual void ReportPresence() const;

        // Properties of Thing
        [[nodiscard]] const DenizenProperties &Properties() const { return m_properties; }

        virtual void EnterCave(const std::shared_ptr<Cave>& cave);
        void RemoveFromCave();

        [[nodiscard]] const std::weak_ptr<Cave> &GetCurrentCave() const { return m_cave; }

        [[nodiscard]] virtual int GetPriority() const;

        [[nodiscard]] const DenizenIdentifier &GetIdentifier() const { return m_identifier; }

        Denizen(const Denizen&) = delete;
        Denizen(Denizen&&) = delete;
        Denizen& operator=(const Denizen&) = delete;
        Denizen& operator=(Denizen&&) = delete;

    protected:

        DenizenIdentifier m_identifier;
        DenizenProperties m_properties;

        Context& m_providers;
        std::weak_ptr<Cave> m_cave;
    };

    std::ostream& operator<<(std::ostream& out, const Category& value);
    std::ostream& operator<<(std::ostream& out, const DenizenIdentifier &value);
}
