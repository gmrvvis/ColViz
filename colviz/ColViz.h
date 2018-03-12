/*
 * Copyright (c) 2017 CCS/GMRV/UPM/URJC.
 *
 * Author: Juan P. Brito <juanpedro.brito@upm.es>
 *
 * This library is free software; you can redistribute it and/or modify it under
 * the terms of the GNU Lesser General Public License version 3.0 as published
 * by the Free Software Foundation.
 *
 * This library is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public License for more
 * details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this library; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 *
 */

#ifndef __COLVIZ__COLVIZ_H
#define __COLVIZ__COLVIZ_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>

#include <nett/nett.h>
#include <colviz/string_message.pb.h>

#include <rapidjson/document.h>
#include <rapidjson/prettywriter.h>

#include "helpers.hpp"

class ColViz
{
    rapidjson::Document mJsonCfg;
    rapidjson::Document mJsonTmp;

    std::string mSourceEndPoint;
    std::string mDestinyEndPoint;

    string_message message;

    bool mShowMessages = false;

    bool mPersistence = false;

  public:

    ColViz ( const std::string & pFile );

    ~ColViz ( );

    void showMessages ( bool pShow )
    {
      mShowMessages = pShow;
    }

    void persistence ( bool pPersist )
    {
      mPersistence = pPersist;
    }

    std::string receiveMassage ( );

    void sendSingleValue ( int pValue );

    void sendCamera2D ( unsigned int pX, unsigned int pY );

    void sendCamera3D ( const std::vector<float> & pCamParams );

    void sendSelection ( const std::vector<int> & pSelection );

    void sendText ( const std::string & pText );

  private:
    void parseFromFile ( const std::string & pFile );

    void prepareMessage ( const std::string & json );
};

#endif
