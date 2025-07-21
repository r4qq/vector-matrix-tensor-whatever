/*
    lol.
    Copyright (C) 2025 r4qq
*/


#pragma once

#include <algorithm>
#include <cstddef>
#include <iostream>
#include <stdexcept>
#include <type_traits>
#include <vector>

namespace Tensor 
{
    template<typename T>
    class Tensor
    {
        private:
            size_t rows, cols;
            std::vector<T> data;

        public: 
            Tensor(size_t rows, size_t cols) 
                : rows(rows), cols(cols), data(rows * cols) 
                {static_assert(std::is_arithmetic<T>::value, "Type must be numeric");}

            constexpr size_t rowCount() const {return rows;}
            constexpr size_t colCount() const {return cols;}
            std::vector<T>& retData() {return data;}
            const std::vector<T>& retData() const {return data;}

            T& operator()(size_t i, size_t j)
            {
                if (i >= rows || j >= cols) 
                    throw std::out_of_range("Index out of range"); 
                return data[(i * cols) + j];
            } 

            const T& operator()(size_t i, size_t j) const
            {
                if (i >= rows || j >= cols) 
                    throw std::out_of_range("Index out of range"); 
                
                return data[(i * cols) + j];
            } 

            bool operator==(const Tensor<T>& otherTensor) const 
            {
                return rows == otherTensor.rows && cols == otherTensor.cols && data == otherTensor.data;
            }

            bool operator!=(const Tensor<T>& otherTensor) const 
            {
                return !(*this == otherTensor);
            }

            Tensor<T> operator+(const Tensor<T>& otherTensor) const
            {
                if(rows != otherTensor.rows || cols != otherTensor.cols)
                    throw std::runtime_error("Size mismatch");
                    
                Tensor<T> result(rows, cols);

                std::transform(data.begin(), data.end(), otherTensor.data.begin(), result.data.begin(),
                               [](const T& a, const T&b){return a + b;});

                return result;
            }

            Tensor<T> operator-(const Tensor<T>& otherTensor) const
            {
                if(rows != otherTensor.rows || cols != otherTensor.cols)
                    throw std::runtime_error("Size mismatch");
                    
                Tensor<T> result(rows, cols);

                std::transform(data.begin(), data.end(), otherTensor.data.begin(), result.data.begin(),
                               [](const T& a, const T&b){return a - b;});

                return result;
            }

            Tensor<T> operator*(const T& scalar) const
            {
                Tensor<T> result(rows, cols);

                std::transform(data.begin(), data.end(), result.data.begin(),
                               [&scalar](const T& val){return val * scalar;});

                return result;                
            }

            Tensor<T> operator*(const Tensor<T>& otherTensor) const
            {
                if(cols != otherTensor.rows)
                    throw std::runtime_error("Matrix dimensions incompatible for multiplication");
                
                size_t T1 = rows;
                size_t T2 = otherTensor.cols;
                size_t T3 = cols;
                Tensor<T> result(T1, T2);

                if (T1 == 1 && T2 == 1)
                { 
                    result(0, 0) = (*this)(0, 0) * otherTensor(0, 0);
                    return result;
                }

                for (size_t i = 0; i < T1; ++i)
                {
                    for (size_t j = 0; j < T2; ++j)
                    {
                        T sum = T{};
                        for (size_t k = 0; k < T3; ++k)
                        {
                            sum += (*this)(i, k) * otherTensor(k, j);
                        }
                        result(i, j) = sum;
                    }
                }
                
                return result;
            }

            void transpose() 
            {
                Tensor<T> result(cols, rows);
                for (size_t i = 0; i < rows; ++i) 
                {
                    for (size_t j = 0; j < cols; ++j) 
                    {
                        result(j, i) = (*this)(i, j);
                    }
                }
                (*this) = result;
            }

            template<typename U>
            void fill(const U& value)
            {
                static_assert(std::is_convertible<U, T>::value, "U must be convertible to T");
                std::fill(data.begin(), data.end(), static_cast<T>(value));
            }

            inline void print() const 
            {
                for (size_t i = 0; i < rows; ++i) 
                {
                    for (size_t j = 0; j < cols; ++j) 
                    {
                        std::cout << data[i * cols + j];
                        if (j < cols - 1) std::cout << " ";
                    }
                    std::cout << std::endl;
                }
            }
    };

    template<typename T, typename U>
    Tensor<T> operator*(const U& scalar, const Tensor<T>& tensor)
    {
        return tensor * scalar;       
    }
}   
