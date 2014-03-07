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

 * MessageHandlerDeep.cpp
 *
 *  Created on: Nov 14, 2013
 *      Author: Francois Temasys
 */
#ifndef MESSAGEHANDLER_H
#define MESSAGEHANDLER_H

#include "talk/base/messagehandler.h"
#include "talk/base/messagequeue.h"
#include <qstring.h>

//interface to inherit for using the GAEChannel
class MessageHandlerDeep //: public talk_base::MessageHandler
{
    public:
        //virtual void OnMessage(Message * msg) = 0; FROM MESSAGE HANDLER
        virtual void OnMessage(std::string msg) = 0;
        virtual void OnOpen() = 0;
        virtual void OnClose() = 0;
        virtual void OnError(int code, std::string description) = 0;

 };
#endif // MESSAGEHANDLER_H
