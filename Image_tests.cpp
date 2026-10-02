#include "Matrix.hpp"
#include "Image_test_helpers.hpp"
#include "unit_test_framework.hpp"
#include <iostream>
#include <string>
#include <sstream>
#include <cassert>

using namespace std;

// Here's a free test for you! Model yours after this one.
// Test functions have no interface and thus no RMEs, but
// add a comment like the one here to say what it is testing.
// -----
// Sets various pixels in a 2x2 Image and checks
// that Image_print produces the correct output.
TEST(test_print_basic) {
  Image img;
  const Pixel red = {255, 0, 0};
  const Pixel green = {0, 255, 0};
  const Pixel blue = {0, 0, 255};
  const Pixel white = {255, 255, 255};

  Image_init(&img, 2, 2);
  Image_set_pixel(&img, 0, 0, red);
  Image_set_pixel(&img, 0, 1, green);
  Image_set_pixel(&img, 1, 0, blue);
  Image_set_pixel(&img, 1, 1, white);

  // Capture our output
  ostringstream s;
  Image_print(&img, s);

  // Correct output
  ostringstream correct;
  correct << "P3\n2 2\n255\n";
  correct << "255 0 0 0 255 0 \n";
  correct << "0 0 255 255 255 255 \n";
  ASSERT_EQUAL(s.str(), correct.str());
}

TEST(test_image_init_bounds) { // testing differently sized images
  Image tinyImage; // Make a very small image
  Image_init(&tinyImage, 1, 1);

  Image bigImage; // Make a very large image
  Image_init(&bigImage, 100, 100);

  ASSERT_EQUAL(Image_width(&tinyImage), 1); // Correct bounds
  ASSERT_EQUAL(Image_height(&tinyImage), 1);

  ASSERT_EQUAL(Image_width(&bigImage), 100); // Correct bounds
  ASSERT_EQUAL(Image_height(&bigImage), 100);
}

TEST(test_ppm_init_blank) { // Test how a blank ppm is handled, with margins
  istringstream is("P3\n" "3 2\n" "255\n");

  Image image; // Initialize pixels and main image
  Image_init(&image, is);
  Pixel blank = {0, 0, 0};

  ASSERT_EQUAL(Image_width(&image), 3); // check dimensions
  ASSERT_EQUAL(Image_height(&image), 2); 
  
  for (int i = 0; i < Image_height(&image); i++) { // Loop through all pixels to make sure all are blank
    for (int j = 0; j < Image_width(&image); j++) {
      ASSERT_TRUE(Pixel_equal(Image_get_pixel(&image, i, j), blank));
    }
  }
}

TEST(test_init_blank) { // Test how a blank init works, expecting an empty pixel for each.
  Image image; // Initialize pixels and main image
  Image_init(&image, 3, 4);
  Pixel blank = {0, 0, 0};

  ASSERT_EQUAL(Image_width(&image), 3); // check dimensions
  ASSERT_EQUAL(Image_height(&image), 4); 
  
  for (int i = 0; i < Image_height(&image); i++) { // Loop through all pixels to make sure all are blank
    for (int j = 0; j < Image_width(&image); j++) {
      ASSERT_TRUE(Pixel_equal(Image_get_pixel(&image, i, j), blank));
    }
  }
}

TEST(test_print_stress) { // testing a large image, with different values for each element to ensure the print result is definitive
  Image norImage; // Initialize testing image with large odd dimensions
  Image_init(&norImage, 2, 5);

  Pixel color; // Initialize temporary pixel

  for (int i = 0; i < Image_height(&norImage); i++) { // make proper image
    for (int j = 0; j < Image_width(&norImage); j++) {
      color.r = (Image_width(&norImage) * 3 * i) + (j * 3); // set each appropriate pixel object
      color.g = (Image_width(&norImage) * 3 * i) + (j * 3) + 1;
      color.b = (Image_width(&norImage) * 3 * i) + (j * 3) + 2;

      Image_set_pixel(&norImage, i, j, color);
    }
  }

  ostringstream os; // Open output
  
  Image_print(&norImage, os); // Run print function

  ASSERT_EQUAL(os.str(), "P3\n" "2 5\n" "255\n" "0 1 2 3 4 5 \n" "6 7 8 9 10 11 \n" "12 13 14 15 16 17 \n" "18 19 20 21 22 23 \n" "24 25 26 27 28 29 \n"); // Check print to expected value
}

TEST(test_print_single) { // test if print can handle stange images like single value images with one value
  Image singleSpec; // Initialize testing image with single pixel
  Image_init(&singleSpec, 1, 1);

  Pixel color = {1, 1, 1};
  Image_set_pixel(&singleSpec, 0, 0, color); // Set the value to a specific value
  
  ostringstream os; // open output
  
  Image_print(&singleSpec, os); // run print function

  ASSERT_EQUAL(os.str(), "P3\n" "1 1\n" "255\n" "1 1 1 \n"); // Check for expected value
}

TEST(test_height_base) { // test a basic image for its height
  Image heightImage; // Initialize test image with target height of 1
  Image_init(&heightImage, 2, 1);

  ASSERT_EQUAL(Image_height(&heightImage), 1); // run and check function
}

TEST(test_width_base) { // test a basic image for its width
  Image widthImage; // Initialize test image with target width of 1
  Image_init(&widthImage, 1, 2);

  ASSERT_EQUAL(Image_width(&widthImage), 1); // run and check function
}

TEST(test_get_stress) { // testing that the at function finds values at all appropriate spots
  Image norImage; // Initialize test image with odd dimensions
  Image_init(&norImage, 4, 3);

  Pixel color;

  for (int i = 0; i < Image_height(&norImage); i++) { // make proper image
    for (int j = 0; j < Image_width(&norImage); j++) {
      color.r = (Image_width(&norImage) * 3 * i) + (j * 3); // set each appropriate pixel object
      color.g = (Image_width(&norImage) * i) + (j * 3) + 1;
      color.b = (Image_width(&norImage) * i) + (j * 3) + 2;

      Image_set_pixel(&norImage, i, j, color);
    }
  }

  Pixel target; // Declare target pixel

  for (int i = 0; i < Image_height(&norImage); i++) { // Match each element to the function to test
    for (int j = 0; j < Image_width(&norImage); j++) {
      target.r = (Image_width(&norImage) * 3 * i) + (j * 3); // set each appropriate pixel object
      target.g = (Image_width(&norImage) * i) + (j * 3) + 1;
      target.b = (Image_width(&norImage) * i) + (j * 3) + 2;
      ASSERT_TRUE(Pixel_equal(Image_get_pixel(&norImage, i, j), target)); // test
    }
  }
}

TEST(test_get_modifications) { // quick test for any possible modifications as a result of the function
  Image singleImage; // Initialize test image with single value
  Image_init(&singleImage, 1, 1);

  Pixel color = {1, 1, 1};
  Image_set_pixel(&singleImage, 0, 0, color); // Set the value to a specific value// initialize specific value

  ASSERT_TRUE(Pixel_equal(Image_get_pixel(&singleImage, 0, 0), color)); // run twice for any changes
  ASSERT_TRUE(Pixel_equal(Image_get_pixel(&singleImage, 0, 0), color));
}

TEST(test_fill_single) { // Test filling a small image
  Image tinyImage; // Make a very small image
  Image_init(&tinyImage, 1, 1);

  Pixel color = {1, 1, 1};
  Image_set_pixel(&tinyImage, 0, 0, color); // Set single value

  Pixel fill = {2, 2, 2};
  Image_fill(&tinyImage, fill); // run fill

  ASSERT_TRUE(Pixel_equal(Image_get_pixel(&tinyImage, 0, 0), fill)); // check fill
}

TEST_MAIN() // Do NOT put a semicolon here
