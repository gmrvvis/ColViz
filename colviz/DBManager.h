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

#pragma once
#ifndef COLVIZ_DBMANAGER_H
#define COLVIZ_DBMANAGER_H

#include <stdlib.h>
#include <iostream>
#include <sstream>
#include <fstream>
#include <stdexcept>
#include <vector>

namespace detail
{
  class DBManager;
};

class DBManager
{
    detail::DBManager *_impl;

    std::string DBHost;
    std::string DBUser;
    std::string DBPass;
    std::string DBSchema;

    int mUserId;
    int mSession;

  public:
    DBManager ( const std::string & pFile );
    ~DBManager ( );

    void simpleQuery ( );

    bool logIn ( const std::string & name, const std::string & pass );

    void loadSessions ( std::vector<std::string> & content );

    void loadDialogs ( unsigned int pSessionId,
        std::vector<std::string> & content );

    void storeMessage ( const std::string & message );

    int getUserId ( )
    {
      return mUserId;
    }
    ;

    std::string getUserName ( );

    int getSessionId ( )
    {
      return mSession;
    }
    ;

  private:
    void parseFromFile ( const std::string & pFile );

};

#endif // COLVIZ_DBMANAGER_H
