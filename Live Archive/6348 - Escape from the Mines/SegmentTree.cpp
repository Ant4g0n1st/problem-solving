#include <algorithm>
#include <iostream>
#include <chrono>
#include <limits>
#include <memory>
#include <random>
#include <vector>
#include <map>

namespace IO
{

    static void OptimizeStandardIO()
    {
        std::ios_base::sync_with_stdio(false);
        std::cout.tie(nullptr);
        std::cin.tie(nullptr);
    }

}

namespace DataStructures
{

    template <typename ValueType>
    class SegmentTree
    {
    public:
        SegmentTree(const int &begin, const int &end) : begin(begin), end(end) {}

        const ValueType &Build()
        {
            if (!IsLeaf())
            {
                const auto middle{RangeMidpoint()};
                left.reset(new SegmentTree(begin, middle));
                right.reset(new SegmentTree(middle + 1, end));
                value = left->Build() + right->Build();
            }
            return value;
        }

        ValueType Query(const int &x)
        {
            if (DisjointWith(x, x))
            {
                return ValueType{};
            }
            PropagatePendingTask();
            if (ContainedIn(x, x))
            {
                return value;
            }
            return left->Query(x) + right->Query(x);
        }

        const ValueType &Update(const int &l, const int &r, const ValueType &v)
        {
            PropagatePendingTask();
            if (DisjointWith(l, r))
            {
                return value;
            }
            if (ContainedIn(l, r))
            {
                SetPendingTask(v);
                return value;
            }
            PropagatePendingTask();
            return value = left->Update(l, r, v) + right->Update(l, r, v);
        }

    private:
        bool DisjointWith(const int &l, const int &r)
        {
            return end < l || r < begin;
        }

        bool ContainedIn(const int &l, const int &r)
        {
            return l <= begin && end <= r;
        }

        void PropagatePendingTask()
        {
            if (HasPendingTask() && HasChildren())
            {
                left->SetPendingTask(value);
                right->SetPendingTask(value);
                pending = false;
            }
        }

        void SetPendingTask(const ValueType &v)
        {
            pending = true;
            value = v;
        }

        int RangeMidpoint() const
        {
            return (begin + end) >> 1;
        }

        bool HasPendingTask() const
        {
            return pending;
        }

        bool IsLeaf() const
        {
            return begin == end;
        }

        bool HasChildren() const
        {
            return !IsLeaf();
        }

        std::unique_ptr<SegmentTree> left{}, right{};
        const int begin{}, end{};
        bool pending{false};
        ValueType value{};
    };

}

namespace Solution
{

    template <typename IntegralType>
    class MaxInteger
    {
        static_assert(std::is_integral<IntegralType>());
        static_assert(std::is_signed<IntegralType>());

    public:
        MaxInteger() : v(std::numeric_limits<IntegralType>::min()) {}

        MaxInteger(const IntegralType &v) : v(v) {}

        MaxInteger operator+(const MaxInteger &other) const
        {
            return {std::max(v, other.v)};
        }

        operator IntegralType() const
        {
            return v;
        }

    private:
        IntegralType v{};
    };

    static void SolveProblem(std::istream &input, std::ostream &output)
    {
        const unsigned int seed = std::chrono::system_clock::now().time_since_epoch().count();
        const int n = 1e5;
        const int q = 1e4;
        std::uniform_int_distribution<int> distribution{0, n - 1};
        std::default_random_engine generator{seed};
        auto random{std::bind(distribution, generator)};
        std::unique_ptr<DataStructures::SegmentTree<MaxInteger<int>>> tree{};
        tree.reset(new DataStructures::SegmentTree<MaxInteger<int>>{0, n - 1});
        std::vector<int> array(n, std::numeric_limits<int>::min());
        tree->Build();
        for (int i = 0; i < q; i++)
        {
            auto l{random()}, r{random()}, v{random()};
            if (l > r)
            {
                std::swap(l, r);
            }
            for (int j = l; j <= r; j++)
            {
                array[j] = v;
            }
            tree->Update(l, r, v);
            for (int j = 0; j < 100; j++)
            {
                const auto x{random()};
                const int y{tree->Query(x)};
                if (y != array[x])
                {
                    output << "DIFFERENT : T[" << y << "] A[" << array[x] << "]" << '\n';
                }
            }
        }
    }

}

int main()
{
    //IO::OptimizeStandardIO();
    Solution::SolveProblem(std::cin, std::cout);
    return 0;
}