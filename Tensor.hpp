/**
 * @file Tensor.hpp
 * @brief Lightweight generic 2D tensor (matrix) implementation.
 * @author r4qq
 * @date 2025
 */

#pragma once

#include <algorithm>
#include <cstddef>
#include <functional>
#include <iostream>
#include <stdexcept>
#include <type_traits>
#include <vector>

namespace Tensor
{
    
    template<typename T>
    class Tensor
    {
        static_assert(std::is_arithmetic<T>::value, "Type must be numeric");

    private:
        std::vector<size_t> shape;     ///< Shape vector representing each dimension  
        std::vector<size_t> strides;
        std::vector<T> data;           ///< Flat storage of matrix elements.

    public:
       
        Tensor(std::vector<size_t> shape)
            : shape(std::move(shape)), 
              strides(this->shape.size()), 
              data([&]()
              {
                size_t size = 1;
                for (const auto& dim : this->shape) 
                {
                    if (dim == 0) 
                    {
                        throw std::invalid_argument("Shape dimension must be > 0");
                    }
                }
              })
        {
            strides.back() = 1;
            for (int i = static_cast<int>(shape.size()) - 2; i >= 0; --i)
                strides[i] = strides[i + 1] * shape[i + 1];
        }

        ~Tensor() = default;

        template<typename Indices>
        T& operator()(Indices indices)
        {
            if (indices)
                throw std::out_of_range("Index out of range: (" + std::to_string(i) + ", " + std::to_string(j) + ")");
            return data[(i * cols) + j];
        }

       
        const T& operator()(size_t i, size_t j) const
        {
            if (i >= rows || j >= cols)
                throw std::out_of_range("Index out of range: (" + std::to_string(i) + ", " + std::to_string(j) + ")");
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

       
        template<typename BinaryOp>
        inline Tensor<T> elementWiseOp(const Tensor<T>& otherTensor, BinaryOp op) const
        {
            if(rows != otherTensor.rows || cols != otherTensor.cols)
                throw std::runtime_error("Size mismatch");

            Tensor<T> result(rows, cols);
            std::transform(data.begin(), data.end(), otherTensor.data.begin(), result.data.begin(), op);
            return result;
        }

       
        Tensor<T> operator+(const Tensor<T>&& otherTensor) const
        {
            return elementWiseOp(otherTensor, std::plus<T>());
        }

        Tensor<T> operator-(const Tensor<T>&& otherTensor) const
        {
            return elementWiseOp(otherTensor, std::minus<T>());
        }

     
        Tensor<T> operator*(const T& scalar) const
        {
            Tensor<T> result(rows, cols);
            std::transform(data.begin(), data.end(), result.data.begin(),
                           [&scalar](const T& val){ return val * scalar; });
            return result;
        }

        
        Tensor<T> operator*(const Tensor<T>& otherTensor) const
        {
            if(cols != otherTensor.rows)
                throw std::runtime_error("Matrix dimensions incompatible for multiplication");

            size_t T1 = rows;
            size_t T2 = otherTensor.cols;
            Tensor<T> result(T1, T2);

            for (size_t i = 0; i < rows; ++i)
            {
                for (size_t j = 0; j < otherTensor.cols; ++j)
                {
                    T sum = T{};
                    for (size_t k = 0; k < cols; ++k)
                    {
                        sum += (*this)(i, k) * otherTensor(k, j);
                    }
                    result(i, j) = sum;
                }
            }
            return result;
        }

        constexpr size_t retShape() const { return shape; }
      
        Tensor<T> transpose() const
        {
            Tensor<T> result(cols, rows);
            for (size_t i = 0; i < rows; ++i)
            {
                for (size_t j = 0; j < cols; ++j)
                {
                    result(j, i) = (*this)(i, j);
                }
            }
            return result;
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
                    std::cout << (*this)(i, j);
                    if (j < cols - 1) std::cout << " ";
                }
                std::cout << std::endl;
            }
        }

        
        /*
        T determinant()
        {
            if(rows != cols)
                throw std::runtime_error("Matrix is not square");
            ...
        }
        */
    };

   
    template<typename T, typename U>
    Tensor<T> operator*(const U& scalar, const Tensor<T>& tensor)
    {
        return tensor * scalar;
    }
}
