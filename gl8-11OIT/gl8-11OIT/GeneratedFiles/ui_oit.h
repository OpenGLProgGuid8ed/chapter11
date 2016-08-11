/********************************************************************************
** Form generated from reading UI file 'oit.ui'
**
** Created by: Qt User Interface Compiler version 5.3.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_OIT_H
#define UI_OIT_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_OITClass
{
public:

    void setupUi(QWidget *OITClass)
    {
        if (OITClass->objectName().isEmpty())
            OITClass->setObjectName(QStringLiteral("OITClass"));
        OITClass->resize(600, 400);

        retranslateUi(OITClass);

        QMetaObject::connectSlotsByName(OITClass);
    } // setupUi

    void retranslateUi(QWidget *OITClass)
    {
        OITClass->setWindowTitle(QApplication::translate("OITClass", "OIT", 0));
    } // retranslateUi

};

namespace Ui {
    class OITClass: public Ui_OITClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_OIT_H
