/* Copyright (c) 2014 Temasys Communications pvt, ltd

   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at

       http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and 
   limitations under the License.

 * MainWindowObserver.h
 *
 *  Created on: Nov 15, 2013
 *      Author: Francois Temasys
 */

#ifndef MAINWINDOWOBSERVER_H_
#define MAINWINDOWOBSERVER_H_

#include <QUrl>

class MainWindowObserver
{
    public:
        virtual void StartLogin(QUrl url, QByteArray html) = 0;
        virtual void DisconnectFromServer() = 0;
        virtual void Close() = 0;

        virtual ~MainWindowObserver(){}
};
#endif /* MAINWINDOWOBSERVER_H_ */

