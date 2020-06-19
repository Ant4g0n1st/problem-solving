#include <forward_list>
#include <algorithm>
#include <iostream>
#include <sstream>
#include <limits>
#include <memory>
#include <string>
#include <vector>
#include <queue>

namespace Types
{

    using Long = long long int;

};

using namespace Types;

namespace Graphs
{

    template <typename T>
    struct Edge
    {
        int v{};
        T w{};

        Edge(const int &u, const T &weight);
    };

    template <typename T>
    struct Path : public Edge<T>
    {
        using Edge<T>::Edge;

        bool operator<(const Path &other) const noexcept;
    };

    template <typename T>
    class Graph
    {
        using Queue = std::priority_queue<Path<T>>;
        using List = std::forward_list<Edge<T>>;

        const T oo{std::numeric_limits<T>::max()};

        std::vector<bool> station{};
        std::vector<List> edges{};
        std::vector<bool> seen{};
        const int n{};

    public:
        const int InvalidNode{std::numeric_limits<int>::max()};

        Graph(int nodes);

        void AddEdge(const int &u, const int &v, const T &w) noexcept;

        void SetStation(const int &node) noexcept;

        int FindOptimalPlacement() noexcept;

    private:
        void ShortestPaths(Queue &q, std::vector<T> &d) noexcept;
    };

}; // namespace Graphs

namespace Solution
{

    void OptimizeIOStreams()
    {
        std::ios_base::sync_with_stdio(false);
        std::cout.tie(nullptr);
        std::cin.tie(nullptr);
    }

    void SolveProblem()
    {
        std::unique_ptr<Graphs::Graph<Long>> g{};
        int cases{}; std::cin >> cases;
        for (int t = 0; t < cases; t++)
        {
            if (t) std::cout << '\n';
            int f{}; std::cin >> f;
            int n{}; std::cin >> n;
            g.reset(new Graphs::Graph<Long>{n});
            for (int i = 0; i < f; i++)
            {
                int u{}; std::cin >> u;
                g->SetStation(--u);
            }
            std::istringstream is{};
            std::string input{};
            std::getline(std::cin, input);
            while (std::getline(std::cin, input))
            {
                is.seekg(std::ios_base::beg);
                if (input.empty()) break;
                is.str(input);
                int u{}; is >> u, --u;
                int v{}; is >> v, --v;
                Long w{}; is >> w;
                g->AddEdge(u, v, w);
            }
            auto u{g->FindOptimalPlacement()};
            std::cout << ++u << '\n';
        }
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
    template <typename T>
    Edge<T>::Edge(const int &u, const T &weight) : v(u), w(weight) {}

    /* Path Implementation. */
    template <typename T>
    bool Path<T>::operator<(const Path &other) const noexcept
    {
        /* This is one way to make a min priority queue using STL. */
        return this->w > other.w;
    }

    /* Graph Implementation. */
    template <typename T>
    Graph<T>::Graph(int nodes) : edges(nodes), n(nodes), station(nodes) {}

    template <typename T>
    void Graph<T>::AddEdge(const int &u, const int &v, const T &w) noexcept
    {
        edges[u].emplace_front(v, w);
        edges[v].emplace_front(u, w);
    }

    template <typename T>
    void Graph<T>::ShortestPaths(Queue &q, std::vector<T> &d) noexcept
    {
        /* Just normal Dijkstra. */
        while (!q.empty())
        {
            const auto &p{q.top()};
            const auto u{p.v};
            q.pop();
            if (seen[u]) continue;
            seen[u] = true;
            for (const auto &e : edges[u])
            {
                if (seen[e.v]) continue;
                const auto& v{e.v};
                if (d[u] + e.w < d[v])
                {
                    d[v] = d[u] + e.w;
                    q.emplace(v, d[v]);
                }
            }
        }
    }

    template <typename T>
    int Graph<T>::FindOptimalPlacement() noexcept
    {
        /* Compute the shortest paths from all fire stations. */
        std::vector<T> df(n, oo);
        seen.assign(n, false);
        Graph::Queue q{};
        for (int s = 0; s < n; s++)
        {
            if (!station[s]) continue;
            q.emplace(s, df[s] = 0);
        }
        ShortestPaths(q, df);
        /* Now we minimize using the distances computed above. */
        T min{*std::max_element(df.begin(), df.end())};
        int best{Graph::InvalidNode};
        std::vector<T> dc{};
        for (int u = 0; u < n; u++)
        {
            if (!station[u])
            {
                /* Find the shortest paths from the city. */
                dc.assign(df.begin(), df.end());
                q.emplace(u, dc[u] = 0);
                seen.assign(n, false);
                ShortestPaths(q, dc);
                /* Minimize the maximum distance. */
                auto m{*std::max_element(dc.begin(), dc.end())};
                if (m < min) best = u, min = m;
            }
        }
        /* If no city was found, place in the first fire station. */
        if (best == Graph::InvalidNode)
        {
            for (int u = 0; u < n; u++)
            {
                if (station[u])
                {
                    best = u;
                    break;
                }
            }
        }
        return best;
    }

    template <typename T>
    void Graph<T>::SetStation(const int &node) noexcept
    {
        station[node] = true;
    }

}; // namespace Graphs