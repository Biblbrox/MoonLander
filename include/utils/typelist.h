#ifndef MOONLANDER_TYPELIST_H
#define MOONLANDER_TYPELIST_H

namespace utils
{
/**
    * TypeList declaration
    * @tparam Args
    */
    template<typename... Args>
    struct TypeList;

    template<typename H, typename... T>
    struct TypeList<H, T...>
    {
        using Head = H;
        using Tail = TypeList<T...>;
    };

    template<>
    struct TypeList<>
    {
    };

    template<typename TypeList>
    struct Length
    {
        static int const value = 1 + Length<typename TypeList::Tail>::value;
    };

    template<>
    struct Length<TypeList<>>
    {
        static int const value = 0;
    };

/**
 * Apply unary functor to each element in TypeList (TL)
 * Also apply binary functor to each pair of elements
 * from right to left which is result of unary functor.
 * Both unary and binary functor must return some value
 * They can't be void.
 * Final return value of this function is result of binary
 * function.
 * @tparam TL
 * @tparam UnFunctor
 * @tparam BinFunctor
 * @param unfunc
 * @param binfunc
 * @return
 */
    template<class TL, class UnFunctor, class BinFunctor>
    constexpr auto typeListReduce(UnFunctor &&unfunc, BinFunctor &&binfunc)
    {
        static_assert(Length<TL>::value >= 2,
                      "Length<TypeList<Args...>>::value >= 2");

        typename TL::Head val;
        auto res = unfunc(val);

        if constexpr (Length<TL>::value == 2) { // Base case
            typename TL::Tail::Head tmp;
            return binfunc(res, unfunc(tmp));
        } else { // Recursion
            return binfunc(res,
                           typeListReduce<typename TL::Tail>(unfunc, binfunc));
        }
    }
}

#endif //MOONLANDER_TYPELIST_H
