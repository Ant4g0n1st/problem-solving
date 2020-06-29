#include <type_traits>
#include <exception>
#include <iostream>
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
        Math::BigInteger<Int> product{1};
        for (const auto &v : meow)
        {
            product *= v;
        }
        std::cout << product << std::endl;
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