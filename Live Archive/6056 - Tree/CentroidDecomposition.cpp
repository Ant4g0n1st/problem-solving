#include <forward_list>
#include <iostream>
#include <memory>
#include <random>
#include <vector>
#include <queue>
#include <set>

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

        std::vector<W> maxBranch{};
        std::vector<List> edges{};
        std::vector<int> parent{};
        std::vector<W> longest{};
        std::vector<bool> cut{};
        std::vector<int> size{};
        std::vector<W> path{};
        const int n{};

    public:
        Tree(const int &);

        void AddEdge(const Node &, const Node &, const W &) noexcept;

        std::vector<W> &LongestPaths() noexcept;

    private:
        void ComputeWeight(const Node &, const Node &, const Node &) noexcept;

        void MaximizePath(const Node &, const Node &, const W &) noexcept;

        int SubtreeSize(const Node &, const Node &) noexcept;

        void MaximizePaths(const Node &) noexcept;

        Node FindCentroid(const Node &) noexcept;

        void LongestPaths(const Node &) noexcept;

        void RemoveNode(const Node &) noexcept;

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

    template <typename W>
    void Tree<W>::ComputeWeight(const Node &u, const Node &p, const Node &r) noexcept
    {
        /* Update the maximum stacked weight in the subtree. */
        maxBranch[r] = std::max(maxBranch[r], path[u]);
        for (const auto &e : edges[u])
        {
            const auto &v{e.v};
            if (!cut[v] and v != p)
            {
                /* Compute the stacked weight relative to a centroid. */
                path[v] = path[u] + e.w;
                ComputeWeight(v, u, r);
            }
        }
    }

    template <typename W>
    void Tree<W>::MaximizePath(const Node &u, const Node &p, const W &max) noexcept
    {
        /* 
            Update the farthest path for this node given the 
            maximum sibling branch in the level.
        */
        longest[u] = std::max(longest[u], path[u] + max);
        for (const auto &e : edges[u])
        {
            const auto &v{e.v};
            if (!cut[v] and v != p)
            {
                MaximizePath(v, u, max);
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
    typename Tree<W>::Node Tree<W>::FindCentroid(const Node &root) noexcept
    {
        /*
            Pre-computing the subtree size to find
            the centroid efficiently.
        */
        auto &&min{SubtreeSize(root, root)};
        std::queue<Node> q{};
        auto centroid{root};
        q.emplace(centroid);
        while (!q.empty())
        {
            const auto &u{q.front()};
            auto max{size[root]};
            max -= size[u];
            q.pop();
            for (const auto &e : edges[u])
            {
                const auto &v{e.v};
                if (!cut[v] and v != parent[u])
                {
                    max = std::max(size[v], max);
                    q.emplace(v);
                }
            }
            /* The centroid must have the smallest residual component. */
            if (max < min)
            {
                centroid = u;
                min = max;
            }
        }
        return centroid;
    }

    template <typename W>
    int Tree<W>::SubtreeSize(const Node &u, const Node &p) noexcept
    {
        /*
            A very well known approach to
            compute the subtree size.
        */
        auto &s{size[u] = 0};
        this->parent[u] = p;
        for (const auto &e : edges[u])
        {
            const auto &v{e.v};
            if (!cut[v] and v != p)
            {
                s += SubtreeSize(v, u);
            }
        }
        return ++s;
    }

    template <typename W>
    void Tree<W>::MaximizePaths(const Node &centroid) noexcept
    {
        /*
            First, find the longest branches
            of all adjacent subtrees.
        */
        std::multiset<W> max{};
        for (const auto &e : edges[centroid])
        {
            const auto &v{e.v};
            if (!cut[v])
            {
                maxBranch[v] = 0;
                path[v] = e.w;
                ComputeWeight(v, centroid, v);
                max.emplace(maxBranch[v]);
                if (longest[centroid] < maxBranch[v])
                {
                    longest[centroid] = maxBranch[v];
                }
            }
        }
        /*
            Then, compute the longest paths for each node
            by temporarily removing the subtree's 
            longest branch.
        */
        for (const auto &e : edges[centroid])
        {
            const auto &v{e.v};
            if (!cut[v])
            {
                auto it{max.find(maxBranch[v])};
                max.erase(it); W m{};
                if (max.rbegin() != max.rend())
                {
                    m = *max.rbegin();
                }
                MaximizePath(v, centroid, m);
                max.emplace(maxBranch[v]);
            }
        }
    }

    template <typename W>
    typename Tree<W>::Node Tree<W>::GetRandomNode() noexcept
    {
        /* Returns a random number in the range [0, n - 1]*/
        std::uniform_int_distribution<int> dist{};
        std::default_random_engine generator{n};
        return dist(generator) % n;
    }

    template <typename W>
    void Tree<W>::LongestPaths(const Node &start) noexcept
    {
        /* Find the centroid. */
        const auto &&centroid{FindCentroid(start)};
        RemoveNode(centroid);
        if (!size[centroid])
        {
            return;
        }
        /* Attempt to maximize. */
        this->MaximizePaths(centroid);
        /* Compute the next decomposition level. */
        for (const auto &e : edges[centroid])
        {
            const auto &v{e.v};
            if (!cut[e.v])
            {
                LongestPaths(v);
            }
        }
    }

    template <typename W>
    std::vector<W> &Tree<W>::LongestPaths() noexcept
    {
        const auto &&start{GetRandomNode()};
        maxBranch.resize(n), cut.resize(n);
        longest.resize(n), path.resize(n);
        parent.resize(n), size.resize(n);
        this->LongestPaths(start);
        return longest;
    }

    template <typename W>
    void Tree<W>::RemoveNode(const Node &u) noexcept
    {
        this->cut[u] = true;
    }

}; // namespace Graphs