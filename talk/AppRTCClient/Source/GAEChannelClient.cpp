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
 * GAEChannelClient.cpp
 *
 *  Created on: Nov 14, 2013
 *      Author: Francois Temasys
 */
#include "GAEChannelClient.h"
#include <QDebug>
#include <QUrl>
#include <QWebElementCollection>

//Static Const
const QString GAEChannelClient::_TAG = QString("GAEChannelClient| ");

//constructor
GAEChannelClient::GAEChannelClient(QString token, MessageHandlerDeep *handler)
{
  _webView = new QWebView();
  _proxyingMessageHandler = new ProxyingMessageHandler(_webView, handler, token);

  QObject::connect(
    _webView->page()->mainFrame(),
    SIGNAL(javaScriptWindowObjectCleared()),
    this,
    SLOT(addJSObject())
  );

  QFileInfo channelFile( QCoreApplication::applicationDirPath() + "/channel.html" );
  if (channelFile.exists())
  {
    QUrl localUrl("file:///"+channelFile.absoluteFilePath());
    _webView->setUrl(localUrl);
  }
  else
  {
    qDebug() << _TAG + " |Channel not exist";
		exit(EXIT_FAILURE);
  }
}

GAEChannelClient::~GAEChannelClient()
{
	delete _proxyingMessageHandler;
	delete _webView;
}

void GAEChannelClient::addJSObject()
{
  // Add GAE Channel to JavaScript Frame as member "imageAnalyzer".
  _webView->page()->mainFrame()->addToJavaScriptWindowObject(
    QString("androidMessageHandler"),
    _proxyingMessageHandler
  );
}
