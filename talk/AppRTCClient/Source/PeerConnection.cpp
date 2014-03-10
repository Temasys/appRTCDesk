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
 * PeerConnection.cpp
 *
 *  Created on: Nov 6, 2013
 *      Author: Francois Temasys
 */

#include "PeerConnection.h"

const std::string
PeerConnection::
_TAG = std::string("PeerConnection| ");

PeerConnection::
PeerConnection(QObject *  parent):
  QObject(parent)
{
	_initiator     = 0;
	_started       = false;
  _state         = NOT_CONNECTED;
  _channelReady  = false;
  _channelClient = NULL;
  _callback      = NULL;
  _id            = QString("");
}

PeerConnection::
~PeerConnection()
{
  delete _channelClient;
  _callback = NULL;
}

void
PeerConnection::
doConnect(QString token, QString id, QString roomid, int initiator)
{
  _token         = token;
  _id            = id;
  _roomid        = roomid;
  _channelClient = new GAEChannelClient(_token, this);
  _initiator     = initiator;
}

bool 
PeerConnection::
isConnected()
{
  return _state == CONNECTED;
}

void 
PeerConnection::
RegisterObserver(PeerConnectionClientObserver * callback)
{
  assert(!_callback);
  _callback = callback;
}

bool
PeerConnection::
SignOut()
{
  if (_state == NOT_CONNECTED)
  {
    return true;
  }

  _channelClient  = NULL;
  _channelReady   = false;
  _started        = false;
  _initiator      = 0;

  Json::Value message;
  message["type"] = "bye";

	_callback->sendMessage(message);
	_state = NOT_CONNECTED;

  return true;
}


void PeerConnection::OnOpen()
{
  LOG(INFO) << _TAG + "onOpen";
  LOG(INFO) << _TAG + "My id: " + _id.toLocal8Bit().constData() + " Room id: "
    + _roomid.toLocal8Bit().constData();

  _channelReady = true;
  _state = CONNECTED;
  this->maybeStart();
}

//void PeerConnection::OnMessage(talk_base::Message data)
void PeerConnection::OnMessage(std::string data)
{
	Json::Value  msg;   // will contains the root value after parsing.
	Json::Reader reader;
	bool parsingSuccessful = reader.parse( data, msg );
	if(!parsingSuccessful)
	{
	  // report to the user the failure and their locations in the document.
	  LOG(LERROR)  << "Failed to parse configuration\n"<< reader.getFormattedErrorMessages();
	  return;
	}
	std::string type = msg.get("type","ERROR").asString();
  if (!_initiator && !_started)
  {
		if (type == "offer")
		{
		  // Add offer to the beginning of msgQueue, since we can't handle
		  // Early candidates before offer at present.
		  _msgQueue.push_front(msg);
		  // Callee creates PeerConnection
		  maybeStart();
		}
		else
		{
		  _msgQueue.push_back(msg);
		}
	}
  else
	{
    processSignalingMessage(msg);
	}
}


void PeerConnection::OnClose()
{
	LOG(INFO) << "onClose";
}

void PeerConnection::OnError(int code,std::string description)
{
	LOG(INFO) << "onError";
}


void PeerConnection::maybeStart()
{
	if (!_started && _channelReady )
	{
     _callback->CreatePC();
	   _started = true;

	   if (_initiator)
	     _callback->CreateOffer();
	   else
	   {
       while (_msgQueue.size() > 0)
       {
         this->processSignalingMessage(_msgQueue.front());
         _msgQueue.pop_front();
	     }
	   }
  }
}


void PeerConnection::processSignalingMessage(Json::Value msg)
{
	std::string type = msg.get("type","ERROR").asString();

	////////////////////////////////////////////////////////////////////////////////
	// SDP message, either offer or answer.
	////////////////////////////////////////////////////////////////////////////////

	if (type == "answer" || type == "offer")
	{
		LOG(INFO) << "onMessage :\n"<< msg;
		std::string sdp = msg.get("sdp","ERROR").asString();
		_callback->SetRemoteOffer(type,sdp);
	}
	else if(type == "candidate")
	{
		LOG(INFO) << _TAG+"Received " + type;
		std::string sdp = msg.get("candidate","ERROR").asString();
		std::string id = msg.get("id","ERROR").asString();
		int label = msg.get("label","ERROR").asInt();
		_callback->addIceCandidate(id,sdp,label);
	}
	////////////////////////////////////////////////////////////////////////////////
	// Bye
	////////////////////////////////////////////////////////////////////////////////
	else if (type == "bye")
	{
		LOG(INFO) << _TAG+"Received " + type;
		_callback->OnDisconnected();
	}
}

