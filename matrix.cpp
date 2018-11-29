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
    friend class miniMatrix;
    struct nodeHash;
    const T c = V;
    class node
    {
    public:
        size_t row;
        size_t col;
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

public:
    auto operator[](const size_t &col)
    {
        miniMatrix o{this, col};
//        std::cout << "operator1[" << col << "]" << std::endl;
        return o;
    }

    size_t size(void)
    {
        return m.size();
    }


    ///////////////////////////////////////////////////////////
    class miniMatrix
    {
    public:
        miniMatrix(class Matrix *m, size_t col) : m(m), col(col) {}
        class Matrix *m;
        size_t col;
        size_t row;
        auto & operator[](const size_t &row)
        {
    //        miniMatrixOut mmo{m, col, row};
//            std::cout << "operator2[" << col << "][" << row << "]" << std::endl;
            this->row = row;
            return *this;
        }
        miniMatrix & operator=(const T &t)
        {
            std::cout << "operatorT=" << t << std::endl;
#if 0
            if (t == m->c)
                std::cout << "same, ignore" << std::endl;
            else
                m->m[node{row, col}] = t;
            return *this;
#else
            auto search = m->m.find(node{row, col});
            if (search != m->m.end()) {
                std::cout << "Found " /*<< search->first */<< " " << search->second << '\n';
                if (t == m->c)
                    m->m.erase(node{row, col});
                else
                    search->second = t;
            } else {
                std::cout << "Not found\n";
            }
            return *this;
#endif
        }
        T& gett(void)
        {
            return m->m[node{row, col}];
        }

        friend std::ostream& operator<< (std::ostream &os, const miniMatrix &mm )
        {
            std::cout << gett();
            return os;
        }
    };
};
#endif

//std::ostream& operator<< (std::ostream &out, const miniMatrix &mm)
//{
//    std::cout << gett();
//}



int main()
{
    Matrix<int, -1> matrix; // бесконечная матрица int заполнена значениями -1
    std::cout << "size = " << matrix.size() << std::endl;

    matrix[0][0];
    matrix[0][0] = 15;
//    std::cout << matrix[0][0] << std::endl;
//    std::cout << "size = " << matrix.size() << std::endl;

    (matrix[0][0] = 16) = 13;
    (matrix[0][0] = 16) = -1;
//    std::cout << matrix[0][1] << std::endl;
//    std::cout << matrix[0][0] << std::endl;
//    std::cout << "size = " << matrix.size() << std::endl;

    return 0;
}


