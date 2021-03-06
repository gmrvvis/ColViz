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

#include "BaseMeshContainer.h"

namespace NSUtils
{
  BaseMeshContainer::BaseMeshContainer ( void )
  {
    mBaseMeshContainer.clear( );
  }

  BaseMeshContainer::~BaseMeshContainer ( void )
  {
    destroyAllElementsInContainer( );
  }

  unsigned int BaseMeshContainer::getNumElements ( )
  {
    return mBaseMeshContainer.size( );
  }

  std::vector<BaseMesh *> & BaseMeshContainer::getContainer ( )
  {
    return mBaseMeshContainer;
  }

  void BaseMeshContainer::addElement ( BaseMesh * pElement )
  {
    mBaseMeshContainer.push_back( pElement );
  }

  void BaseMeshContainer::addElement ( std::string pElementPath )
  {
    BaseMesh * ltempMesh = new BaseMesh( pElementPath );

    mBaseMeshContainer.push_back( ltempMesh );
  }

  BaseMesh * BaseMeshContainer::getElementAt ( unsigned int i )
  {
    return mBaseMeshContainer.at( i );
  }

  void BaseMeshContainer::scaleContainer ( float /* pScale */ )
  {
  }

  void BaseMeshContainer::destroyAllElementsInContainer ( )
  {
    for ( unsigned int i = 0; i < mBaseMeshContainer.size( ); ++i )
    {
      delete mBaseMeshContainer.at( i );
    }

    mBaseMeshContainer.clear( );
  }
}

