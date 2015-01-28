// @file: maze.cpp
// Uses homework 2 solution (provided on website) as a base.
// Modifies solution to use recursion instead of a stack to implement pathExists.


bool pathExists(char maze[][10], int sr, int sc, int er, int ec)
{
    
    bool pathN = false;
    bool pathS = false;
    bool pathE = false;
    bool pathW = false;
    
    if (sr < 0  ||  sr > 9  ||  sc < 0  || sc > 9  ||
        er < 0  ||  er > 9  ||  ec < 0  || ec > 9  ||
        maze[sr][sc] != '.'  ||  maze[er][ec] != '.')
        return false;
        
    maze[sr][sc] = '#';
    
    if (sr == er && sc == ec)
        return true;
    if (maze[sr-1][sc] == '.')
        pathS = pathExists(maze, sr-1, sc, er, ec);
    if (maze[sr+1][sc] == '.')
       pathN = pathExists(maze, sr+1, sc, er, ec);
    if (maze[sr][sc-1] == '.')
        pathE = pathExists(maze, sr, sc-1, er, ec);
    if (maze[sr][sc+1] == '.')
        pathW = pathExists(maze, sr, sc+1, er, ec);
    
    if (pathE || pathW || pathS || pathN)
        return true;

}
