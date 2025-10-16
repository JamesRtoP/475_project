#ifndef FRAME_OPERATIONS_HPP
#define FRAME_OPERATIONS_HPP

#include "source.hpp"
double max_rgb_difference(cv::Vec3b p1, cv::Vec3b p2);
double pixel_vector_distance(cv::Vec3b p1, cv::Vec3b p2);
cv::Mat get_next_single_frame_difference(cv::VideoCapture video, double (*difference_formula) (cv::Vec3b p1, cv::Vec3b p2), double threshold);
int pixel_is_black(cv::Vec3b pixel);



#endif