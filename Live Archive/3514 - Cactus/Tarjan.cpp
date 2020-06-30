#include <forward_list>
#include <type_traits>
#include <exception>
#include <iostream>
#include <iterator>
#include <memory>
#include <vector>

namespace Types
{

    using Int = unsigned int;

};

using namespace Types;

namespace Constants
{

    const Int BASE10{10};

};

namespace Exceptions
{

    class NotACactus : virtual public std::exception
    {
        using std::exception::exception;

    public:
        virtual const char *what() const throw() override;
    };

}; // namespace Exceptions

namespace Math
{

    template <typename T, T Base = static_cast<T>(Constants::BASE10)>
    class BigInteger
    {
        static_assert(std::is_integral<T>::value);

        std::vector<T> digits{};

    public:
        BigInteger(T = T{}) noexcept;

        const std::vector<T> &GetDigits() const noexcept;

        BigInteger &operator*=(const T &) noexcept;
    };

    template <typename T, T Base>
    std::ostream &operator<<(std::ostream &output, const BigInteger<T, Base> &integer);

}; // namespace Math

namespace DataStructures
{

    template <typename T>
    class Stack
    {
        std::vector<T> elements{};
        int pointer{-1};

    public:
        typename std::vector<T>::iterator Top();

        void Push(const T &) noexcept;

        void Resize(const int &);

        bool Empty() noexcept;

        void Pop() noexcept;
    };

}; // namespace DataStructures

namespace Graphs
{

    template <typename NodeType>
    struct Edge
    {
        NodeType v{};
        Int id{};

        Edge(const NodeType &, const Int &);
    };

    class Graph
    {
        using Node = unsigned int;
        using List = std::forward_list<Edge<Node>>;

        std::vector<std::pair<Node, Node>> pairs{};
        DataStructures::Stack<Int> stack{};
        std::vector<Int> biconnected{};
        std::vector<Int> lowlink{};
        std::vector<List> edges{};
        std::vector<bool> seen{};
        std::vector<bool> used{};
        std::vector<Int> index{};
        Int edgeCount{};
        const int n{};

    public:
        Graph(const int &);

        void AddEdge(const Node &, const Node &) noexcept;

        Math::BigInteger<Int> Cactusness();

    private:
        void DetectBiconnected(const Node &, const Node &, Int &);

        void AddBiconnected(const Int &);

        void DetectBiconnected();
    };

}; // namespace Graphs

namespace Solution
{

    void SolveProblem()
    {
        std::unique_ptr<Graphs::Graph> g{};
        int m{}, n{}; bool firstCase{true};
        while (std::cin >> n >> m)
        {
            g.reset(new Graphs::Graph(n));
            if (!firstCase)
            {
                std::cout << '\n';
            }
            firstCase = false;
            for (int i = 0; i < m; i++)
            {
                int k{}; std::cin >> k;
                int u{}; std::cin >> u, --u;
                for (int j = 1; j < k; j++)
                {
                    int v{}; std::cin >> v;
                    g->AddEdge(u, --v);
                    u = v;
                }
            }
            try
            {
                std::cout << g->Cactusness();
            }
            catch (const Exceptions::NotACactus &e)
            {
                std::cout << 0;
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

namespace Exceptions
{

    /* NotACactus Implementation. */
    const char *NotACactus::what() const throw()
    {
        return "The graph is not a cactus.";
    }

}; // namespace Exceptions

namespace Math
{

    /* Output stream overload for BigInteger. */
    template <typename T, T Base>
    std::ostream &operator<<(std::ostream &output, const BigInteger<T, Base> &integer)
    {
        auto iterator{integer.GetDigits().rbegin()};
        auto end{integer.GetDigits().rend()};
        for (; iterator != end; iterator++)
        {
            output << static_cast<char>(*iterator + '0');
        }
        return output;
    }

    /* BigInteger Implementation. */
    template <typename T, T Base>
    BigInteger<T, Base>::BigInteger(T value) noexcept
    {
        for (; value > 0; value /= Base)
        {
            digits.emplace_back(value % Base);
        }
    }

    template <typename T, T Base>
    BigInteger<T, Base> &BigInteger<T, Base>::operator*=(const T &v) noexcept
    {
        T carry{};
        for (int i = 0; i < digits.size() or carry; i++)
        {
            if (i == digits.size())
            {
                digits.emplace_back();
            }
            const auto &&product{digits[i] * v + carry};
            digits[i] = product % Base;
            carry = product / Base;
        }
        return *this;
    }

    template <typename T, T Base>
    const std::vector<T> &BigInteger<T, Base>::GetDigits() const noexcept
    {
        return digits;
    }

}; // namespace Math

namespace DataStructures
{

    /* Stack Implementation. */
    template <typename T>
    typename std::vector<T>::iterator Stack<T>::Top()
    {
        return elements.begin() + pointer;
    }

    template <typename T>
    void Stack<T>::Push(const T &element) noexcept
    {
        elements[++pointer] = element;
    }

    template <typename T>
    void Stack<T>::Resize(const int &n)
    {
        this->elements.resize(n);
    }

    template <typename T>
    bool Stack<T>::Empty() noexcept
    {
        return pointer == -1;
    }

    template <typename T>
    void Stack<T>::Pop() noexcept
    {
        this->pointer--;
    }

}; // namespace DataStructures

namespace Graphs
{

    /* Edge Implementation. */
    template <typename NodeType>
    Edge<NodeType>::Edge(const NodeType &u, const Int &i) : id(i), v(u) {}

    /* Graph Implementation. */
    Graph::Graph(const int &nodes) : edges(nodes), n(nodes) {}

    void Graph::DetectBiconnected(const Node &u, const Node &p, Int &i)
    {
        index[u] = lowlink[u] = ++i;
        Int children{0};
        for (const auto &e : edges[u])
        {
            const auto &v{e.v};
            if (v == p && !children++)
            {
                continue;
            }
            if (!used[e.id])
            {
                used[e.id] = true;
                stack.Push(e.id);
            }
            if (!index[v])
            {
                DetectBiconnected(v, u, i);
                if (index[u] <= lowlink[v])
                {
                    AddBiconnected(e.id);
                }
                if (lowlink[v] < lowlink[u])
                {
                    lowlink[u] = lowlink[v];
                }
            }
            if (index[v] < lowlink[u])
            {
                lowlink[u] = index[v];
            }
        }
    }

    void Graph::AddEdge(const Node &u, const Node &v) noexcept
    {
        edges[u].emplace_front(v, edgeCount);
        edges[v].emplace_front(u, edgeCount);
        pairs.emplace_back(u, v);
        this->edgeCount++;
    }

    void Graph::AddBiconnected(const Int &id)
    {
        if (!stack.Empty())
        {
            auto end{stack.Top()};
            while (!stack.Empty())
            {
                if (*stack.Top() == id)
                {
                    break;
                }
                stack.Pop();
            }
            auto begin{stack.Top()};
            std::advance(end, 1);
            Int componentNodes{};
            Int componentEdges{};
            this->stack.Pop();
            for (auto it{begin}; it != end; it++)
            {
                const auto &p{pairs[*it]};
                if (!seen[p.first])
                {
                    seen[p.first] = true;
                    ++componentNodes;
                }
                if (!seen[p.second])
                {
                    seen[p.second] = true;
                    ++componentNodes;
                }
                ++componentEdges;
            }
            if (componentEdges > componentNodes)
            {
                throw Exceptions::NotACactus();
            }
            for (auto it{begin}; it != end; it++)
            {
                const auto &p{pairs[*it]};
                seen[p.second] = false;
                seen[p.first] = false;
            }
            biconnected.emplace_back(componentEdges);
        }
    }

    void Graph::DetectBiconnected()
    {
        used.resize(edgeCount), seen.resize(n);
        lowlink.resize(n), index.resize(n);
        this->stack.Resize(edgeCount);
        bool firstCall{true};
        Int time{};
        for (Node u = 0; u < n; u++)
        {
            if (!index[u])
            {
                if (!firstCall)
                {
                    throw Exceptions::NotACactus{};
                }
                DetectBiconnected(u, u, time);
                firstCall = false;
            }
        }
    }

    Math::BigInteger<Int> Graph::Cactusness()
    {
        this->DetectBiconnected();
        Math::BigInteger<Int> cactusness{1};
        for (const auto &size : biconnected)
        {
            if (size > 1)
            {
                cactusness *= (size + 1);
            }
        }
        return cactusness;
    }

}; // namespace Graphs