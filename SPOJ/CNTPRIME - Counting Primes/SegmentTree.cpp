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
    constexpr Int N = 1e4 + 1;

}; // namespace Constants

namespace Math
{

    template <Int Size>
    class Sieve
    {
    public:
        void DetectPrimes(const Int & = Size) noexcept;

        bool IsPrime(const Int &) const noexcept;

        static Sieve *Get() noexcept;

    private:
        Sieve() = default;

    private:
        static std::array<bool, Size + 1> table;
    };

}; // namespace Math

namespace Util
{

    template <typename T, Int Size>
    class Factory
    {
    public:
        Factory() = default;

        template <typename... Args>
        T *operator()(Args &&... args) noexcept;

        void Reset() noexcept;

    private:
        std::array<T, Size> elements;
        Int pointer{};
    };

}; // namespace Util

namespace DataStructures
{

    template <typename T>
    class SegmentTree
    {
        static_assert(std::is_integral<T>::value, "The type must be integral.");

    public:
        SegmentTree(const Int &, const Int &) noexcept;

        SegmentTree() = default;

        const T &Update(const Int &, const Int &, const T &) noexcept;

        template <typename Factory, typename Iterator>
        const T &Build(Factory &, Iterator) noexcept;

        T Query(const Int &, const Int &) noexcept;

    private:
        bool Contained(const Int &, const Int &) const noexcept;

        bool Disjoint(const Int &, const Int &) const noexcept;

        void SetPending(const T &) noexcept;

        const T &ApplyPending() noexcept;

        void PropagatePending() noexcept;

        T IntervalWidth() const noexcept;

        bool IsLeaf() const noexcept;

    private:
        SegmentTree *right{};
        SegmentTree *left{};
        bool hasPending{};
        T pending{};
        T value{};
        Int l{};
        Int r{};

    public:
        static constexpr Int MaxTrees{2 * Constants::N + 1};
    };

}; // namespace DataStructures

namespace Solutions
{

    template <typename T>
    using SegTree = DataStructures::SegmentTree<T>;

    using Sieve = Math::Sieve<Constants::MaxPrime>;

    using Integral = Int;

    enum class OperationType : Int
    {
        SetRangeToValue = 0,
        QueryRangeSum
    };

    Util::Factory<SegTree<Integral>, SegTree<Integral>::MaxTrees> factory{};
    std::array<Integral, Constants::N> sequence;

    void SolveProblem(std::istream &input, std::ostream &output)
    {
        Sieve *sieve = Sieve::Get();
        Int tests{}; input >> tests;
        sieve->DetectPrimes();
        for (Int tc = 1; tc <= tests; tc++)
        {
            Int m{}, n{}; input >> n >> m;
            for (Int i = 0; i < n; i++)
            {
                auto &v{sequence[i]}; input >> v;
                v = sieve->IsPrime(v);
            }
            SegTree<Integral> *st{factory(0, n - 1)};
            st->Build(factory, sequence.begin());
            output << "Case " << tc << ":\n";
            for (Int i = 0; i < m; i++)
            {
                Int type{}; input >> type;
                Int l{}; input >> l, --l;
                Int r{}; input >> r, --r;
                switch (static_cast<OperationType>(type))
                {
                case OperationType::SetRangeToValue:
                {
                    Integral v{}; input >> v;
                    v = sieve->IsPrime(v);
                    st->Update(l, r, v);
                }
                break;
                case OperationType::QueryRangeSum:
                {
                    output << st->Query(l, r);
                    output << '\n';
                }
                break;
                }
            }
            factory.Reset();
        }
    }

    void OptimizeIOStreams()
    {
        std::ios_base::sync_with_stdio(false);
        std::cout.tie(nullptr);
        std::cin.tie(nullptr);
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
    void Sieve<Size>::DetectPrimes(const Int &n) noexcept
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

namespace Util
{

    /* Factory Implementation. */
    template <typename T, Int Size>
    template <typename... Args>
    T *Factory<T, Size>::operator()(Args &&... args) noexcept
    {
        return new (&elements[pointer++]) T{std::forward<Args>(args)...};
    }

    template <typename T, Int Size>
    void Factory<T, Size>::Reset() noexcept
    {
        this->pointer = Int{};
    }

}; // namespace Util

namespace DataStructures
{

    /* SegmentTree Implementation. */
    template <typename T>
    SegmentTree<T>::SegmentTree(const Int &l, const Int &r) noexcept : l{l}, r{r} {}

    template <typename T>
    const T &SegmentTree<T>::ApplyPending() noexcept
    {
        if (this->hasPending)
        {
            value = IntervalWidth() * pending;
            PropagatePending();
            hasPending = false;
        }
        return value;
    }

    template <typename T>
    T SegmentTree<T>::IntervalWidth() const noexcept
    {
        return static_cast<T>(r - l + 1);
    }

    template <typename T>
    void SegmentTree<T>::PropagatePending() noexcept
    {
        if (!this->IsLeaf())
        {
            right->SetPending(pending);
            left->SetPending(pending);
        }
    }

    template <typename T>
    template <typename Factory, typename Iterator>
    const T &SegmentTree<T>::Build(
        Factory &f,
        Iterator it
    ) noexcept
    {
        auto h{(l + r) / 2};
        if (!this->IsLeaf())
        {
            right = f(std::cref(++h), std::cref(r));
            left = f(std::cref(l), std::cref(--h));
            value = left->Build(f, it)
                + right->Build(f, it);
        }
        else
        {
            std::advance(it, l);
            this->value = *it;
        }
        return value;
    }

    template <typename T>
    bool SegmentTree<T>::IsLeaf() const noexcept
    {
        return l == r;
    }

    template <typename T>
    const T &SegmentTree<T>::Update(
        const Int &l,
        const Int &r,
        const T &v
    ) noexcept
    {
        ApplyPending();
        if (Disjoint(l, r))
        {
            return value;
        }
        if (Contained(l, r))
        {
            SetPending(v);
            return ApplyPending();
        }
        return value = left->Update(l, r, v)
                    + right->Update(l, r, v);
    }

    template <typename T>
    bool SegmentTree<T>::Contained(
        const Int &l,
        const Int &r
    ) const noexcept
    {
        return l <= this->l && this->r <= r;
    }

    template <typename T>
    bool SegmentTree<T>::Disjoint(
        const Int &l,
        const Int &r
    ) const noexcept
    {
        return this->r < l || r < this->l;
    }

    template <typename T>
    void SegmentTree<T>::SetPending(
        const T &pending
    ) noexcept
    {
        this->pending = pending;
        hasPending = true;
    }

    template <typename T>
    T SegmentTree<T>::Query(
        const Int &l,
        const Int &r
    ) noexcept
    {
        if (Disjoint(l, r))
        {
            return T{};
        }
        ApplyPending();
        if (Contained(l, r))
        {
            return value;
        }
        return left->Query(l, r)
            + right->Query(l, r);
    }

}; // namespace DataStructures