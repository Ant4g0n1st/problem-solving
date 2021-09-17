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
            PropagatePendingTask();
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

    template <typename IntegralType>
    class MaxInteger
    {
        static_assert(std::is_integral<IntegralType>());
        static_assert(std::is_signed<IntegralType>());

    public:
        MaxInteger() : v(std::numeric_limits<IntegralType>::min()) {}

        MaxInteger(const IntegralType &v) : v(v) {}

        MaxInteger operator+(const MaxInteger &other) const
        {
            return {std::max(v, other.v)};
        }

        operator IntegralType() const
        {
            return v;
        }

    private:
        IntegralType v{};
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

    using CoordinateType = int;

    static void SolveProblem(std::istream &input, std::ostream &output)
    {
        std::map<CoordinateType, int> ordinate_map{};
        int room_count{};
        input >> room_count;
        std::vector<Math::Rectangle<CoordinateType>> rooms(room_count);
        for (int i = 0; i < room_count; i++)
        {
            auto &sw{rooms[i].south_west_corner};
            auto &ne{rooms[i].north_east_corner};
            input >> sw.first >> sw.second;
            input >> ne.first >> sw.second;
            ordinate_map[sw.second] = i;
            ordinate_map[ne.second] = i;
        }
        int unique_ordinate_count{};
        for (auto &ordinate_index : ordinate_map)
        {
            ordinate_index.second = unique_ordinate_count++;
        }
        std::vector<RoomEvent<CoordinateType>> room_events{};
        for (int room_index = 0; room_index < room_count; room_index++)
        {
            auto &room{rooms[room_index]};
            room.north_east_corner.second = ordinate_map[room.north_east_corner.second];
            room.south_west_corner.second = ordinate_map[room.south_west_corner.second];
            room_events.emplace_back(room.north_east_corner.first, room_index, false);
            room_events.emplace_back(room.south_west_corner.first, room_index, true);
        }
        std::sort(room_events.begin(), room_events.end(),
                  [](const RoomEvent<CoordinateType> &a, const RoomEvent<CoordinateType> &b)
                  {
                      return a.GetTime() < b.GetTime();
                  });
        std::unique_ptr<DataStructures::SegmentTree<Math::MaxInteger<int>>> indices{};
        std::vector<Math::MaxInteger<int>> room_overwritten(room_count);
        const int room_event_count = room_events.size();
        for (int i = 0; i < room_event_count; i++)
        {
            const auto &event{room_events[i]};
            const auto &room_index{event.GetRoomIndex()};
            const auto &room{rooms[room_index]};
            if (event.IsBegin())
            {
                room_overwritten[room_index] = indices->Query(room.north_east_corner.second);
            }
        }
    }

}

int main()
{
    //IO::OptimizeStandardIO();
    Solution::SolveProblem(std::cin, std::cout);
    return 0;
}