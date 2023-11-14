#pragma once

#include "matrix.hpp"
#include "vector.hpp"

#include <unordered_map>
#include <cassert>
#include <vector>
#include <algorithm>
#include <stack>
#include <iostream>

namespace LinAl {
    template <typename T>
    class MatrixChain {
    public:
        using value_type = Matrix<T>::value_type;
        using size_type = long long;
    public:
        containers::vector<size_type> getOptimalOrder() {
            computeDp();
            computeOptimalOrder_impl(0, dp.size() - 1);
            return bestOrder;
        }

        size_type getOptimalNumberOfMultiplications() {
            if (dims.size() <= 2)
                return 0;
                
            computeDp();
            return dp[0][dp.size() - 1];
        }

        size_type getNormalNumberOfMultiplications() const {
            long long res = 0;
            
            if (dims.size() < 3)
                return 0;

            for (int i = 1; i < dims.size() - 1; i++) {
                res += dims[0] * dims[i] * dims[i + 1];
            }
            return res;
        }

        void addMatrix(const Matrix<T>& matrix) {
            if (!dims.empty() && dims[dims.size() - 1] != matrix.rows())
                throw std::runtime_error("m1 rows must be equal to m2 cols");
            
            bestOrder.resize(0);
            
            if (dims.empty()) {
                dims.push_back(matrix.rows());
            }
            dims.push_back(matrix.cols());
        }
        
    private:
        void computeDp() {
            auto sz = dims.size() - 1;
            dp.resize(sz);
            p.resize(sz);
            for (auto& v : dp)
                v.resize(sz, 0);
            for (auto& v : p)
                v.resize(sz, 0);

            for (int len = 1; len < dp.size(); len++) {
                for (int i = 0; i < dp.size() - len; i++) {
                    int j = i + len;
                    dp[i][j] = INT_MAX;
                    for (int k = i; k < j; k++) {
                        if (relax(i, j, k)) {
                            p[i][j] = k;
                        }
                    }
                }
            }
        }
        
        bool relax(int i, int j, int k) {
            int x = dp[i][j];
            dp[i][j] = std::min(dp[i][j], dp[i][k] + dp[k + 1][j] + dims[i] * dims[k + 1] * dims[j + 1]);
            return x != dp[i][j];
        }

        void computeOptimalOrder_impl(int i, int j) {
            if (i == j)
                return;

            computeOptimalOrder_impl(i, p[i][j]);
            computeOptimalOrder_impl(p[i][j] + 1, j);

            bestOrder.push_back(p[i][j]);
        }

    private:
        containers::vector<size_type> dims;
        containers::vector<size_type> bestOrder;
        containers::vector<containers::vector<size_type>> dp, p;
    };

    template<typename T>
    class MatrixChainComputer {
    public:
        using value_type = MatrixChain<T>::value_type;
        using size_type = MatrixChain<T>::size_type;

    public:
        template<typename It1, typename It2>
        MatrixChainComputer(It1 begin, It1 end, It2 order_begin, It2 order_end) : m(begin, end), order(order_begin, order_end) {}

    public:
        Matrix<value_type> computeChainOptimal() const {
            containers::vector<Matrix<T>> tmp(m);
            containers::vector<size_type> bestOrder = order;
            std::unordered_map<int, int> mp;

            for (int i = 0; i < m.size(); i++)
                mp[i] = i;
                
            for (int x : bestOrder) {
                int first = x, second = x + 1;
                
                while (first != mp[first])
                    first = mp[first];

                mp[second] = first;

                tmp[first] *= tmp[second];
            }
            return tmp[0];
        }

        Matrix<value_type> computeChainDefault() const {
            Matrix res(m[0]);
            for (int i = 1; i < m.size(); i++) {
                res *= m[i];
            }
            return res;
        }
    
    private:
        containers::vector<Matrix<T>> m;
        containers::vector<size_type> order;
    };
}