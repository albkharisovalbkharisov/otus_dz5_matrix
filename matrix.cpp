#include <iostream>
#include <unordered_map>
//#include <map>
#include <forward_list>
#include <functional>       // dbg_
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
        size_t row;
        size_t col;
        node (const size_t &row, const size_t &col) : row(row), col(col) {}
        bool operator==(const node &other) const
        {
            return (this->row == other.row) && (this->col == other.col);
        }

        friend struct nodeHash;
        bool operator < (const node &n)
        {
            return n.row < this->row;
        }
    };
//    using data_container = typename std::map<node, T>;

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
        miniMatrix(class Matrix *mat, size_t col) : mat(mat), col(col) {}
        class Matrix *mat;
        size_t col;
        size_t row;
        auto & operator[](const size_t &row)
        {
//            std::cout << "operator2[" << col << "][" << row << "]" << std::endl;
            this->row = row;
            return *this;
        }
        miniMatrix & operator=(const T &t)
        {
//            std::cout << "operatorT=" << t << std::endl;
            auto n = node{row, col};
            auto search = mat->m.find(n);

//            std::cout << "==================================" << std::endl;
//            for (auto a : mat->m)
//                std::cout << a.second << std::endl;
//            std::cout << "==================================" << std::endl;

            if (search != mat->m.end()) {
//                std::cout << "Found " /*<< search->first */<< " " << search->second << '\n';
                if (t == mat->c)
                    mat->m.erase(search);
                else
                    search->second = t;
            } else {
                if (t != mat->c)
                    mat->m.insert(std::make_pair(n, t));
//                std::make_pair<std::string,double>("eggs",6.0)
//                std::cout << "Not found\n";
            }
            return *this;
        }
        const T& gett(void) const
        {
            auto search = mat->m.find(node{row, col});
//            std::cout << "node: " << row << "," << col;
            if (search != mat->m.end()) {
//                std::cout << " : " << search->second << std::endl;
                return search->second;
            }
            else {
//                std::cout << " : not found " << std::endl;
                return mat->c;
            }
        }

        friend std::ostream& operator<< (std::ostream &os, const miniMatrix &mm )
        {
            //std::cout << mm.gett();
            std::cout << "[" << mm.col << "][" << mm.row << "] = " << mm.gett();
            return os;
        }
    };
};



int main()
{
#if 0
    Matrix<int, -1> matrix; // бесконечная матрица int заполнена значениями -1
    std::cout << "size = " << matrix.size() << std::endl;

    std::cout << matrix[0][0] << std::endl;
    matrix[0][0] = 15;
    std::cout << matrix[0][0] << std::endl;
    matrix[0][0] = 16;
    std::cout << matrix[0][0] << std::endl;
    matrix[0][0] = 19;
    std::cout << matrix[0][0] << std::endl;
    matrix[0][1] = 16;
    std::cout << matrix[0][1] << std::endl;
    matrix[0][1] = 17;
    std::cout << "size = " << matrix.size() << std::endl;
    matrix[0][0] = -1;
    std::cout << matrix[0][1] << std::endl;
    std::cout << "size = " << matrix.size() << std::endl;
    ((matrix[0][1] = -1) = 15) = -1;
    std::cout << matrix[0][1] << std::endl;
    std::cout << "size = " << matrix.size() << std::endl;
//    std::cout << matrix[0][0] << std::endl;
//    std::cout << "size = " << matrix.size() << std::endl;

//    (matrix[0][0] = 16) = 13;
//    (matrix[0][0] = 16) = -1;
//    std::cout << matrix[0][1] << std::endl;
//    std::cout << matrix[0][0] << std::endl;
//    std::cout << "size = " << matrix.size() << std::endl;
#else
    Matrix<int, -1> matrix; // бесконечная матрица int заполнена значениями -1
//    assert(matrix.size() == 0); // все ячейки свободны
//    auto a = matrix[0][0];
    std::cout << matrix[0][0] << std::endl;
//    assert(a == -1);
//    assert(matrix.size() == 0);
    matrix[100][100] = 314;
    std::cout << matrix[100][100] << std::endl;
//    assert(matrix[100][100] == 314);
//    assert(matrix.size() == 1);

    // 100100314
//    for(auto c: matrix)
//    {
//        int x;
//        int y;
//        int v;
//        std::tie(x, y, v) = c;
//        std::cout << x << y << v << std::endl;
//    }


#endif
    return 0;
}


