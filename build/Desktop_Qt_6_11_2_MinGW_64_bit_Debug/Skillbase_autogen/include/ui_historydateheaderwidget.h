/********************************************************************************
** Form generated from reading UI file 'historydateheaderwidget.ui'
**
** Created by: Qt User Interface Compiler version 6.11.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_HISTORYDATEHEADERWIDGET_H
#define UI_HISTORYDATEHEADERWIDGET_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_HistoryDateHeaderWidget
{
public:
    QHBoxLayout *dateHeaderLayout;
    QLabel *dateLabel;

    void setupUi(QWidget *HistoryDateHeaderWidget)
    {
        if (HistoryDateHeaderWidget->objectName().isEmpty())
            HistoryDateHeaderWidget->setObjectName("HistoryDateHeaderWidget");
        HistoryDateHeaderWidget->resize(420, 32);
        dateHeaderLayout = new QHBoxLayout(HistoryDateHeaderWidget);
        dateHeaderLayout->setSpacing(0);
        dateHeaderLayout->setObjectName("dateHeaderLayout");
        dateHeaderLayout->setContentsMargins(0, 24, 0, 8);
        dateLabel = new QLabel(HistoryDateHeaderWidget);
        dateLabel->setObjectName("dateLabel");
        dateLabel->setAlignment(Qt::AlignmentFlag::AlignLeft|Qt::AlignmentFlag::AlignVCenter);

        dateHeaderLayout->addWidget(dateLabel);


        retranslateUi(HistoryDateHeaderWidget);

        QMetaObject::connectSlotsByName(HistoryDateHeaderWidget);
    } // setupUi

    void retranslateUi(QWidget *HistoryDateHeaderWidget)
    {
        dateLabel->setText(QCoreApplication::translate("HistoryDateHeaderWidget", "HEUTE", nullptr));
        (void)HistoryDateHeaderWidget;
    } // retranslateUi

};

namespace Ui {
    class HistoryDateHeaderWidget: public Ui_HistoryDateHeaderWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_HISTORYDATEHEADERWIDGET_H
