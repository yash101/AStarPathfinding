#include "code/grid/grid.h"
#include <random>
#include <SFML/Graphics.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <fstream>
int main()
{
//    std::cout << "Running!" << std::endl;
//    grid::Grid grid(dev::Vector<int>(5, 5), grid::Euclidian);
//    dev::Vector<size_t> start(2, 2);
//    dev::Vector<size_t> end(2, 3);

//    std::vector<dev::Vector<int> > path = grid(start, end);

//    std::ofstream fout("deserialization_dbg.txt");
//    for(size_t i = 0; i < grid.width(); i++)
//    {
//        for(size_t j = 0; j < grid.height(); j++)
//        {
//            fout << "(" << grid[i][j].parent.x() << "," << grid[i][j].parent.y() << ") ";
//        }
//        fout << std::endl;
//    }

//    cv::namedWindow("Window", CV_WINDOW_AUTOSIZE);
//    cv::Mat mat(600, 900, CV_8UC3, cv::Scalar(0, 0, 0));

//    for(size_t i = 0; i < grid.width(); i++)
//    {
//        //horiz
//        cv::line(mat, cv::Point(i * (mat.cols / grid.width())), cv::Point(i * (mat.cols / grid.width()), mat.rows), cv::Scalar(255, 255, 255), 1, 0);
//    }
//    for(size_t i = 0; i < grid.height(); i++)
//    {
//        //vert
//        cv::line(mat, cv::Point(0, i * (mat.rows / grid.height())), cv::Point(mat.cols, i * (mat.rows / grid.height())), cv::Scalar(255, 255, 255));
//    }
//    cv::circle(mat, cv::Point(start.getX() * (mat.cols / grid.width()), start.getY() * (mat.rows / grid.height())), 4, cv::Scalar(0, 0, 255), 4, 8, 0);
//    cv::circle(mat, cv::Point(end.getX() * (mat.cols / grid.width()), end.getY() * (mat.rows / grid.height())), 4, cv::Scalar(255, 0, 0), 4, 8, 0);
//    for(size_t i = 1; i < path.size(); i++)
//    {
//        cv::line(
//            mat,
//            cv::Point(path[i - 1].getX() * (mat.cols / grid.width()), path[i - 1].getY() * (mat.rows / grid.height())),
//            cv::Point(path[i].getX() * (mat.cols / grid.width()), path[i].getY() * (mat.rows / grid.height())),
//            cv::Scalar(0, 255, 0), 1, 8, 0
//        );
//    }

//    cv::imshow("Window", mat);
//    cv::waitKey(0);
    return 0;
}
