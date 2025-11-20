#include "source.hpp"
#include "frame_operations.hpp"
#include "cluster_operations.hpp"
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
    video >> frame;
    video.set(cv::CAP_PROP_POS_FRAMES,0);

    const int num_clusters = 5;
    Point cluster_array[num_clusters];
    randomly_place_clusters(cluster_array, num_clusters, frame);
    for(int i = 0; i < frame_count; i++)
    {
        //printf("%lf\n", video.get(cv::CAP_PROP_POS_FRAMES));

        frame = get_next_single_frame_difference(video, max_rgb_difference, 20);
        //printf("%lf\n", video.get(cv::CAP_PROP_POS_FRAMES));

        k_means_cluster_single_frame(cluster_array, num_clusters, frame);
        paint_clusters(cluster_array, num_clusters, frame,5);
        clustered_video[i] = frame;
        if(i == 50)
        {
            cv::imwrite("example_1.png", frame);
        }
        if (frame.empty()) {
            std::cout << "Video Processing Complete\n";
            break;
        }
        printf("Processing: %d\n", i);

    }
    
    
    for(int i = 0; i < frame_count-1; i++)
    {
        printf("Showing: %d\n", i);
        // if (clustered_video[i].empty()) {
        //     std::cout << "End of Video" << std::endl;
        //     break;
        // }
        cv::imshow("Video", clustered_video[i]);
        // Wait 30 ms and break if 'q' is pressed
        if (cv::waitKey(1000/fps) == 'q')
        {
            break;
        }
    }
    video.release();
    cv::destroyAllWindows();
    delete[] clustered_video;
    return 14;
    
}