#include <stdio.h>
#include <opencv2/opencv.hpp>
#include <array>
#include <unistd.h>
using namespace cv;

#define INTENSIVITY_THRESHOLD 5

void get_circle(Mat& image)
{
    std::array<std::pair<int, int>, 16> circle = { { { -3, 0 }, { -3, 1 }, { -2, 2 }, { -1, 3}, 
    { 0, 3 }, { 1, 3 }, { 2, 2 }, { 3, 1}, { 3, 0 }, { 3, -1 }, { 2, -2 }, { 1, -3 }, { 0, -3 }, 
    { -1, -3 }, { -2, -2 }, { -3, -1} } };
    for(size_t n = 0;n < 16;n++)
    {
        image.at<uchar>(image.rows/2+circle[n].first,image.cols/2+circle[n].second) = 0;
    }
    return;
}

void find_angles(Mat& image)
{
    std::unique_ptr<std::vector<std::pair<int,int>>> candidates = std::unique_ptr<std::vector<std::pair<int,int>>>(new std::vector<std::pair<int,int>>());
    std::unique_ptr<std::vector<std::pair<int,int>>> marks = std::unique_ptr<std::vector<std::pair<int,int>>>(new std::vector<std::pair<int,int>>());

    for(size_t i = 3;i<image.rows-4;i++)
    {
        for(size_t j = 3;j<image.cols-4;j++)
        {
            int intensity_candidate = (int)image.at<uchar>(i,j);
            int intensities[4] = { (int)image.at<uchar>(i-3,j), (int)image.at<uchar>(i,j+3), (int)image.at<uchar>(i+3,j), (int)image.at<uchar>(i,j-3) };
            size_t counter_more = 0,counter_less = 0;
            for(size_t n = 0;n < 4;n++)
            {
                if(intensities[n]>intensity_candidate+INTENSIVITY_THRESHOLD)
                {
                    counter_more++;
                }
                else if(intensities[n]<intensity_candidate-INTENSIVITY_THRESHOLD)
                {
                    counter_less++;
                }
            }
            if(counter_more >=3 || counter_less >=3)
            {
                candidates->push_back({i,j});
            }
        }
    }

    for(auto& candidate : *candidates)
    {
        int i = candidate.first;
        int j = candidate.second;
        int intensity_candidate = (int)image.at<uchar>(i,j);
        std::array<std::pair<int, int>, 16> circle = { { { -3, 0 }, { -3, 1 }, { -2, 2 }, { -1, 3}, 
        { 0, 3 }, { 1, 3 }, { 2, 2 }, { 3, 1}, { 3, 0 }, { 3, -1 }, { 2, -2 }, { 1, -3 }, { 0, -3 }, 
        { -1, -3 }, { -2, -2 }, { -3, -1} } };
        int intensities[16];
        size_t counter_more = 0;
        size_t counter_less = 0;
        for(size_t n = 0;n < 16;n++)
        {
            intensities[n] = (int)image.at<uchar>(i+circle[n].first,j+circle[n].second);
        }
        bool found_sequence = false;
        for(size_t n = 0;n < 16 && !found_sequence;n++)
        {
            for(size_t m = n;m < n + 16;m++){
                if(intensities[m%16]>intensity_candidate+INTENSIVITY_THRESHOLD)
                {
                    counter_more++;
                    counter_less = 0;
                }
                else if(intensities[m%16]<intensity_candidate-INTENSIVITY_THRESHOLD)
                {
                    counter_less++;
                    counter_more = 0;
                }
                else
                {
                    counter_more = 0;
                    counter_less = 0;
                }
            }
            if(counter_more==12 || counter_less==12)
                {
                    marks->push_back({i,j});
                    found_sequence = true;
                    break;
                }
        }
    
        sleep(0.5);
    }

    for(auto& i : *marks)
    {
        image.at<uchar>(i.first,i.second) = 0;
    }

    return;
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
    //get_circle(gray_image);
    imwrite("after.jpg",gray_image);
    waitKey(0); 
    
    return 0;
}