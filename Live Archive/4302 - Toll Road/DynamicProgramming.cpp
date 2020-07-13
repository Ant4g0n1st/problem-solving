#include <forward_list>
#include <type_traits>
#include <iostream>
#include <limits>
#include <memory>
#include <random>
#include <vector>
#include <map>

namespace Types
{

    using Int = unsigned int;

};

using namespace Types;

namespace DataStructures
{

    template <typename K, typename V>
    class Map : public std::map<K, V>
    {
        static_assert(std::is_integral<V>::value);
        static_assert(std::is_unsigned<V>::value);

    public:
        V &operator[](const K &) noexcept;
    };

}; // namespace DataStructures

namespace Graphs
{

    template <typename NodeType, typename WeightType>
    struct Edge
    {
        static_assert(std::is_integral<WeightType>::value);
        static_assert(std::is_integral<NodeType>::value);

        WeightType w{};
        NodeType v{};

        Edge(const NodeType &, const WeightType &);
    };

    template <typename Weight>
    class Tree
    {
        static_assert(std::is_integral<Weight>::value);

        using Node = unsigned int;
        using List = std::forward_list<Edge<Node, Weight>>;

        const static Weight oo{std::numeric_limits<Weight>::max()};

        std::vector<List> edges{};
        const unsigned int n{};

    public:
        Tree(const unsigned int &);

        void AddEdge(const Node &, const Node &, const Weight &) noexcept;

        Weight HeaviestSubtree() noexcept;

    private:
        template <typename DPTable>
        Weight HeaviestSubtree(const Node &, const Node &, DPTable &) noexcept;

        Node GetRandomNode() noexcept;
    };

}; // namespace Graphs

namespace Solution
{

    void SolveProblem()
    {
        std::unique_ptr<Graphs::Tree<int>> t{};
        for (Int n{}; std::cin >> n && n;)
        {
            t.reset(new Graphs::Tree<int>(n + 1));
            DataStructures::Map<Int, Int> index{};
            for (int i = 0; i < n; i++)
            {
                Int u{}; std::cin >> u;
                Int v{}; std::cin >> v;
                int w{}; std::cin >> w;
                t->AddEdge(index[u], index[v], w);
            }
            std::cout << t->HeaviestSubtree();
            std::cout << '\n';
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
    Solution::SolveProblem();
    return 0;
}

namespace DataStructures
{

    /* Map Implementation. */
    template <typename K, typename V>
    V &Map<K, V>::operator[](const K &key) noexcept
    {
        return this->emplace(key, this->size()).first->second;
    }

}; // namespace DataStructures

namespace Graphs
{

    /* Edge Implementation. */
    template <typename NodeType, typename WeightType>
    Edge<NodeType, WeightType>::Edge(const NodeType &u, const WeightType &weight) : v(u), w(weight) {}

    /* Tree Implementation. */
    template <typename Weight>
    Tree<Weight>::Tree(const unsigned int &nodes) : edges(nodes), n(nodes) {}

    template <typename Weight>
    template <typename DPTable>
    Weight Tree<Weight>::HeaviestSubtree(const Node &u, const Node &p, DPTable &dp) noexcept
    {
        auto &max{dp[u] = -oo};
        Weight total{};
        for (const auto &e : edges[u])
        {
            const auto &v{e.v};
            const auto &w{e.w};
            if (v != p)
            {
                const auto &&subtree{HeaviestSubtree(v, u, dp)};
                max = std::max(dp[v], max);
                /* Basically, Kadane's algorithm on trees. */
                if (subtree + w > 0)
                {
                    total += subtree + w;
                }
            }
        }
        max = std::max(total, max);
        return total;
    }

    template <typename Weight>
    void Tree<Weight>::AddEdge(const Node &u, const Node &v, const Weight &w) noexcept
    {
        edges[u].emplace_front(v, w);
        edges[v].emplace_front(u, w);
    }

    template <typename Weight>
    typename Tree<Weight>::Node Tree<Weight>::GetRandomNode() noexcept
    {
        std::uniform_int_distribution<Node> dist{};
        std::default_random_engine generator{n};
        return dist(generator) % static_cast<Node>(n);
    }

    template <typename Weight>
    Weight Tree<Weight>::HeaviestSubtree() noexcept
    {
        const auto &&r{GetRandomNode()};
        std::vector<Weight> dp(n);
        HeaviestSubtree(r, r, dp);
        return dp[r];
    }

}; // namespace Graphs