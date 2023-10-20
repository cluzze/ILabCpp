#pragma once

#include "vector.hpp"

#include <concepts>
#include <iterator>
#include <numeric>
#include <iostream>
namespace LinAl
{
    template<typename T>
    class Matrix
    {
    public:
        using value_type = T;
        using size_type = std::size_t;
    public:
        Matrix(size_type rows, size_type cols, const value_type& value = value_type{}) : 
                            rows_{rows}, cols_{cols},
                            m1(rows_ * cols_, value),
                            m2(rows_, containers::vector<value_type>(cols_, value)) {

        }

        template <typename It>
        Matrix(size_type rows, size_type cols, It begin, It end) : rows_{rows}, cols_{cols}, m1(begin, end) {
            fill_m2();
        }

        Matrix(size_type rows, size_type cols, std::initializer_list<value_type> init) : Matrix(rows, cols, init.begin(), init.end()) {}

        containers::vector<value_type>& operator[](size_type id) & {
            return m2[id];
        }

        const containers::vector<value_type>& operator[](size_type id) const & {
            return m2[id];
        }

        Matrix& transpose() & {
            Matrix res(cols_, rows_);
            for (int i = 0; i < rows_; i++) {
                for (int j = 0; j < cols_; j++) {
                    res[j][i] = m2[i][j];
                }
            }
            std::swap(*this, res);
            return *this;
        }

        int row_echelon_form() {
            int sign = 1;

            for (size_type k = 0; k < rows_; ++k) {
                std::pair<size_type, value_type> pivot = find_pivot(k, k);

                if (pivot.second == T{})
                    return 0;

                if (pivot.first != k) {
                    swap_rows(pivot.first, k);
                    sign *= -1;
                }

                auto cur_row = m2[k];
                
                for (size_type i = k + 1; i < rows_; ++i) {
                    auto& elim_row = m2[i];

                    if (elim_row[k] == T{}) {
                        continue;
                    }

                    value_type coef = elim_row[k] / cur_row[k];

                    for (size_type j = k; j < cols_; ++j) {
                        elim_row[j] -= cur_row[j] * coef;
                    }

                }
                // std::cout << '\n';
                //print();
            }

            return sign;
        }

        void print() const {
            for (int i = 0; i < rows_; i++) {
                for (int j = 0; j < cols_; j++)
                    std::cout << m2[i][j] << ' ';
                std::cout << '\n';
            }
        }

        value_type determinant() const requires std::is_floating_point<T>::value {
            if (!isSquare())
                throw std::runtime_error("Non square matrix asking for its determinent :(");

            Matrix tmp{*this};

            int k = tmp.row_echelon_form();
            int det = k > 0 ? 1 : -1;
        
            if (!k)
                return T{};

            for (int i = 0; i < rows_; i++)
                det *= tmp[i][i];

            det /= std::abs(k);

            return det;
        }
        
        value_type bareiss_det() requires std::is_integral<T>::value {
            value_type sign = 1;

            for (size_type k = 0; k < rows_ - 1; ++k) {
                auto pivot = findFirstNonZeroInColumn(k, k);

                if (pivot.second == T{})
                    return 0;

                if (pivot.first != k) {
                    swap_rows(pivot.first, k);
                    sign *= -1;
                }

                for (size_type i = k + 1; i < rows_; ++i) {
                    for (size_type j = k + 1; j < cols_; ++j) {
                        m2[i][j] = m2[k][k] * m2[i][j] - m2[i][k] * m2[k][j];
                        if (k == 0)
                            continue;
                        m2[i][j] /= m2[k - 1][k - 1];
                    }
                }
            }
            return sign * m2[rows_ - 1][rows_ - 1];
        }

        value_type determinant() const requires std::is_integral<T>::value {
            if (!isSquare())
                throw std::runtime_error("Non square matrix asking for its determinent :(");

            Matrix tmp{*this};

            return tmp.bareiss_det();
        }

        bool isSquare() const {
            return rows_ == cols_;
        }
        
        void swap_rows(size_type row1, size_type row2) {
            std::swap(m2[row1], m2[row2]);
        }

        size_type rows() const { return rows_; }
        size_type cols() const { return cols_; }

        bool equals(const Matrix& other, value_type prec = value_type{}) const {
            if (rows_ != other.rows_ || cols_ != other.cols_)
                return false;

            for (int i = 0; i < rows_; i++)
                for (int j = 0; j < cols_; j++)
                    if (std::abs(m2[i][j] - other[i][j]) > prec)
                        return false;

            return true;
        }
        
        Matrix& operator*=(const Matrix& rhs) {
            if (cols_ != rhs.rows_)
                throw std::runtime_error("number of cols must be equal number of rows");

            Matrix res(rows_, rhs.cols_);
            Matrix tmp(rhs);
            tmp.transpose();
            for (int i = 0; i < res.rows_; i++) {
                for (int j = 0; j < res.cols_; j++) {
                    res[i][j] = std::inner_product(m2[i].begin(), m2[i].end(), tmp[j].begin(), value_type{});
                }
            }
            std::swap(*this, res);
            return *this;
        }

    private:
        std::pair<size_type, value_type> find_pivot(size_type row, size_type col) const {
            size_type id = row;
            for (size_type i = row; i < rows_; ++i)
                if (std::abs(m2[id][col]) < std::abs(m2[i][col]))
                    id = i;

            return {id, m2[id][col]};
        }

        std::pair<size_type, value_type> findFirstNonZeroInColumn(size_type row, size_type col) const {
            size_type id = row;
            for (; id < rows_; id++)
                if (m2[id][col] != 0)
                    break;
            if (id == rows_)
                return {id, 0};
                
            return {id, m2[id][col]};
        }

        void fill_m2() {
            m2.resize(rows_);
            for (int i = 0; i < rows_; i++) {
                m2[i].resize(cols_);
                for (int j = 0; j < cols_; j++) {
                    m2[i][j] = m1[i * cols_ + j];
                }
            }
        }

    private:
        size_type rows_, cols_;
        containers::vector<value_type> m1;
        containers::vector<containers::vector<value_type>> m2;
    };

    template<typename T>
    Matrix<T> operator*(const Matrix<T>& lhs, const Matrix<T>& rhs) {
        Matrix<T> tmp(lhs);
        return tmp *= rhs;
    }
} // namespace LinAl
