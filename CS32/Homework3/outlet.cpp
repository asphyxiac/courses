//  @file: outlet.cpp
//  requires #include <string> and #include <iostream> to function.

class Outlet
{
public:

    Outlet( string idName ): m_ID(idName) {}
    
    virtual ~Outlet();
    
    virtual string transmit( string msg ) const { return "text: "+msg; }
    
    virtual string connect() const = 0;
    
    string id() const { return m_ID; }
    

private:
    
    string m_ID;  
    

};

class Phone : public Outlet
{
public:
    
    Phone( string idName, int type );
    
    virtual ~Phone();
    
    virtual string connect() const { return "Call"; };
    
    virtual string transmit ( string msg ) const;
    
private:
        string m_type;
};

class TwitterAccount : public Outlet
{
public:
    
    TwitterAccount( string idName ): Outlet(idName){}
    
    virtual ~TwitterAccount();
    
    virtual string connect() const { return "Tweet"; }
};

class EmailAccount : public Outlet
{
public:
    EmailAccount( string idName ): Outlet(idName){}
    
    virtual ~EmailAccount();
    
    virtual string connect() const { return "Email"; }
};

Outlet::~Outlet()
{
    cout << id() << "." << endl;
}

Phone::Phone( string idName, int type )
: Outlet(idName)
{
    if (type == 0)
        m_type = "voice";
    else if (type == 1)
        m_type = "text";
    else
        m_type = "NULL";
}

string Phone::transmit( string msg ) const
{
    return m_type+": "+msg;
}

Phone::~Phone()
{
    cout << "Destroying the phone ";
}

TwitterAccount::~TwitterAccount()
{
    cout << "Destroying the Twitter account ";
}

EmailAccount::~EmailAccount()
{
    cout << "Destroying the email account ";
}