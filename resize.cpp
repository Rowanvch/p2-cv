#include <iostream>
#include <fstream>
#include <string>
#include "Image.hpp"
#include "processing.hpp"

using namespace std;

int main(int argc, char* argv[]) {
  cout << "Hello World!\n";

  string ifile = argv[1]; // Pull file names
  string ofile = argv[2];

  ifstream input_file(argv[1]); // open stream

  if (!input_file) {
    cout << "Error opening file: " << ifile << endl;
    return 1;
  }

  Image img; // Open and initiate picture
  Image_init(&img, input_file);

  int newWidth = stoi(argv[3]); // take width

  if (newWidth > Image_width(&img)) { // check if width is too big
    cout << "Usage: resize.exe IN_FILENAME OUT_FILENAME WIDTH [HEIGHT]\n" << "WIDTH and HEIGHT must be less than or equal to original" << endl;
    return 1;
  } if (newWidth <= 0) { // check if width is too small
    cout << "Usage: resize.exe IN_FILENAME OUT_FILENAME WIDTH [HEIGHT]\n" << "WIDTH and HEIGHT must be less than or equal to original" << endl;
    return 1;
  }

  if (argc < 5) { // if height not provided, seam carve only width
    seam_carve_width(&img, newWidth);
  } else if (argc == 5) {
    int newHeight = stoi(argv[4]);

    if (newHeight > Image_height(&img)) { // check if height is too big
      cout << "Usage: resize.exe IN_FILENAME OUT_FILENAME WIDTH [HEIGHT]\n" << "WIDTH and HEIGHT must be less than or equal to original" << endl;
      return 1;
    } if (newHeight <= 0) { // check if height is too small
      cout << "Usage: resize.exe IN_FILENAME OUT_FILENAME WIDTH [HEIGHT]\n" << "WIDTH and HEIGHT must be less than or equal to original" << endl;
      return 1;
    }
    seam_carve(&img, newWidth, newHeight);
  }

  ofstream output_file(ofile); // open output and check if it's opened properly

  if (!output_file) {
    cout << "Error opening file: " << ofile << endl;
    return 1;
  }

  Image_print(&img, output_file); // final print
}