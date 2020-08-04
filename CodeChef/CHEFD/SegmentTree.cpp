#include <type_traits>
#include <algorithm>
#include <iostream>
#include <memory>
#include <vector>
#include <array>

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
    public:
        SegmentTree(const Int &, const Int &) noexcept;

        void Update(const Int &, const Int &, const T &) noexcept;

        void Update(const Int &, const T &) noexcept;

        template <typename Iterator>
        void GetLeafValues(Iterator) noexcept;

        template <typename Iterator>
        void Build(Iterator) noexcept;

    private:
        bool Contained(const Int &, const Int &) const noexcept;

        bool Disjoint(const Int &, const Int &) const noexcept;

        void SetPending(const T &) noexcept;

        void ApplyPending() noexcept;

        bool IsLeaf() const noexcept;

        void Propagate() noexcept;

        std::unique_ptr<SegmentTree> right{};
        std::unique_ptr<SegmentTree> left{};
        const Int l{};
        const Int r{};
        T pending{};
        T value{};
    };

}; // namespace DataStructures

namespace Math
{

    template <typename T>
    class PartialFactorization
    {
        static_assert(std::is_integral<T>::value);

        enum Primes
        {
            Begin = 0, Two = Begin, Three, Five, Count
        };

        static std::array<const T, Primes::Count> primes;

    public:
        PartialFactorization(T value) noexcept;

        PartialFactorization() = default;

        void Nullify() noexcept;

        PartialFactorization &operator-=(const PartialFactorization &other) noexcept;

        PartialFactorization &operator+=(const PartialFactorization &other) noexcept;

        operator bool() const noexcept;

        operator T() const noexcept;

    private:
        bool IsNull() const noexcept;

        T GetValue() const noexcept;

        std::array<int, Primes::Count> powers{};
        T remainder{};
    };

}; // namespace Math

namespace Solution
{

    template <typename T>
    using Factorization = Math::PartialFactorization<T>;

    template <typename T>
    using SegTree = DataStructures::SegmentTree<T>;

    enum class QueryType : int
    {
        RangeDivision = 1,
        ChangeElement
    };

    void SolveProblem(std::istream &input, std::ostream &output)
    {
        using Value = int;
        std::unique_ptr<SegTree<Factorization<Value>>> st{};
        std::vector<Factorization<Value>> values{};
        Int n{}; input >> n;
        values.reserve(n);
        for (int i = 0; i < n; i++)
        {
            Value v{}; input >> v;
            values.emplace_back(v);
        }
        st.reset(new SegTree<Factorization<Value>>{0, --n});
        st->Build(values.begin());
        int m{}; input >> m;
        for (int i = 0; i < m; i++)
        {
            int type{}; input >> type;
            auto query = static_cast<QueryType>(type);
            if (query == QueryType::RangeDivision)
            {
                Int l{}; input >> l, --l;
                Int r{}; input >> r, --r;
                Value p{}; input >> p;
                st->Update(l, r, p);
                continue;
            }
            if (query == QueryType::ChangeElement)
            {
                Int idx{}; input >> idx;
                Value v{}; input >> v;
                st->Update(--idx, v);
            }
        }
        st->GetLeafValues(values.begin());
        auto it = values.begin();
        output << static_cast<Value>(*it);
        for (++it; it != values.end(); it++)
        {
            output << ' ' << static_cast<Value>(*it);
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

    /* Segment Tree Implementation. */
    template <typename T>
    SegmentTree<T>::SegmentTree(const Int &l, const Int &r) noexcept : l{l}, r{r} {}

    template <typename T>
    void SegmentTree<T>::Update(const Int &l, const Int &r, const T &update) noexcept
    {
        if (!Disjoint(l, r))
        {
            this->Propagate();
            if (Contained(l, r))
            {
                return SetPending(update);
            }
            right->Update(l, r, update);
            left->Update(l, r, update);
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
    void SegmentTree<T>::Update(const Int &index, const T &update) noexcept
    {
        if (!Disjoint(index, index))
        {
            this->Propagate();
            if (Contained(index, index))
            {
                value = update;
                return;
            }
            right->Update(index, update);
            left->Update(index, update);
        }
    }

    template <typename T>
    template <typename Iterator>
    void SegmentTree<T>::GetLeafValues(Iterator it) noexcept
    {
        this->Propagate();
        if (!this->IsLeaf())
        {
            right->GetLeafValues(it);
            left->GetLeafValues(it);
        }
        else
        {
            std::advance(it, l);
            *it = this->value;
        }
    }

    template <typename T>
    void SegmentTree<T>::SetPending(const T &pending) noexcept
    {
        this->pending += pending;
    }

    template <typename T>
    template <typename Iterator>
    void SegmentTree<T>::Build(Iterator it) noexcept
    {
        auto h{(l + r) / 2};
        if (!this->IsLeaf())
        {
            left.reset(new SegmentTree{l, h++});
            right.reset(new SegmentTree{h, r});
            left->Build(it), right->Build(it);
        }
        else
        {
            std::advance(it, h);
            this->value = *it;
        }
    }

    template <typename T>
    bool SegmentTree<T>::IsLeaf() const noexcept
    {
        return l == r;
    }

    template <typename T>
    void SegmentTree<T>::ApplyPending() noexcept
    {
        this->value -= pending;
        pending.Nullify();
    }

    template <typename T>
    void SegmentTree<T>::Propagate() noexcept
    {
        if (pending)
        {
            if (!this->IsLeaf())
            {
                right->SetPending(pending);
                left->SetPending(pending);
            }
            ApplyPending();
        }
    }

}; // namespace DataStructures

namespace Math
{

    /* Partial Factorization Implementation. */
    template <typename T>
    std::array<const T, PartialFactorization<T>::Primes::Count> PartialFactorization<T>::primes{2, 3, 5};

    template <typename T>
    PartialFactorization<T>::PartialFactorization(T value) noexcept
    {
        for (int i = Primes::Begin; i < Primes::Count; i++)
        {
            const auto &p{primes[i]};
            while (value % p == 0)
            {
                ++powers[i];
                value /= p;
            }
        }
        remainder = value;
    }

    template <typename T>
    PartialFactorization<T> &PartialFactorization<T>::operator+=(const PartialFactorization &other) noexcept
    {
        for (int i = Primes::Begin; i < Primes::Count; i++)
        {
            powers[i] += other.powers[i];
        }
        return *this;
    }

    template <typename T>
    PartialFactorization<T> &PartialFactorization<T>::operator-=(const PartialFactorization &other) noexcept
    {
        for (int i = Primes::Begin; i < Primes::Count; i++)
        {
            powers[i] -= other.powers[i];
        }
        return *this;
    }

    template <typename T>
    PartialFactorization<T>::operator bool() const noexcept
    {
        return !this->IsNull();
    }

    template <typename T>
    T PartialFactorization<T>::GetValue() const noexcept
    {
        T product{remainder};
        for (int i = Primes::Begin; i < Primes::Count; i++)
        {
            for (int j = 0; j < powers[i]; j++)
            {
                product *= primes[i];
            }
        }
        return product;
    }

    template <typename T>
    bool PartialFactorization<T>::IsNull() const noexcept
    {
        for (int i = Primes::Begin; i < Primes::Count; i++)
        {
            if (powers[i])
            {
                return false;
            }
        }
        return !remainder;
    }

    template <typename T>
    PartialFactorization<T>::operator T() const noexcept
    {
        return this->GetValue();
    }

    template <typename T>
    void PartialFactorization<T>::Nullify() noexcept
    {
        std::fill(powers.begin(), powers.end(), int());
        remainder = T();
    }

}; // namespace Math