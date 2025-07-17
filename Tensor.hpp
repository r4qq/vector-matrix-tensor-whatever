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

            size_t rowCout() const {return rows;}
            size_t colCout() const {return cols;}

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

            Tensor<T> transpose() const
            {
                Tensor<T> result(rows, cols);

                
            }

            template<typename U>
            void fill(const U& value)
            {
                static_assert(std::is_convertible<T, U>::value, "U must be convertible to T");
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
}   
