/********************************************************************************
** Form generated from reading UI file 'overdrawcount.ui'
**
** Created by: Qt User Interface Compiler version 5.3.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_OVERDRAWCOUNT_H
#define UI_OVERDRAWCOUNT_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_OverDrawCountClass
{
public:

    void setupUi(QWidget *OverDrawCountClass)
    {
        if (OverDrawCountClass->objectName().isEmpty())
            OverDrawCountClass->setObjectName(QStringLiteral("OverDrawCountClass"));
        OverDrawCountClass->resize(600, 400);

        retranslateUi(OverDrawCountClass);

        QMetaObject::connectSlotsByName(OverDrawCountClass);
    } // setupUi

    void retranslateUi(QWidget *OverDrawCountClass)
    {
        OverDrawCountClass->setWindowTitle(QApplication::translate("OverDrawCountClass", "OverDrawCount", 0));
    } // retranslateUi

};

namespace Ui {
    class OverDrawCountClass: public Ui_OverDrawCountClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_OVERDRAWCOUNT_H
