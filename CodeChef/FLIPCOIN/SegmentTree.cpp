#include <type_traits>
#include <iostream>
#include <memory>

namespace Types
{

    using Int = unsigned int;

};

using namespace Types;

namespace DataStructures
{

    template <typename T>
    class SegmentTree
    {
        static_assert(std::is_integral<T>::value);

    public:
        SegmentTree(const Int &, const Int &) noexcept;

        T Update(const Int &, const Int &) noexcept;

        T Query(const Int &, const Int &) noexcept;

        void Invert() noexcept;

        void Build() noexcept;

    private:
        bool Contained(const Int &, const Int &) noexcept;

        bool Disjoint(const Int &, const Int &) noexcept;

        T IntervalWidth() noexcept;

        void Propagate() noexcept;

        bool IsLeaf() noexcept;

        std::unique_ptr<SegmentTree> right{};
        std::unique_ptr<SegmentTree> left{};
        const Int l{};
        const Int r{};
        bool invert{};
        T sum{};
    };

}; // namespace DataStructures

namespace Solution
{

    enum class QueryType : int
    {
        FlipCoins = 0,
        CountHeads
    };

    void SolveProblem()
    {
        namespace ds = DataStructures;
        using Coin = unsigned int;
        std::unique_ptr<ds::SegmentTree<Coin>> st{};
        Int n{}; std::cin >> n;
        int q{}; std::cin >> q;
        st.reset(new ds::SegmentTree<Coin>{0, --n});
        st->Build();
        for (int i = 0; i < q; i++)
        {
            int t{}; std::cin >> t;
            int l{}; std::cin >> l;
            int r{}; std::cin >> r;
            switch (static_cast<QueryType>(t))
            {
            case QueryType::FlipCoins:
                st->Update(l, r);
                break;

            case QueryType::CountHeads:
                std::cout << st->Query(l, r);
                std::cout << '\n';
                break;

            default:
                break;
            }
        }
    }

    void OptimizeIOStreams()
    {
        std::ios_base::sync_with_stdio(false);
        std::cout.tie(nullptr);
        std::cin.tie(nullptr);
    }

} // namespace Solution

#include <fstream>

int main()
{
    Solution::OptimizeIOStreams();
    Solution::SolveProblem();
    return 0;
}

namespace DataStructures
{

    template <typename T>
    SegmentTree<T>::SegmentTree(const Int &l, const Int &r) noexcept : l{l}, r{r} {}

    template <typename T>
    bool SegmentTree<T>::Contained(const Int &l, const Int &r) noexcept
    {
        return l <= this->l && this->r <= r;
    }

    template <typename T>
    bool SegmentTree<T>::Disjoint(const Int &l, const Int &r) noexcept
    {
        return this->r < l or r < this->l;
    }

    template <typename T>
    T SegmentTree<T>::Update(const Int &l, const Int &r) noexcept
    {
        this->Propagate();
        if (Disjoint(l, r))
        {
            return sum;
        }
        if (Contained(l, r))
        {
            this->Invert();
            Propagate();
            return sum;
        }
        return sum = left->Update(l, r) + right->Update(l, r);
    }

    template <typename T>
    T SegmentTree<T>::Query(const Int &l, const Int &r) noexcept
    {
        if (Disjoint(l, r))
        {
            return T{};
        }
        Propagate();
        if (Contained(l, r))
        {
            return sum;
        }
        return left->Query(l, r) + right->Query(l, r);
    }

    template <typename T>
    void SegmentTree<T>::Build() noexcept
    {
        if (!this->IsLeaf())
        {
            auto h{(l + r) / 2};
            left.reset(new SegmentTree{l, h++});
            right.reset(new SegmentTree{h, r});
            left->Build(), right->Build();
        }
    }

    template <typename T>
    void SegmentTree<T>::Propagate() noexcept
    {
        if (invert)
        {
            sum = IntervalWidth() - sum;
            if (!this->IsLeaf())
            {
                right->Invert();
                left->Invert();
            }
            this->Invert();
        }
    }

    template <typename T>
    T SegmentTree<T>::IntervalWidth() noexcept
    {
        return static_cast<T>(r - l + 1);
    }

    template <typename T>
    void SegmentTree<T>::Invert() noexcept
    {
        this->invert = !invert;
    }

    template <typename T>
    bool SegmentTree<T>::IsLeaf() noexcept
    {
        return l == r;
    }

}; // namespace DataStructures