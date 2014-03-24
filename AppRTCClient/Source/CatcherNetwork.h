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
 * CatcherNetwork.h
 *
 *  Created on: Nov 27, 2013
 *      Author: Francois Temasys
 */

#ifndef CATCHERNETWORK_H_
#define CATCHERNETWORK_H_

#include <qobject.h>
#include <qnetworkreply.h>
#include <qbytearray.h>
#include <qdebug.h>
#include "talk/base/logging.h"

class CatcherNetwork: public QObject
{
Q_OBJECT

public:
	CatcherNetwork(QObject * parent = 0);
	CatcherNetwork(QByteArray *message,QObject * parent);
	virtual ~CatcherNetwork();

public slots:
	void slotFinished(QNetworkReply * reply);
	void slotError(QNetworkReply::NetworkError e);

private:
	QByteArray *_message;
};

#endif /* CatcherNetwork_H_ */

