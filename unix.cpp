#include <iostream>
#include "all_headers.hpp"

// Tvec2 tests
void test_tvec2_addition() {
    vec2 v1(1.0f, 2.0f);
    vec2 v2(3.0f, 4.0f);
    vec2 expected(4.0f, 6.0f);
    vec2 result = v1 + v2;

    if (result.x == expected.x && result.y == expected.y)
        std::cout << "Tvec2 addition test PASSED!\n";
    else
        std::cout << "Tvec2 addition test FAILED!\n";
}

void test_tvec2_subtraction() {
    vec2 v1(5.0f, 7.0f);
    vec2 v2(3.0f, 4.0f);
    vec2 expected(2.0f, 3.0f);
    vec2 result = v1 - v2;

    if (result.x == expected.x && result.y == expected.y)
        std::cout << "Tvec2 subtraction test PASSED!\n";
    else
        std::cout << "Tvec2 subtraction test FAILED!\n";
}

void test_tvec2_scalar_multiplication() {
    vec2 v(2.0f, 3.0f);
    float scalar = 2.0f;
    vec2 expected(4.0f, 6.0f);
    vec2 result = v * scalar;

    if (result.x == expected.x && result.y == expected.y)
        std::cout << "Tvec2 scalar multiplication test PASSED!\n";
    else
        std::cout << "Tvec2 scalar multiplication test FAILED!\n";
}

void test_tvec2_dot_product() {
    vec2 v1(1.0f, 2.0f);
    vec2 v2(3.0f, 4.0f);
    float expected = 11.0f;  // 1*3 + 2*4
    float result = v1.dot(v2);

    if (result == expected)
        std::cout << "Tvec2 dot product test PASSED!\n";
    else
        std::cout << "Tvec2 dot product test FAILED!\n";
}

void test_tvec2_magnitude() {
    vec2 v(3.0f, 4.0f);
    float expected = 5.0f; // sqrt(3^2 + 4^2) = 5
    float result = v.magnitude();

    if (result == expected)
        std::cout << "Tvec2 magnitude test PASSED!\n";
    else
        std::cout << "Tvec2 magnitude test FAILED!\n";
}

void test_tvec2_unit_vector() {
    vec2 v(3.0f, 4.0f);
    vec2 result = v.unit();
    float magnitude = result.magnitude();
    
    if (std::abs(magnitude - 1.0f) < 1e-5)
        std::cout << "Tvec2 unit vector test PASSED!\n";
    else
        std::cout << "Tvec2 unit vector test FAILED!\n";
}

// Tvec3 tests
void test_tvec3_addition() {
    vec3 v1(1.0f, 2.0f, 3.0f);
    vec3 v2(4.0f, 5.0f, 6.0f);
    vec3 expected(5.0f, 7.0f, 9.0f);
    vec3 result = v1 + v2;

    if (result.x == expected.x && result.y == expected.y && result.z == expected.z)
        std::cout << "Tvec3 addition test PASSED!\n";
    else
        std::cout << "Tvec3 addition test FAILED!\n";
}

void test_tvec3_subtraction() {
    vec3 v1(5.0f, 7.0f, 9.0f);
    vec3 v2(3.0f, 4.0f, 5.0f);
    vec3 expected(2.0f, 3.0f, 4.0f);
    vec3 result = v1 - v2;

    if (result.x == expected.x && result.y == expected.y && result.z == expected.z)
        std::cout << "Tvec3 subtraction test PASSED!\n";
    else
        std::cout << "Tvec3 subtraction test FAILED!\n";
}

void test_tvec3_scalar_multiplication() {
    vec3 v(2.0f, 3.0f, 4.0f);
    float scalar = 2.0f;
    vec3 expected(4.0f, 6.0f, 8.0f);
    vec3 result = v * scalar;

    if (result.x == expected.x && result.y == expected.y && result.z == expected.z)
        std::cout << "Tvec3 scalar multiplication test PASSED!\n";
    else
        std::cout << "Tvec3 scalar multiplication test FAILED!\n";
}

void test_tvec3_dot_product() {
    vec3 v1(1.0f, 2.0f, 3.0f);
    vec3 v2(4.0f, 5.0f, 6.0f);
    float expected = 32.0f;  // 1*4 + 2*5 + 3*6
    float result = v1.dot(v2);

    if (result == expected)
        std::cout << "Tvec3 dot product test PASSED!\n";
    else
        std::cout << "Tvec3 dot product test FAILED!\n";
}

void test_tvec3_magnitude() {
    vec3 v(1.0f, 2.0f, 2.0f);
    float expected = 3.0f; // sqrt(1^2 + 2^2 + 2^2) = 3
    float result = v.magnitude();

    if (result == expected)
        std::cout << "Tvec3 magnitude test PASSED!\n";
    else
        std::cout << "Tvec3 magnitude test FAILED!\n";
}

void test_tvec3_unit_vector() {
    vec3 v(1.0f, 2.0f, 2.0f);
    vec3 result = v.unit();
    float magnitude = result.magnitude();
    
    if (std::abs(magnitude - 1.0f) < 1e-5)
        std::cout << "Tvec3 unit vector test PASSED!\n";
    else
        std::cout << "Tvec3 unit vector test FAILED!\n";
}

void test_tvec3_cross_product() {
    vec3 v1(1.0f, 0.0f, 0.0f);
    vec3 v2(0.0f, 1.0f, 0.0f);
    vec3 expected(0.0f, 0.0f, 1.0f);  // cross product of (1,0,0) and (0,1,0) = (0,0,1)
    vec3 result = v1.cross(v2);

    if (result.x == expected.x && result.y == expected.y && result.z == expected.z)
        std::cout << "Tvec3 cross product test PASSED!\n\n";
    else
        std::cout << "Tvec3 cross product test FAILED!\n\n";
}

// Matrix tests
void test_matrix_creation() {
    Matrix<int> mat(3, 3);
    bool pass = true;

    // Check if all elements are initialized to 0
    for (int i = 0; i < mat.numRows(); ++i) {
        for (int j = 0; j < mat.numCols(); ++j) {
            if (mat(i, j) != 0) {
                pass = false;
                break;
            }
        }
    }

    if (pass) std::cout << "Matrix creation test PASSED!\n";
    else std::cout << "Matrix creation test FAILED!\n";
}

void test_matrix_access() {
    Matrix<int> mat(2, 2);
    mat(0, 0) = 1;
    mat(0, 1) = 2;
    mat(1, 0) = 3;
    mat(1, 1) = 4;

    if (mat(0, 0) == 1 && mat(0, 1) == 2 && mat(1, 0) == 3 && mat(1, 1) == 4)
        std::cout << "Matrix access test PASSED!\n";
    else
        std::cout << "Matrix access test FAILED!\n";
}

void test_matrix_multiplication() {
    Matrix<int> mat1(2, 3);
    Matrix<int> mat2(3, 2);

    // mat1: 2x3
    mat1(0, 0) = 1; mat1(0, 1) = 2; mat1(0, 2) = 3;
    mat1(1, 0) = 4; mat1(1, 1) = 5; mat1(1, 2) = 6;

    // mat2: 3x2
    mat2(0, 0) = 7; mat2(0, 1) = 8;
    mat2(1, 0) = 9; mat2(1, 1) = 10;
    mat2(2, 0) = 11; mat2(2, 1) = 12;

    // Expected result: 2x2 matrix
    Matrix<int> expected(2, 2);
    expected(0, 0) = 58; expected(0, 1) = 64;
    expected(1, 0) = 139; expected(1, 1) = 154;

    Matrix<int> result = mat1 * mat2;

    if (result == expected)
        std::cout << "Matrix multiplication test PASSED!\n";
    else
        std::cout << "Matrix multiplication test FAILED!\n";
}

void test_matrix_transpose() {
    Matrix<int> mat(2, 3);
    mat(0, 0) = 1; mat(0, 1) = 2; mat(0, 2) = 3;
    mat(1, 0) = 4; mat(1, 1) = 5; mat(1, 2) = 6;

    Matrix<int> expected(3, 2);
    expected(0, 0) = 1; expected(1, 0) = 2; expected(2, 0) = 3;
    expected(0, 1) = 4; expected(1, 1) = 5; expected(2, 1) = 6;

    Matrix<int> result = mat.transpose();

    if (result == expected)
        std::cout << "Matrix transpose test PASSED!\n";
    else
        std::cout << "Matrix transpose test FAILED!\n";
}

void test_matrix_equality() {
    Matrix<int> mat1(2, 2);
    Matrix<int> mat2(2, 2);

    mat1(0, 0) = 1; mat1(0, 1) = 2;
    mat1(1, 0) = 3; mat1(1, 1) = 4;

    mat2(0, 0) = 1; mat2(0, 1) = 2;
    mat2(1, 0) = 3; mat2(1, 1) = 4;

    if (mat1 == mat2)
        std::cout << "Matrix equality test PASSED!\n";
    else
        std::cout << "Matrix equality test FAILED!\n";
}

int main() {
    std::cout << "Running Tvec2 tests...\n";
    test_tvec2_addition();
    test_tvec2_subtraction();
    test_tvec2_scalar_multiplication();
    test_tvec2_dot_product();
    test_tvec2_magnitude();
    test_tvec2_unit_vector();

    std::cout << "\nRunning Tvec3 tests...\n";
    test_tvec3_addition();
    test_tvec3_subtraction();
    test_tvec3_scalar_multiplication();
    test_tvec3_dot_product();
    test_tvec3_magnitude();
    test_tvec3_unit_vector();
    test_tvec3_cross_product();

    std::cout << "Running Matrix tests...\n";
    test_matrix_creation();
    test_matrix_access();
    test_matrix_multiplication();
    test_matrix_transpose();
    test_matrix_equality();

    return 0;
}