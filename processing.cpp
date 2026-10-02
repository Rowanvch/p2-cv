#include <cassert>
#include <vector>
#include <cmath>
#include "processing.hpp"
#include <iostream>

using namespace std;

// v DO NOT CHANGE v ------------------------------------------------
// The implementation of rotate_left is provided for you.
// REQUIRES: img points to a valid Image
// MODIFIES: *img
// EFFECTS:  The image is rotated 90 degrees to the left (counterclockwise).
void rotate_left(Image* img) {

  // for convenience
  int width = Image_width(img);
  int height = Image_height(img);

  // auxiliary image to temporarily store rotated image
  Image aux;
  Image_init(&aux, height, width); // width and height switched

  // iterate through pixels and place each where it goes in temp
  for (int r = 0; r < height; ++r) {
    for (int c = 0; c < width; ++c) {
      Image_set_pixel(&aux, width - 1 - c, r, Image_get_pixel(img, r, c));
    }
  }

  // Copy data back into original
  *img = aux;
}
// ^ DO NOT CHANGE ^ ------------------------------------------------

// v DO NOT CHANGE v ------------------------------------------------
// The implementation of rotate_right is provided for you.
// REQUIRES: img points to a valid Image.
// MODIFIES: *img
// EFFECTS:  The image is rotated 90 degrees to the right (clockwise).
void rotate_right(Image* img){

  // for convenience
  int width = Image_width(img);
  int height = Image_height(img);

  // auxiliary image to temporarily store rotated image
  Image aux;
  Image_init(&aux, height, width); // width and height switched

  // iterate through pixels and place each where it goes in temp
  for (int r = 0; r < height; ++r) {
    for (int c = 0; c < width; ++c) {
      Image_set_pixel(&aux, c, height - 1 - r, Image_get_pixel(img, r, c));
    }
  }

  // Copy data back into original
  *img = aux;
}
// ^ DO NOT CHANGE ^ ------------------------------------------------


// v DO NOT CHANGE v ------------------------------------------------
// The implementation of diff2 is provided for you.
static int squared_difference(Pixel p1, Pixel p2) {
  int dr = p2.r - p1.r;
  int dg = p2.g - p1.g;
  int db = p2.b - p1.b;
  // Divide by 100 is to avoid possible overflows
  // later on in the algorithm.
  return (dr*dr + dg*dg + db*db) / 100;
}
// ^ DO NOT CHANGE ^ ------------------------------------------------


// ------------------------------------------------------------------
// You may change code below this line!



// REQUIRES: img points to a valid Image.
//           energy points to a Matrix.
// MODIFIES: *energy
// EFFECTS:  energy serves as an "output parameter".
//           The Matrix pointed to by energy is initialized to be the same
//           size as the given Image, and then the energy matrix for that
//           image is computed and written into it.
//           See the project spec for details on computing the energy matrix.
void compute_energy_matrix(const Image* img, Matrix* energy) {
  Matrix_init(energy, Image_width(img), Image_height(img)); // Initialize energy

  int h = Matrix_height(energy); // Pull dimensions 
  int w = Matrix_width(energy);


  for (int i = 0; i < h; i++) {
    for (int j = 0; j < w; j++) {
      if (i == 0 || i == h - 1 || j == 0 || j == w - 1) {
        continue;
      }
      Pixel west_pixel = Image_get_pixel(img, i, j - 1); // set the pixels
      Pixel east_pixel = Image_get_pixel(img, i, j + 1);
      Pixel north_pixel = Image_get_pixel(img, i - 1, j);
      Pixel south_pixel = Image_get_pixel(img, i + 1, j);

      *Matrix_at(energy, i, j) = squared_difference(west_pixel, east_pixel) + squared_difference(north_pixel, south_pixel); // Run main energy equation into energy
    
      

    }
  }
  int max = Matrix_max(energy); // find max
  // fix border
  Matrix_fill_border(energy, max);

}


// REQUIRES: energy points to a valid Matrix.
//           cost points to a Matrix.
//           energy and cost aren't pointing to the same Matrix
// MODIFIES: *cost
// EFFECTS:  cost serves as an "output parameter".
//           The Matrix pointed to by cost is initialized to be the same
//           size as the given energy Matrix, and then the cost matrix is
//           computed and written into it.
//           See the project spec for details on computing the cost matrix.
void compute_vertical_cost_matrix(const Matrix* energy, Matrix *cost) {
  Matrix_init(cost, Matrix_width(energy), Matrix_height(energy)); // Initialize cost

  int h = Matrix_height(cost); // Pull dimensions 
  int w = Matrix_width(cost);

  for (int i = 0; i < h; i++) { // cycle all cells to calculate cost
    for (int j = 0; j < w; j++) {
      if (i == 0) { // First row doesn't have cells above to calculate
        *Matrix_at(cost, i, j) = *Matrix_at(energy, i, j);
        continue;
      }

      int s; // initiate index range
      int e;

      if (j == 0) { // count for left and right bounds
        s = 0;
      } else {
        s = j -1;
      }

      if (j > w - 2) {
        e = w;
      } else {
        e = j + 2;
      }

      *Matrix_at(cost, i, j) = *Matrix_at(energy, i, j) + Matrix_min_value_in_row(cost, i -1, s, e); // Sum up costs from all applicable cells and save the value

    }
  }
}


// REQUIRES: cost points to a valid Matrix
// EFFECTS:  Returns the vertical seam with the minimal cost according to the given
//           cost matrix, represented as a vector filled with the column numbers for
//           each pixel along the seam, with index 0 representing the lowest numbered
//           row (top of image). The length of the returned vector is equal to
//           Matrix_height(cost).
//           While determining the seam, if any pixels tie for lowest cost, the
//           leftmost one (i.e. with the lowest column number) is used.
//           See the project spec for details on computing the minimal seam.
//           Note: When implementing the algorithm, compute the seam starting at the
//           bottom row and work your way up.
vector<int> find_minimal_vertical_seam(const Matrix* cost) {
  int h = Matrix_height(cost); // Pull dimensions 
  int w = Matrix_width(cost);

  vector<int> seam(h); // make vector the size of the height of the matrix for one target per row
  int fminCol = Matrix_column_of_min_value_in_row(cost, h - 1, 0, w); // Declare and find first column

  
  seam[h - 1] = fminCol; // Report min column on bottom
  int minCol = fminCol; // Prepare next loop
  int col_start;
  int col_end;

  for (int i = h - 2; i > -1; i--) { // loop through row
    col_start = minCol - 1;
    col_end = minCol + 2;

    if (col_start < 0) { // Mind left and right bounds
      col_start = 0;
    } if (col_end > w) {
      col_end = w;
    }

    minCol = Matrix_column_of_min_value_in_row(cost, i, col_start, col_end);
    seam[i] = minCol; // Find and report results
  }


  return seam; // Final output
}


// REQUIRES: img points to a valid Image with width >= 2
//           seam.size() == Image_height(img)
//           each element x in seam satisfies 0 <= x < Image_width(img)
// MODIFIES: *img
// EFFECTS:  Removes the given vertical seam from the Image. That is, one
//           pixel will be removed from every row in the image. The pixel
//           removed from row r will be the one with column equal to seam[r].
//           The width of the image will be one less than before.
//           See the project spec for details on removing a vertical seam.
// NOTE:     Declare a new variable to hold the smaller Image, and
//           then do an assignment at the end to copy it back into the
//           original image.
void remove_vertical_seam(Image *img, const vector<int> &seam) {
  int h = Image_height(img); // Pull dimensions 
  int w = Image_width(img);

  Image sliced; // Prepare temp image
  Image_init(&sliced, w - 1, h);

  int m;
  for (int i = 0; i < h; i++) {
    m = 0;
    for (int j = 0; j < w; j++) {
      if (j == seam[i]) {
        continue;
      } else {
        Image_set_pixel(&sliced, i, m, Image_get_pixel(img, i, j));
        m++;
      }
    }
  }
  *img = sliced; // return results
}


// REQUIRES: img points to a valid Image
//           0 < newWidth && newWidth <= Image_width(img)
// MODIFIES: *img
// EFFECTS:  Reduces the width of the given Image to be newWidth by using
//           the seam carving algorithm. See the spec for details.
void seam_carve_width(Image *img, int newWidth) {
  
  while (Image_width(img) > newWidth) {
    Matrix energy; // prepare main matrixes
    Matrix cost;

    compute_energy_matrix(img, &energy); // get energy
    compute_vertical_cost_matrix(&energy, &cost); // get cost

    remove_vertical_seam(img, find_minimal_vertical_seam(&cost)); // return out cut pieces
  }
}

// REQUIRES: img points to a valid Image
//           0 < newHeight && newHeight <= Image_height(img)
// MODIFIES: *img
// EFFECTS:  Reduces the height of the given Image to be newHeight.
// NOTE:     This is equivalent to first rotating the Image 90 degrees left,
//           then applying seam_carve_width(img, newHeight), then rotating
//           90 degrees right.
void seam_carve_height(Image *img, int newHeight) {
  rotate_left(img); // run each function in order
  seam_carve_width(img, newHeight);
  rotate_right(img);
}

// REQUIRES: img points to a valid Image
//           0 < newWidth && newWidth <= Image_width(img)
//           0 < newHeight && newHeight <= Image_height(img)
// MODIFIES: *img
// EFFECTS:  Reduces the width and height of the given Image to be newWidth
//           and newHeight, respectively.
// NOTE:     This is equivalent to applying seam_carve_width(img, newWidth)
//           and then applying seam_carve_height(img, newHeight).
void seam_carve(Image *img, int newWidth, int newHeight) {
  seam_carve_width(img, newWidth); // run each function in order
  seam_carve_height(img, newHeight);
}
