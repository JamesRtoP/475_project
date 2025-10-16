#include "cluster_frame.hpp"

cluster_frame::cluster_frame()
{
    this->pixel_array = nullptr;
    this->cols = 0;
    this->rows = 0;

    this->num_clusters = 0;
    this->clusters_array = nullptr;
}

cluster_frame::cluster_frame(int new_rows, int new_cols, int new_num_clusters)
{
    this->rows = new_rows;
    this->cols = new_cols;
    this->pixel_array = new short[new_rows*new_cols];
    for(int i = 0; i < this->rows; i++)
    {
        for(int j = 0; j < this->cols; j++)
        {
            this->set_pixel(i,j,-1);
        }
    }

    this->num_clusters = new_num_clusters;
    this->clusters_array = new Point[new_num_clusters];
}

cluster_frame::~cluster_frame()
{
    if(this->pixel_array != nullptr)
    {
        delete this->pixel_array;
    }
    if(this->clusters_array != nullptr)
    {
        delete(this->clusters_array);
    }
}

int cluster_frame::get_pixel(int row, int col)
{
    return this->pixel_array[row*this->rows + col];
}

void cluster_frame::set_pixel(int row, int col, int cluster_num)
{
    this->pixel_array[row*this->rows + col] = cluster_num;
}

int cluster_frame::get_rows()
{
    return this->rows;
}

int cluster_frame::get_cols()
{
    return this->cols;
}
