#pragma once

namespace HuntTheWumpus
{
    class IRandomProvider
    {
    public:

        IRandomProvider() = default;
        virtual ~IRandomProvider() = default;

        // Produce random numbers in the [1, 20] range uniformly.
        virtual int MakeRandomCave() = 0;

        // Produce random numbers in the [0, 2] range uniformly.
        virtual int MakeRandomTunnel() = 0;

        // Produce random number in the range [0, 1] range uniformly.
        virtual float MakeRandomNumber() = 0;

        IRandomProvider(const IRandomProvider &) = default;
        IRandomProvider(IRandomProvider &&) = default;
        IRandomProvider &operator=(const IRandomProvider &) = default;
        IRandomProvider &operator=(IRandomProvider &&) = default;
    };
}