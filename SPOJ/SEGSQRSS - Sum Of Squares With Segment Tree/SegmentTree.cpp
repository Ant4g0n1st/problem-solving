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

        const T &Update(const Int &, const Int &, const UpdateType &, const T &) noexcept;

        template <typename Factory, typename Iterator>
        const T &Build(Factory &, Iterator) noexcept;

        T Query(const Int &, const Int &) noexcept;

    private:
        void SetPending(const UpdateType &, const T &) noexcept;

        bool Contained(const Int &, const Int &) const noexcept;

        bool Disjoint(const Int &, const Int &) const noexcept;

        const T &ApplyPending() noexcept;

        T IntervalWidth() const noexcept;

        bool IsLeaf() const noexcept;

    private:
        UpdateType pendingType{};
        SegmentTree *right{};
        SegmentTree *left{};
        T sumOfSquares{};
        T totalSum{};
        T pending{};
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
                    st->Update(l, r, SegTree<Integral>::UpdateType::AddValue, v);
                }
                break;
                case OperationType::SetRangeToValue:
                {
                    Integral v{}; input >> v;
                    st->Update(l, r, SegTree<Integral>::UpdateType::SetValue, v);
                }
                break;
                case OperationType::QuerySum:
                {
                    output << st->Query(l, r) << '\n';
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
        if (pendingType != UpdateType::NoType)
        {
            this->pendingType = pendingType;
            this->pending = pending;
        }
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
    const T &SegmentTree<T>::ApplyPending() noexcept
    {
        switch (pendingType)
        {
        case UpdateType::AddValue:
        {
            const auto &&w{this->IntervalWidth()};
            /* (a + b)^2 = a^2 + 2ab + b^2 */
            sumOfSquares += totalSum * pending * 2;
            sumOfSquares += pending * pending * w;
            totalSum += pending * w;
        }
        break;
        case UpdateType::SetValue:
        {
            const auto &&w{this->IntervalWidth()};
            sumOfSquares = pending * pending * w;
            totalSum = pending * w;
        }
        break;
        case UpdateType::NoType:
            return sumOfSquares;
        }
        if (!this->IsLeaf())
        {
            right->SetPending(pendingType, pending);
            left->SetPending(pendingType, pending);
        }
        pendingType = UpdateType::NoType;
        return this->sumOfSquares;
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