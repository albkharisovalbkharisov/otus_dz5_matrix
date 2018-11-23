#include <stdio.h>
#include <unordered_map>

template <typename P, typename T, T V>
class matrix
{
    const T V;
    struct node
    {
        size_t row;
        size_t col;
        T val;
    };
    std::unordered_map<node, T> m;

    T& operator[](const T&)
    {
        return ;
    }
};

// custom specialization of std::hash can be injected in namespace std
namespace std
{
    template<typename P, typename T>
    template<> struct hash<S>
    {
        typedef S argument_type;
        typedef std::size_t result_type;
        result_type operator()(argument_type const& s) const noexcept
        {
            result_type const h1 ( std::hash<P>{}(s.first_name) );
            result_type const h2 ( std::hash<P>{}(s.last_name) );
            return h1 ^ (h2 << 1);
        }
    };
}
                                                                                          




int main()
{
    return 0;
}


