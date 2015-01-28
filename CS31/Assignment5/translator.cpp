// Translator.cpp
// This is a simple program that translates english
// text into pig-latin. 
//
// Last edited by Prianna Ahsan on 8/7/12.
// prianna@ucla.edu
//

#include <iostream>
#include <sstream>
#include <string>
#include <fstream>
#include <cctype>
using namespace std;

string analyze( string& word );
string punctuation( int& frontPunct, string& word ); 
//above function checks for leading punctuation
string punctuation( int i, int& backPunct, string& word );
//above function checks for trailing punctuation

int main()
{
    string inFile, outFile;
    
    cout << "Input file: ";
    cin >> inFile;
    
    ifstream fin;
    ofstream fout;
    
    fin.open( inFile.c_str() );
    
    while (fin.fail( ))//checking if input file is readable.
    {
        cout << "Try Again. ";
        cout << "Input file: ";
        cin >> inFile;
        fin.open( inFile.c_str() );
    }
    
    cout << "Output file: ";
    cin >> outFile;
    
    fout.open( outFile.c_str() );
    
    while (fout.fail( )) //checking if output file is readable.
    {
        cout << "Try Again. ";
        cout << "Output file: ";
        cin >> outFile;
        fin.open( outFile.c_str() );
    }

    stringstream myStr;
    
    if( fin )
    {
        myStr << fin.rdbuf();
        fin.close();
        string word;
        
        while( myStr >> word)
        {
            string pigWord = analyze( word );
            fout << pigWord << " ";
                
        }
        
    }
    fout.close();
    
    return 0;
}

string punctuation( int& frontPunct, string& word )
{
    int k = 0;
    
    string punctf;
    
    frontPunct = 0;
    
    if( ispunct(word[0]) )// if the first character is punctuation...
    {
        while( ispunct(word[k]) ) //while there are any leading punct marks...
        {
            punctf = punctf+word[k]; //value and position of punct chars.
            frontPunct++; // note number of leading punct chars.
                        
            k++;
        }
        
        return punctf;
    }
    
    return "";
}
    
string punctuation( int i, int& backPunct, string& word )
{
    int k = 0, j = i-1;
    
    string punctb;
    
    backPunct = 0;
    
    if( ispunct(word[j])) // if the last char is punctuation...
    {
        while( ispunct(word[j]) )
        {
            punctb = word[j]+punctb; //value and position of punct chars.
            backPunct++; // note number of lagging punct chars.
            j--;
            k++;
        }
        return punctb;
    }
    return "";
}


string analyze( string& word )
{
    int i = static_cast<int>(word.length());
    int isUp = 0, frontPunct = 0, backPunct = 0;
    string pigLatin;
    

    //checking for punctuation (leading/trailing)
    string punctf = punctuation( frontPunct, word) ;
    string punctb = punctuation( i, backPunct, word );
    
    if( isupper(word[frontPunct]) ) //checking capitalization
    {
        word[frontPunct] = tolower(word[frontPunct]);
        isUp = 1;
    }
   
    //case with no punctuation
    if( frontPunct == 0 && backPunct == 0 )
    {
        int n = 0;
        char c = word[n];
        
        switch ( c ) {
            case 'a':
            case 'e':
            case 'i':
            case 'o':
            case 'u':
                pigLatin = word+"way";
                if( isUp > n )
                {
                    pigLatin[n] = toupper( pigLatin[n] );
                }
                
                break;
                
            default:
                pigLatin = word.substr(1, i-1)+c+"ay";
                if( isUp > n )
                {
                    pigLatin[n] = toupper( pigLatin[n] );
                }
                
                break;
        }
        
    }
    
    else
    {
        char c = word[frontPunct];
        
        switch ( c ) {
            case 'a':
            case 'e':
            case 'i':
            case 'o':
            case 'u':
                if( frontPunct > 0 && backPunct > 0 )// e.g. i = 9, p = 2, i-p = 7
                {
                    pigLatin = punctf+word.substr( frontPunct, i-(backPunct+frontPunct)) + "way" +punctb;
                    

                }
                else if (frontPunct > 0 && backPunct < 1 )
                {
                    pigLatin = punctf+word.substr( frontPunct, i-frontPunct ) + "way";
                 

                }
                
                else
                {
                    pigLatin = word.substr(0, i-backPunct) + "way"+punctb;
                 
                }
                
                if( isUp > 0 )
                {
                    pigLatin[frontPunct] = toupper( pigLatin[frontPunct] );
                }
                
                break;
                
            default:
                if( frontPunct > 0 && backPunct > 0 )// e.g. i = 3, p.length = 1, i-1-p.length = 1
                {
                    pigLatin = punctf+word.substr(frontPunct+1, i-(backPunct+frontPunct)-1 ) + word[frontPunct] + "ay"+punctb;
                    
                }
                else if (frontPunct > 0 && backPunct < 1 ) // n = 2, i = 9 [0][1] - substr at [2] counting to i-punct.
                {
                    pigLatin = punctf+word.substr(frontPunct+1, i-frontPunct-1) + word[frontPunct] + "ay";
                 
                }
                
                else
                {
                    pigLatin = word.substr(frontPunct+1, i-backPunct-1) + word[frontPunct] + "ay"+punctb;
                
                }
                
                if( isUp > 0 )
                {
                    pigLatin[frontPunct] = toupper( pigLatin[frontPunct] );
                }
                

                
                break;
        }
        
    }
    
    return pigLatin;

        
}

/*void readLine( string& line )
{
    unsigned long i = 0, stopPos = 0, wordPos = 0;
    string word;
    for (i = line.find(" ", 0); i != string::npos; i = line.find(" ", i)) 
    {
        if( !line[i+1] )
        {
            i++;
            continue;
        }
        else
        {
            wordPos = i - stopPos;
            word = line.substr(i, (line.length()-1)-i);
        }
        
    }
    
     string word;
     stringstream str;
     
     while( str << line )
     {
         str >> word;
         string pigWord = translate( word );
         
     }
}*/