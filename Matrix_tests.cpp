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

TEST(test_print_stress) { // testing a large matrix, with different values for each element to ensure the print result is definitive
  Matrix norMatrix; // Initialize testing matrix with large odd dimensions
  Matrix_init(&norMatrix, 7, 5);

  for (int i = 0; i < (Matrix_width(&norMatrix) * Matrix_height(&norMatrix)); i++) { // Fill matrix with sequential values
    norMatrix.data[i] = i;
  }

  ostringstream os; // Open output
  
  Matrix_print(&norMatrix, os); // Run print function

  ASSERT_EQUAL(os.str(), "7 5\n" "0 1 2 3 4 5 6 \n" "7 8 9 10 11 12 13 \n" "14 15 16 17 18 19 20 \n" "21 22 23 24 25 26 27 \n" "28 29 30 31 32 33 34 \n"); // Check print to expected value
}

TEST(test_print_single_negative) { // test if print can handle stange matrixes like single value matrixes with one value
  Matrix singleNeg; // Initialize testing matrix with single cell
  Matrix_init(&singleNeg, 1, 1);

  singleNeg.data[0] = -32; // Set the value to a negative specific value
  
  ostringstream os; // open output
  
  Matrix_print(&singleNeg, os); // run print function

  ASSERT_EQUAL(os.str(), "1 1\n" "-32 \n"); // Check for expected value
}

TEST(test_height_base) { // test a basic matrix for its height
  Matrix heightMatrix; // Initialize test matrix with target height of 1
  Matrix_init(&heightMatrix, 2, 1);

  ASSERT_EQUAL(Matrix_height(&heightMatrix), 1); // run and check function
}

TEST(test_width_base) { // test a basic matrix for its width
  Matrix widthMatrix; // Initialize test matrix with target width of 1
  Matrix_init(&widthMatrix, 1, 2);

  ASSERT_EQUAL(Matrix_width(&widthMatrix), 1); // run and check function
}

TEST(test_at_stress) { // testing that the at function finds values at all appropriate spots
  Matrix norMatrix; // Initialize test matrix with odd dimensions
  Matrix_init(&norMatrix, 4, 3);

  for (int i = 0; i < (Matrix_width(&norMatrix) * Matrix_height(&norMatrix)); i++) { // fill matrix
    norMatrix.data[i] = i;
  }

  for (int i = 0; i < Matrix_height(&norMatrix); i++) { // Match each element to the function to test
    for (int j = 0; j < Matrix_width(&norMatrix); j++) {
      ASSERT_EQUAL(*Matrix_at(&norMatrix, i, j), (i * Matrix_width(&norMatrix)) + j);
    }
  }

  const Matrix* constMatrix = &norMatrix; // make const reference to test const version

  for (int i = 0; i < Matrix_height(constMatrix); i++) { // Match each element to the function to test
    for (int j = 0; j < Matrix_width(constMatrix); j++) {
      ASSERT_EQUAL(*Matrix_at(constMatrix, i, j), (i * Matrix_width(constMatrix)) + j);
    }
  }
}

TEST(test_at_modifications) { // quick test for any possible modifications as a result of the function
  Matrix singleMatrix; // Initialize test matrix with single value
  Matrix_init(&singleMatrix, 1, 1);

  singleMatrix.data[0] = 3; // initialize specific value

  ASSERT_EQUAL(*Matrix_at(&singleMatrix, 0, 0), 3); // run twice for any changes
  ASSERT_EQUAL(*Matrix_at(&singleMatrix, 0, 0), 3);
}

TEST(test_fill_stress) { // test fill function with some values already there
  Matrix diverseMatrix; // initialize matrix to test
  Matrix_init(&diverseMatrix, 5, 5); 

  *Matrix_at(&diverseMatrix, 3, 2) = 487; // Set some specific values including negative values
  *Matrix_at(&diverseMatrix, 4, 4) = 0;
  *Matrix_at(&diverseMatrix, 0, 0) = -1;

  Matrix_fill(&diverseMatrix, -2); // Run fill

  for (int i = 0; i < Matrix_height(&diverseMatrix); i++) { // Check that all values are -2
    for (int j = 0; j < Matrix_width(&diverseMatrix); j++) {
      ASSERT_EQUAL(*Matrix_at(&diverseMatrix, i, j), -2);
    }
  }
}

TEST(test_fill_single) { // Test filling a small matrix
  Matrix tinyMatrix; // Make a very small matrix
  Matrix_init(&tinyMatrix, 1, 1);

  *Matrix_at(&tinyMatrix, 0, 0) = 1; // Set single value

  Matrix_fill(&tinyMatrix, 0); // run fill

  ASSERT_EQUAL(*Matrix_at(&tinyMatrix, 0, 0), 0); // check fill
}

TEST(test_border_fill_negative) { // Test border fill's capabilities with diverse numbers
  Matrix testMatrix; // Initialize test matrixes
  Matrix expectedMatrix;

  Matrix_init(&testMatrix, 3, 3); 
  Matrix_init(&expectedMatrix, 3, 3);

  Matrix_fill(&testMatrix, -3); // Fill both expected and test 
  Matrix_fill_border(&testMatrix, 3);

  Matrix_fill(&expectedMatrix, 3);
  *Matrix_at(&expectedMatrix, 1, 1) = -3;

  ASSERT_TRUE(Matrix_equal(&testMatrix, &expectedMatrix)); // Check matrix
}

TEST(test_border_fill_single) { // test that the border of a single element matrix is fuctional
  Matrix tinyMatrix; // Make a very small matrix
  Matrix_init(&tinyMatrix, 1, 1);

  *Matrix_at(&tinyMatrix, 0, 0) = 1; // Set single value to 1

  Matrix_fill_border(&tinyMatrix, 0); // Run fill with 0

  ASSERT_EQUAL(*Matrix_at(&tinyMatrix, 0, 0), 0); // Check matrix value to 0
}

TEST(test_max_positioning) { // test that different positioning works for max
  Matrix testMatrix1; // initialize test matrixes
  Matrix testMatrix2;

  Matrix_init(&testMatrix1, 3, 3); // Have different shapes, to make sure all work
  Matrix_init(&testMatrix2, 3, 4);

  Matrix_fill(&testMatrix1, 1); // fill both with 1s
  Matrix_fill(&testMatrix2, 1);

  *Matrix_at(&testMatrix1, 0, 0) = 99; // Set first or last positions to high number
  *Matrix_at(&testMatrix2, 3, 2) = 99;

  ASSERT_EQUAL(Matrix_max(&testMatrix1), 99); // Check matrixes
  ASSERT_EQUAL(Matrix_max(&testMatrix2), 99);
}

TEST(test_max_equal) { // Make sure there is no error in comparing equal numbers
  Matrix testMatrix; // initialize Matrix

  Matrix_init(&testMatrix, 3, 3);

  Matrix_fill(&testMatrix, 1); // fill matrix

  ASSERT_EQUAL(Matrix_max(&testMatrix), 1); // Check function
}

TEST(test_minimum_column_bounds) { // test small and large ranges
  Matrix norMatrix1; // Initialize matrixes
  Matrix norMatrix2;

  Matrix_init(&norMatrix1, 4, 3);
  Matrix_init(&norMatrix2, 5, 5);

  for (int i = 0; i < (Matrix_width(&norMatrix1) * Matrix_height(&norMatrix1)); i++) { // Fill matrixes sequentially
    norMatrix1.data[i] = i;
  }

  for (int i = 0; i < (Matrix_width(&norMatrix2) * Matrix_height(&norMatrix2)); i++) {
    norMatrix2.data[i] = i;
  }

  *Matrix_at(&norMatrix1, 2, 2) = -2; // set low values to check that bounds are correct
  *Matrix_at(&norMatrix1, 0, 3) = -1;

  *Matrix_at(&norMatrix2, 2, 2) = -1;

  ASSERT_EQUAL(Matrix_column_of_min_value_in_row(&norMatrix1, 0, 0, 4), 3); // Check min values
  ASSERT_EQUAL(Matrix_column_of_min_value_in_row(&norMatrix2, 0, 0, 1), 0);
}

TEST(test_minimum_column_two) { // test that left min is preferred
  Matrix testMatrix; // Initialize matrix
  Matrix_init(&testMatrix, 3, 1);

  *Matrix_at(&testMatrix, 0, 0) = 1; // set two small values, one in between
  *Matrix_at(&testMatrix, 0, 1) = 2;
  *Matrix_at(&testMatrix, 0, 2) = 1;

  ASSERT_EQUAL(Matrix_column_of_min_value_in_row(&testMatrix, 0, 0, 3), 0); // Check and run function
}

TEST(test_minimum_column_value_bounds) { // test small and large ranges
  Matrix norMatrix1; // initialize Matrixes
  Matrix norMatrix2;

  Matrix_init(&norMatrix1, 4, 3);
  Matrix_init(&norMatrix2, 5, 5);

  for (int i = 0; i < (Matrix_width(&norMatrix1) * Matrix_height(&norMatrix1)); i++) { // Fill matrixes sequentially
    norMatrix1.data[i] = i;
  }

  for (int i = 0; i < (Matrix_width(&norMatrix2) * Matrix_height(&norMatrix2)); i++) {
    norMatrix2.data[i] = i;
  }

  *Matrix_at(&norMatrix1, 2, 2) = -2; // set low values to check that bounds are correct
  *Matrix_at(&norMatrix1, 0, 3) = -1;

  *Matrix_at(&norMatrix2, 2, 2) = -1;

  ASSERT_EQUAL(Matrix_min_value_in_row(&norMatrix1, 0, 0, 4), -1); // Check and run function
  ASSERT_EQUAL(Matrix_min_value_in_row(&norMatrix2, 0, 0, 1), 0);
}


TEST_MAIN() // Do NOT put a semicolon here
