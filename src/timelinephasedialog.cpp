#include "timelinephasedialog.h"
#include "ui_timelinephasedialog.h"
#include <QMessageBox>

#include <QPushButton>

TimelinePhaseDialog::TimelinePhaseDialog(QWidget *parent)
    : QDialog(parent),
    ui(new Ui::TimelinePhaseDialog)
{
    ui->setupUi(this);

    // Die Standard-Buttontexte von Qt werden an die deutsche UI angepasst.
    ui->buttonBox->button(QDialogButtonBox::Cancel)->setText("Abbrechen");
    ui->buttonBox->button(QDialogButtonBox::Save)->setText("Speichern");
}

TimelinePhaseDialog::~TimelinePhaseDialog()
{
    delete ui;

}
QString TimelinePhaseDialog::name() const
{
    return ui->nameEdit->text().trimmed();
}

QString TimelinePhaseDialog::description() const
{
    return ui->descriptionEdit->toPlainText().trimmed();
}

QDate TimelinePhaseDialog::startDate() const
{
    return ui->startDateEdit->date();
}

QDate TimelinePhaseDialog::endDate() const
{
    return ui->endDateEdit->date();
}

void TimelinePhaseDialog::setDateRange(
    const QDate &startDate,
    const QDate &endDate
    )
{
    // Die übergebenen Werte werden als Vorschlag
    // in die beiden Datumsfelder des Dialogs übernommen.
    ui->startDateEdit->setDate(startDate);
    ui->endDateEdit->setDate(endDate);
}

void TimelinePhaseDialog::setName(const QString &name)
{
    // Beim Bearbeiten wird der bestehende Name
    // wieder in das Eingabefeld übernommen.
    ui->nameEdit->setText(name);
}

void TimelinePhaseDialog::setDescription(const QString &description)
{
    // Die bestehende Beschreibung wird ebenfalls
    // für die Bearbeitung vorausgefüllt.
    ui->descriptionEdit->setPlainText(description);
}


bool TimelinePhaseDialog::validateInput()
{
    // Der Name ist das einzige Pflichtfeld für die Bezeichnung der Phase.
    if (name().isEmpty()) {

        QMessageBox::warning(
            this,
            "Ungültige Eingabe",
            "Bitte gib einen Namen für die Phase ein."
            );

        ui->nameEdit->setFocus();
        return false;
    }

    // Der Zeitraum muss chronologisch korrekt sein.
    if (endDate() < startDate()) {

        QMessageBox::warning(
            this,
            "Ungültiger Zeitraum",
            "Das Enddatum darf nicht vor dem Startdatum liegen."
            );

        ui->endDateEdit->setFocus();
        return false;
    }

    return true;
}

void TimelinePhaseDialog::accept()
{
    // accept() wird weiterhin verwendet, um den Dialog
    // nach einem erfolgreichen Speichern tatsächlich zu schließen.
    QDialog::accept();
}

QPushButton *TimelinePhaseDialog::saveButton() const
{
    // Gibt den Speichern-Button zurück,
    // damit MainWindow seinen Klick selbst behandeln kann.
    return ui->buttonBox->button(QDialogButtonBox::Save);
}