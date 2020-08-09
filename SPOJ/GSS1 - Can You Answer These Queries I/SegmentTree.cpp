/*
    I apologize for such ugly code, but SPOJ doesn't really give
    you the chance to do otherwise due to the very strict limits.
    However, the judge has really good problems, so I had to try.
*/
#include <type_traits>
#include <functional>
#include <iostream>
#include <limits>
#include <array>

namespace Types
{

    using Long = long long int;
    using Int = unsigned int;

}; // namespace Types

using namespace Types;

namespace Constants
{

    const Int N = 5e4 + 1;

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
        T *operator()(Args... args) noexcept;

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
    public:
        SegmentTree(const Int &, const Int &) noexcept;

        SegmentTree() = default;

        template <typename Factory, typename Iterator>
        const T &Build(Factory &, Iterator) noexcept;

        T Query(const Int &, const Int &) const noexcept;

    private:
        bool Contained(const Int &, const Int &) const noexcept;

        bool Disjoint(const Int &, const Int &) const noexcept;

        bool IsLeaf() noexcept;

        SegmentTree *right{nullptr};
        SegmentTree *left{nullptr};
        T value{};
        Int l{};
        Int r{};

    public:
        static constexpr Int MaxTrees = 2 * Constants::N + 1;
    };

    template <typename T>
    class MaxSubArray
    {
        static_assert(std::is_integral<T>::value);

    public:
        MaxSubArray(const T &) noexcept;

        MaxSubArray() = default;

        MaxSubArray operator+(const MaxSubArray &) const noexcept;

        operator T() const noexcept;

    private:
        static constexpr T Min{std::numeric_limits<T>::min()};

        T maxPrefix{};
        T maxSuffix{};
        T max{Min};
        T sum{};
    };

}; // namespace DataStructures

namespace Solution
{
    template <typename T>
    using SegTree = DataStructures::SegmentTree<T>;

    using Integral = int;

    using Value = DataStructures::MaxSubArray<Integral>;

    /* Global variables so they are stored in stack memory, again, sorry. */
    Util::Factory<SegTree<Value>, SegTree<Value>::MaxTrees> factory{};
    std::array<Value, Constants::N> sequence;

    void SolveProblem(std::istream &input, std::ostream &output)
    {
        Int n{}; input >> n;
        for (int i = 0; i < n; i++)
        {
            Integral v{}; input >> v;
            sequence[i] = v;
        }
        auto st = factory(0u, n - 1);
        st->Build(factory, sequence.begin());
        Int m{}; input >> m;
        for (int i = 0; i < m; i++)
        {
            Int l{}; input >> l, --l;
            Int r{}; input >> r, --r;
            output << st->Query(l, r) << '\n';
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
    T *Factory<T, Size>::operator()(Args... args) noexcept
    {
        /* Constructing the element in-place without allocating memory. */
        return new (&elements[pointer++]) T{std::forward<Args>(args)...};
    }

    template <typename T, Int Size>
    void Factory<T, Size>::Reset() noexcept
    {
        pointer = Int{};
    }

}; // namespace Util

namespace DataStructures
{

    /* SegmentTree Implementation. */
    template <typename T>
    SegmentTree<T>::SegmentTree(const Int &l, const Int &r) noexcept : l{l}, r{r} {}

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
    T SegmentTree<T>::Query(const Int &l, const Int &r) const noexcept
    {
        if (Disjoint(l, r))
        {
            return T{};
        }
        if (Contained(l, r))
        {
            return value;
        }
        if (left->Disjoint(l, r))
        {
            return right->Query(l, r);
        }
        if (right->Disjoint(l, r))
        {
            return left->Query(l, r);
        }
        return left->Query(l, r) + right->Query(l, r);
    }

    template <typename T>
    template <typename Factory, typename Iterator>
    const T &SegmentTree<T>::Build(Factory &f, Iterator it) noexcept
    {
        auto h{(l + r) / 2};
        if (!this->IsLeaf())
        {
            /* 
                Attempt to simulate the use of
                dynamic memory allocation.
            */
            left = f(std::cref(l), std::cref(h));
            right = f(std::cref(++h), std::cref(r));
            value = left->Build(f, it) + right->Build(f, it);
        }
        else
        {
            std::advance(it, h);
            this->value = *it;
        }
        return value;
    }

    template <typename T>
    bool SegmentTree<T>::IsLeaf() noexcept
    {
        return l == r;
    }

    /* MaxSubArray Implementation. */
    template <typename T>
    MaxSubArray<T>::MaxSubArray(const T &v) noexcept : max{v}, maxPrefix{v}, maxSuffix{v}, sum{v} {}

    template <typename T>
    MaxSubArray<T> MaxSubArray<T>::operator+(const MaxSubArray &other) const noexcept
    {
        /*
            This is valid assuming 'this' and 'other' are the left and right halves
            of a contiguous subarray, respectively.
        */
        MaxSubArray result{};
        T &m{result.max};
        result.maxSuffix = std::max(other.maxSuffix, other.sum + maxSuffix);
        result.maxPrefix = std::max(maxPrefix, sum + other.maxPrefix);
        m = std::max(m, maxSuffix + other.maxPrefix);
        m = std::max(m, result.maxPrefix);
        m = std::max(m, result.maxSuffix);
        result.sum = sum + other.sum;
        m = std::max(m, other.max);
        m = std::max(m, max);
        return result;
    }

    template <typename T>
    MaxSubArray<T>::operator T() const noexcept
    {
        return max;
    }

}; // namespace DataStructures