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

#include "BaseMeshRenderer.h"

namespace NSUtils
{
  BaseMeshRenderer::BaseMeshRenderer ( BaseMesh * pBaseMesh )
  {
    mBaseMesh = pBaseMesh;
    mDrawMode = DrawMode::SMOOTH;
  }

  void BaseMeshRenderer::draw ( )
  {
    if ( mBaseMesh == NULL ) return;

    typename MeshDef::ConstFaceIter fIt( mBaseMesh->getMesh( ).faces_begin( ) ),
        fEnd( mBaseMesh->getMesh( ).faces_end( ) );

    typename MeshDef::ConstFaceVertexIter fvIt;

    if ( mDrawMode == DrawMode::POINTS )
    {
      glEnableClientState (GL_VERTEX_ARRAY);
      glVertexPointer( 3, GL_FLOAT, 0, mBaseMesh->getMesh( ).points( ) );

      if ( mBaseMesh->getMesh( ).has_vertex_colors( ) )
      {
        glEnableClientState (GL_COLOR_ARRAY);
        glColorPointer( 3, GL_UNSIGNED_BYTE, 0,
            mBaseMesh->getMesh( ).vertex_colors( ) );
      }

      glDrawArrays( GL_POINTS, 0,
          static_cast<GLsizei>( mBaseMesh->getMesh( ).n_vertices( ) ) );
      glDisableClientState( GL_VERTEX_ARRAY );
      glDisableClientState (GL_COLOR_ARRAY);
    }
    else if ( mDrawMode == DrawMode::WIRE )
    {
      glBegin (GL_TRIANGLES);
      for ( ; fIt != fEnd; ++fIt )
      {
        fvIt = mBaseMesh->getMesh( ).cfv_iter( *fIt );
        glVertex3fv( &mBaseMesh->getMesh( ).point( *fvIt )[0] );
        ++fvIt;
        glVertex3fv( &mBaseMesh->getMesh( ).point( *fvIt )[0] );
        ++fvIt;
        glVertex3fv( &mBaseMesh->getMesh( ).point( *fvIt )[0] );
      }
      glEnd( );
    }
    else if ( mDrawMode == DrawMode::FLAT )
    {
      glBegin (GL_TRIANGLES);
      for ( ; fIt != fEnd; ++fIt )
      {
        glNormal3fv( &mBaseMesh->getMesh( ).normal( *fIt )[0] );

        fvIt = mBaseMesh->getMesh( ).cfv_iter( *fIt );
        glVertex3fv( &mBaseMesh->getMesh( ).point( *fvIt )[0] );
        ++fvIt;
        glVertex3fv( &mBaseMesh->getMesh( ).point( *fvIt )[0] );
        ++fvIt;
        glVertex3fv( &mBaseMesh->getMesh( ).point( *fvIt )[0] );
      }
      glEnd( );

    }

    else if ( mDrawMode == DrawMode::SMOOTH )
    {
      glEnableClientState (GL_VERTEX_ARRAY);
      glVertexPointer( 3, GL_FLOAT, 0, mBaseMesh->getMesh( ).points( ) );

      glEnableClientState (GL_NORMAL_ARRAY);
      glNormalPointer( GL_FLOAT, 0, mBaseMesh->getMesh( ).vertex_normals( ) );

      glBegin (GL_TRIANGLES);
      for ( ; fIt != fEnd; ++fIt )
      {
        fvIt = mBaseMesh->getMesh( ).cfv_iter( *fIt );
        glArrayElement( fvIt->idx( ) );
        ++fvIt;
        glArrayElement( fvIt->idx( ) );
        ++fvIt;
        glArrayElement( fvIt->idx( ) );
      }
      glEnd( );

      glDisableClientState( GL_VERTEX_ARRAY );
      glDisableClientState( GL_NORMAL_ARRAY );
      glDisableClientState (GL_TEXTURE_COORD_ARRAY);

    }

    else if ( mDrawMode == DrawMode::POINTSCOLOR )
    {
      glEnableClientState (GL_VERTEX_ARRAY);
      glVertexPointer( 3, GL_FLOAT, 0, mBaseMesh->getMesh( ).points( ) );

      glEnableClientState (GL_NORMAL_ARRAY);
      glNormalPointer( GL_FLOAT, 0, mBaseMesh->getMesh( ).vertex_normals( ) );

      if ( mBaseMesh->getMesh( ).has_vertex_colors( ) )
      {
        glEnableClientState (GL_COLOR_ARRAY);
        glColorPointer( 3, GL_UNSIGNED_BYTE, 0,
            mBaseMesh->getMesh( ).vertex_colors( ) );
      }

      glBegin (GL_TRIANGLES);
      for ( ; fIt != fEnd; ++fIt )
      {
        fvIt = mBaseMesh->getMesh( ).cfv_iter( *fIt );
        glArrayElement( fvIt->idx( ) );
        ++fvIt;
        glArrayElement( fvIt->idx( ) );
        ++fvIt;
        glArrayElement( fvIt->idx( ) );
      }
      glEnd( );

      glDisableClientState( GL_VERTEX_ARRAY );
      glDisableClientState( GL_NORMAL_ARRAY );
      glDisableClientState (GL_COLOR_ARRAY);
    }

    else if ( mDrawMode == DrawMode::FLATCOLORFACES )
    {
      glEnableClientState (GL_VERTEX_ARRAY);
      glVertexPointer( 3, GL_FLOAT, 0, mBaseMesh->getMesh( ).points( ) );

      glEnableClientState (GL_NORMAL_ARRAY);
      glNormalPointer( GL_FLOAT, 0, mBaseMesh->getMesh( ).vertex_normals( ) );

      glBegin (GL_TRIANGLES);
      for ( ; fIt != fEnd; ++fIt )
      {
        fvIt = mBaseMesh->getMesh( ).cfv_iter( *fIt );
        glArrayElement( fvIt->idx( ) );
        ++fvIt;
        glArrayElement( fvIt->idx( ) );
        ++fvIt;
        glArrayElement( fvIt->idx( ) );
      }
      glEnd( );

      glDisableClientState( GL_VERTEX_ARRAY );
      glDisableClientState( GL_NORMAL_ARRAY );
    }

    else if ( mDrawMode == DrawMode::SMOOTHCOLORFACES )
    {
      glEnableClientState (GL_VERTEX_ARRAY);
      glVertexPointer( 3, GL_FLOAT, 0, mBaseMesh->getMesh( ).points( ) );

      glEnableClientState (GL_NORMAL_ARRAY);
      glNormalPointer( GL_FLOAT, 0, mBaseMesh->getMesh( ).vertex_normals( ) );

      glBegin (GL_TRIANGLES);
      for ( ; fIt != fEnd; ++fIt )
      {
        fvIt = mBaseMesh->getMesh( ).cfv_iter( *fIt );
        glArrayElement( fvIt->idx( ) );
        ++fvIt;
        glArrayElement( fvIt->idx( ) );
        ++fvIt;
        glArrayElement( fvIt->idx( ) );
      }
      glEnd( );

      glDisableClientState( GL_VERTEX_ARRAY );
      glDisableClientState( GL_NORMAL_ARRAY );
    }
  }
}
