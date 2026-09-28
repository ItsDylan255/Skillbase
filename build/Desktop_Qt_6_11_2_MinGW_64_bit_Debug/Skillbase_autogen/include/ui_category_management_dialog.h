/********************************************************************************
** Form generated from reading UI file 'category_management_dialog.ui'
**
** Created by: Qt User Interface Compiler version 6.11.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CATEGORY_MANAGEMENT_DIALOG_H
#define UI_CATEGORY_MANAGEMENT_DIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QFrame>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_CategoryManagementDialog
{
public:
    QVBoxLayout *verticalLayout;
    QLabel *descriptionLabel;
    QListWidget *categoryListWidget;
    QHBoxLayout *actionButtonLayout;
    QPushButton *addButton;
    QPushButton *renameButton;
    QPushButton *deleteButton;
    QSpacerItem *actionSpacer;
    QFrame *separator;
    QHBoxLayout *closeButtonLayout;
    QSpacerItem *closeSpacer;
    QPushButton *closeButton;

    void setupUi(QDialog *CategoryManagementDialog)
    {
        if (CategoryManagementDialog->objectName().isEmpty())
            CategoryManagementDialog->setObjectName("CategoryManagementDialog");
        CategoryManagementDialog->resize(360, 300);
        verticalLayout = new QVBoxLayout(CategoryManagementDialog);
        verticalLayout->setSpacing(12);
        verticalLayout->setObjectName("verticalLayout");
        verticalLayout->setContentsMargins(24, 24, 24, 24);
        descriptionLabel = new QLabel(CategoryManagementDialog);
        descriptionLabel->setObjectName("descriptionLabel");

        verticalLayout->addWidget(descriptionLabel);

        categoryListWidget = new QListWidget(CategoryManagementDialog);
        categoryListWidget->setObjectName("categoryListWidget");

        verticalLayout->addWidget(categoryListWidget);

        actionButtonLayout = new QHBoxLayout();
        actionButtonLayout->setObjectName("actionButtonLayout");
        addButton = new QPushButton(CategoryManagementDialog);
        addButton->setObjectName("addButton");

        actionButtonLayout->addWidget(addButton);

        renameButton = new QPushButton(CategoryManagementDialog);
        renameButton->setObjectName("renameButton");
        renameButton->setEnabled(false);

        actionButtonLayout->addWidget(renameButton);

        deleteButton = new QPushButton(CategoryManagementDialog);
        deleteButton->setObjectName("deleteButton");
        deleteButton->setEnabled(false);

        actionButtonLayout->addWidget(deleteButton);

        actionSpacer = new QSpacerItem(0, 0, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        actionButtonLayout->addItem(actionSpacer);


        verticalLayout->addLayout(actionButtonLayout);

        separator = new QFrame(CategoryManagementDialog);
        separator->setObjectName("separator");
        separator->setFrameShape(QFrame::Shape::HLine);

        verticalLayout->addWidget(separator);

        closeButtonLayout = new QHBoxLayout();
        closeButtonLayout->setObjectName("closeButtonLayout");
        closeSpacer = new QSpacerItem(0, 0, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        closeButtonLayout->addItem(closeSpacer);

        closeButton = new QPushButton(CategoryManagementDialog);
        closeButton->setObjectName("closeButton");

        closeButtonLayout->addWidget(closeButton);


        verticalLayout->addLayout(closeButtonLayout);

        QWidget::setTabOrder(categoryListWidget, addButton);
        QWidget::setTabOrder(addButton, renameButton);
        QWidget::setTabOrder(renameButton, deleteButton);
        QWidget::setTabOrder(deleteButton, closeButton);

        retranslateUi(CategoryManagementDialog);
        QObject::connect(closeButton, &QPushButton::clicked, CategoryManagementDialog, qOverload<>(&QDialog::accept));

        QMetaObject::connectSlotsByName(CategoryManagementDialog);
    } // setupUi

    void retranslateUi(QDialog *CategoryManagementDialog)
    {
        CategoryManagementDialog->setWindowTitle(QCoreApplication::translate("CategoryManagementDialog", "Kategorien verwalten", nullptr));
        descriptionLabel->setText(QCoreApplication::translate("CategoryManagementDialog", "Kategorien f\303\274r dieses Hobby:", nullptr));
        addButton->setText(QCoreApplication::translate("CategoryManagementDialog", "Hinzuf\303\274gen", nullptr));
        renameButton->setText(QCoreApplication::translate("CategoryManagementDialog", "Umbenennen", nullptr));
        deleteButton->setText(QCoreApplication::translate("CategoryManagementDialog", "L\303\266schen", nullptr));
        closeButton->setText(QCoreApplication::translate("CategoryManagementDialog", "Schlie\303\237en", nullptr));
    } // retranslateUi

};

namespace Ui {
    class CategoryManagementDialog: public Ui_CategoryManagementDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CATEGORY_MANAGEMENT_DIALOG_H
