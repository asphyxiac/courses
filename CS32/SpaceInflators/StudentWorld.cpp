#include "StudentWorld.h"
#include "GameWorld.h"
#include "actor.h"
#include "GameConstants.h"
#include <vector>
#include <iterator>
#include <ctime>
#include <cstdlib>
#include <iostream>
#include <sstream> 
#include <iomanip>

using namespace std;

GameWorld* createStudentWorld()
{
    return new StudentWorld();
}


StudentWorld::~StudentWorld()
{    
    while( !m_actors.empty() ) 
    {
        delete m_actors.back();
        m_actors.pop_back();
    }
    
    delete m_player;
}


void StudentWorld::init()
{
    srand(time(NULL));
    m_player = new Player ( this );
    m_torpedoes = 0;
}

int StudentWorld::move()
{
    setDisplayText();

    if (testParamsProvided())
        addTestParam();
    
    else 
        gameState();
        
    // Do something, or GTFO!        
    if( m_player->isAlive() )
        m_player->actOut();
    else // the player is dead
    {   decLives();
        return GWSTATUS_PLAYER_DIED;
    }
    
    for ( int i = 0; i < m_actors.size(); ++i )
    {
        if ( m_actors[i]->isAlive() )
        {
            m_actors[i]->actOut();
        }
    }
    
    // Remove m_actors that have died this round.
    vector<Actor*>::iterator it = m_actors.begin();
    
    while ( it < m_actors.end() && (*it) )
    {
        if (! ((*it)->isAlive()) )
        {
            delete (*it);
            it = m_actors.erase(it);
        }
        else
            ++it;
    }
    
    updateRound();
    setDisplayText();

    if( m_player->isAlive() )
        return GWSTATUS_CONTINUE_GAME;
    else // the player is dead
    {   decLives();
        return GWSTATUS_PLAYER_DIED;
    }
}

void StudentWorld::cleanUp()
{    
    while( !m_actors.empty() ) 
    {
        delete m_actors.back();
        m_actors.pop_back();
    }
    
    delete m_player;
}

void StudentWorld::addTestParam()
{
    
    if ( m_actors.empty() )
    {
        int paramID = 0;
        
        getTestParam(paramID);
        
        switch (paramID)
        {
            case TEST_PARAM_NACHLING:
                addActor( (rand()%VIEW_WIDTH-1), VIEW_HEIGHT-1, IID_NACHLING);
                break;
                
            case TEST_PARAM_WEALTHY_NACHLING:
                addActor( (rand()%VIEW_WIDTH-1), VIEW_HEIGHT-1, IID_WEALTHY_NACHLING);
                break;
                
            case TEST_PARAM_SMALLBOT:
                addActor( (rand()%VIEW_WIDTH-1), VIEW_HEIGHT-1, IID_SMALLBOT);
                break;
                
            case TEST_PARAM_GOODIE_ENERGY:
                addActor( 15, VIEW_HEIGHT-1, IID_ENERGY_GOODIE);
                break;
                
            case TEST_PARAM_GOODIE_TORPEDO:
                addActor( 15, VIEW_HEIGHT-1, IID_TORPEDO_GOODIE);
                break;
                
            case TEST_PARAM_GOODIE_FREE_SHIP:
                addActor( 15, VIEW_HEIGHT-1, IID_FREE_SHIP_GOODIE);
                break;
                
            default:
                break;
        }
    }
    else
        return;
}

void StudentWorld::gameState()
{        
    int max = int( 2 + .5 * m_round );
    
    if( m_xenos < max && ( max - toDestroy()) < m_xenos )
    {
        int p1 = ( rand()%100 );
        
        if ( p1 < 70 ) 
        {
            int p2 = ( rand()%100 );
            
            if ( p2 < 20 ) 
            {
                addActor( (rand()%VIEW_WIDTH-1), VIEW_HEIGHT-1, IID_WEALTHY_NACHLING);
            }
            else
            {
                addActor( (rand()%VIEW_WIDTH-1), VIEW_HEIGHT-1, IID_NACHLING);
            }
        }
        
        else
        {
            addActor( (rand()%VIEW_WIDTH-1), VIEW_HEIGHT-1, IID_SMALLBOT);
        }
    }
    
    int x_star = ( rand()%VIEW_WIDTH );
    int y_star = ( rand()%VIEW_HEIGHT );
    
    int p3 = ( rand()%100 );

    if ( p3 < 33 ) // 33% chance
        addActor( x_star, y_star, IID_STAR );
}


void StudentWorld::setDisplayText()
{
    int score = getScore(); 
    int round = m_round;
    int energyPercent = static_cast<int>( getPlayerEnergy() );
    int torpedoes = m_torpedoes;
    int shipsLeft = getLives();
    // Next, create a string from your statistics, of the form: 
    // “Score: 0000123 Round: 01 Energy: 98% Torpedoes: 003 Ships: 03”
    // where all numeric values except the Energy value must have leading 
    // zeros (e.g., 003 vs. 3), and each
    // statistic should be separated from the last statistic by two 
    // spaces. E.g., between the “0000123” of the
    // score and the “R” in “Round” there must be two spaces. Each field 
    // must be exactly as wide as shown,
    // e.g., the score must be exactly 7 digits long, the torpedo field 
    // must be 3 digits long, except for the 
    // Energy field, which could be between 1 and 3 digits (e.g., 5%, 89% 
    // or 100%)
    string s = formatScore( score, round, energyPercent, torpedoes, shipsLeft); // to define
    // Finally, update the display text at the top of the screen with your 
    // newly created stats
    setGameStatText(s); // calls GameWorld::setGameStatText
}

std::string StudentWorld::formatScore( int s, int r, int e, int t, int numships )
{
    ostringstream oss;
    
    oss.setf(ios::fixed);
    
    
    oss << "Score:  " << setw(7) << setfill('0') << s;
    oss << "  Round:  " << setw(2) << setfill('0') << r;
    oss << "  Energy:  " << e << '\%';
    oss << "  Torpedoes:  " <<  setw(3) << setfill('0') << t;
    oss << "  Ships:  " <<  setw(2) << setfill('0') << numships;
    
    string display = oss.str();
    
    return display;
}

const double StudentWorld::getPlayerEnergy()
{
    int player_e = m_player->getEnergy();
    
    double percent_e = (player_e/50.0)*100.0;
    
    return percent_e+0.5;
}


void StudentWorld::getPlayerLoc( int& x, int& y )
{
    x = m_player->getX();
    y = m_player->getY();
}

void StudentWorld::destroyedAlien( Alien* alienptr )
{
    bool dropGoodie = (rand()%3 == 0 ) ? true : false;
                       
    WealthyNatchling* wn_temp = dynamic_cast<WealthyNatchling*>(alienptr);
    Smallbot* sb_temp = dynamic_cast<Smallbot*>(alienptr);
    
    int IID_ALIEN = -1;
    
    if (wn_temp)
        IID_ALIEN = IID_WEALTHY_NACHLING;
    else if (sb_temp)
        IID_ALIEN = IID_SMALLBOT;
    else
        IID_ALIEN = IID_NACHLING;
    
    switch (IID_ALIEN)
    {
        case IID_NACHLING:
            increaseScore(NATCHLING_POINTS);
            incrementDestroyedCount();
            break;
            
        case IID_WEALTHY_NACHLING:
            increaseScore(WEALTHY_NATCHLING_POINTS);
            incrementDestroyedCount();
           if ( dropGoodie )
            {
                bool fifty50 = ( rand()% 2  < 1 ) ? true : false;
                if ( fifty50 )
                    addActor(wn_temp->getX(), wn_temp->getY(), IID_ENERGY_GOODIE);
                else
                    addActor(wn_temp->getX(), wn_temp->getY(), IID_TORPEDO_GOODIE);
            }
            break;
        
        case IID_SMALLBOT:
            increaseScore(SMALLBOT_POINTS);
            incrementDestroyedCount();
            if (dropGoodie)
                addActor(sb_temp->getX(), sb_temp->getY(), IID_FREE_SHIP_GOODIE);
            break;
        
        default:
            break;
    }
}

void StudentWorld::addActor( const int x, const int y, const int kind )
{
    int roundFactor = 0.9 + 0.1 * m_round;

    switch (kind) {
        
        case IID_STAR: 
            m_actors.push_back( new Star( x, y, this ) );
            break;
            
        case IID_NACHLING:
            m_actors.push_back( new Natchling( IID_NACHLING, x, y, int( 5*roundFactor ), this) );
            m_xenos++;
            break;
            
        case IID_WEALTHY_NACHLING:
            m_actors.push_back( new WealthyNatchling( x, y, int( 8*roundFactor ), this) );
            m_xenos++;
            break;
            
        case IID_SMALLBOT:
            m_actors.push_back(new Smallbot ( x, y, int(12*roundFactor), this ) );
            m_xenos++;
            break;
        
        case IID_ENERGY_GOODIE:
            m_actors.push_back( new EnergyGoodie( x, y, this ) );
            break;
            
        case IID_FREE_SHIP_GOODIE:
            m_actors.push_back( new FreeShipGoodie( x, y, this ) );
            break;
            
        case IID_TORPEDO_GOODIE:
            m_actors.push_back( new TorpedoGoodie( x, y, this ) );
            break;
            
        default:
            break;
    }
}


void StudentWorld::addProjectile( const int x, const int y, const int kind, bool alien  )
{    
    switch (kind) {
            
        case IID_BULLET:
            m_actors.push_back( new Bullet( x, y, this, alien) );
            if (alien)
                m_alienprojectiles++;
            break;
            
        case IID_TORPEDO:
            m_actors.push_back( new Torpedo( x, y, this, alien) );
            if (alien)
                m_alienprojectiles++;
            else
                decTorpedoes();
            break;
            
        default:
            break;
    }
}


int StudentWorld::searchForAliens( const int IID, const int x, const int y )
{
    int n = 0;
    
    // Finding aliens at player/projectile location.
    for ( vector<Actor*>::iterator i = m_actors.begin(); i != m_actors.end(); ++i )
    {
        if ( x == (**i).getX() && y == (**i).getY() )
        {
            Alien* s = dynamic_cast<Alien*>(*i);
            if (s != NULL)
            {
                n++;
                switch (IID)
                {
                    case IID_PLAYER_SHIP:
                        playSound(SOUND_ENEMY_PLAYER_COLLISION);
                        (*i)->collidedWith(IID);
                        break;
                        
                    case IID_TORPEDO:
                        (*i)->collidedWith(IID);
                        break;
                        
                    case IID_BULLET:
                        (*i)->collidedWith(IID);
                        break;
                        
                    default:
                        break;
                }
            }
            
        }
    }
    return n; // In this method, n is only incremented if the player collides with a ship.
}

int StudentWorld::collideWithPlayer( const int IID, const int x, const int y )
{
    int n = 0;
    int player_x, player_y;
    
    getPlayerLoc( player_x, player_y );
    
    if ( x == player_x && y == player_y ) 
    {
        switch (IID)
        {
            case IID_BULLET:
                m_player->collidedWith( IID_BULLET );
                n++;
                break;
                
            case IID_TORPEDO:
                m_player->collidedWith( IID_TORPEDO );
                n++;
                break;
                
            case IID_NACHLING:
                m_player->collidedWith( IID_NACHLING );
                n++;
                break;
                
            case IID_WEALTHY_NACHLING:
                m_player->collidedWith( IID_WEALTHY_NACHLING );
                n++;
                break;
                
            case IID_SMALLBOT:
                m_player->collidedWith( IID_SMALLBOT );
                n++;
                break;
                
            case IID_ENERGY_GOODIE:
                m_player->collidedWith( IID_ENERGY_GOODIE );
                playSound(SOUND_GOT_GOODIE);
                increaseScore(GOODIE_SCORE_VAL);
                n++;
                break;
                
            case IID_FREE_SHIP_GOODIE:
                playSound(SOUND_GOT_GOODIE);
                increaseScore(GOODIE_SCORE_VAL);
                incLives();
                n++;
                break;
                
            case IID_TORPEDO_GOODIE:
            {
                playSound(SOUND_GOT_GOODIE);
                increaseScore(GOODIE_SCORE_VAL);
                for ( int i = 0; i < TORPEDOS; i++ )
                {
                    incTorpedoes();
                }
                n++;
            }
                break;
                
            default:
                break;
        }
        
    }
    
    return n;
}