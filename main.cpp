#include <iostream>

/* Metaprogramming list of values. */
template <int ... values> struct MetaList;
template <int H, int ... T>
struct MetaList<H, T...>{
    static constexpr int Head = H;
    using Tail = MetaList<T...>;
};
template <int value>
struct MetaList<value> {
    static constexpr int Head = value;
};


/* Iterates over list and counts it's length. */
template <typename TL>
struct MetaListLength {
    static constexpr int length = 1 + MetaListLength<typename TL::Tail>::length;
};
template <int lastValue>
struct MetaListLength<MetaList<lastValue>>{ static constexpr int length = 1; };


/* Combines all list values into one binary representation ( */
template <typename TL>
struct BinaryBackward {
    static int const binary = (BinaryBackward<typename TL::Tail>::binary << 1) | TL::Head;
};
template <int lastBit>
struct BinaryBackward<MetaList<lastBit>> { static const int binary = lastBit; };

/* Prints list entities one by one. */
template <typename TL>
struct Print {
    static void show() { std::cout << TL::Head << ' '; Print<typename TL::Tail>::show(); }
};
template <int lastValue>
struct Print<MetaList<lastValue>> { static void show() { std::cout << lastValue << std::endl;; } };


/* Adds one element in existing list. */
template <int H, typename L> struct AddElement;
template <int H, int ... Values>
struct AddElement<H, MetaList<Values...>> { using type = MetaList<H, Values...>; };


/* Generates list of values from 0 to N-1. */
template <typename L, int H> struct PushBack;
template <int ... Values, int H>
struct PushBack<MetaList<Values...>, H> { using type = MetaList<Values..., H>; };
template <int N>
struct Generate {
    using type = typename PushBack<typename Generate<N - 1>::type, N - 1>::type;
};
template <> struct Generate <0> { using type = MetaList<>; };


/* Combines elements of two lists of equall sizes. */
template <typename L1, typename L2, template <int...> class Func>
struct Zip {
    using combinedList = typename AddElement<
            Func<L1::Head, L2::Head>::value,
            typename Zip<typename L1::Tail, typename L2::Tail, Func>::combinedList>::type;
};
template <int lastL, int lastR, template <int...> class Func>
struct Zip <MetaList<lastL>, MetaList<lastR>, Func> { using combinedList = MetaList<Func<lastL, lastR>::value>;};
template<int a, int b>
struct MetaPlus { static int const value = a + b; };


int main() {
    using valuesList = MetaList<10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20>;
    std::cout << "List head: " << valuesList::Head << ", list tail: "; Print<valuesList::Tail>::show(); // 11, 12, 13, 14, 15, 16, 17, 18, 19, 20


    std::cout << "List length: " << MetaListLength<valuesList>::length << std::endl; // 10


    using binaryList = MetaList<0, 1, 0, 1, 0, 1, 0, 1>;
    std::cout << "To binary: " << std::bitset<8>(BinaryBackward<binaryList>::binary) << std::endl;


    using extendedList = AddElement<9, valuesList>::type;
    std::cout << "Extended list: "; Print<extendedList>::show();


    using generatedList = Generate<11>::type;
    std::cout << "Generated list: "; Print<generatedList>::show();


    using listOne = MetaList<1, 2, 3, 4, 5>;
    using listTwo = MetaList<10, 11, 12, 13, 14>;
    using combinedList = Zip<listOne, listTwo, MetaPlus>::combinedList;
    std::cout << "Combined list: "; Print<combinedList>::show();
}


