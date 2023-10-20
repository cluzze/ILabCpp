#include <gtest/gtest.h>

#include "matrix_chain.hpp"

#include <random>

using namespace LinAl;

TEST(chain, add) {
    MatrixChain<int> chain;
    chain.addMatrix({5, 1, {1, 2, 1, 1, 1}});
    chain.addMatrix({1, 5, {4, 5, 1, 2, 3}});
    chain.addMatrix({5, 1, {1, 2, 3, 4, 5}});
}

TEST(chain, add1) {
    MatrixChain<int> chain;
    chain.addMatrix({10, 5});
    chain.addMatrix({5, 60});
    chain.addMatrix({60, 10});
    //chain.printOptimalOrder();
}

TEST(chain, add2) {
    MatrixChain<int> chain;
    chain.addMatrix({10, 5});
    chain.addMatrix({5, 15});
    chain.addMatrix({15, 35});
    chain.addMatrix({35, 30});
    //chain.printOptimalOrder();
}

TEST(chain, add3) {
    MatrixChain<int> chain;
    chain.addMatrix({10, 5});
    chain.addMatrix({5, 15});
    chain.addMatrix({15, 35});
    chain.addMatrix({35, 5});
    chain.addMatrix({5, 60});
    chain.addMatrix({60, 10});
    chain.addMatrix({10, 30});
    chain.addMatrix({30, 5});
    chain.addMatrix({5, 15});
    chain.addMatrix({15, 35});
    chain.addMatrix({35, 100});
    chain.addMatrix({100, 30});
    //chain.printOptimalOrder();
}

TEST(chain, mult1) {
    MatrixChain<int> chain;
    chain.addMatrix({5, 1, {1, 2, 1, 1, 1}});
    chain.addMatrix({1, 5, {4, 5, 1, 2, 3}});
    chain.addMatrix({5, 1, {1, 2, 3, 4, 5}});
    Matrix<int> m1 = chain.computeChainOptimal(), m2 = chain.computeChainDefault();
    ASSERT_TRUE(m1.equals(m2));
}

TEST(chain, mult2) {
    MatrixChain<long long> chain;
    chain.addMatrix({1, 5, {2, 1, 4, 7, 1}});
    chain.addMatrix({5, 1, {1, 2, 3, 4, 5}});
    chain.addMatrix({1, 3, {5, 6, 7}});
    chain.addMatrix({3, 5, {2, 1, 4, 7, 1, 2, 1, 4, 7, 1, 2, 1, 4, 7, 1}});
    chain.addMatrix({5, 2, {9, 8, 7, 6, 5, 4, 3, 2, 1, 1}});
    chain.addMatrix({2, 1, {1, 2}});
    chain.addMatrix({1, 3, {3, 2, 1}});
    chain.addMatrix({3, 5, {5, 4, 3, 2, 1, 1, 2, 3, 4, 5, 5, 4, 3, 2, 1}});
    chain.addMatrix({5, 1, {1, 1, 1, 1, 1}});
    chain.addMatrix({1, 3, {2, 2, 2}});
    chain.addMatrix({3, 4, {1, 1, 1, 2, 2, 2, 3, 3, 3, 4, 4, 4}});
    chain.addMatrix({4, 3, {4, 4, 4, 3, 3, 3, 2, 2, 2, 1, 1, 1}});
    //chain.printOptimalOrder();
    Matrix<long long> m1 = chain.computeChainOptimal(), m2 = chain.computeChainDefault();
    // m1.print();
    // m2.print();
    ASSERT_TRUE(m1.equals(m2));
}

template<typename T>
void fillMatrix(Matrix<T>& matrix) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> dist(-2, 2);
    for (int i = 0; i < matrix.rows(); i++) {
        for (int j = 0; j < matrix.cols(); j++) {
            matrix[i][j] = dist(gen);
        }
    }
}

TEST(chain, mult_gen) {
    MatrixChain<long long> chain;
    std::vector<Matrix<long long>> v;
    v.push_back({500, 5});
    v.push_back({5, 1000});
    v.push_back({1000, 350});
    v.push_back({350, 50});
    v.push_back({50, 600});
    v.push_back({600, 100});
    v.push_back({100, 300});
    v.push_back({300, 50});
    v.push_back({50, 105});
    v.push_back({105, 350});
    v.push_back({350, 100});
    v.push_back({100, 1});
    for (int i = 0; i < v.size(); i++) {
        fillMatrix(v[i]);
        chain.addMatrix(v[i]);
    }
    //chain.printOptimalOrder();
    Matrix<long long> m1 = chain.computeChainOptimal();//, m2 = chain.computeChainDefault();
    std::cout << "Optimal: " << chain.getOptimalNumberOfMultiplications() << "\nDefault: " << chain.getNormalNumberOfMultiplications() << '\n';
    //m1.print();
    //ASSERT_TRUE(m1.equals(m2));
}

TEST(chain, mult_gen1) {
    MatrixChain<long long> chain;
    std::vector<Matrix<long long>> v;
    v.push_back({500, 5});
    v.push_back({5, 1000});
    v.push_back({1000, 350});
    v.push_back({350, 50});
    v.push_back({50, 600});
    v.push_back({600, 100});
    v.push_back({100, 300});
    v.push_back({300, 50});
    v.push_back({50, 105});
    v.push_back({105, 350});
    v.push_back({350, 100});
    v.push_back({100, 1});
    for (int i = 0; i < v.size(); i++) {
        fillMatrix(v[i]);
        chain.addMatrix(v[i]);
    }
    //chain.printOptimalOrder();
    Matrix<long long> m1 = chain.computeChainOptimal(), m2 = chain.computeChainDefault();
    //std::cout << chain.getOptimalNumberOfMultiplications() << '\n' << chain.getNormalNumberOfMultiplications() << '\n';
    //m1.print();
    ASSERT_TRUE(m1.equals(m2));
}