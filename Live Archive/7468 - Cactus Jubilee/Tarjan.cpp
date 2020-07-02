#include <forward_list>
#include <type_traits>
#include <iostream>
#include <limits>
#include <memory>
#include <random>
#include <vector>

namespace Types
{

    using Long = unsigned long long int;
    using Int = unsigned int;

}; // namespace Types

using namespace Types;

namespace DataStructures
{

    template <typename ElementType>
    class Stack
    {
        std::vector<ElementType> elements{};
        int pointer{-1};

    public:
        Stack() = default;

        void Push(const ElementType &) noexcept;

        const ElementType &Top() noexcept;

        const int &GetPointer() noexcept;

        void Resize(const Int &);

        bool Empty() noexcept;

        void Pop() noexcept;

        const ElementType &operator[](const int &) noexcept;
    };

}; // namespace DataStructures

namespace Graphs
{

    template <typename NodeType, typename WeightType>
    struct Edge
    {
        static_assert(std::is_integral<WeightType>::value);
        static_assert(std::is_integral<NodeType>::value);

        bool isBridge{false};
        NodeType u{}, v{};
        bool used{false};
        WeightType w{};

        Edge(const NodeType &, const NodeType &);

        const NodeType &GetDestination(const NodeType &) noexcept;
    };

    template <typename NodeType, typename SizeType>
    struct Component
    {
        static_assert(std::is_integral<NodeType>::value);

        std::vector<NodeType> members{};
        bool isCycle{false};
        SizeType size{};

        Component() = default;
    };

    template <typename T>
    class Cactus
    {
        static_assert(std::is_integral<T>::value);

        using Node = unsigned int;
        using List = std::forward_list<Int>;

        const Node InvalidNode{std::numeric_limits<Node>::max()};
        const Int InvalidID{std::numeric_limits<Int>::max()};

        std::vector<Component<Node, T>> components{};
        std::vector<Edge<Node, T>> edges{};
        DataStructures::Stack<Int> stack{};
        std::vector<List> connected{};
        std::vector<bool> treeNode{};
        std::vector<Int> lowlink{};
        std::vector<Int> index{};
        std::vector<Int> super{};
        std::vector<bool> seen{};
        const int n{};
        int m{};

    public:
        Cactus(const int &);

        void AddEdge(const Node &, const Node &) noexcept;

        T CountTheWays() noexcept;

    private:
        void CompressTree(const Node &, const Node &, const Int &) noexcept;

        Int DetectBridges(const Node &, const Node &, Int &) noexcept;

        void CompressCycle(const Int &) noexcept;

        void DetectComponents() noexcept;

        Node GetRandomNode() noexcept;
    };

}; // namespace Graphs

namespace Solution
{

    void SolveProblem()
    {
        std::unique_ptr<Graphs::Cactus<Long>> c{};
        int m{}, n{};
        while (std::cin >> n >> m)
        {
            c.reset(new Graphs::Cactus<Long>(n));
            for (int i = 0; i < m; i++)
            {
                int k{}; std::cin >> k;
                int u{}; std::cin >> u, --u;
                for (int j = 1; j < k; j++)
                {
                    int v{}; std::cin >> v;
                    c->AddEdge(u, --v);
                    u = v;
                }
            }
            std::cout << c->CountTheWays();
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

    template <typename ElementType>
    const ElementType &Stack<ElementType>::operator[](const int &i) noexcept
    {
        return elements[i];
    }

    template <typename ElementType>
    void Stack<ElementType>::Push(const ElementType &element) noexcept
    {
        elements[++pointer] = element;
    }

    template <typename ElementType>
    const ElementType &Stack<ElementType>::Top() noexcept
    {
        return elements[pointer];
    }

    template <typename ElementType>
    const int &Stack<ElementType>::GetPointer() noexcept
    {
        return this->pointer;
    }

    template <typename ElementType>
    void Stack<ElementType>::Resize(const Int &n)
    {
        this->elements.resize(n);
    }

    template <typename ElementType>
    bool Stack<ElementType>::Empty() noexcept
    {
        return pointer < 0;
    }

    template <typename ElementType>
    void Stack<ElementType>::Pop() noexcept
    {
        this->pointer--;
    }

}; // namespace DataStructures

namespace Graphs
{

    /* Edge Implementation. */
    template <typename NodeType, typename WeightType>
    Edge<NodeType, WeightType>::Edge(const NodeType &x, const NodeType &y) : u(x), v(y) {}

    template <typename NodeType, typename WeightType>
    const NodeType &Edge<NodeType, WeightType>::GetDestination(const NodeType &node) noexcept
    {
        return u == node ? v : u;
    }

    /* Cactus Implementation. */
    template <typename T>
    Cactus<T>::Cactus(const int &nodes) : connected(nodes), n(nodes) {}

    template <typename T>
    void Cactus<T>::CompressTree(const Node &u, const Node &p, const Int &s) noexcept
    {
        /*
            Color a tree that connects cycles and compute the total size.
            Needed to compute the formula efficiently.
        */
        components[super[u] = s].size++;
        this->seen[u] = true;
        for (const auto &id : connected[u])
        {
            /* All the edges in a tree are bridges. */
            const auto &v{edges[id].GetDestination(u)};
            const auto &e{edges[id]};
            if (v != p and e.isBridge)
            {
                CompressTree(v, u, s);
            }
        }
    }

    template <typename T>
    Int Cactus<T>::DetectBridges(const Node &u, const Node &p, Int &i) noexcept
    {
        /*
            Bridge & biconnected component detection using Tarjan's algorithm.
            The only twist is keeping track of the implicit DFS subtree size
            to compute the cost of a bridge efficiently.
        */
        index[u] = lowlink[u] = ++i;
        Int subtreeSize{0};
        for (const auto &id : connected[u])
        {
            const auto &v{edges[id].GetDestination(u)};
            Edge<Node, T> &edge{edges[id]};
            if (v == p)
            {
                continue;
            }
            if (!edge.used)
            {
                edge.used = true;
                stack.Push(id);
            }
            if (!index[v])
            {
                const auto &&size{DetectBridges(v, u, i)};
                if (index[u] <= lowlink[v])
                {
                    CompressCycle(id);
                }
                if (index[u] < lowlink[v])
                {
                    edge.isBridge = true;
                    edge.w = size;
                }
                if (lowlink[v] < lowlink[u])
                {
                    lowlink[u] = lowlink[v];
                }
                subtreeSize += size;
            }
            if (index[v] < lowlink[u])
            {
                lowlink[u] = index[v];
            }
        }
        return ++subtreeSize;
    }

    template <typename T>
    T Cactus<T>::CountTheWays() noexcept
    {
        treeNode.resize(n), seen.resize(n);
        lowlink.resize(n), super.resize(n);
        index.resize(n), stack.Resize(m);
        DetectComponents();
        T treeTotal{};
        /*
            Pre-compute the sum of all edge placements
            within every tree within the cactus.
        */
        for (const auto &component : components)
        {
            if (!component.isCycle)
            {
                const auto &s{component.size};
                treeTotal += ((s - 1) * (s - 2)) >> 1;
            }
        }
        /*
            It's possible to move an edge from a cycle to any
            of the trees within the cactus, including the
            newly created tree after removing the edge.
        */
        T totalWays{};
        for (const auto &component : components)
        {
            if (component.isCycle)
            {
                T currentTotal{treeTotal};
                T size{component.size};
                for (const auto &u : component.members)
                {
                    /*
                        Merge the tree created from the cycle with
                        all trees neighbouring the former cycle.
                    */
                    if (treeNode[u])
                    {
                        const auto &s{components[super[u]].size};
                        currentTotal -= ((s - 1) * (s - 2)) >> 1;
                        size += (s - 1);
                    }
                }
                /*
                    For every edge removed from the cycle, place it in
                    the new tree or place it in any other tree.
                */
                totalWays += ((size * (size - 3)) >> 1) * component.size;
                totalWays += currentTotal * component.size;
            }
        }
        /*
            Removing a bridge causes the cactus to partition into two
            disjoint components, thus all possible ways to place an
            edge there is equivalent to the product of the sizes
            of both components, minus the edge been removed.
        */
        auto nodes{static_cast<Long>(n)};
        for (const auto &e : edges)
        {
            const auto &w{e.w};
            if (e.isBridge)
            {
                totalWays += (w * (nodes - w) - 1);
            }
        }
        return totalWays;
    }

    template <typename T>
    void Cactus<T>::AddEdge(const Node &u, const Node &v) noexcept
    {
        connected[u].emplace_front(m);
        connected[v].emplace_front(m);
        edges.emplace_back(u, v);
        this->m++;
    }

    template <typename T>
    typename Cactus<T>::Node Cactus<T>::GetRandomNode() noexcept
    {
        /* Returns a random node from the cactus. */
        std::uniform_int_distribution<Node> dist{};
        std::default_random_engine generator{n};
        return dist(generator) % static_cast<Node>(n);
    }

    template <typename T>
    void Cactus<T>::CompressCycle(const Int &id) noexcept
    {
        /*
            Using a stack with Tarjan's algorithm to
            compress the biconnected components.
        */
        if (stack.Empty())
        {
            return;
        }
        auto end{stack.GetPointer()};
        while (!stack.Empty())
        {
            if (stack.Top() == id)
            {
                break;
            }
            stack.Pop();
        }
        auto begin{stack.GetPointer()};
        begin = std::max(begin, 0);
        this->stack.Pop();
        if (end - begin == 0)
        {
            return;
        }
        /*
            Using flags to achieve node uniqueness
            in linear time over the cycle size.
        */
        this->components.emplace_back();
        auto &cycle{components.back()};
        cycle.isCycle = true;
        for (auto i = begin; i <= end; i++)
        {
            const auto &edge{edges[stack[i]]};
            const auto &u{edge.u};
            if (!seen[u])
            {
                cycle.members.push_back(u);
                seen[u] = true;
            }
            const auto &v{edge.v};
            if (!seen[v])
            {
                cycle.members.push_back(v);
                seen[v] = true;
            }
        }
        for (auto i = begin; i <= end; i++)
        {
            const auto &edge{edges[stack[i]]};
            seen[edge.u] = false;
            seen[edge.v] = false;
        }
        cycle.size = cycle.members.size();
    }

    template <typename T>
    void Cactus<T>::DetectComponents() noexcept
    {
        /* 
            First, label the bridges and
            compress the cycles.
        */
        const auto &&r{GetRandomNode()};
        Int time{};
        DetectBridges(r, r, time);
        /*
            This is just in case there's a
            leftover cycle in the stack. 
        */
        CompressCycle(InvalidID);
        /*
            Label the endpoints of the bridges
            to efficiently identify the trees
            adjacent to each of the cycles.
        */
        for (const auto &e : edges)
        {
            if (e.isBridge)
            {
                treeNode[e.u] = true;
                treeNode[e.v] = true;
            }
        }
        /*
            Compress all the trees
            in the cactus.
        */
        Int s{components.size()};
        for (Node u = 0; u < n; u++)
        {
            if (treeNode[u] and !seen[u])
            {
                components.emplace_back();
                CompressTree(u, u, s);
                s = components.size();
            }
        }
    }

}; // namespace Graphs