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

#ifndef WINDOW_H
#define WINDOW_H

#include <QWidget>
#include <QSlider>

#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

#include <cassert>
#include <string>
#include <chrono>

#include <thread>
#include <iostream>

#include <colviz.h>
#include <DBManager.h>

namespace Ui
{
  class Window;
}

class Window: public QWidget
{
    Q_OBJECT

  public:
    explicit Window ( QWidget *parent = 0 );
    ~Window ( );

  protected:
    void keyPressEvent ( QKeyEvent *event );

  private:
    Ui::Window* ui;
    std::thread* subThread;

    ColViz* mColViz;
    DBManager * mDBManager;
    std::string mUser;
    std::vector<float> mCamParams;

    QJsonDocument JSONDocument;
    QJsonObject JSONObject;
    QJsonValue JSONObjectValue;
    QJsonArray JSONArray;

    void receiveMessage ( );
    void updateVisualization ( std::string pMessage );

    signals:
    void updateChatNow(QString pMessage);
    void updateSelectionNow ( QList<int> pSelection );
    void updateCameraNow ( );

    public slots:
    void setupColViz();
    void setupPersistence ( );

    void logInOnPersistenceSystem ( );
    void logOffOnPersistenceSystem ( );

    void loadDialogsFromPersistenceSystem ( );

    void updateChat ( QString pMessage );
    void updateSelection ( QList<int> pSelection );
    void updateCamera ( );

    void setSendContinuousCamera ( );
    void setLetsInteract ( );

    private slots:
    void loadScene();

    void sendMessage ( );
    void sendCamera ( );
    void sendSelection ( );

    void setRepType ( );
};

#endif // WINDOW_H
