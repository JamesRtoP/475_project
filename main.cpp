#include "source.hpp"
#include "frame_operations.hpp"
#include "k_means_cluster_frame.hpp"
int main(void)
{
    cv::VideoCapture video("soccer.mp4");
    if (!video.isOpened()) 
    {
        std::cout << "Error: Camera not opened" << std::endl;
        return -1;
    }
    double fps = video.get(cv::CAP_PROP_FPS);
    int frame_count = video.get(cv::CAP_PROP_FRAME_COUNT);
    cv::Mat* clustered_video = new cv::Mat[frame_count];

    cv::Mat frame;
    k_means_cluster_frame clusterer(video.get(cv::CAP_PROP_FRAME_HEIGHT),video.get(cv::CAP_PROP_FRAME_WIDTH),5);
    for(int i = 0; i < frame_count; i++)
    {
        frame = get_next_single_frame_difference(video, max_rgb_difference, 20);
        if (frame.empty()) {
            std::cout << "End of Video" << std::endl;
            break;
        }
        clusterer.cluster_single_frame(frame);
        clusterer.paint_clusters(frame,5);
        clustered_video[i] = frame;
    }
    
    
    for(int i = 0; i < frame_count; i++)
    {
        if (clustered_video[i].empty()) {
            std::cout << "End of Video" << std::endl;
            break;
        }
        cv::imshow("Video", clustered_video[i]);
        // Wait 30 ms and break if 'q' is pressed
        if (cv::waitKey(100/fps) == 'q')
        {
            break;
        }
    }
    video.release();
    cv::destroyAllWindows();
    return 14;
    
}