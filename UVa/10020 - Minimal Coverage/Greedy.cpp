#include <algorithm>
#include <iostream>
#include <vector>
#include <deque>

namespace DataStructures
{

    template <typename T>
    class Segment
    {
    public:
        Segment() = default;
        Segment(const T &begin, const T &end) : begin(begin), end(end) {}

        void SetBegin(const T &begin)
        {
            this->begin = begin;
        }

        void SetEnd(const T &end)
        {
            this->end = end;
        }

        const T &GetBegin() const
        {
            return this->begin;
        }

        const T &GetEnd() const
        {
            return this->end;
        }

    private:
        T begin{};
        T end{};
    };

}

namespace IO::Configuration
{

    static void OptimizeStandardIO()
    {
        std::ios_base::sync_with_stdio(false);
        std::cout.tie(nullptr);
        std::cin.tie(nullptr);
    }

}

namespace Algorithms
{
    template<typename T>
    static T MinimalCoverage(const std::vector<DataStructures::Segment<T>>& segments, const DataStructures::Segment<T>& space){

    }
} 


namespace Solution
{
    using EndpointType = int;

    static void SolveInstance(std::istream &input, std::ostream &output)
    {
        std::vector<DataStructures::Segment<EndpointType>> segments{};
        EndpointType space_size{}; input >> space_size;
        DataStructures::Segment<EndpointType> space{0, space_size};
        EndpointType begin{}, end{};
        while (input >> begin >> end && begin && end)
        {
            if (space_begin > end || space_end < begin)
            {
                continue;
            }
            segments.emplace_back(begin, end);
        }
    }

    static void SolveProblem(std::istream &input, std::ostream &output)
    {
        int cases{};
        input >> cases;
        for (int i = 0; i < cases; i++)
        {
            if (i > 0)
            {
                output << '\n';
            }
            SolveInstance(input, output);
        }
    }

}

int main()
{
    IO::Configuration::OptimizeStandardIO();
    Solution::SolveProblem(std::cin, std::cout);
    return 0;
}