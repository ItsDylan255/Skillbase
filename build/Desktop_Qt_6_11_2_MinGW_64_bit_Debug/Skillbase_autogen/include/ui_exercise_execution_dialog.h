/********************************************************************************
** Form generated from reading UI file 'exercise_execution_dialog.ui'
**
** Created by: Qt User Interface Compiler version 6.11.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_EXERCISE_EXECUTION_DIALOG_H
#define UI_EXERCISE_EXECUTION_DIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QAbstractButton>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QDialogButtonBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_ExerciseExecutionDialog
{
public:
    QVBoxLayout *verticalLayout;
    QLabel *titleLabel;
    QHBoxLayout *exerciseLayout;
    QLabel *exerciseLabel;
    QLabel *exerciseNameLabel;
    QHBoxLayout *goalLayout;
    QLabel *goalTitleLabel;
    QLabel *goalLabel;
    QHBoxLayout *valueLayout;
    QLabel *valueTitleLabel;
    QLineEdit *valueLineEdit;
    QHBoxLayout *durationLayout;
    QLabel *durationTitleLabel;
    QLineEdit *durationLineEdit;
    QSpacerItem *verticalSpacer;
    QHBoxLayout *bottomButtonLayout;
    QSpacerItem *bottomButtonSpacer;
    QDialogButtonBox *buttonBox;

    void setupUi(QDialog *ExerciseExecutionDialog)
    {
        if (ExerciseExecutionDialog->objectName().isEmpty())
            ExerciseExecutionDialog->setObjectName("ExerciseExecutionDialog");
        ExerciseExecutionDialog->resize(360, 320);
        verticalLayout = new QVBoxLayout(ExerciseExecutionDialog);
        verticalLayout->setSpacing(12);
        verticalLayout->setObjectName("verticalLayout");
        verticalLayout->setContentsMargins(24, 24, 24, 24);
        titleLabel = new QLabel(ExerciseExecutionDialog);
        titleLabel->setObjectName("titleLabel");

        verticalLayout->addWidget(titleLabel);

        exerciseLayout = new QHBoxLayout();
        exerciseLayout->setSpacing(8);
        exerciseLayout->setObjectName("exerciseLayout");
        exerciseLabel = new QLabel(ExerciseExecutionDialog);
        exerciseLabel->setObjectName("exerciseLabel");

        exerciseLayout->addWidget(exerciseLabel);

        exerciseNameLabel = new QLabel(ExerciseExecutionDialog);
        exerciseNameLabel->setObjectName("exerciseNameLabel");
        exerciseNameLabel->setWordWrap(true);

        exerciseLayout->addWidget(exerciseNameLabel);


        verticalLayout->addLayout(exerciseLayout);

        goalLayout = new QHBoxLayout();
        goalLayout->setSpacing(8);
        goalLayout->setObjectName("goalLayout");
        goalTitleLabel = new QLabel(ExerciseExecutionDialog);
        goalTitleLabel->setObjectName("goalTitleLabel");

        goalLayout->addWidget(goalTitleLabel);

        goalLabel = new QLabel(ExerciseExecutionDialog);
        goalLabel->setObjectName("goalLabel");
        goalLabel->setWordWrap(true);

        goalLayout->addWidget(goalLabel);


        verticalLayout->addLayout(goalLayout);

        valueLayout = new QHBoxLayout();
        valueLayout->setSpacing(8);
        valueLayout->setObjectName("valueLayout");
        valueTitleLabel = new QLabel(ExerciseExecutionDialog);
        valueTitleLabel->setObjectName("valueTitleLabel");

        valueLayout->addWidget(valueTitleLabel);

        valueLineEdit = new QLineEdit(ExerciseExecutionDialog);
        valueLineEdit->setObjectName("valueLineEdit");

        valueLayout->addWidget(valueLineEdit);


        verticalLayout->addLayout(valueLayout);

        durationLayout = new QHBoxLayout();
        durationLayout->setSpacing(8);
        durationLayout->setObjectName("durationLayout");
        durationTitleLabel = new QLabel(ExerciseExecutionDialog);
        durationTitleLabel->setObjectName("durationTitleLabel");

        durationLayout->addWidget(durationTitleLabel);

        durationLineEdit = new QLineEdit(ExerciseExecutionDialog);
        durationLineEdit->setObjectName("durationLineEdit");

        durationLayout->addWidget(durationLineEdit);


        verticalLayout->addLayout(durationLayout);

        verticalSpacer = new QSpacerItem(20, 10, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding);

        verticalLayout->addItem(verticalSpacer);

        bottomButtonLayout = new QHBoxLayout();
        bottomButtonLayout->setObjectName("bottomButtonLayout");
        bottomButtonSpacer = new QSpacerItem(0, 0, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        bottomButtonLayout->addItem(bottomButtonSpacer);

        buttonBox = new QDialogButtonBox(ExerciseExecutionDialog);
        buttonBox->setObjectName("buttonBox");
        buttonBox->setOrientation(Qt::Orientation::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::StandardButton::Cancel|QDialogButtonBox::StandardButton::Save);

        bottomButtonLayout->addWidget(buttonBox);


        verticalLayout->addLayout(bottomButtonLayout);


        retranslateUi(ExerciseExecutionDialog);
        QObject::connect(buttonBox, &QDialogButtonBox::accepted, ExerciseExecutionDialog, qOverload<>(&QDialog::accept));
        QObject::connect(buttonBox, &QDialogButtonBox::rejected, ExerciseExecutionDialog, qOverload<>(&QDialog::reject));

        QMetaObject::connectSlotsByName(ExerciseExecutionDialog);
    } // setupUi

    void retranslateUi(QDialog *ExerciseExecutionDialog)
    {
        ExerciseExecutionDialog->setWindowTitle(QCoreApplication::translate("ExerciseExecutionDialog", "\303\234bung ausf\303\274hren", nullptr));
        titleLabel->setText(QCoreApplication::translate("ExerciseExecutionDialog", "\303\234bung ausf\303\274hren", nullptr));
        exerciseLabel->setText(QCoreApplication::translate("ExerciseExecutionDialog", "\303\234bung:", nullptr));
        exerciseNameLabel->setText(QString());
        goalTitleLabel->setText(QCoreApplication::translate("ExerciseExecutionDialog", "Ziel:", nullptr));
        goalLabel->setText(QString());
        valueTitleLabel->setText(QCoreApplication::translate("ExerciseExecutionDialog", "Wert:", nullptr));
        valueLineEdit->setPlaceholderText(QCoreApplication::translate("ExerciseExecutionDialog", "Wert", nullptr));
        durationTitleLabel->setText(QCoreApplication::translate("ExerciseExecutionDialog", "Dauer:", nullptr));
        durationLineEdit->setPlaceholderText(QCoreApplication::translate("ExerciseExecutionDialog", "MM:SS", nullptr));
    } // retranslateUi

};

namespace Ui {
    class ExerciseExecutionDialog: public Ui_ExerciseExecutionDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_EXERCISE_EXECUTION_DIALOG_H
