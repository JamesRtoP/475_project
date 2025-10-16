#include "k_means_cluster_frame.hpp"

k_means_cluster_frame::k_means_cluster_frame(int new_row, int new_col, int new_cluster_num):cluster_frame(new_col, new_col,new_cluster_num)
{
    for(int i = 0; i < this->num_clusters; i++)
    {
        this->clusters_array[i].x = rand() % new_col;
        this->clusters_array[i].y = rand() % new_row;
    }
}

k_means_cluster_frame::k_means_cluster_frame() : cluster_frame()
{
    
}

k_means_cluster_frame::~k_means_cluster_frame()
{
    
}

int k_means_cluster_frame::distance_to_cluster(int cluster_index, int row, int col)
{
    int x_difference = this->clusters_array[cluster_index].x - col;
    int y_difference = this->clusters_array[cluster_index].y - row;
    return sqrt(pow(x_difference,2) + pow(y_difference,2));
}

int k_means_cluster_frame::closest_cluster_index(int row, int col)
{
    int closest_index = 0;
    int closest_distance = distance_to_cluster(0,row,col);
    for(int i = 1; i < this->num_clusters; i++)
    {
        int new_distance = distance_to_cluster(i,row,col);
        if(new_distance < closest_distance)
        {
            closest_index = i;
            closest_distance = new_distance;
        }
    }
    return closest_index;
}

int k_means_cluster_frame::single_cluster_shift(cv::Mat& frame)
{
    int * total_pixels_per_cluster = new int[this->num_clusters]();
    Point * total_x_and_y_per_cluster = new Point[this->num_clusters]();
    for(int i = 0; i < frame.rows; i++)
    {
        for(int j = 0; j < frame.cols; j++)
        {
            cv::Vec3b & cur_pixel = frame.at<cv::Vec3b>(i,j);
            if(!pixel_is_black(cur_pixel))
            {
                
                int index_of_closest_cluster = closest_cluster_index(i,j);
                this->set_pixel(i,j,index_of_closest_cluster);
                cur_pixel[0] = colors[index_of_closest_cluster][0];
                cur_pixel[1] = colors[index_of_closest_cluster][1];
                cur_pixel[2] = colors[index_of_closest_cluster][2];

                total_pixels_per_cluster[index_of_closest_cluster]++;
                total_x_and_y_per_cluster[index_of_closest_cluster].x += j;
                total_x_and_y_per_cluster[index_of_closest_cluster].y += i;

            }
        }

    }
    int clusters_centroid_changed = 0;
    for(int i = 0; i < this->num_clusters; i++)
    {
        int center_x_pos = total_x_and_y_per_cluster[i].x / total_pixels_per_cluster[i];
        int center_y_pos = total_x_and_y_per_cluster[i].y / total_pixels_per_cluster[i];
        
        if(center_x_pos != this->clusters_array[i].x ||this->clusters_array[i].y != center_y_pos)
        {
            clusters_centroid_changed++;
        }
        this->clusters_array[i].x = center_x_pos;
        this->clusters_array[i].y = center_y_pos;
    }
    if(clusters_centroid_changed == 0)
    {
        return 1 == 1;
    }
    else
    {
        return 1 == 2;
    }
    
}

void k_means_cluster_frame::cluster_single_frame(cv::Mat &frame)
{
    while(!this->single_cluster_shift(frame))
    {

    }
}

void k_means_cluster_frame::paint_clusters(cv::Mat &frame, int radius)
{
    for(int i = 0; i < this->num_clusters; i++)
    {
        int minimum_y_value = this->clusters_array[i].y - radius > 0 ? this->clusters_array[i].y - radius: 0;
        int minimum_x_value = this->clusters_array[i].x - radius > 0 ? this->clusters_array[i].x - radius: 0;
        int maximum_y_value = this->clusters_array[i].y + radius < frame.rows ? this->clusters_array[i].y + radius: frame.rows-1;
        int maximum_x_value = this->clusters_array[i].x + radius < frame.cols ? this->clusters_array[i].x + radius: frame.cols -1;
        for(int row = minimum_y_value; row < maximum_y_value; row++)
        {
            for(int col = minimum_x_value; col < maximum_x_value; col++)
            {
                frame.at<cv::Vec3b>(row,col)[0] = 255;
                frame.at<cv::Vec3b>(row,col)[1] = 255;
                frame.at<cv::Vec3b>(row,col)[2] = 255;

            }
        }
    }
}

void k_means_cluster_frame::paint_clusters(cv::Mat &frame)
{
    for(int i = 0; i < this->num_clusters; i++)
    {
        cv::Vec3b & cur_pixel = frame.at<cv::Vec3b>(this->clusters_array[i].y,this->clusters_array[i].x);
        cur_pixel[0] = 255;
        cur_pixel[1] = 255;
        cur_pixel[2] = 255;
    }
}
