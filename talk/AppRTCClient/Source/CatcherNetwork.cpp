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
 * CatcherNetwork.cpp
 *
 *  Created on: Nov 27, 2013
 *      Author: Francois Temasys
 */

#include "CatcherNetwork.h"

CatcherNetwork::
CatcherNetwork( QObject * parent )
:QObject(parent)
{
	_message = NULL;
}

CatcherNetwork::
CatcherNetwork( QByteArray *message, QObject * parent )
:QObject(parent)
{
	_message = message;
}

CatcherNetwork::
~CatcherNetwork()
{
	delete _message;
}

void
CatcherNetwork::
slotFinished( QNetworkReply * reply )
{
	if(reply->error() != QNetworkReply::NoError)
		qDebug() << "network error! " << reply->error();
	else
	{
		if(!_message->isNull())
		{
			LOG(INFO) << "====> postSuccesfully Sent:";
			qDebug() << *_message;
			LOG(INFO) << " ===========>";
		}
	}
	reply->deleteLater();
}

void
CatcherNetwork::
slotError( QNetworkReply::NetworkError e )
{
	LOG(LERROR) << "slot Error" << e;
}
