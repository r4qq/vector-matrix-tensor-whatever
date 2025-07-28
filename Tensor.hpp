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
    /**
     * @brief A simple 2D tensor (matrix) class template for numeric types.
     * 
     * @tparam T Numeric type (e.g., int, float, double).
     */
    template<typename T>
    class Tensor
    {
        static_assert(std::is_arithmetic<T>::value, "Type must be numeric");

    private:
        size_t rows, cols;             ///< Number of rows and columns.
        std::vector<T> data;           ///< Flat storage of matrix elements.

    public:
        /**
         * @brief Constructs a Tensor of specified dimensions, initialized with zeros.
         * 
         * @param rows Number of rows.
         * @param cols Number of columns.
         * @throws std::invalid_argument if either dimension is zero.
         */
        Tensor(size_t rows, size_t cols)
            : rows(rows), cols(cols), data(rows * cols, T{})
        {
            if (rows == 0 || cols == 0)
                throw std::invalid_argument("Size can't be 0");
        }

        /// @brief Default destructor.
        ~Tensor() = default;

        /**
         * @brief Accesses (modifiable) the element at position (i, j).
         * 
         * @param i Row index.
         * @param j Column index.
         * @return Reference to the element.
         * @throws std::out_of_range on invalid indices.
         */
        T& operator()(size_t i, size_t j)
        {
            if (i >= rows || j >= cols)
                throw std::out_of_range("Index out of range: (" + std::to_string(i) + ", " + std::to_string(j) + ")");
            return data[(i * cols) + j];
        }

        /**
         * @brief Accesses (read-only) the element at position (i, j).
         * 
         * @param i Row index.
         * @param j Column index.
         * @return Const reference to the element.
         * @throws std::out_of_range on invalid indices.
         */
        const T& operator()(size_t i, size_t j) const
        {
            if (i >= rows || j >= cols)
                throw std::out_of_range("Index out of range: (" + std::to_string(i) + ", " + std::to_string(j) + ")");
            return data[(i * cols) + j];
        }

        /**
         * @brief Checks equality with another tensor.
         * 
         * @param otherTensor Tensor to compare.
         * @return true if dimensions and elements match.
         */
        bool operator==(const Tensor<T>& otherTensor) const
        {
            return rows == otherTensor.rows && cols == otherTensor.cols && data == otherTensor.data;
        }

        /**
         * @brief Checks inequality with another tensor.
         * 
         * @param otherTensor Tensor to compare.
         * @return true if tensors differ.
         */
        bool operator!=(const Tensor<T>& otherTensor) const
        {
            return !(*this == otherTensor);
        }

        /**
         * @brief Performs an element-wise operation with another tensor.
         * 
         * @tparam BinaryOp A callable binary operator (e.g., std::plus).
         * @param otherTensor The other tensor.
         * @param op Binary operation to apply.
         * @return Resulting tensor.
         * @throws std::runtime_error if dimensions mismatch.
         */
        template<typename BinaryOp>
        inline Tensor<T> elementWiseOp(const Tensor<T>& otherTensor, BinaryOp op) const
        {
            if(rows != otherTensor.rows || cols != otherTensor.cols)
                throw std::runtime_error("Size mismatch");

            Tensor<T> result(rows, cols);
            std::transform(data.begin(), data.end(), otherTensor.data.begin(), result.data.begin(), op);
            return result;
        }

        /**
         * @brief Element-wise addition with another tensor.
         * 
         * @param otherTensor The tensor to add.
         * @return Sum tensor.
         */
        Tensor<T> operator+(const Tensor<T>&& otherTensor) const
        {
            return elementWiseOp(otherTensor, std::plus<T>());
        }

        /**
         * @brief Element-wise subtraction with another tensor.
         * 
         * @param otherTensor The tensor to subtract.
         * @return Difference tensor.
         */
        Tensor<T> operator-(const Tensor<T>&& otherTensor) const
        {
            return elementWiseOp(otherTensor, std::minus<T>());
        }

        /**
         * @brief Scalar multiplication.
         * 
         * @param scalar Value to multiply each element.
         * @return Scaled tensor.
         */
        Tensor<T> operator*(const T& scalar) const
        {
            Tensor<T> result(rows, cols);
            std::transform(data.begin(), data.end(), result.data.begin(),
                           [&scalar](const T& val){ return val * scalar; });
            return result;
        }

        /**
         * @brief Matrix multiplication with another tensor.
         * 
         * @param otherTensor The tensor to multiply with.
         * @return Product tensor.
         * @throws std::runtime_error if dimensions are incompatible.
         */
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

        /**
         * @brief Returns the number of rows.
         * 
         * @return Number of rows.
         */
        constexpr size_t rowCount() const { return rows; }

        /**
         * @brief Returns the number of columns.
         * 
         * @return Number of columns.
         */
        constexpr size_t colCount() const { return cols; }

        /**
         * @brief Returns the transpose of the tensor.
         * 
         * @return Transposed tensor.
         */
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

        /**
         * @brief Fills all elements with a specific value.
         * 
         * @tparam U A type convertible to T.
         * @param value The value to fill.
         */
        template<typename U>
        void fill(const U& value)
        {
            static_assert(std::is_convertible<U, T>::value, "U must be convertible to T");
            std::fill(data.begin(), data.end(), static_cast<T>(value));
        }

        /**
         * @brief Prints the tensor to standard output.
         */
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

    /**
     * @brief Scalar multiplication.
     * 
     * @tparam T Tensor value type.
     * @tparam U Scalar type.
     * @param scalar The scalar value.
     * @param tensor The tensor.
     * @return Scaled tensor.
     */
    template<typename T, typename U>
    Tensor<T> operator*(const U& scalar, const Tensor<T>& tensor)
    {
        return tensor * scalar;
    }
}
