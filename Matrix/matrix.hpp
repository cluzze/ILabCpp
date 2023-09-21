#pragma once

namespace LinAl
{
    template<typename T>
    class Matrix
    {
    public:
        Matrix(int rows, int cols, const T& value);

    private:
        int rows_, cols_;
        
    };
} // namespace LinAl
