#pragma once

namespace HuntTheWumpus
{
    class IGameStateChange
    {
    public:
        virtual ~IGameStateChange() = default;

        virtual void GameOver(bool won) = 0;
        [[nodiscard]] virtual bool IsPlaying() const = 0;

        IGameStateChange() = default;
        IGameStateChange(const IGameStateChange &) = default;
        IGameStateChange(IGameStateChange &&) = default;
        IGameStateChange &operator=(const IGameStateChange &) = default;
        IGameStateChange &operator=(IGameStateChange &&) = default;
    };
}