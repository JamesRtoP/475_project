#ifndef CLUSTER_OPERATIONS_HPP
#define CLUSTER_OPERATIONS_HPP
#include "source.hpp"
#include "frame_operations.hpp"
void k_means_cluster_single_frame(Point* cluster_array, int num_clusters, cv::Mat &frame);
void paint_clusters(Point* clusters_array, int num_clusters, cv::Mat &frame, int radius);
void randomly_place_clusters(Point* cluster_array, int num_clusters, cv::Mat &frame);
void db_scan(cv::Mat &frame, int radius, int minPts);

#endif