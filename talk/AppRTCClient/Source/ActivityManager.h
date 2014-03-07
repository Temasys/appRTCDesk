/* (c) Copyright 2014 Temasys Communication, pvt, ltd.

   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at

       http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and 
   limitations under the License.

 *
 * ActivityManager.h
 *
 *  Created on: Nov 14, 2013
 *      Author: Francois Temasys
 */
#ifndef ACTIVITYMANAGER_H
#define ACTIVITYMANAGER_H

#include "talk/base/refcount.h"

#include "GUI/QtMainWindow.h"
#include "PeerConnection.h"
#include "Conductor.h"

#include <QObject>
#include <QUrl>

#include <QDebug>

class ActivityManager: public QObject
{
  Q_OBJECT

public:
  ActivityManager(
    QObject *parent = 0,
    QString urlstart = QString("https://apprtc.appspot.com/?r=11111111")
  );
  ~ActivityManager();
  void Start();

private:
  QtkMainWindow  *_mainWindow;
  PeerConnection *_peerConnection;

  talk_base::scoped_refptr<Conductor> _mainConductor;
};

#endif // ACTIVITYMANAGER_H
