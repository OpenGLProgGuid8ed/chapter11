/********************************************************************************
** Form generated from reading UI file 'doublewrite.ui'
**
** Created by: Qt User Interface Compiler version 5.3.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_DOUBLEWRITE_H
#define UI_DOUBLEWRITE_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_doubleWriteClass
{
public:

    void setupUi(QWidget *doubleWriteClass)
    {
        if (doubleWriteClass->objectName().isEmpty())
            doubleWriteClass->setObjectName(QStringLiteral("doubleWriteClass"));
        doubleWriteClass->resize(600, 400);

        retranslateUi(doubleWriteClass);

        QMetaObject::connectSlotsByName(doubleWriteClass);
    } // setupUi

    void retranslateUi(QWidget *doubleWriteClass)
    {
        doubleWriteClass->setWindowTitle(QApplication::translate("doubleWriteClass", "doubleWrite", 0));
    } // retranslateUi

};

namespace Ui {
    class doubleWriteClass: public Ui_doubleWriteClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DOUBLEWRITE_H
