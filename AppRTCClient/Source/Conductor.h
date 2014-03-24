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
 * Conductor.h
 *
 *  Created on: Nov 14, 2013
 *      Author: Francois Temasys
 */

#ifndef CONDUCTOR_H_
#define CONDUCTOR_H_

// libjingle
#include "talk/app/webrtc/videosourceinterface.h"
#include "talk/app/webrtc/peerconnectioninterface.h"
#include "talk/app/webrtc/jsep.h"

#include "talk/base/common.h"
#include "talk/base/scoped_ptr.h"
#include "talk/base/logging.h"
#include "talk/media/devices/devicemanager.h"
#include "talk/base/refcount.h"

#include "talk/base/json.h"

// this project
#include "Interface/MainWindow.h"
#include "Interface/PeerConnectionClientObserver.h"
#include "PeerConnection.h"
#include "postQuery.h"
#include "MediaConstraintsNative.h"
#include <iostream>

// QT
#include <QUrl>
#include <qdebug.h>
#include <QTimer>
#include <qtconcurrentrun.h>


/** class Conductor
 *
 *
 */
class Conductor:
  public QObject,
  public PeerConnectionClientObserver,
  public MainWindowObserver,
  public webrtc::PeerConnectionObserver,
  public webrtc::CreateSessionDescriptionObserver
{
  Q_OBJECT

public:
  Conductor(QObject *parent,PeerConnection * client, MainWindow *window);
  void Start();
  bool connection_active() const;

protected:
  ~Conductor();

  void OnDisconnected();
	bool CreatePC();
	void CreateOffer();
	void SetRemoteOffer(std::string type, std::string sdp);
	void addIceCandidate(std::string id, std::string sdp, int label);
	void SetLocalMedia(webrtc::VideoTrackInterface *video_track);
	void SetRemoteMedia(webrtc::VideoTrackInterface *video_track);
	void OnServerConnectionFailure();
	void sendMessage(Json::Value message);

  // MainWindowObserver implementation.
  void StartLogin(QUrl url, QByteArray html);
  void DisconnectFromServer();
  void Close();

  // PeerConnectionObserver implementation.
	virtual void OnError();
  virtual void OnStateChange(webrtc::PeerConnectionObserver::StateType state_changed);
  virtual void OnAddStream(webrtc::MediaStreamInterface * stream);
  virtual void OnRemoveStream(webrtc::MediaStreamInterface * stream);
  virtual void OnRenegotiationNeeded();
  virtual void OnIceChange();
  virtual void OnIceCandidate(const webrtc::IceCandidateInterface * candidate);

  //  CreateSessionDescriptionObserver implementation.
  virtual void OnSuccess(webrtc::SessionDescriptionInterface * desc);
  virtual void OnFailure(const std::string & error);

  PeerConnection * _client;
  MainWindow *     _mainWindow;

  talk_base::scoped_refptr<webrtc::PeerConnectionInterface>        _peer_connection;
  talk_base::scoped_refptr<webrtc::MediaStreamInterface>           _localstream;
  talk_base::scoped_refptr<webrtc::PeerConnectionFactoryInterface> _peer_connection_factory;
  MediaConstraintsNative *_constrains;

  QUrl    _url;
  QUrl 		_baseURL;
  QString _htmldata;
  QString _id;

	QString _roomid;
	QString _token;
	QString _serverUrl;
	int     _initiator;

	std::string _labelRemote;

private:
  bool getInfoFromData();
  void sendMessageToPath(Json::Value message,QString path);
  void DeletePeerConnection();
  bool InitializePeerConnection();
  void AddLocalStream();

  cricket::VideoCapturer* OpenVideoCaptureDevice();

  static const std::string _TAG;

private slots:
  void DeleteAll();
};
#endif /* CONDUCTOR_H_ */
