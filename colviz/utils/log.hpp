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
#ifndef LOG_H
#define LOG_H

class Log
{
  public:

    Log ( )
    {
    }

    ~Log ( )
    {
    }

    QString showPlugins ( const std::vector<plugArgs> &pluginList )
    {
      QString text = "";

      if ( !pluginList.empty( ) )
      {
        for ( unsigned int i = 0; i < pluginList.size( ); ++i )
        {
          text.append( "Plugin Id: " + pluginList.at( i ).name + "\n" );
        }
      }
      text.append(
          "Number of plugins: " + QString::number( pluginList.size( ) )
              + "\n" );
      return text;
    }

    QString addToLog ( QString pMessage )
    {
      return pMessage;
    }

};

#endif //LOG
