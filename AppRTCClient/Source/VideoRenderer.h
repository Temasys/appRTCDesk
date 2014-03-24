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
 * VideoRenderer.h
 *
 *  Created on: Dec 2, 2013
 *      Author: Francois Temasys
 */

#ifndef VIDEORENDERER_H_
#define VIDEORENDERER_H_

//libjingle

//QT
#include <qthread.h>
#include <qmutex.h>

//Perso
#include "GUI/QtMainWindow.h"

class QtkMainWindow;

class VideoRenderer:
		public QThread,
		public webrtc::VideoRendererInterface

{
	Q_OBJECT
	public:
		VideoRenderer(const int target,webrtc::VideoTrackInterface * track_to_render);

		virtual ~VideoRenderer();
		////////////////////////////////////////////////////////////////////////////////
		// VideoRendererInterface implementation
		//																			  //
		virtual void SetSize(int width,int height);
		virtual void RenderFrame(const cricket::VideoFrame * frame);
		//																			  //
		////////////////////////////////////////////////////////////////////////////////

		////////////////////////////////////////////////////////////////////////////////
		// QThread implementation
		//																			  //
		void run();
		//																			  //
		////////////////////////////////////////////////////////////////////////////////

		const uint8 * getImage() const;
		int getWidth() const;
		int getHeight() const;
		void turnUpdated(){_updated = true;};

	signals:
		void imageToUpdate(int target,QImage *image);

	protected:
		talk_base::scoped_ptr<uint8[]>                        _image;
		int                                                   _width;
		int                                                   _height;
		const int _nameReceiver;
		bool _updated;
		talk_base::scoped_ptr<QImage> _imageReceiver;
		talk_base::scoped_refptr<webrtc::VideoTrackInterface> _rendered_track;
		QMutex _mutex;
};

#endif /* VIDEORENDERER_H_ */
