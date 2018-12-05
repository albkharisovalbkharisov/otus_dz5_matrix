#include <iostream>
#include <unordered_map>
#include <forward_list>
#include <functional>
#include <typeinfo>
#include <type_traits>
#include <utility>
#include <assert.h>

template <typename T, T V>
class Matrix
{
    friend class miniMatrix;
    struct nodeHash;
    const T c = V;

    class node
    {
    public:
        friend struct nodeHash;
        size_t row;
        size_t col;
        node (const size_t &row, const size_t &col) : row(row), col(col) {}
        bool operator==(const node &other) const
        {
            return (this->row == other.row) && (this->col == other.col);
        }
    };

    using data_container = typename std::unordered_map<node, T, nodeHash>;

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

//    template <typename S1, typename S2, typename T1>
//    operator std::tuple<S1, S2, T1>(const typename data_container::value_type &pair)
//    {
//        auto n = pair.first;
//        return std::tuple<S1, S2, T1>(n.row, n.col, pair.second);
//    }

    data_container m;

public:
    class MatrixIt;
    auto operator[](const size_t &col)
    {
        miniMatrix o{this, col};
        return o;
    }

    MatrixIt begin(void)
    {
        std::cout << "begin()" << std::endl;
        return MatrixIt{m.begin()};
    }

    MatrixIt end(void)
    {
        std::cout << "end()" << std::endl;
        return MatrixIt{m.end()};
    }

    size_t size(void)
    {
        return m.size();
    }

///////////////////////////////////////////////////////
    struct MatrixIt
    {
        typename data_container::iterator it;
        MatrixIt(decltype(it) &&it) : it{it} {}

        MatrixIt& operator++()
        {
            std::cout << "It op++" << std::endl;
            ++it;
            return *this;
        }

        MatrixIt& operator*()
        {
            std::cout << "It op*" << std::endl;
//            size_t col, row;
//            std::cout << "before col/row = " << col << "/" << row << std::endl;
//            std::tie<col, row> = it->first;
//            std::cout << "after col/row = " << col << "/" << row << std::endl;
            return *this;
        }

        bool operator!=(const MatrixIt& other)
        {
            std::cout << "It op!=" << std::endl;
            return this->it != other.it;
        }

        template <typename S1, typename S2, typename T1>
        operator std::tuple<S1, S2, T1>()
        {
            std::cout << "It castTuple" << std::endl;
            // remove 'const' for passing to tuple only. It's safe.
//            return std::tuple<S1, S2, T1>(row, col, const_cast<T&>(gett()));
            S1 row = it->first.row;
            S2 col = it->first.col;
            return std::tuple<S1, S2, T1>(row, col, it->second);
//            return std::tuple<S1, S2, T1>(it->first.row, it->first.col, it->second);
        }
    };

///////////////////////////////////////////////////////////
    class miniMatrix
    {
    private:
        const T& gett(void) const
        {
            auto search = mat->m.find(node{row, col});
            if (search != mat->m.end()) {
                return search->second;
            }
            else {
                return mat->c;
            }
        }

        class Matrix *mat;
        size_t col;
        size_t row;
    public:
        miniMatrix(class Matrix *mat, size_t col) : mat(mat), col(col) {}

        bool operator==(const T& other)
        {
            return this->gett() == other;
        }

        auto & operator[](const size_t &row)
        {
            this->row = row;
            return *this;
        }

        miniMatrix & operator=(const T &t)
        {
            auto n = node{row, col};
            auto search = mat->m.find(n);

            if (search != mat->m.end()) {
                if (t == mat->c)
                    mat->m.erase(search);
                else
                    search->second = t;
            } else {
                if (t != mat->c)
                    mat->m.insert(std::make_pair(n, t));
            }
            return *this;
        }

        friend std::ostream& operator<< (std::ostream &os, miniMatrix &mm )
        {
            std::cout << "[" << mm.col << "][" << mm.row << "] = " << mm.gett();
            return os;
        }

//        template <typename S1, typename S2, typename T1>
//        operator std::tuple<S1, S2, T1>()
//        {
//            // remove 'const' for passing to tuple only. It's safe.
//            return std::tuple<S1, S2, T1>(row, col, const_cast<T&>(gett()));
//        }
    };
};



int main()
{
    Matrix<int, -1> matrix; // бесконечная матрица int заполнена значениями -1
    matrix[100][100] = 314;

    for(auto c: matrix)
    {
        size_t x = 1;
        size_t y = 2;
        int v = 3;
        std::tie(x, y, v) = c;
        std::cout << x << y << v << std::endl;
    }
//    return 0;
}


