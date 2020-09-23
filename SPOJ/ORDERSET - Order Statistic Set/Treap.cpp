#include <type_traits>
#include <iostream>
#include <chrono>
#include <memory>
#include <random>

namespace Types
{

    using Long = long long int;
    using Int = unsigned int;

}; // namespace Types

using namespace Types;
namespace Random
{

    template <typename T>
    T GetRandom()
    {
        const static Int seed = std::chrono::system_clock::now().time_since_epoch().count();
        static std::uniform_int_distribution<T> distribution{};
        static std::default_random_engine generator{seed};
        static auto random{std::bind(distribution, generator)};
        return random();
    }

}; // namespace Random

namespace DataStructures
{

    template <typename K, typename V>
    struct Treap
    {
        static_assert(std::is_integral<K>::value);
        static_assert(std::is_integral<V>::value);

    private:
        struct Node
        {
            using pNode = std::shared_ptr<Node>;

            Node(const K &, const V &) noexcept;

            void SetValue(const V &) noexcept;

            static void Update(pNode) noexcept;

            static V Total(pNode) noexcept;

            Long priority{};
            pNode l{};
            pNode r{};
            V total{};
            V value{};
            K key{};
        };

        using pNode = std::shared_ptr<Node>;

    private:
        static void Split(pNode, const K &, pNode &, pNode &) noexcept;

        static bool SetKeyValue(pNode, const K &, const V &) noexcept;

        static bool FindByOrder(pNode, int, K &) noexcept;

        static V OrderOfKey(pNode, const K &) noexcept;

        static void Insert(pNode &, pNode) noexcept;

    public:
        bool FindByOrder(const int &, K &) noexcept;

        V OrderOfKey(const K &) noexcept;

        void Delete(const K &) noexcept;

        void Insert(const K &) noexcept;

    private:
        pNode root{};
    };

}; // namespace DataStructures

namespace Solution
{

    template <typename K, typename V>
    using Treap = DataStructures::Treap<K, V>;

    using Value = int;

    using Key = int;

    enum class Operation : char
    {
        DeleteElement = 'D',
        InsertElement = 'I',
        FindByOrder = 'K',
        OrderOfKey = 'C'
    };

    std::istream &operator>>(std::istream &inputStream, Operation &op)
    {
        char buffer{}; inputStream >> buffer;
        op = static_cast<Operation>(buffer);
        return inputStream;
    }

    void SolveProblem(std::istream &input, std::ostream &output)
    {
        std::unique_ptr<Treap<Key, Value>> treap{};
        treap.reset(new Treap<Key, Value>{});
        Int n{}; input >> n;
        for (Int i = 0; i < n; ++i)
        {
            Operation operation{};
            input >> operation;
            switch (operation)
            {
            case Operation::OrderOfKey:
            {
                Key key{}; input >> key;
                output << treap->OrderOfKey(key) << '\n';
            }
            break;
            case Operation::FindByOrder:
            {
                Key k{}, key{}; input >> k;
                if (!treap->FindByOrder(k, key))
                {
                    output << "invalid";
                }
                else
                {
                    output << key;
                }
                output << '\n';
            }
            break;
            case Operation::DeleteElement:
            {
                Key key{}; input >> key;
                treap->Delete(key);
            }
            break;
            case Operation::InsertElement:
            {
                Key key{}; input >> key;
                treap->Insert(key);
            }
            break;
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
    Solution::SolveProblem(std::cin, std::cout);
    return 0;
}

namespace DataStructures
{

    /* Treap Node Implementation. */
    template <typename K, typename V>
    Treap<K, V>::Node::Node(const K &key, const V &value) noexcept
        : key{key}, priority{Random::GetRandom<Long>()}, total{value}, value{value} {}

    template <typename K, typename V>
    void Treap<K, V>::Node::SetValue(const V &value) noexcept
    {
        this->value = value;
    }

    template <typename K, typename V>
    void Treap<K, V>::Node::Update(pNode node) noexcept
    {
        if (node)
        {
            auto &total{node->total};
            total = node->value;
            total += Total(node->l);
            total += Total(node->r);
        }
    }

    template <typename K, typename V>
    V Treap<K, V>::Node::Total(pNode node) noexcept
    {
        return node ? node->total : V{};
    }

    /* Treap Implementation. */
    template <typename K, typename V>
    bool Treap<K, V>::SetKeyValue(pNode node, const K &key, const V &value) noexcept
    {
        bool keyFound{false};
        if (node)
        {
            if (key < node->key)
            {
                keyFound = SetKeyValue(node->l, key, value);
            }
            else if (key > node->key)
            {
                keyFound = SetKeyValue(node->r, key, value);
            }
            else
            {
                node->SetValue(value);
                keyFound = true;
            }
        }
        Node::Update(node);
        return keyFound;
    }

    template <typename K, typename V>
    void Treap<K, V>::Split(pNode node, const K &key, pNode &l, pNode &r) noexcept
    {
        if (node)
        {
            if (key < node->key)
            {
                Split(node->l, key, l, node->l);
                r = node;
            }
            else
            {
                Split(node->r, key, node->r, r);
                l = node;
            }
        }
        else
        {
            l.reset(), r.reset();
        }
        Node::Update(node);
    }

    template <typename K, typename V>
    bool Treap<K, V>::FindByOrder(pNode node, int k, K &key) noexcept
    {
        if (node)
        {
            const auto &&total{Node::Total(node->l)};
            if (k <= total)
            {
                return FindByOrder(node->l, k, key);
            }
            k = k - total;
            if (k <= node->value)
            {
                key = node->key;
                return true;
            }
            k = k - node->value;
            return FindByOrder(node->r, k, key);
        }
        return false;
    }

    template <typename K, typename V>
    V Treap<K, V>::OrderOfKey(pNode node, const K &key) noexcept
    {
        if (node)
        {
            if (key > node->key)
            {
                auto total{Node::Total(node)};
                total -= Node::Total(node->r);
                return total + OrderOfKey(node->r, key);
            }
            else if (key < node->key)
            {
                return OrderOfKey(node->l, key);
            }
            else
            {
                return Node::Total(node->l);
            }
        }
        return V{};
    }

    template <typename K, typename V>
    void Treap<K, V>::Insert(pNode &node, pNode n) noexcept
    {
        if (node)
        {
            if (n->priority > node->priority)
            {
                Split(node, n->key, n->l, n->r);
                node = n;
            }
            else if (n->key < node->key)
            {
                Insert(node->l, n);
            }
            else
            {
                Insert(node->r, n);
            }
        }
        else
        {
            node = n;
        }
        Node::Update(node);
    }

    template <typename K, typename V>
    bool Treap<K, V>::FindByOrder(const int &k, K &key) noexcept
    {
        return Treap::FindByOrder(root, k, key);
    }

    template <typename K, typename V>
    void Treap<K, V>::Insert(const K &key) noexcept
    {
        if (!Treap::SetKeyValue(root, key, 1))
        {
            auto node{std::make_shared<Node>(key, 1)};
            Treap::Insert(root, node);
        }
    }

    template <typename K, typename V>
    V Treap<K, V>::OrderOfKey(const K &key) noexcept
    {
        return Treap::OrderOfKey(root, key);
    }

    template <typename K, typename V>
    void Treap<K, V>::Delete(const K &key) noexcept
    {
        Treap::SetKeyValue(root, key, 0);
    }

}; // namespace DataStructures