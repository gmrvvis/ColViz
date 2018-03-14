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

#ifndef BASEMESH_H
#define BASEMESH_H

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

#include "MeshDef.hpp"

namespace NSUtils
{

  class BaseMesh
  {
    protected:
      MeshDef * mMesh;

      unsigned int mNumTris;
      unsigned int mNumQuads;
      unsigned int mNumVertex;
      unsigned int mNumFaces;

      std::vector<unsigned int> mTriIndices;
      std::vector<unsigned int> mQuadIndices;

      unsigned char * mTexture;
      int mTextureWidth;
      int mTextureHeight;

      unsigned int calcNumVertex ( );
      unsigned int calcNumFaces ( );
      unsigned int calcNumTri ( );
      unsigned int calcNumQuads ( );
      void calcTriIndices ( );
      void calcQuadIndices ( );

      void prepare ( );

    public:
      BaseMesh ( );

      BaseMesh ( std::string pMeshName );

      ~BaseMesh ( );

      MeshDef& getMesh ( )
      {
        return *mMesh;
      }
      const MeshDef& getMesh ( ) const
      {
        return *mMesh;
      }

      void loadMesh ( std::string pMeshName );

      unsigned int getNumTri ( )
      {
        return mNumTris;
      }
      ;
      unsigned int getNumQuads ( )
      {
        return mNumQuads;
      }
      ;
      unsigned int getNumVertexs ( )
      {
        return mNumVertex;
      }
      ;
      unsigned int getNumFaces ( )
      {
        return mNumFaces;
      }
      ;

      std::vector<unsigned> getTriIndices ( )
      {
        return mTriIndices;
      }
      ;
      std::vector<unsigned> getQuadIndices ( )
      {
        return mQuadIndices;
      }
      ;

  };
}
#endif // BASEMESH_H
