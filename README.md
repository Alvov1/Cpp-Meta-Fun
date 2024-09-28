# Metaprogramming
Basics of C++ template metaprogramming. Generating lists of values during the compile-time

```cpp
template <int ... values>
struct MetaList;

template <int H, int ... T>
struct MetaList<H, T...>{
    static constexpr int Head = H;
    using Tail = MetaList<T...>;
};

template <int value>
struct MetaList<value> {
    static constexpr int Head = value;
};
```


Implemented list generation method and some supporting functions:
- Counting the lenght of the list
- Combining boolean values into binary representation of the list
- Printing list elements on the screen
- Adding elements to the existing list
- Generating sequenced lists
- Combining values of two separated lists

