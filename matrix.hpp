#ifndef __MATRIX_HPP__
#define __MATRIX_HPP__

#include <cmath> // for sqrt function
#include <vector> // for matrix storage
#include <stdexcept> // for exception handling

// Template class for a generic matrix
template <typename T>
class Matrix {
private:
    std::vector<std::vector<T>> data;  // 2D vector to store matrix elements
    int rows, cols;  // Matrix dimensions (rows and columns)

public:
    // Constructor to initialize matrix of given size (rows x cols) with all elements set to 0
    Matrix(int _rows, int _cols) : rows(_rows), cols(_cols) {
        data.resize(rows, std::vector<T>(cols, 0)); // Initialize all elements to zero
    }

    // Access element at given row and column (with bounds checking)
    T& operator()(int row, int col) {
        if (row >= rows || col >= cols) {
            throw std::out_of_range("Index out of bounds");
        }
        return data[row][col];
    }

    // Get the number of rows in the matrix
    int numRows() const { return rows; }

    // Get the number of columns in the matrix
    int numCols() const { return cols; }

    // Matrix multiplication
    Matrix operator*(const Matrix& rhs) const {
        if (cols != rhs.rows) {
            throw std::invalid_argument("Matrix dimensions do not match for multiplication");
        }

        Matrix result(rows, rhs.cols);  // Result matrix with appropriate size
        for (int i = 0; i < rows; ++i) {
            for (int j = 0; j < rhs.cols; ++j) {
                for (int k = 0; k < cols; ++k) {
                    result(i, j) += data[i][k] * rhs.data[k][j];  // Multiply and accumulate
                }
            }
        }
        return result;
    }

    // Transpose the matrix (swap rows and columns)
    Matrix transpose() const {
        Matrix result(cols, rows);  // Create matrix with swapped dimensions
        for (int i = 0; i < rows; ++i) {
            for (int j = 0; j < cols; ++j) {
                result(j, i) = data[i][j];  // Swap elements
            }
        }
        return result;
    }

    // Equality check between two matrices
    bool operator==(const Matrix& rhs) const {
        if (rows != rhs.rows || cols != rhs.cols) {
            return false;  // Different dimensions, matrices are not equal
        }

        // Compare each element
        for (int i = 0; i < rows; ++i) {
            for (int j = 0; j < cols; ++j) {
                if (data[i][j] != rhs.data[i][j]) {
                    return false;  // Found a mismatch
                }
            }
        }
        return true;  // All elements are equal
    }
};

#endif // __MATRIX_HPP__
