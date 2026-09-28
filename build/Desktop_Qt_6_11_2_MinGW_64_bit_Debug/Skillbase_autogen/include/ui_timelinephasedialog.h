/********************************************************************************
** Form generated from reading UI file 'timelinephasedialog.ui'
**
** Created by: Qt User Interface Compiler version 6.11.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_TIMELINEPHASEDIALOG_H
#define UI_TIMELINEPHASEDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QAbstractButton>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDateEdit>
#include <QtWidgets/QDialog>
#include <QtWidgets/QDialogButtonBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_TimelinePhaseDialog
{
public:
    QVBoxLayout *dialogLayout;
    QVBoxLayout *nameFieldLayout;
    QLabel *nameLabel;
    QLineEdit *nameEdit;
    QVBoxLayout *descriptionFieldLayout;
    QLabel *descriptionLabel;
    QTextEdit *descriptionEdit;
    QVBoxLayout *periodFieldLayout;
    QLabel *periodLabel;
    QHBoxLayout *periodRowLayout;
    QVBoxLayout *startDateColumnLayout;
    QLabel *startDateLabel;
    QDateEdit *startDateEdit;
    QVBoxLayout *endDateColumnLayout;
    QLabel *endDateLabel;
    QDateEdit *endDateEdit;
    QSpacerItem *dialogSpacer;
    QDialogButtonBox *buttonBox;

    void setupUi(QDialog *TimelinePhaseDialog)
    {
        if (TimelinePhaseDialog->objectName().isEmpty())
            TimelinePhaseDialog->setObjectName("TimelinePhaseDialog");
        TimelinePhaseDialog->resize(440, 420);
        dialogLayout = new QVBoxLayout(TimelinePhaseDialog);
        dialogLayout->setSpacing(12);
        dialogLayout->setObjectName("dialogLayout");
        dialogLayout->setContentsMargins(24, 24, 24, 24);
        nameFieldLayout = new QVBoxLayout();
        nameFieldLayout->setSpacing(8);
        nameFieldLayout->setObjectName("nameFieldLayout");
        nameLabel = new QLabel(TimelinePhaseDialog);
        nameLabel->setObjectName("nameLabel");

        nameFieldLayout->addWidget(nameLabel);

        nameEdit = new QLineEdit(TimelinePhaseDialog);
        nameEdit->setObjectName("nameEdit");

        nameFieldLayout->addWidget(nameEdit);


        dialogLayout->addLayout(nameFieldLayout);

        descriptionFieldLayout = new QVBoxLayout();
        descriptionFieldLayout->setSpacing(8);
        descriptionFieldLayout->setObjectName("descriptionFieldLayout");
        descriptionLabel = new QLabel(TimelinePhaseDialog);
        descriptionLabel->setObjectName("descriptionLabel");

        descriptionFieldLayout->addWidget(descriptionLabel);

        descriptionEdit = new QTextEdit(TimelinePhaseDialog);
        descriptionEdit->setObjectName("descriptionEdit");
        descriptionEdit->setMaximumSize(QSize(16777215, 96));

        descriptionFieldLayout->addWidget(descriptionEdit);


        dialogLayout->addLayout(descriptionFieldLayout);

        periodFieldLayout = new QVBoxLayout();
        periodFieldLayout->setSpacing(8);
        periodFieldLayout->setObjectName("periodFieldLayout");
        periodLabel = new QLabel(TimelinePhaseDialog);
        periodLabel->setObjectName("periodLabel");

        periodFieldLayout->addWidget(periodLabel);

        periodRowLayout = new QHBoxLayout();
        periodRowLayout->setSpacing(16);
        periodRowLayout->setObjectName("periodRowLayout");
        startDateColumnLayout = new QVBoxLayout();
        startDateColumnLayout->setSpacing(8);
        startDateColumnLayout->setObjectName("startDateColumnLayout");
        startDateLabel = new QLabel(TimelinePhaseDialog);
        startDateLabel->setObjectName("startDateLabel");

        startDateColumnLayout->addWidget(startDateLabel);

        startDateEdit = new QDateEdit(TimelinePhaseDialog);
        startDateEdit->setObjectName("startDateEdit");
        startDateEdit->setCalendarPopup(true);

        startDateColumnLayout->addWidget(startDateEdit);


        periodRowLayout->addLayout(startDateColumnLayout);

        endDateColumnLayout = new QVBoxLayout();
        endDateColumnLayout->setSpacing(8);
        endDateColumnLayout->setObjectName("endDateColumnLayout");
        endDateLabel = new QLabel(TimelinePhaseDialog);
        endDateLabel->setObjectName("endDateLabel");

        endDateColumnLayout->addWidget(endDateLabel);

        endDateEdit = new QDateEdit(TimelinePhaseDialog);
        endDateEdit->setObjectName("endDateEdit");
        endDateEdit->setCalendarPopup(true);

        endDateColumnLayout->addWidget(endDateEdit);


        periodRowLayout->addLayout(endDateColumnLayout);


        periodFieldLayout->addLayout(periodRowLayout);


        dialogLayout->addLayout(periodFieldLayout);

        dialogSpacer = new QSpacerItem(20, 10, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding);

        dialogLayout->addItem(dialogSpacer);

        buttonBox = new QDialogButtonBox(TimelinePhaseDialog);
        buttonBox->setObjectName("buttonBox");
        buttonBox->setOrientation(Qt::Orientation::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::StandardButton::Cancel|QDialogButtonBox::StandardButton::Save);

        dialogLayout->addWidget(buttonBox);


        retranslateUi(TimelinePhaseDialog);
        QObject::connect(buttonBox, &QDialogButtonBox::rejected, TimelinePhaseDialog, qOverload<>(&QDialog::reject));

        QMetaObject::connectSlotsByName(TimelinePhaseDialog);
    } // setupUi

    void retranslateUi(QDialog *TimelinePhaseDialog)
    {
        TimelinePhaseDialog->setWindowTitle(QCoreApplication::translate("TimelinePhaseDialog", "Phase hinzuf\303\274gen", nullptr));
        nameLabel->setText(QCoreApplication::translate("TimelinePhaseDialog", "Name", nullptr));
        nameEdit->setPlaceholderText(QCoreApplication::translate("TimelinePhaseDialog", "z. B. Akkorde lernen", nullptr));
        descriptionLabel->setText(QCoreApplication::translate("TimelinePhaseDialog", "Beschreibung (optional)", nullptr));
        descriptionEdit->setPlaceholderText(QCoreApplication::translate("TimelinePhaseDialog", "Was m\303\266chte ich in dieser Phase erreichen?", nullptr));
        periodLabel->setText(QCoreApplication::translate("TimelinePhaseDialog", "Zeitraum", nullptr));
        startDateLabel->setText(QCoreApplication::translate("TimelinePhaseDialog", "Startdatum", nullptr));
        startDateEdit->setDisplayFormat(QCoreApplication::translate("TimelinePhaseDialog", "dd.MM.yyyy", nullptr));
        endDateLabel->setText(QCoreApplication::translate("TimelinePhaseDialog", "Enddatum", nullptr));
        endDateEdit->setDisplayFormat(QCoreApplication::translate("TimelinePhaseDialog", "dd.MM.yyyy", nullptr));
    } // retranslateUi

};

namespace Ui {
    class TimelinePhaseDialog: public Ui_TimelinePhaseDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_TIMELINEPHASEDIALOG_H
