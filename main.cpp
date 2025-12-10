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
    cv::Size frameSize(frame.cols, frame.rows);

    int num_clusters = -1;
    Point* cluster_array;//= (Point*) malloc(num_clusters*sizeof(Point));
    //randomly_place_clusters(cluster_array, num_clusters, frame);
    for(int i = 0; i < frame_count; i++)
    {
        
        frame = get_next_single_frame_difference(video, max_rgb_difference, 20);

        //k_means_cluster_single_frame(cluster_array, num_clusters, frame);
        
        db_scan(frame, 2, 5, cluster_array, num_clusters);
        for(int i = 0; i < frame.rows; i++)
        {
            for(int j = 0; j < frame.cols; j++)
            {
                frame.at<cv::Vec3b>(i,j) = {0,0,0};
            }
        }
        paint_clusters(cluster_array, num_clusters, frame,5);
        clustered_video[i] = frame;
        if(i == 50)
        {
            cv::imwrite("processed.png", frame);
        }
        if (frame.empty()) {
            std::cout << "Video Processing Complete\n";
            break;
        }
        printf("Processing: %d\n", i);

    }
    
    
    for(int i = 0; i < 0-1; i++)
    {
        printf("Showing: %d\n", i);
        cv::imshow("Video", clustered_video[i]);
        if (cv::waitKey(1000/fps) == 'q')
        {
            break;
        }
    }
    video.release();

    int fourcc = cv::VideoWriter::fourcc('M', 'J', 'P', 'G');
    cv::VideoWriter writer("k-means.avi", fourcc, fps, frameSize);
    for (int i = 0; i < 100; ++i) {
        writer.write(clustered_video[i]);  // Write frame to the video
    }
    writer.release();
    cv::destroyAllWindows();
    delete[] clustered_video;
    return 14;
    
}