/*
 * vecMatricExample.cpp
 *
 * These are the values for the input matrix:
 * 0.707107 0 -0.707107 0
 * -0.331295 0.883452 -0.331295 0
 * 0.624695 0.468521 0.624695 0
 * 4.000574 3.00043 4.000574 1
 *
 * Given the input matrix, the inverse matrix computed by our code should match
 * the following values: 0.707107 -0.331295 0.624695 0 0 0.883452 0.468521 0
 * -0.707107 -0.331295 0.624695
 *  0 0 0 -6.404043 1
 */
// Copyright (C) 2012  www.scratchapixel.com
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.

#include <iostream>

#include "core/utils.h"
#include "math/geometry.h"
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <iostream>

Vec3f check() {
  Vec3f a;
  a[0] = 8;
  std::cout << a << " ";
  return a;
}
int main(int argc, char **argv) {

  // define vector
  Vec3f v(0, 1, 2);

  std::cerr << "Example vector:" << v << std::endl;
  v.normalize();
  std::cerr << v << std::endl;

  Matrix44f a, b, c;
  c = a * b;

  Matrix44f d(0.707107, 0, -0.707107, 0, -0.331295, 0.883452, -0.331295, 0,
              0.624695, 0.468521, 0.624695, 0, 4.000574, 3.00043, 4.000574, 1);

  std::cerr << "Example matrix:" << std::endl;
  std::cerr << d << std::endl;
  d.invert();
  std::cerr << "Inverse of above matrix:" << std::endl;
  std::cerr << d << std::endl;

  std::string image_path = cv::samples::findFile("../textures/sicily.jpg");
  cv::Mat img = cv::imread(image_path, cv::IMREAD_COLOR);
  if (img.empty()) {
    std::cout << "Could not read the image: " << image_path << std::endl;
    return 1;
  }
  int b_ = img.at<cv::Vec3b>(10, 29)[0];
  int g_ = img.at<cv::Vec3b>(10, 29)[1];
  int r_ = img.at<cv::Vec3b>(10, 29)[2];
  std::cout << b_ << " " << g_ << " asd" << r_ << std::endl;
  std::cout << image_path << std::endl;

  return 0;
}
