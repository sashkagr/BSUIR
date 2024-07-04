#include <opencv2/opencv.hpp>
#include <iostream>

using namespace cv;
using namespace std;

//void clasters() {
//    Mat image = imread("your_image.jpg"); // Загрузите ваше изображение
//
//    if (image.empty()) {
//        cout << "Не удалось загрузить изображение." << endl;
//
//    }
//
//    Mat reshaped_image = image.reshape(1, image.rows * image.cols);
//    reshaped_image.convertTo(reshaped_image, CV_32F);
//
//    int clustersCount = 3; // Количество кластеров (можно изменить)
//
//    Mat bestLabels, centers, clustered;
//    TermCriteria criteria(TermCriteria::EPS + TermCriteria::MAX_ITER, 100, 0.2);
//    kmeans(reshaped_image, clustersCount, bestLabels, criteria, 1, KMEANS_PP_CENTERS, centers);
//
//    centers.convertTo(centers, CV_8U);
//    Mat segmented_image = centers.row(bestLabels).reshape(0, image.rows);
//
//    imshow("Original Image", image);
//    imshow("Segmented Image", segmented_image);
//    waitKey(0);
//
//}

int main() {

    cv::Mat image = cv::imread("C:\\Users\\sashk\\OneDrive\\Pulpit\\bonstic\\sobel\\cmake-build-debug\\1.png", cv::IMREAD_GRAYSCALE);

    if (image.empty()) {
        std::cout << "Could not open or find the image!" << std::endl;
        return -1;
    }

    cv::Mat sobelX = (cv::Mat_<char>(3, 3) << -1, 0, 1,
            -2, 0, 2,
            -1, 0, 1);

    cv::Mat sobelY = (cv::Mat_<char>(3, 3) << -1, -2, -1,
            0,  0,  0,
            1,  2,  1);

    cv::Mat gradientX = cv::Mat::zeros(image.size(), image.type());
    cv::Mat gradientY = cv::Mat::zeros(image.size(), image.type());
    cv::Mat gradientMag = cv::Mat::zeros(image.size(), image.type());

    for (int y = 1; y < image.rows - 1; ++y) {
        for (int x = 1; x < image.cols - 1; ++x) {
            int pixelValX = 0, pixelValY = 0;
            for (int j = -1; j <= 1; ++j) {
                for (int i = -1; i <= 1; ++i) {
                    pixelValX += image.at<uchar>(y + j, x + i) * sobelX.at<char>(j + 1, i + 1);
                    pixelValY += image.at<uchar>(y + j, x + i) * sobelY.at<char>(j + 1, i + 1);
                }
            }
            gradientX.at<uchar>(y, x) = cv::saturate_cast<uchar>(std::abs(pixelValX) / 8);
            gradientY.at<uchar>(y, x) = cv::saturate_cast<uchar>(std::abs(pixelValY) / 8);
            gradientMag.at<uchar>(y, x) = cv::saturate_cast<uchar>(std::sqrt(pixelValX * pixelValX + pixelValY * pixelValY) / 8);
        }
    }


    cv::imshow("Original Image", image);
    cv::imshow("Sobel Edge Detection X direction", gradientX);
    cv::imshow("Sobel Edge Detection Y direction", gradientY);
    cv::imshow("Sobel Edge Detection Magnitude", gradientMag);
    cv::waitKey(0);

    return 0;
}
