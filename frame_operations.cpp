#include "frame_operations.hpp"

double max_rgb_difference( cv::Vec3b p1, cv::Vec3b p2)
{
    int max = abs(p1[0]-p2[0]);
    int g = abs(p1[1]-p2[1]);
    int b = abs(p1[2]-p2[2]);
    if(g > b)
    {
        if(g > max)
        {
            max = g;
        }
    }
    else
    {
        if(b > max)
        {
            max = b;
        }
    }
    return max;
}

double pixel_vector_distance(cv::Vec3b p1, cv::Vec3b p2)
{
    return sqrt(pow((p1[0] - p2[0]),2) + pow((p1[1] - p2[1]),2) + pow((p1[3] - p2[3]),2));
}


cv::Mat get_next_single_frame_difference(cv::VideoCapture video, double (*difference_formula) (cv::Vec3b p1, cv::Vec3b p2), double threshold)
{
    cv::Mat cur_frame, next_frame;
    video >> cur_frame; //extract 1 frame from video
    int current_frame_num = video.get(cv::CAP_PROP_POS_FRAMES);
    int is_not_last_frame = video.read(next_frame);
    video.set(cv::CAP_PROP_POS_FRAMES, current_frame_num);
    if(!is_not_last_frame)
    {
        return next_frame;
    }
    for(int i = 0; i < cur_frame.rows; i++)
    {
        for(int j = 0; j < cur_frame.cols; j++)
        {
            cv::Vec3b &cur_pix = cur_frame.at<cv::Vec3b>(i,j);
            cv::Vec3b &next_pix = next_frame.at<cv::Vec3b>(i,j);

            if(threshold > difference_formula(cur_pix, next_pix))
            {
                for(int i = 0; i < 3; i++)
                {
                    cur_pix[i] = 0;
                }
            }
        }
    }
    return cur_frame;
    //video.set(cv::CAP_PROP_POS_FRAMES, 0);
}

int pixel_is_black(cv::Vec3b pixel)
{
    if(pixel[0] == 0 && pixel[1] == 0 && pixel[2] == 0)
    {
        return true;
    }
    else
    {
        return false;
    }
}

