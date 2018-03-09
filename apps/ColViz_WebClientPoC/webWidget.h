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

#ifndef WEBWIDGET_H
#define WEBWIDGET_H

#include <QWidget>
#include <QWebEngineView>
#include <QtWebEngineWidgets>
#include <QtWebChannel/QtWebChannel>

#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

#include <colviz.h>

class webWidget: public QWebEngineView
{
    Q_OBJECT

  public:

    explicit webWidget ( QWidget *parent = 0 );
    ~webWidget ( );

    QList<int> getSelection ( );
    void setSelection ( const QList<int> & pSelection );
    void setColViz ( ColViz* pColViz )
    {
      mColViz = pColViz;
    }

    void setLetsInteract ( bool pLetsInteract )
    {
      mLetsInteract = pLetsInteract;
    }
    bool getLetsInteract ( )
    {
      return mLetsInteract;
    }

  private:

    QString jQuery;
    QWebChannel channel;

    void buildWidget ( const QUrl& url );

    QList<int> selection_;

    ColViz* mColViz;

    bool mLetsInteract;

    QJsonDocument JSONDocument;
    QJsonObject JSONObject;
    QJsonValue JSONObjectValue;
    QJsonArray JSONArray;

    public slots:

    void loadUrl(QString pURL);
    void injectJavaScrip ( QString pJSCode );

};
#endif //WEBWIDGET_H
