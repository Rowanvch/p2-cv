#include <cassert>
#include "Image.hpp"
using namespace std;

// REQUIRES: img points to an Image
//           0 < width && 0 < height
// MODIFIES: *img
// EFFECTS:  Initializes the Image with the given width and height, with
//           all pixels initialized to RGB values of 0.
void Image_init(Image* img, int width, int height) {
  if (0 >= width) { // Check if width is invalid, exit if so
    cout << "Width is negative!" << endl;
    assert(false);
  } if (0 >= height) { // Check if height is invalid, exit if so
    cout << "Height is negative!" << endl;
    assert(false);
  } if (img == nullptr) { // Check if img is not formatted correctly
    cout << "That image is not formatted correctly" << endl;
    assert(false);
  }  

  img->width = width; // Initialize width and height
  img->height = height; 

  Matrix_init(&img->red_channel, width, height); // initialize rgb channels
  Matrix_init(&img->green_channel, width, height);
  Matrix_init(&img->blue_channel, width, height);
}

// REQUIRES: img points to an Image
//           is contains an image in PPM format without comments
//           (any kind of whitespace is ok)
// MODIFIES: *img, is
// EFFECTS:  Initializes the Image by reading in an image in PPM format
//           from the given input stream.
// NOTE:     See the project spec for a discussion of PPM format.
void Image_init(Image* img, std::istream& is) {
  string setting; // Initialize and test the P3 setting at the start of the document
  is >> setting;
  if (setting != "P3") {
    cout << "That isn't the standard P3 setting!" << endl;
    assert(false);
  }

  int width; // initialize and test width
  is >> width;
  if (0 >= width) { // Check if width is invalid, exit if so
    cout << "Width is negative!" << endl;
    assert(false);
  } 
  
  int height; // initialize and test height
  is >> height;
  if (0 >= height) { // Check if height is invalid, exit if so
    cout << "Height is negative!" << endl;
    assert(false);
  }

  int intensity; // initialize and test intensity
  is >> intensity;
  if (intensity != MAX_INTENSITY) {
    cout << "Invalid intensity!" << endl;
  }

  Image_init(img, width, height); // prepare img

  int r = 0; //Prepare color variables
  int g = 0;
  int b = 0;

  for (int i = 0; i < height; i++) { // Check that all values are -2
    for (int j = 0; j < width; j++) {
      
      is >> r; // Read each color and check that they are valid
      is >> g;
      is >> b;
      if ((r < 0 || r > intensity) || (g < 0 || g > intensity) || (b < 0 || b > intensity)) {
        cout << "The color data is invalid at (" << i  << ", " << j << ")!"<< endl;
        assert(false);
      }

      *Matrix_at(&img->red_channel, i, j) = r; // set each appropriate pixel object
      *Matrix_at(&img->green_channel, i, j) = g;
      *Matrix_at(&img->blue_channel, i, j) = b;
    }
  }
}

// REQUIRES: img points to a valid Image
// MODIFIES: os
// EFFECTS:  Writes the image to the given output stream in PPM format.
//           You must use the kind of whitespace specified here.
//           First, prints out the header for the image like this:
//             P3 [newline]
//             WIDTH [space] HEIGHT [newline]
//             255 [newline]
//           Next, prints out the rows of the image, each followed by a
//           newline. Each pixel in a row is printed as three ints
//           for its red, green, and blue components, in that order. Each
//           int is followed by a space. This means that there will be an
//           "extra" space at the end of each line. See the project spec
//           for an example.
void Image_print(const Image* img, std::ostream& os) {
  if (img == nullptr) { // Check if img is not formatted correctly
    cout << "That image is not formatted correctly" << endl;
    assert(false);
  }

  os << "P3" << endl; // Print information
  os << img->width << " " << img->height << endl;
  os << MAX_INTENSITY << endl;

  for (int i = 0; i < img->height; i++) { // Loop indexing each pixel and outputing it
    for (int j = 0; j < img->width; j++) {
    os << *Matrix_at(&img->red_channel, i, j) << " ";
    os << *Matrix_at(&img->green_channel, i, j) << " ";
    os << *Matrix_at(&img->blue_channel, i, j) << " ";
    }

    os << endl;
  }
}

// REQUIRES: img points to a valid Image
// EFFECTS:  Returns the width of the Image.
int Image_width(const Image* img) {
  if (img == nullptr) { // Check if img is not formatted correctly
    cout <<"That image is not formatted correctly" << endl;
    assert(false);
  }

  return img->width; // return width
}

// REQUIRES: img points to a valid Image
// EFFECTS:  Returns the height of the Image.
int Image_height(const Image* img) {
  if (img == nullptr) { // Check if img is not formatted correctly
    cout <<"That image is not formatted correctly" << endl;
    assert(false);
  }

  return img->height; // return height
}

// REQUIRES: img points to a valid Image
//           0 <= row && row < Image_height(img)
//           0 <= column && column < Image_width(img)
// EFFECTS:  Returns the pixel in the Image at the given row and column.
Pixel Image_get_pixel(const Image* img, int row, int column) {
  if (img == nullptr) { // Check if img is not formatted correctly
    cout <<"That image is not formatted correctly" << endl;
    assert(false);
  } if (0 > column) { // Check if column is invalid, exit if so
    cout << "That column is negative!" << endl;
    assert(false);
  } if (column >= Image_width(img)) { // Check if column is invalid, exit if so
    cout << "That column is too large!" << endl;
    assert(false);
  } if (0 > row) { // Check if row is invalid, exit if so
    cout << "That row is negative!" << endl;
    assert(false);
  } if (row >= Image_height(img)) { // Check if row is invalid, exit if so
    cout << "That column is too large!" << endl;
    assert(false);
  }

  Pixel target; // Initialize pixel, fill with specified pixel info, and return
  target.r = *Matrix_at(&img->red_channel, row, column);
  target.g = *Matrix_at(&img->green_channel, row, column);
  target.b = *Matrix_at(&img->blue_channel, row, column);
  return target;
}

// REQUIRES: img points to a valid Image
//           0 <= row && row < Image_height(img)
//           0 <= column && column < Image_width(img)
// MODIFIES: *img
// EFFECTS:  Sets the pixel in the Image at the given row and column
//           to the given color.
void Image_set_pixel(Image* img, int row, int column, Pixel color) {
  if (img == nullptr) { // Check if img is not formatted correctly
    cout <<"That image is not formatted correctly" << endl;
    assert(false);
  } if (0 > column) { // Check if column is invalid, exit if so
    cout << "That column is negative!" << endl;
    assert(false);
  } if (column >= Image_width(img)) { // Check if column is invalid, exit if so
    cout << "That column is too large!" << endl;
    assert(false);
  } if (0 > row) { // Check if row is invalid, exit if so
    cout << "That row is negative!" << endl;
    assert(false);
  } if (row >= Image_height(img)) { // Check if row is invalid, exit if so
    cout << "That column is too large!" << endl;
    assert(false);
  }

  *Matrix_at(&img->red_channel, row, column) = color.r; // modify each color of pixel to specified pixel
  *Matrix_at(&img->green_channel, row, column) = color.g;
  *Matrix_at(&img->blue_channel, row, column) = color.b;
}

// REQUIRES: img points to a valid Image
// MODIFIES: *img
// EFFECTS:  Sets each pixel in the image to the given color.
void Image_fill(Image* img, Pixel color) {
  if (img == nullptr) { // Check if img is not formatted correctly
    cout << "That image is not formatted correctly" << endl;
    assert(false);
  }

  Matrix_fill(&img->red_channel, color.r); // fill each matrix channel with appropriate color value
  Matrix_fill(&img->green_channel, color.g);
  Matrix_fill(&img->blue_channel, color.b);
}
