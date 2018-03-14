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

#include "BaseMesh.h"

namespace NSUtils
{
  BaseMesh::BaseMesh ( )
  {
    mTexture = NULL;
    mTextureWidth = mTextureHeight = 0;

    mNumTris = mNumQuads = mNumVertex = mNumFaces = 0;

    mMesh = new MeshDef( );

    mMesh->request_face_normals( );
    mMesh->request_face_colors( );
    mMesh->request_vertex_normals( );
    mMesh->request_vertex_colors( );
    mMesh->request_vertex_texcoords2D( );
    mMesh->request_edge_status( );
    mMesh->request_face_status( );
    mMesh->request_vertex_status( );

  }

  BaseMesh::BaseMesh ( std::string pMeshName )
  {
    mTexture = NULL;
    mTextureWidth = mTextureHeight = 0;

    mNumTris = mNumQuads = mNumVertex = mNumFaces = 0;

    mMesh = new MeshDef( );

    mMesh->request_face_normals( );
    mMesh->request_face_colors( );
    mMesh->request_vertex_normals( );
    mMesh->request_vertex_colors( );
    mMesh->request_vertex_texcoords2D( );
    mMesh->request_edge_status( );
    mMesh->request_face_status( );
    mMesh->request_vertex_status( );

    loadMesh( pMeshName );
    prepare( );
  }

  BaseMesh::~BaseMesh ( )
  {
    delete mMesh;
  }

  void BaseMesh::loadMesh ( std::string pMeshName )
  {
    OpenMesh::IO::Options opt;
    if ( !OpenMesh::IO::read_mesh( *mMesh, pMeshName, opt ) )
    {
      std::cerr << "Error Reading File\n";
      delete mMesh;
      throw;
    }

    if ( !opt.check( OpenMesh::IO::Options::FaceNormal ) ) mMesh->update_face_normals( );
    else
      std::cout << "File provides face normals\n";

    if ( !opt.check( OpenMesh::IO::Options::VertexNormal ) ) mMesh->update_vertex_normals( );
    else
      std::cout << "File provides vertex normals\n";

    opt.clear( );
  }

  unsigned int BaseMesh::calcNumVertex ( )
  {
    mNumVertex = 0;
    for ( MeshDef::ConstVertexIter v_it = mMesh->vertices_begin( );
        v_it != mMesh->vertices_end( ); ++v_it )
    {
      ++mNumVertex;
    }
    return mNumVertex;
  }

  unsigned int BaseMesh::calcNumFaces ( )
  {
    mNumFaces = ( mNumTris + mNumQuads );
    return mNumFaces;
  }

  unsigned int BaseMesh::calcNumTri ( )
  {
    int numTri = 0;
    for ( MeshDef::ConstFaceIter cfi = mMesh->faces_sbegin( );
        cfi != mMesh->faces_end( ); ++cfi )
    {
      if ( mMesh->valence( *cfi ) == 3 ) ++numTri;
    }
    mNumTris = numTri;
    return mNumTris;
  }

  unsigned int BaseMesh::calcNumQuads ( )
  {
    int numQuads = 0;
    for ( MeshDef::ConstFaceIter cfi = mMesh->faces_sbegin( );
        cfi != mMesh->faces_end( ); ++cfi )
    {
      if ( mMesh->valence( *cfi ) == 4 ) ++numQuads;
    }
    mNumQuads = numQuads;
    return mNumQuads;
  }

  void BaseMesh::calcTriIndices ( )
  {
    mTriIndices.clear( );
    mTriIndices.reserve( 3 * getNumTri( ) );

    MeshDef::ConstFaceIter f, fEnd = mMesh->faces_end( );
    MeshDef::ConstFaceVertexIter v;

    for ( f = mMesh->faces_begin( ); f != fEnd; ++f )
    {
      if ( mMesh->valence( *f ) != 3 ) // ignore non-triangles
      continue;
      for ( v = mMesh->cfv_iter( *f ); v.is_valid( ); ++v )
      {
        mTriIndices.push_back( v->idx( ) );
      }
    }

    assert( mTriIndices.size( ) == 3 * getNumTri( ) );
  }

  void BaseMesh::calcQuadIndices ( )
  {
    mQuadIndices.clear( );
    mQuadIndices.reserve( 4 * getNumQuads( ) );

    MeshDef::ConstFaceIter f, fEnd = mMesh->faces_end( );
    MeshDef::ConstFaceVertexIter v;

    for ( f = mMesh->faces_begin( ); f != fEnd; ++f )
    {
      if ( mMesh->valence( *f ) != 4 ) // ignore non-quads
      continue;
      for ( v = mMesh->cfv_iter( *f ); v.is_valid( ); ++v )
      {
        mQuadIndices.push_back( v->idx( ) );
      }
    }

    assert( mQuadIndices.size( ) == 4 * getNumQuads( ) );
  }

  void BaseMesh::prepare ( )
  {
    calcNumTri( );
    calcNumQuads( );

    calcTriIndices( );
    calcQuadIndices( );

    calcNumVertex( );
    calcNumFaces( );
  }
}
