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

#include "DBManager.h"
#include "mysql_connection.h"

#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>
#include <cppconn/prepared_statement.h>

#include <rapidjson/document.h>
#include <rapidjson/prettywriter.h>


namespace detail
{
  class DBManager
  {
  public:
    rapidjson::Document mJsonCfg;
    sql::Driver *driver;
    sql::Connection *con;
    sql::Statement *stmt;
    sql::ResultSet *res;
    sql::PreparedStatement *pstmt;
  };
}


DBManager::DBManager ( const std::string & pFile )
{
  _impl = new detail::DBManager;

  mUserId = -1;
  try
  {
    parseFromFile( pFile );

    const std::string endpoint(
        std::string( _impl->mJsonCfg["protocol"].GetString( ) ) + "://"
            + _impl->mJsonCfg["database"]["ip"].GetString( ) + ":"
            + _impl->mJsonCfg["database"]["port"].GetString( ) );

    DBUser = _impl->mJsonCfg["database"]["user"].GetString( );
    DBPass = _impl->mJsonCfg["database"]["pass"].GetString( );
    DBSchema = _impl->mJsonCfg["database"]["schema"].GetString( );

    _impl->driver = get_driver_instance( );
    _impl->con = _impl->driver->connect( endpoint, DBUser, DBPass );
    _impl->con->setSchema( DBSchema );
  }
  catch ( sql::SQLException &e )
  {
    /*
     MySQL Connector/C++ throws three different exceptions:
     - sql::MethodNotImplementedException (derived from sql::SQLException)
     - sql::InvalidArgumentException (derived from sql::SQLException)
     - sql::SQLException (derived from std::runtime_error)
     */
    std::cout << "# ERR: SQLException in " << __FILE__;
    std::cout << "(" << __FUNCTION__ << ") on line " << __LINE__ << std::endl;
    /* what() (derived from std::runtime_error) fetches error message */
    std::cout << "# ERR: " << e.what( );
    std::cout << " (MySQL error code: " << e.getErrorCode( );
    std::cout << ", SQLState: " << e.getSQLState( ) << " )" << std::endl;
    throw 1;
  }
  catch ( ... )
  {
    std::cerr << "Propagating exception ..."<< std::endl;
    throw 0;
  }
}

DBManager::~DBManager ( )
{
  delete _impl->con;
  delete _impl;
}

void DBManager::parseFromFile ( const std::string & pFile )
{
  std::ifstream file;
  std::string str;
  std::string file_contents;

  try
  {
    file.open( pFile );
    while ( std::getline( file, str ) )
    {
      file_contents += str; // + "\n";
    }

    if ( _impl->mJsonCfg.Parse( file_contents.c_str( ) ).HasParseError( ) ) throw ( 0 );
  }
  catch ( const std::ifstream::failure& e )
  {
    std::cout << "Exception opening/reading file:" + pFile + "\n";
  }
  catch ( int e )
  {
    switch ( e )
    {
      case 0:
        std::cout << "Error parsing config Persistence System file:" + pFile + "\n";
        throw 0;
        break;
      default:
        break;
    }
  }

  file.close( );
}

void DBManager::simpleQuery ( )
{
  _impl->pstmt = _impl->con->prepareStatement( "SELECT * FROM USERS ORDER BY ID ASC" );
  _impl->res = _impl->pstmt->executeQuery( );

  while ( _impl->res->next( ) )
  {
    std::cout << "\t... MySQL counts: " << _impl->res->getInt( "ID" )
        << _impl->res->getString( "NAME" ) << std::endl;
  }

  delete _impl->res;
  delete _impl->pstmt;
}

bool DBManager::logIn ( const std::string & name, const std::string & pass )
{
  _impl->pstmt = _impl->con->prepareStatement(
      "SELECT ID FROM USERS WHERE NAME='" + name + "' AND USERPASSWORD='" + pass
          + "';" );
  _impl->res = _impl->pstmt->executeQuery( );

  while ( _impl->res->next( ) )
  {
    mUserId = _impl->res->getInt( "ID" );
  }

  delete _impl->res;
  delete _impl->pstmt;

  if ( mUserId != -1 ) return true;
  else
    return false;
}

void DBManager::loadSessions ( std::vector<std::string> & content )
{
  _impl->pstmt = _impl->con->prepareStatement(
      "SELECT ID, DESCRIPTION FROM SESSIONS WHERE USERID="
          + std::to_string( mUserId ) + ";" );
  _impl->res = _impl->pstmt->executeQuery( );

  while ( _impl->res->next( ) )
  {
    content.push_back( _impl->res->getString( "DESCRIPTION" ) );
  }

  delete _impl->res;
  delete _impl->pstmt;
}

void DBManager::loadDialogs ( unsigned int pSessionId,
    std::vector<std::string> & content )
{
  _impl->pstmt =
      _impl->con->prepareStatement(
          "SELECT USERS.NAME AS NAME, DIALOGS.MESSAGE AS MESSAGE FROM DIALOGS INNER JOIN USERS where USERS.ID = DIALOGS.USERID and DIALOGS.SESSIONID="
              + std::to_string( pSessionId ) + ";" );
  _impl->res = _impl->pstmt->executeQuery( );

  while ( _impl->res->next( ) )
  {
    content.push_back(
        "[" + _impl->res->getString( "NAME" ) + "]: " + _impl->res->getString( "MESSAGE" ) );
  }

  mSession = pSessionId;

  delete _impl->res;
  delete _impl->pstmt;
}

void DBManager::storeMessage ( const std::string & message )
{
  _impl->pstmt = _impl->con->prepareStatement(
      "INSERT INTO DIALOGS (MESSAGE, USERID, SESSIONID) VALUES('" + message
          + "'," + std::to_string( mUserId ) + ", " + std::to_string( mSession )
          + ")" );
  _impl->pstmt->executeUpdate( );
}

std::string DBManager::getUserName ( )
{
  _impl->pstmt = _impl->con->prepareStatement(
      "SELECT NAME FROM USERS WHERE ID = " + std::to_string( mUserId ) + ";" );
  _impl->res = _impl->pstmt->executeQuery( );
  _impl->res->next( );
  return _impl->res->getString( "NAME" );
}
