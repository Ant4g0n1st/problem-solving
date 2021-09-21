#include <algorithm>
#include <iostream>
#include <chrono>
#include <limits>
#include <memory>
#include <random>
#include <vector>
#include <map>

namespace IO
{

    static void OptimizeStandardIO()
    {
        std::ios_base::sync_with_stdio(false);
        std::cout.tie(nullptr);
        std::cin.tie(nullptr);
    }

}

namespace DataStructures
{

    // Segment Tree for Range Update, Point Query.
    // Operations:
    //  - Set an entire range to a given value.
    //  - Query the value in a given position.
    template <typename ValueType>
    class SegmentTree
    {
    public:
        SegmentTree(const int &begin, const int &end) : begin(begin), end(end) {}

        const ValueType &Build()
        {
            if (!IsLeaf())
            {
                const auto middle{RangeMidpoint()};
                left.reset(new SegmentTree(begin, middle));
                right.reset(new SegmentTree(middle + 1, end));
                value = left->Build() + right->Build();
            }
            return value;
        }

        ValueType Query(const int &x)
        {
            if (DisjointWith(x, x))
            {
                return ValueType{};
            }
            PropagatePendingTask();
            if (ContainedIn(x, x))
            {
                return value;
            }
            return left->Query(x) + right->Query(x);
        }

        const ValueType &Update(const int &l, const int &r, const ValueType &v)
        {
            PropagatePendingTask();
            if (DisjointWith(l, r))
            {
                return value;
            }
            if (ContainedIn(l, r))
            {
                SetPendingTask(v);
                return value;
            }
            return value = left->Update(l, r, v) + right->Update(l, r, v);
        }

    private:
        bool DisjointWith(const int &l, const int &r)
        {
            return end < l || r < begin;
        }

        bool ContainedIn(const int &l, const int &r)
        {
            return l <= begin && end <= r;
        }

        void PropagatePendingTask()
        {
            if (HasPendingTask() && HasChildren())
            {
                left->SetPendingTask(value);
                right->SetPendingTask(value);
                pending = false;
            }
        }

        void SetPendingTask(const ValueType &v)
        {
            pending = true;
            value = v;
        }

        int RangeMidpoint() const
        {
            return (begin + end) >> 1;
        }

        bool HasPendingTask() const
        {
            return pending;
        }

        bool IsLeaf() const
        {
            return begin == end;
        }

        bool HasChildren() const
        {
            return !IsLeaf();
        }

        std::unique_ptr<SegmentTree> left{}, right{};
        const int begin{}, end{};
        bool pending{false};
        ValueType value{};
    };

}

namespace Math
{

    // A class that wraps an integer and overloads the addition operator to compute the maximum of two values.
    template <typename IntegralType, IntegralType MinValue = std::numeric_limits<IntegralType>::min()>
    class MaxInteger
    {
        static_assert(std::is_integral<IntegralType>());
        static_assert(std::is_signed<IntegralType>());

    public:
        MaxInteger(const IntegralType &v) : v(v) {}

        MaxInteger() = default;

        MaxInteger operator+(const MaxInteger &other) const
        {
            return {std::max(v, other.v)};
        }

        operator IntegralType() const
        {
            return v;
        }

    private:
        IntegralType v{MinValue};
    };

    template <typename NumericType>
    class Rectangle
    {
        static_assert(std::is_arithmetic<NumericType>());

    public:
        Rectangle() = default;

        std::pair<NumericType, NumericType> south_west_corner{};
        std::pair<NumericType, NumericType> north_east_corner{};
    };

}

namespace Solution
{

    template <typename NumericType>
    class RoomEvent
    {
        static_assert(std::is_arithmetic<NumericType>());

    public:
        RoomEvent(const NumericType &time, const int &room_index, const bool &is_begin) : time(time), room_index(room_index), is_begin(is_begin) {}

        const NumericType &GetTime() const
        {
            return time;
        }

        const int &GetRoomIndex() const
        {
            return room_index;
        }

        const bool &IsBegin() const
        {
            return is_begin;
        }

    private:
        NumericType time{};
        int room_index{};
        bool is_begin{};
    };

    static void SolveProblem(std::istream &input, std::ostream &output)
    {
        // First, we read the room descriptions and add the ordinates to the map for compression.
        using CoordinateType = int;
        std::map<CoordinateType, int> ordinate_map{};
        int room_count{}; input >> room_count;
        std::vector<Math::Rectangle<CoordinateType>> rooms(room_count);
        for (int i = 0; i < room_count; i++)
        {
            auto &sw{rooms[i].south_west_corner};
            auto &ne{rooms[i].north_east_corner};
            input >> sw.first >> sw.second;
            input >> ne.first >> ne.second;
            ordinate_map[sw.second] = i;
            ordinate_map[ne.second] = i;
        }
        // Then we compress the ordinates.
        int unique_ordinate_count{};
        for (auto &ordinate_index : ordinate_map)
        {
            ordinate_index.second = unique_ordinate_count++;
        }
        // Next, split each room into a pair of events, one when the room first appears
        // and one when the room ends, in the x-axis.
        std::vector<RoomEvent<CoordinateType>> room_events{};
        for (int room_index = 0; room_index < room_count; room_index++)
        {
            auto &room{rooms[room_index]};
            auto &ne{room.north_east_corner};
            auto &sw{room.south_west_corner};
            // Compress the ordinates in each room for later.
            ne.second = ordinate_map[ne.second];
            sw.second = ordinate_map[sw.second];
            room_events.emplace_back(ne.first, room_index, false);
            room_events.emplace_back(sw.first, room_index, true);
        }
        std::sort(room_events.begin(), room_events.end(),
                  [](const RoomEvent<CoordinateType> &a, const RoomEvent<CoordinateType> &b)
                  {
                      return a.GetTime() < b.GetTime();
                  });
        using MaxInt = Math::MaxInteger<int, -1>;
        // We use a Segment Tree to detect which room contains another.
        std::unique_ptr<DataStructures::SegmentTree<MaxInt>> indices{};
        std::vector<MaxInt> room_overwritten(room_count);
        std::vector<MaxInt> exits_into(room_count);
        indices.reset(new DataStructures::SegmentTree<MaxInt>{0, unique_ordinate_count});
        indices->Build();
        // Process every room event in order.
        for (const auto &event : room_events)
        {
            const auto &room_index{event.GetRoomIndex()};
            const auto &room{rooms[room_index]};
            const auto &ne{room.north_east_corner};
            const auto &sw{room.south_west_corner};
            if (event.IsBegin())
            {
                // Check if we will obscure any room that is already open.
                room_overwritten[room_index] = indices->Query(ne.second);
                if (room_overwritten[room_index] >= 0)
                {
                    // If so, there's a room that started before, is still open and covers the height of this one,
                    // as a consequence, because there's no room overlapping, that room contains this one.
                    exits_into[room_index] = room_overwritten[room_index];
                }
                // We flag the area for any rooms contained inside this one.
                indices->Update(sw.second, ne.second, room_index);
            }
            else
            {
                // We clear our flag and restore the previous one as we exit this room.
                indices->Update(sw.second, ne.second, room_overwritten[room_index]);
            }
        }
        // Print the results.
        for (int i = 0; i < room_count; i++)
        {
            output << exits_into[i] << '\n';
        }
    }

}

int main()
{
    IO::OptimizeStandardIO();
    Solution::SolveProblem(std::cin, std::cout);
    return 0;
}