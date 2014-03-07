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
 * main.cpp
 *
 *  Created on: Nov 14, 2013
 *      Author: Francois Temasys
 */
#include <qdebug.h>
#include <QApplication>

#include "ActivityManager.h"
#include "talk/base/ssladapter.h"

int main(int argc, char *argv[])
{
    QApplication a(argc,argv);
    ActivityManager *am;
    talk_base::InitializeSSL();
    if (argc > 1)
    {
    	std::string idroomstart(argv[1]);
    	idroomstart.insert(0,"https://apprtc.appspot.com/?r=");
    	am = new ActivityManager(0,QString(idroomstart.c_str()));
    }
	else
	{
		qDebug() << "Default website start";
    	am = new ActivityManager();
	}
    am->Start();
    QCoreApplication::exec();
    talk_base::CleanupSSL();
    return EXIT_SUCCESS;
}
