#include <type_traits>
#include <functional>
#include <iostream>
#include <array>

namespace Types
{

    using Long = long long int;
    using Int = unsigned int;

}; // namespace Types

using namespace Types;

namespace Constants
{

    constexpr Int N = 1e5 + 1;

};

namespace Util
{

    /*
        This is my attempt to simulate
        dynamic memory allocation.
    */
    template <typename T, Int Size>
    class Factory
    {
    public:
        Factory() = default;

        template <typename... Args>
        T *operator()(Args &&...) noexcept;

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
        static constexpr Int MaxTrees{2 * Constants::N + 1};

        enum class UpdateType : Int
        {
            NoType = 0,
            AddValue,
            SetValue
        };

    public:
        SegmentTree(const Int &, const Int &) noexcept;

        SegmentTree() = default;

        void AddToRange(const Int &, const Int &, const T &) noexcept;

        void SetRangeTo(const Int &, const Int &, const T &) noexcept;

        template <typename Factory, typename Iterator>
        const T &Build(Factory &, Iterator) noexcept;

        T Query(const Int &, const Int &) noexcept;

    private:
        const T &Update(const Int &, const Int &, const UpdateType &, const T &) noexcept;

        void SetPending(const UpdateType &, const T &) noexcept;

        bool Contained(const Int &, const Int &) const noexcept;

        bool Disjoint(const Int &, const Int &) const noexcept;

        const T &ApplyPending() noexcept;

        T IntervalWidth() const noexcept;

        bool IsLeaf() const noexcept;

    private:
        bool hasPendingSet{};
        SegmentTree *right{};
        SegmentTree *left{};
        T sumOfSquares{};
        T pendingAdd{};
        T pendingSet{};
        T totalSum{};
        Int l{};
        Int r{};
    };

}; // namespace DataStructures

namespace Solution
{

    template <typename T>
    using SegTree = DataStructures::SegmentTree<T>;

    using Integral = Long;

    enum class OperationType : Int
    {
        SetRangeToValue = 0,
        AddValueToRange,
        QuerySum
    };

    Util::Factory<SegTree<Integral>, SegTree<Integral>::MaxTrees> factory{};
    std::array<Integral, Constants::N> sequence;

    void SolveProblem(std::istream &input, std::ostream &output)
    {
        Int testCases{}; input >> testCases;
        for (Int t = 1; t <= testCases; t++)
        {
            output << "Case " << t << ":\n";
            Int m{}, n{}; input >> n >> m;
            for (Int i = 0; i < n; i++)
            {
                input >> sequence[i];
            }
            SegTree<Integral> *st{factory(0, n - 1)};
            st->Build(factory, sequence.begin());
            for (Int i = 0; i < m; i++)
            {
                Int type{}; input >> type;
                Int l{}; input >> l, --l;
                Int r{}; input >> r, --r;
                switch (static_cast<OperationType>(type))
                {
                case OperationType::AddValueToRange:
                {
                    Integral v{}; input >> v;
                    st->AddToRange(l, r, v);
                }
                break;
                case OperationType::SetRangeToValue:
                {
                    Integral v{}; input >> v;
                    st->SetRangeTo(l, r, v);
                }
                break;
                case OperationType::QuerySum:
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

}; // namespace Solution

int main()
{
    Solution::OptimizeIOStreams();
    Solution::SolveProblem(std::cin, std::cout);
    return 0;
}

namespace Util
{

    /* Factory Implementation. */
    template <typename T, Int Size>
    template <typename... Args>
    T *Factory<T, Size>::operator()(Args &&... args) noexcept
    {
        /* Constructing the element in-place without allocating memory. */
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
    const T &SegmentTree<T>::Update(const Int &l, const Int &r, const UpdateType &type, const T &u) noexcept
    {
        this->ApplyPending();
        if (Disjoint(l, r))
        {
            return sumOfSquares;
        }
        if (Contained(l, r))
        {
            this->SetPending(type, u);
            return ApplyPending();
        }
        sumOfSquares = left->Update(l, r, type, u) + right->Update(l, r, type, u);
        totalSum = left->totalSum + right->totalSum;
        return sumOfSquares;
    }

    template <typename T>
    void SegmentTree<T>::SetPending(const UpdateType &pendingType, const T &pending) noexcept
    {
        switch (pendingType)
        {
        case UpdateType::AddValue:
        {
            pendingAdd += pending;
        }
        break;
        case UpdateType::SetValue:
        {
            hasPendingSet = true;
            pendingSet = pending;
            pendingAdd = T{};
        }
        break;
        default:
            break;
        }
    }

    template <typename T>
    void SegmentTree<T>::AddToRange(const Int &l, const Int &r, const T &v) noexcept
    {
        Update(l, r, UpdateType::AddValue, v);
    }

    template <typename T>
    void SegmentTree<T>::SetRangeTo(const Int &l, const Int &r, const T &v) noexcept
    {
        Update(l, r, UpdateType::SetValue, v);
    }

    template <typename T>
    bool SegmentTree<T>::Contained(const Int &l, const Int &r) const noexcept
    {
        return l <= this->l && this->r <= r;
    }

    template <typename T>
    bool SegmentTree<T>::Disjoint(const Int &l, const Int &r) const noexcept
    {
        return this->r < l || r < this->l;
    }

    template <typename T>
    template <typename Factory, typename Iterator>
    const T &SegmentTree<T>::Build(Factory &f, Iterator it) noexcept
    {
        auto &&h{(l + r) / 2};
        if (!this->IsLeaf())
        {
            this->left = f(std::cref(l), std::cref(h));
            right = f(std::cref(++h), std::cref(r));
            sumOfSquares = left->Build(f, it) + right->Build(f, it);
            totalSum = left->totalSum + right->totalSum;
        }
        else
        {
            std::advance(it, l);
            const auto &v{*it};
            sumOfSquares = v * v;
            totalSum = v;
        }
        return sumOfSquares;
    }

    template <typename T>
    const T &SegmentTree<T>::ApplyPending() noexcept
    {
        if (!this->IsLeaf())
        {
            if (hasPendingSet)
            {
                right->SetPending(UpdateType::SetValue, pendingSet);
                left->SetPending(UpdateType::SetValue, pendingSet);
            }
            right->SetPending(UpdateType::AddValue, pendingAdd);
            left->SetPending(UpdateType::AddValue, pendingAdd);
        }
        if (hasPendingSet)
        {
            /* Must be performed before addition. */
            const auto &&w{IntervalWidth()};
            const auto &v{pendingSet};
            sumOfSquares = v * v * w;
            totalSum = v * w;
        }
        if (pendingAdd)
        {
            const auto &&w{IntervalWidth()};
            const auto &v{pendingAdd};
            /* (a + b)^2 = a^2 + 2ab + b^2 */
            sumOfSquares += totalSum * v * 2;
            sumOfSquares += v * v * w;
            totalSum += v * w;
        }
        pendingAdd = pendingSet = T{};
        this->hasPendingSet = false;
        return sumOfSquares;
    }

    template <typename T>
    T SegmentTree<T>::Query(const Int &l, const Int &r) noexcept
    {
        if (Disjoint(l, r))
        {
            return T{};
        }
        ApplyPending();
        if (Contained(l, r))
        {
            return sumOfSquares;
        }
        return left->Query(l, r) + right->Query(l, r);
    }

    template <typename T>
    T SegmentTree<T>::IntervalWidth() const noexcept
    {
        return static_cast<T>(r - l + 1);
    }

    template <typename T>
    bool SegmentTree<T>::IsLeaf() const noexcept
    {
        return l == r;
    }

}; // namespace DataStructures