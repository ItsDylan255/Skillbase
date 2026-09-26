#include "exerciseexecutiondialog.h"
#include "ui_exercise_execution_dialog.h"
#include "exerciselogrepository.h"

#include <QDoubleValidator>
#include <QRegularExpression>
#include <QMessageBox>

ExerciseExecutionDialog::ExerciseExecutionDialog(
    const Exercise &exercise,
    QWidget *parent
    )
    : QDialog(parent),
    ui(new Ui::ExerciseExecutionDialog)
{
    ui->setupUi(this);

    // Die Dauer wird von rechts nach links aufgebaut.
    // Der Nutzer gibt nur die Ziffern ein, der Doppelpunkt wird automatisch gesetzt.
    connect(
        ui->durationLineEdit,
        &QLineEdit::textChanged,
        this,
        [this](const QString &text) {

            QString digits = text;

            // Der Doppelpunkt gehört nur zur Darstellung und wird
            // deshalb entfernt, bevor wir die eigentliche Eingabe verarbeiten.
            digits.remove(':');

            // Es sind maximal vier Ziffern erlaubt: MMSS.
            if (digits.length() > 4)
                digits = digits.right(4);

            // Nur Ziffern behalten.
            digits.remove(
                QRegularExpression("[^0-9]")
                );

            if (digits.isEmpty()) {
                ui->durationLineEdit->setText("");
                return;
            }

            // Von rechts auffüllen:
            // 5    -> 00:05
            // 30   -> 00:30
            // 530  -> 05:30
            // 1230 -> 12:30
            digits = digits.rightJustified(
                4,
                '0'
                );

            const QString formatted =
                digits.left(2)
                + ":"
                + digits.right(2);

            if (formatted != text) {
                ui->durationLineEdit->setText(formatted);
                ui->durationLineEdit->setCursorPosition(
                    formatted.length()
                    );
            }
        }
        );

    // Diese Werte kommen aus der Übung und werden nur angezeigt.
    ui->exerciseNameLabel->setText(exercise.name);

    // Wenn bereits eine Ausführung existiert,
    // wird deren letzter Wert als Startwert vorgeschlagen.
    ExerciseLog latestLog;

    if (ExerciseLogRepository::getLatestForExercise(
            exercise.id,
            latestLog
            )) {

        ui->valueLineEdit->setText(
            QString::number(latestLog.value, 'g', 15)
            );

    } else if (exercise.value > 0) {

        // Wenn noch keine Ausführung existiert,
        // verwenden wir den ursprünglichen Wert der Übung.
        ui->valueLineEdit->setText(
            QString::number(exercise.value, 'g', 15)
            );
    }

    // Ziel und Einheit werden gemeinsam angezeigt,
    // da die Einheit zur Übung gehört und nicht separat eingegeben wird.
    ui->goalLabel->setText(
        QString::number(exercise.goal, 'g', 15) + " " + exercise.unit
        );

    // Der Wert darf nur als positive Zahl eingegeben werden.
    auto *valueValidator = new QDoubleValidator(
        0.0,
        999999.0,
        2,
        this
        );

    valueValidator->setNotation(
        QDoubleValidator::StandardNotation
        );

    ui->valueLineEdit->setValidator(valueValidator);

    connect(
        ui->buttonBox,
        &QDialogButtonBox::accepted,
        this,
        [this, exercise]() {

            // Der Wert wird aus dem Eingabefeld gelesen.
            // Durch den Validator kann hier nur eine gültige Zahl stehen.
            bool valueOk = false;

            const double value =
                ui->valueLineEdit->text().toDouble(&valueOk);

            if (!valueOk) {
                QMessageBox::warning(
                    this,
                    "Ungültiger Wert",
                    "Bitte gib einen gültigen Wert ein."
                    );

                return;
            }

            // Die Dauer wird in der Datenbank als Sekunden gespeichert.
            // Deshalb zerlegen wir die Anzeige "MM:SS" in Minuten und Sekunden.
            const QString durationText =
                ui->durationLineEdit->text();

            const QStringList durationParts =
                durationText.split(':');

            int durationSeconds = 0;

            if (durationParts.size() == 2) {

                const int minutes =
                    durationParts[0].toInt();

                const int seconds =
                    durationParts[1].toInt();

                durationSeconds =
                    minutes * 60 + seconds;
            }

            int logId = 0;

            if (!ExerciseLogRepository::add(
                    exercise.id,
                    value,
                    exercise.unit,
                    durationSeconds,
                    logId
                    )) {

                QMessageBox::warning(
                    this,
                    "Fehler",
                    "Die Ausführung konnte nicht gespeichert werden."
                    );

                return;
            }

            // Erst wenn das Speichern erfolgreich war,
            // schließen wir den Dialog mit Accepted.
            accept();
        }
        );

}

ExerciseExecutionDialog::~ExerciseExecutionDialog()
{
    delete ui;
}