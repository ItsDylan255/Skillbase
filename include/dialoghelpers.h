#pragma once

// Gemeinsame Hilfsfunktionen für Dialoge.
//
// Header-only und ohne Q_OBJECT: Es muss nichts in CMake/qmake eingetragen
// werden.

#include <QAbstractButton>
#include <QBoxLayout>
#include <QDialog>
#include <QDialogButtonBox>
#include <QFormLayout>
#include <QLabel>
#include <QLayout>
#include <QLayoutItem>
#include <QLineEdit>
#include <QObject>
#include <QPushButton>
#include <QStyle>
#include <QVBoxLayout>
#include <QString>
#include <QWidget>

#include <memory>

namespace DialogUi {

namespace detail {

// Sucht das Layout, in dem "widget" direkt liegt (auch in verschachtelten
// Layouts).
inline QLayout *ownerLayout(QLayout *layout, QWidget *widget)
{
    if (!layout)
        return nullptr;

    for (int i = 0; i < layout->count(); ++i) {
        QLayoutItem *item = layout->itemAt(i);

        if (item->widget() == widget)
            return layout;

        if (QLayout *child = item->layout()) {
            if (QLayout *found = ownerLayout(child, widget))
                return found;
        }
    }

    return nullptr;
}

// Fügt "label" direkt unter "field" ein. Gibt false zurück, wenn das Layout
// des Feldes kein Form-/Box-Layout ist (dann bleibt das Label ungenutzt).
inline bool insertBelow(QWidget *field, QWidget *label)
{
    QWidget *host = field->parentWidget();
    QLayout *owner = host ? ownerLayout(host->layout(), field) : nullptr;

    if (auto *form = qobject_cast<QFormLayout *>(owner)) {
        int row = -1;
        QFormLayout::ItemRole role = QFormLayout::FieldRole;
        form->getWidgetPosition(field, &row, &role);

        if (row < 0)
            return false;

        // Einzelnes Widget über beide Spalten, damit keine leere
        // Label-Zelle entsteht.
        form->insertRow(row + 1, label);
        return true;
    }

    if (auto *box = qobject_cast<QBoxLayout *>(owner)) {
        box->insertWidget(box->indexOf(field) + 1, label);
        return true;
    }

    return false;
}

inline void repolish(QWidget *widget)
{
    widget->style()->unpolish(widget);
    widget->style()->polish(widget);
    widget->update();
}

} // namespace detail

// Pflichtfeld "Text darf nicht leer sein".
//
//  - Der Bestätigen-Button ist deaktiviert, solange das Feld leer ist.
//  - Direkt unter dem Feld erscheint eine erklärende Fehlerzeile (Text,
//    nicht nur Farbe), sobald der Nutzer ins Feld getippt hat und es leer
//    ist oder das Feld leer verlässt / Enter drückt.
//  - Das Aussehen kommt aus theme.qss:
//      QLineEdit[invalid="true"]  und  QLabel#fieldErrorLabel
//  - Ein frisch geöffneter, leerer Dialog zeigt noch keinen Fehler.
//
// Alle Verbindungen hängen am Feld; sie verschwinden mit dem Dialog.
inline void requireText(QLineEdit *field,
                        QAbstractButton *acceptButton,
                        const QString &message)
{
    if (!field || !acceptButton)
        return;

    auto *label = new QLabel(message, field->parentWidget());
    label->setObjectName(QStringLiteral("fieldErrorLabel"));
    label->setWordWrap(true);
    label->setVisible(false);

    const bool inserted = detail::insertBelow(field, label);

    if (!inserted) {
        // Kein passendes Layout: Hinweis stattdessen als Tooltip.
        label->deleteLater();
    }

    // true, sobald der Nutzer selbst getippt oder das Feld verlassen hat.
    auto touched = std::make_shared<bool>(false);

    auto refresh = [field, acceptButton, label, inserted, message, touched]() {
        const bool valid = !field->text().trimmed().isEmpty();
        const bool showError = !valid && *touched;

        acceptButton->setEnabled(valid);

        if (field->property("invalid").toBool() != showError) {
            field->setProperty("invalid", showError);
            detail::repolish(field);
        }

        if (inserted)
            label->setVisible(showError);
        else
            field->setToolTip(showError ? message : QString());
    };

    // Der Bestätigen-Button wird meist erst im Code zum Default-Button
    // gemacht, nachdem er schon gestylt wurde. Ohne erneutes Polish würde
    // theme.qss ("QDialogButtonBox QPushButton[default="true"]") nicht greifen.
    if (auto *pushButton = qobject_cast<QPushButton *>(acceptButton)) {
        if (pushButton->isDefault())
            detail::repolish(pushButton);
    }

    QObject::connect(field, &QLineEdit::textEdited, field,
                     [touched]() { *touched = true; });
    QObject::connect(field, &QLineEdit::textChanged, field, refresh);
    QObject::connect(field, &QLineEdit::editingFinished, field,
                     [touched, refresh]() {
                         *touched = true;
                         refresh();
                     });

    refresh();
}

// Einheitlicher Ersatz für QInputDialog::getText():
// Label über Input, Abstände 24/12, Breite 440, Abbrechen + Bestätigen
// rechts. Der Bestätigen-Button bleibt deaktiviert, solange der Text leer
// ist (mit erklärendem Hinweis unter dem Feld, siehe requireText()).
//
// Rückgabe: der getrimmte Text oder ein leerer String bei Abbruch.
// Das passt zum bisherigen Muster "if (!ok || text.isEmpty()) return;".
inline QString getText(QWidget *parent,
                       const QString &title,
                       const QString &label,
                       const QString &confirmText,
                       const QString &initialText = QString(),
                       const QString &emptyMessage =
                           QStringLiteral("Bitte gib einen Namen ein."))
{
    QDialog dialog(parent);
    dialog.setWindowTitle(title);
    dialog.setMinimumWidth(520);
    dialog.setWindowFlag(Qt::WindowContextHelpButtonHint, false);

    auto *layout = new QVBoxLayout(&dialog);
    layout->setContentsMargins(24, 24, 24, 24);
    layout->setSpacing(12);

    // objectName "nameLabel": gleiche Feldlabel-Optik wie in den anderen
    // Dialogen (theme.qss, "QDialog QLabel#nameLabel").
    auto *fieldLabel = new QLabel(label, &dialog);
    fieldLabel->setObjectName(QStringLiteral("nameLabel"));
    layout->addWidget(fieldLabel);

    auto *edit = new QLineEdit(initialText, &dialog);
    edit->setObjectName(QStringLiteral("textInputLineEdit"));
    layout->addWidget(edit);

    layout->addSpacing(12);

    auto *buttons = new QDialogButtonBox(
        QDialogButtonBox::Save | QDialogButtonBox::Cancel, &dialog);

    QPushButton *confirmButton = buttons->button(QDialogButtonBox::Save);
    confirmButton->setText(confirmText);
    confirmButton->setDefault(true);
    buttons->button(QDialogButtonBox::Cancel)->setText(QStringLiteral("Abbrechen"));
    layout->addWidget(buttons);

    QObject::connect(buttons, &QDialogButtonBox::accepted, &dialog, &QDialog::accept);
    QObject::connect(buttons, &QDialogButtonBox::rejected, &dialog, &QDialog::reject);

    requireText(edit, confirmButton, emptyMessage);

    edit->selectAll();
    edit->setFocus();

    if (dialog.exec() != QDialog::Accepted)
        return QString();

    return edit->text().trimmed();
}

} // namespace DialogUi
