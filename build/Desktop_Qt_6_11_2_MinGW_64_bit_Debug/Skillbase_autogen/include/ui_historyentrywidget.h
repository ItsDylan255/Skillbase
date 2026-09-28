/********************************************************************************
** Form generated from reading UI file 'historyentrywidget.ui'
**
** Created by: Qt User Interface Compiler version 6.11.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_HISTORYENTRYWIDGET_H
#define UI_HISTORYENTRYWIDGET_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QFrame>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_HistoryEntryWidget
{
public:
    QVBoxLayout *entryMainLayout;
    QHBoxLayout *entryLayout;
    QLabel *entryTypeIconLabel;
    QVBoxLayout *entryTextLayout;
    QLabel *entryTitleLabel;
    QLabel *entrySubtitleLabel;
    QSpacerItem *entrySpacer;
    QLabel *entryTimeLabel;
    QFrame *historyEntrySeparator;

    void setupUi(QWidget *HistoryEntryWidget)
    {
        if (HistoryEntryWidget->objectName().isEmpty())
            HistoryEntryWidget->setObjectName("HistoryEntryWidget");
        HistoryEntryWidget->resize(420, 60);
        HistoryEntryWidget->setCursor(QCursor(Qt::CursorShape::PointingHandCursor));
        entryMainLayout = new QVBoxLayout(HistoryEntryWidget);
        entryMainLayout->setSpacing(0);
        entryMainLayout->setObjectName("entryMainLayout");
        entryMainLayout->setContentsMargins(0, 4, 0, 0);
        entryLayout = new QHBoxLayout();
        entryLayout->setSpacing(12);
        entryLayout->setObjectName("entryLayout");
        entryLayout->setContentsMargins(12, 4, 12, 8);
        entryTypeIconLabel = new QLabel(HistoryEntryWidget);
        entryTypeIconLabel->setObjectName("entryTypeIconLabel");
        entryTypeIconLabel->setMinimumSize(QSize(8, 8));
        entryTypeIconLabel->setMaximumSize(QSize(8, 8));
        entryTypeIconLabel->setAlignment(Qt::AlignmentFlag::AlignCenter);

        entryLayout->addWidget(entryTypeIconLabel);

        entryTextLayout = new QVBoxLayout();
        entryTextLayout->setSpacing(4);
        entryTextLayout->setObjectName("entryTextLayout");
        entryTitleLabel = new QLabel(HistoryEntryWidget);
        entryTitleLabel->setObjectName("entryTitleLabel");

        entryTextLayout->addWidget(entryTitleLabel);

        entrySubtitleLabel = new QLabel(HistoryEntryWidget);
        entrySubtitleLabel->setObjectName("entrySubtitleLabel");

        entryTextLayout->addWidget(entrySubtitleLabel);


        entryLayout->addLayout(entryTextLayout);

        entrySpacer = new QSpacerItem(0, 0, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        entryLayout->addItem(entrySpacer);

        entryTimeLabel = new QLabel(HistoryEntryWidget);
        entryTimeLabel->setObjectName("entryTimeLabel");
        entryTimeLabel->setAlignment(Qt::AlignmentFlag::AlignRight|Qt::AlignmentFlag::AlignTrailing|Qt::AlignmentFlag::AlignVCenter);

        entryLayout->addWidget(entryTimeLabel);


        entryMainLayout->addLayout(entryLayout);

        historyEntrySeparator = new QFrame(HistoryEntryWidget);
        historyEntrySeparator->setObjectName("historyEntrySeparator");
        historyEntrySeparator->setMinimumSize(QSize(0, 1));
        historyEntrySeparator->setMaximumSize(QSize(16777215, 1));
        historyEntrySeparator->setFrameShape(QFrame::Shape::HLine);
        historyEntrySeparator->setFrameShadow(QFrame::Shadow::Plain);

        entryMainLayout->addWidget(historyEntrySeparator);


        retranslateUi(HistoryEntryWidget);

        QMetaObject::connectSlotsByName(HistoryEntryWidget);
    } // setupUi

    void retranslateUi(QWidget *HistoryEntryWidget)
    {
        entryTypeIconLabel->setText(QString());
        entryTypeIconLabel->setProperty("role", QVariant(QCoreApplication::translate("HistoryEntryWidget", "accent", nullptr)));
        entryTitleLabel->setText(QCoreApplication::translate("HistoryEntryWidget", "Bankdr\303\274cken", nullptr));
        entrySubtitleLabel->setText(QCoreApplication::translate("HistoryEntryWidget", "80 kg \302\267 20 min", nullptr));
        entrySubtitleLabel->setProperty("role", QVariant(QCoreApplication::translate("HistoryEntryWidget", "secondary", nullptr)));
        entryTimeLabel->setText(QCoreApplication::translate("HistoryEntryWidget", "18:42", nullptr));
        entryTimeLabel->setProperty("role", QVariant(QCoreApplication::translate("HistoryEntryWidget", "muted", nullptr)));
        (void)HistoryEntryWidget;
    } // retranslateUi

};

namespace Ui {
    class HistoryEntryWidget: public Ui_HistoryEntryWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_HISTORYENTRYWIDGET_H
