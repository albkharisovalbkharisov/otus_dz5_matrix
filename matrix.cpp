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
    auto operator[](const size_t &col)
    {
        miniMatrix o{this, col};
        return o;
    }

    miniMatrix begin(void)
    {
        ;
            if (search != mat->m.end()) {
    }

    size_t size(void)
    {
        return m.size();
    }

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
        miniMatrix(class Matrix *mat, size_t col) : mat(mat), col(col), it(mat->m.begin()) {}
        miniMatrix(decltype(mat->m.begin()) it) {
            it(mat->m.begin());
        }
        decltype(mat->m.begin()) it;
        
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

        auto begin(void)
        {
            return miniMatrix{mat->m.begin()};
        }

        auto end(void)
        {
            return miniMatrix{mat->m.end()};
        }

        friend std::ostream& operator<< (std::ostream &os, miniMatrix &mm )
        {
            std::cout << "[" << mm.col << "][" << mm.row << "] = " << mm.gett();
            return os;
        }

        template <typename S1, typename S2, typename T1>
        operator std::tuple<S1, S2, T1>()
        {
            // remove 'const' for passing to tuple only. It's safe.
            return std::tuple<S1, S2, T1>(row, col, const_cast<T&>(gett()));
        }
    };
};

int main()
{
    Matrix<int, -1> matrix; // бесконечная матрица int заполнена значениями -1
    matrix[100][100] = 314;

//    for(auto c: matrix)
//    {
//        int x;
//        int y;
//        int v;
//        std::tie(x, y, v) = c;
//        std::cout << x << y << v << std::endl;
//    }
//    return 0;
}


