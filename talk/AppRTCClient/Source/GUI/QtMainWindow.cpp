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

 *
 * QtMainWindow.cpp
 *
 *  Created on: Nov 14, 2013
 *      Author: Francois Temasys
 */
#include "QtMainWindow.h"

//
// QtkMainWindow implementation.
//
QtkMainWindow::QtkMainWindow(QWidget * parent,
                             QUrl      url):
	 QMainWindow(parent)
{

    this->resize(800, 600);
    this->setWindowTitle(tr("AppRTC Desk"));

    _stackedLayout = new QStackedWidget();
    _vbox          = new ConnectWindow(0, url);


    _localLabel = new QLabel();
    _localLabel->setAccessibleName(QString("local"));
    QLabel *nameLocalLabel = new QLabel("Local Webcam");
    nameLocalLabel->setAlignment(Qt::AlignCenter);
    _remoteLabel = new QLabel();
    _remoteLabel->setAccessibleName(QString("remote"));
    QLabel *nameRemoteLabel = new QLabel("Remote Webcam");
    nameRemoteLabel->setAlignment(Qt::AlignCenter);


    QGridLayout *streamingLayout = new QGridLayout;
    streamingLayout->addWidget(_localLabel, 0, 0);
    streamingLayout->addWidget(_remoteLabel, 0,1);
    streamingLayout->addWidget(nameLocalLabel, 1, 0);
    streamingLayout->addWidget(nameRemoteLabel, 1, 1);
    QWidget *streamingWidget = new QWidget;
    streamingWidget->setLayout(streamingLayout);

    _stackedLayout -> addWidget(_vbox);
    _stackedLayout -> addWidget(streamingWidget);
    this->setCentralWidget(_stackedLayout);

    QObject::connect(_vbox,SIGNAL(toConnect(QUrl, QByteArray)),this,SLOT(toConnect(QUrl, QByteArray)));
}

QtkMainWindow::~QtkMainWindow()
{
    _vbox->close();
    delete _vbox;
    delete _localLabel;
    delete _remoteLabel;
    delete _stackedLayout;
    _callback = NULL;
}

///////////////////////////////////////////////////////////////////////////////////////////
// Implement MainWindow
//
void QtkMainWindow::RegisterObserver(MainWindowObserver * callback)
{
    _callback = callback;
}

void QtkMainWindow::MessageBox(const QString caption,
                               const QString text,
                               bool          is_error)
{
    if (is_error)
    {
        QMessageBox::critical(0, QString(caption), QString(text));
    }
    else
    {
        QMessageBox::information(0, QString(caption), QString(text));
    }
}

MainWindow::UI QtkMainWindow::current_ui()
{
	if(_stackedLayout->currentWidget() == _vbox)
    {
        return CONNECT_TO_SERVER;
    }

    return STREAMING;
}

void QtkMainWindow::SwitchToConnectUI()
{
    qDebug() << __FUNCTION__;

    assert(_vbox != NULL);
    if(_stackedLayout->currentWidget() != _vbox)
       	_stackedLayout -> setCurrentWidget(_vbox);
}

void QtkMainWindow::SwitchToStreamingUI()
{
    qDebug() << __FUNCTION__;
    if(_stackedLayout->currentIndex() != 1)
    	_stackedLayout -> setCurrentIndex(1);
}

void QtkMainWindow::StartLocalRenderer(webrtc::VideoTrackInterface * local_video)
{
	LOG(INFO) << "\n\n!!!!!!!!!!!!! Local Stream: " << local_video->id() << "!!!!!!!!!!!!!!!!!\n\n";

    local_renderer_.reset(new VideoRenderer(0,local_video));
    QObject::connect(local_renderer_.get(),SIGNAL(imageToUpdate(const int,QImage *)),
    		this,SLOT(updateRenderImage(const int,QImage *)));
}

void QtkMainWindow::StopLocalRenderer()
{
	_localLabel->clear();
	_localLabel->setFixedSize(1,1);
    local_renderer_.reset();

}

void QtkMainWindow::StartRemoteRenderer(webrtc::VideoTrackInterface * remote_video)
{
    remote_renderer_.reset(new VideoRenderer(1,remote_video));
    QObject::connect(remote_renderer_.get(),SIGNAL(imageToUpdate(const int,QImage *)),
        		this,SLOT(updateRenderImage(const int,QImage *)));
}

void QtkMainWindow::StopRemoteRenderer()
{
    remote_renderer_.reset();
    _remoteLabel->clear();
	_remoteLabel->setFixedSize(1,1);
}
//																						 //
///////////////////////////////////////////////////////////////////////////////////////////


void QtkMainWindow::Create()
{
	SwitchToConnectUI();
}


void QtkMainWindow::toConnect(QUrl url,QByteArray htmldata)
{
	LOG(INFO) << "to Connect";
    _callback -> StartLogin(url, htmldata);
}

void QtkMainWindow::updateRenderImage(const int target,QImage *newImage)
{
	QLabel *drawed = NULL;
	if(target == 0)
		drawed = _localLabel;
	else if(target == 1)
		drawed = _remoteLabel;
	if(drawed != NULL && !newImage->isNull() && newImage->width() > 0 && newImage->height() > 0)
	{
		drawed->setFixedSize(newImage->width(),newImage->height());
		drawed->setPixmap(QPixmap::fromImage(*newImage));
		drawed->show();
	}
	if(target == 0)
		local_renderer_->turnUpdated();
	else if(target == 1)
		remote_renderer_->turnUpdated();

}

void QtkMainWindow::closeEvent(QCloseEvent * event)
{
	qDebug() << "Terminate Program";
	if(_callback)
		_callback->DisconnectFromServer();
}

void QtkMainWindow::keyPressEvent( QKeyEvent * event )
{
	if(event->key() == Qt::Key_Escape)
	{
		_callback->DisconnectFromServer();
	}
}
