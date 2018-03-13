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

#include "ColViz.h"
#include <rapidjson/document.h>
#include <rapidjson/prettywriter.h>
#include <memory>

namespace detail
{
  class ColViz
  {
  public:
    rapidjson::Document mJsonCfg;
    rapidjson::Document mJsonTmp;
  };
}

ColViz::ColViz ( const std::string & pFile )
{
  _impl = new detail::ColViz;

  parseFromFile( pFile );
  if ( strcmp( _impl->mJsonCfg["type"].GetString( ), "nettCfg" ) == 0 )
  {
    const std::string endpoint(
        std::string( _impl->mJsonCfg["protocol"].GetString( ) ) + "://"
            + _impl->mJsonCfg["source"]["ip"].GetString( ) + ":"
            + _impl->mJsonCfg["source"]["port"].GetString( ) );

    mDestinyEndPoint = mSourceEndPoint = endpoint;

    if ( mShowMessages ) std::cout << "ColViz--->>> Calculated Source endpoint:"
        << mSourceEndPoint << std::endl;

    nett::initialize( mSourceEndPoint );

    if ( !_impl->mJsonCfg["destiny"].IsNull( ) )
    {
      mDestinyEndPoint = _impl->mJsonCfg["protocol"].GetString( );
      mDestinyEndPoint += "://";
      mDestinyEndPoint += _impl->mJsonCfg["destiny"]["ip"].GetString( );
      mDestinyEndPoint += ":";
      mDestinyEndPoint += _impl->mJsonCfg["destiny"]["port"].GetString( );

      if ( mShowMessages ) std::cout
          << "ColViz--->>>Calculated Destiny endpoint:" << mDestinyEndPoint
          << std::endl;
    }
  }
  //ZeroEQ configuration
  else
  {
    std::cout << "ColViz--->>>Cfg:" << _impl->mJsonCfg["type"].GetString( )
        << std::endl;
    //On-going! ...
  }
}

ColViz::~ColViz ( )
{
  delete _impl;
}

void ColViz::parseFromFile ( const std::string & pFile )
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
        std::cout << "Error parsing file:" + pFile + "\n";
        break;
      default:
        break;
    }
  }

  file.close( );
}

std::string ColViz::receiveMassage ( )
{
  static bool flag = false;
  static auto slotIn = nett::make_slot_in<string_message>( );

  if ( !flag )
  {
    slotIn->connect( nett::slot_address( mDestinyEndPoint, "string_event" ) );
    flag = true;
  }

  return slotIn->receive( ).value( );

}

void ColViz::prepareMessage ( const std::string & json )
{
  rapidjson::StringBuffer sb;
  if ( _impl->mJsonTmp.Parse( json.c_str( ) ).HasParseError( ) ) throw 0;
  rapidjson::PrettyWriter<rapidjson::StringBuffer> writer( sb );
  _impl->mJsonTmp.Accept( writer );
  message.set_value( sb.GetString( ) );
}

void ColViz::sendSingleValue ( int pValue )
{
  static auto slotOut = nett::make_slot_out<string_message>( "string_event" );

  std::string json(
      "{\"type\":\"singleValue\",\"val\":" + std::to_string( pValue ) + "}" );
  prepareMessage( json );

  if ( mShowMessages ) std::cout << "ColViz--->>> Sent Value:"
      << message.value( ) << std::endl;

  slotOut->send( message );
}

void ColViz::sendCamera2D ( unsigned int pX, unsigned int pY )
{
  static auto slotOut = nett::make_slot_out<string_message>( "string_event" );

  //Build JSON
  std::string json(
      "{\"type\":\"camera2D\",\"val\":[" + std::to_string( pX ) + ","
          + std::to_string( pY ) + "] }" );
  prepareMessage( json );

  if ( mShowMessages ) std::cout << "ColViz--->>> Sent Value:"
      << message.value( ) << std::endl;

  slotOut->send( message );
}

void ColViz::sendCamera3D ( const std::vector<float> & pCamParams )
{
  static auto slotOut = nett::make_slot_out<string_message>( "string_event" );

  //Build JSON
  std::string json( "{\"type\":\"camera3D\",\"val\":[" );
  std::string strAux = std::to_string( pCamParams[0] );
  std::replace( strAux.begin( ), strAux.end( ), ',', '.' );
  json += strAux;
  for ( size_t i = 1; i < pCamParams.size( ); ++i )
  {
    std::string strAux_ = std::to_string( pCamParams[i] );
    std::replace( strAux_.begin( ), strAux_.end( ), ',', '.' );
    json += "," + strAux_;
  }
  json += "] }";

  prepareMessage( json );

  if ( mShowMessages ) std::cout << "ColViz--->>> Sent Value:"
      << message.value( ) << std::endl;

  slotOut->send( message );
}

void ColViz::sendSelection ( const std::vector<int> & pSelection )
{
  if ( pSelection.size( ) > 0 )
  {
    static auto slotOut = nett::make_slot_out<string_message>( "string_event" );
    std::string json(
        "{ \"type\":\"selection\", \"val\":["
            + std::to_string( pSelection[0] ) );
    for ( size_t i = 1; i < pSelection.size( ); ++i )
    {
      json += "," + std::to_string( pSelection[i] );
    }
    json += "] }";

    prepareMessage( json );

    if ( mShowMessages ) std::cout << "ColViz--->>> Sent Value:"
        << message.value( ) << std::endl;

    slotOut->send( message );
  }
}

void ColViz::sendText ( const std::string & pText )
{
  static auto slotOut = nett::make_slot_out<string_message>( "string_event" );
  std::string json( "{\"type\":\"text\",\"val\":\"" + pText + "\"}" );
  prepareMessage( json );

  if ( mShowMessages ) std::cout << "ColViz--->>> Sent Value:"
      << message.value( ) << std::endl;

  slotOut->send( message );
}
