#include <algorithm>
#include <iostream>
#include <numeric>
#include <array>
#include <map>

namespace Constants
{

    constexpr int N = 1e5;

}

namespace Types
{

    using ValueType = long long int;

}

namespace IO
{

    static void OptimizeStandardIO()
    {
        std::ios_base::sync_with_stdio(false);
        std::cout.tie(nullptr);
        std::cin.tie(nullptr);
    }

}

namespace DisjointSets
{

    using Node = int;

    std::array<Types::ValueType, Constants::N> component_size;
    std::array<int, Constants::N> parent;
    int n{};

    static void Initialize(const int &n)
    {
        std::fill(component_size.begin(), component_size.begin() + n, 1);
        std::iota(parent.begin(), parent.begin() + n, 0);
        DisjointSets::n = n;
    }

    static int Root(const Node &node)
    {
        if (node == parent[node])
        {
            return node;
        }
        return parent[node] = Root(parent[node]);
    }

    static std::pair<Types::ValueType, Types::ValueType> Join(const Node &u, const Node &v)
    {
        const auto &root_u = Root(u);
        const auto &root_v = Root(v);
        // We know every edge will connect two components, no need to test for same component.
        const auto component_sizes = std::make_pair(component_size[root_u], component_size[root_v]);
        component_size[root_v] += component_size[root_u];
        parent[root_u] = root_v;
        return component_sizes;
    }

}

namespace PartialSums
{

    std::array<Types::ValueType, Constants::N> partial_sums{};
    int n{};

    static Types::ValueType Query(const int &i, const int &j)
    {
        return partial_sums[j] - (i ? partial_sums[i - 1] : Types::ValueType{});
    }

    static void AddValue(const int &index, const Types::ValueType &value)
    {
        partial_sums[index] += value;
    }

    static void ComputeSums(const int &n)
    {
        PartialSums::n = n;
        for (int i = 1; i < n; i++)
        {
            partial_sums[i] += partial_sums[i - 1];
        }
    }

}

namespace Graphs
{

    template <typename NodeType, typename WeightType>
    class Edge
    {
    public:
        Edge() = default;

        WeightType weight{};
        NodeType u{}, v{};
    };

}

namespace Solution
{

    std::array<Graphs::Edge<int, Types::ValueType>, Constants::N> tree_edges;

    static void SolveProblem(std::istream &input, std::ostream &output)
    {
        int node_count{}, query_count{};
        input >> node_count >> query_count;
        const auto edge_count = node_count - 1;
        std::map<Types::ValueType, int> weight_index{};
        // Read the edges, add the weight as a key in the index map.
        for (int i = 0; i < edge_count; i++)
        {
            auto &edge{tree_edges[i]};
            input >> edge.u >> edge.v >> edge.weight;
            weight_index[edge.weight] = i;
            edge.u--, edge.v--;
        }
        std::sort(tree_edges.begin(), tree_edges.begin() + edge_count,
                  [](const auto &a, const auto &b)
                  {
                      return a.weight < b.weight;
                  });
        // Map the sorted weights to increasing indices.
        int weight_count{};
        for (auto &pair : weight_index)
        {
            pair.second = weight_count++;
        }
        DisjointSets::Initialize(node_count);
        for (int i = 0; i < edge_count; i++)
        {
            const auto &edge{tree_edges[i]};
            const auto &component_sizes{DisjointSets::Join(edge.u, edge.v)};
            // Add the number of paths that pass through the edge with this weight to the index of the weight.
            PartialSums::AddValue(weight_index[edge.weight], component_sizes.first * component_sizes.second);
        }
        PartialSums::ComputeSums(weight_count);
        for (int i = 0; i < query_count; i++)
        {
            if (node_count == 1) // There aren't any edges.
            {
                output << Types::ValueType{} << '\n';
                continue;
            }
            Types::ValueType weight_l{}, weight_r{};
            input >> weight_l >> weight_r;
            auto begin = weight_index.lower_bound(weight_l);
            auto end = weight_index.upper_bound(weight_r);
            // Check if there's any edge with a weight inside the range.
            if (begin == weight_index.end() || end == weight_index.begin())
            {
                output << Types::ValueType{} << '\n';
                continue;
            }
            output << PartialSums::Query(begin->second, (--end)->second) << '\n';
        }
    }

}

int main()
{
    IO::OptimizeStandardIO();
    Solution::SolveProblem(std::cin, std::cout);
    return 0;
}