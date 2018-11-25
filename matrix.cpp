#include <iostream>
#include <unordered_map>
#include <forward_list>
#include <functional>       // dbg_
#include <typeinfo>
#include <type_traits>

#if 0
template <typename T, T V>
class Matrix
{
    const T c = V;
};

#else
template <typename T, T V>
class Matrix
{
    struct nodeHash;
    const T c = V;
    class node
    {
        size_t row;
        size_t col;
    public:
        node (const size_t &row, const size_t &col) : row(row), col(col) {}
        bool operator==(const node &other) const
        {
            return (this->row == other.row) && (this->col == other.col);
        }

        friend struct nodeHash;
    };
    using data_container = typename std::unordered_map<node, T, nodeHash>;

    // custom hash can be a standalone function object:
    struct nodeHash
    {
        size_t operator()(const node &s) const noexcept
        {
            size_t h1 = std::hash<size_t>{}(s.row);
            size_t h2 = std::hash<size_t>{}(s.col);
            return h1 ^ (h2 << 1); // or use boost::hash_combine (see Discussion)
        }
    };


    //////////////////// Hash, Key, Node and other staff //////////////////////

    data_container m;

    struct supClass
    {
        supClass(data_container *m, size_t col) : m(m), col(col) {}
        data_container *m;
        size_t col;
    public:
        auto & operator[](const size_t &row)
//        void operator[](const size_t &row)
        {
            std::cout << "operator2[" << row << "] (col = " << col << ")" << std::endl;
            node n{row, col};
//            std::cout << "n{size_t,size_t} = " << typeid(n).name() << std::endl;
//            std::cout << "m::key_type = " << typeid(*m).name() << std::endl;
            return (*m)[n];
        }
    };

public:
    supClass operator[](const size_t &col)
    {
        supClass o{&m, col};
        std::cout << "operator1[" << col << "]" << std::endl;
        return o;
    }

    size_t size(void)
    {
        return m.size();
    }
};
#endif

int main()
{
    Matrix<int, -1> matrix; // бесконечная матрица int заполнена значениями -1
    std::cout << "size = " << matrix.size() << std::endl;

//    matrix[0][0];
    matrix[0][0] = 15;
    std::cout << matrix[0][0] << std::endl;
    std::cout << "size = " << matrix.size() << std::endl;

    (matrix[0][0] = 16) = 13;
    std::cout << matrix[0][0] << std::endl;
    std::cout << "size = " << matrix.size() << std::endl;

    return 0;
}


