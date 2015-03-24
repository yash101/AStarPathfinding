#include "grid.h"
#define Me (*this)
#define success true
#define failure false
typedef grid::Cell Cell;
typedef dev::Vector<long> point;

//Save us a bit of typing!
using std::vector;
using dev::Vector;
using dev::abs;
using dev::Distance;
using dev::modulus;
using dev::pow;
using dev::PythagoreanHypotenuse;



//Perform dynamic allocation and get us some RAM. This populates the grid!
grid::AStarGrid::AStarGrid(size_t width, size_t height) :
    _grid_sz(width, height),
    _start(0, 0), _end(0, 0)
{
    _cells = new Cell*[width];
    for(size_t i = 0; i < width; i++)
    {
        _cells[i] = new Cell[height];
    }
}

//Clean up. Dynamic memory allocation without smart classes is very messy
grid::AStarGrid::~AStarGrid()
{
    for(size_t i = 0; i < Me.width(); i++)
    {
        delete[] _cells[i];
    }
    delete[] _cells;
}

//=========================================================================================//
//std::vector<point> grid::Grid::path(point start, point end)
//{
//    std::vector<point> open;
//    std::vector<point> closed;
//    open.push_back(start);
//    Me[start].G() = 0.f;
//    Me[start].H() = heuristicf(end, start, 1.f, Me.heuristic());
//    //Probably will crash the program but who cares lol :D
//    if(!valid(start) || !valid(end)) return (*(std::vector<point>*) NULL);
//    while(!open.empty())
//    {
//        std::cout << ".";
//        point current;
//        float l_F = -1;
//        int l_i = INT_MIN;
//        for(size_t i = 0; i < open.size(); i++)
//        {
//            float m_F = heuristicf(start, open[i], 1.0, Me.heuristic()) + heuristicf(end, open[i], 1.0, Me.heuristic());
//            if(m_F < l_F || l_i < 0)
//            {
//                l_i = i;
//                l_F = m_F;
//            }
//        }
//        current = open[l_i];

//        //Trace back the path if we have reached the end
//        if(current == end)
//        {
//            std::cout << "Generating Path!" << std::endl;
//            std::vector<point> path;
//            point tmp = end;
//            while(tmp != start && !Me[tmp]())
//            {
//                std::cout << "*";
//                path.push_back(tmp);
//                tmp = Me[tmp].parent;
//            }
//        }
//        open.erase(open.begin() + l_i);
//        closed.push_back(current);
//        std::vector<point> neighbors = getNeighbors(current);
//        for(size_t i = 0; i < neighbors.size(); i++)
//        {
//            if(vecFind(closed, neighbors[i])) continue;
//            float tentative_g = heuristicf(neighbors[i], start, 1.0, Me.heuristic());
//            tentative_g += std::sqrtf(dev::pow(current.x() - neighbors[i].x(), 2) + dev::pow(current.y() - neighbors[i].y(), 2));
//            if(!vecFind(open, neighbors[i]) || tentative_g < heuristicf(neighbors[i], start, 1.0, _heuristic))
//            {
//                if(!vecFind(open, neighbors[i])) open.push_back(neighbors[i]);
//                Me[neighbors[i]].parent = current;
//            }
//        }
//    }
//    return std::vector<point>(0);
//}

////std::vector<dev::Vector<int> > grid::Grid::path(dev::Vector<int> start, dev::Vector<int> end)
////{
////    std::vector<dev::Vector<int> > open, closed;
////    open.push_back(start);
////    for(size_t watchdog = 0; watchdog < 96; watchdog++)
////    {
////        float lowestN = -1;
////        size_t lowestI = 0;
////        //Find the lowest F value
////        for(size_t i = 0; i < open.size(); i++)
////        {
////            compute(start, end, open[i], 1.f);
////            float F = Me[open[i]].F();
////            if(lowestN < 0.f || F < lowestN)
////            {
////                lowestN = F;
////                lowestI = i;
////            }
////        }
////        //Save a copy of the good node
////        dev::Vector<int> current(open[lowestI].x(), open[lowestI].y());
////        //Erase the good node from the open vector
////        open.erase(open.begin() + lowestI);
////        //Check and see if we have reached our goal
////        if(current == end) break;
////        closed.push_back(current);

////        std::vector<dev::Vector<int> > neighbors = getNeighbors(current);
////        for(size_t i = 0; i < neighbors.size(); i++)
////        {
////            float mcost = (dev::pow(current.x() - neighbors[i].x(), 2) + dev::pow(current.y() - neighbors[i].y(), 2));
////            float cost = Me[current].G() + mcost;
////            if(vecFind(open, neighbors[i]) && cost < Me[neighbors[i]].G())
////            {
////                for(size_t j = 0; j < open.size(); j++)
////                {
////                    if(open[j] == neighbors[i])
////                    {
////                        open.erase(open.begin() + j);
////                        break;
////                    }
////                }
////                continue;
////            }
////            if(vecFind(closed, neighbors[i]) && cost < Me[neighbors[i]].G())
////            {
////                for(size_t j = 0; j < closed.size(); j++)
////                {
////                    if(closed[j] == neighbors[i])
////                    {
////                        closed.erase(closed.begin() + j);
////                        break;
////                    }
////                }
////                continue;
////            }
////            if(!vecFind(closed, neighbors[i]) && !vecFind(open, neighbors[i]))
////            {
////                Me[neighbors[i]].G() = cost;
////                open.push_back(neighbors[i]);
////                Me[neighbors[i]].parent = current;
////            }
////        }

////    }
////    std::vector<dev::Vector<int> > path;
////    dev::Vector<int> pcur = end;
////    while(pcur != start)
////    {
////        path.push_back(pcur);
////        pcur = Me[pcur].parent;
////    }
////    return path;
////}

////grid::Cell grid::Grid::compute(dev::Vector<int> start, dev::Vector<int> end, dev::Vector<int> position, float D)
////{
////    Me[position].G() = heuristicf(start, position, D, _heuristic);
////    Me[position].H() = heuristicf(end, position, D, _heuristic);
////    return Me[position];
////}

//std::vector<dev::Vector<int> > grid::Grid::getNeighbors(dev::Vector<int> pt)
//{
//    std::vector<dev::Vector<int> > poss;
//    poss.push_back(dev::Vector<int>(pt.x() + 1, pt.y()));
//    poss.push_back(dev::Vector<int>(pt.x() - 1, pt.y()));
//    poss.push_back(dev::Vector<int>(pt.x(), pt.y() + 1));
//    poss.push_back(dev::Vector<int>(pt.x(), pt.y() - 1));
//    if(Me.heuristic() != Manhattan)
//    {
//        poss.push_back(dev::Vector<int>(pt.x() + 1, pt.y() + 1));
//        poss.push_back(dev::Vector<int>(pt.x() + 1, pt.y() - 1));
//        poss.push_back(dev::Vector<int>(pt.x() - 1, pt.y() + 1));
//        poss.push_back(dev::Vector<int>(pt.x() - 1, pt.y() - 1));
//    }
//    std::vector<dev::Vector<int> > neigh;
//    for(size_t i = 0; i < poss.size(); i++)
//    {
//        if(valid(poss[i])) neigh.push_back(poss[i]);
//    }
//    return neigh;
//}

////////Includes
//////#include "grid.h"
//////#include <vector>
//////#include <sstream>
//////#include <list>
//////#include "../../lib/devlib/string.hpp"
//////#ifndef MAX_ITERATIONS_PATHGEN
//////#define MAX_ITERATIONS_PATHGEN  900           //If we hit this, the path generation is halted
//////#endif

//////template<typename T> std::vector<T> operator+=(std::vector<T>& vec, std::vector<T> vec2)
//////{
//////    for(size_t i = 0; i < vec2.size(); i++)
//////    {
//////        vec.push_back(vec2[i]);
//////    }
//////    return vec;
//////}

//////template<typename T> std::vector<T> operator+=(std::vector<T>& vec, T& obj)
//////{
//////    vec.push_back(obj);
//////    return vec;
//////}

//////template<typename T> bool contains(std::vector<T>& vec, T obj)
//////{
//////    for(size_t i = 0; i < vec.size(); i++)
//////    {
//////        if(vec[i] == obj) return true;
//////    }
//////    return false;
//////}

//////float Grid::HeuristicF(dev::Vector<size_t> start, dev::Vector<size_t> child, Heuristic type)
//////{
//////    if(type == Manhattan)
//////    {
//////        return dev::abs(start.getX() - child.getX()) + dev::abs(start.getY() - child.getY());
//////    }
//////    else if(type == Euclidian)
//////    {
//////        return std::sqrtf(dev::pow(dev::abs(start.getX() - child.getX()), 2) + dev::pow(dev::abs(start.getY() - child.getY()), 2));
//////    }
//////    return -1.f;
//////}

//////std::vector<dev::Vector<size_t> > Grid::Grid::neighbors(dev::Vector<size_t> parent)
//////{
//////    //X and Y only
//////    std::vector<dev::Vector<long long> > neigh;
//////    neigh.push_back(dev::Vector<long long>(parent.getX() + 1, parent.getY()));
//////    neigh.push_back(dev::Vector<long long>(parent.getX() - 1, parent.getY()));
//////    neigh.push_back(dev::Vector<long long>(parent.getX(), parent.getY() + 1));
//////    neigh.push_back(dev::Vector<long long>(parent.getX(), parent.getY() - 1));

//////    if(_heuristic == Euclidian)
//////    {
//////        //Diagonal
//////        neigh.push_back(dev::Vector<long long>(parent.getX() + 1, parent.getY() + 1));
//////        neigh.push_back(dev::Vector<long long>(parent.getX() - 1, parent.getY() - 1));
//////        neigh.push_back(dev::Vector<long long>(parent.getX() + 1, parent.getY() - 1));
//////        neigh.push_back(dev::Vector<long long>(parent.getX() - 1, parent.getY() + 1));
//////    }

//////    std::vector<dev::Vector<size_t> > cells;
//////    for(size_t i = 0; i < neigh.size(); i++)
//////    {
//////        if(isOnGrid(neigh[i]))
//////            if(!(*this)[dev::Vector<size_t>(neigh[i].getX(), neigh[i].getY())].blocked)
//////                cells.push_back(dev::Vector<size_t>(neigh[i].getX(), neigh[i].getY()));
//////    }
//////    return cells;
//////}






//////std::vector<dev::Vector<size_t> > Grid::Grid::generatePath(dev::Vector<size_t> start, dev::Vector<size_t> end)
//////{
//////    typedef dev::Vector<size_t> vecst;
//////    typedef std::vector<vecst> vecvecst;
//////#define me  (*this)

//////    vecvecst open;
//////    vecvecst closed;
//////    closed.push_back(start);
//////    open += neighbors(start);


//////    for(size_t watchdog_0 = 0; watchdog_0 < MAX_ITERATIONS_PATHGEN && open.size() > 0; watchdog_0++)
//////    {
//////        float lowestF = -1.f;
//////        float lowestI = -1;
//////        for(size_t i = 0; i < open.size(); i++)
//////        {
//////            me[open[i]].G = HeuristicF(start, open[i], _heuristic);
//////            me[open[i]].H = HeuristicF(end, open[i], _heuristic);
//////            if(me[open[i]].F() < lowestF || lowestI < 0)
//////            {
//////                lowestI = i;
//////                lowestF = me[open[i]].F();
//////            }
//////        }

//////        vecst lowest = open[lowestI];
//////        closed.push_back(lowest);
//////        open.erase(open.begin() + lowestI);

//////        vecvecst neigh = neighbors(lowest);
//////        for(size_t i = 0; i < neigh.size(); i++)
//////        {
//////            if(contains(closed, neigh[i])) continue;
//////            if(!contains(open, neigh[i]))
//////            {
//////                me[neigh[i]].G = HeuristicF(start, neigh[i], _heuristic);
//////                me[neigh[i]].H = HeuristicF(end, neigh[i], _heuristic);
//////                open.push_back(neigh[i]);
//////            }
//////            else
//////            {
//////            }
//////        }

////////        //What is the lowest F value and it's index?
////////        float lowestF = -1.f;
////////        long long lowestI = -1;

////////        //Find the square with the lowest cost function
////////        for(size_t i = 0; i < open.size(); i++)
////////        {
////////            me[open[i]].G = HeuristicF(start, open[i], _heuristic);
////////            me[open[i]].H = HeuristicF(end, open[i], _heuristic);
////////            if(me[open[i]].F() < lowestF || lowestI < 0)
////////            {
////////                lowestF = me[open[i]].F();
////////                lowestI = i;
////////            }
////////        }

////////        vecst S = open[lowestI];
////////        open.erase(open.begin() + lowestI);
////////        closed.push_back(S);

////////        vecvecst T = neighbors(S);
////////        for(size_t i = 0; i < T.size(); i++)
////////        {
////////            if(!contains(closed, T[i]))
////////            {
////////                if(!contains(open, T[i]))
////////                {
////////                    open.push_back(T[i]);
////////                    me[T[i]].G = HeuristicF(start, T[i], _heuristic);
////////                    me[T[i]].H = HeuristicF(end, T[i], _heuristic);
////////                }
////////                else
////////                {
////////                    if(me[T[i]].F() < me[S].F())
////////                    {
////////                        me[T[i]].G = HeuristicF(start, T[i], _heuristic);
////////                        me[T[i]].H = HeuristicF(end, T[i], _heuristic);
////////                        me[T[i]].parent = S;
////////                    }
////////                }
////////            }
////////        }

////////        if(contains(open, end)) break;
//////    }

//////    for(size_t i = 0; i < open.size(); i++)
//////    {
//////        std::cout << "[OPEN]->[" << i << "]=>(" << open[i].getX() << ", " << open[i].getY() << ")" << std::endl;
//////    }
//////    for(size_t i = 0; i < closed.size(); i++)
//////    {
//////        std::cout << "[CLOSED]->[" << i << "]=>(" << closed[i].getX() << ", " << closed[i].getY() << ")" << std::endl;
//////    }

//////    vecvecst path;
//////    for(size_t i = closed.size(); i != 0; i--)
//////    {
//////        path.push_back(closed[i]);
//////        std::cout << "[PATH]->[" << i << "]=>(" << closed[i].getX() << ", " << closed[i].getY() << ")" << std::endl;
//////    }
//////    return path;

////////    std::cout << "Reached path generation!" << std::endl;
////////    vecst cur = end;
////////    for(size_t i = 0; i < MAX_ITERATIONS_PATHGEN; i++)
////////    {
////////        path += cur;
////////        if(path[path.size()] == start) break;
////////        cur = (*this)[cur].parent;
////////    }
////////    std::cout << "Completed path generation!" << std::endl;
//////    return path;
//////}






////////==================================//
////////Important stuff that we aren't worried about atm!

////////Constructs the grid. Creates the two-dimentional array of cells.
//////Grid::Grid::Grid(size_t width, size_t height, Heuristic heuristic) : _width(width), _height(height), _heuristic(heuristic)
//////{
//////    //Initialize the grid
//////    _grid = new Cell*[width];
//////    for(size_t i = 0; i < _width; i++)
//////    {
//////        _grid[i] = new Cell[height];
//////    }
//////    for(size_t i = 0; i < _width; i++)
//////    {
//////        for(size_t j = 0; j < _height; j++)
//////        {
//////            _grid[i][j].x = i;
//////            _grid[i][j].y = j;
//////        }
//////    }
//////}

////////To make things easier for the user, this class uses dynamic memory
////////allocation! We must clean up after ourselves. Note that this is
////////extremely important because we are storing just a ton of data in this
////////two-dimentional pointer array!
//////Grid::Grid::~Grid()
//////{
//////    for(size_t i = 0; i < _width; i++)
//////    {
//////        delete[] _grid[i];
//////    }
//////    delete[] _grid;
//////}
////////==================================//
