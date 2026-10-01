#include <cassert>
#include "Matrix.hpp"
using namespace std;

// REQUIRES: mat points to a Matrix
//           0 < width && 0 < height
// MODIFIES: *mat
// EFFECTS:  Initializes *mat as a Matrix with the given width and height,
//           with all elements initialized to 0.
void Matrix_init(Matrix* mat, int width, int height) {
  if (0 >= width) { // Check if width is invalid, exit if so
    cout << "Width is negative!" << endl;
    assert(false);
  } if (0 >= height) { // Check if height is invalid, exit if so
    cout << "Height is negative!" << endl;
    assert(false);
  } if (mat == nullptr) { // Check if mat is not formatted correctly
    cout << "That matrix is not formatted correctly" << endl;
    assert(false);
  }
  
  mat->data = vector<int>(height * width, 0); // Init data
  mat->width = width; // Init width
  mat->height = height; // Init height
}

// REQUIRES: mat points to a valid Matrix
// MODIFIES: os
// EFFECTS:  First, prints the width and height for the Matrix to os:
//             WIDTH [space] HEIGHT [newline]
//           Then prints the rows of the Matrix to os with one row per line.
//           Each element is followed by a space and each row is followed
//           by a newline. This means there will be an "extra" space at
//           the end of each line.
void Matrix_print(const Matrix* mat, std::ostream& os) {
  if (mat == nullptr) { // Check if mat is not formatted correctly
    cout <<"That matrix is not formatted correctly" << endl;
    assert(false);
  }

  os << mat->width << " " << mat->height << endl; // Print WEIGHT and HEIGHT

  for (int i = 0; i < mat->height; i++) { // Loop through each row, printing all elements by width, and ending the line after each row
    for (int j = i * mat->width; j < (mat->width * i) + mat->width; j++) {
      os << mat->data[j] << " ";
    }

    os << endl;
  }
}

// REQUIRES: mat points to a valid Matrix
// EFFECTS:  Returns the width of the Matrix.
int Matrix_width(const Matrix* mat) {
  if (mat == nullptr) { // Check if mat is not formatted correctly
    cout <<"That matrix is not formatted correctly" << endl;
    assert(false);
  }

  return mat->width; // return width
}

// REQUIRES: mat points to a valid Matrix
// EFFECTS:  Returns the height of the Matrix.
int Matrix_height(const Matrix* mat) {
  if (mat == nullptr) { // Check if mat is not formatted correctly
    cout <<"That matrix is not formatted correctly" << endl;
    assert(false);
  }

  return mat->height; // return height
}

// REQUIRES: mat points to a valid Matrix
//           0 <= row && row < Matrix_height(mat)
//           0 <= column && column < Matrix_width(mat)
//
// MODIFIES: (The returned pointer may be used to modify an
//            element in the Matrix.)
// EFFECTS:  Returns a pointer to the element in the Matrix
//           at the given row and column.
int* Matrix_at(Matrix* mat, int row, int column) {
  if (0 > column) { // Check if column is invalid, exit if so
    cout << "That column is negative!" << endl;
    assert(false);
  } if (column >= Matrix_width(mat)) { // Check if column is invalid, exit if so
    cout << "That column is too large!" << endl;
    assert(false);
  } if (0 > row) { // Check if row is invalid, exit if so
    cout << "That row is negative!" << endl;
    assert(false);
  } if (row >= Matrix_height(mat)) { // Check if row is invalid, exit if so
    cout << "That column is too large!" << endl;
    assert(false);
  } if (mat == nullptr) { // Check if mat is not formatted correctly
    cout << "That matrix is not formatted correctly" << endl;
    assert(false);
  }

  return &mat->data[(row * mat->width) + column]; // Fiind desired location
}

// REQUIRES: mat points to a valid Matrix
//           0 <= row && row < Matrix_height(mat)
//           0 <= column && column < Matrix_width(mat)
//
// EFFECTS:  Returns a pointer-to-const to the element in
//           the Matrix at the given row and column.
const int* Matrix_at(const Matrix* mat, int row, int column) {
  if (0 > column) { // Check if column is invalid, exit if so
    cout << "That column is negative!" << endl;
    assert(false);
  } if (column >= Matrix_width(mat)) { // Check if column is invalid, exit if so
    cout << "That column is too large!" << endl;
    assert(false);
  } if (0 > row) { // Check if row is invalid, exit if so
    cout << "That row is negative!" << endl;
    assert(false);
  } if (row >= Matrix_height(mat)) { // Check if row is invalid, exit if so
    cout << "That column is too large!" << endl;
    assert(false);
  } if (mat == nullptr) { // Check if mat is not formatted correctly
    cout << "That matrix is not formatted correctly" << endl;
    assert(false);
  }

  return &mat->data[(row * mat->width) + column]; // Fiind desired location
}

// REQUIRES: mat points to a valid Matrix
// MODIFIES: *mat
// EFFECTS:  Sets each element of the Matrix to the given value.
void Matrix_fill(Matrix* mat, int value) {
  if (mat == nullptr) { // Check if mat is not formatted correctly
    cout << "That matrix is not formatted correctly" << endl;
    assert(false);
  }

  for (int i = 0; i < (mat->height * mat->width); i++) { // Go throught each value in mat and make it the value specified
    mat->data[i] = value;
  }
}

// REQUIRES: mat points to a valid Matrix
// MODIFIES: *mat
// EFFECTS:  Sets each element on the border of the Matrix to
//           the given value. These are all elements in the first/last
//           row or the first/last column.
void Matrix_fill_border(Matrix* mat, int value) {
  if (mat == nullptr) { // Check if mat is not formatted correctly
    cout << "That matrix is not formatted correctly" << endl;
    assert(false);
  }

  for (int i = 0; i < mat->height; i++) { // Loop through each row, and perform adjustments
    if (i != 0 && i != (mat->height - 1)) { // make intermediate rows only get edge elements
      mat->data[i * mat->width] = value; // Set first in row to specified value
      mat->data[(i * mat->width) + mat->width - 1] = value; // Set last in row to speciied value
      continue;
    }
    for (int j = i * mat->width; j < (mat->width * i) + mat->width; j++) { // Modify first and last row
      mat->data[j] = value;
    }
  }
}

// REQUIRES: mat points to a valid Matrix
// EFFECTS:  Returns the value of the maximum element in the Matrix
int Matrix_max(const Matrix* mat) {
  if (mat == nullptr) { // Check if mat is not formatted correctly
    cout << "That matrix is not formatted correctly" << endl;
    assert(false);
  }


  int max = mat->data[0]; //Initializing max value
  for (int i = 1; i < (mat->height * mat->width); i++) { // Go trhrough all values and compare to current max value, then set the new max if it is bigger
    if (mat->data[i] > max) {
      max = mat->data[i];
    }
  }

  return max; // Return max value
}

// REQUIRES: mat points to a valid Matrix
//           0 <= row && row < Matrix_height(mat)
//           0 <= column_start && column_end <= Matrix_width(mat)
//           column_start < column_end
// EFFECTS:  Returns the column of the element with the minimal value
//           in a particular region. The region is defined as elements
//           in the given row and between column_start (inclusive) and
//           column_end (exclusive).
//           If multiple elements are minimal, returns the column of
//           the leftmost one.
int Matrix_column_of_min_value_in_row(const Matrix* mat, int row,
                                      int column_start, int column_end) {
  if (0 > column_start) { // Check if column is invalid, exit if so
    cout << "The starting column is negative!" << endl;
    assert(false);
  } if (column_end > Matrix_width(mat)) { // Check if column is invalid, exit if so
    cout << "The ending column is too large!" << endl;
    assert(false);
  } if (column_start >= column_end) { // Check if column bounds are invalid, exit if so
    cout << "The starting column is bigger than or equal to the ending column!" << endl;
    assert(false);
  } if (0 > row) { // Check if row is invalid, exit if so
    cout << "That row is negative!" << endl;
    assert(false);
  } if (row >= Matrix_height(mat)) { // Check if row is invalid, exit if so
    cout << "That column is too large!" << endl;
    assert(false);
  } if (mat == nullptr) { // Check if mat is not formatted correctly
    cout << "That matrix is not formatted correctly" << endl;
    assert(false);
  }
  int min = mat->data[(row * mat->width) + column_start]; // Initialize min value as first in region
  int minCol = column_start; // initialize target column as first column in region
  for (int i = (row * mat->width) + column_start; i < (row * mat->width) + column_end; i++) { // Check each value in region, and record new mins alongside column number
    if (mat->data[i] < min) {
      min = mat->data[i];
      minCol = i % mat->width;
    }
  } 

  return minCol; // Return final column number
}

// REQUIRES: mat points to a valid Matrix
//           0 <= row && row < Matrix_height(mat)
//           0 <= column_start && column_end <= Matrix_width(mat)
//           column_start < column_end
// EFFECTS:  Returns the minimal value in a particular region. The region
//           is defined as elements in the given row and between
//           column_start (inclusive) and column_end (exclusive).
int Matrix_min_value_in_row(const Matrix* mat, int row,
                            int column_start, int column_end) {
  if (0 > column_start) { // Check if column is invalid, exit if so
    cout << "The starting column is negative!" << endl;
    assert(false);
  } if (column_end > Matrix_width(mat)) { // Check if column is invalid, exit if so
    cout << "The ending column is too large!" << endl;
    assert(false);
  } if (column_start > column_end) { // Check if column bounds are invalid, exit if so
    cout << "The starting column is bigger than the ending column!" << endl;
    assert(false);
  } if (0 > row) { // Check if row is invalid, exit if so
    cout << "That row is negative!" << endl;
    assert(false);
  } if (row >= Matrix_height(mat)) { // Check if row is invalid, exit if so
    cout << "That column is too large!" << endl;
    assert(false);
  } if (mat == nullptr) { // Check if mat is not formatted correctly
    cout << "That matrix is not formatted correctly" << endl;
    assert(false);
  }
  int min = mat->data[(row * mat->width) + column_start]; // Initialize min value as first in region
  for (int i = (row * mat->width) + column_start; i < (row * mat->width) + column_end; i++) { // Check each value in region, and record new min
    if (mat->data[i] < min) {
      min = mat->data[i];
    }
  } 

  return min; // Return final min value
}
