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
 * ConnectWindow.h
 *
 *  Created on: Nov 14, 2013
 *      Author: Francois Temasys
 */
#ifndef ConnectWindow_H
#define ConnectWindow_H


#include <QWidget>

#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QSslError>
#include <QUrl>
#include <QLineEdit>
#include <QLabel>
#include <QPushButton>
#include <QProgressDialog>
#include <QMessageBox>
#include <QDialogButtonBox>
#include <QHBoxLayout>
#include <qauthenticator.h>
#include <QDebug>

#include "ui_authenticationdialog.h"

class ConnectWindow : public QWidget
{
 Q_OBJECT

public:
    ConnectWindow(QWidget *parent, QUrl url);
    ~ConnectWindow();

signals:
 void toConnect(QUrl url,QByteArray data);
public slots:
	void startRequest();
private slots:

    void httpFinished();
    void slotAuthenticationRequired(QNetworkReply*,QAuthenticator *);
    void enableconnectButton();


private:
    QWidget *_parent;
    QLabel *_statusLabel;
    QLabel *_urlLabel;
    QLineEdit *_urlLineEdit;
    QPushButton *_connectButton;
    QDialogButtonBox *_buttonBox;


    QUrl _url;
    QNetworkAccessManager *_qnam;
    QNetworkReply *_reply;
    QByteArray _htmldata;
};

#endif
