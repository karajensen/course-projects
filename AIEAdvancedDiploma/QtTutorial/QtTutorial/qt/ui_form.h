/********************************************************************************
** Form generated from reading UI file 'form.ui'
**
** Created by: Qt User Interface Compiler version 5.3.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_FORM_H
#define UI_FORM_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDoubleSpinBox>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Form
{
public:
    QPushButton *exitApplication;
    QPushButton *testButton;
    QDoubleSpinBox *testSpinBox;

    void setupUi(QWidget *Form)
    {
        if (Form->objectName().isEmpty())
            Form->setObjectName(QStringLiteral("Form"));
        Form->resize(329, 312);
        exitApplication = new QPushButton(Form);
        exitApplication->setObjectName(QStringLiteral("exitApplication"));
        exitApplication->setGeometry(QRect(100, 70, 121, 28));
        testButton = new QPushButton(Form);
        testButton->setObjectName(QStringLiteral("testButton"));
        testButton->setGeometry(QRect(110, 150, 93, 28));
        testSpinBox = new QDoubleSpinBox(Form);
        testSpinBox->setObjectName(QStringLiteral("testSpinBox"));
        testSpinBox->setGeometry(QRect(120, 230, 62, 22));

        retranslateUi(Form);

        QMetaObject::connectSlotsByName(Form);
    } // setupUi

    void retranslateUi(QWidget *Form)
    {
        Form->setWindowTitle(QApplication::translate("Form", "Form", 0));
        exitApplication->setText(QApplication::translate("Form", "Exit Application", 0));
        testButton->setText(QApplication::translate("Form", "Test Button", 0));
    } // retranslateUi

};

namespace Ui {
    class Form: public Ui_Form {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_FORM_H
