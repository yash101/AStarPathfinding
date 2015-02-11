//Includes
#include "grid.h"
#include <vector>
#include <sstream>
#include <list>
#include "../../lib/devlib/string.hpp"
#ifndef MAX_ITERATIONS_PATHGEN
#define MAX_ITERATIONS_PATHGEN  16384           //If we hit this, the path generation is halted
#endif

//The code which does the work

std::vector<dev::Vector<size_t> > Grid::Grid::neighbors(dev::Vector<long long> point)
{
    std::vector<dev::Vector<long long> > neigh;
    neigh.push_back(dev::Vector<long long>(point.getX() + 1, point.getY()));
    neigh.push_back(dev::Vector<long long>(point.getX() - 1, point.getY()));
    neigh.push_back(dev::Vector<long long>(point.getX(), point.getY() + 1));
    neigh.push_back(dev::Vector<long long>(point.getX(), point.getY() - 1));

    neigh.push_back(dev::Vector<long long>(point.getX() + 1, point.getY() + 1));
    neigh.push_back(dev::Vector<long long>(point.getX() - 1, point.getY() - 1));
    neigh.push_back(dev::Vector<long long>(point.getX() + 1, point.getY() - 1));
    neigh.push_back(dev::Vector<long long>(point.getX() - 1, point.getY() + 1));

    std::vector<dev::Vector<long long> > neigh_g;
    for(size_t i = 0; i < neigh.size(); i++)
    {
        if(isOnGrid(neigh[i]))
        {
            neigh_g.push_back(neigh[i]);
        }
    }

    std::vector<dev::Vector<size_t> > ret;
    for(size_t i = 0; i < neigh_g.size(); i++)
    {
        ret.push_back(dev::Vector<size_t>((size_t) neigh_g[i].getX(), (size_t) neigh_g[i].getY()));
    }
    return ret;
}

std::vector<dev::Vector<size_t> > Grid::Grid::generatePath(dev::Vector<size_t> start, dev::Vector<size_t> end)
{
    if(!(isOnGrid(start) && isOnGrid(end))) return std::vector<dev::Vector<size_t> >();
    typedef std::vector<dev::Vector<size_t > > veccel;
    typedef dev::Vector<size_t> vecst;
    typedef dev::Vector<float> vecf;

    std::list<Cell> open;       //List of open nodes
    std::list<Cell> closed;     //List of closed nodes

    open.push_back(_grid[start.getX()][start.getY()]);
    open.back().open = true;
    for(size_t watchdog = 0; watchdog < MAX_ITERATIONS_PATHGEN; watchdog++)
    {
        if(dev::Vector<size_t>(open.back().x, open.back().y) == end) break;
        veccel neigh = neighbors(dev::Vector<long long>(open.back().x, open.back().y));
        size_t lowestF = SIZE_T_MAX;
        size_t index_lowestF = 0;
        for(size_t i = 0; i < neigh.size(); i++)
        {
            std::cout << "Angle " << i << ": " << (dev::Vector<float>(neigh[i].getX(), neigh[i].getY()).angle(dev::Vector<float>(open.back().x, open.back().y))) << std::endl;
            _grid[neigh[i].getX()][neigh[i].getY()].G = ((dev::Vector<float>(neigh[i].getX(), neigh[i].getY()).angle(dev::Vector<float>(open.back().x, open.back().y))) / (2 * PI)) * 10;
            _grid[neigh[i].getX()][neigh[i].getY()].H = dev::abs(end.getX() - neigh[i].getX()) + dev::abs(end.getY() - neigh[i].getY());
            if(_grid[neigh[i].getX()][neigh[i].getY()].F() < lowestF)
            {
                lowestF = _grid[neigh[i].getX()][neigh[i].getY()].F();
                index_lowestF = i;
            }
            else
            {
                closed.push_back(_grid[neigh[index_lowestF].getX()][neigh[index_lowestF].getY()]);
            }
        }
        open.push_back(_grid[neigh[index_lowestF].getX()][neigh[index_lowestF].getY()]);
    }
    std::vector<dev::Vector<size_t> > out;
    for(std::list<Cell>::const_iterator it = open.end(); it != open.begin(); --it)
    {
        out.push_back(dev::Vector<size_t>((*it).x, (*it).y));
    }
    out.push_back(end);
    return out;
}


//==================================//
//EXTRAS!

std::string Grid::Grid::print(std::vector<dev::Vector<size_t> > path)
{
    char** out = new char*[_width];
    for(size_t i = 0; i < _width; i++)
    {
        out[i] = new char[_height];
    }
    for(size_t i = 0; i < _width; i++)
    {
        for(size_t j = 0; j < _height; j++)
        {
            out[i][j] = 'X';
        }
    }
    for(size_t i = 0; i < path.size(); i++)
    {
        out[path[i].getX()][path[i].getY()] = dev::toString(i)[0];
    }
    out[path[0].getX()][path[0].getY()] = 'S';
    std::cout << path[path.size()].getX() << ',' << path[path.size()].getY() << std::endl;
    out[path[path.size()].getX()][path[path.size()].getY()] = 'E';
    std::stringstream o;
    for(size_t i = 0; i < _width; i++)
    {
        for(size_t j = 0; j < _height; j++)
        {
            o << out[i][j];
        }
        o << std::endl;
    }
    return o.str();
}


//==================================//
//Important stuff that we aren't worried about atm!

//Constructs the grid. Creates the two-dimentional array of cells.
Grid::Grid::Grid(size_t width, size_t height) : _width(width), _height(height)
{
    //Initialize the grid
    _grid = new Cell*[width];
    for(size_t i = 0; i < _width; i++)
    {
        _grid[i] = new Cell[height];
    }
    for(size_t i = 0; i < _width; i++)
    {
        for(size_t j = 0; j < _height; j++)
        {
            _grid[i][j].x = i;
            _grid[i][j].y = j;
        }
    }
}

//To make things easier for the user, this class uses dynamic memory
//allocation! We must clean up after ourselves. Note that this is
//extremely important because we are storing just a ton of data in this
//two-dimentional pointer array!
Grid::Grid::~Grid()
{
    for(size_t i = 0; i < _width; i++)
    {
        delete[] _grid[i];
    }
    delete[] _grid;
}
//==================================//
