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
 * postQuery.h
 *
 *  Created on: Nov 27, 2013
 *      Author: Francois Temasys
 */

#ifndef POSTQUERY_H_
#define POSTQUERY_H_

//libjingle
#include "talk/base/logging.h"

//QT
#include <QUrl>
#include <qtconcurrentrun.h>
#include <qthread.h>
#include <QNetworkAccessManager>
#include <qnetworkreply.h>

#include "CatcherNetwork.h"

class postQuery :
	public QThread
{
public:
	postQuery( QUrl path, std::string message );
	virtual ~postQuery();
	void run();

private:
	QNetworkAccessManager *_qnam;
	QByteArray            *_message;
	CatcherNetwork        *_catch;
	QUrl _path;

private slots:
	void postFinished();
};

#endif /* POSTQUERY_H_ */
