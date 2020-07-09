#include <forward_list>
#include <type_traits>
#include <iostream>
#include <limits>
#include <memory>
#include <string>
#include <vector>
#include <queue>
#include <map>

namespace Types
{

    using Int = unsigned int;

}; // namespace Types

using namespace Types;

namespace DataStructures
{

    template <typename AlphabetType, typename OutputType>
    class Trie
    {
        static_assert(std::is_integral<AlphabetType>::value);

        std::map<AlphabetType, Trie> children{};
        OutputType value{};

    public:
        Trie() = default;

        OutputType &operator[](const std::basic_string<AlphabetType> &) noexcept;

        Trie &GetChild(const AlphabetType &) noexcept;
    };

}; // namespace DataStructures

namespace Exceptions
{

    class NotConnected : virtual public std::exception
    {
        using std::exception::exception;

    public:
        virtual const char *what() const throw() override;
    };

}; // namespace Exceptions

namespace Graphs
{

    template <typename NodeType, typename WeightType>
    struct Edge
    {
        WeightType w{};
        NodeType v{};

        Edge(const NodeType &, const WeightType &);
    };

    template <typename T>
    class Graph
    {
        using Node = unsigned int;
        using List = std::forward_list<Edge<Node, T>>;

        const Int oo{std::numeric_limits<Int>::max()};

        std::vector<List> edges{};
        const int n{};

    public:
        Graph(const int &);

        void AddEdge(const Node &, const Node &, const T &) noexcept;

        T MinimumDistanceST(const Node &) const;
    };

}; // namespace Graphs

namespace Solution
{

    void SolveProblem()
    {
        for (int m{}, n{}; std::cin >> n >> m;)
        {
            std::unique_ptr<Graphs::Graph<Int>> g{};
            DataStructures::Trie<char, Int> trie{};
            for (int i = 0; i < n; i++)
            {
                std::string language{};
                std::cin >> language;
                trie[language] = i;
            }
            const auto &s{trie["English"] = n++};
            g.reset(new Graphs::Graph<Int>(n));
            for (int i = 0; i < m; i++)
            {
                std::string language{};
                std::cin >> language;
                const auto &u{trie[language]};
                std::cin >> language;
                const auto &v{trie[language]};
                Int w{}; std::cin >> w;
                g->AddEdge(u, v, w);
            }
            try
            {
                std::cout << g->MinimumDistanceST(s);
            }
            catch (const Exceptions::NotConnected &e)
            {
                std::cout << "Impossible";
            }
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

    /* Trie Implementation. */
    template <typename AlphabetType, typename OutputType>
    Trie<AlphabetType, OutputType> &Trie<AlphabetType, OutputType>::GetChild(const AlphabetType &label) noexcept
    {
        return children[label];
    }

    template <typename AlphabetType, typename OutputType>
    OutputType &Trie<AlphabetType, OutputType>::operator[](const std::basic_string<AlphabetType> &key) noexcept
    {
        /*
            Due to the input format, this implementation can be minimalistic.
            Overriding the subscript operator because it looks cool
            and works like std::map.
        */
        Trie *currentNode{this};
        for (int i = 0; i < key.size(); i++)
        {
            currentNode = &currentNode->GetChild(key[i]);
        }
        return currentNode->value;
    }

}; // namespace DataStructures

namespace Exceptions
{

    /* NotConnected Implementation. */
    const char *NotConnected::what() const throw()
    {
        return "The graph is not connected.";
    }

}; // namespace Exceptions

namespace Graphs
{

    /* DirectedEdge Implementation. */
    template <typename NodeType, typename WeightType>
    Edge<NodeType, WeightType>::Edge(const NodeType &u, const WeightType &weight) : v(u), w(weight) {}

    /* Graph Implementaion. */
    template <typename T>
    Graph<T>::Graph(const int &nodes) : n(nodes), edges(nodes) {}

    template <typename T>
    void Graph<T>::AddEdge(const Node &u, const Node &v, const T &w) noexcept
    {
        edges[u].emplace_front(v, w);
        edges[v].emplace_front(u, w);
    }

    template <typename T>
    T Graph<T>::MinimumDistanceST(const Node &source) const
    {
        /* 
            Breadth First Search guarantees the shortest distance from the source,
            the way to minimize the total cost of the spanning subtree is by
            minimizing the incoming edge of a node from all it's possible
            parents, i.e. adjacent nodes from the previous search level.
        */
        std::vector<Int> depth(n, oo);
        std::vector<T> weight(n);
        std::queue<Node> q{};
        q.emplace(source);
        depth[source] = 0;
        T totalCost{};
        while (!q.empty())
        {
            const auto &u{q.front()};
            for (const auto &e : edges[u])
            {
                const auto &v{e.v};
                const auto &w{e.w};
                if (depth[u] + 1 == depth[v])
                {
                    /*
                        If this node would be my child,
                        would that improve the cost?
                    */
                    auto &m{weight[v]};
                    m = std::min(m, w);
                }
                if (depth[u] + 1 < depth[v])
                {
                    depth[v] = depth[u] + 1;
                    weight[v] = w;
                    q.push(v);
                }
            }
            q.pop();
        }
        for (int u = 0; u < n; u++)
        {
            if (depth[u] == oo)
            {
                /*
                    A value of infinity means the node
                    was never reached.
                */
                throw Exceptions::NotConnected{};
            }
            totalCost += weight[u];
        }
        return totalCost;
    }

}; // namespace Graphs