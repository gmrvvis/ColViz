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

DBManager::DBManager ( const std::string & pFile )
{
  mUserId = -1;
  parseFromFile( pFile );
  try
  {
    const std::string endpoint(
        std::string( mJsonCfg["protocol"].GetString( ) ) + "://"
            + mJsonCfg["database"]["ip"].GetString( ) + ":"
            + mJsonCfg["database"]["port"].GetString( ) );

    DBUser = mJsonCfg["database"]["user"].GetString( );
    DBPass = mJsonCfg["database"]["pass"].GetString( );
    DBSchema = mJsonCfg["database"]["schema"].GetString( );

    driver = get_driver_instance( );
    con = driver->connect( endpoint, DBUser, DBPass );
    con->setSchema( DBSchema );
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

    //return EXIT_FAILURE;
  }
}

DBManager::~DBManager ( )
{
  delete con;
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

    if ( mJsonCfg.Parse( file_contents.c_str( ) ).HasParseError( ) ) throw ( 0 );
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
        std::cout << "Error parsing file:" + pFile + "\n";
        break;
      default:
        break;
    }
  }

  file.close( );
}

void DBManager::simpleQuery ( )
{
  pstmt = con->prepareStatement( "SELECT * FROM USERS ORDER BY ID ASC" );
  res = pstmt->executeQuery( );

  while ( res->next( ) )
  {
    std::cout << "\t... MySQL counts: " << res->getInt( "ID" )
        << res->getString( "NAME" ) << std::endl;
  }

  delete res;
  delete pstmt;
}

bool DBManager::logIn ( const std::string & name, const std::string & pass )
{
  pstmt = con->prepareStatement(
      "SELECT ID FROM USERS WHERE NAME='" + name + "' AND USERPASSWORD='" + pass
          + "';" );
  res = pstmt->executeQuery( );

  while ( res->next( ) )
  {
    mUserId = res->getInt( "ID" );
  }

  delete res;
  delete pstmt;

  if ( mUserId != -1 ) return true;
  else
    return false;
}

void DBManager::loadSessions ( std::vector<std::string> & content )
{
  pstmt = con->prepareStatement(
      "SELECT ID, DESCRIPTION FROM SESSIONS WHERE USERID="
          + std::to_string( mUserId ) + ";" );
  res = pstmt->executeQuery( );

  while ( res->next( ) )
  {
    content.push_back( res->getString( "DESCRIPTION" ) );
  }

  delete res;
  delete pstmt;
}

void DBManager::loadDialogs ( unsigned int pSessionId,
    std::vector<std::string> & content )
{
  pstmt =
      con->prepareStatement(
          "SELECT USERS.NAME AS NAME, DIALOGS.MESSAGE AS MESSAGE FROM DIALOGS INNER JOIN USERS where USERS.ID = DIALOGS.USERID and DIALOGS.SESSIONID="
              + std::to_string( pSessionId ) + ";" );
  res = pstmt->executeQuery( );

  while ( res->next( ) )
  {
    content.push_back(
        "[" + res->getString( "NAME" ) + "]: " + res->getString( "MESSAGE" ) );
  }

  mSession = pSessionId;

  delete res;
  delete pstmt;
}

void DBManager::storeMessage ( const std::string & message )
{
  pstmt = con->prepareStatement(
      "INSERT INTO DIALOGS (MESSAGE, USERID, SESSIONID) VALUES('" + message
          + "'," + std::to_string( mUserId ) + ", " + std::to_string( mSession )
          + ")" );
  pstmt->executeUpdate( );
}

std::string DBManager::getUserName ( )
{
  std::cout << "User Id" << mUserId << std::endl;
  pstmt = con->prepareStatement(
      "SELECT NAME FROM USERS WHERE ID = " + std::to_string( mUserId ) + ";" );
  res = pstmt->executeQuery( );
  res->next( );
  return res->getString( "NAME" );
}
