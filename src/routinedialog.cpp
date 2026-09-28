#include "routinedialog.h"
#include "ui_routine_dialog.h"
#include "ui_selected_exercise_row.h"

#include "exerciserepository.h"
#include "categoryrepository.h"
#include "routinerepository.h"

#include <QAbstractItemModel>
#include <QDebug>
#include <QFrame>
#include <QMessageBox>

RoutineDialog::RoutineDialog(
    int hobbyId,
    QWidget *parent
    )
    : QDialog(parent),
    ui(new Ui::routineDialog),
    hobbyId(hobbyId)
{
    ui->setupUi(this);

    // Drag & Drop ist für später vorbereitet, wird aber momentan
    // nicht weiter behandelt. Die Liste bleibt trotzdem für
    // spätere Erweiterung als interne Drag-Liste konfiguriert.
    ui->selectedExercisesList->setDragEnabled(true);
    ui->selectedExercisesList->setAcceptDrops(true);
    ui->selectedExercisesList->setDropIndicatorShown(true);
    ui->selectedExercisesList->setDragDropMode(
        QAbstractItemView::InternalMove
        );
    ui->selectedExercisesList->setDefaultDropAction(
        Qt::MoveAction
        );

    loadAvailableExercises();
    loadCategories();

    // Ein Klick auf eine verfügbare Übung fügt sie zur Routine hinzu.
    connect(
        ui->availableExercisesList,
        &QListWidget::itemClicked,
        this,
        [this](QListWidgetItem *item) {
            const int exerciseId =
                item->data(Qt::UserRole).toInt();

            addSelectedExercise(exerciseId);
        }
        );

    // Die Suche filtert die verfügbaren Übungen sofort.
    connect(
        ui->routineSearchLineEdit,
        &QLineEdit::textChanged,
        this,
        [this]() {
            updateAvailableExercisesList();
        }
        );

    // Auch ein Kategorienwechsel aktualisiert die Übungsliste.
    connect(
        ui->routineCategoryComboBox,
        &QComboBox::currentIndexChanged,
        this,
        [this]() {
            updateAvailableExercisesList();
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

    // Abbrechen schließt den Dialog ohne Änderungen zu speichern.
    connect(
        ui->routineCancelButton,
        &QPushButton::clicked,
        this,
        &QDialog::reject
        );

    // Die Reihenfolge der Listeneinträge kann später über Drag & Drop
    // verändert werden. Die eigentliche Verarbeitung der neuen
    // Reihenfolge erfolgt beim Speichern anhand der aktuellen Liste.
    connect(
        ui->selectedExercisesList->model(),
        &QAbstractItemModel::rowsMoved,
        this,
        [this]() {
            updateSelectedExerciseIds();
        }
        );
}

RoutineDialog::~RoutineDialog()
{
    delete ui;
}

void RoutineDialog::loadAvailableExercises()
{
    // Wir laden die Übungen einmal in den Speicher.
    // Dadurch können Suche, Kategorie-Filter und Auswahl
    // auf denselben Daten arbeiten.
    availableExercises =
        ExerciseRepository::getForHobby(hobbyId);

    updateAvailableExercisesList();
}

void RoutineDialog::updateAvailableExercisesList()
{
    ui->availableExercisesList->clear();

    const QString searchText =
        ui->routineSearchLineEdit->text()
            .trimmed()
            .toLower();

    const int selectedCategoryId =
        ui->routineCategoryComboBox->currentData().toInt();

    for (const Exercise &exercise : availableExercises) {

        // ID 0 steht für "Alle Kategorien".
        const bool matchesCategory =
            selectedCategoryId == 0 ||
            exercise.categoryId == selectedCategoryId;

        if (!matchesCategory)
            continue;

        const bool matchesSearch =
            searchText.isEmpty() ||
            exercise.name.toLower().contains(searchText);

        if (!matchesSearch)
            continue;

        QListWidgetItem *item =
            new QListWidgetItem(exercise.name);

        // Die ID wird unsichtbar am Listeneintrag gespeichert.
        // Beim Anklicken können wir dadurch die genaue Übung
        // wiederfinden.
        item->setData(Qt::UserRole, exercise.id);

        ui->availableExercisesList->addItem(item);
    }
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

        // Der Entfernen-Button gehört zu genau dieser Zeile.
        connect(
            rowUi.selectedExerciseRemoveButton,
            &QPushButton::clicked,
            this,
            [this, row]() {
                removeSelectedExerciseRow(row);
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

        // Erst den Listeneintrag entfernen.
        delete ui->selectedExercisesList->takeItem(i);

        // Danach unsere Zuordnung zwischen Zeile und Übungs-ID entfernen.
        selectedExerciseIds.remove(row);

        // Das Widget selbst wird nicht mehr benötigt.
        delete row;

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