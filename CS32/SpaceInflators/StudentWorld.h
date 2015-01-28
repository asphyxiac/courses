#ifndef _STUDENTWORLD_H_
#define _STUDENTWORLD_H_

#include "GameWorld.h"
#include "GameConstants.h"
#include <string>
#include <vector>
#include <list>

class Actor;
class Alien;
class Player;
// Students:  Add code to this file, StudentWorld.cpp, actor.h, and actor.cpp

class StudentWorld : public GameWorld
{
public:
    
    StudentWorld()
    : GameWorld(), m_torpedoes(0), m_actors(0), m_player(NULL), m_round(1), 
      m_destroyed(0), m_xenos(0), m_alienprojectiles(0)
    
    {}
    
    ~StudentWorld();
    
	virtual void init();
    
	virtual int move();
    
	virtual void cleanUp();
        
    // Determines if there are any non-player objects in the world.
    const int isEmpty()
    {
        return m_actors.empty();
    }
    
    // Adds a new actor to m_actors.
    void addActor( const int x, const int y, const int kind );
    
    // Adds a projectile of player or alien origin.
    // Pre-condition: m_actors contains n actors.
    // Post-condition: m_actors contains n+1 actors.
    void addProjectile( const int x, const int y, const int kind, bool alien );
    
    // Finds and damages alien ships with coords x, y.
    int searchForAliens( const int IID, const int x, const int y );
    
    int collideWithPlayer( const int IID, const int x, const int y );
    
    void destroyedAlien( Alien* alienptr );
    
    void incTorpedoes()
    {
        m_torpedoes++;
    }
    
    void decTorpedoes()
    {
        m_torpedoes--;
    }
    
    void decAlienProjectiles()
    {
        m_alienprojectiles--;
    }
    
    void decXenos()
    {
        m_xenos--;
    }
    
    void getPlayerLoc( int& x, int& y );
    
    bool getTorpedoes()
    {
        if (m_torpedoes > 0)
            return true;
        else
            return false;
    }
    
    const double getPlayerEnergy();
    
    const int getRound()
    {
        return m_round;
    }
    
    const int getAlienProjectiles()
    {
        return m_alienprojectiles;
    }
    
    const bool canAddAlienProjectiles()
    {
        int count = getAlienProjectiles();
        int twiceRound = 2*getRound();
        
        if ( count < twiceRound ) // If there aren't already too many projectiles,
            return true; // fire.
        
        else
            return false;
    }
    
    
    Player* getPlayer()
    {
        return m_player;
    }
    
private:
    
    std::string formatScore( int score, int round, int energyPercent, int torpedoes, int shipsLeft);
    
    void incrementDestroyedCount()
    {
        m_destroyed++;
    }
    
    int toDestroy()
    {
        return 4*m_round;
    }
    
    void updateRound()
    {
        if ( m_destroyed >= toDestroy() )
            m_round++;
    }

    void setDisplayText();
    
    void gameState();
    
    void addTestParam();
    
    std::vector<Actor*> m_actors;
    Player* m_player;
    int m_torpedoes; // Number of torpedoes player has left.
    int m_alienprojectiles; // Number of alien projectiles in space.
    int m_xenos; // Number of active aliens.
    int m_destroyed; // Number of ships destroyed by player.
    int m_round; // Round number
    
};

#endif // _GAMEWORLD_H_
