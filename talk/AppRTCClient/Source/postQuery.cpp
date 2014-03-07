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
 * postQuery.cpp
 *
 *  Created on: Nov 27, 2013
 *      Author: Francois Temasys
 */

#include "postQuery.h"

//----------------------------------------------------------------------------------------------
postQuery::postQuery(QUrl path, std::string jsonmessage) :
  QThread(),
  _path(path)
{
  _qnam = NULL;
  _catch = NULL;
  _message = new QByteArray(jsonmessage.c_str());
}
//----------------------------------------------------------------------------------------------
postQuery::~postQuery()
{
  delete _qnam;
  delete _catch;
}
//----------------------------------------------------------------------------------------------
void postQuery::run()
{
  _qnam = new QNetworkAccessManager();
  _catch = new CatcherNetwork(_message,0);

  QByteArray postDataSize = QByteArray::number(_message->size());

  // Build POST request
  QNetworkRequest request(_path);

  request.setRawHeader("Content-Type", "application/json");
  request.setRawHeader("Content-Length", postDataSize);

  // Send POST request
  _qnam->post(request, *_message);
  QObject::connect(
    _qnam,
    SIGNAL(finished(QNetworkReply *)),
    _catch,
    SLOT(slotFinished(QNetworkReply *))
  );
  exec();
}

