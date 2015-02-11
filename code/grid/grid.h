#ifndef GRID_H
#define GRID_H
#include <stddef.h>
#include <vector>
#include "../../lib/devlib/math.hpp"
#include "../../lib/devlib/vector.hpp"
namespace Grid
{
    struct Cell
    {
    public:
        bool blocked;   //Is the cell blocked?
        bool open;      //Is the cell open?
        float G;        //Movement Cost
        float H;        //Estimated Movement Cost
        float F() { return G + H; }
        size_t x;
        size_t y;
        Cell() :
            blocked(false),
            G(0.f),
            H(0.f),
            x(0),
            y(0)
        {}
        bool operator()() { return open; }
    };

    class Grid
    {
    private:
        size_t _width;
        size_t _height;
        Cell** _grid;       //(X, Y)
    public:
        Grid(size_t width, size_t height);
        ~Grid();

        size_t width() { return _width; }
        size_t height() { return _height; }

        bool isOnGrid(dev::Vector<long long> pt) { return pt.getX() >= 0 && pt.getX() < _width && pt.getY() >= 0 && pt.getX() < _height; }
        std::vector<dev::Vector<size_t> > neighbors(dev::Vector<long long> point);
        std::vector<dev::Vector<size_t> > generatePath(dev::Vector<size_t> start, dev::Vector<size_t> end);
        Cell* operator[](size_t location) { return _grid[location]; }
        std::string print(std::vector<dev::Vector<size_t> > path);
    };
}
#endif // GRID_H
