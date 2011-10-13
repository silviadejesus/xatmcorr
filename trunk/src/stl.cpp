#include "stl.h"


float StrToFloat(string s) {
    istringstream iss(s);
    float value;
    if (! (iss>>value)) {
        return 0;
    } else {
        return value;
    }
}

std::string FloatToStr(double f) {
  stringstream ss (stringstream::in | stringstream::out);
  ss << f;
  return ss.str();
}

string trim_right (const string & s, const string & t )
  {
  string d (s);
  string::size_type i (d.find_last_not_of (t));
  if (i == string::npos)
    return "";
  else
   return d.erase (d.find_last_not_of (t) + 1) ;
  }  // end of trim_right

string trim_left (const string & s, const string & t )
  {
  string d (s);
  return d.erase (0, s.find_first_not_of (t)) ;
  }  // end of trim_left

string trim (const string & s, const string & t )
  {
  string d (s);
  return trim_left (trim_right (d, t), t) ;
  }  // end of trim

// returns a lower case version of the string
string tolower (const string & s) {
string d (s);

  transform (d.begin (), d.end (), d.begin (), (int(*)(int)) tolower);
  return d;
  }  // end of tolower

// returns an upper case version of the string
string toupper (const string & s) {
string d (s);

  transform (d.begin (), d.end (), d.begin (), (int(*)(int)) toupper);
  return d;
  }   // end of toupper

// transformation function for tocapitals that has a "state"
// so it can capitalise a sequence
class fCapitals : public unary_function<char,char> {
  bool bUpper;

  public:

  // first letter in string will be in capitals
  fCapitals () : bUpper (true) {}; // constructor

  char operator() (const char & c)
    {
    char c1;
    // capitalise depending on previous letter
    if (bUpper)
      c1 = toupper (c);
    else
      c1 = tolower (c);

    // work out whether next letter should be capitals
    bUpper = isalnum (c) == 0;
    return c1;
    }
  };  // end of class fCapitals

// returns a capitalized version of the string
string tocapitals (const string & s)
  {
string d (s);

  transform (d.begin (), d.end (), d.begin (), fCapitals ());
  return d;
  }  // end of tocapitals


// split a line into the first word, and rest-of-the-line
string GetWord (string & s,
                const string delim = " ",
                const bool trim_spaces = true)
  {

  // find delimiter
  string::size_type i (s.find (delim));

  // split into before and after delimiter
  string w (s.substr (0, i));

  // if no delimiter, remainder is empty
  if (i == string::npos)
    s.erase ();
  else
    // erase up to the delimiter
    s.erase (0, i + delim.size ());

  // trim spaces if required
  if (trim_spaces)
    {
    w = trim (w);
    s = trim (s);
    }

  // return first word in line
  return w;

  } // end of GetWord

// To be symmetric, we assume an empty string (after trimming spaces)
// will give an empty vector.
// However, a non-empty string (with no delimiter) will give one item
// After that, you get an item per delimiter, plus 1.
// eg.  ""      => empty
//      "a"     => 1 item
//      "a,b"   => 2 items
//      "a,b,"  => 3 items (last one empty)

void StringToVector (const string s,
                     vector<string> & v,
                     const string delim,
                     const bool trim_spaces)
  {

  // start with initial string, trimmed of leading/trailing spaces if required
  string s1 (trim_spaces ? trim (s) : s);

  v.clear (); // ensure vector empty

  // no string? no elements
  if (s1.empty ())
    return;

  // add to vector while we have a delimiter
  while (!s1.empty () && s1.find (delim) != string::npos)
    v.push_back (GetWord (s1, delim, trim_spaces));

  // add final element
  v.push_back (s1);
  } // end of StringToVector

// Takes a vector of strings and converts it to a string
// like "apples,peaches,pears"
// Should be symmetric with StringToVector (excepting any spaces that might have
//  been trimmed).

string VectorToString (const vector<string> & v,
                       const string delim)
  {
  // vector empty gives empty string
  if (v.empty ())
    return "";

  // for copying results into
  ostringstream os;

  // copy all but last one, with delimiter after each one
  copy (v.begin (), v.end () - 1,
        ostream_iterator<string> (os, delim.c_str ()));

  // return string with final element appended
  return os.str () + *(v.end () - 1);

  } // end of VectorToString

tm StrToDate(string strDate) {
    vector<string> pieces,pieces2,pieces3;
    tm res={0};
    StringToVector(strDate,pieces,"-");
    res.tm_mday=atoi(pieces[2].c_str());
    res.tm_mon=atoi(pieces[1].c_str());
    res.tm_year=atoi(pieces[0].c_str());
    int pos=strDate.find("T");
    if (pos>0) {
        StringToVector(strDate,pieces2,"T");
        string t=pieces2[1];
        StringToVector(t,pieces3,":");
        res.tm_sec=atoi(pieces3[2].c_str());
        res.tm_min=atoi(pieces3[1].c_str());
        res.tm_hour=atoi(pieces3[0].c_str());
        
    } else {
        res.tm_sec=0;
        res.tm_min=0;
        res.tm_hour=0;
    }
    return res;
}

double toJulian(int day, int month,int year, int hour, int minute, int second) {
    int a = (14 - month) / 12;
    int y = year + 4800 - a;
    int m = month + (12 * a) - 3;
    // for a date in the Gregorian calendar (at noon)
    int jdn = day + (((153 * m) + 2) / 5) + (365 * y) + (y / 4) - (y / 100) + (y / 400) - 32045;
    return  jdn + ((hour - 12) / 24) + (minute / 1440) + (second / 86400);
}

double toJulian(tm dateTime) {
    return toJulian(dateTime.tm_mday,dateTime.tm_mon, dateTime.tm_year, dateTime.tm_hour,dateTime.tm_min,dateTime.tm_sec);
}