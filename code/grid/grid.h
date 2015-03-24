#ifndef GRID_H
#define GRID_H
#include <stddef.h>
#include <vector>
#include <queue>
#include "../../lib/devlib/math.hpp"
#include "../../lib/devlib/vector.hpp"
#include <map>
namespace grid
{
    class AStarGrid;
    class Cell;

    enum Heuristic
    {
        Manhattan,
        Euclidian,
        EuclidianSquared
    };

    template<typename T, typename DT> inline T heuristicf(dev::Vector<T> major, dev::Vector<T> minor, DT D, Heuristic type)
    {
        if(type == Manhattan)
        {
            return D * (dev::abs(major.getX() - minor.getX()) + dev::abs(major.getY() - minor.getY()));
        }
        else if(type == Euclidian)
        {
            return D * std::sqrtf(dev::pow(dev::abs(major.x() - minor.x()), 2) + (dev::pow(dev::abs(major.y() - minor.y()), 2)));
        }
        else if(type == EuclidianSquared)
        {
            return D * dev::pow(dev::abs(major.x() - minor.x()), 2) + (dev::pow(dev::abs(major.y() - minor.y()), 2));
        }
        return (T) -1.f;
    }

    template<typename T> inline bool locatev(std::vector<T>& st, T item)
    {
        for(size_t i = 0; i < st.size(); i++) if(st[i] == item) return true;
        return false;
    }

    class AStarGrid
    {
    private:
        Cell** _cells;
        dev::Vector<size_t> _grid_sz;
    public:
        AStarGrid(size_t width, size_t height);
        ~AStarGrid();

        dev::Vector<long>& start() { return _start; }
        dev::Vector<long>& end() { return _end; }
        dev::Vector<size_t> size() const { return _grid_sz; }
        size_t width() const { return _grid_sz.getX(); }
        size_t height() const { return _grid_sz.getY(); }

        template<typename T> Cell& operator[](dev::Vector<T> loc) { return _cells[loc.x()][loc.y()]; }
        template<typename T> Cell* operator[](T loc) { return _cells[loc]; }
    };

    class Cell
    {
    private:
        dev::Vector<long> _position;
        dev::Vector<long> _parent;
        AStarGrid* _parent_grid;
    public:
        Cell(AStarGrid* grid) :
            _position(dev::Vector<long>(-1, -1)), _parent(dev::Vector<long>(-1, -1)), _parent_grid(grid) {}
        Cell() : _position(dev::Vector<long>(-1, -1)), _parent(dev::Vector<long>(-1, -1)), _parent_grid(NULL) {}

        dev::Vector<long>& position() { return _position; }
        dev::Vector<long>& parent() { return _parent; }
        long& x() { return _position.x(); }
        long& y() { return _position.y(); }
        long& parentX() { return _parent.x(); }
        long& parentY() { return _parent.y(); }
    };

    class AStarPath
    {
    private:
        std::priority_queue<dev::Vector<int> > _frontier;

    }

//    struct Cell
//    {
//    private:
//        float _G;        //Movement Cost
//        float _H;        //Estimated Movement Cost
//        float _F;
//        bool _blocked;   //Is the cell blocked?
//    public:
//        bool& blocked() { return _blocked; }
//        float& F() { return _F; }
//        float FCalc() { _F = _G + _H; return _F; }
//        float& G() { return _G; }
//        float& H() { return _H; }
//        dev::Vector<int> parent;
//        dev::Vector<int> location;
//        Cell() : _blocked(false), parent(-1, -1) {}
//        bool operator()() { return parent.x() < 0 || parent.y() < 0; }
//    };
//    class Grid
//    {
//    private:
//        std::vector<std::vector<Cell> > _grid;
//        dev::Vector<int> _size;
//        Heuristic _heuristic;
//    public:
//        Grid(dev::Vector<int> size, Heuristic heuristic) :
//            _grid(size.y(), std::vector<Cell>(size.x())),
//            _size(size),
//            _heuristic(heuristic)
//        {}

//        dev::Vector<int> size() const { return _size; }
//        std::vector<Cell> operator[](int location) { return _grid[location]; }
//        Cell operator[](dev::Vector<int> location) { return _grid[location.getX()][location.getY()]; }
//        Heuristic& heuristic() { return _heuristic; }
//        int width() const { return _size.getX(); }
//        int height() const { return _size.getY(); }

//        std::vector<dev::Vector<int> > getNeighbors(dev::Vector<int> pt);

//        Cell compute(dev::Vector<int> start, dev::Vector<int> end, dev::Vector<int> position, float D);

//        //Is a position on the grid good?
//        bool valid(dev::Vector<int> pos)
//        { return (pos.x() >= 0 && pos.x() < _size.x() && pos.y() >= 0 && pos.y() < _size.y()) && !(*this)[pos].blocked(); }

//        std::vector<dev::Vector<int> > operator()(dev::Vector<int> start, dev::Vector<int> end) { return path(start, end); }
//        std::vector<dev::Vector<int> > path(dev::Vector<int> start, dev::Vector<int> end);
//    };

////    class Grid
////    {
////    private:
////        size_t _width;
////        size_t _height;
////        Cell** _grid;       //(X, Y)
////        Heuristic _heuristic;
////    public:
////        Grid(size_t width, size_t height, Heuristic heuristic);
////        ~Grid();

////        size_t width() { return _width; }
////        size_t height() { return _height; }

////        bool isOnGrid(dev::Vector<long long> cell) { return cell.getX() >= 0 && cell.getX() < _width && cell.getY() >= 0 && cell.getY() < _height; }
////        std::vector<dev::Vector<size_t> > neighbors(dev::Vector<size_t> parent);
////        std::vector<dev::Vector<size_t> > generatePath(dev::Vector<size_t> start, dev::Vector<size_t> end);



////        Cell& operator[](dev::Vector<size_t> index) { return _grid[index.getX()][index.getY()]; }
////        std::string print(std::vector<dev::Vector<size_t> > path);
////    };
}
#endif // GRID_H
