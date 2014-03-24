/* (c) Copyright 2014 Temasys Communication, Pte Ltd.

  Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at

       http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and 
   limitations under the License.

 * QtMainWindow.h
 *
 *  Created on: Nov 6, 2013
 *      Author: Francois Temasys
 */

#ifndef QTMAINWINDOW_H_
#define QTMAINWINDOW_H_

// Perso

#include "../Interface/MainWindowObserver.h"
#include "../Interface/MainWindow.h"
#include "ConnectWindow.h"
#include "../VideoRenderer.h"

// libjingle

#include "talk/base/common.h"
#include "talk/base/logging.h"
#include "talk/base/stringutils.h"
#include "talk/base/scoped_ptr.h"
#include "talk/app/webrtc/mediastreaminterface.h"
#include "talk/media/base/mediachannel.h"
#include "talk/media/base/videocommon.h"
#include "talk/media/base/videoframe.h"
#include "talk/media/base/videorenderer.h"
#include <assert.h>

// QT
#include <qtconcurrentrun.h>
#include <qobject.h>
#include <qmainwindow.h>
#include <qevent.h>
#include <qlayout.h>
#include <qstackedwidget.h>
#include <qdebug.h>

class VideoRenderer;

class QtkMainWindow:
    public QMainWindow,
    public MainWindow
{
    Q_OBJECT

    public:
        QtkMainWindow(QWidget * parent,QUrl      url);
        ~QtkMainWindow();

        ///////////////////////////////////////////////////////////////////////////////////////////
        // Implement MainWindow
        //
        void RegisterObserver(MainWindowObserver * callback);
        void MessageBox(const QString caption,const QString text,bool is_error);
        virtual UI current_ui();
        virtual void SwitchToConnectUI();
        virtual void SwitchToStreamingUI();
        virtual void StartLocalRenderer(webrtc::VideoTrackInterface * local_video);
        virtual void StopLocalRenderer();
        virtual void StartRemoteRenderer(webrtc::VideoTrackInterface * remote_video);
        virtual void StopRemoteRenderer();
		//																						 //
        ///////////////////////////////////////////////////////////////////////////////////////////

        // Creates and shows the main window with the |Connect UI| enabled.
        void Create();


private slots:
        void toConnect(QUrl url,QByteArray htmldata);
        void updateRenderImage(const int target,QImage *newImage);

    protected:
        QStackedWidget *                     _stackedLayout;
        QLabel *                            _remoteLabel;          // The drawing surface for rendering video streams.
        QLabel * 							_localLabel;
        ConnectWindow *                      _vbox;               // Container for the Connect UI
        MainWindowObserver *                 _callback;
        QObject *                            _parent;
        talk_base::scoped_ptr<VideoRenderer> local_renderer_;
        talk_base::scoped_ptr<VideoRenderer> remote_renderer_;

        talk_base::scoped_ptr<uint8>         draw_buffer_;
        int                                  draw_buffer_size_;

        //QT OVERWRITE
        void closeEvent(QCloseEvent * event );
        void keyPressEvent( QKeyEvent * event );
};
#endif /* QTMAINWINDOW_H_ */

