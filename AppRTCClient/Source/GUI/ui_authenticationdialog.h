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


 * authenticationdialog.h
 *
 *  Created on: Nov 14, 2013
 *      Author: Francois Temasys
 */

#ifndef UI_AUTHENTICATIONDIALOG_H
#define UI_AUTHENTICATIONDIALOG_H

#include <QtCore/QVariant>
#include <QAction>
#include <QApplication>
#include <QButtonGroup>
#include <QDialog>
#include <QDialogButtonBox>
#include <QGridLayout>
#include <QHeaderView>
#include <QLabel>
#include <QLineEdit>
#include <QSpacerItem>

QT_BEGIN_NAMESPACE

class Ui_Dialog
{
public:
    QGridLayout *gridLayout;
    QLabel *label;
    QLabel *label_2;
    QLineEdit *userEdit;
    QLabel *label_3;
    QLineEdit *passwordEdit;
    QDialogButtonBox *buttonBox;
    QLabel *label_4;
    QLabel *siteDescription;
    QSpacerItem *spacerItem;

    void setupUi(QDialog *Dialog)
    {
        if (Dialog->objectName().isEmpty())
            Dialog->setObjectName(QString("Dialog"));
        Dialog->resize(389, 243);
        gridLayout = new QGridLayout(Dialog);
        gridLayout->setObjectName(QString("gridLayout"));
        label = new QLabel(Dialog);
        label->setObjectName(QString("label"));
        label->setWordWrap(false);

        gridLayout->addWidget(label, 0, 0, 1, 2);

        label_2 = new QLabel(Dialog);
        label_2->setObjectName(QString("label_2"));

        gridLayout->addWidget(label_2, 2, 0, 1, 1);

        userEdit = new QLineEdit(Dialog);
        userEdit->setObjectName(QString("userEdit"));

        gridLayout->addWidget(userEdit, 2, 1, 1, 1);

        label_3 = new QLabel(Dialog);
        label_3->setObjectName(QString("label_3"));

        gridLayout->addWidget(label_3, 3, 0, 1, 1);

        passwordEdit = new QLineEdit(Dialog);
        passwordEdit->setObjectName(QString("passwordEdit"));

        gridLayout->addWidget(passwordEdit, 3, 1, 1, 1);

        buttonBox = new QDialogButtonBox(Dialog);
        buttonBox->setObjectName(QString("buttonBox"));
        buttonBox->setOrientation(Qt::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);

        gridLayout->addWidget(buttonBox, 5, 0, 1, 2);

        label_4 = new QLabel(Dialog);
        label_4->setObjectName(QString("label_4"));

        gridLayout->addWidget(label_4, 1, 0, 1, 1);

        siteDescription = new QLabel(Dialog);
        siteDescription->setObjectName(QString("siteDescription"));
        QFont font;
        font.setBold(true);
        font.setWeight(75);
        siteDescription->setFont(font);
        siteDescription->setWordWrap(true);

        gridLayout->addWidget(siteDescription, 1, 1, 1, 1);

        spacerItem = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout->addItem(spacerItem, 4, 0, 1, 1);


        retranslateUi(Dialog);
        QObject::connect(buttonBox, SIGNAL(accepted()), Dialog, SLOT(accept()));
        QObject::connect(buttonBox, SIGNAL(rejected()), Dialog, SLOT(reject()));

        QMetaObject::connectSlotsByName(Dialog);
    } // setupUi

    void retranslateUi(QDialog *Dialog)
    {
        Dialog->setWindowTitle(QApplication::translate("Dialog", "Http authentication required", 0));
        label->setText(QApplication::translate("Dialog", "You need to supply a Username and a Password to access this site", 0));
        label_2->setText(QApplication::translate("Dialog", "Username:", 0));
        label_3->setText(QApplication::translate("Dialog", "Password:", 0));
        label_4->setText(QApplication::translate("Dialog", "Site:", 0));
        siteDescription->setText(QApplication::translate("Dialog", "%1 at %2", 0));
    } // retranslateUi

};

namespace Ui {
    class Dialog: public Ui_Dialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_AUTHENTICATIONDIALOG_H
