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


#include "auxtable.h"

auxTable::auxTable() {

    
}

bool auxTable::readAuxTable(const char*  bandNumber, const char*  sensor, const char* satellite, tm timeStamp) {
    std::string line;
    std::ifstream myfile("radiancia.csv");
    if (myfile.is_open()) {
        while (! myfile.eof() ) {
            getline (myfile,line);
            std::vector<std::string> params;
            StringToVector(line,params,",");
            if (params.size()>1) {
                if (!params[0].compare(satellite) && !params[1].compare(sensor) && !params[2].compare(bandNumber) ) {
                    this->lmin=StrToFloat(params[3]);
                    this->lmax=StrToFloat(params[4]);
                    this->esun=StrToFloat(params[5]);
                    this->id6s=atoi(params[6].c_str());
                    this->dnMin=StrToFloat(params[7]);
                    this->dnMax=StrToFloat(params[8]);
                    this->geometry=atoi(params[9].c_str());
                    this->dateMin=StrToDate(params[10]);
                    this->dateMax=StrToDate(params[11]);
                    if ((toJulian(this->dateMin)<toJulian(timeStamp)) && (toJulian(this->dateMax)>toJulian(timeStamp)) ) {
                        std::cout<<satellite<<" "<<sensor <<" "<<bandNumber <<" "<<params[10]<<" "<<params[11]<<std::endl;
                        return true;
                    }
                    //std::cout<<line<<std::endl;
                }
            }
        }
        myfile.close();
    }
	return true;
}
