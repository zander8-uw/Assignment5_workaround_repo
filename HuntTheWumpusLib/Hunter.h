#pragma once

#include "Denizen.h"

#include <memory>
#include <vector>

namespace HuntTheWumpus
{
    class Arrow;
    constexpr auto QuiverCapacity = 5;

    class Hunter final : public Denizen
    {
    public:
        explicit Hunter(Context& providers);
        ~Hunter() override = default;

        [[nodiscard]] int GetPriority() const override { return 1; }

        void EnterCave(const std::shared_ptr<Cave>& cave) override;
        bool ObserveCaveEntrance(const std::shared_ptr<Denizen>& trigger) override;

        std::shared_ptr<Arrow> GetArrow();

        [[nodiscard]] bool OutOfArrows() const { return m_arrows.empty(); }

        Hunter(const Hunter&) = delete;
        Hunter(Hunter&&) = delete;
        Hunter& operator=(const Hunter&) = delete;
        Hunter& operator=(Hunter&&) = delete;

    private:
        std::vector<std::shared_ptr<Arrow>> m_arrows;
    };
}
