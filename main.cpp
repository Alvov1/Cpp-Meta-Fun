#include <iostream>
template <int ... values>
struct IntList;
template <int H, int ... T>
struct IntList<H, T...>{
    static const int Head = H;
    using Tail = IntList<T...>;
};
template <>
struct IntList<> {};
template <int H, typename L>
struct IntCons;
template <int H, int ... Values>
struct IntCons<H, IntList<Values...>> {
    using type = IntList<H, Values...>;
};
template <typename L, int H>
struct PushBack;
template <int ... Values, int H>
struct PushBack<IntList<Values...>, H> {
    using type = IntList<Values..., H>;
};
template <int Len>
struct Generate {
    using type = typename PushBack<typename Generate<Len - 1>::type, Len - 1>::type;
};
template <>
struct Generate <0> {
    using type = IntList<>;
};
template <typename L1, typename L2, template <int...> class Func>
struct Zip {
    using type = typename IntCons<
            Func<L1::Head, L2::Head>::value,
            typename Zip<typename L1::Tail, typename L2::Tail, Func>::type>::type;
};
template <template <int...> class Func>
struct Zip <IntList<>, IntList<>, Func> {
    using type = IntList<>;
};

template<int a, int b>
struct Plus
{
    static int const value = a + b;
};

template<int a, int b>
struct Minus
{
    static int const value = a - b;
};

template <typename L>
class Quantity;

template <int ... Ts>
class Quantity<IntList<Ts...>> {
public:
    Quantity() = default;
    explicit Quantity(double value) : value_(value){};

    double value() const {
        return value_;
    }

private:
    double value_;
};

template <int ... values>
Quantity<IntList<values...>> operator+(
        const Quantity<IntList<values...>>& left,
        const Quantity<IntList<values...>>& right){
    return Quantity<IntList<values...>>(left.value() + right.value());
}

template <int ... values>
Quantity<IntList<values...>> operator-(
        const Quantity<IntList<values...>>& left,
        const Quantity<IntList<values...>>& right){
    return Quantity<IntList<values...>>(left.value() - right.value());
}

template <int ... leftValues, int ... rightValues>
auto operator* (
        const Quantity<IntList<leftValues...>>& left,
        const Quantity<IntList<rightValues...>>& right)
        -> decltype(Quantity<typename Zip<IntList<leftValues...>, IntList<rightValues...>, Plus>::type>()){
    return Quantity<typename Zip<
            IntList<leftValues...>,
                    IntList<rightValues...>,
                            Plus>::type>(left.value() * right.value());
}

template <int ... leftValues, int ... rightValues>
auto operator/ (
        const Quantity<IntList<leftValues...>>& left,
        const Quantity<IntList<rightValues...>>& right)
        -> decltype(Quantity<typename Zip<IntList<leftValues...>, IntList<rightValues...>, Minus>::type>()){
    return Quantity<typename Zip<
            IntList<leftValues...>,
            IntList<rightValues...>,
            Minus>::type>(left.value() / right.value());
}
template <int ... values>
auto operator* (
        const Quantity<IntList<values...>>& elem, const double d)
        -> decltype(Quantity<IntList<values...>>()){
    return Quantity<IntList<values...>>(elem.value() * d);
}
template <int ... values>
auto operator/ (
        const Quantity<IntList<values...>>& elem, const double d)
        -> decltype(Quantity<IntList<values...>>()){
    return Quantity<IntList<values...>>(elem.value() / d);
}
template <int ... values>
auto operator* (const double d, const Quantity<IntList<values...>>& elem)
-> decltype(Quantity<typename Zip<IntList<0, 0, 0, 0, 0, 0, 0>, IntList<values...>, Plus>::type>()){
    return Quantity<typename Zip<
            IntList<0, 0, 0, 0, 0, 0, 0>,
                    IntList<values...>,
                            Plus>::type>(elem.value() * d);
}
template <int ... values>
auto operator/ (const double d, const Quantity<IntList<values...>>& elem)
-> decltype(Quantity<typename Zip<IntList<0, 0, 0, 0, 0, 0, 0>, IntList<values...>, Minus>::type>()){
    return Quantity<typename Zip<
            IntList<0, 0, 0, 0, 0, 0, 0>,
            IntList<values...>,
            Minus>::type>(elem.value() / d);
}

template<int m = 0, int kg = 0, int s = 0, int A = 0, int K = 0, int mol = 0, int cd = 0>
using Dimension = IntList<m, kg, s, A, K, mol, cd>;

using NumberQ   = Quantity<Dimension<>>;          // число без размерности
using LengthQ   = Quantity<Dimension<1>>;          // метры
using MassQ     = Quantity<Dimension<0, 1>>;       // килограммы
using TimeQ     = Quantity<Dimension<0, 0, 1>>;    // секунды
using VelocityQ = Quantity<Dimension<1, 0, -1>>;   // метры в секунду
using AccelQ    = Quantity<Dimension<1, 0, -2>>;   // ускорение, метры в секунду в квадрате
using ForceQ    = Quantity<Dimension<1, 1, -2>>;   // сила в ньютонах