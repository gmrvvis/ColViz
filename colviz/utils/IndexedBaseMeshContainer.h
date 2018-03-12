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

#ifndef INDEXEDBASEMESHCONTAINER_H
#define INDEXEDBASEMESHCONTAINER_H

#include <vector>

#include "BaseMeshContainer.h"
#include "Distribution.h"

namespace NSUtils
{

  struct TransformationNode
  {
      int mIdRender;
      NSUtils::NodeDist mNodeDist;
  };

  class IndexedBaseMeshContainer
  {
      NSUtils::BaseMeshContainer mBaseMeshContainer;
      NSUtils::Distribution mDistribution;
      std::vector<std::string> mMeshNames;
      std::vector<TransformationNode> mTransformations;

    public:
      IndexedBaseMeshContainer ( );

      const std::vector<NSUtils::TransformationNode> & getTransformations ( ) const
      {
        return mTransformations;
      }
      ;

      void loadDistribution ( std::string mDistributionFile );

      int getIdByNeuronName ( std::string pNeuronName );

      NSUtils::BaseMesh * getMeshById ( unsigned int pMeshId )
      {
        return mBaseMeshContainer.getElementAt( pMeshId );
      }
      ;

      unsigned int getNumElements ( )
      {
        return mTransformations.size( );
      }
      ;

      void setTraslationToNode ( int pNodeId, NSUtils::Vec3 pTranslation );

      void setRotationToNode ( int pNodeId, float pRotation );

      void setScalationToNode ( int pNodeId, float pScaleFactor );
  };

}
#endif // INDEXEDBASEMESHCONTAINER_H
