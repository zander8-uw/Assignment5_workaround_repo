#pragma once

#include "Denizen.h"

namespace HuntTheWumpus
{
    class Pit final : public Denizen
    {
    public:
        Pit(int pitInstance, Context& providers);
        ~Pit() override = default;

        [[nodiscard]] int GetPriority() const override { return 4; }

        bool ObserveCaveEntrance(const std::shared_ptr<Denizen>& trigger) override;
        void ReportPresence() const override;

        Pit(const Pit&) = delete;
        Pit(Pit&&) = delete;
        Pit& operator=(const Pit&) = delete;
        Pit& operator=(Pit&&) = delete;
    };
}
