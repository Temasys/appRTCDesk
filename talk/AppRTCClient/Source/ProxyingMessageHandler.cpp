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
 * ProxyingMessageHandler.cpp
 *
 *  Created on: Nov 14, 2013
 *      Author: Francois Temasys
 */
#include "ProxyingMessageHandler.h"

ProxyingMessageHandler::ProxyingMessageHandler(QWebView *inView, MessageHandlerDeep *handler,QString token)
    : webView(inView),handler(handler),token(token)
{
    disconnected = false;
}

ProxyingMessageHandler::~ProxyingMessageHandler()
{
	handler = NULL;
	webView = NULL;
}

void ProxyingMessageHandler::disconnect()
{
 disconnected = true;
}

bool ProxyingMessageHandler::isDisconnected()
{
 return disconnected;
}

//JavascriptInterface
QString ProxyingMessageHandler::getToken()
{
 return token;
}

//JavascriptInterface
void ProxyingMessageHandler::onOpen()
{
    //TODO Thread
    if (!disconnected)
        handler->OnOpen();
}

//JavascriptInterface
void ProxyingMessageHandler::onMessage(QString data)
{
    //TODO Thread
    if (!disconnected)
        handler->OnMessage(data.toLocal8Bit().constData());
}

//JavascriptInterface
void ProxyingMessageHandler::onClose()
{
    //TODO Thread
    if (!disconnected)
        handler->OnClose();
}

//JavascriptInterface
void ProxyingMessageHandler::onError(int code, QString description)
{
    //TODO Thread
    if (!disconnected)
        handler->OnError(code, description.toLocal8Bit().constData());
}
