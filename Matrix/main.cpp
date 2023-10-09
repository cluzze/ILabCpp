#include "matrix.hpp"

int main() {
    size_t size;
    std::cin >> size;
    LinAl::Matrix<long long> m(size, size);
    
    for (size_t i = 0; i < size; ++i) {
        for (size_t j = 0; j < size; j++) {
            std::cin >> m[i][j];
        }
    }

    //m.print();
    std::cout << m.determinant() << '\n';
}