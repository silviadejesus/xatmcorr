#ifndef STL_H
#define STL_H
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <iterator>
#include <algorithm>
#include <functional>
#include <ctime>

using namespace std;
#define SPACES " \t\r\n"
string trim_right (const string & s, const string & t = SPACES);
string trim_left (const string & s, const string & t = SPACES);
string trim (const string & s, const string & t = SPACES);
string tolower (const string & s);
string toupper (const string & s);
void StringToVector (const string s,
                     vector<string> & v,
                     const string delim = " ",
                     const bool trim_spaces = true);
string VectorToString (const vector<string> & v,
                       const string delim = " ");
float StrToFloat (string);
std::string FloatToStr(double);
tm StrToDate(string);
double toJulian(int day, int month,int year, int hour=0, int minute=0, int second=0);
double toJulian(tm dateTime);

//For Each item in a STL container
#define foreach(m_itname,m_container) \
    for( typeof(m_container.begin()) m_itname=m_container.begin() ; \
    m_itname!=m_container.end() ; \
    m_itname++ )


#endif // STL_H
// disable warnings about long names

#define  Round(x)  ((int)(x+0.5))

#define print(x) std::cout<<x<<std::endl;
#define pi 3.14159265

