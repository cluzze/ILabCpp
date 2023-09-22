#pragma once

#include "vector.hpp"

namespace LinAl
{
    template<typename T>
    class Matrix
    {
    public:
        std::pair<int, int> find_pivot(int col, int row) {
            int id = row;
            for (int i = row; i < rows; i++)
                if (std::abs((*this)[id][col]) < std::abs((*this)[i][col]))
                    id = i;

            return {i, (*this)[i][col]};
        }

        int row_echelon_form() {
            int sign = 1;

            for (int i = 0; i < rows; i++) {
                auto pivot = find_pivot(i, i);
                if (pivot == T{})
                    return 0;
                
                if (pivot != i) {
                    swap_rows(pivot, i);
                    sign *= -1;
                }

                for (int j = 0; j < rows; j++) {
                    if (i == j)
                        continue;
                    
                    
                }
            }
        }

        T determinant() const {
            if (!square)
                throw std::runtime_error("Non square matrix asking for its determinent :(");

            Matrix tmp{*this};

            bool m = tmp.row_echelon_form();
            
            if (!m)
                return T{};
            
            int det = m.value();

            for (int i = 0; i < rows; i++)
                det *= tmp[i][i];

            return det;
        }

    private:
        int rows_, cols_;
        
    };
} // namespace LinAl
