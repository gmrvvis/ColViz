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
#include "webWidget.h"

#include<iostream>

webWidget::webWidget ( QWidget *parent ) :
    QWebEngineView( parent )
{
  buildWidget( QUrl( "https://www.humanbrainproject.eu/en/" ) );

  mLetsInteract = true;
  mColViz = NULL;

  show( );
}

webWidget::~webWidget ( )
{
}

void webWidget::buildWidget ( const QUrl& url )
{
  QFile file;
  QDir::setCurrent( qApp->applicationDirPath( ) );
  file.setFileName( "qrc:/jquery.min.js" );
  try
  {
    file.open( QIODevice::ReadOnly );
  }
  catch ( const std::exception& )
  {
    std::exit( -1 );
  }

  jQuery = file.readAll( );
  jQuery.append( "\n var qt={'jQuery':jQuery.noConflict(true)};" );
  file.close( );

  page( )->setWebChannel( &channel );
  channel.registerObject( "widget", this );

  load( url );
}

void webWidget::loadUrl ( QString pURL )
{
  load( QUrl::fromUserInput( pURL ) );
  injectJavaScrip( jQuery );
}

QList<int> webWidget::getSelection ( )
{
  QList<int> tmpSelection;

  page( )->runJavaScript( "extractJSONforSelection()",
      [&](const QVariant &result)
      {
        JSONDocument = QJsonDocument::fromJson(result.toString().toUtf8());
        JSONObject = JSONDocument.object();
        JSONObjectValue = JSONObject.value(QString("Selection"));;
        JSONArray = JSONObjectValue.toArray();

        std::vector<int> lVectSelection;
        foreach (const QJsonValue & value, JSONArray)
        lVectSelection.push_back((value.toInt()) );

        mColViz->sendSelection(lVectSelection);
      } );
  return tmpSelection;
}

void webWidget::setSelection ( const QList<int> & pSelection )
{
  QString lJSONString = "[";

  for ( int i = 0; i < pSelection.size( ); ++i )
  {
    lJSONString += QString::number( pSelection.at( i ) ) + ",";
  }

  lJSONString = lJSONString.left( lJSONString.length( ) - 1 );
  lJSONString += "]";

  page( )->runJavaScript( "injectJSONforSelection(" + lJSONString + ")" );
}

void webWidget::injectJavaScrip ( QString pJSCode )
{
  page( )->runJavaScript( pJSCode );
}
