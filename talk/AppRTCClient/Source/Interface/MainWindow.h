/* Copyright (c) 2014 Temasys Communications pvt, ltd

   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at

       http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and 
   limitations under the License.

 * MainWindow.h
 *
 *  Created on: Nov 14, 2013
 *      Author: Francois Temasys
 */
#ifndef MAINWINDOW_H_
#define MAINWINDOW_H_

//libjingle
#include "talk/app/webrtc/mediastreaminterface.h"
#include "talk/media/base/mediachannel.h"
#include "talk/media/base/videocommon.h"
#include "talk/media/base/videoframe.h"
#include "talk/media/base/videorenderer.h"
//QT
#include <qstring.h>
//Perso
#include "MainWindowObserver.h"

// Pure virtual interface for the main window.
class MainWindow
{
 public:
  virtual ~MainWindow() {}

  enum UI {
    CONNECT_TO_SERVER,
    STREAMING
  };

  virtual void RegisterObserver(MainWindowObserver* callback) = 0;
  virtual void MessageBox(const QString caption, const QString text, bool is_error) = 0;
  virtual UI current_ui() = 0;
  virtual void SwitchToConnectUI() = 0;
  virtual void SwitchToStreamingUI() = 0;

  virtual void StartLocalRenderer(webrtc::VideoTrackInterface* local_video) = 0;
  virtual void StopLocalRenderer() = 0;
  virtual void StartRemoteRenderer(webrtc::VideoTrackInterface* remote_video) = 0;
  virtual void StopRemoteRenderer() = 0;
};

#endif /* MAINWINDOW_H_ */
