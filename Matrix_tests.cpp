#include "Matrix.hpp"
#include "Matrix_test_helpers.hpp"
#include "unit_test_framework.hpp"

using namespace std;

// Here's a free test for you! Model yours after this one.
// Test functions have no interface and thus no RMEs, but
// add a comment like the one here to say what it is testing.
// -----
// Fills a 3x5 Matrix with a value and checks
// that Matrix_at returns that value for each element.
TEST(test_fill_basic) {
  Matrix mat;
  const int width = 3;
  const int height = 5;
  const int value = 42;
  Matrix_init(&mat, 3, 5);
  Matrix_fill(&mat, value);

  for(int r = 0; r < height; ++r){
    for(int c = 0; c < width; ++c){
      ASSERT_EQUAL(*Matrix_at(&mat, r, c), value);
    }
  }
}

TEST(test_matrix_init_bounds) { // testing differently sized matrixes
  Matrix tinyMatrix; // Make a very small matrix
  Matrix_init(&tinyMatrix, 1, 1);

  Matrix bigMatrix; // Make a very large matrix
  Matrix_init(&bigMatrix, 100, 100);

  ASSERT_EQUAL(Matrix_width(&tinyMatrix), 1); // Correct bounds
  ASSERT_EQUAL(Matrix_height(&tinyMatrix), 1);

  ASSERT_EQUAL(Matrix_width(&bigMatrix), 100); // Correct bounds
  ASSERT_EQUAL(Matrix_height(&bigMatrix), 100);
}

TEST(test_print_base) {
  Matrix norMatrix;
  Matrix_init(&norMatrix, 7, 5);

  for (int i = 0; i < (Matrix_width(&norMatrix) * Matrix_height(&norMatrix)); i++) {
    norMatrix.data[i] = i;
  }

  ostringstream os;
  
  Matrix_print(&norMatrix, os);

  ASSERT_EQUAL(os.str(), "7 5\n" "0 1 2 3 4 5 6 \n" "7 8 9 10 11 12 13 \n" "14 15 16 17 18 19 20 \n" "21 22 23 24 25 26 27 \n" "28 29 30 31 32 33 34 \n");
}

TEST(test_print_single_negative) {
  Matrix singleNeg;
  Matrix_init(&singleNeg, 1, 1);

  singleNeg.data[0] = -32;
  
  ostringstream os;
  
  Matrix_print(&singleNeg, os);

  ASSERT_EQUAL(os.str(), "1 1\n" "-32 \n");
}

TEST(test_height_base) {
  Matrix heightMatrix;
  Matrix_init(&heightMatrix, 2, 1);

  ASSERT_EQUAL(Matrix_height(&heightMatrix), 1);
}

TEST(test_width_base) {
  Matrix widthMatrix;
  Matrix_init(&widthMatrix, 1, 2);

  ASSERT_EQUAL(Matrix_width(&widthMatrix), 1);
}

TEST(test_at_base) {
  Matrix norMatrix;
  Matrix_init(&norMatrix, 4, 3);

  for (int i = 0; i < (Matrix_width(&norMatrix) * Matrix_height(&norMatrix)); i++) {
    norMatrix.data[i] = i;
  }

  for (int i = 0; i < Matrix_height(&norMatrix); i++) {
    for (int j = 0; j < Matrix_width(&norMatrix); j++) {
      ASSERT_EQUAL(*Matrix_at(&norMatrix, i, j), (i * Matrix_width(&norMatrix)) + j);
    }
  }

  const Matrix* constMatrix = &norMatrix;

  for (int i = 0; i < Matrix_height(constMatrix); i++) {
    for (int j = 0; j < Matrix_width(constMatrix); j++) {
      ASSERT_EQUAL(*Matrix_at(constMatrix, i, j), (i * Matrix_width(constMatrix)) + j);
    }
  }
}

TEST(test_at_modifications) {
  Matrix singleMatrix;
  Matrix_init(&singleMatrix, 1, 1);

  singleMatrix.data[0] = 3;

  ASSERT_EQUAL(*Matrix_at(&singleMatrix, 0, 0), 3);
  ASSERT_EQUAL(*Matrix_at(&singleMatrix, 0, 0), 3);
}

TEST(test_fill_stress) {
  Matrix diverseMatrix;
  Matrix_init(&diverseMatrix, 5, 5);

  *Matrix_at(&diverseMatrix, 3, 2) = 487;
  *Matrix_at(&diverseMatrix, 4, 4) = 0;
  *Matrix_at(&diverseMatrix, 0, 0) = -1;

  Matrix_fill(&diverseMatrix, -2);

  for (int i = 0; i < Matrix_height(&diverseMatrix); i++) {
    for (int j = 0; j < Matrix_width(&diverseMatrix); j++) {
      ASSERT_EQUAL(*Matrix_at(&diverseMatrix, i, j), -2);
    }
  }
}

TEST(test_fill_single) {
  Matrix tinyMatrix; // Make a very small matrix
  Matrix_init(&tinyMatrix, 1, 1);

  *Matrix_at(&tinyMatrix, 0, 0) = 1;

  Matrix_fill(&tinyMatrix, 0);

  ASSERT_EQUAL(*Matrix_at(&tinyMatrix, 0, 0), 0);
}

TEST(test_border_fill_negative) {
  Matrix testMatrix;
  Matrix expectedMatrix;

  Matrix_init(&testMatrix, 3, 3);
  Matrix_init(&expectedMatrix, 3, 3);

  Matrix_fill(&testMatrix, -3);
  Matrix_fill_border(&testMatrix, 3);

  Matrix_fill(&expectedMatrix, 3);
  *Matrix_at(&expectedMatrix, 1, 1) = -3;

  ASSERT_TRUE(Matrix_equal(&testMatrix, &expectedMatrix));
}

TEST(test_border_fill_single) {
  Matrix tinyMatrix; // Make a very small matrix
  Matrix_init(&tinyMatrix, 1, 1);

  *Matrix_at(&tinyMatrix, 0, 0) = 1;

  Matrix_fill_border(&tinyMatrix, 0);

  ASSERT_EQUAL(*Matrix_at(&tinyMatrix, 0, 0), 0);
}

TEST(test_max_positioning) {
  Matrix testMatrix1;
  Matrix testMatrix2;

  Matrix_init(&testMatrix1, 3, 3);
  Matrix_init(&testMatrix2, 3, 4);

  Matrix_fill(&testMatrix1, 1);
  Matrix_fill(&testMatrix2, 1);

  *Matrix_at(&testMatrix1, 0, 0) = 99;
  *Matrix_at(&testMatrix2, 3, 2) = 99;

  ASSERT_EQUAL(Matrix_max(&testMatrix1), 99);
  ASSERT_EQUAL(Matrix_max(&testMatrix2), 99);
}

TEST(test_max_equal) {
  Matrix testMatrix;

  Matrix_init(&testMatrix, 3, 3);

  Matrix_fill(&testMatrix, 1);

  ASSERT_EQUAL(Matrix_max(&testMatrix), 1);
}

TEST(test_minimum_column_bounds) {
  Matrix norMatrix1;
  Matrix norMatrix2;

  Matrix_init(&norMatrix1, 4, 3);
  Matrix_init(&norMatrix2, 5, 5);

  for (int i = 0; i < (Matrix_width(&norMatrix1) * Matrix_height(&norMatrix1)); i++) {
    norMatrix1.data[i] = i;
  }

  for (int i = 0; i < (Matrix_width(&norMatrix2) * Matrix_height(&norMatrix2)); i++) {
    norMatrix2.data[i] = i;
  }

  *Matrix_at(&norMatrix1, 2, 2) = -2;
  *Matrix_at(&norMatrix1, 0, 3) = -1;

  *Matrix_at(&norMatrix2, 2, 2) = -1;

  ASSERT_EQUAL(Matrix_column_of_min_value_in_row(&norMatrix1, 0, 0, 4), 3);
  ASSERT_EQUAL(Matrix_column_of_min_value_in_row(&norMatrix2, 0, 0, 1), 0);
}

TEST(test_minimum_column_two) {
  Matrix testMatrix;
  Matrix_init(&testMatrix, 3, 1);

  *Matrix_at(&testMatrix, 0, 0) = 1;
  *Matrix_at(&testMatrix, 0, 1) = 2;
  *Matrix_at(&testMatrix, 0, 2) = 1;

  ASSERT_EQUAL(Matrix_column_of_min_value_in_row(&testMatrix, 0, 0, 3), 0);
}

TEST(test_minimum_column_value_bounds) {
  Matrix norMatrix1;
  Matrix norMatrix2;

  Matrix_init(&norMatrix1, 4, 3);
  Matrix_init(&norMatrix2, 5, 5);

  for (int i = 0; i < (Matrix_width(&norMatrix1) * Matrix_height(&norMatrix1)); i++) {
    norMatrix1.data[i] = i;
  }

  for (int i = 0; i < (Matrix_width(&norMatrix2) * Matrix_height(&norMatrix2)); i++) {
    norMatrix2.data[i] = i;
  }

  *Matrix_at(&norMatrix1, 2, 2) = -2;
  *Matrix_at(&norMatrix1, 0, 3) = -1;

  *Matrix_at(&norMatrix2, 2, 2) = -1;

  ASSERT_EQUAL(Matrix_min_value_in_row(&norMatrix1, 0, 0, 4), -1);
  ASSERT_EQUAL(Matrix_min_value_in_row(&norMatrix2, 0, 0, 1), 0);
}


TEST_MAIN() // Do NOT put a semicolon here
