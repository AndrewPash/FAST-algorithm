#include <stdio.h>
#include <opencv2/opencv.hpp>
#include <array>
using namespace cv;


void get_circle(Mat& image)
{
    std::array<std::pair<int, int>, 16> circle = { { { -3, 0 }, { -3, 1 }, { -2, 2 }, { -1, 3}, 
                { 0, 3 }, { 1, 3 }, { 2, 2 }, { 3, 1}, { 3, 0 }, { 3, -1 }, { 2, -2 }, { 1, -3 }, { 0, -3 }, 
                { -1, -3 }, { -2, -2 }, { -1, -3} } };

    for(size_t n = 0;n < 16;n++)
    {
        image.at<uchar>(image.rows/2+circle[n].second,image.cols/2+circle[n].first) = 255;
    }
    return;

}

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
                if(intensities[n]>intensity_candidate+30)
                {
                    counter_more++;
                }
                else if(intensities[n]<intensity_candidate-30)
                {
                    counter_less++;
                }
            }
            
            if(counter_more>=12 || counter_less>=12)
            {
                image.at<uchar>(j,i) = 254;
            }
        }
    }
    return 0;
}

int main(int argc,char** argv)
{
    std::string filename = "image.jpg";
    Mat image,gray_image,black_image; 
    image = imread(filename.c_str(), 1);
    black_image = imread("black.jpg",1);
    cvtColor(black_image, black_image, cv::COLOR_BGR2GRAY); 
    //get_circle(black_image);
    imwrite("test.jpg", black_image); 
    imshow("Display Image Before", black_image);
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