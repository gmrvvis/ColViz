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

#include <QString>
#include <QFileInfo>

#include "IndexedBaseMeshContainer.h"

namespace NSUtils
{
  IndexedBaseMeshContainer::IndexedBaseMeshContainer ( )
  {

  }

  void IndexedBaseMeshContainer::loadDistribution (
      std::string mDistributionFile )
  {
    mDistribution.loadFromFile( mDistributionFile );

    TransformationNode lTransformationNode;
    lTransformationNode.mIdRender = 0;

    QFileInfo lFileInfo( QString::fromStdString( mDistributionFile ) );
    QString lpath = lFileInfo.absolutePath( );

    for ( unsigned int i = 0; i < mDistribution.getNodeContainer( ).size( );
        ++i )
    {
      int lIdNode = getIdByNeuronName(
          mDistribution.getNodeContainer( ).at( i ).mName );

      if ( lIdNode != -1 ) lTransformationNode.mIdRender = lIdNode;
      else
      {
        std::string lMeshName =
            QString::fromStdString(
                mDistribution.getNodeContainer( ).at( i ).mName ).simplified( ).toStdString( );
        mMeshNames.push_back( lMeshName );

        lIdNode = mBaseMeshContainer.getContainer( ).size( );

        QString lAuxPath( "" );

        lAuxPath = lpath + "/" + QString::fromStdString( lMeshName ) + ".obj";
        NSUtils::BaseMesh * lBaseMesh = new NSUtils::BaseMesh(
            lAuxPath.toStdString( ) );
        mBaseMeshContainer.addElement( lBaseMesh );
      }

      lTransformationNode.mNodeDist = mDistribution.getNodeContainer( ).at( i );
      mTransformations.push_back( lTransformationNode );
    }

  }

  int IndexedBaseMeshContainer::getIdByNeuronName ( std::string pNeuronName )
  {
    int result = -1;

    for ( unsigned int i = 0; i < mMeshNames.size( ); ++i )
    {
      if ( pNeuronName == mMeshNames.at( i ) )
      {
        result = i;
        return result;
      }
    }
    return result;
  }

  void IndexedBaseMeshContainer::setTraslationToNode ( int pNodeId,
      NSUtils::Vec3 pTranslation )
  {
    mTransformations.at( pNodeId ).mNodeDist.mPos = pTranslation;
  }

  void IndexedBaseMeshContainer::setRotationToNode ( int pNodeId,
      float pRotation )
  {
    mTransformations.at( pNodeId ).mNodeDist.mAngle = pRotation;
  }

  void IndexedBaseMeshContainer::setScalationToNode ( int pNodeId,
      float pScaleFactor )
  {
    mTransformations.at( pNodeId ).mNodeDist.mScale = pScaleFactor;
  }
}
