#include "matrix.hpp"
#include "matrix_chain.hpp"

int main() {
    int size;
    std::cin >> size;
    LinAl::MatrixChain<long long> chain;
    int rows = -1, cols = -1;
    std::cin >> rows;
    for (int i = 0; i < size; i++) {
        std::cin >> cols;
        LinAl::Matrix<long long> m(rows, cols);
        rows = cols;
        chain.addMatrix(m);
    }
    chain.getOptimalOrder();
    auto n = chain.getOptimalNumberOfMultiplications();
    std::cout << n;
}