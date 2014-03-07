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
 * PeerConnection.h
 *
 *  Created on: Nov 6, 2013
 *      Author: Francois Temasys
 */

#ifndef PeerConnection_H_
#define PeerConnection_H_

//QT
#include <QUrl>

#include <assert.h>
//libjingle
#include "talk/base/logging.h"

//Perso
#include "Interface/PeerConnectionClientObserver.h"
#include "Interface/MessageHandlerDeep.h"
#include "GAEChannelClient.h"

class PeerConnection:
  public QObject,
  public MessageHandlerDeep
{
  Q_OBJECT

public:
  enum State { NOT_CONNECTED, SIGNING_IN, CONNECTED };

  void doConnect( QString token, QString id,QString roomid,int initiator);
  PeerConnection(QObject *  parent);

  //Access
  bool isConnected();
  //Used by the conductor
  void RegisterObserver(PeerConnectionClientObserver* callback);
  bool SignOut();
  //////////////////////////////////////////////////////////////
	// Message Handler Deep Implementation
	//
  void OnOpen();
  void OnMessage(std::string data);
  void OnClose();
  void OnError(int code,std::string description);
  //
  //////////////////////////////////////////////////////////////

  virtual ~PeerConnection();

protected:
  QString  _id;
  QString  _roomid;
  QString  _token;
  int			_initiator;
  bool    _started;
  std::deque<Json::Value> _msgQueue;

private:
  static const std::string _TAG;
  // functions
  void maybeStart();
  void processSignalingMessage(Json::Value msg);

	// parameters
  State _state;
  bool  _channelReady;

  PeerConnectionClientObserver* _callback;
  GAEChannelClient *   _channelClient;
};
#endif /* PeerConnection_H_ */
