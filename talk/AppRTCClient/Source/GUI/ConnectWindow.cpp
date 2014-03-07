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

 * ConnectWindow.cpp
 *
 *  Created on: Nov 14, 2013
 *      Author: Francois Temasys
 */
#include "ConnectWindow.h"

ConnectWindow::ConnectWindow(QWidget *parent,QUrl url)
 : QWidget(parent)
{
    _parent = parent;

    _urlLineEdit = new QLineEdit(url.toString());
    _qnam = new QNetworkAccessManager();

    _urlLabel = new QLabel(tr("&URL:"));
    _urlLabel->setBuddy(_urlLineEdit);
    _statusLabel = new QLabel(tr("Please enter the URL"));
    _statusLabel->setWordWrap(true);

    _connectButton = new QPushButton(tr("Connect"));
    _connectButton->setDefault(true);

    _buttonBox = new QDialogButtonBox;
    _buttonBox->addButton(_connectButton, QDialogButtonBox::ActionRole);

    //When text is changed enable the button
    connect(_urlLineEdit,SIGNAL(textChanged(QString)),this,SLOT(enableconnectButton()));
    connect(_urlLineEdit,SIGNAL(returnPressed()),this,SLOT(startRequest()));
    connect(_connectButton,SIGNAL(clicked()),this,SLOT(startRequest()));

    QHBoxLayout *topLayout = new QHBoxLayout;
    topLayout->addWidget(_urlLabel);
    topLayout->addWidget(_urlLineEdit);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addLayout(topLayout);
    mainLayout->addWidget(_statusLabel);
    mainLayout->addWidget(_buttonBox);
    setLayout(mainLayout);


    setWindowTitle(tr("HTTP"));
    this->setFixedSize(600,200);
    _urlLineEdit->setFocus();
}

ConnectWindow::~ConnectWindow()
{
	delete _qnam;
	delete _reply;
	delete _urlLineEdit;
	delete _urlLabel;
	delete _statusLabel;
}



void ConnectWindow::enableconnectButton()
{
    _connectButton->setEnabled(!_urlLineEdit->text().isEmpty());
}


void ConnectWindow::startRequest()
{
	qDebug() << "Start request";
    _url = _urlLineEdit->text();
    _reply = _qnam->get(QNetworkRequest(_url));
    //request finished
    connect(_reply,SIGNAL(finished()),this,SLOT(httpFinished()));
}




void ConnectWindow::httpFinished()
{
    QVariant redirectionTarget = _reply->attribute(QNetworkRequest::RedirectionTargetAttribute);
    if (_reply->error())
    {
        QMessageBox::information(this, tr("HTTP"),tr("Connection failed: %1.").arg(_reply->errorString()));
        _connectButton->setEnabled(true);
    }
    else if (!redirectionTarget.isNull())
    {
		QUrl newUrl = _url.resolved(redirectionTarget.toUrl());
		if (QMessageBox::question(this, tr("HTTP"),
							   tr("Redirect to %1 ?").arg(newUrl.toString()),
							   QMessageBox::Yes | QMessageBox::No) == QMessageBox::Yes)
		{
			 _url = newUrl;
			 _reply->deleteLater();
			 startRequest();
			 return;
		}
    }
    else
    {
        //Received all the HTML without error ready to fetch and work on it
        _statusLabel->setText(tr("Connection with %1").arg(_url.toString()));
        _connectButton->setEnabled(true);

        qDebug() << "Connection to " + _url.toString();
        _htmldata = _reply->readAll();
        Q_EMIT(toConnect(_url,_htmldata));
    }
    _reply->deleteLater();
}




void ConnectWindow::slotAuthenticationRequired(QNetworkReply*,QAuthenticator *authenticator)
{
    QDialog dlg;
    Ui::Dialog ui;
    ui.setupUi(&dlg);
    dlg.adjustSize();
    ui.siteDescription->setText(tr("%1 at %2").arg(authenticator->realm()).arg(_url.host()));

    // Did the URL have information? Fill the UI
    // This is only relevant if the URL-supplied credentials were wrong
    ui.userEdit->setText(_url.userName());
    ui.passwordEdit->setText(_url.password());

    if (dlg.exec() == QDialog::Accepted)
    {
        authenticator->setUser(ui.userEdit->text());
        authenticator->setPassword(ui.passwordEdit->text());
    }
}
