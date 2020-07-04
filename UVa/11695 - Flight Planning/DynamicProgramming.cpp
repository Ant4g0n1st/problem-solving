#include <forward_list>
#include <type_traits>
#include <iostream>
#include <limits>
#include <memory>
#include <random>
#include <vector>
#include <queue>

namespace Types
{

    using Int = unsigned int;

};

using namespace Types;

namespace Graphs
{

    template <typename NodeType, typename LengthType>
    struct Path
    {
        static_assert(std::is_integral<LengthType>::value);
        static_assert(std::is_integral<NodeType>::value);

        LengthType length{};
        NodeType center{};

        Path() = default;

        constexpr LengthType Midpoint() const noexcept;

        bool operator<(const Path &) const noexcept;

        constexpr LengthType Half() const noexcept;
    };

    template <typename NodeType>
    struct Edge
    {
        static_assert(std::is_integral<NodeType>::value);

        NodeType u{};
        NodeType v{};

        Edge() = default;
    };

    template <typename T>
    std::ostream &operator<<(std::ostream &, const Edge<T> &);

    template <typename T>
    class Tree
    {
        static_assert(std::is_integral<T>::value);

        using Node = unsigned int;
        using List = std::forward_list<Node>;

        const Node InvalidNode{std::numeric_limits<T>::max()};

        std::vector<Path<Node, T>> maxBranch{};
        std::vector<Path<Node, T>> maxRight{};
        std::vector<Path<Node, T>> maxLeft{};
        std::vector<Node> parent{};
        std::vector<List> edges{};
        const int n{};

    public:
        Tree(const int &);

        T OptimalRemoval(Edge<Node> &, Edge<Node> &) noexcept;

        void AddEdge(const Node &, const Node &) noexcept;

    private:
        template <typename DPTable>
        void MaxPath(const Node &, const Node &, DPTable &) noexcept;

        Node FarthestNode(const Node &) noexcept;

        Node GetRandomNode() noexcept;
    };

}; // namespace Graphs

namespace Solution
{

    void SolveProblem()
    {
        std::unique_ptr<Graphs::Tree<Int>> t{};
        int cases{}; std::cin >> cases;
        for (int c = 0; c < cases; c++)
        {
            int n{}; std::cin >> n;
            t.reset(new Graphs::Tree<Int>(n));
            for (int i = 1; i < n; i++)
            {
                int u{}; std::cin >> u;
                int v{}; std::cin >> v;
                t->AddEdge(--u, --v);
            }
            Graphs::Edge<Int> remove{};
            Graphs::Edge<Int> place{};
            std::cout << t->OptimalRemoval(remove, place) << '\n';
            std::cout << remove << '\n';
            std::cout << place << '\n';
        }
    }

    void OptimizeIOStreams()
    {
        std::ios_base::sync_with_stdio(false);
        std::cout.tie(nullptr);
        std::cin.tie(nullptr);
    }

} // namespace Solution

int main()
{
    Solution::OptimizeIOStreams();
    Solution::SolveProblem();
    return 0;
}

namespace Graphs
{

    /* Path Implementation. */
    template <typename NodeType, typename LengthType>
    bool Path<NodeType, LengthType>::operator<(const Path &other) const noexcept
    {
        if (length == other.length)
        {
            return center < other.center;
        }
        return length < other.length;
    }

    template <typename NodeType, typename LengthType>
    constexpr LengthType Path<NodeType, LengthType>::Midpoint() const noexcept
    {
        return length >> 1;
    }

    template <typename NodeType, typename LengthType>
    constexpr LengthType Path<NodeType, LengthType>::Half() const noexcept
    {
        return (length + 1) >> 1;
    }

    /* Output Stream Override for Edge Implementation. */
    template <typename NodeType>
    std::ostream &operator<<(std::ostream &out, const Edge<NodeType> &e)
    {
        out << e.u + 1 << ' ' << e.v + 1;
        return out;
    }

    /* Tree Implementation. */
    template <typename T>
    Tree<T>::Tree(const int &nodes) : edges(nodes), n(nodes) {}

    template <typename T>
    template <typename DPTable>
    void Tree<T>::MaxPath(const Node &u, const Node &p, DPTable &dp) noexcept
    {
        /*
            Keep track of the longest and second longest branches from
            this node, as well as their respective centers.
        */
        auto &first{maxBranch[u]};
        Path<Node, T> second{};
        first.center = second.center = InvalidNode;
        first.length = second.length = 0;
        auto &maxPath{dp[u]};
        parent[u] = p;
        for (const auto &v : edges[u])
        {
            if (v != p)
            {
                this->MaxPath(v, u, dp);
                maxPath = std::max(dp[v], maxPath);
                const auto &childMax{maxBranch[v]};
                if (first.length < childMax.length + 1)
                {
                    std::swap(first, second);
                    first = childMax;
                    ++first.length;
                    continue;
                }
                if (second.length < childMax.length + 1)
                {
                    second = childMax;
                    ++second.length;
                }
            }
        }
        /* If the node is a leaf, just return. */
        if (first.center == InvalidNode)
        {
            maxPath.center = first.center = u;
            return;
        }
        /*
            The center remains the same if the length becomes odd,
            but must go up if it becomes even.
            This is because the branch is it's own diameter,
            an odd length diameter has two centers
            and an even length one has only one.
        */
        if (first.length % 2 == 0)
        {
            first.center = parent[first.center];
        }
        /*
            If a diameter of the tree passes through this node,
            i.e. merging the longest branches improves the
            running maximum, create a path and move
            the center where it should be.
        */
        if (maxPath.length < first.length + second.length)
        {
            maxPath.length = first.length + second.length;
            maxPath.center = first.center;
            const auto &&jumps{maxPath.Midpoint() - first.Midpoint()};
            for (int i = 0; i < jumps; i++)
            {
                maxPath.center = parent[maxPath.center];
            }
        }
    }

    template <typename T>
    T Tree<T>::OptimalRemoval(Edge<Node> &remove, Edge<Node> &place) noexcept
    {
        /* Compute the endpoints of the diameter and the diameter itself. */
        maxBranch.resize(n), parent.resize(n);
        maxRight.resize(n), maxLeft.resize(n);
        const auto &&start{FarthestNode(GetRandomNode())};
        const auto &&end{FarthestNode(start)};
        std::vector<Node> diameter{};
        for (auto u{end}; parent[u] != InvalidNode; u = parent[u])
        {
            diameter.emplace_back(u);
        }
        diameter.emplace_back(start);
        /*
            Compute the diameters for all subtrees starting from
            one endpoint and then starting from the other.
            This helps when computing the new diameter when
            removing an edge from the original diameter in
            constant time.
        */
        MaxPath(start, start, maxLeft);
        MaxPath(end, end, maxRight);
        /*
            Minimize the residual diameter by removing
            edges from the original diameter.
        */
        Int min{std::numeric_limits<T>::max()};
        const auto &d{diameter.size()};
        for (int i = 1; i < d; i++)
        {
            const auto &u{diameter[i - 1]};
            const auto &v{diameter[i]};
            const auto &mv{maxRight[v]};
            const auto &mu{maxLeft[u]};
            const auto &&merge{mu.Half() + mv.Half()};
            auto path{std::max(mu.length, mv.length)};
            path = std::max(path, merge + 1);
            if (min > path)
            {
                place.u = std::min(mu.center, mv.center);
                place.v = std::max(mu.center, mv.center);
                remove.u = std::min(u, v);
                remove.v = std::max(u, v);
                min = path;
            }
        }
        return min;
    }

    template <typename T>
    typename Tree<T>::Node Tree<T>::FarthestNode(const Node &start) noexcept
    {
        /* Breadth First Search to find the farthest node. */
        parent[start] = InvalidNode;
        std::queue<Node> q{};
        auto farthest{start};
        q.emplace(start);
        while (!q.empty())
        {
            const auto &u{q.front()};
            q.pop();
            for (const auto &v : edges[u])
            {
                if (v != parent[u])
                {
                    parent[v] = u;
                    farthest = v;
                    q.push(v);
                }
            }
        }
        return farthest;
    }

    template <typename T>
    void Tree<T>::AddEdge(const Node &u, const Node &v) noexcept
    {
        edges[u].emplace_front(v);
        edges[v].emplace_front(u);
    }

    template <typename T>
    typename Tree<T>::Node Tree<T>::GetRandomNode() noexcept
    {
        /* Returns a random node from the tree. */
        std::uniform_int_distribution<Node> dist{};
        std::default_random_engine generator{n};
        return dist(generator) % static_cast<Node>(n);
    }

}; // namespace Graphs
