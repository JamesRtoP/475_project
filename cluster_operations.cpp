#include "cluster_operations.hpp"

int distance_to_cluster(Point* cluster_array, int cluster_index, int row, int col)
{
    int x_difference = cluster_array[cluster_index].x - col;
    int y_difference = cluster_array[cluster_index].y - row;
    return sqrt(pow(x_difference,2) + pow(y_difference,2));
}

int closest_cluster_index(Point* cluster_array, int num_clusters, int row, int col)
{
    int closest_index = 0;
    int closest_distance = distance_to_cluster(cluster_array,0,row,col);
    for(int i = 1; i < num_clusters; i++)
    {
        int new_distance = distance_to_cluster(cluster_array,i,row,col);
        if(new_distance < closest_distance)
        {
            closest_index = i;
            closest_distance = new_distance;
        }
    }
    return closest_index;
}


int k_means_single_cluster_shift(Point* cluster_array, int num_clusters,cv::Mat& frame)
{
    int * total_pixels_per_cluster = new int[num_clusters]();
    Point * total_x_and_y_per_cluster = new Point[num_clusters]();
    for(int i = 0; i < frame.rows; i++)
    {
        for(int j = 0; j < frame.cols; j++)
        {
            cv::Vec3b & cur_pixel = frame.at<cv::Vec3b>(i,j);
            if(!pixel_is_black(cur_pixel))
            {
                int index_of_closest_cluster = closest_cluster_index(cluster_array,num_clusters,i,j);
                //this->set_pixel(i,j,index_of_closest_cluster);
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
    for(int i = 0; i < num_clusters; i++)
    {
        if(total_pixels_per_cluster[i] > 1)
        {
            int center_x_pos = total_x_and_y_per_cluster[i].x / total_pixels_per_cluster[i];
            int center_y_pos = total_x_and_y_per_cluster[i].y / total_pixels_per_cluster[i];
            
            if(center_x_pos != cluster_array[i].x || cluster_array[i].y != center_y_pos)
            {
                clusters_centroid_changed++;
            }
            cluster_array[i].x = center_x_pos;
            cluster_array[i].y = center_y_pos;
        }
        
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

//void k_means_place_initial_clusters(Point* cluster_array, int num_clusters, cv::Mat &frame)

void k_means_cluster_single_frame(Point* cluster_array, int num_clusters, cv::Mat &frame)
{
    while(!k_means_single_cluster_shift(cluster_array, num_clusters, frame))
    {

    }
}

void randomly_place_clusters(Point* cluster_array, int num_clusters, cv::Mat &frame)
{
    //frame.rows;
    //frame.cols;
    for(int i = 0; i < num_clusters; i++)
    {
        cluster_array[i].x = rand() %frame.cols;
        cluster_array[i].y = rand() %frame.rows;
    }
}

void paint_clusters(Point* clusters_array, int num_clusters, cv::Mat &frame, int radius)
{
    for(int i = 0; i < num_clusters; i++)
    {
        int minimum_y_value = clusters_array[i].y - radius > 0 ? clusters_array[i].y - radius: 0;
        int minimum_x_value = clusters_array[i].x - radius > 0 ? clusters_array[i].x - radius: 0;
        int maximum_y_value = clusters_array[i].y + radius < frame.rows ? clusters_array[i].y + radius: frame.rows-1;
        int maximum_x_value = clusters_array[i].x + radius < frame.cols ? clusters_array[i].x + radius: frame.cols -1;
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



enum db_point_type
{
    not_a_point, unlabeled_point, noise_point, border_point, core_point
};

typedef struct
{
    int point_type;
    int cluster_num;
}db_point;

db_point db_point_true()
{
    return db_point{unlabeled_point,-1};
}

db_point db_point_false()
{
    return db_point{not_a_point,-1};
}

int squared_difference_between_two_points(Point i1, Point i2)//because sqrt is slow
{
    int dx = i1.x-i2.x;
    int dy = i1.y-i2.y;
    return dx*dx +dy*dy;
}

int element_is_within_radius(Point cur_point, int i, int j, int radius)
{
    int squared_difference = squared_difference_between_two_points(cur_point, Point{j,i});
    return squared_difference < radius*radius;
}



int point_is_core_point(Point cur_point, db_point ** arr, Point max_point, int radius, int minPts)
{
    int left_bound, right_bound, top_bound, bottom_bound;
    left_bound = cur_point.x - radius > 0 ? cur_point.x-radius : 0;
    top_bound = cur_point.y - radius > 0 ? cur_point.y - radius : 0;
    right_bound = cur_point.x + radius < max_point.x ? cur_point.x + radius: max_point.x;
    bottom_bound = cur_point.y + radius < max_point.y ? cur_point.y + radius: max_point.y;
    
    int num_points_within_radius = -1;//negative one because it will count itself
    for(int i = top_bound; i <= bottom_bound; i++)
    {
        for(int j = left_bound; j <= right_bound; j++)
        {
            if(element_is_within_radius(cur_point, i, j, radius) && arr[i][j].point_type != not_a_point)
            {
                num_points_within_radius++;
                if(num_points_within_radius >= minPts)
                {
                    return 1==1;
                }
            }
        }
    }
    return num_points_within_radius >= minPts;
}

int point_is_border_point(Point cur_point, db_point ** arr, Point max_point, int radius, int minPts)
{
    int left_bound, right_bound, top_bound, bottom_bound;
    left_bound = cur_point.x - radius > 0 ? cur_point.x-radius : 0;
    top_bound = cur_point.y - radius > 0 ? cur_point.y - radius : 0;
    right_bound = cur_point.x + radius < max_point.x ? cur_point.x + radius: max_point.x;
    bottom_bound = cur_point.y + radius < max_point.y ? cur_point.y + radius: max_point.y;
    
    int num_points_within_radius = -1;//negative one because it will count itself
    for(int i = top_bound; i <= bottom_bound; i++)
    {
        for(int j = left_bound; j <= right_bound; j++)
        {
            if(element_is_within_radius(cur_point, i, j, radius) && arr[i][j].point_type == core_point)
            {
                return 1==1;
            }
        }
    }
    return 1 == 2;
}

cv::Vec3b set_pixel_color(int i)
{
    i = i % 20; 
    cv::Vec3b return_pixel;
    return_pixel[0] = colors[i][2];
    return_pixel[1] = colors[i][1];
    return_pixel[2] = colors[i][0];
    return return_pixel;
}

void label(int cluster, Point cur_point, db_point ** arr)
{
    arr[cur_point.y][cur_point.x].cluster_num = cluster;
}


void label_neighbors(Point cur_point, db_point ** arr, Point max_point, int radius, int cluster)
{
    int left_bound, right_bound, top_bound, bottom_bound;
    left_bound = cur_point.x - radius > 0 ? cur_point.x-radius : 0;
    top_bound = cur_point.y - radius > 0 ? cur_point.y - radius : 0;
    right_bound = cur_point.x + radius < max_point.x ? cur_point.x + radius: max_point.x;
    bottom_bound = cur_point.y + radius < max_point.y ? cur_point.y + radius: max_point.y;
    
    for(int i = top_bound; i <= bottom_bound; i++)
    {
        for(int j = left_bound; j <= right_bound; j++)
        {
            if(element_is_within_radius(cur_point, i, j, radius) && (arr[i][j].point_type == core_point || arr[i][j].point_type == border_point) && arr[i][j].cluster_num < 0)
            {
                label(cluster, Point {j,i}, arr);
                label_neighbors(Point {j,i}, arr, max_point, radius, cluster);
            }
        }
    }
}

void db_scan(cv::Mat &frame, int radius, int minPts, Point* &cluster_array, int& num_clusters)
{
    // use new dumbass
    db_point ** arr = NULL;
    arr = (db_point**)malloc(frame.rows*sizeof(db_point*));
    Point max_point = {frame.cols-1, frame.rows-1};//the bottom right most pixel//defined so we have a boundry for scanning pixels
    for(int i = 0; i < frame.rows; i++)
    {
        arr[i] = (db_point*)malloc(frame.cols*sizeof(db_point));
    }

    for(int i = 0; i < frame.rows; i++)
    {
        for(int j = 0; j < frame.cols; j++)
        {
            cv::Vec3b & cur_pixel = frame.at<cv::Vec3b>(i,j);
            if(!pixel_is_black(cur_pixel))
            {
                arr[i][j] = db_point_true();
            }
            else
            {
                arr[i][j] = db_point_false();
            }
        }
    }

    for(int i = 0; i < frame.rows; i++)
    {
        for(int j = 0; j < frame.cols; j++)
        {
            if(arr[i][j].point_type == unlabeled_point)
            {
                if(point_is_core_point(Point{j,i},arr,max_point,radius,minPts))
                {
                    arr[i][j].point_type = core_point;
                }
            }
        }
    }
    for(int i = 0; i < frame.rows; i++)
    {
        for(int j = 0; j < frame.cols; j++)
        {
            if(arr[i][j].point_type == unlabeled_point)
            {
                if(point_is_border_point(Point{j,i},arr,max_point,radius,minPts))
                {
                    arr[i][j].point_type = border_point;
                }
                else
                {
                    arr[i][j].point_type = noise_point;
                }
            }
        }
    }

    

    int current_cluster = 0;
    for(int i = 0; i < frame.rows; i++)
    {
        for(int j = 0; j < frame.cols; j++)
        {
            if(arr[i][j].point_type == core_point && arr[i][j].cluster_num < 0)
            {
                label(current_cluster, Point {j,i},arr);
                label_neighbors(Point {j,i}, arr, max_point, radius, current_cluster);
                current_cluster++;
            }
        }
    }

    

    
    int type_coloring = 1 == 2;
    if(type_coloring)
    {
        for(int i = 0; i < frame.rows; i++)
        {
            for(int j = 0; j < frame.cols; j++)
            {
                if(arr[i][j].point_type == core_point)
                {
                    cv::Vec3b & cur_pixel = frame.at<cv::Vec3b>(i,j);
                    cur_pixel = set_pixel_color(0);
                }
                else if (arr[i][j].point_type == border_point)
                {
                    cv::Vec3b & cur_pixel = frame.at<cv::Vec3b>(i,j);
                    cur_pixel = set_pixel_color(1);
                }
                else if (arr[i][j].point_type == noise_point)
                {
                    cv::Vec3b & cur_pixel = frame.at<cv::Vec3b>(i,j);
                    cur_pixel = set_pixel_color(2);
                }
            }
        }
    }
    else if (1 == 2)
    {
        for(int i = 0; i < frame.rows; i++)
        {
            for(int j = 0; j < frame.cols; j++)
            {
                if(arr[i][j].point_type == core_point || arr[i][j].point_type == border_point)
                {
                    frame.at<cv::Vec3b>(i,j) = set_pixel_color(arr[i][j].cluster_num);
                }
                else if (arr[i][j].point_type == noise_point)
                {
                    frame.at<cv::Vec3b>(i,j) = {255,255,255};
                }

            }
        }
    }

    num_clusters = current_cluster+1;
    cluster_array = (Point*) malloc(num_clusters*sizeof(Point));
    for(int i = 0; i < num_clusters; i++)
    {
        cluster_array[i].x = 0;
        cluster_array[i].y = 0;
    }
    int * total_pixels_per_cluster = new int[num_clusters]();
    Point * total_x_and_y_per_cluster = new Point[num_clusters]();
    for(int i = 0; i < frame.rows; i++)
    {
        for(int j = 0; j < frame.cols; j++)
        {
            if(arr[i][j].point_type == core_point || arr[i][j].point_type == border_point)
            {
                //frame.at<cv::Vec3b>(i,j) = set_pixel_color(arr[i][j].cluster_num);
                total_pixels_per_cluster[arr[i][j].cluster_num]++;
                total_x_and_y_per_cluster[arr[i][j].cluster_num].x += j;
                total_x_and_y_per_cluster[arr[i][j].cluster_num].y += i;
            }
        }
    }
    for(int i = 0; i < num_clusters; i++)
    {
        if(total_pixels_per_cluster[i] > 1)
        {
            int center_x_pos = total_x_and_y_per_cluster[i].x / total_pixels_per_cluster[i];
            int center_y_pos = total_x_and_y_per_cluster[i].y / total_pixels_per_cluster[i];
            cluster_array[i].x = center_x_pos;
            cluster_array[i].y = center_y_pos;
        }
        
    }

    
    for(int i = 0; i < frame.rows; i++)
    {
        free(arr[i]);
    }
    free(arr);
}