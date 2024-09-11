#ifndef __VEC_HPP__
#define __VEC_HPP__

template <typename T>
class Matrix {
private:
    std::vector<std::vector<T>> data;
    int rows, cols;

public:
    // Constructor
    Matrix(int _rows, int _cols) : rows(_rows), cols(_cols) {
        data.resize(rows, std::vector<T>(cols, 0)); // Initialize all elements to zero
    }

    // Access element
    T& operator()(int row, int col) {
        if (row >= rows || col >= cols) {
            throw std::out_of_range("Index out of bounds");
        }
        return data[row][col];
    }

    // Get matrix dimensions
    int numRows() const { return rows; }
    int numCols() const { return cols; }

    // Matrix multiplication
    Matrix operator*(const Matrix& rhs) const {
        if (cols != rhs.rows) {
            throw std::invalid_argument("Matrix dimensions do not match for multiplication");
        }

        Matrix result(rows, rhs.cols);
        for (int i = 0; i < rows; ++i) {
            for (int j = 0; j < rhs.cols; ++j) {
                for (int k = 0; k < cols; ++k) {
                    result(i, j) += data[i][k] * rhs.data[k][j];
                }
            }
        }
        return result;
    }

    // Transpose
    Matrix transpose() const {
        Matrix result(cols, rows);
        for (int i = 0; i < rows; ++i) {
            for (int j = 0; j < cols; ++j) {
                result(j, i) = data[i][j];
            }
        }
        return result;
    }

    // Equality operator
    bool operator==(const Matrix& rhs) const {
        if (rows != rhs.rows || cols != rhs.cols) {
            return false;
        }

        for (int i = 0; i < rows; ++i) {
            for (int j = 0; j < cols; ++j) {
                if (data[i][j] != rhs.data[i][j]) {
                    return false;
                }
            }
        }
        return true;
    }
};

#endif // __VEC_HPP__