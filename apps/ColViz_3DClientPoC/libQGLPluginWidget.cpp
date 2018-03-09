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

#include <QtWidgets>
#include <QColor>
#include <QMouseEvent>

#include <GL/glut.h>

#include "libQGLPluginWidget.h"
#include "manipulatedFrameSetConstraint.h"

using namespace std;
using namespace qglviewer;

libQGLPluginWidget::libQGLPluginWidget ( QWidget *parent ) :
    QGLViewer( parent )
{
  mBaseMeshRenderer = NULL;
  mIndexedBaseMeshContainer = NULL;

  mBackgroundColor = QColor( 0, 180, 255 );

  mSendContinuousCamera = false;
  mLetsInteract = true;

  mColViz = NULL;

  mActiveSimStep = mSimSteps = 0;

  mRepresentationMode = RepresentationMode::REAL;
}

libQGLPluginWidget::~libQGLPluginWidget ( )
{
  if ( mBaseMeshRenderer != NULL ) delete mBaseMeshRenderer;
  if ( mIndexedBaseMeshContainer != NULL ) delete mIndexedBaseMeshContainer;
}

void libQGLPluginWidget::animate ( )
{
  ++mActiveSimStep;
  if ( mActiveSimStep > ( mSimSteps - 2 ) ) mActiveSimStep = 0;
}

void libQGLPluginWidget::draw ( )
{
  const qglviewer::Vec cameraPos = camera( )->position( );
  const GLfloat pos[4] = { ( float ) cameraPos[0], ( float ) cameraPos[1],
      ( float ) cameraPos[2], 1.0 };
  glLightfv( GL_LIGHT1, GL_POSITION, pos );
  glLightfv( GL_LIGHT1, GL_SPOT_DIRECTION, camera( )->viewDirection( ) );

  if ( ( mSendContinuousCamera ) && ( mColViz != NULL ) ) mColViz->sendCamera3D(
      getCamera( ) );

  if ( mIndexedBaseMeshContainer != NULL )
  {
    if ( mIndexedBaseMeshContainer->getNumElements( ) > 0 )
    {
      for ( unsigned int i = 0;
          i < mIndexedBaseMeshContainer->getNumElements( ); ++i )
      {
        mBaseMeshRenderer->setBaseMesh(
            mIndexedBaseMeshContainer->getMeshById(
                mIndexedBaseMeshContainer->getIdByNeuronName(
                    mIndexedBaseMeshContainer->getTransformations( ).at( i ).mNodeDist.mName ) ) );

        mAuxTrans = mIndexedBaseMeshContainer->getTransformations( ).at( i );

        glPushMatrix( );
        glTranslatef( mAuxTrans.mNodeDist.mPos.x, mAuxTrans.mNodeDist.mPos.y,
            mAuxTrans.mNodeDist.mPos.z );

        if ( mRepresentationMode == RepresentationMode::ICONIC )
        {
          static int slices = 16;
          static int stacks = 16;

          if ( QString::fromStdString(
              mIndexedBaseMeshContainer->getTransformations( ).at( i ).mNodeDist.mName ).left(
              1 ) == "I" )
          {
            glColor3f( 1.0, 0, 0 );
            glutSolidSphere( 12, slices, stacks );
          }
          else
          {
            glColor3f( 0, 1.0, 0 );
            glRotatef( -90, 1, 0, 0 );
            glutSolidCone( 12, 12, slices, stacks );
          }

        }
        else
        {
          glRotatef( mAuxTrans.mNodeDist.mAngle, 0, 1, 0 );
          int lCol = mSimValues.at( i ).at( mActiveSimStep );
          glColor3i( lCol, lCol, lCol );
          mBaseMeshRenderer->draw( );
        }

        glPopMatrix( );

        if ( mRepresentationMode == RepresentationMode::ICONIC )
        {
          for ( int j = 0;
              j < mSimConections.at( i ).at( mActiveSimStep ).size( ); ++j )
          {
            mAuxTrans2 = mIndexedBaseMeshContainer->getTransformations( ).at(
                mSimConections.at( i ).at( mActiveSimStep ).at( j ) );

            glLineWidth( 5.5 );
            glColor3f( 1.0, 0.0, 0.0 );
            glBegin (GL_LINES);
            glVertex3f( mAuxTrans.mNodeDist.mPos.x, mAuxTrans.mNodeDist.mPos.y,
                mAuxTrans.mNodeDist.mPos.z );
            glVertex3f( mAuxTrans2.mNodeDist.mPos.x,
                mAuxTrans2.mNodeDist.mPos.y, mAuxTrans2.mNodeDist.mPos.z );
            glEnd( );
          }
        }
      }
    }
  }

  //Selection
  for ( QList<int>::const_iterator it = selection_.begin( ), end =
      selection_.end( ); it != end; ++it )
    objects_.at( *it )->draw( );

  // Draws rectangular selection area. Could be done in postDraw() instead.
  if ( selectionMode_ != NONE ) drawSelectionRectangle( );
}

void libQGLPluginWidget::init ( )
{
  camera( )->setZClippingCoefficient( 500.0 );
  setBackgroundColor( mBackgroundColor );
  selectionMode_ = SelectionMode::NONE;

  int argc = 0;
  glutInit( &argc, NULL );

  glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
  glEnable (GL_BLEND);

  glEnable (GL_ALPHA_TEST);
  glAlphaFunc( GL_GREATER, 0.1f );
}

void libQGLPluginWidget::loadScene ( std::string pPathToFile )
{
  QString lFileVal;
  QFile file;
  file.setFileName( QString::fromStdString( pPathToFile ) );
  file.open( QIODevice::ReadOnly | QIODevice::Text );
  lFileVal = file.readAll( );
  file.close( );

  JSONDocument = QJsonDocument::fromJson( lFileVal.toUtf8( ) );
  JSONObject = JSONDocument.object( );
  JSONObjectValue = JSONObject.value( QString( "type" ) );

  //scene
  if ( JSONObjectValue.toString( ) == "simulation" )
  {
    JSONObjectValue = JSONObject.value( QString( "simSteps" ) );
    mSimSteps = JSONObjectValue.toInt( );

    JSONObjectValue = JSONObject.value( QString( "val" ) );
    JSONArray = JSONObjectValue.toArray( );

    std::vector<int> lAuxVec;
    foreach (const QJsonValue & v, JSONArray)
    {
      lAuxVec.clear();
      foreach (const QJsonValue & v2, v.toArray())
      {
        lAuxVec.push_back(v2.toInt());
      }
      mSimValues.push_back(lAuxVec);
    }

    JSONObjectValue = JSONObject.value( QString( "conn" ) );
    JSONArray = JSONObjectValue.toArray( );

    std::vector<std::vector<int>> lNeuronConnectVec;
    foreach (const QJsonValue & v, JSONArray)
    {
      lNeuronConnectVec.clear();
      foreach (const QJsonValue & v2, v.toArray())
      {
        lAuxVec.clear();
        foreach (const QJsonValue & v3, v2.toArray())
        {
          lAuxVec.push_back(v3.toInt());
        }
        lNeuronConnectVec.push_back(lAuxVec);
      }
      mSimConections.push_back(lNeuronConnectVec);
    }

    JSONObjectValue = JSONObject.value( QString( "scene" ) );

    if ( mIndexedBaseMeshContainer != NULL ) delete mIndexedBaseMeshContainer;
    mIndexedBaseMeshContainer = new NSUtils::IndexedBaseMeshContainer( );

    QFileInfo lFileInfo( QString::fromStdString( pPathToFile ) );
    QString lpath = lFileInfo.absolutePath( );
    QString lAuxPath( "" );
    lAuxPath = lpath + "/" + JSONObjectValue.toString( );

    mIndexedBaseMeshContainer->loadDistribution( lAuxPath.toStdString( ) );

    mBaseMeshRenderer = new NSUtils::BaseMeshRenderer( NULL );

    NSUtils::Vec3 lTransformation;

    for ( int i = 0; i < mIndexedBaseMeshContainer->getNumElements( ); ++i )
    {
      lTransformation =
          mIndexedBaseMeshContainer->getTransformations( ).at( i ).mNodeDist.mPos;

      Object* o = new Object( );
      o->frame.setPosition( lTransformation.x, lTransformation.y,
          lTransformation.z );
      objects_.append( o );
    }

    updateGL( );
    startAnimation( );
  }
}

void libQGLPluginWidget::setRepType ( unsigned int pRetType )
{
  switch ( pRetType )
  {
    case 0:
      mRepresentationMode = RepresentationMode::REAL;
      break;
    case 1:
      mRepresentationMode = RepresentationMode::ICONIC;
      break;
    default:
      break;
  }
}

void libQGLPluginWidget::setMyBackgroundColor ( QColor pBackground )
{
  mBackgroundColor = pBackground;
  setBackgroundColor( mBackgroundColor );
}

void libQGLPluginWidget::setSelection ( const QList<int> & pSelection )
{
  selection_ = pSelection;
  updateGL( );
}

std::vector<float> libQGLPluginWidget::getCamera ( )
{
  std::vector<float> lCameraParams;

  lCameraParams.push_back( camera( )->position( ).x );
  lCameraParams.push_back( camera( )->position( ).y );
  lCameraParams.push_back( camera( )->position( ).z );

  lCameraParams.push_back( camera( )->viewDirection( ).x );
  lCameraParams.push_back( camera( )->viewDirection( ).y );
  lCameraParams.push_back( camera( )->viewDirection( ).z );

  lCameraParams.push_back( camera( )->upVector( ).x );
  lCameraParams.push_back( camera( )->upVector( ).y );
  lCameraParams.push_back( camera( )->upVector( ).z );

  return lCameraParams;
}

void libQGLPluginWidget::setCamera ( std::vector<float> pCamParams )
{
  camera( )->setPosition(
      qglviewer::Vec( pCamParams[0], pCamParams[1], pCamParams[2] ) );
  camera( )->setViewDirection(
      qglviewer::Vec( pCamParams[3], pCamParams[4], pCamParams[5] ) );
  camera( )->setUpVector(
      qglviewer::Vec( pCamParams[6], pCamParams[7], pCamParams[8] ) );

  updateGL( );
}

//Manipulated methods
void libQGLPluginWidget::drawWithNames ( )
{
  for ( int i = 0; i < int( objects_.size( ) ); i++ )
  {
    glPushName( i );
    objects_.at( i )->draw( );
    glPopName( );
  }
}

void libQGLPluginWidget::endSelection ( const QPoint& )
{
  glFlush( );

  GLint nbHits = glRenderMode( GL_RENDER );

  if ( nbHits > 0 )
  {
    for ( int i = 0; i < nbHits; ++i )
      switch ( selectionMode_ )
      {
        case ADD:
          addIdToSelection ((selectBuffer ( ))[4*i+3]);break;
        case REMOVE: removeIdFromSelection((selectBuffer())[4*i+3]); break;
        default: break;
      }
  }

  selectionMode_ = NONE;
}

void libQGLPluginWidget::mousePressEvent ( QMouseEvent* e )
{
  rectangle_ = QRect( e->pos( ), e->pos( ) );

  if ( ( e->button( ) == Qt::LeftButton )
      && ( e->modifiers( ) == Qt::ShiftModifier ) ) selectionMode_ = ADD;
  else if ( ( e->button( ) == Qt::LeftButton )
      && ( e->modifiers( ) == Qt::ControlModifier ) ) selectionMode_ = REMOVE;
  else
  {
    if ( e->modifiers( ) == Qt::AltModifier ) startManipulation( );
    QGLViewer::mousePressEvent( e );
  }
}

void libQGLPluginWidget::mouseMoveEvent ( QMouseEvent* e )
{
  if ( selectionMode_ != NONE )
  {
    rectangle_.setBottomRight( e->pos( ) );
    updateGL( );
  }
  else
    QGLViewer::mouseMoveEvent( e );
}

void libQGLPluginWidget::mouseReleaseEvent ( QMouseEvent* e )
{
  if ( selectionMode_ != NONE )
  {
    rectangle_ = rectangle_.normalized( );

    setSelectRegionWidth( rectangle_.width( ) );
    setSelectRegionHeight( rectangle_.height( ) );
    select( rectangle_.center( ) );

    updateGL( );
  }
  else
  {
    if ( e->modifiers( ) == Qt::ControlModifier ) startManipulation( );

    QGLViewer::mouseReleaseEvent( e );
  }
}

void libQGLPluginWidget::startManipulation ( )
{
  //Not implemented yet!
}

//   S e l e c t i o n   t o o l s
void libQGLPluginWidget::addIdToSelection ( int id )
{
  if ( !selection_.contains( id ) ) selection_.push_back( id );
}

void libQGLPluginWidget::removeIdFromSelection ( int id )
{
  selection_.removeAll( id );
}

void libQGLPluginWidget::drawSelectionRectangle ( ) const
{
  startScreenCoordinatesSystem( );
  glDisable (GL_LIGHTING);

  glColor4f( 0.0, 0.0, 0.3f, 0.3f );
  glBegin (GL_QUADS);
  glVertex2i( rectangle_.left( ), rectangle_.top( ) );
  glVertex2i( rectangle_.right( ), rectangle_.top( ) );
  glVertex2i( rectangle_.right( ), rectangle_.bottom( ) );
  glVertex2i( rectangle_.left( ), rectangle_.bottom( ) );
  glEnd( );

  glLineWidth( 2.0 );
  glColor4f( 0.4f, 0.4f, 0.5f, 0.5f );
  glBegin (GL_LINE_LOOP);
  glVertex2i( rectangle_.left( ), rectangle_.top( ) );
  glVertex2i( rectangle_.right( ), rectangle_.top( ) );
  glVertex2i( rectangle_.right( ), rectangle_.bottom( ) );
  glVertex2i( rectangle_.left( ), rectangle_.bottom( ) );
  glEnd( );

  glEnable( GL_LIGHTING );
  stopScreenCoordinatesSystem( );
}

void libQGLPluginWidget::resetScene ( )
{
  selectionMode_ = NONE;
  objects_.clear( );
  selection_.clear( );
}

QString libQGLPluginWidget::helpString ( ) const
{
  QString text(
      "<h2>Simple Proof of concept for 3D Visualization using ColViz</h2>" );
  text +=
      "The right controls give the user the main controls of the visualization.";
  text +=
      "Basic Tab: Load the 3D scenes and select the view (real or 3D abstract).";
  text +=
      "ColViz Tab: Load the configuration of the ColViz middleware and define some shapes of interaction between users.";
  text +=
      "Persistence Tab: Load the persistence system for the ColViz interaction between users.";
  text += "Use the mouse to move the camera around the objects. ";
  text +=
      "You can respectively revolve around, zoom and translate with the three mouse buttons. ";
  text +=
      "Left and middle buttons pressed together rotate around the camera view direction axis<br><br>";
  text +=
      "Pressing <b>Alt</b> and one of the function keys (<b>F1</b>..<b>F12</b>) defines a camera keyFrame. ";
  text +=
      "Press <b>F</b> to display the frame rate, <b>A</b> for the world axis, ";
  text += "Press <b>Escape</b> to exit the viewer.";
  return text;
}
