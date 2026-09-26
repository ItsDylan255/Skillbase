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

void TimelinePhaseDialog::accept()
{
    // Der Name ist das einzige Pflichtfeld für die Bezeichnung der Phase.
    if (name().isEmpty()) {

        QMessageBox::warning(
            this,
            "Ungültige Eingabe",
            "Bitte gib einen Namen für die Phase ein."
            );

        ui->nameEdit->setFocus();
        return;
    }

    // Der Zeitraum muss chronologisch korrekt sein.
    if (endDate() < startDate()) {

        QMessageBox::warning(
            this,
            "Ungültiger Zeitraum",
            "Das Enddatum darf nicht vor dem Startdatum liegen."
            );

        ui->endDateEdit->setFocus();
        return;
    }

    // Alle Eingaben sind gültig.
    // QDialog::accept() schließt den Dialog mit dem Ergebnis Accepted.
    QDialog::accept();
}