#include "matrix.hpp"

#include <algorithm>
#include <numeric>
#include <random>
#include <gtest/gtest.h>

using namespace LinAl;

std::vector<std::vector<int>> tests = {
    {4,
     0, 0,  0, -1,
     0, 0, -1,  0,
     0, 1,  0,  0,
     1, 0,  0,  0 },

    {4,
     0,  0, -1, 0,
     0,  0,  0, 1,
     1,  0,  0, 0,
     0, -1,  0, 0 },
     
    {4,
     0, -1, 0, 0,
     1,  0, 0, 0,
     0,  0, 0, -1,
     0,  0, 1, 0 },

    {4,
     0, 0, 9, 1,
     0, 0, 9, 2,
     0, 0, 9, 3,
     1, 2, 3, 4 },
    
    {4,
     1, 1, 1, 0,
    -1, 2, 1, 0,
     1, 4, 1, 0,
     0, 0, 0, 3},
    
    {4,
     1, 2, 2, 2,
     2, 1, 2, 2,
     2, 2, 1, 2,
     2, 2, 2, 1},

    {8,
     0, 1, 0, 3, 0, 0, 0, 0,
     2, 0, 1, 0, 3, 0, 0, 0,
     0, 2, 0, 1, 0, 3, 0, 0,
     3, 0, 2, 0, 1, 0, 3, 0,
     0, 3, 0, 2, 0, 1, 0, 3,
     0, 0, 3, 0, 2, 0, 1, 0,
     0, 0, 0, 3, 0, 2, 0, 1,
     0, 0, 0, 0, 3, 0, 2, 0 }
};

std::vector<int> res = {1, 1, 1, 0, -18, -7, 7081};

TEST(matrix, ctor) {
    LinAl::Matrix m(2, 2, 0);
    std::vector<double> v(9);
    std::iota(v.begin(), v.end(), 1);
    LinAl::Matrix<double> m2(3, 3, v.begin(), v.end());
}

TEST(matrix, det1) {
    LinAl::Matrix<int> m(tests[0][0], tests[0][0], tests[0].begin() + 1, tests[0].end());
    int det = m.determinant();
    ASSERT_EQ(det, res[0]);
}

TEST(matrix, det2) {
    LinAl::Matrix<int> m(tests[1][0], tests[1][0], tests[1].begin() + 1, tests[1].end());
    int det = m.determinant();
    ASSERT_EQ(det, res[1]);
}

TEST(matrix, det3) {
    LinAl::Matrix<int> m(tests[2][0], tests[2][0], tests[2].begin() + 1, tests[2].end());
    int det = m.determinant();
    ASSERT_EQ(det, res[2]);
}

TEST(matrix, det4) {
    LinAl::Matrix<int> m(tests[3][0], tests[3][0], tests[3].begin() + 1, tests[3].end());
    int det = m.determinant();
    ASSERT_EQ(det, res[3]);
}

TEST(matrix, det5) {
    LinAl::Matrix<int> m(tests[4][0], tests[4][0], tests[4].begin() + 1, tests[4].end());
    int det = m.determinant();
    ASSERT_EQ(det, res[4]);
}

TEST(matrix, det6) {
    LinAl::Matrix<int> m(tests[5][0], tests[5][0], tests[5].begin() + 1, tests[5].end());
    int det = m.determinant();
    ASSERT_EQ(det, res[5]);
}

TEST(matrix, det7) {
    LinAl::Matrix<int> m(tests[6][0], tests[6][0], tests[6].begin() + 1, tests[6].end());
    int det = m.determinant();
    ASSERT_EQ(det, res[6]);
}

TEST(matrix, gen1) {
    size_t size = 10, res = 1;;
    LinAl::Matrix<int> m(size, size);
    for (int i = 0; i < size; ++i) {
        m[i][i] = i;
        res *= i;
    }
    m.swap_rows(1, 4);
    m.swap_rows(2, 8);
    m.swap_rows(3, 5);
    int det = m.determinant();
    ASSERT_EQ(det, res);
}

int calculateDeterminant(const std::vector<std::vector<int>>& matrix) {
    int size = matrix.size();

    if (size == 2) {
        return matrix[0][0] * matrix[1][1] - matrix[0][1] * matrix[1][0];
    }

    int determinant = 0;

    for (int col = 0; col < size; ++col) {
        std::vector<std::vector<int>> submatrix(size - 1, std::vector<int>(size - 1, 0));

        for (int subRow = 1; subRow < size; ++subRow) {
            int subCol = 0;
            for (int colIdx = 0; colIdx < size; ++colIdx) {
                if (colIdx != col) {
                    submatrix[subRow - 1][subCol] = matrix[subRow][colIdx];
                    ++subCol;
                }
            }
        }

        int sign = (col % 2 == 0) ? 1 : -1;
        determinant += sign * matrix[0][col] * calculateDeterminant(submatrix);
    }

    return determinant;
}

std::vector<std::vector<int>> generateMatrix(int size) {
    std::vector<std::vector<int>> matrix(size, std::vector<int>(size, 0));

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> dist(-10, 10);

    for (int row = 0; row < size; ++row) {
        for (int col = 0; col < size; ++col) {
            matrix[row][col] = dist(gen);
        }
    }

    return matrix;
}

TEST(matrix, det_gen1) {
    std::vector<std::vector<int>> matrix = generateMatrix(5);
    int det = calculateDeterminant(matrix);
    std::vector<int> matrix2(5 * 5);

    for (int i = 0; i < 5; i++) {
        for (int j = 0; j < 5; j++) {
            matrix2[i * 5 + j] = matrix[i][j];
        }
    }
    LinAl::Matrix<int> m(5, 5, matrix2.begin(), matrix2.end());
    int mdet = m.determinant();
    ASSERT_EQ(mdet, det);
}

std::vector<int> factorise(int x) {
    std::vector<int> res;
    for (int i = 2; x != 1 && i <= x; ++i) {
        if (x % i == 0) {
            res.push_back(i);
            x /= i;
            i = 2;
        }
    }
    return res;
}

std::vector<std::vector<int>> genMatrixWithDet(int size, int det, int scale) {
    std::vector<std::vector<int>> matrix(size, std::vector<int>(size));
    std::vector<int> fact = factorise(det);
    // for (int x : fact)
    //     std::cout << x << ' ';
    // std::cout << '\n';
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> dist(-scale, scale);
    for (int i = 0; i < size; ++i) {
        for (int j = 0; j < size; ++j) {
            if (j < i)
                matrix[i][j] = 0;
            else if (i == j)
                matrix[i][j] = i < fact.size() ? fact[i] : 1;
            else
                matrix[i][j] = dist(gen);
            //std::cout << matrix[i][j] << ' ';
        }
        //std::cout << '\n';
    }

    std::uniform_int_distribution<int> dist2(0, size - 1);    
    for (int k = 0; k < size; ++k) {
        int row = dist2(gen);
        int add = dist2(gen);
        if (row == add)
            continue;

        for (int i = 0; i < size; i++) {
            matrix[row][i] += matrix[add][i];
        }
    }

    // for (int i = 0; i < size; ++i) {
    //     for (int j = 0; j < size; ++j) {
    //         std::cout << matrix[i][j] << ' ';
    //     }
    //     std::cout << '\n';
    // }

    return matrix;
}

TEST(matrix, det_gen2) {
    size_t size = 100;
    int det = 42;
    std::vector<std::vector<int>> matrix = genMatrixWithDet(size, det, 2);
    std::vector<int> matrix2(size * size);

    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            matrix2[i * size + j] = matrix[i][j];
            
        }
    }
    LinAl::Matrix<int> m(size, size, matrix2.begin(), matrix2.end());
    int mdet = m.determinant();
    ASSERT_EQ(mdet, det);
}

TEST(Matrix, transpose) {
    Matrix<int> m1(2, 1, {1, 2}), m2 = m1;
    ASSERT_TRUE(m1.equals(m2.transpose().transpose()));
}

TEST(Matrix, mult1) {
    Matrix<int> m1(2, 1, {1, 2});
    Matrix<int> m2(1, 2, {4, 3});
    Matrix<int> m3 = m1 * m2;
    Matrix<int> exp(2, 2, {4, 3, 8, 6});
    ASSERT_TRUE(exp.equals(m3));
}