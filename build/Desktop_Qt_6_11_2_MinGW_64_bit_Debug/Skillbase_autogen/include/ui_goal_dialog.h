/********************************************************************************
** Form generated from reading UI file 'goal_dialog.ui'
**
** Created by: Qt User Interface Compiler version 6.11.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_GOAL_DIALOG_H
#define UI_GOAL_DIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QAbstractButton>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QDateEdit>
#include <QtWidgets/QDialog>
#include <QtWidgets/QDialogButtonBox>
#include <QtWidgets/QFormLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPlainTextEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_GoalDialog
{
public:
    QVBoxLayout *verticalLayout;
    QFormLayout *formLayout;
    QLabel *titleLabel;
    QLineEdit *titleLineEdit;
    QLabel *descriptionLabel;
    QPlainTextEdit *descriptionTextEdit;
    QLabel *deadlineLabel;
    QHBoxLayout *deadlineRow;
    QCheckBox *hasDeadlineCheckBox;
    QDateEdit *deadlineDateEdit;
    QSpacerItem *deadlineRowSpacer;
    QSpacerItem *verticalSpacer;
    QHBoxLayout *bottomButtonLayout;
    QPushButton *statusButton;
    QPushButton *deleteButton;
    QSpacerItem *bottomButtonSpacer;
    QDialogButtonBox *buttonBox;

    void setupUi(QDialog *GoalDialog)
    {
        if (GoalDialog->objectName().isEmpty())
            GoalDialog->setObjectName("GoalDialog");
        GoalDialog->resize(400, 320);
        verticalLayout = new QVBoxLayout(GoalDialog);
        verticalLayout->setSpacing(12);
        verticalLayout->setObjectName("verticalLayout");
        verticalLayout->setContentsMargins(24, 24, 24, 24);
        formLayout = new QFormLayout();
        formLayout->setObjectName("formLayout");
        formLayout->setHorizontalSpacing(12);
        formLayout->setVerticalSpacing(12);
        titleLabel = new QLabel(GoalDialog);
        titleLabel->setObjectName("titleLabel");

        formLayout->setWidget(0, QFormLayout::ItemRole::LabelRole, titleLabel);

        titleLineEdit = new QLineEdit(GoalDialog);
        titleLineEdit->setObjectName("titleLineEdit");

        formLayout->setWidget(0, QFormLayout::ItemRole::FieldRole, titleLineEdit);

        descriptionLabel = new QLabel(GoalDialog);
        descriptionLabel->setObjectName("descriptionLabel");

        formLayout->setWidget(1, QFormLayout::ItemRole::LabelRole, descriptionLabel);

        descriptionTextEdit = new QPlainTextEdit(GoalDialog);
        descriptionTextEdit->setObjectName("descriptionTextEdit");
        descriptionTextEdit->setMinimumSize(QSize(0, 64));

        formLayout->setWidget(1, QFormLayout::ItemRole::FieldRole, descriptionTextEdit);

        deadlineLabel = new QLabel(GoalDialog);
        deadlineLabel->setObjectName("deadlineLabel");

        formLayout->setWidget(2, QFormLayout::ItemRole::LabelRole, deadlineLabel);

        deadlineRow = new QHBoxLayout();
        deadlineRow->setObjectName("deadlineRow");
        hasDeadlineCheckBox = new QCheckBox(GoalDialog);
        hasDeadlineCheckBox->setObjectName("hasDeadlineCheckBox");

        deadlineRow->addWidget(hasDeadlineCheckBox);

        deadlineDateEdit = new QDateEdit(GoalDialog);
        deadlineDateEdit->setObjectName("deadlineDateEdit");
        deadlineDateEdit->setEnabled(false);
        deadlineDateEdit->setCalendarPopup(true);

        deadlineRow->addWidget(deadlineDateEdit);

        deadlineRowSpacer = new QSpacerItem(0, 0, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        deadlineRow->addItem(deadlineRowSpacer);


        formLayout->setLayout(2, QFormLayout::ItemRole::FieldRole, deadlineRow);


        verticalLayout->addLayout(formLayout);

        verticalSpacer = new QSpacerItem(20, 10, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding);

        verticalLayout->addItem(verticalSpacer);

        bottomButtonLayout = new QHBoxLayout();
        bottomButtonLayout->setObjectName("bottomButtonLayout");
        statusButton = new QPushButton(GoalDialog);
        statusButton->setObjectName("statusButton");

        bottomButtonLayout->addWidget(statusButton);

        deleteButton = new QPushButton(GoalDialog);
        deleteButton->setObjectName("deleteButton");

        bottomButtonLayout->addWidget(deleteButton);

        bottomButtonSpacer = new QSpacerItem(0, 0, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        bottomButtonLayout->addItem(bottomButtonSpacer);

        buttonBox = new QDialogButtonBox(GoalDialog);
        buttonBox->setObjectName("buttonBox");
        buttonBox->setOrientation(Qt::Orientation::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::StandardButton::Cancel|QDialogButtonBox::StandardButton::Save);

        bottomButtonLayout->addWidget(buttonBox);


        verticalLayout->addLayout(bottomButtonLayout);

        QWidget::setTabOrder(titleLineEdit, descriptionTextEdit);
        QWidget::setTabOrder(descriptionTextEdit, hasDeadlineCheckBox);
        QWidget::setTabOrder(hasDeadlineCheckBox, deadlineDateEdit);

        retranslateUi(GoalDialog);
        QObject::connect(buttonBox, &QDialogButtonBox::accepted, GoalDialog, qOverload<>(&QDialog::accept));
        QObject::connect(buttonBox, &QDialogButtonBox::rejected, GoalDialog, qOverload<>(&QDialog::reject));
        QObject::connect(hasDeadlineCheckBox, &QCheckBox::toggled, deadlineDateEdit, &QDateEdit::setEnabled);

        QMetaObject::connectSlotsByName(GoalDialog);
    } // setupUi

    void retranslateUi(QDialog *GoalDialog)
    {
        GoalDialog->setWindowTitle(QCoreApplication::translate("GoalDialog", "Neues Ziel", nullptr));
        titleLabel->setText(QCoreApplication::translate("GoalDialog", "Titel *", nullptr));
        titleLineEdit->setPlaceholderText(QCoreApplication::translate("GoalDialog", "z. B. Spanisch B2", nullptr));
        descriptionLabel->setText(QCoreApplication::translate("GoalDialog", "Beschreibung *", nullptr));
        descriptionTextEdit->setPlaceholderText(QCoreApplication::translate("GoalDialog", "Worum geht es bei diesem Ziel?", nullptr));
        deadlineLabel->setText(QCoreApplication::translate("GoalDialog", "Deadline", nullptr));
        hasDeadlineCheckBox->setText(QString());
        deadlineDateEdit->setDisplayFormat(QCoreApplication::translate("GoalDialog", "dd.MM.yyyy", nullptr));
        statusButton->setText(QCoreApplication::translate("GoalDialog", "Als geschafft markieren", nullptr));
        deleteButton->setText(QCoreApplication::translate("GoalDialog", "L\303\266schen", nullptr));
    } // retranslateUi

};

namespace Ui {
    class GoalDialog: public Ui_GoalDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_GOAL_DIALOG_H
