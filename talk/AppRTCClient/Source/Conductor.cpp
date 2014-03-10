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
 * Conductor.cpp
 *
 *  Created on: Nov 14, 2013
 *      Author: Francois Temasys
 */

#include "Conductor.h"

// Names used for a IceCandidate JSON object.
const char kCandidateSdpMidName[]        = "id";
const char kCandidateSdpMlineIndexName[] = "label";
const char kCandidateSdpName[]           = "candidate";

// Names used for a SessionDescription JSON object.
const char kSessionDescriptionTypeName[] = "type";
const char kSessionDescriptionSdpName[]  = "sdp";

const char kAudioLabel[]  = "audio_label";
const char kVideoLabel[]  = "video_label";
const char kStreamLabel[] = "stream_label";

static const QString MODEL_TOKEN      = "var channelToken =";
static const QString MODEL_USERNAME   = "var me =";
static const QString MODEL_ROOMKEY    = "var roomKey =";
static const QString MODEL_PC_CONFIG  = "var pcConfig =";
static const char MODEL_SEPARATOR     = '\'';
static const QString MODEL_INITIATOR  = "var initiator = ";

const std::string Conductor::_TAG = std::string("Conductor| ");

//----------------------------------------------------------------------------------------------
class DummySetSessionDescriptionObserver:
  public webrtc::SetSessionDescriptionObserver
{
public:
  static DummySetSessionDescriptionObserver * Create()
  {
    return new talk_base::RefCountedObject<DummySetSessionDescriptionObserver>();
  }

  virtual void OnSuccess()
  {
    LOG(INFO) << "Dummy session description: " << __FUNCTION__;
  }

  virtual void OnFailure(const std::string & error)
  {
    LOG(INFO) << "Dummy session description: " << __FUNCTION__ << " " << error;
  }

protected:
  DummySetSessionDescriptionObserver() { }
  ~DummySetSessionDescriptionObserver() { }
};

//----------------------------------------------------------------------------------------------
Conductor::Conductor(QObject *parent,PeerConnection *client,MainWindow *window)
  :QObject(parent),
 _client(client),
 _mainWindow(window),
 _initiator(0)
{

  webrtc::MediaConstraintsInterface::Constraint
    cAudio(webrtc::MediaConstraintsInterface::kOfferToReceiveAudio,"true");
  webrtc::MediaConstraintsInterface::Constraint
    cVideo(webrtc::MediaConstraintsInterface::kOfferToReceiveVideo,"true");
  webrtc::MediaConstraintsInterface::Constraints
    cAll;
  webrtc::MediaConstraintsInterface::Constraints
    cNULL;

  cAll.push_back(cAudio);
  cAll.push_back(cVideo);
  _constrains = new MediaConstraintsNative(cAll,cNULL);
  _client->RegisterObserver(this);
  _mainWindow->RegisterObserver(this);

  uint32_t id = 0;
}

//----------------------------------------------------------------------------------------------
void Conductor::Start()
{
  _mainWindow->SwitchToConnectUI();
}

//----------------------------------------------------------------------------------------------
bool Conductor::connection_active() const
{
  return _peer_connection.get() != NULL;
}


//----------------------------------------------------------------------------------------------
Conductor::~Conductor()
{
	assert(_peer_connection.get() == NULL);
	assert(_peer_connection_factory.get() == NULL);
}

//----------------------------------------------------------------------------------------------
// PeerConnectionClientObserver implementation.
void Conductor::OnDisconnected()
{
	DeletePeerConnection();
}

//----------------------------------------------------------------------------------------------
bool Conductor::CreatePC()
{
	bool res = true;
	if(_peer_connection.get() == NULL)
		if(!InitializePeerConnection())
		{
			res = false;
			_mainWindow->MessageBox(
        "Error Initializing Peer Connection",
        "Unable to initialize the peer connection",
        true);
		}
	return res;
}

//----------------------------------------------------------------------------------------------
void Conductor::CreateOffer()
{
  assert( !_id.isNull());
  assert(_peer_connection.get() != NULL);
  _peer_connection->CreateOffer(this,_constrains);
}


//----------------------------------------------------------------------------------------------
void Conductor::SetRemoteOffer(std::string type, std::string sdp)
{
	assert( !_id.isNull());

	if(!_peer_connection.get())
		InitializePeerConnection();

	webrtc::SessionDescriptionInterface *
    session_description( webrtc::CreateSessionDescription(type, sdp)
  );

  _peer_connection->SetRemoteDescription(
    DummySetSessionDescriptionObserver::Create(),
    session_description
  );

	if (session_description->type() == webrtc::SessionDescriptionInterface::kOffer)
		_peer_connection -> CreateAnswer(this, _constrains);
}

//----------------------------------------------------------------------------------------------
void  Conductor::addIceCandidate(std::string id, std::string sdp, int label)
{
	assert( !_id.isNull());

	talk_base::scoped_ptr<webrtc::IceCandidateInterface>
     candidate(webrtc::CreateIceCandidate(id,label,sdp));

	if (!candidate.get())
	{
		LOG(WARNING) << "Can't parse received candidate message.";
		return;
	}
	if(_peer_connection.get())
	{
		if (!_peer_connection -> AddIceCandidate(candidate.get()))
		{
			LOG(WARNING) << "Failed to apply the received candidate";
			return;
		}
	}
}

//----------------------------------------------------------------------------------------------
void Conductor::SetLocalMedia(webrtc::VideoTrackInterface *video_track)
{
	_mainWindow -> StartLocalRenderer(video_track);
}

//----------------------------------------------------------------------------------------------
void Conductor::SetRemoteMedia(webrtc::VideoTrackInterface *video_track)
{
	_mainWindow -> StartRemoteRenderer(video_track);
}

//----------------------------------------------------------------------------------------------
void Conductor::OnServerConnectionFailure()
{
  _mainWindow -> MessageBox("Error", QString("Failed to connect to " + _url.toString()),true);
}

//----------------------------------------------------------------------------------------------
void Conductor::sendMessage(Json::Value message)
{
  QString path( _baseURL.toString() + "/message?r=" + _roomid + "&u=" + _id );
  sendMessageToPath( message, path );
}


//----------------------------------------------------------------------------------------------
// MainWndCallback implementation.
void Conductor::StartLogin(QUrl url,QByteArray html)
{
    if (_client -> isConnected())
    {
        return;
    }
    _url              = url;
    _htmldata = QString(html);
	//std::string blabla = std::string(_htmldata.toLocal8Bit().constData());
	//std::cout << "HTML RECEIVE: " << blabla << std::endl;
    if (!getInfoFromData())
    {
      //Not received all the information from html
      _mainWindow -> MessageBox("Error", QString("Failed to connect to " + _url.toString()),true);
    }
    else
    {
        _client->doConnect(_token,_id,_roomid,_initiator);
        this->CreatePC();
        _mainWindow->SwitchToStreamingUI();
    }

}

//----------------------------------------------------------------------------------------------
void Conductor::DisconnectFromServer()
{
	//We disconnect from server
	LOG(INFO) << __FUNCTION__;

	_client -> SignOut();

	//TODO Current protection for disconnecting the peer before us
	QTimer *timer = new QTimer(this);
	QTimer::singleShot(1000,this,SLOT(DeleteAll()));


    _mainWindow->SwitchToConnectUI();
}

//----------------------------------------------------------------------------------------------
void Conductor::Close()
{
	DeleteAll();
}

//----------------------------------------------------------------------------------------------
// PeerConnectionObserver implementation.
void Conductor::OnError()
{
    LOG(LS_ERROR) << __FUNCTION__;
    _mainWindow->MessageBox("Peer Connection Error","Unknow error thrown by the peer connection", true);
}

//----------------------------------------------------------------------------------------------
void Conductor::OnStateChange(webrtc::PeerConnectionObserver::StateType state_changed)
{
	LOG(INFO) << _TAG+"state Changed";
}

//----------------------------------------------------------------------------------------------
// Called when a remote stream is added
void Conductor::OnAddStream(webrtc::MediaStreamInterface * stream)
{
    LOG(INFO) << __FUNCTION__ << "\n\n!!!!!!!!!!!!! Add Stream: " << stream -> label() << "!!!!!!!!!!!!!!!!!\n\n";

    webrtc::VideoTrackVector       tracks = stream -> GetVideoTracks();

	// Only render the first track.
	if (!tracks.empty())
	{
		webrtc::VideoTrackInterface *track = tracks[0];
		this->SetRemoteMedia(track);
	}
	stream -> Release();
}

//----------------------------------------------------------------------------------------------
void Conductor::OnRemoveStream(webrtc::MediaStreamInterface * stream)
{
    LOG(INFO) << __FUNCTION__ << " " << stream -> label();

    stream->AddRef();
    stream->Release();
}

//----------------------------------------------------------------------------------------------
void Conductor::OnRenegotiationNeeded()
{
	LOG(INFO) <<  __FUNCTION__ ;
}


//----------------------------------------------------------------------------------------------
void Conductor::OnIceChange()
{
	LOG(INFO) <<  __FUNCTION__;
}

//----------------------------------------------------------------------------------------------
void Conductor::OnIceCandidate(const webrtc::IceCandidateInterface * candidate)
{
	LOG(INFO) << __FUNCTION__ ;

	std::string sdp;
	if (!candidate->ToString(&sdp))
	{
		LOG(LS_ERROR) << "Failed to serialize candidate";

		return;
	}
	Json::Value message;
	message["type"] = "candidate";
	message[kCandidateSdpMidName] = candidate->sdp_mid();
	message[kCandidateSdpMlineIndexName] = candidate->sdp_mline_index();
	message[kCandidateSdpName] = sdp;

	sendMessage(message);

}

//----------------------------------------------------------------------------------------------
//  CreateSessionDescriptionObserver implementation.
void Conductor::OnSuccess(webrtc::SessionDescriptionInterface * desc)
{
	_peer_connection -> SetLocalDescription(DummySetSessionDescriptionObserver::Create(),desc);

	Json::Value message;
	message["type"] = desc->type();
	std::string sdp;
	desc -> ToString(&sdp);
	message["sdp"] = sdp;
	LOG(INFO) << _TAG+"Offer sent";

	sendMessage(message);
}

//----------------------------------------------------------------------------------------------
void Conductor::OnFailure(const std::string & error)
{
  LOG(LERROR) << _TAG+error;
}


//----------------------------------------------------------------------------------------------
bool Conductor::getInfoFromData()
{
  QString baseURLString = _url.toString();
  bool       validData = false;
  QList<QString> model;
  model << "https://apprtc.appspot.com";
  for (int i = 0; i < model.size(); ++i)
  {
    if (baseURLString.indexOf(model.at(i)) != -1)
    {
      baseURLString = model.at(i);
      _baseURL = QUrl::fromUserInput(baseURLString);
      validData = true;
      break;
    }
  }

  char separator = MODEL_SEPARATOR;
  char separator_server = '\"';
  signed int index = 0;


  // Token
  index = _htmldata.indexOf(MODEL_TOKEN);
  if (index != -1)
  {
    index  = _htmldata.indexOf(separator, index);
    _token = _htmldata.mid(index + 1,_htmldata.indexOf(separator, index + 1) - index - 1);
  }
  else
  {
    validData = false;
    LOG(LS_ERROR) << "NO TOKEN";
  }

	// ID given by server
  index = 0;
  index = _htmldata.indexOf(MODEL_USERNAME);

  if (index != -1)
  {
    index = _htmldata.indexOf(separator, index);
    _id   = _htmldata.mid(index + 1, _htmldata.indexOf(separator, index + 1) - index - 1);
  }
  else
  {
    validData = false;
    LOG(LS_ERROR) << "NO ID";
  }

	// roomKey
  index = 0;
  index = _htmldata.indexOf(MODEL_ROOMKEY);
  if (index != -1)
  {
    index   = _htmldata.indexOf(separator, index);
    _roomid = _htmldata.mid(index + 1,_htmldata.indexOf(separator, index + 1) - index - 1);
  }
  else
  {
    validData = false;
    LOG(LS_ERROR) << "NO ROOMKEY";
  }

	// Serveur used (turn,stun)
  index = 0;
  index = _htmldata.indexOf(MODEL_PC_CONFIG);
  if (index != -1)
  {
    index = _htmldata.indexOf("url", index);
    index = _htmldata.indexOf(separator_server, index+5);

    _serverUrl = _htmldata.mid(
      index + 1,_htmldata.indexOf(separator_server, index + 1) - index - 1
    );
  }
  else
  {
    validData = false;
    LOG(LS_ERROR) << "NO SERVER";
  }

  // Are we initiator?
  index = 0;
	index = _htmldata.indexOf(MODEL_INITIATOR);
	if (index != -1)
	{
		index   = _htmldata.indexOf(";", index);
		_initiator = _htmldata.mid(index-1,1).toInt();
	}
	else
	{
		validData = false;
		LOG(LS_ERROR) << "NO INITIATOR OR NOT";
	}

  LOG(INFO) << _TAG + "base URL: " + _baseURL.toString().toLocal8Bit().constData()
    + "\nusername: " + _id.toLocal8Bit().constData() + "\nserver url: "
    + _serverUrl.toLocal8Bit().constData();

  return validData;
}


void Conductor::sendMessageToPath(Json::Value message,QString path)
{
	Json::StyledWriter writer;
  QUrl          serviceURL(path);

  QThread *query = new postQuery(serviceURL,writer.write(message));
  query->start();
}

void Conductor::DeletePeerConnection()
{
	_mainWindow->StopRemoteRenderer();
	_peer_connection = NULL;
}

bool Conductor::InitializePeerConnection()
{
	ASSERT(_peer_connection.get() == NULL);

	if(_peer_connection_factory == NULL)
		_peer_connection_factory  = webrtc::CreatePeerConnectionFactory();

	if (!_peer_connection_factory.get())
	{
		_mainWindow->MessageBox("Error","Failed to initialize PeerConnectionFactory", true);
		return false;
	}

	webrtc::PeerConnectionInterface::IceServers servers;
	webrtc::PeerConnectionInterface::IceServer  server;
	server.uri = std::string(_serverUrl.toLocal8Bit().constData());
	servers.push_back(server);
	_peer_connection =  _peer_connection_factory->CreatePeerConnection(servers,NULL,NULL,this);

	if (!_peer_connection.get())
	{
		_mainWindow->MessageBox("Error","CreatePeerConnection failed", true);
		return false;
	}

	AddLocalStream();
	_mainWindow->SwitchToStreamingUI();
	return _peer_connection.get() != NULL;
}

void Conductor::AddLocalStream()
{
	if(!_localstream.get())
	{
		talk_base::scoped_refptr<webrtc::AudioTrackInterface>
      audio_track(
        _peer_connection_factory->CreateAudioTrack(
          kAudioLabel,
          _peer_connection_factory->CreateAudioSource(NULL)
        )
      );
		talk_base::scoped_refptr<webrtc::VideoTrackInterface>
      video_track(
        _peer_connection_factory->CreateVideoTrack(
          kVideoLabel,_peer_connection_factory->CreateVideoSource(
            OpenVideoCaptureDevice(),
            NULL
          )
        )
      );
		this->SetLocalMedia(video_track);

		_localstream = _peer_connection_factory->CreateLocalMediaStream( kStreamLabel );
		_localstream->AddTrack(audio_track);
		_localstream->AddTrack(video_track);
	}

	if (!_peer_connection->AddStream( _localstream, _constrains ))
	{
		LOG(LS_ERROR) << "Adding stream to PeerConnection failed";
	}
}

cricket::VideoCapturer* Conductor::OpenVideoCaptureDevice()
{
  talk_base::scoped_ptr<cricket::DeviceManagerInterface>
    dev_manager(cricket::DeviceManagerFactory::Create());

  if (!dev_manager->Init())
  {
    LOG(LS_ERROR) << "Can't create device manager";
    return NULL;
  }

  std::vector<cricket::Device> devs;
  if (!dev_manager->GetVideoCaptureDevices(&devs))
  {
    LOG(LS_ERROR) << "Can't enumerate video devices";
    return NULL;
  }

  std::vector<cricket::Device>::iterator dev_it = devs.begin();
  cricket::VideoCapturer* capturer = NULL;
  for (; dev_it != devs.end(); ++dev_it)
  {
    capturer = dev_manager->CreateVideoCapturer(*dev_it);
    if (capturer != NULL)
      break;
  }
  return capturer;
}

void Conductor::DeleteAll()
{
	//Delete the peer connections and stop all rendering
	LOG(INFO) << "Stop Local Renderer";
	_mainWindow->StopLocalRenderer();
	LOG(INFO) << "Delete Local Stream";
	_localstream = NULL;
	LOG(INFO) << "Stop Remote Renderer";
	_mainWindow->StopRemoteRenderer();
	LOG(INFO) << "Destroy Peer Connection";
	_peer_connection = NULL;
	LOG(INFO) << "Destroy Peer Connection Factory";
	_peer_connection_factory = NULL;
	_constrains = NULL;
}
