#include <stdio.h>
#include <opencv2/opencv.hpp>
#include <array>
using namespace cv;

#define INTENSIVITY_THRESHOLD 30

int find_angles(Mat& image)
{
    for(size_t i = 3;i<image.rows-4;i++)
    {
        for(size_t j = 3;j<image.cols-4;j++)
        {
            int intensity_candidate = (int)image.at<uchar>(j,i);
            std::array<std::pair<int, int>, 16> circle = { { { -3, 0 }, { -3, 1 }, { -2, 2 }, { -1, 3}, 
            { 0, 3 }, { 1, 3 }, { 2, 2 }, { 3, 1}, { 3, 0 }, { 3, -1 }, { 2, -2 }, { 1, -3 }, { 0, -3 }, 
            { -1, -3 }, { -2, -2 }, { -1, -3} } };
            int intensities[16];
            size_t counter_more = 0;
            size_t counter_less = 0;
            for(size_t n = 0;n < 16;n++)
            {
                intensities[n] = (int)image.at<uchar>(j+circle[n].second,i+circle[n].first);
                if(intensities[n]>intensity_candidate+INTENSIVITY_THRESHOLD)
                {
                    counter_more++;
                }
                else if(intensities[n]<intensity_candidate-INTENSIVITY_THRESHOLD)
                {
                    counter_less++;
                }
            }
            
            if(counter_more>=12 || counter_less>=12)
            {
                image.at<uchar>(j,i) = 255;
            }
        }
    }
    return 0;
}

int main(int argc,char** argv)
{
    if(argc<2){
        std::cout << "USAGE:./angle_detector image.jpg\n";
        return 0;
    }
    Mat image,gray_image; 
    image = imread(argv[1], 1);
    cvtColor(image, gray_image, cv::COLOR_BGR2GRAY); 
    if (!image.data) { 
        printf("No image data \n"); 
        return -1; 
    }
    imwrite("before.jpg", gray_image); 
    find_angles(gray_image);
    imwrite("after.jpg",gray_image);
    waitKey(0); 
    
    return 0;
}