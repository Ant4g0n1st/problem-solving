#include <type_traits>
#include <functional>
#include <iostream>
#include <array>

namespace Types
{

    using Int = unsigned int;

};

using namespace Types;

namespace Constants
{

    constexpr Int MaxPrime = 1e6;

};

namespace Math
{

    template <Int Size>
    class Sieve
    {
    public:
        void Compute(const Int & = Size) noexcept;

        bool IsPrime(const Int &) const noexcept;

        static Sieve *Get() noexcept;

    private:
        Sieve() = default;

        static std::array<bool, Size + 1> table;
    };

}; // namespace Math

namespace Solutions
{

    void SolveProblem(std::istream &input, std::ostream &output)
    {
        auto sieve = Math::Sieve<Constants::MaxPrime>::Get();
        sieve->Compute();
    }

    void OptimizeIOStreams()
    {
    }

}; // namespace Solutions

int main()
{
    Solutions::OptimizeIOStreams();
    Solutions::SolveProblem(std::cin, std::cout);
    return 0;
}

namespace Math
{

    /* Sieve Implementation. */
    template <Int Size>
    std::array<bool, Size + 1> Sieve<Size>::table;

    template <Int Size>
    bool Sieve<Size>::IsPrime(const Int &k) const noexcept
    {
        return !table[k];
    }

    template <Int Size>
    void Sieve<Size>::Compute(const Int &n) noexcept
    {
        for (Int i = 4; i <= n; i += 2)
        {
            table[i] = true;
        }
        for (Int i = 3; i * i <= n; i += 2)
        {
            if (!table[i])
            {
                for (Int j = i * i; j <= n; j += 2 * i)
                {
                    table[j] = true;
                }
            }
        }
    }

    template <Int Size>
    Sieve<Size> *Sieve<Size>::Get() noexcept
    {
        static Sieve<Size> instance{};
        return &instance;
    }

}; // namespace Math