#include "actor.h"
#include "StudentWorld.h"
#include <cstdlib>
#include <list>

using namespace std;


void Actor::collidedWith( int IID )
{
    return;
}

void Star::actOut()
{
    damage();
    if (isAlive())
    {
        moveTo( getX(), (getY()-1) );
        damage();
    }
}

void Star::damage()
{
	if( getY() <= 0 )
        setAlive(false);
    else
        setAlive(true);
}

// Goodie base class definitions.

void Goodie::collision()
{
    char goodie = returnType();
    
    int n = 0;
    
    switch (goodie)
    {
        case 'E':
            n = getWorld()->collideWithPlayer( IID_ENERGY_GOODIE, getX(), getY() );
            break;
            
        case 'T':
            n = getWorld()->collideWithPlayer( IID_TORPEDO_GOODIE, getX(), getY() );
            break;
            
        case 'F':
            n = getWorld()->collideWithPlayer( IID_FREE_SHIP_GOODIE, getX(), getY() );
            break;
            
        default:
            break;
    }
    
    if ( n > 0 )
        setAlive(false);
}

void Goodie::collidedWith(const int IID)
{
    if (IID == IID_PLAYER_SHIP)
        setAlive(false);
}

void Goodie::actOut()
{
    collision();

    if (! isAlive() )
        return;

    else
    {
        int currentTick = m_goodieTickLifetime-m_ticksLeft;
                
        if ( isAlive() )
        {
            double brightness = ( m_ticksLeft/m_goodieTickLifetime ) + 0.2;
            
            setBrightness( brightness );
            
            if ( currentTick%3 == 0 )
            {
                moveTo(getX(), getY()-1);
                collision();
                damage();
            }
            else
                damage();
        }
    }
}

void Goodie::damage()
{
    if( getY() <= 0 || m_ticksLeft == 0)
        setAlive(false);
    else
    {
        m_ticksLeft--;
        setAlive(true);
    }
}

// Projectile member functions

Projectile::~Projectile()
{
    if ( m_alien )
    {
        getWorld()->decAlienProjectiles();
    }
}

void Projectile::damage()
{
    if( getY() < 0 || getY() > (VIEW_HEIGHT-1) || getX() < 0 || getX() > (VIEW_WIDTH - 1) )
        setAlive(false);
    else
        setAlive(true);
}

void Projectile::actOut()
{
    damage();
    collision();
    
    if ( !isAlive() )
        return;
    
    else if (m_alien) 
        moveTo( getX(), (getY()-1) );
    else
        moveTo( getX(), (getY()+1) );
    
    damage();
    
    if ( isAlive() )
        collision();
}


void Projectile::collision()
{
    Bullet* bullet = dynamic_cast<Bullet*>(this);
    Torpedo* torpedo = dynamic_cast<Torpedo*>(this);
    int n = 0;
    
    if( m_alien )
    {
        if ( bullet )
            n = getWorld()->collideWithPlayer( IID_BULLET, getX(), getY() );
        else if ( torpedo )
            n = getWorld()->collideWithPlayer( IID_TORPEDO, getX(), getY() );
        
        if ( n > 0 )
            setAlive(false);
    
    }
    else // If it's a player projectile, damage some aliens
    {
        if ( bullet )
            n = getWorld()->searchForAliens( IID_BULLET, getX(), getY() );
        else if ( torpedo )
            n = getWorld()->searchForAliens( IID_TORPEDO, getX(), getY() );

        if ( n > 0 )
            setAlive(false);
    }
}

// Ship member functions

void Ship::damage()
{
    m_energy--;
    
    if( m_energy > 0 )
        setAlive(true);
    else
    {
        Player* player = dynamic_cast<Player*>(this);
        
        if (player)
            getWorld()->playSound(SOUND_PLAYER_DIE);
        else
            getWorld()->playSound(SOUND_ENEMY_DIE);

        setAlive(false);
    }
}


// Alien member functions

Alien::~Alien()
{
    getWorld()->decXenos();
}

void Alien::collision()
{
    kill();
    setAlive(false);
    getWorld()->playSound(SOUND_ENEMY_DIE);
}

bool Alien::doIFire( const int BASE_CHANCE_OF_FIRING )
{
    int roundNum = getWorld()->getRound();
    
    int chancesOfFiring = int(BASE_CHANCE_OF_FIRING/roundNum ) + 1;
    
    // In the expression below, rand() returns a value between 1 and chances of firing.
    // If chancesOfFiring is greater than 1, then there is a 1/chancesOfFiring chance
    // that doFire will evaluate to true. If chancesOfFiring is equal to 1,
    // (i.e., if the round is above 100), then doFire will always evaluate to true,
    // because rand() can only return the value 1.
    
    // If LHS is false, doFire is true. If LHS is true, doFire is false.
    bool doFire = ( ( rand()%chancesOfFiring ) + 1 ) < chancesOfFiring ? false : true;
    
    return doFire;
}

void Alien::fire( const int IID )
{
    switch (IID) {
        case IID_BULLET:
            getWorld()->addProjectile( getX(), getY()-1, IID_BULLET, true );
            getWorld()->playSound( SOUND_ENEMY_FIRE );
            break;
            
        case IID_TORPEDO:
            getWorld()->addProjectile( getX(), getY()-1, IID_TORPEDO, true );
            getWorld()->playSound( SOUND_ENEMY_FIRE );
            break;
            
        default:
            break;
    }
}

void Alien::collidedWith( const int COLLIDED_IID )
{
    Smallbot* isSmallbot = dynamic_cast<Smallbot*>(this);
    
    switch ( COLLIDED_IID )
    {
        case IID_TORPEDO:
            for (int i = 0; i < TORPEDO_DAMAGE; i++)// Damaging alien ship.
            {
                damage();
            }
            if ( isAlive() )
            {
                getWorld()->playSound( SOUND_ENEMY_HIT );
                if (isSmallbot)
                    isSmallbot->justHit( true );
            }
            else
            {
                getWorld()->destroyedAlien( this );
            }
            break;
            
        case IID_BULLET:
            for (int i = 0; i < BULLET_DAMAGE; i++)// Damaging alien ship.
            {
                damage();
            }
            if ( isAlive() )
            {
                getWorld()->playSound( SOUND_ENEMY_HIT );
                if (isSmallbot)
                    isSmallbot->justHit( true );
            }
            else
            {
                getWorld()->destroyedAlien( this );
            }
            break;
            
        case IID_PLAYER_SHIP:
            collision();
            break;
            
        default:
            break;
    }
}

// Begin Natchling member definitions.

void Natchling::actOut()
{
    
    if ( cantAct() )
    {
        actedLastTick( false ); 
        return;
    }
    
    else
    {
        int player_x, player_y;
        
        getWorld()->getPlayerLoc( player_x, player_y );
        
        switch ( m_state ) 
        {
            case 0:
                stateZero( player_x );
                break;
                
            case 1:
                stateOne( player_y );
                break;
                
            case 2:
                stateTwo();
                break;
                
            default:
                break;
        }
        actedLastTick( true );
    }
}

void Natchling::stateZero( int player_x )
{
    // If my horizontal position is not the same as the player's...
    if ( getX() != player_x )                            
    {
        int randomVal = rand() % 3; 
        
        if ( randomVal == 0 ) // With 1/3 chance,
        {   // If my horizontal position is to the right of the player's...
            if ( getX() > player_x ) 
                moveTo( getX()-1, getY()-1 ); // Move me left and down a square.
            else // If my horizontal position is to the left of the player's
                moveTo( getX()+1, getY()-1 ); // Move me right and down a square.
            return; // And then do nothing more.
        }
    }
    // If my horizontal position matches the players & I'm not at the edges...
    else if ( getX() < 29 && getX() > 0 )
    {
        int MDB = (VIEW_WIDTH-1) - getX();
        MDB = MDB < getX() ? MDB : getX(); // Minimum distance to border
        
        m_state = 1;
        
        if (MDB > 3) 
            m_HMD = ( rand()%3 )+1; // Generate a random number between 1 and 3.
        else
            m_HMD = MDB;
        
        m_dir = rand()%2; // Pick a random direction.
        m_HMR = m_HMD; 
    }
    
    moveTo( getX(), getY()-1 );
    
    checkPos();
}

void Natchling::stateOne( int player_y )
{
    if ( player_y > getY() ) // If the player's ship is above mine,
    {
        m_state = 2; // Move to state 2.
        return;
    }
    
    if ( m_HMR == 0 ) 
    {
        m_dir = ( m_dir == RIGHT ) ? LEFT : RIGHT; // Switch directions.
        m_HMR = 2*m_HMD;
        
    }
    else
        m_HMR--;
    
    switch ( m_dir ) // Moving in m_dir direction.
    {
        case RIGHT:
            moveTo( getX()+1, getY() );
            break;
            
        case LEFT:
            moveTo( getX()-1, getY() );
            break;
            
        default:
            break;
    }
    
    if ( doIFire(NATCHLING_FIRE_CHANCE) ) // With probability 1 out of chancesOfFiring (which is always >= 1).
    {
        if ( getWorld()->canAddAlienProjectiles() ) // If there aren't too many projectiles...
            fire( IID_BULLET ); // fire a bullet.
    }
    
    // Decide whether or not to change states.
    bool transition = ( rand()%20 ) < 1 ? true : false;
    
    if ( transition )
        m_state = 2;
}

void Natchling::stateTwo()
{
    if ( getY() == VIEW_HEIGHT-1 ) 
    {
        m_state = 0;
        return;
    }
    
    else if ( getX() == 0 )
        m_dir = RIGHT;
    else if ( getX() == VIEW_WIDTH-1 )
        m_dir = LEFT;
    else
        m_dir = rand()%2;
    
    switch ( m_dir ) // Moving in m_dir direction.
    {
        case RIGHT:
            moveTo( getX()+1, getY()+1 );
            break;
            
        case LEFT:
            moveTo( getX()-1, getY()+1 );
            break;
            
        default:
            break;
    }
}

// Wealthy Natchling definitions

void WealthyNatchling::actOut()
{
    // If malfunctioning, return.
    if (m_malfunction)
    {
        m_malfunctionCount--;
        checkMalfunctionCount();
        return;
    }
    
    // Check if we're going to begin malfunctioning.
    else
        malfunction();
    
    // Do nothing more this tick and for the next 30 ticks if we just began
    // mulfunctioning.
    if (m_malfunction)
        return;
    else // Else, behave like a regualr Natchling
        Natchling::actOut();
}

void WealthyNatchling::checkMalfunctionCount()
{
    if ( m_malfunctionCount == 0 )
        m_malfunction = false;
}
void WealthyNatchling::malfunction()
{
    
    // 1 out of 200 chance to begin malfunctioning.
    m_malfunction = ( (rand() % 200) < 1 ) ? true : false;
    
    // Malfunction for 30 ticks.
    if (m_malfunction)
        m_malfunctionCount = 30;
}

// Smallbot definitions

void Smallbot::actOut()
{
    
    if ( cantAct() )
    {
        actedLastTick( false );
        return;
    }
    
    else
        actedLastTick(true);

    // Beginning to adjust position.
    
    // 1 out of 3 chances of adjusting horizontal position
    bool p_adjust = (rand()%3) < 1 ? true : false;
    
    if (m_hit && p_adjust)
    {
        // If rand%2 is 0, x_factor is -1 and if rand%2 is 1, x_factor is 1.
        int x_factor = rand()%2 < 1 ? -1 : 1;
        switch (getX())
        {
            case 0:
                moveTo( getX()+1, getY()-1 );
                break;
                
            case VIEW_WIDTH-1:
                moveTo( getX()-1, getY()-1 );
                break;
                
            default:
                moveTo( (getX()+x_factor), getY()-1 );
                break;
        }
    }
    
    else
        moveTo( getX(), getY()-1 );
    
    checkPos(); // Check position to see if I've gone off the bottom of
                // the screen.

    if ( !isAlive() ) // If I flew off the bottom and died, do nothing more.
        return;
    
    // Deciding whether to fire or not.
    int player_x, player_y;
    int my_x = getX();
    int my_y = getY();
    
    getWorld()->getPlayerLoc( player_x, player_y );
    
    if ( my_x == player_x && my_y > player_y )
    {   // If there aren't too many projectiles in the world
        if ( getWorld()->canAddAlienProjectiles() )
        {
            if ( doIFire(SMALLBOT_FIRE_CHANCE) ) // Fire a torpedo, if I can.
                fire(IID_TORPEDO);
            else
                fire(IID_BULLET); // Otherwise, fire a bullet.
        }
    }
}

// Player member definitions
void Player::actOut()
{
    collision(); // Calculate collision damage.
    
    if ( isAlive() ) // If I survived, find out where to go next.
    {
        input();
        collision(); // See if I collided with anything.
    }
}

void Player::input()
{
    if( getWorld()->getKey( m_input ) )
    {
        switch (m_input) {
                
            case KEY_PRESS_LEFT:
                if ( getX() > 0 )
                    moveTo(getX()-1, getY());
                setProjectile(false);
                break;
            case KEY_PRESS_RIGHT:
                if ( getX() < VIEW_WIDTH-1 )
                    moveTo(getX()+1, getY());
                setProjectile(false);
                break;
            case KEY_PRESS_UP:
                if ( getY() < VIEW_HEIGHT-1 )
                    moveTo(getX(), getY()+1);
                setProjectile(false);
                break;
            case KEY_PRESS_DOWN:
                if ( getY() > 0 )
                    moveTo(getX(), getY()-1);
                setProjectile(false);
                break;
            case KEY_PRESS_SPACE:
                if ( (!getProjectile()) && getY() < VIEW_HEIGHT-2 )
                {
                    getWorld()->addProjectile( getX(), getY(), IID_BULLET, false);
                    getWorld()->playSound(SOUND_PLAYER_FIRE);
                    setProjectile(true);
                }
                else
                    setProjectile(false);
                break;
            case KEY_PRESS_TAB:
                if ( (!getProjectile()) && getWorld()->getTorpedoes()
                                        && getY() < VIEW_HEIGHT-2 )
                {
                    getWorld()->addProjectile( getX(), getY()+1, IID_TORPEDO, false );
                    getWorld()->playSound(SOUND_PLAYER_TORPEDO);
                    setProjectile(true);
                }
                else
                    setProjectile(false);
                break;
            default:
                break;
        }
    }
    else
        return;
}


void Player::collision()
{
    if ( getWorld()->isEmpty() ) // If no actors, do nothing.
        return;
    
    else
    {
        int n = SHIP_DAMAGE * getWorld()->searchForAliens( IID_PLAYER_SHIP, getX(), getY() );
        
        for (int i = 0; i < n; i++)// Damaging my ship.
        {
            damage();
        }
        
    }
    
    
}

void Player::collidedWith( const int IID )
{
    switch (IID) {
        case IID_TORPEDO:
            getWorld()->playSound(SOUND_PLAYER_HIT);
            for (int i = 0; i < TORPEDO_DAMAGE; i++)// Damaging my ship.
            {
                damage();
            }
            break;
            
        case IID_BULLET:
            getWorld()->playSound(SOUND_PLAYER_HIT);
            for (int i = 0; i < BULLET_DAMAGE; i++)// Damaging my ship.
            {
                damage();
            }  
            break;
            
        case IID_NACHLING:
        case IID_WEALTHY_NACHLING:
        case IID_SMALLBOT:
            getWorld()->playSound(SOUND_ENEMY_PLAYER_COLLISION);
            for (int i = 0; i < SHIP_DAMAGE; i++)// Damaging my ship.
            {
                damage();
            }
            break;
            
        case IID_ENERGY_GOODIE:
            setEnergy(PLAYER_ENERGY);
            break;
            
        default:
            break;
    }
}