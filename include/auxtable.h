/*
    <one line to give the library's name and an idea of what it does.>
    Copyright (C) 2011  mauricio <mauricio@dpi.inpe.br>

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 2.1 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with this library; if not, write to the Free Software
    Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
*/


#ifndef AUXTABLE_H
#define AUXTABLE_H

#include <string>
#include <fstream>
#include <vector>
#include "stl.h"
//#include <ctime>
#include "qdatetime.h"

class auxTable {
public:
    double lmin,lmax,esun, dnMin, dnMax,ee0,rho_water;
    int id6s,geometry;
    QDate dateMin, dateMax;
    auxTable();
    bool readAuxTable(const char*  bandNumber, const char*  sensor,const char* satellite, QDateTime timeStamp,const char * csvPath);
    //res={lmin:0,lmax:0,esun:0,id6s:0, dnMin:1, dnMax:255}
    //return res
};

#endif // AUXTABLE_H
