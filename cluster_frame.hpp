#ifndef CLUSTER_FRAME_HPP
#define CLUSTER_FRAME_HPP
#include "source.hpp"
class cluster_frame
{
public:
    cluster_frame();
    cluster_frame(int new_rows, int new_cols, int num_clusters);
    virtual ~cluster_frame();

    int get_pixel(int row, int col);
    virtual void cluster_single_frame(cv::Mat& frame) = 0;


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
};



#endif