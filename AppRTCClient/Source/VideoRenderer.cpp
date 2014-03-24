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
 * VideoRenderer.cpp
 *
 *  Created on: Dec 2, 2013
 *      Author: Francois Temasys
 */

#include "VideoRenderer.h"


VideoRenderer::VideoRenderer(const int nameReceiver,webrtc::VideoTrackInterface *track_to_render)
:_nameReceiver(nameReceiver)
{
  _width          = 0;
  _height         = 0;
  _rendered_track = track_to_render;
	_updated        = true;
  _rendered_track -> AddRenderer(this);
}

VideoRenderer::~VideoRenderer()
{
	qDebug() << "Destroy Video Renderer";
	this->wait();
  _rendered_track->RemoveRenderer(this);
  _rendered_track = NULL;
}

////////////////////////////////////////////////////////////////////////////////
// VideoRendererInterface implementation
//
void VideoRenderer::SetSize(int width,int height)
{
  _width  = width;
  _height = height;
  _image.reset(new uint8[width * height * 4]);
}


void VideoRenderer::RenderFrame(const cricket::VideoFrame * frame)
{
  int size = _width * _height * 4;
  frame->ConvertToRgbBuffer(cricket::FOURCC_ARGB, _image.get(), size, _width * 4);
  this->start();
}

////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////
// QThread implementation
//
void VideoRenderer::run()
{
	_mutex.lock();
	//TODO scale possibility

	uchar* data = reinterpret_cast<uchar *>(_image.get());
	if(_updated == true)
	{	
		_updated = false;
		_imageReceiver.reset(new QImage(data, _width, _height, QImage::Format_ARGB32));
		Q_EMIT imageToUpdate(_nameReceiver,_imageReceiver.get());
	}
	_mutex.unlock();
}

////////////////////////////////////////////////////////////////////////////////
const uint8 * VideoRenderer::getImage() const
{
	return _image.get();
}

int VideoRenderer::getWidth() const
{
	return _width;
}

int VideoRenderer::getHeight() const
{
	return _height;
}
