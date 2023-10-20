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
        containers::vector<size_type> getOptimalOrder() const {    
            return bestOrder;
        }

        size_type getOptimalNumberOfMultiplications() const {
            if (dims.size() < 3)
                return 0;
            
            return dp[0][chain.size() - 1];
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

        void printOptimalOrder() const {
            for (auto x : bestOrder)
                std::cout << x << ' ';
            std::cout << '\n';
        }
        
        /*get result of chain multiplacation*/
        Matrix<value_type> computeChainOptimal() const {
            containers::vector<Matrix<T>> tmp(chain);
            std::unordered_map<int, int> mp;

            for (int i = 0; i < chain.size(); i++)
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
            Matrix res(chain[0]);
            for (int i = 1; i < chain.size(); i++) {
                res *= chain[i];
            }
            return res;
        }

        void addMatrix(const Matrix<T>& matrix) {
            bestOrder.resize(0);
            chain.push_back(matrix);
            if (chain.size() == 1)
                dims.push_back(matrix.rows());
            dims.push_back(matrix.cols());
            resizeDp();
            recomputeDp();
            //computeFromScratch();
            computeOptimalOrder_impl(0, chain.size() - 1);
        }

        void printDims() const {
            for (auto x : dims)
                std::cout << x << ' ';
            std::cout << '\n';
        }
        
        void printDp() const {
            for (int i = 0; i < dp.size(); i++) {
                for (int j = 0; j < dp[0].size(); j++) {
                    std::cout << dp[i][j] << ' ';
                }
                std::cout << '\n';
            }
            std::cout << '\n';
        }

        void printP() const {
            for (int i = 0; i < p.size(); i++) {
                for (int j = 0; j < p[0].size(); j++) {
                    std::cout << p[i][j] << ' ';
                }
                std::cout << '\n';
            }
            std::cout << '\n';
        }

    private:
        void recomputeDp() {
            assert(chain.size() == dp.size());

            for (int len = 1; len < chain.size(); len++) {
                int i = chain.size() - len - 1;
                int j = chain.size() - 1;
                dp[i][j] = INT_MAX;
                for (int k = i; k < j; k++) {
                    if (relax(i, j, k)) {
                        p[i][j] = k;
                    }
                }
            }
        }
        
        bool relax(int i, int j, int k) {
            int x = dp[i][j];
            dp[i][j] = std::min(dp[i][j], dp[i][k] + dp[k + 1][j] + dims[i] * dims[k + 1] * dims[j + 1]);
            return x != dp[i][j];
        }

        void computeFromScratch() {
            assert(chain.size() == dp.size());

            for (int i = 0; i < p.size() - 1; i++) {
                p[i][i] = i;
            }

            for (int len = 1; len < chain.size(); len++) {
                for (int i = 0; i < chain.size() - len; i++) {
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

        void computeOptimalOrder_impl(int i, int j) {
            if (i == j)
                return;

            computeOptimalOrder_impl(i, p[i][j]);
            computeOptimalOrder_impl(p[i][j] + 1, j);

            bestOrder.push_back(p[i][j]);
        }

        void resizeDp() {
            auto sz = chain.size();
            dp.resize(sz);
            p.resize(sz);
            for (auto& v : dp)
                v.resize(sz, 0);
            for (auto& v : p)
                v.resize(sz, 0);
        }

    private:
        containers::vector<Matrix<T>> chain;
        containers::vector<size_type> dims;
        containers::vector<size_type> bestOrder;
        containers::vector<containers::vector<size_type>> dp, p;
    };
}