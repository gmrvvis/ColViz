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

#include "Distribution.h"
#include <iostream>

using namespace std;

namespace NSUtils
{
  /**
   *
   */
  Distribution::Distribution ( )
  {
    fileVersion = 0.03;
  }

  Distribution::~Distribution ( )
  {

  }

  /**
   * Generate the distribution with format: Position(x,y,z), rotation angle, neuron type.
   */
  void Distribution::generateDistribution ( unsigned int XDim,
      unsigned int YDim, unsigned int ZDim )
  {

  }

  /*
   * Generate the files with the distribution with format: Position(x,y,z), rotation angle, neuron type.
   */
  void Distribution::saveToFile ( std::string pathFile )
  {
    string fileTXT = pathFile + ".txt";
    string fileBIN = pathFile + ".bin";

    std::ofstream( fileTXT.c_str( ) );
    std::ofstream( fileBIN.c_str( ) );

    outputFileTXT.open( fileTXT.c_str( ), std::ios::app );
    outputFileBIN.open( fileBIN.c_str( ), std::ofstream::binary );

    outputFileTXT << fileVersion << endl;
    outputFileBIN.write( ( char * ) &fileVersion, sizeof(float) );

    unsigned int totalNeurons = mNodeContainer.size( );
    outputFileTXT << mNodeContainer.size( ) << endl;
    outputFileBIN.write( ( char * ) &totalNeurons, sizeof(unsigned int) );

    NodeDist lNodeNDist;
    float lAuxPosX, lAuxPosY, lAuxPosZ;

    for ( unsigned int i = 0; i < mNodeContainer.size( ); ++i )
    {
      lNodeNDist = mNodeContainer.at( i );

      outputFileTXT << lNodeNDist.mPos.x << " " << lNodeNDist.mPos.y << " "
          << lNodeNDist.mPos.z << " " << lNodeNDist.mAngle << " "
          << lNodeNDist.mScale << " " << lNodeNDist.mName << endl;

      lAuxPosX = lNodeNDist.mPos.x;
      lAuxPosY = lNodeNDist.mPos.y;
      lAuxPosZ = lNodeNDist.mPos.z;

      outputFileBIN.write( ( char * ) &lAuxPosX, sizeof(float) );
      outputFileBIN.write( ( char * ) &lAuxPosY, sizeof(float) );
      outputFileBIN.write( ( char * ) &lAuxPosZ, sizeof(float) );
      outputFileBIN.write( ( char * ) &lNodeNDist.mAngle, sizeof(int) );
      outputFileBIN.write( ( char * ) &lNodeNDist.mScale, sizeof(float) );
      outputFileBIN.write( lNodeNDist.mName.c_str( ), 9 * sizeof(char) );
    }

    outputFileTXT.close( );
    outputFileBIN.close( );
  }

  void Distribution::loadFromFile ( std::string pathFile )
  {
    unsigned int lIdNeuron = 150;

    string lAux = " ";

    char * lNameBuff = new char[lIdNeuron];
    float lPosAux[3];

    NodeDist lNodeNDist;
    mNodeContainer.clear( );

    std::ifstream inputFileTXT;
    inputFileTXT.open( pathFile.c_str( ), std::ios::in );

    inputFileTXT >> fileVersion;

    unsigned int totalNeurons;
    inputFileTXT >> totalNeurons;

    while ( !inputFileTXT.eof( ) )
    {
      inputFileTXT >> lPosAux[0];
      inputFileTXT >> lPosAux[1];
      inputFileTXT >> lPosAux[2];

      lNodeNDist.mPos.x = lPosAux[0];
      lNodeNDist.mPos.y = lPosAux[1];
      lNodeNDist.mPos.z = lPosAux[2];

      inputFileTXT >> lNodeNDist.mAngle;
      inputFileTXT >> lNodeNDist.mScale;

      inputFileTXT.get( lNameBuff, lIdNeuron * sizeof(char), '\n' );
      lNodeNDist.mName = lNameBuff;
      lAux = lNameBuff;
      lNodeNDist.mName = "";
      lNodeNDist.mName += Trim( lAux.begin( ), lAux.end( ) );
      mNodeContainer.push_back( lNodeNDist );
    }

    mNodeContainer.pop_back( );

    delete lNameBuff;
    inputFileTXT.close( );
  }
}
