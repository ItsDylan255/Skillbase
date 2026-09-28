/********************************************************************************
** Form generated from reading UI file 'exercise_dialog.ui'
**
** Created by: Qt User Interface Compiler version 6.11.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_EXERCISE_DIALOG_H
#define UI_EXERCISE_DIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QAbstractButton>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QDialogButtonBox>
#include <QtWidgets/QFormLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_ExerciseDialog
{
public:
    QVBoxLayout *verticalLayout;
    QFormLayout *formLayout;
    QLabel *nameLabel;
    QLineEdit *nameLineEdit;
    QLabel *descriptionLabel;
    QLineEdit *descriptionLineEdit;
    QLabel *categoryLabel;
    QComboBox *categoryComboBox;
    QLabel *valueLabel;
    QLineEdit *valueLineEdit;
    QLabel *unitLabel;
    QLineEdit *unitLineEdit;
    QLabel *goalLabel;
    QLineEdit *goalLineEdit;
    QSpacerItem *verticalSpacer;
    QHBoxLayout *bottomButtonLayout;
    QPushButton *archiveButton;
    QPushButton *deleteButton;
    QSpacerItem *bottomButtonSpacer;
    QDialogButtonBox *buttonBox;

    void setupUi(QDialog *ExerciseDialog)
    {
        if (ExerciseDialog->objectName().isEmpty())
            ExerciseDialog->setObjectName("ExerciseDialog");
        ExerciseDialog->resize(400, 340);
        verticalLayout = new QVBoxLayout(ExerciseDialog);
        verticalLayout->setSpacing(12);
        verticalLayout->setObjectName("verticalLayout");
        verticalLayout->setContentsMargins(24, 24, 24, 24);
        formLayout = new QFormLayout();
        formLayout->setObjectName("formLayout");
        formLayout->setHorizontalSpacing(12);
        formLayout->setVerticalSpacing(12);
        nameLabel = new QLabel(ExerciseDialog);
        nameLabel->setObjectName("nameLabel");

        formLayout->setWidget(0, QFormLayout::ItemRole::LabelRole, nameLabel);

        nameLineEdit = new QLineEdit(ExerciseDialog);
        nameLineEdit->setObjectName("nameLineEdit");

        formLayout->setWidget(0, QFormLayout::ItemRole::FieldRole, nameLineEdit);

        descriptionLabel = new QLabel(ExerciseDialog);
        descriptionLabel->setObjectName("descriptionLabel");

        formLayout->setWidget(1, QFormLayout::ItemRole::LabelRole, descriptionLabel);

        descriptionLineEdit = new QLineEdit(ExerciseDialog);
        descriptionLineEdit->setObjectName("descriptionLineEdit");

        formLayout->setWidget(1, QFormLayout::ItemRole::FieldRole, descriptionLineEdit);

        categoryLabel = new QLabel(ExerciseDialog);
        categoryLabel->setObjectName("categoryLabel");

        formLayout->setWidget(2, QFormLayout::ItemRole::LabelRole, categoryLabel);

        categoryComboBox = new QComboBox(ExerciseDialog);
        categoryComboBox->addItem(QString());
        categoryComboBox->setObjectName("categoryComboBox");

        formLayout->setWidget(2, QFormLayout::ItemRole::FieldRole, categoryComboBox);

        valueLabel = new QLabel(ExerciseDialog);
        valueLabel->setObjectName("valueLabel");

        formLayout->setWidget(3, QFormLayout::ItemRole::LabelRole, valueLabel);

        valueLineEdit = new QLineEdit(ExerciseDialog);
        valueLineEdit->setObjectName("valueLineEdit");

        formLayout->setWidget(3, QFormLayout::ItemRole::FieldRole, valueLineEdit);

        unitLabel = new QLabel(ExerciseDialog);
        unitLabel->setObjectName("unitLabel");

        formLayout->setWidget(4, QFormLayout::ItemRole::LabelRole, unitLabel);

        unitLineEdit = new QLineEdit(ExerciseDialog);
        unitLineEdit->setObjectName("unitLineEdit");

        formLayout->setWidget(4, QFormLayout::ItemRole::FieldRole, unitLineEdit);

        goalLabel = new QLabel(ExerciseDialog);
        goalLabel->setObjectName("goalLabel");

        formLayout->setWidget(5, QFormLayout::ItemRole::LabelRole, goalLabel);

        goalLineEdit = new QLineEdit(ExerciseDialog);
        goalLineEdit->setObjectName("goalLineEdit");

        formLayout->setWidget(5, QFormLayout::ItemRole::FieldRole, goalLineEdit);


        verticalLayout->addLayout(formLayout);

        verticalSpacer = new QSpacerItem(20, 10, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding);

        verticalLayout->addItem(verticalSpacer);

        bottomButtonLayout = new QHBoxLayout();
        bottomButtonLayout->setObjectName("bottomButtonLayout");
        archiveButton = new QPushButton(ExerciseDialog);
        archiveButton->setObjectName("archiveButton");

        bottomButtonLayout->addWidget(archiveButton);

        deleteButton = new QPushButton(ExerciseDialog);
        deleteButton->setObjectName("deleteButton");

        bottomButtonLayout->addWidget(deleteButton);

        bottomButtonSpacer = new QSpacerItem(0, 0, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        bottomButtonLayout->addItem(bottomButtonSpacer);

        buttonBox = new QDialogButtonBox(ExerciseDialog);
        buttonBox->setObjectName("buttonBox");
        buttonBox->setOrientation(Qt::Orientation::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::StandardButton::Cancel|QDialogButtonBox::StandardButton::Save);

        bottomButtonLayout->addWidget(buttonBox);


        verticalLayout->addLayout(bottomButtonLayout);

        QWidget::setTabOrder(nameLineEdit, descriptionLineEdit);
        QWidget::setTabOrder(descriptionLineEdit, categoryComboBox);
        QWidget::setTabOrder(categoryComboBox, valueLineEdit);
        QWidget::setTabOrder(valueLineEdit, unitLineEdit);
        QWidget::setTabOrder(unitLineEdit, goalLineEdit);

        retranslateUi(ExerciseDialog);
        QObject::connect(buttonBox, &QDialogButtonBox::accepted, ExerciseDialog, qOverload<>(&QDialog::accept));
        QObject::connect(buttonBox, &QDialogButtonBox::rejected, ExerciseDialog, qOverload<>(&QDialog::reject));

        QMetaObject::connectSlotsByName(ExerciseDialog);
    } // setupUi

    void retranslateUi(QDialog *ExerciseDialog)
    {
        ExerciseDialog->setWindowTitle(QCoreApplication::translate("ExerciseDialog", "Neue \303\234bung", nullptr));
        nameLabel->setText(QCoreApplication::translate("ExerciseDialog", "Name *", nullptr));
        nameLineEdit->setPlaceholderText(QCoreApplication::translate("ExerciseDialog", "z. B. Bankdr\303\274cken", nullptr));
        descriptionLabel->setText(QCoreApplication::translate("ExerciseDialog", "Beschreibung", nullptr));
        descriptionLineEdit->setPlaceholderText(QCoreApplication::translate("ExerciseDialog", "Kurze Beschreibung (optional)", nullptr));
        categoryLabel->setText(QCoreApplication::translate("ExerciseDialog", "Kategorie", nullptr));
        categoryComboBox->setItemText(0, QCoreApplication::translate("ExerciseDialog", "\342\200\223 keine \342\200\223", nullptr));

        valueLabel->setText(QCoreApplication::translate("ExerciseDialog", "Wert", nullptr));
        valueLineEdit->setPlaceholderText(QCoreApplication::translate("ExerciseDialog", "z. B. 60.5 (optional)", nullptr));
        unitLabel->setText(QCoreApplication::translate("ExerciseDialog", "Einheit", nullptr));
        unitLineEdit->setPlaceholderText(QCoreApplication::translate("ExerciseDialog", "z. B. kg, min, km (optional)", nullptr));
        goalLabel->setText(QCoreApplication::translate("ExerciseDialog", "Ziel", nullptr));
        goalLineEdit->setPlaceholderText(QCoreApplication::translate("ExerciseDialog", "z. B. 100 kg erreichen (optional)", nullptr));
        archiveButton->setText(QCoreApplication::translate("ExerciseDialog", "Archivieren", nullptr));
        deleteButton->setText(QCoreApplication::translate("ExerciseDialog", "L\303\266schen", nullptr));
    } // retranslateUi

};

namespace Ui {
    class ExerciseDialog: public Ui_ExerciseDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_EXERCISE_DIALOG_H
