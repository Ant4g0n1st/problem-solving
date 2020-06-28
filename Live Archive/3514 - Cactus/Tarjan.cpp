#include <type_traits>
#include <exception>
#include <iostream>
#include <string>
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

namespace Math
{

    template <typename T = Int, T Base = static_cast<T>(Constants::BASE10)>
    class BigInteger
    {
        static_assert(std::is_integral<T>::value);

        std::vector<T> digits{};

    public:
        BigInteger(T = T{}) noexcept;

        BigInteger &operator*=(const T &) noexcept;

        const std::string ToString() noexcept;
    };

    template <typename T>
    const std::string MultiplySequence(const std::vector<T> &) noexcept;

}; // namespace Math

namespace Exceptions
{

    class NotACactus : virtual public std::exception
    {
        using std::exception::exception;

    public:
        virtual const char *what() const throw() override;
    };

}; // namespace Exceptions

namespace Solution
{

    void SolveProblem()
    {
        std::vector<Int> meow(223, 224);
        std::cout << Math::MultiplySequence(meow) << std::endl;
    }

}; // namespace Solution

int main()
{
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
    const std::string BigInteger<T, Base>::ToString() noexcept
    {
        std::string output{};
        for (auto it{digits.rbegin()}; it != digits.rend(); it++)
        {
            output.push_back('0' + static_cast<char>(*it));
        }
        return output;
    }

    /* MultiplySequence Implementation. */
    template <typename T>
    const std::string MultiplySequence(const std::vector<T> &sequence) noexcept
    {
        static_assert(std::is_integral<T>::value);
        BigInteger<T> result{1};
        for (const auto &element : sequence)
        {
            result *= element;
        }
        return result.ToString();
    }

}; // namespace Math