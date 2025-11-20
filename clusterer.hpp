#ifndef CLUSTERER
#define CLUSTERER
#include "source.hpp"
#include "frame_operations.hpp"

class Clusterer
{
public:
    Clusterer();
    virtual ~Clusterer();

    int get_pixel(int row, int col);

    void cluster_single_frame(cv::Mat& frame);
    void paint_clusters(cv::Mat& frame, int radius);
    void paint_clusters(cv::Mat& frame);

protected:
    int num_clusters;
    Point* clusters_array;
    void set_pixel(int row, int col, int cluster_num);
    int get_rows();
    int get_cols();
private:
    short *pixel_array;//array of pixels for clustering, stores an int based on which cluster it is part of
    int rows;
    int cols;

    
    int distance_to_cluster(int cluster_index, int row, int col);
    int closest_cluster_index(int row, int col);
    int single_cluster_shift(cv::Mat& frame);//shift being the clusters moving towards the center point of their pixels
};



#endif