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

#ifndef LIBGLWIDGET_H
#define LIBGLWIDGET_H

#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

#include <QGLViewer/qglviewer.h>

#include <BaseMeshRenderer.h>
#include <IndexedBaseMeshContainer.h>

#include <colviz.h>

#include "object.h"

class libQGLPluginWidget: public QGLViewer
{
    Q_OBJECT

    enum RepresentationMode
    {
      REAL, ICONIC
    };

    NSUtils::IndexedBaseMeshContainer* mIndexedBaseMeshContainer;
    NSUtils::BaseMeshRenderer* mBaseMeshRenderer;
    NSUtils::TransformationNode mAuxTrans;
    NSUtils::TransformationNode mAuxTrans2;

  public:
    explicit libQGLPluginWidget ( QWidget *parent = 0 );
    ~libQGLPluginWidget ( );

    void loadScene ( std::string pPathToFile );

    void setRepType ( unsigned int pRetType );

    void setMyBackgroundColor ( QColor pBackground );

    const QList<int> & getSelection ( )
    {
      return selection_;
    };

    void setSelection ( const QList<int> & pSelection );

    std::vector<float> getCamera ( );

    void setCamera ( std::vector<float> pCamParams );

    void setColViz ( ColViz* pColViz )
    {
      mColViz = pColViz;
    }

    void setSendContinuousCamera ( bool pSendContinuousCamera )
    {
      mSendContinuousCamera = pSendContinuousCamera;
    }
    bool getSendContinuousCamera ( )
    {
      return mSendContinuousCamera;
    }

    void setLetsInteract ( bool pLetsInteract )
    {
      mLetsInteract = pLetsInteract;
    }
    bool getLetsInteract ( )
    {
      return mLetsInteract;
    }

  protected:

    virtual void draw ( );
    virtual void init ( );
    virtual QString helpString ( ) const;

    virtual void drawWithNames ( );
    virtual void endSelection ( const QPoint& );

    virtual void animate ( );

    virtual void mousePressEvent ( QMouseEvent *e );
    virtual void mouseMoveEvent ( QMouseEvent *e );
    virtual void mouseReleaseEvent ( QMouseEvent *e );

  private:

    void startManipulation ( );
    void addIdToSelection ( int id );
    void removeIdFromSelection ( int id );
    void drawSelectionRectangle ( ) const;

    void resetScene ( );

    QRect rectangle_;

    enum SelectionMode
    {
      NONE, ADD, REMOVE
    };
    SelectionMode selectionMode_;

    QList<Object*> objects_;
    QList<int> selection_;

    QColor mBackgroundColor;

    ColViz* mColViz;
    bool mSendContinuousCamera;
    bool mLetsInteract;

    QJsonDocument JSONDocument;
    QJsonObject JSONObject;
    QJsonValue JSONObjectValue;
    QJsonArray JSONArray;

    unsigned int mSimSteps;
    std::vector<std::vector<int>> mSimValues;
    std::vector<std::vector<std::vector<int>>>mSimConections;

    unsigned int mActiveSimStep;

    RepresentationMode mRepresentationMode;
  };

#endif //LIBGLWIDGET_H
