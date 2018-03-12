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

#ifndef BASEMESHRENDERER_H
#define BASEMESHRENDERER_H

#include <GL/glut.h>
#include "BaseMesh.h"

namespace NSUtils
{
  enum DrawMode
  {
    POINTS, POINTSCOLOR, WIRE, FLAT, FLATCOLORFACES, SMOOTH, SMOOTHCOLORFACES
  };

  class BaseMeshRenderer
  {
      BaseMesh * mBaseMesh;

      DrawMode mDrawMode;

    public:

      BaseMeshRenderer ( BaseMesh * pBaseMesh );

      void setBaseMesh ( BaseMesh * pBaseMesh )
      {
        mBaseMesh = pBaseMesh;
      }

      void setDrawMode ( DrawMode pDrawMode )
      {
        mDrawMode = pDrawMode;
      }

      void draw ( );
  };
}
#endif // BASEMESHRENDERER_H
