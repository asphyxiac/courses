#ifndef _ACTOR_H_
#define _ACTOR_H_

#include "StudentWorld.h"
#include "GraphObject.h"
#include "GameConstants.h"
#include <cstdlib>
#include <iostream>


// Constants as specified.
const int TORPEDO_DAMAGE = 8;
const int BULLET_DAMAGE = 2;
const int SHIP_DAMAGE = 15;
const int NATCHLING_POINTS = 1000;
const int WEALTHY_NATCHLING_POINTS = 1200;
const int SMALLBOT_POINTS = 1500;
const int NATCHLING_FIRE_CHANCE = 10;
const int SMALLBOT_FIRE_CHANCE = 100;
const int GOODIE_SCORE_VAL = 5000;
const int TORPEDOS = 5;

class StudentWorld;

class Actor: public GraphObject
{
public:
	Actor( int iid, int x, int y, StudentWorld* worldptr )
    : GraphObject( iid,	x, y ), stillAlive(true), world(worldptr)
	{
		setVisible( stillAlive );
	}
    
	virtual void actOut()
    { return; }
    
    virtual void damage()= 0;
    
    void setAlive( bool alive )
    {
        stillAlive = alive;
    }
        
    StudentWorld* getWorld()
    {
        return world;
    }
    
	const bool isAlive()
	{
		return stillAlive;
	}
    
    virtual void collidedWith( const int IID );
    
    
private:
    virtual void collision() = 0;

	bool stillAlive;
    StudentWorld* world;
};


// Class Star
class Star: public Actor
{
public:
	Star(int x, int y, StudentWorld* worldptr )
    : Actor( IID_STAR, x, y, worldptr )
	{}
    
    virtual ~Star()
    {
        setVisible(false);
        setBrightness(0);
    }
    
	virtual void actOut();
    virtual void damage();
private:
    virtual void collision()
    {return;}
};

// Class Goodie

class Goodie : public Actor
{
public:
    Goodie( int IID, int x, int y, StudentWorld* worldptr, char type )
    : Actor( IID, x, y, worldptr), m_type(type)
    {
        m_goodieTickLifetime = int( 100.0/getWorld()->getRound() ) + 30;
        m_ticksLeft = m_goodieTickLifetime;
    }
    
    virtual void actOut();
        
    virtual void collidedWith( const int IID );
    
    virtual void damage();
    
    const char returnType()
    {
        return m_type;
    }
    
    
private:
    virtual void collision();
    
    int m_goodieTickLifetime;
    int m_ticksLeft;
    char m_type;
};

// Class TorpedoGoodie

class TorpedoGoodie : public Goodie
{
public:
    TorpedoGoodie( int x, int y, StudentWorld* worldptr )
    : Goodie( IID_TORPEDO_GOODIE, x, y, worldptr, 'T' )
    {}
    
    virtual ~TorpedoGoodie()
    {
        std::cout << "Being Destroyed!"  << std::endl;
    }
};

// Class EnergyGoodie
class EnergyGoodie : public Goodie
{
public:
    EnergyGoodie( int x, int y, StudentWorld* worldptr )
    : Goodie( IID_ENERGY_GOODIE, x, y, worldptr, 'E' )
    {}
};

// Class FreeShipGoodie
class FreeShipGoodie : public Goodie
{
public:
    FreeShipGoodie( int x, int y, StudentWorld* worldptr )
    : Goodie( IID_FREE_SHIP_GOODIE, x, y, worldptr, 'F' )
    {}
};

// Class Projectile
class Projectile: public Actor
{
public:
    Projectile( int iid, int x, int y, StudentWorld* worldptr, int dam, bool alien)
    : Actor( iid, x, y, worldptr ), m_damage(dam), m_alien (alien)
    {}
    
    virtual ~Projectile();
    
    virtual void actOut();
    
    virtual void damage();
        
private:
    virtual void collision();

    int m_damage;
    bool m_alien;
};

// Class Bullet
class Bullet: public Projectile
{
public:
    Bullet(int x, int y, StudentWorld* worldptr, bool alien)
    : Projectile( IID_BULLET, x, y, worldptr, BULLET_DAMAGE, alien )
    {}

};

// Class Torpedo
class Torpedo: public Projectile
{
public:
    
    Torpedo( int x, int y, StudentWorld* worldptr, bool alien)
    : Projectile( IID_TORPEDO, x, y, worldptr, TORPEDO_DAMAGE, alien )
    {}


};


// Ship abstract class
class Ship: public Actor
{
public:
	Ship( int iid, int x, int y, int e, StudentWorld* worldptr )
    : Actor( iid, x, y, worldptr ), m_energy(e), m_projectile(false)
	{}
    
	const int getEnergy()
	{
		return m_energy;
	}
    
    const bool getProjectile()
    {
        return m_projectile;
    }
    
    void setProjectile( bool value )
    {
        m_projectile = value;
    }
    
    void setEnergy( int value )
    {
        m_energy = value;
    }
    
    virtual void damage();
    
    virtual void collidedWith( const int IID ) = 0;
    
private:
	int m_energy;
    bool m_projectile;
};

class Alien: public Ship
{
public:
    
    Alien( int iid, int x, int y, int e, StudentWorld* worldptr )
    : Ship( iid, x, y, e, worldptr ), m_cantAct(false)
    {}
    
    virtual ~Alien();
    
    virtual void collidedWith( const int COLLIDED_IID );
    
    void fire( const int IID );
    
    bool doIFire( const int BASE_CHANCE );
    
    const bool cantAct()
    {
        return m_cantAct;
    }
    
    void checkPos()
    {
        if (getY() < 0) // If I flew off the bottom of the screen
            setAlive(false); // I've died :(.
    }
    
    void actedLastTick( bool acted )
    {
        m_cantAct = acted;
    }
        
private:
    void kill()
    {
        setEnergy(0);
    }
    virtual void collision();

    bool m_cantAct;
};

class Natchling: public Alien
{
public:
    
    Natchling( int iid, int x, int y, int e, StudentWorld* worldptr )
    : Alien( iid, x, y, e, worldptr ), m_state(0), m_HMR(0), m_HMD(0)
    {}
    
    virtual void actOut();
    
    virtual void stateZero( int player_x );
    
    virtual void stateOne( int player_y );
    
    virtual void stateTwo();
        
private:
    int m_state;
    int m_HMR;
    int m_HMD;
    enum DIRECTION{ RIGHT, LEFT };
    int m_dir;
};

// Wealthy Natchling

class WealthyNatchling : public Natchling
{
public:
    
    WealthyNatchling( int x, int y, int e, StudentWorld* worldptr )
    : Natchling( IID_WEALTHY_NACHLING, x, y, e, worldptr ), m_malfunction(false),
      m_malfunctionCount(0)
    {}
    
    virtual void actOut();
        
private:
    
    void malfunction();
    
    void checkMalfunctionCount();
    
    bool m_malfunction;
    int m_malfunctionCount;
};

// Smallbot

class Smallbot : public Alien
{
public:
    
    Smallbot( int x, int y, int e, StudentWorld* worldptr )
    : Alien( IID_SMALLBOT, x, y, e, worldptr ), m_hit(false)
    {}
    
    virtual void actOut();
    
    void justHit( bool wasIhit )
    {
        m_hit = wasIhit;
    }

        
private:
    bool m_hit;
};

// Class Player
class Player: public Ship
{
public:
    
	Player( StudentWorld* worldptr)
    : Ship( IID_PLAYER_SHIP, VIEW_WIDTH/2, 1, PLAYER_ENERGY, worldptr), m_input(0)
	{}
    
    ~Player()
    {
        setVisible(false);
        setBrightness(0);
    }
        
	virtual void actOut();
        
    virtual void collidedWith( const int IID );
    
private:
    // Collision finds aliens that I've hit, if any.
    // Post-condition: If I've hit aliens, take damage equal to collided.size()*15,
    // and kill all aliens whom I've hit.
    // Else, do nothing.
    virtual void collision();
    
    void input();
    
	static const int PLAYER_ENERGY = 50;
    int m_input;
};

#endif // _ACTOR_H_