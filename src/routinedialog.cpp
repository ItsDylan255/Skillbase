#include "routinedialog.h"
#include "ui_routine_dialog.h"
#include "ui_selected_exercise_row.h"

#include "exerciserepository.h"
#include "categoryrepository.h"
#include "routinerepository.h"

#include <QAbstractButton>
#include <QAbstractItemView>
#include <QCompleter>
#include <QDebug>
#include <QEvent>
#include <QFocusEvent>
#include <QFrame>
#include <QLabel>
#include <QLineEdit>
#include <QMessageBox>
#include <QSpinBox>
#include <QStringListModel>
#include <QStyle>
#include <QTimer>

RoutineDialog::RoutineDialog(
    int hobbyId,
    QWidget *parent
    )
    : QDialog(parent),
    ui(new Ui::routineDialog),
    hobbyId(hobbyId)
{
    ui->setupUi(this);

    // Der Completer übernimmt die Textsuche. Das Suchfeld bleibt ein
    // normales QLineEdit und behält deshalb immer den Fokus. Das Popup
    // verändert weder Text noch Auswahl im Suchfeld.
    exerciseNamesModel = new QStringListModel(this);

    exerciseCompleter = new QCompleter(exerciseNamesModel, this);
    exerciseCompleter->setCaseSensitivity(Qt::CaseInsensitive);
    exerciseCompleter->setFilterMode(Qt::MatchContains);
    exerciseCompleter->setCompletionMode(QCompleter::PopupCompletion);

    ui->routineExerciseSearchLineEdit->setCompleter(exerciseCompleter);

    // Der Event-Filter zeigt beim Klick ins leere Feld alle Übungen
    // (siehe eventFilter()).
    ui->routineExerciseSearchLineEdit->installEventFilter(this);

    // Kategorien müssen vor den Übungen geladen werden,
    // weil die Namensliste auf die ausgewählte Kategorie zugreift.
    loadCategories();
    loadAvailableExercises();

    // Ein Kategorienwechsel baut nur die Namensliste des Completers neu auf.
    // Der Suchtext im Feld bleibt dabei unberührt.
    connect(
        ui->routineCategoryComboBox,
        &QComboBox::currentIndexChanged,
        this,
        [this](int) {
            updateAvailableExercisesList();
        }
        );

    // Hinzugefügt wird erst über den Button. Ein Klick auf einen Eintrag
    // im Popup schreibt nur den Namen ins Suchfeld.
    connect(
        ui->routineAddExerciseButton,
        &QAbstractButton::clicked,
        this,
        [this]() {
            const QString typedName =
                ui->routineExerciseSearchLineEdit->text().trimmed();

            // Ohne Text passiert bewusst nichts.
            if (typedName.isEmpty())
                return;

            const int selectedCategoryId =
                ui->routineCategoryComboBox->currentData().toInt();

            // Der Name im Feld muss genau zu einer Übung der gewählten
            // Kategorie passen. Sonst passiert ebenfalls nichts.
            int exerciseId = 0;

            for (const Exercise &exercise : availableExercises) {

                if (selectedCategoryId != 0 &&
                    exercise.categoryId != selectedCategoryId)
                    continue;

                if (exercise.name.compare(
                        typedName,
                        Qt::CaseInsensitive
                        ) == 0) {
                    exerciseId = exercise.id;
                    break;
                }
            }

            if (exerciseId <= 0)
                return;

            ui->routineExerciseSearchLineEdit->clear();
            addSelectedExercise(exerciseId);
        }
        );

    // Speichern wird zentral über saveRoutine() behandelt.
    connect(
        ui->routineSaveButton,
        &QPushButton::clicked,
        this,
        [this]() {
            saveRoutine();
        }
        );

    // Abbrechen schließt den Dialog ohne zu speichern.
    connect(
        ui->routineCancelButton,
        &QPushButton::clicked,
        this,
        &QDialog::reject
        );
}

RoutineDialog::~RoutineDialog()
{
    delete ui;
}

bool RoutineDialog::eventFilter(QObject *watched, QEvent *event)
{
    if (watched == ui->routineExerciseSearchLineEdit) {

        bool shouldOpen = false;

        if (event->type() == QEvent::MouseButtonPress) {
            shouldOpen = true;
        }
        else if (event->type() == QEvent::FocusIn) {
            // Nur bei Tab-Navigation öffnen. Andere Gründe (z. B. Rückkehr
            // vom geschlossenen Popup) würden das Popup sofort wieder öffnen.
            const Qt::FocusReason reason =
                static_cast<QFocusEvent *>(event)->reason();

            shouldOpen =
                reason == Qt::TabFocusReason ||
                reason == Qt::BacktabFocusReason;
        }

        if (shouldOpen) {
            // Verzögert, damit das Popup nicht mit dem Klick-Event kollidiert.
            QTimer::singleShot(0, this, [this]() {

                if (exerciseCompleter->popup()->isVisible())
                    return;

                // Ein leerer Präfix zeigt alle Übungen der Kategorie,
                // sonst die Treffer zum bereits getippten Text.
                exerciseCompleter->setCompletionPrefix(
                    ui->routineExerciseSearchLineEdit->text()
                    );
                exerciseCompleter->complete();
            });
        }
    }

    return QDialog::eventFilter(watched, event);
}

void RoutineDialog::loadAvailableExercises()
{
    // Wir laden die Übungen einmal in den Speicher.
    // Dadurch arbeiten Kategorie-Filter, Suche und Hinzufügen
    // auf denselben Daten.
    availableExercises =
        ExerciseRepository::getForHobby(hobbyId);

    updateAvailableExercisesList();
}

void RoutineDialog::updateAvailableExercisesList()
{
    const int selectedCategoryId =
        ui->routineCategoryComboBox->currentData().toInt();

    // Nur die Kategorie bestimmt, welche Namen der Completer kennt.
    // Der Suchtext wird vom Completer selbst gefiltert.
    QStringList names;

    for (const Exercise &exercise : availableExercises) {

        if (selectedCategoryId != 0 &&
            exercise.categoryId != selectedCategoryId)
            continue;

        names.append(exercise.name);
    }

    exerciseNamesModel->setStringList(names);
}

void RoutineDialog::loadCategories()
{
    ui->routineCategoryComboBox->clear();

    // ID 0 verwenden wir als Sonderwert für "Alle Kategorien".
    // Dadurch kann der Filter später einfach prüfen, ob eine
    // bestimmte Kategorie ausgewählt wurde.
    ui->routineCategoryComboBox->addItem(
        "Alle Kategorien",
        0
        );

    const QList<Category> categories =
        CategoryRepository::getForHobby(hobbyId);

    for (const Category &category : categories) {
        ui->routineCategoryComboBox->addItem(
            category.name,
            category.id
            );
    }
}

void RoutineDialog::addSelectedExercise(int exerciseId)
{
    // Dieselbe Übung darf mehrfach hinzugefügt werden. Jede Zeile
    // speichert ihre eigene Übungs-ID, deshalb gibt es keine Prüfung.
    // Die eigentliche Row wird in einer eigenen Methode aufgebaut,
    // damit die Darstellung der ausgewählten Übung getrennt bleibt.
    createSelectedExerciseRow(exerciseId);

    // Sobald mindestens eine Übung vorhanden ist,
    // zeigen wir die Liste statt des Empty-State an.
    ui->selectedExercisesStack->setCurrentWidget(
        ui->selectedExercisesList
        );
}

void RoutineDialog::createSelectedExerciseRow(int exerciseId)
{
    for (const Exercise &exercise : availableExercises) {

        if (exercise.id != exerciseId)
            continue;

        // Die Row-UI basiert auf einem QFrame.
        // Deshalb muss auch das tatsächliche Widget ein QFrame sein.
        QFrame *row = new QFrame();

        Ui_selectedExerciseRow rowUi;
        rowUi.setupUi(row);

        rowUi.selectedExerciseNameLabel->setText(
            exercise.name
            );

        // Jede Übung startet mit der voreingestellten Dauer von 5 Minuten.
        // Der Nutzer kann die Dauer anschließend pro Übung ändern.
        rowUi.selectedExerciseDurationSpinBox->setValue(5);

        // Die Übungs-ID wird an der Zeile gespeichert,
        // damit wir sie später beim Speichern wiederfinden.
        selectedExerciseIds.insert(row, exercise.id);

        // QAbstractButton::clicked funktioniert für QToolButton
        // und QPushButton gleichermaßen.
        connect(
            rowUi.selectedExerciseRemoveButton,
            &QAbstractButton::clicked,
            this,
            [this, row]() {
                removeSelectedExerciseRow(row);
            }
            );

        connect(
            rowUi.selectedExerciseMoveUpButton,
            &QAbstractButton::clicked,
            this,
            [this, row]() {
                moveSelectedExerciseRow(row, -1);
            }
            );

        connect(
            rowUi.selectedExerciseMoveDownButton,
            &QAbstractButton::clicked,
            this,
            [this, row]() {
                moveSelectedExerciseRow(row, +1);
            }
            );

        QListWidgetItem *item =
            new QListWidgetItem();

        // Die Höhe des Listeneintrags richtet sich nach der
        // tatsächlichen Höhe unserer Row-UI.
        item->setSizeHint(row->sizeHint());

        ui->selectedExercisesList->addItem(item);
        ui->selectedExercisesList->setItemWidget(item, row);

        break;
    }
}

void RoutineDialog::removeSelectedExerciseRow(QWidget *row)
{
    // Wir suchen den Listeneintrag, zu dem diese sichtbare Zeile gehört.
    for (int i = 0; i < ui->selectedExercisesList->count(); ++i) {

        QListWidgetItem *item =
            ui->selectedExercisesList->item(i);

        if (ui->selectedExercisesList->itemWidget(item) != row)
            continue;

        selectedExerciseIds.remove(row);
        delete ui->selectedExercisesList->takeItem(i);

        // Nicht direkt löschen: Diese Methode läuft im clicked-Signal
        // eines Buttons, der selbst zu 'row' gehört.
        row->deleteLater();

        break;
    }

    // Wenn keine Übungen mehr ausgewählt sind,
    // zeigen wir wieder den Empty-State.
    if (ui->selectedExercisesList->count() == 0) {
        ui->selectedExercisesStack->setCurrentWidget(
            ui->selectedExercisesEmptyPage
            );
    }
}

void RoutineDialog::moveSelectedExerciseRow(QWidget *row, int delta)
{
    int from = -1;

    for (int i = 0; i < ui->selectedExercisesList->count(); ++i) {
        if (ui->selectedExercisesList->itemWidget(
                ui->selectedExercisesList->item(i)) == row) {
            from = i;
            break;
        }
    }

    const int to = from + delta;

    if (from < 0 || to < 0 || to >= ui->selectedExercisesList->count())
        return;

    QWidget *other = ui->selectedExercisesList->itemWidget(
        ui->selectedExercisesList->item(to));

    if (!other)
        return;

    // Statt Items zu verschieben (das würde die Item-Widgets zerstören),
    // tauschen wir nur die Inhalte der beiden Zeilen.
    auto *labelA = row->findChild<QLabel*>("selectedExerciseNameLabel");
    auto *labelB = other->findChild<QLabel*>("selectedExerciseNameLabel");
    auto *spinA = row->findChild<QSpinBox*>("selectedExerciseDurationSpinBox");
    auto *spinB = other->findChild<QSpinBox*>("selectedExerciseDurationSpinBox");

    if (!labelA || !labelB || !spinA || !spinB)
        return;

    const QString textA = labelA->text();
    labelA->setText(labelB->text());
    labelB->setText(textA);

    const int valueA = spinA->value();
    spinA->setValue(spinB->value());
    spinB->setValue(valueA);

    const int idA = selectedExerciseIds.value(row);
    selectedExerciseIds[row] = selectedExerciseIds.value(other);
    selectedExerciseIds[other] = idA;

    ui->selectedExercisesList->setCurrentRow(to);
}

void RoutineDialog::updateSelectedExerciseIds()
{
    // Die Zuordnung wird anhand der aktuell sichtbaren Zeilen
    // neu aufgebaut. Dadurch bleibt sie auch nach einer späteren
    // Änderung der Reihenfolge konsistent.
    QMap<QWidget*, int> updatedIds;

    for (int i = 0; i < ui->selectedExercisesList->count(); ++i) {

        QListWidgetItem *item =
            ui->selectedExercisesList->item(i);

        QWidget *row =
            ui->selectedExercisesList->itemWidget(item);

        if (!row)
            continue;

        const int exerciseId =
            selectedExerciseIds.value(row);

        updatedIds.insert(row, exerciseId);
    }

    selectedExerciseIds = updatedIds;
}

void RoutineDialog::saveRoutine()
{
    const QString name =
        ui->routineNameLineEdit->text().trimmed();

    // Eine Routine ohne Namen wäre später schwer zu unterscheiden.
    if (name.isEmpty()) {
        QMessageBox::warning(
            this,
            "Routine speichern",
            "Bitte gib einen Namen für die Routine ein."
            );

        ui->routineNameLineEdit->setFocus();
        return;
    }

    // Eine Routine ohne Übung kann momentan nicht sinnvoll ausgeführt
    // werden. Deshalb verlangen wir mindestens eine ausgewählte Übung.
    if (ui->selectedExercisesList->count() == 0) {
        QMessageBox::warning(
            this,
            "Routine speichern",
            "Bitte füge mindestens eine Übung zur Routine hinzu."
            );

        return;
    }

    const QString description =
        ui->routineDescriptionTextEdit->toPlainText().trimmed();

    int routineId = 0;

    // Zuerst wird der Kopf der Routine gespeichert.
    if (!RoutineRepository::add(
            hobbyId,
            name,
            description,
            routineId
            )) {

        QMessageBox::critical(
            this,
            "Routine speichern",
            "Die Routine konnte nicht gespeichert werden."
            );

        return;
    }

    // Danach speichern wir jeden ausgewählten Eintrag einzeln.
    // Die Position entspricht der aktuellen Reihenfolge in der Liste.
    for (int position = 0;
         position < ui->selectedExercisesList->count();
         ++position) {

        QListWidgetItem *item =
            ui->selectedExercisesList->item(position);

        QWidget *row =
            ui->selectedExercisesList->itemWidget(item);

        if (!row)
            continue;

        const int exerciseId =
            selectedExerciseIds.value(row);

        // Die Row-UI wird über ihre Kinder gesucht.
        // So können wir die vom Nutzer eingestellte Dauer auslesen,
        // ohne die UI-Struktur zusätzlich im Model speichern zu müssen.
        QSpinBox *durationSpinBox =
            row->findChild<QSpinBox*>(
                "selectedExerciseDurationSpinBox"
                );

        const int durationMinutes =
            durationSpinBox
                ? durationSpinBox->value()
                : 5;

        const int durationSeconds =
            durationMinutes * 60;

        int stepId = 0;

        if (!RoutineRepository::addStep(
                routineId,
                exerciseId,
                position,
                durationSeconds,
                stepId
                )) {

            // Die Routine wurde bereits angelegt. Wir brechen hier ab,
            // damit keine weiteren Schritte mit einer unvollständigen
            // Routine gespeichert werden.
            QMessageBox::critical(
                this,
                "Routine speichern",
                "Die Routine wurde angelegt, aber ein Routine-Schritt "
                "konnte nicht gespeichert werden."
                );

            return;
        }
    }

    // Alles wurde erfolgreich gespeichert.
    accept();
}
