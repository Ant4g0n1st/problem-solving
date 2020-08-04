#include <type_traits>
#include <iostream>
#include <memory>
#include <vector>

namespace Types
{

    using Long = long long int;
    using Int = unsigned int;

}; // namespace Types

using namespace Types;

namespace DataStructures
{

    template <typename T>
    class PartialSums
    {
        static_assert(std::is_integral<T>::value);
        static_assert(std::is_signed<T>::value);

    public:
        PartialSums(const Int &) noexcept;

        void AddToRange(const Int &, const Int &, const T &) noexcept;

        const T &Query(const Int &) const noexcept;

        void ComputeSums() noexcept;

    private:
        std::vector<T> sums{};
    };

    template <typename T>
    struct Operation
    {
        const Int l{};
        const Int r{};
        const T d{};

        Operation(const Int &, const Int &, const T &) noexcept;
    };

}; // namespace DataStructures

namespace Solution
{

    void SolveProblem(std::istream &input, std::ostream &output)
    {
        namespace ds = DataStructures;
        using Value = Long;
        for (Int k{}, m{}, n{}; input >> n >> m >> k;)
        {
            std::unique_ptr<ds::PartialSums<Value>> array{};
            array.reset(new ds::PartialSums<Value>{n});
            for (int i = 0; i < n; i++)
            {
                Value a{}; input >> a;
                array->AddToRange(i, i, a);
            }
            std::vector<ds::Operation<Value>> operations{};
            operations.reserve(m);
            for (int i = 0; i < m; i++)
            {
                Int l{}; input >> l, --l;
                Int r{}; input >> r, --r;
                Value d{}; input >> d;
                operations.emplace_back(l, r, d);
            }
            std::unique_ptr<ds::PartialSums<int>> count{};
            count.reset(new ds::PartialSums<int>{m});
            for (int i = 0; i < k; i++)
            {
                Int x{}; input >> x, --x;
                Int y{}; input >> y, --y;
                count->AddToRange(x, y, 1);
            }
            count->ComputeSums();
            for (int i = 0; i < m; i++)
            {
                const auto &c{count->Query(i)};
                const auto &op{operations[i]};
                array->AddToRange(op.l, op.r, op.d * c);
            }
            array->ComputeSums();
            output << array->Query(0);
            for (int i = 1; i < n; i++)
            {
                output << ' ' << array->Query(i);
            }
            output << '\n';
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

namespace DataStructures
{

    /* PartialSums Implementation. */
    template <typename T>
    PartialSums<T>::PartialSums(const Int &n) noexcept : sums(n + 1) {}

    template <typename T>
    void PartialSums<T>::AddToRange(const Int &l, const Int &r, const T &v) noexcept
    {
        sums[r + 1] -= v;
        sums[l] += v;
    }

    template <typename T>
    const T &PartialSums<T>::Query(const Int &i) const noexcept
    {
        return sums[i];
    }

    template <typename T>
    void PartialSums<T>::ComputeSums() noexcept
    {
        T running{};
        for (auto &s : sums)
        {
            running += s;
            s = running;
        }
    }

    /* Operation Implementation. */
    template <typename T>
    Operation<T>::Operation(const Int &l, const Int &r, const T &d) noexcept : d{d}, l{l}, r{r} {}

}; // namespace DataStructures