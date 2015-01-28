//  @file: mazequeue.cpp

#include <iostream>
#include <queue>
using namespace std;


bool pathExists(char maze[][10], int sr, int sc, int er, int ec);
// Return true if there is a path from (sr,sc) to (er,ec)
// through the maze; return false otherwise

class Coord
{
public:
    Coord(int rr, int cc) : m_r(rr), m_c(cc) {}
    int r() const { return m_r; }
    int c() const { return m_c; }
private:
    int m_r;
    int m_c;
};

bool pathExists(char maze[][10], int sr, int sc, int er, int ec)
{
    queue<Coord> mazeStack;
    
    enum dir {North, South, East, West};
    
    Coord start(sr, sc);
    Coord end(er, ec);
    
    mazeStack.push(start);
    
    maze[sr][sc] = '*';
    
    while (! (mazeStack.empty()) ) 
    {
        Coord current = mazeStack.front();
        mazeStack.pop();
        
        //cout << char(40) << current.r() << ", " << current.c() << char(41) << endl;
        
        
        if ( current.r() == end.r() && current.c() == end.c() )
            return true;
        else
        {
            for (int i = 0; i < 4; i++) 
            {
                switch (i) 
                {
                    case North:
                        if (current.r()-1 >= 0 && maze[current.r()-1][current.c()] == '.') 
                        {
                            maze[current.r()-1][current.c()] = '*';
                            Coord step(current.r()-1, current.c());
                            mazeStack.push(step);
                        }
                        break;
                        
                    case South:
                        if (current.r()+1 < 10 && maze[current.r()+1][current.c()] == '.') 
                        {
                            maze[current.r()+1][current.c()] = '*';
                            Coord step(current.r()+1, current.c());
                            mazeStack.push(step);
                        }
                        break;
                        
                    case East:
                        if (current.c()+1 < 10 && maze[current.r()][current.c()+1] == '.') 
                        {
                            maze[current.r()][current.c()+1] = '*';
                            Coord step(current.r(), current.c()+1);
                            mazeStack.push(step);
                        }
                        break;
                        
                    case West:
                        if (current.c()-1 >= 0 && maze[current.r()][current.c()-1] == '.') 
                        {
                            maze[current.r()][current.c()-1] = '*';
                            Coord step(current.r(), current.c()-1);
                            mazeStack.push(step);
                        }
                        break;
                        
                    default:
                        break;
                }
            }
        }
    }
    
    return false;
}