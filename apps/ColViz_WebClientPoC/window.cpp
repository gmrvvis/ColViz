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
#include <QtWebEngineWidgets>
#include <QWebEngineView>

#include "window.h"
#include "ui_window.h"

Window::Window ( QWidget *parent ) :
    QWidget( parent ), ui( new Ui::Window )
{
  ui->setupUi( this );

  QObject::connect( ui->lineEdit_url, SIGNAL( returnPressed( ) ),
      SLOT( loadURL( ) ) );
  QObject::connect(ui->comboBox_places, SIGNAL(currentIndexChanged(int)), SLOT(loadPredefinedURL()));

  //ColViz
  QObject::connect( ui->pushButton_loadColVizCfgFile, SIGNAL( clicked( ) ),
      this, SLOT( setupColViz( ) ) );
  QObject::connect( ui->pushButton_sendSelection, SIGNAL( clicked( ) ), this,
      SLOT( sendSelection( ) ) );
  QObject::connect( ui->pushButton_sendMessage, SIGNAL( clicked( ) ), this,
      SLOT( sendMessage( ) ) );

  QObject::connect( ui->checkBox_letsInteract, SIGNAL( clicked( ) ), this,
      SLOT( setLetsInteract( ) ) );

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

  mColViz = nullptr;
  mDBManager = nullptr;
  subThread = nullptr;
  mUser = "[User not logged in]: ";

  update( );
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
  ui->viewer->setLetsInteract( ui->checkBox_letsInteract->isChecked( ) );
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
      return;
    }

    ui->viewer->setColViz( mColViz );

    subThread = new std::thread( &Window::receiveMessage, this );

    QObject::connect( this, SIGNAL( updateChatNow( QString ) ),
	    SLOT( updateChat( QString ) ) );
    QObject::connect( this, SIGNAL( updateSelectionNow( QList<int> ) ),
	    SLOT( updateSelection( QList<int> ) ) );

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
    for ( size_t i = 0; i < content.size( ); ++i )
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
  for ( size_t i = 0; i < content.size( ); ++i )
  {
    updateChat( QString::fromStdString( content[i] ) );
  }
}

void Window::updateChat ( QString pMessage )
{
  if ( ui->viewer->getLetsInteract( ) ) ui->plainTextEdit_chat->setPlainText(
      pMessage + "\n" + ui->plainTextEdit_chat->toPlainText( ) );
}

void Window::updateSelection ( QList<int> pSelection )
{
  if ( ui->viewer->getLetsInteract( ) ) ui->viewer->setSelection( pSelection );
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

void Window::sendSelection ( )
{
  ui->viewer->getSelection( );
}

void Window::loadURL ( )
{
  mURL = QString( ui->lineEdit_url->text( ) );
  ui->viewer->loadUrl( mURL );
}

void Window::loadPredefinedURL ( )
{
  QString lPath = QCoreApplication::applicationDirPath( );

  switch ( ui->comboBox_places->currentIndex( ) )
  {
    case 0:
      ui->lineEdit_url->setText( "https://www.humanbrainproject.eu/en/" );
      break;
    case 1:
      ui->lineEdit_url->setText( lPath + "/../content/web/SimpleWebViz/index.html" );
      break;
    case 2:
      ui->lineEdit_url->setText( lPath + "/../content/web/WebGL/index.html" );
      break;
    default:
      ui->lineEdit_url->setText( "https://www.humanbrainproject.eu/en/" );
      break;
  }

  loadURL( );
}

void Window::reLoadURL ( )
{
  ui->viewer->loadUrl( mURL );
}

void Window::forwardURL ( )
{
  ui->viewer->pageAction( QWebEnginePage::Forward );
}

void Window::backwardURL ( )
{
  ui->viewer->pageAction( QWebEnginePage::Back );
}

void Window::receiveMessage ( )
{
  while ( true )
    updateVisualization( mColViz->receiveMassage( ) );
}

void Window::updateVisualization ( std::string pMessage )
{
  JSONDocument = QJsonDocument::fromJson(
      QString::fromStdString( pMessage ).toUtf8( ) );
  JSONObject = JSONDocument.object( );
  JSONObjectValue = JSONObject.value( QString( "type" ) );
  ;

  if ( JSONObjectValue.toString( ) == "selection" )
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

void Window::changeColor ( unsigned int pColor )
{
  QString code = "myFunctionColor(" + QString::number( pColor ) + ","
      + QString::number( pColor ) + "," + QString::number( pColor ) + ")";
  ui->viewer->injectJavaScrip( code );
}

void Window::test ( )
{
  QString code = "";
  ui->viewer->injectJavaScrip( code );
}
