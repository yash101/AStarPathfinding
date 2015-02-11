#include "code/grid/grid.h"
#include <random>
#include <SFML/Graphics.hpp>
#include <opencv2/highgui/highgui.hpp>
int main()
{
    Grid::Grid grid(12, 12);
//    std::cout << grid.print(grid.generatePath(dev::Vector<size_t>(0, 0), dev::Vector<size_t>(4, 4))) << std::endl;
    cv::namedWindow("Window", CV_WINDOW_AUTOSIZE);
    cv::Mat mat(300, 300, CV_8UC3, cv::Scalar(0, 0, 0));

    for(size_t i = 0; i < grid.width(); i++)
    {
        cv::line(mat, cv::Point(i * (300 / grid.width()), 0), cv::Point(i * (300 / grid.width()), 300), cv::Scalar(255, 255, 255), 1, 0);
    }
    for(size_t i = 0; i < grid.height(); i++)
    {
        cv::line(mat, cv::Point(0, i * (300 / grid.width())), cv::Point(300, i * (300 / grid.width())), cv::Scalar(255, 255, 255), 1, 0);
    }

    std::vector<dev::Vector<size_t> > path = grid.generatePath(dev::Vector<size_t>(0, 0), dev::Vector<size_t>(11, 11));

    int k = 0;

    for(size_t i = 1; i < path.size(); i++)
    {
        k++;
        cv::line(mat, cv::Point(path[i].getX() * (300 / grid.width()), path[i - 1].getY() * (300 / grid.height())), cv::Point(path[i - 1].getX() * (300 / grid.width()), path[i].getY() * (300 / grid.height())), cv::Scalar(0, 255, 0), k, 8, 0);
    }

    cv::imshow("Window", mat);
    cv::waitKey(0);
    return 0;
}
