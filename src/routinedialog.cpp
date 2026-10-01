#include "routinedialog.h"
#include "ui_routine_dialog.h"
#include "ui_selected_exercise_row.h"
#include "routinestep.h"

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
#include <QKeyEvent>
#include <QLabel>
#include <QLineEdit>
#include <QMessageBox>
#include <QSpinBox>
#include <QStringListModel>
#include <QStyle>
#include <QTimer>
#include <QToolButton>

namespace {

// Gibt den Namen der Übung zurück, die genau (ohne Beachtung der Groß-/
// Kleinschreibung) zum Text passt und in der gewählten Kategorie liegt.
// Leer, wenn es keine solche Übung gibt.
template <typename ExerciseList>
QString findExerciseName(
    const Ui::routineDialog *ui,
    const ExerciseList &exercises,
    const QString &text
    )
{
    const QString typed = text.trimmed();

    if (typed.isEmpty())
        return QString();

    const int categoryId =
        ui->routineCategoryComboBox->currentData().toInt();

    for (const auto &exercise : exercises) {

        if (categoryId != 0 && exercise.categoryId != categoryId)
            continue;

        if (exercise.name.compare(typed, Qt::CaseInsensitive) == 0)
            return exercise.name;
    }

    return QString();
}

// Legt das Tag links im Suchfeld an die richtige Stelle. Rechts bleibt
// Platz für den Clear-Button (✕) des Felds.
void positionExerciseTag(QLineEdit *edit)
{
    QLabel *tag =
        edit->findChild<QLabel *>("routineSelectedChipLabel");

    if (!tag)
        return;

    const int tagHeight = 24;
    const int leftInset = 5;
    const int rightReserved = 30;

    tag->ensurePolished();

    const int maxWidth =
        qMax(0, edit->width() - leftInset - rightReserved);
    const int width =
        qMin(tag->sizeHint().width(), maxWidth);

    tag->setGeometry(
        leftInset,
        (edit->height() - tagHeight) / 2,
        width,
        tagHeight
        );
}

// Zeigt oder versteckt das Tag über dem Suchfeld.
void setExerciseTagVisible(
    Ui::routineDialog *ui,
    bool visible,
    const QString &text = QString()
    )
{
    QLineEdit *edit = ui->routineExerciseSearchLineEdit;

    QLabel *tag =
        edit->findChild<QLabel *>("routineSelectedChipLabel");

    if (!tag) {
        tag = new QLabel(edit);
        tag->setObjectName("routineSelectedChipLabel");
        // Klicks gehen an das Suchfeld durch (Cursor setzen, editieren).
        tag->setAttribute(Qt::WA_TransparentForMouseEvents);
        tag->hide();
    }

    // Die Property blendet über theme.qss den Text im Feld aus.
    if (edit->property("tagged").toBool() != visible) {
        edit->setProperty("tagged", visible);
        edit->style()->unpolish(edit);
        edit->style()->polish(edit);
    }

    if (visible) {
        tag->setText(text);
        positionExerciseTag(edit);
        tag->show();
        tag->raise();
    }
    else {
        tag->hide();
    }
}

} // namespace


RoutineDialog::RoutineDialog(
    int hobbyId,
    QWidget *parent
    )
    : RoutineDialog(hobbyId, 0, parent)
{
}

RoutineDialog::RoutineDialog(
    int hobbyId,
    int routineId,
    QWidget *parent
    )
    : QDialog(parent),
    ui(new Ui::routineDialog),
    hobbyId(hobbyId),
    routineId(routineId)
{

    ui->setupUi(this);



    if (routineId > 0) {
        setWindowTitle("Routine bearbeiten");
        ui->routineDeleteButton->setVisible(true);
        ui->routineArchiveButton->setVisible(true);

        // Der Button zeigt immer die Aktion an, die als Nächstes
        // mit der aktuell bearbeiteten Routine ausgeführt werden kann.
        Routine routine;

        if (RoutineRepository::getById(routineId, routine)) {
            ui->routineArchiveButton->setText(
                routine.archived
                    ? "Wiederherstellen"
                    : "Archivieren"
                );
        }
    }
    else {
        // Beim Erstellen werden Löschen und Archivieren nicht benötigt.
        setWindowTitle("Routine erstellen");
        ui->routineDeleteButton->setVisible(false);
        ui->routineArchiveButton->setVisible(false);
    }

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

    // Wird im Popup eine Übung gewählt (Klick oder Enter), erscheint das
    // Tag. Der Fokus wandert danach zu "Hinzufügen"; das Tag bleibt sichtbar,
    // weil das Suchfeld den Fokus verliert (siehe eventFilter()).
    connect(
        exerciseCompleter,
        QOverload<const QString &>::of(&QCompleter::activated),
        this,
        [this](const QString &exerciseName) {
            showSelectedExerciseChip(exerciseName);

            QTimer::singleShot(0, this, [this]() {
                ui->routineAddExerciseButton->setFocus();
            });
        }
        );

    // "Hinzufügen" ist aktiv, sobald der Text genau zu einer Übung passt.
    // Jede Textänderung (Tippen, Löschen, Clear-Button) entfernt das Tag.
    connect(
        ui->routineExerciseSearchLineEdit,
        &QLineEdit::textChanged,
        this,
        [this](const QString &text) {
            ui->routineAddExerciseButton->setEnabled(
                !findExerciseName(ui, availableExercises, text).isEmpty()
                );

            if (ui->routineExerciseSearchLineEdit
                    ->property("tagged").toBool()) {
                setExerciseTagVisible(ui, false);
            }
        }
        );

    // Kategorien müssen vor den Übungen geladen werden,
    // weil die Namensliste auf die ausgewählte Kategorie zugreift.
    loadCategories();
    loadAvailableExercises();

    // Nur beim Bearbeiten gibt es eine bestehende Routine,
    // deren Daten aus der Datenbank geladen werden müssen.
    // Beim Erstellen bleibt routineId = 0 und der Dialog startet leer.
    if (routineId > 0) {
        loadRoutineForEditing();
    }


    // Ein Kategorienwechsel baut nur die Namensliste des Completers neu auf.
    // Der Suchtext im Feld bleibt dabei unberührt.
    connect(
        ui->routineCategoryComboBox,
        &QComboBox::currentIndexChanged,
        this,
        [this](int) {
            updateAvailableExercisesList();

            // Der Text im Feld passt evtl. nicht mehr zur neuen Kategorie.
            const QString name = findExerciseName(
                ui,
                availableExercises,
                ui->routineExerciseSearchLineEdit->text()
                );

            ui->routineAddExerciseButton->setEnabled(!name.isEmpty());

            if (name.isEmpty()) {
                setExerciseTagVisible(ui, false);
            }
            else if (!ui->routineExerciseSearchLineEdit->hasFocus()) {
                setExerciseTagVisible(ui, true, name);
            }
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

            resetExerciseInput();
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
    connect(
        ui->routineDeleteButton,
        &QPushButton::clicked,
        this,
        [this, routineId]() {

            const QMessageBox::StandardButton answer =
                QMessageBox::question(
                    this,
                    "Routine löschen",
                    "Möchtest du diese Routine wirklich löschen?",
                    QMessageBox::Yes | QMessageBox::No,
                    QMessageBox::No
                    );

            // Bei "Nein" bleibt der Dialog geöffnet.
            if (answer != QMessageBox::Yes)
                return;

            if (!RoutineRepository::remove(routineId)) {
                QMessageBox::critical(
                    this,
                    "Routine löschen",
                    "Die Routine konnte nicht gelöscht werden."
                    );

                return;
            }

            // Die Routine wurde erfolgreich gelöscht.
            // Der aufrufende MainWindow-Code kann dadurch die
            // Routinenliste anschließend neu laden.
            accept();
        }
        );

    connect(
        ui->routineArchiveButton,
        &QPushButton::clicked,
        this,
        [this, routineId]() {

            Routine routine;

            if (!RoutineRepository::getById(
                    routineId,
                    routine
                    )) {
                QMessageBox::critical(
                    this,
                    "Routine",
                    "Die Routine konnte nicht geladen werden."
                    );

                return;
            }

            // Der aktuelle Archivstatus bestimmt,
            // ob die Routine archiviert oder wiederhergestellt wird.
            const bool newArchivedState =
                !routine.archived;

            const QString title =
                newArchivedState
                    ? "Routine archivieren"
                    : "Routine wiederherstellen";

            const QString message =
                newArchivedState
                    ? "Möchtest du diese Routine wirklich archivieren?"
                    : "Möchtest du diese Routine wirklich wiederherstellen?";

            const QMessageBox::StandardButton answer =
                QMessageBox::question(
                    this,
                    title,
                    message,
                    QMessageBox::Yes | QMessageBox::No,
                    QMessageBox::No
                    );

            // Bei "Nein" bleibt der Dialog geöffnet.
            if (answer != QMessageBox::Yes)
                return;

            if (!RoutineRepository::setArchived(
                    routineId,
                    newArchivedState
                    )) {

                QMessageBox::critical(
                    this,
                    title,
                    newArchivedState
                        ? "Die Routine konnte nicht archiviert werden."
                        : "Die Routine konnte nicht wiederhergestellt werden."
                    );

                return;
            }

            // Die Änderung wurde gespeichert.
            // MainWindow lädt danach die Routinen neu.
            accept();
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

        // Das Tag folgt der Größe des Suchfelds.
        if (event->type() == QEvent::Resize) {
            positionExerciseTag(ui->routineExerciseSearchLineEdit);
        }

        // Enter bei einem vollständig und richtig geschriebenen Namen
        // (Popup geschlossen): Tag anzeigen und den Enter-Druck verbrauchen,
        // damit nicht der Standard-Button "Speichern" ausgelöst wird.
        if (event->type() == QEvent::KeyPress) {

            const int key = static_cast<QKeyEvent *>(event)->key();

            if ((key == Qt::Key_Return || key == Qt::Key_Enter) &&
                !exerciseCompleter->popup()->isVisible()) {

                const QString name = findExerciseName(
                    ui,
                    availableExercises,
                    ui->routineExerciseSearchLineEdit->text()
                    );

                if (!name.isEmpty()) {
                    showSelectedExerciseChip(name);
                    ui->routineAddExerciseButton->setFocus();
                    return true;
                }
            }
        }

        // Verlässt man das Suchfeld (Klick woanders, Tab), wird ein
        // vollständig und richtig geschriebener Name zum Tag. Das Öffnen
        // des Popups und das Wechseln des Fensters zählen nicht.
        if (event->type() == QEvent::FocusOut) {

            const Qt::FocusReason reason =
                static_cast<QFocusEvent *>(event)->reason();

            if (reason != Qt::PopupFocusReason &&
                reason != Qt::ActiveWindowFocusReason) {

                const QString name = findExerciseName(
                    ui,
                    availableExercises,
                    ui->routineExerciseSearchLineEdit->text()
                    );

                if (!name.isEmpty())
                    setExerciseTagVisible(ui, true, name);
            }
        }

        bool shouldOpen = false;

        if (event->type() == QEvent::MouseButtonPress) {
            shouldOpen = true;
        }
        else if (event->type() == QEvent::FocusIn) {

            const Qt::FocusReason reason =
                static_cast<QFocusEvent *>(event)->reason();

            // Zum Bearbeiten wird der normale Text wieder sichtbar.
            if (reason != Qt::PopupFocusReason &&
                reason != Qt::ActiveWindowFocusReason) {
                setExerciseTagVisible(ui, false);
            }

            // Nur bei Tab-Navigation öffnen. Andere Gründe (z. B. Rückkehr
            // vom geschlossenen Popup) würden das Popup sofort wieder öffnen.
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

void RoutineDialog::showSelectedExerciseChip(const QString &exerciseName)
{
    // Der Text im Suchfeld bleibt erhalten (unsichtbar unter dem Tag),
    // damit der "Hinzufügen"-Handler den Namen daraus lesen kann.
    setExerciseTagVisible(ui, true, exerciseName);
    ui->routineAddExerciseButton->setEnabled(true);
}

void RoutineDialog::resetExerciseInput()
{
    // Zurück zur leeren Suche: Tag weg, Text leeren, "Hinzufügen" sperren.
    setExerciseTagVisible(ui, false);
    ui->routineExerciseSearchLineEdit->clear();
    ui->routineAddExerciseButton->setEnabled(false);
    ui->routineExerciseSearchLineEdit->setFocus();
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

    updateMoveButtonVisibility();
}

void RoutineDialog::createSelectedExerciseRow(
    int exerciseId,
    int durationMinutes
    )
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
        rowUi.selectedExerciseDurationSpinBox->setValue(durationMinutes);        // Die Übungs-ID wird an der Zeile gespeichert,
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
        // Ausgeblendete Pfeile behalten ihren Platz, damit die Zeile
        // beim Ausblenden nicht seitlich springt.
        for (QToolButton *arrow : {
                 rowUi.selectedExerciseMoveUpButton,
                 rowUi.selectedExerciseMoveDownButton
             }) {
            if (!arrow)
                continue;

            QSizePolicy policy = arrow->sizePolicy();
            policy.setRetainSizeWhenHidden(true);
            arrow->setSizePolicy(policy);
        }

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

    updateMoveButtonVisibility();
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

    updateMoveButtonVisibility();
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

void RoutineDialog::loadRoutineForEditing()
{
    Routine routine;

    // Zuerst laden wir die Grunddaten der bestehenden Routine.
    if (!RoutineRepository::getById(routineId, routine)) {
        QMessageBox::critical(
            this,
            "Routine bearbeiten",
            "Die Routine konnte nicht geladen werden."
            );

        reject();
        return;
    }

    ui->routineNameLineEdit->setText(
        routine.name
        );

    ui->routineDescriptionTextEdit->setPlainText(
        routine.description
        );

    // Die gespeicherten Übungen werden in ihrer gespeicherten
    // Reihenfolge geladen.
    const QList<RoutineStep> steps =
        RoutineRepository::getSteps(routineId);

    for (const RoutineStep &step : steps) {

        // Die Datenbank speichert die Dauer in Sekunden,
        // das Dialogfeld arbeitet dagegen mit Minuten.
        const int durationMinutes =
            qMax(1, step.durationSeconds / 60);

        createSelectedExerciseRow(
            step.exerciseId,
            durationMinutes
            );
    }

    // Nach dem Laden muss die Liste sichtbar sein.
    // Die leere Ansicht darf nur angezeigt werden,
    // wenn tatsächlich keine Übungen vorhanden sind.
    if (!steps.isEmpty()) {

        ui->selectedExercisesStack->setCurrentWidget(
            ui->selectedExercisesList
            );

    } else {

        ui->selectedExercisesStack->setCurrentWidget(
            ui->selectedExercisesEmptyPage
            );
    }

    updateMoveButtonVisibility();
}

void RoutineDialog::updateMoveButtonVisibility()
{
    const int count = ui->selectedExercisesList->count();

    for (int i = 0; i < count; ++i) {

        QListWidgetItem *item =
            ui->selectedExercisesList->item(i);

        QWidget *row =
            ui->selectedExercisesList->itemWidget(item);

        if (!row)
            continue;

        auto *upButton =
            row->findChild<QToolButton *>(
                "selectedExerciseMoveUpButton"
                );

        auto *downButton =
            row->findChild<QToolButton *>(
                "selectedExerciseMoveDownButton"
                );

        if (upButton) {
            // Bei der ersten Zeile gibt es nichts nach oben.
            upButton->setVisible(i > 0);
        }

        if (downButton) {
            // Bei der letzten Zeile gibt es nichts nach unten.
            downButton->setVisible(i < count - 1);
        }
    }
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

    // Beim Erstellen gibt es noch keine Routine-ID.
    // Beim Bearbeiten ist routineId bereits die ID der bestehenden Routine.
    if (routineId == 0) {

        // Neue Routine anlegen.
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
    }
    else {

        // Bestehende Routine aktualisieren.
        if (!RoutineRepository::update(
                routineId,
                name,
                description
                )) {

            QMessageBox::critical(
                this,
                "Routine speichern",
                "Die Routine konnte nicht aktualisiert werden."
                );

            return;
        }

        // Die alten Schritte werden anschließend durch
        // die aktuelle Reihenfolge aus dem Dialog ersetzt.
        if (!RoutineRepository::removeSteps(routineId)) {

            QMessageBox::critical(
                this,
                "Routine speichern",
                "Die alten Routine-Schritte konnten nicht aktualisiert werden."
                );

            return;
        }
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
