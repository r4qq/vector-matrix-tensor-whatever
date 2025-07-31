/**
 * @file Tensor.hpp
 * @brief Lightweight generic 2D tensor (matrix) implementation.
 * @author r4qq
 * @date 2025
 */

#pragma once

#include <algorithm>
#include <array>
#include <cstddef>
#include <functional>
#include <iostream>
#include <stdexcept>
#include <string>
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

        template<typename BinaryOp>
        inline Tensor<T> elementWiseOp(const Tensor<T>& otherTensor, BinaryOp op) const
        {
            
        }

        template<size_t N>
        size_t computeFlatIndex(const std::array<size_t, N>& indices) const
        {
            size_t flatIndex = 0;
            for(size_t i = 0; i < indices.size(); i++)
            {
                flatIndex += strides[i] * indices[i];
            }

            return flatIndex;
        }

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

        template<typename... Indices>
        T& operator()(Indices... indices)
        {
            if (sizeof...(indices) != shape.size())
                throw std::invalid_argument("Invalid number of indices");

            std::array<size_t, sizeof...(indices)> idxArr{static_cast<size_t>(indices)...};
            for(size_t i = 0; i < shape.size(); i++)
            {
                if(idxArr[i] >= shape[i])
                    throw std::out_of_range("Index" + std::to_string(idxArr[i]) + " is out of range");
            }

            return data[computeFlatIndex(idxArr)];
        }

        template<typename... Indices>
        const T& operator()(Indices... indices) const
        {
            if (sizeof...(indices) != shape.size())
                throw std::invalid_argument("Invalid number of indices");

            std::array<size_t, sizeof...(indices)> idxArr{static_cast<size_t>(indices)...};
            for(size_t i = 0; i < shape.size(); i++)
            {
                if(idxArr[i] >= shape[i])
                    throw std::out_of_range("Index" + std::to_string(idxArr[i]) + " is out of range");
            }

            return data[computeFlatIndex(idxArr)];
        }

       
        bool operator==(const Tensor<T>& otherTensor) const
        {
            return this->shape == otherTensor.shape && 
                   this->strides == otherTensor.strides && 
                   this->data == otherTensor.data;
        }

      
        bool operator!=(const Tensor<T>& otherTensor) const
        {
            return !(*this == otherTensor);
        }

       
        
       
        Tensor<T> operator+(const Tensor<T>& otherTensor) const
        {
            return elementWiseOp(otherTensor, std::plus<T>());
        }

        Tensor<T> operator-(const Tensor<T>& otherTensor) const
        {
            return elementWiseOp(otherTensor, std::minus<T>());
        }

     
        Tensor<T> operator*(const T& scalar) const
        {
            Tensor<T> result(this->data.size());
            std::transform(data.begin(), data.end(), result.data.begin(),
                           [&scalar](const T& val){ return val * scalar; });
            return result;
        }

        
        Tensor<T> operator*(const Tensor<T>& otherTensor) const
        {
            
        }
      
        Tensor<T> transpose() const
        {
            
        }

        template<typename U>
        void fill(const U& value)
        {
            static_assert(std::is_convertible<U, T>::value, "U must be convertible to T");
            std::fill(data.begin(), data.end(), static_cast<T>(value));
        }

        
        inline void print() const
        {
            
        }
    };

   
    template<typename T, typename U>
    Tensor<T> operator*(const U& scalar, const Tensor<T>& tensor)
    {
        return tensor * scalar;
    }
}
