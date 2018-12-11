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
    struct nodeHash;
    const T c = V;

    struct node
    {
        size_t row;
        size_t col;

        node (const size_t &row, const size_t &col) : row(row), col(col) {}

        bool operator==(const node &other) const
        {
            return (this->row == other.row) && (this->col == other.col);
        }
    };

    struct nodeHash
    {
        size_t operator()(const node &s) const noexcept
        {
            size_t h1 = std::hash<size_t>{}(s.row);
            size_t h2 = std::hash<size_t>{}(s.col);
            return h1 ^ (h2 << 1);
        }
    };

    using data_container = typename std::unordered_map<node, T, nodeHash>;

    data_container m;

    // Support class to work with Range-Based for loop.
    // Some kind of iterator wrapper. Developed for tuple conversion only.
    class MatrixIt
    {
        typename data_container::iterator it;

    public:
        MatrixIt(decltype(it) &&it) : it{it} {}
        MatrixIt& operator++() {
            ++it;
            return *this;
        }

        // does nothing, but it's necessery for RB-for-loop
        MatrixIt& operator*() { return *this; }

        bool operator!=(const MatrixIt& other) {
            return this->it != other.it;
        }

        template <typename S1, typename S2, typename T1>
        operator std::tuple<S1, S2, T1>()
        {
            // remove 'const' for passing to tuple only. It's safe.
            return std::tuple<S1, S2, T1>(const_cast<S1>(it->first.row), const_cast<S1>(it->first.col), it->second);
        }
    };

    // Support class to work with [][] operator
    class MatrixValue
    {
        class Matrix *mat;
        size_t row;
        size_t col;

        const T& valueGet(void) const
        {
            auto search = mat->m.find(node{row, col});
            if (search != mat->m.end()) {
                return search->second;
            }
            else {
                return mat->c;
            }
        }

    public:
        MatrixValue(class Matrix *mat, size_t row) : mat(mat), row(row) {}

        bool operator==(const T& other)
        {
            return this->valueGet() == other;
        }

        auto & operator[](const size_t &col)
        {
            this->col = col;
            return *this;
        }

        MatrixValue & operator=(const T &t)
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

        friend std::ostream& operator<< (std::ostream &os, MatrixValue &mm )
        {
//            std::cout << "[" << mm.col << "][" << mm.row << "] = " << mm.valueGet();    // for tests only
            std::cout << mm.valueGet();
            return os;
        }
    };

public:
    auto operator[](const size_t &col)
    {
        MatrixValue o{this, col};
        return o;
    }

    MatrixIt begin(void)
    {
        return MatrixIt{m.begin()};
    }

    MatrixIt end(void)
    {
        return MatrixIt{m.end()};
    }

    size_t size(void)
    {
        return m.size();
    }

    void print(size_t colS, size_t rowS, size_t colE, size_t rowE)
    {
        for (size_t i = colS; i <= colE; ++i)
        {
            for(size_t j = rowS; j <= rowE; ++j)
            {
                std::cout << (*this)[i][j] << " ";
            }
            std::cout << std::endl;
        }
    }
};



int main()
{
    Matrix<int, 0> matrix;

    for (size_t i = 0; i <= 9; ++i)
    {
        matrix[i][i] = i;
        matrix[i][9-i] = i;
    }

    matrix.print(1,1,8,8);
    std::cout << matrix.size() << std::endl;
    for(auto c: matrix)
    {
        size_t x;
        size_t y;
        int v;
        std::tie(x, y, v) = c;
        std::cout << x << y << v << std::endl;
    }

    ((matrix[100][100] = 314) = 0) = 217;                     // supports canonical form of operator=
    return 0;
}

