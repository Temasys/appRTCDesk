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
 * ProxyingMessageHandler.h
 *
 *  Created on: Nov 14, 2013
 *      Author: Francois Temasys
 */
#ifndef PROXYINGMESSAGEHANDLER_H
#define PROXYINGMESSAGEHANDLER_H

#include "Interface/MessageHandlerDeep.h"

#include <QString>
#include <QWebView>
#include <QThread>


// Helper class for proxying callbacks from the C++<->JS interaction
class ProxyingMessageHandler:public QObject
{
   Q_OBJECT
private:
    const QWebView *webView;
    MessageHandlerDeep *handler;
    bool disconnected;
    const QString token;

public:
    ProxyingMessageHandler(QWebView *inView, MessageHandlerDeep *handler,QString token);
    ~ProxyingMessageHandler();
    void disconnect();
    bool isDisconnected();
public slots:
    QString getToken();
    void onOpen();
    void onMessage(QString data);
    void onClose();
    void onError(const int code,QString description);
};
#endif // PROXYINGMESSAGEHANDLER_H
