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
#include "window.h"
#include "ui_window.h"

#include "libQGLPluginWidget.h"

Window::Window ( QWidget *parent ) :
    QWidget( parent ), ui( new Ui::Window )
{
  ui->setupUi( this );

  QObject::connect( ui->pushButton_loadScene, SIGNAL( clicked( ) ), this,
      SLOT( loadScene( ) ) );

  //ColViz
  QObject::connect( ui->pushButton_loadColVizCfgFile, SIGNAL( clicked( ) ),
      this, SLOT( setupColViz( ) ) );
  QObject::connect( ui->pushButton_sendSelection, SIGNAL( clicked( ) ), this,
      SLOT( sendSelection( ) ) );
  QObject::connect( ui->pushButton_sendCamera, SIGNAL( clicked( ) ), this,
      SLOT( sendCamera( ) ) );
  QObject::connect( ui->pushButton_sendMessage, SIGNAL( clicked( ) ), this,
      SLOT( sendMessage( ) ) );

  QObject::connect( ui->checkBox_letsInteract, SIGNAL( clicked( ) ), this,
      SLOT( setLetsInteract( ) ) );
  QObject::connect( ui->checkBox_sendContCamera, SIGNAL( clicked( ) ), this,
      SLOT( setSendContinuousCamera( ) ) );
  QObject::connect(ui->comboBox_repType,SIGNAL(currentIndexChanged(int)),this,SLOT(setRepType()));

  //Persistence
  QObject::connect( ui->pushButton_loadPersistence, SIGNAL( clicked( ) ), this,
      SLOT( setupPersistence( ) ) );
  QObject::connect( ui->pushButton_logIn, SIGNAL( clicked( ) ), this,
      SLOT( logInOnPersistenceSystem( ) ) );
  QObject::connect( ui->pushButton_restoreSession, SIGNAL( clicked( ) ), this,
      SLOT( loadDialogsFromPersistenceSystem( ) ) );
  QObject::connect( ui->pushButton_LogOut, SIGNAL( clicked( ) ), this,
      SLOT( logOffOnPersistenceSystem( ) ) );

  ui->lineEdit_password->setEchoMode( QLineEdit::Password );

  mColViz 	= nullptr;
  mDBManager 	= nullptr;
  subThread 	= nullptr;
  mUser 	= "[User not logged in]: ";
}

Window::~Window ( )
{
  delete mColViz;

  std::terminate( );
  delete subThread;

  delete ui;
}

void Window::keyPressEvent ( QKeyEvent *e )
{
  if ( e->key( ) == Qt::Key_Escape ) close( );
  else
    QWidget::keyPressEvent( e );
}

void Window::setLetsInteract ( )
{
  ui->myGLWidget->setLetsInteract( ui->checkBox_letsInteract->isChecked( ) );
}

void Window::setSendContinuousCamera ( )
{
  ui->myGLWidget->setSendContinuousCamera(
      ui->checkBox_sendContCamera->isChecked( ) );
}

void Window::loadScene ( )
{
  QString fileName = QFileDialog::getOpenFileName( this, tr( "Open File" ),
      "./", tr( "Scene(*.sml)" ) );

  if ( !fileName.isNull( ) )
  {
    ui->myGLWidget->loadScene( fileName.toStdString( ) );
  }
}

void Window::setupColViz ( )
{
  QString fileName = QFileDialog::getOpenFileName( this, tr( "Open File" ),
      "./", tr( "ColVizInteract(*.cvi)" ) );

  if ( !fileName.isNull( ) )
  {
    if ( mColViz != nullptr )
    {
      delete mColViz;
      mColViz = nullptr;
      std::terminate( );
      delete subThread;
      subThread = nullptr;
    }

    try
    {
      mColViz = new ColViz( fileName.toStdString( ) );
    }
    catch ( ... )
    {
      std::cerr << "Exception detected. ColViz will not be started." << std::endl;
      ui->groupBox_ColVizInteract->setEnabled( false );
      return;
    }
    
    ui->myGLWidget->setColViz( mColViz );

    subThread = new std::thread( &Window::receiveMessage, this );

    QObject::connect( this, SIGNAL( updateChatNow( QString ) ),
        SLOT( updateChat( QString ) ) );
    QObject::connect( this, SIGNAL( updateSelectionNow( QList<int> ) ),
        SLOT( updateSelection( QList<int> ) ) );
    QObject::connect( this, SIGNAL( updateCameraNow( ) ),
        SLOT( updateCamera( ) ) );

    ui->groupBox_ColVizInteract->setEnabled( true );
    ui->pushButton_loadColVizCfgFile->setEnabled( false );
  }
}

void Window::setupPersistence ( )
{
  QString fileName = QFileDialog::getOpenFileName( this, tr( "Open File" ),
      "./", tr( "ColVizDB(*.cvd)" ) );

  if ( !fileName.isNull( ) )
  {
    if ( mDBManager != nullptr )
    {
      delete mDBManager;
      mDBManager=nullptr;
    }

    try
    {
      mDBManager = new DBManager( fileName.toStdString( ) );
    }
    catch ( ... )
    {
      std::cerr << "Exception detected. Persistence System is not active" << std::endl;
      ui->groupBox_logIn->setEnabled( false );
      ui->comboBox_sessioId->clear( );
      ui->pushButton_LogOut->setEnabled( false );
      return;
    }

    ui->groupBox_logIn->setEnabled( true );
  }
}

void Window::logInOnPersistenceSystem ( )
{
  bool success = mDBManager->logIn(
      ui->lineEdit_user->text( ).toUtf8( ).constData( ),
      ui->lineEdit_password->text( ).toUtf8( ).constData( ) );
  if ( success )
  {
    ui->groupBox_session->setEnabled( true );
    std::vector < std::string > content;
    mDBManager->loadSessions( content );

    ui->comboBox_sessioId->clear( );
    for ( unsigned int i = 0; i < content.size( ); ++i )
    {
      ui->comboBox_sessioId->addItem( QString::fromStdString( content[i] ) );
    }
    mUser = "[" + mDBManager->getUserName( ) + "]: ";

    ui->pushButton_LogOut->setEnabled( true );
    ui->groupBox_logIn->setEnabled( false );
  }
  else
  {
    std::cerr<<"Incorrect user or password, please try it again!."<<std::endl;
  }
}

void Window::logOffOnPersistenceSystem ( )
{
  ui->comboBox_sessioId->clear( );
  ui->groupBox_logIn->setEnabled( true );
  ui->groupBox_session->setEnabled( false );
  ui->pushButton_LogOut->setEnabled( false );
  mUser = "[User not logged in]: ";
}

void Window::loadDialogsFromPersistenceSystem ( )
{
  std::vector < std::string > content;
  mDBManager->loadDialogs( ui->comboBox_sessioId->currentIndex( ) + 1,
      content );

  ui->plainTextEdit_chat->setPlainText( "" );
  for ( unsigned int i = 0; i < content.size( ); ++i )
  {
    updateChat( QString::fromStdString( content[i] ) );
  }
}

void Window::updateChat ( QString pMessage )
{
  if ( ui->myGLWidget->getLetsInteract( ) ) ui->plainTextEdit_chat->setPlainText(
      pMessage + "\n" + ui->plainTextEdit_chat->toPlainText( ) );
}

void Window::updateSelection ( QList<int> pSelection )
{
  if ( ui->myGLWidget->getLetsInteract( ) ) ui->myGLWidget->setSelection(
      pSelection );
}

void Window::updateCamera ( )
{
  if ( ui->myGLWidget->getLetsInteract( ) ) ui->myGLWidget->setCamera(
      mCamParams );
}

void Window::sendMessage ( )
{
  if ( ui->plainTextEdit_textToSend->toPlainText( ).length( ) > 0 )
  {
    if ( mDBManager != nullptr )
    {
      mDBManager->storeMessage(
          ui->plainTextEdit_textToSend->toPlainText( ).toStdString( ) );
    }

    ui->plainTextEdit_chat->setPlainText(
        QString::fromStdString( mUser )
            + ui->plainTextEdit_textToSend->toPlainText( ) + "\n"
            + ui->plainTextEdit_chat->toPlainText( ) );
    mColViz->sendText(
        mUser + ui->plainTextEdit_textToSend->toPlainText( ).toStdString( ) );

    ui->plainTextEdit_textToSend->setPlainText( "" );
  }
}

void Window::sendCamera ( )
{
  std::vector<float> lCamParams = ui->myGLWidget->getCamera( );
  mColViz->sendCamera3D( lCamParams );
}

void Window::sendSelection ( )
{
  QList<int> lSelection = ui->myGLWidget->getSelection( );
  std::vector<int> lVectSelection;

  for ( QList<int>::const_iterator it = lSelection.begin( ), end =
      lSelection.end( ); it != end; ++it )
    lVectSelection.push_back( *it );

  mColViz->sendSelection( lVectSelection );
}

void Window::setRepType ( )
{
  ui->myGLWidget->setRepType( ui->comboBox_repType->currentIndex( ) );
}

void Window::receiveMessage ( )
{
  while ( true )
  {
    updateVisualization( mColViz->receiveMassage( ) );
  }
}

void Window::updateVisualization ( std::string pMessage )
{
  JSONDocument = QJsonDocument::fromJson(
      QString::fromStdString( pMessage ).toUtf8( ) );
  JSONObject = JSONDocument.object( );
  JSONObjectValue = JSONObject.value( QString( "type" ) );

  if ( JSONObjectValue.toString( ) == "singleValue" )
  {
    JSONObjectValue = JSONObject.value( QString( "val" ) );
    QString val = QString::number( JSONObjectValue.toInt( ) );
  }
  else if ( JSONObjectValue.toString( ) == "camera2D" )
  {
    JSONObjectValue = JSONObject.value( QString( "val" ) );
  }
  else if ( JSONObjectValue.toString( ) == "camera3D" )
  {
    JSONObjectValue = JSONObject.value( QString( "val" ) );
    JSONArray = JSONObjectValue.toArray( );
    mCamParams.clear( );
    foreach (const QJsonValue & value, JSONArray)
    {
      mCamParams.push_back(value.toDouble() );
    }
    emit updateCameraNow ( );
  }
  else if ( JSONObjectValue.toString( ) == "selection" )
  {
    JSONObjectValue = JSONObject.value( QString( "val" ) );
    JSONArray = JSONObjectValue.toArray( );

    QList<int> lSelection;
    foreach (const QJsonValue & value, JSONArray)
    {
      lSelection.push_back(value.toInt());
    }

    emit updateSelectionNow( lSelection );
  }
  else if ( JSONObjectValue.toString( ) == "text" )
  {
    JSONObjectValue = JSONObject.value( QString( "val" ) );
    QString val = JSONObjectValue.toString( );
    emit updateChatNow( val );
  }
}
