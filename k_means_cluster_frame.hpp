#ifndef K_MEANS_CLUSTER_FRAME
#define K_MEANS_CLUSTER_FRAME

#include "source.hpp"
#include "cluster_frame.hpp"
#include "frame_operations.hpp"

class k_means_cluster_frame : public cluster_frame
{
public:
    k_means_cluster_frame(int new_row, int new_col, int new_cluster_num);
    k_means_cluster_frame();
    ~k_means_cluster_frame();
    //returns true if no cluster centroids moved, false if a centroid moved
    void cluster_single_frame(cv::Mat& frame);
    void paint_clusters(cv::Mat& frame, int radius);
    void paint_clusters(cv::Mat& frame);
    
protected:
    
private:
    int distance_to_cluster(int cluster_index, int row, int col);
    int closest_cluster_index(int row, int col);
    int single_cluster_shift(cv::Mat& frame);//shift being the clusters moving towards the center point of their pixels

    //returns false if the clusters are not moved
};


#endif