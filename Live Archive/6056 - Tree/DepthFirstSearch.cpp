#include <forward_list>
#include <iostream>
#include <memory>
#include <random>
#include <vector>

namespace Types
{

    using Int = unsigned int;

};

using namespace Types;

namespace Graphs
{

    template <typename NodeType, typename WeightType>
    struct Edge
    {
        WeightType w{};
        NodeType v{};

        Edge(const NodeType &, const WeightType &);
    };

    template <typename W>
    class Tree
    {
        using Node = unsigned int;
        using List = std::forward_list<Edge<Node, W>>;

        std::vector<std::pair<W, W>> maxChildren{};
        std::vector<List> edges{};
        std::vector<W> longest{};
        std::vector<W> path{};
        const int n{};

    public:
        Tree(const int &);

        void AddEdge(const Node &, const Node &, const W &) noexcept;

        std::vector<W> &LongestPaths() noexcept;

    private:
        void MaximizePath(const Node &, const Node &, const W & = 0) noexcept;

        W ComputeWeight(const Node &, const Node &) noexcept;

        void LongestPaths(const Node &) noexcept;

        Node GetRandomNode() noexcept;
    };

}; // namespace Graphs

namespace Solution
{

    void SolveProblem()
    {
        std::unique_ptr<Graphs::Tree<Int>> t{}; int n{};
        while (std::cin >> n)
        {
            t.reset(new Graphs::Tree<Int>(n));
            for (int i = 1; i < n; i++)
            {
                int u; std::cin >> u, --u;
                int v; std::cin >> v, --v;
                Int w; std::cin >> w;
                t->AddEdge(u, v, w);
            }
            const auto &paths{t->LongestPaths()};
            for (const auto &w : paths)
            {
                std::cout << w << '\n';
            }
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

namespace Graphs
{

    /* Edge Implementation. */
    template <typename N, typename W>
    Edge<N, W>::Edge(const N &u, const W &weight) : v(u), w(weight) {}

    /* Tree Implementation. */
    template <typename W>
    Tree<W>::Tree(const int &nodes) : edges(nodes), n(nodes) {}

    /* 'max' contains the longest running path disjoint with this subtree. */
    template <typename W>
    void Tree<W>::MaximizePath(const Node &u, const Node &p, const W &max) noexcept
    {
        /* Maximize between the paths above and below the node. */
        longest[u] = std::max(maxChildren[u].first, max);
        for (const auto &e : edges[u])
        {
            const auto &v{e.v};
            const auto &w{e.w};
            if (v != p)
            {
                /* Pick the heaviest branch disjoint with this child. */
                auto m{maxChildren[u].first};
                if (m == maxChildren[v].first + w)
                {
                    m = maxChildren[u].second;
                }
                /* Continue the running path or reset it. */
                MaximizePath(v, u, std::max(m, max) + w);
            }
        }
    }

    template <typename W>
    void Tree<W>::AddEdge(const Node &u, const Node &v, const W &w) noexcept
    {
        edges[u].emplace_front(v, w);
        edges[v].emplace_front(u, w);
    }

    template <typename W>
    W Tree<W>::ComputeWeight(const Node &u, const Node &p) noexcept
    {
        auto &second{maxChildren[u].second};
        auto &first{maxChildren[u].first};
        for (const auto &e : edges[u])
        {
            const auto &v{e.v};
            const auto &w{e.w};
            if (v != p)
            {
                /*
                    Maximize the heaviest branch from this node and
                    keep track of the second heaviest as well.
                */
                path[v] = path[u] + w;
                const auto &&branch{ComputeWeight(v, u)};
                if (first <= branch - path[u])
                {
                    std::swap(first, second);
                    first = branch - path[u];
                    continue;
                }
                if (second < branch - path[u])
                {
                    second = branch - path[u];
                }
            }
        }
        return path[u] + first;
    }

    template <typename W>
    typename Tree<W>::Node Tree<W>::GetRandomNode() noexcept
    {
        /* Returns a random number in the range [0, n - 1] */
        std::uniform_int_distribution<Node> dist{};
        std::default_random_engine generator{n};
        return dist(generator) % static_cast<Node>(n);
    }

    template <typename W>
    void Tree<W>::LongestPaths(const Node &root) noexcept
    {
        this->ComputeWeight(root, root);
        this->MaximizePath(root, root);
    }

    template <typename W>
    std::vector<W> &Tree<W>::LongestPaths() noexcept
    {
        maxChildren.resize(n);
        longest.resize(n);
        path.resize(n);
        LongestPaths(GetRandomNode());
        return this->longest;
    }

}; // namespace Graphs