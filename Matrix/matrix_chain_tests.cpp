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
    containers::vector<Matrix<long long>> m;
    m.push_back({5, 1, {1, 2, 1, 1, 1}});
    m.push_back({1, 5, {4, 5, 1, 2, 3}});
    m.push_back({5, 1, {1, 2, 3, 4, 5}});
    MatrixChain<long long> chain;
    chain.addMatrix(m[0]);
    chain.addMatrix(m[1]);
    chain.addMatrix(m[2]);
    auto order = chain.getOptimalOrder();
    MatrixChainComputer<long long> chainComp(m.begin(), m.end(), order.begin(), order.end());
    Matrix<long long> m1 = chainComp.computeChainOptimal(), m2 = chainComp.computeChainDefault();
    ASSERT_TRUE(m1.equals(m2));
}

TEST(chain, mult2) {
    containers::vector<Matrix<long long>> v;
    v.push_back({1, 5, {2, 1, 4, 7, 1}});
    v.push_back({5, 1, {1, 2, 3, 4, 5}});
    v.push_back({1, 3, {5, 6, 7}});
    v.push_back({3, 5, {2, 1, 4, 7, 1, 2, 1, 4, 7, 1, 2, 1, 4, 7, 1}});
    v.push_back({5, 2, {9, 8, 7, 6, 5, 4, 3, 2, 1, 1}});
    v.push_back({2, 1, {1, 2}});
    v.push_back({1, 3, {3, 2, 1}});
    v.push_back({3, 5, {5, 4, 3, 2, 1, 1, 2, 3, 4, 5, 5, 4, 3, 2, 1}});
    v.push_back({5, 1, {1, 1, 1, 1, 1}});
    v.push_back({1, 3, {2, 2, 2}});
    v.push_back({3, 4, {1, 1, 1, 2, 2, 2, 3, 3, 3, 4, 4, 4}});
    v.push_back({4, 3, {4, 4, 4, 3, 3, 3, 2, 2, 2, 1, 1, 1}});
    MatrixChain<long long> chain1;
    for (auto& x : v)
        chain1.addMatrix(x);

    auto order = chain1.getOptimalOrder();
    MatrixChainComputer<long long> chainComp1(v.begin(), v.end(), order.begin(), order.end());
    Matrix<long long> m1 = chainComp1.computeChainOptimal(), m2 = chainComp1.computeChainDefault();
    ASSERT_TRUE(m1.equals(m2));

    containers::vector<Matrix<long long>> v2;
    v2.push_back({1, 5, {2, 1, 4, 7, 1}});
    v2.push_back({5, 1, {1, 2, 3, 4, 5}});
    v2.push_back({1, 3, {5, 6, 7}});
    v2.push_back({3, 5, {2, 1, 4, 7, 1, 2, 1, 4, 7, 1, 2, 1, 4, 7, 1}});
    v2.push_back({5, 2, {9, 8, 7, 6, 5, 4, 3, 2, 1, 1}});
    v2.push_back({2, 1, {1, 2}});
    v2.push_back({1, 3, {3, 2, 1}});
    MatrixChain<long long> chain2;
    for (auto& x : v2)
        chain2.addMatrix(x);
    auto order2 = chain2.getOptimalOrder();
    // chain2.printDp();
    // chain2.printP();
    containers::vector<Matrix<long long>> v3;
    v3.push_back({3, 5, {5, 4, 3, 2, 1, 1, 2, 3, 4, 5, 5, 4, 3, 2, 1}});
    v3.push_back({5, 1, {1, 1, 1, 1, 1}});
    v3.push_back({1, 3, {2, 2, 2}});
    v3.push_back({3, 4, {1, 1, 1, 2, 2, 2, 3, 3, 3, 4, 4, 4}});
    v3.push_back({4, 3, {4, 4, 4, 3, 3, 3, 2, 2, 2, 1, 1, 1}});
    for (auto& x : v3)
        chain2.addMatrix(x);
    order2 = chain2.getOptimalOrder();
    // chain2.printDp();
    // chain2.printP();
    auto n2 = chain2.getOptimalNumberOfMultiplications();
    auto n1 = chain1.getOptimalNumberOfMultiplications();
    ASSERT_EQ(n1, n2);
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
    auto order = chain.getOptimalOrder();
    MatrixChainComputer<long long> chainComp(v.begin(), v.end(), order.begin(), order.end());
    Matrix<long long> m1 = chainComp.computeChainOptimal(), m2 = chainComp.computeChainDefault();
    //std::cout << "Optimal: " << chain.getOptimalNumberOfMultiplications() << "\nDefault: " << chain.getNormalNumberOfMultiplications() << '\n';
    ASSERT_TRUE(m1.equals(m2));
}

TEST(chain, mult_gen1) {
    MatrixChain<long long> chain;
    std::vector<Matrix<long long>> v;
    v.push_back({200, 5});
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
    auto order = chain.getOptimalOrder();
    MatrixChainComputer<long long> chainComp(v.begin(), v.end(), order.begin(), order.end());
    Matrix<long long> m1 = chainComp.computeChainOptimal(), m2 = chainComp.computeChainDefault();
    ASSERT_TRUE(m1.equals(m2));
}

long unsigned genInt() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<long unsigned> dist(5, 10);
    return dist(gen);
}

TEST(chain, gen1) {
    MatrixChain<long long> chain;
    std::vector<Matrix<long long>> v;
    long unsigned last = genInt();
    for (int i = 0; i < 20; i++) {
        v.push_back({last, genInt()});
        last = v[i].cols();
    }
    for (int i = 0; i < v.size(); i++) {
        fillMatrix(v[i]);
        chain.addMatrix(v[i]);
    }
    auto order = chain.getOptimalOrder();
    MatrixChainComputer<long long> chainComp(v.begin(), v.end(), order.begin(), order.end());
    Matrix<long long> m1 = chainComp.computeChainOptimal(), m2 = chainComp.computeChainDefault();
    ASSERT_TRUE(m1.equals(m2));
    //std::cout << "Optimal: " << chain.getOptimalNumberOfMultiplications() << "\nDefault: " << chain.getNormalNumberOfMultiplications() << '\n';
}

template<typename T>
void createIdentityMatrix(Matrix<T>& matrix) {
    auto sz = std::min(matrix.rows(), matrix.cols());
    for (int i = 0; i < sz; i++) {
        matrix[i][i] = 1;
    }
}

// TEST(chain, gen2) {
//     MatrixChain<long long> chain;
//     std::vector<Matrix<long long>> v;
//     long unsigned last = genInt();
//     for (int i = 0; i < 1000; i++) {
//         v.push_back({last, genInt()});
//         last = v[i].cols();
//     }
//     for (int i = 0; i < v.size(); i++) {
//         createIdentityMatrix(v[i]);
//         chain.addMatrix(v[i]);
//     }
//     auto order = chain.getOptimalOrder();
//     MatrixChainComputer<long long> chainComp(v.begin(), v.end(), order.begin(), order.end());
//     Matrix<long long> m1 = chainComp.computeChainOptimal();
//     Matrix<long long> exp(v[0].rows(), v[v.size() - 1].cols());
//     createIdentityMatrix(exp);
//     exp.print();
//     m1.print();


//     ASSERT_TRUE(m1.equals(exp));
// }