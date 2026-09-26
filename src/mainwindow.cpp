#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QInputDialog>
#include <QMessageBox>
#include "hobbyrepository.h"
#include "exerciserepository.h"
#include <QDebug>
#include "hobbynoterepository.h"
#include <QSignalBlocker>
#include "ui_exercise_dialog.h"
#include "ui_category_management_dialog.h"
#include "categoryrepository.h"
#include <QGroupBox>
#include <QFrame>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include "exerciselogrepository.h"
#include <QDateTime>
#include "exerciseexecutiondialog.h"
#include "historyentrywidget.h"
#include "historydateheaderwidget.h"
#include <algorithm>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(
        ui->historyFilterAllButton,
        &QPushButton::clicked,
        this,
        [this]() {
            historyFilter = HistoryFilter::All;
            loadHistory();
        }
        );

    connect(
        ui->historyFilterExercisesButton,
        &QPushButton::clicked,
        this,
        [this]() {
            historyFilter = HistoryFilter::Exercises;
            loadHistory();
        }
        );

    connect(
        ui->historyFilterRoutinesButton,
        &QPushButton::clicked,
        this,
        [this]() {
            historyFilter = HistoryFilter::Routines;
            loadHistory();
        }
        );
    connect(
        ui->historySearchLineEdit,
        &QLineEdit::textChanged,
        this,
        [this](const QString &) {
            loadHistory();
        }
        );

    for (const Hobby &hobby : HobbyRepository::getAll()) {
        QListWidgetItem *item = new QListWidgetItem(hobby.name);
        item->setData(Qt::UserRole, hobby.id);
        ui->hobbyList->addItem(item);
    }
    QListWidgetItem *addHobbyItem = new QListWidgetItem("+ hinzufügen");
    addHobbyItem->setFlags(addHobbyItem->flags() & ~Qt::ItemIsSelectable);
    ui->hobbyList->addItem(addHobbyItem);

    ui->pageStack->setCurrentWidget(ui->dashboardPage);

    // ── Sidebar-Navigation ──────────────────────────────────────────────────

    connect(ui->dashboardButton, &QPushButton::clicked, this, [this]() {
        ui->pageStack->setCurrentWidget(ui->dashboardPage);
    });

    connect(ui->statisticsButton, &QPushButton::clicked, this, [this]() {
        ui->pageStack->setCurrentWidget(ui->statisticsPage);
    });

    connect(ui->settingsButton, &QPushButton::clicked, this, [this]() {
        ui->pageStack->setCurrentWidget(ui->settingsPage);
    });

    // ── Hobby-Tab-Navigation ────────────────────────────────────────────────

    connect(ui->dashboardTab, &QToolButton::clicked, this, [this]() {
        ui->hobbyPageStack->setCurrentWidget(ui->dashboardHobbyPage);
    });

    connect(ui->routinesTab, &QToolButton::clicked, this, [this]() {
        ui->hobbyPageStack->setCurrentWidget(ui->routinesPage);
    });

    connect(ui->exercisesTab, &QToolButton::clicked, this, [this]() {
        ui->hobbyPageStack->setCurrentWidget(ui->exercisesPage);
    });

    connect(ui->historyTab, &QToolButton::clicked, this, [this]() {
        ui->hobbyPageStack->setCurrentWidget(ui->historyPage);
    });

    connect(ui->progressTab, &QToolButton::clicked, this, [this]() {
        ui->hobbyPageStack->setCurrentWidget(ui->progressPage);
    });

    connect(ui->goalsTab, &QToolButton::clicked, this, [this]() {
        ui->hobbyPageStack->setCurrentWidget(ui->goalsPage);
    });

    connect(ui->timelineTab, &QToolButton::clicked, this, [this]() {
        ui->hobbyPageStack->setCurrentWidget(ui->timelinePage);
    });

    connect(ui->roadmapTab, &QToolButton::clicked, this, [this]() {
        ui->hobbyPageStack->setCurrentWidget(ui->roadmapPage);
    });

    // ── Notizen ─────────────────────────────────────────────────────────────

    connect(ui->hobbyNotesTextEdit, &QPlainTextEdit::textChanged, this, [this]() {
        if (currentHobbyId == 0)
            return;
        HobbyNoteRepository::setContent(
            currentHobbyId,
            ui->hobbyNotesTextEdit->toPlainText()
        );
    });

    // ── Hobby-Liste ─────────────────────────────────────────────────────────

    connect(ui->hobbyList, &QListWidget::itemClicked, this, [this](QListWidgetItem *item) {

        if (item->text() == "+ hinzufügen") {
            bool ok;
            QString hobbyName = QInputDialog::getText(
                this, "Neues Hobby", "Name des Hobbys:",
                QLineEdit::Normal, "", &ok);

            if (ok && !hobbyName.isEmpty()) {
                int hobbyId;
                if (HobbyRepository::add(hobbyName, "", hobbyId)) {
                    QListWidgetItem *newItem = new QListWidgetItem(hobbyName);
                    newItem->setData(Qt::UserRole, hobbyId);
                    int addHobbyIndex = ui->hobbyList->count() - 1;
                    ui->hobbyList->insertItem(addHobbyIndex, newItem);
                }
            }
            return;
        }

        currentHobby   = item->text();
        currentHobbyId = item->data(Qt::UserRole).toInt();

        {
            QSignalBlocker blocker(ui->hobbyNotesTextEdit);
            ui->hobbyNotesTextEdit->setPlainText(
                HobbyNoteRepository::getContent(currentHobbyId));
        }

        // Kategorien des ausgewählten Hobbys in den Filter laden
        ui->exerciseCategoryComboBox->clear();

        ui->exerciseCategoryComboBox->addItem(
            "Alle Kategorien",
            0
            );

        ui->exerciseCategoryComboBox->addItem(
            "Archiv",
            -1
            );

        for (const Category &cat :
             CategoryRepository::getForHobby(currentHobbyId)) {

            ui->exerciseCategoryComboBox->addItem(
                cat.name,
                cat.id
                );
        }

        loadExerciseCards();
        loadHistory();

        ui->dashboardTab->setChecked(true);
        ui->hobbyPageStack->setCurrentWidget(ui->dashboardHobbyPage);
        ui->pageStack->setCurrentWidget(ui->hobbyPage);
    });

    // ── Übung hinzufügen ────────────────────────────────────────────────────

    connect(ui->addExerciseButton, &QPushButton::clicked, this, [this]() {
        QDialog dialog(this);
        Ui::ExerciseDialog dialogUi;
        dialogUi.setupUi(&dialog);

        dialogUi.categoryComboBox->clear();
        dialogUi.categoryComboBox->addItem("– keine –", -1);
        for (const Category &cat : CategoryRepository::getForHobby(currentHobbyId))
            dialogUi.categoryComboBox->addItem(cat.name, cat.id);
        dialogUi.categoryComboBox->addItem("+ Kategorie erstellen", -2);

        connect(dialogUi.categoryComboBox, &QComboBox::activated, &dialog,
            [&dialog, &dialogUi, this](int index) {
                if (dialogUi.categoryComboBox->itemData(index).toInt() != -2)
                    return;

                bool ok = false;
                QString categoryName = QInputDialog::getText(
                    &dialog, "Neue Kategorie", "Name der Kategorie:",
                    QLineEdit::Normal, "", &ok).trimmed();

                if (!ok || categoryName.isEmpty()) {
                    dialogUi.categoryComboBox->setCurrentIndex(0);
                    return;
                }

                int categoryId = 0;
                if (!CategoryRepository::add(currentHobbyId, categoryName, categoryId)) {
                    qDebug() << "Kategorie konnte nicht gespeichert werden.";
                    dialogUi.categoryComboBox->setCurrentIndex(0);
                    return;
                }

                const int createIndex = dialogUi.categoryComboBox->findData(-2);
                dialogUi.categoryComboBox->insertItem(createIndex, categoryName, categoryId);
                dialogUi.categoryComboBox->setCurrentIndex(createIndex);
            });

        if (dialog.exec() != QDialog::Accepted)
            return;

        QString name = dialogUi.nameLineEdit->text().trimmed();
        if (name.isEmpty())
            return;

        QString description = dialogUi.descriptionLineEdit->text().trimmed();
        int     categoryId  = dialogUi.categoryComboBox->currentData().toInt();
        QString unit        = dialogUi.unitLineEdit->text().trimmed();
        double  value       = dialogUi.valueLineEdit->text().replace(',', '.').toDouble();
        QString goal        = dialogUi.goalLineEdit->text().trimmed();

        int exerciseId;
        if (ExerciseRepository::add(
                currentHobbyId, name, description,
                categoryId, value, unit, goal, exerciseId)) {
            loadExerciseCards();
        } else {
            qDebug() << "Übung konnte nicht gespeichert werden.";
        }
    });

    // ── Suche & Kategoriefilter ──────────────────────────────────────────────

    connect(ui->exerciseSearchLineEdit, &QLineEdit::textChanged,
            this, [this]() { loadExerciseCards(); });

    connect(ui->exerciseCategoryComboBox, &QComboBox::currentIndexChanged,
            this, [this]() { loadExerciseCards(); });

    // ── Kategorien verwalten ────────────────────────────────────────────────

    connect(ui->manageCategoriesButton, &QPushButton::clicked, this, [this]() {
        QDialog dialog(this);
        Ui::CategoryManagementDialog dialogUi;
        dialogUi.setupUi(&dialog);

        // Kategorien des aktuellen Hobbys laden
        for (const Category &cat : CategoryRepository::getForHobby(currentHobbyId)) {
            auto *item = new QListWidgetItem(cat.name);
            item->setData(Qt::UserRole, cat.id);
            dialogUi.categoryListWidget->addItem(item);
        }

        // Buttons erst aktiv wenn eine Kategorie ausgewählt ist
        connect(dialogUi.categoryListWidget, &QListWidget::currentItemChanged, &dialog,
            [&dialogUi](QListWidgetItem *current) {
                bool has = current != nullptr;
                dialogUi.renameButton->setEnabled(has);
                dialogUi.deleteButton->setEnabled(has);
            });

        // Umbenennen
        connect(dialogUi.renameButton, &QPushButton::clicked, &dialog, [&]() {
            auto *item = dialogUi.categoryListWidget->currentItem();
            if (!item) return;
            bool ok;
            QString newName = QInputDialog::getText(
                &dialog, "Kategorie umbenennen", "Neuer Name:",
                QLineEdit::Normal, item->text(), &ok).trimmed();
            if (ok && !newName.isEmpty()) {
                CategoryRepository::rename(item->data(Qt::UserRole).toInt(), newName);
                item->setText(newName);
            }
        });

        // Löschen
        connect(dialogUi.deleteButton, &QPushButton::clicked, &dialog, [&]() {
            auto *item = dialogUi.categoryListWidget->currentItem();
            if (!item) return;
            auto res = QMessageBox::question(
                &dialog, "Kategorie löschen",
                QString("\"%1\" wirklich löschen?\n\nZugehörige Übungen bleiben erhalten.")
                    .arg(item->text()));
            if (res == QMessageBox::Yes) {
                CategoryRepository::remove(item->data(Qt::UserRole).toInt());
                delete dialogUi.categoryListWidget->takeItem(
                    dialogUi.categoryListWidget->row(item));
            }
        });

        connect(dialogUi.addButton, &QPushButton::clicked, &dialog, [&]() {
            bool ok;
            QString name = QInputDialog::getText(
                               &dialog, "Neue Kategorie", "Name der Kategorie:",
                               QLineEdit::Normal, "", &ok).trimmed();

            if (!ok || name.isEmpty())
                return;

            int categoryId = 0;
            if (!CategoryRepository::add(currentHobbyId, name, categoryId)) {
                qDebug() << "Kategorie konnte nicht gespeichert werden.";
                return;
            }

            auto *item = new QListWidgetItem(name);
            item->setData(Qt::UserRole, categoryId);
            dialogUi.categoryListWidget->addItem(item);
            dialogUi.categoryListWidget->setCurrentItem(item);
        });

        dialog.exec();

        // Nach dem Schließen Kategoriefilter und Cards neu laden
        ui->exerciseCategoryComboBox->clear();

        ui->exerciseCategoryComboBox->addItem(
            "Alle Kategorien",
            0
            );

        ui->exerciseCategoryComboBox->addItem(
            "Archiv",
            -1
            );

        const QList<Category> categories =
            CategoryRepository::getForHobby(currentHobbyId);

        for (const Category &category : categories) {
            ui->exerciseCategoryComboBox->addItem(
                category.name,
                category.id
                );
        }
    });


}

// ── Übungs-Cards laden ──────────────────────────────────────────────────────

void MainWindow::loadExerciseCards()
{
    // Alle bisherigen Cards entfernen
    while (ui->exerciseCardsLayout->count() > 0) {
        QLayoutItem *layoutItem = ui->exerciseCardsLayout->takeAt(0);
        if (layoutItem->widget())
            layoutItem->widget()->deleteLater();
        delete layoutItem;
    }

    if (currentHobbyId == 0)
        return;

    // Kategorienamen für Tag-Anzeige vorhalten
    QHash<int, QString> categoryNames;
    for (const Category &cat : CategoryRepository::getForHobby(currentHobbyId))
        categoryNames.insert(cat.id, cat.name);

    const QList<Exercise> exercises = ExerciseRepository::getForHobby(currentHobbyId, true);
    const QString searchText        = ui->exerciseSearchLineEdit->text().trimmed();
    const int selectedCategoryId    = ui->exerciseCategoryComboBox->currentData().toInt();
    const int columnCount           = 2;
    int visibleCount                = 0;

    for (const Exercise &exercise : exercises) {

        // Suchfilter
        if (!searchText.isEmpty() &&
            !exercise.name.contains(searchText, Qt::CaseInsensitive))
            continue;

        // Filter:
        // 0  = alle aktiven Übungen
        // -1 = archivierte Übungen
        // >0 = aktive Übungen einer bestimmten Kategorie
        if (selectedCategoryId == -1) {

            if (!exercise.archived)
                continue;

        } else if (selectedCategoryId == 0) {

            if (exercise.archived)
                continue;

        } else {

            if (exercise.archived ||
                exercise.categoryId != selectedCategoryId)
                continue;
        }

        // ── Card ────────────────────────────────────────────────────────────
        auto *card = new QFrame(ui->exerciseCardsWidget);
        card->setFrameShape(QFrame::StyledPanel);

        auto *cardLayout = new QVBoxLayout(card);
        cardLayout->setSpacing(4);
        cardLayout->setContentsMargins(10, 10, 10, 10);

        // Name
        auto *nameLabel = new QLabel(exercise.name, card);
        nameLabel->setStyleSheet("font-weight: bold; font-size: 13px;");
        nameLabel->setWordWrap(true);
        cardLayout->addWidget(nameLabel);

        cardLayout->addSpacing(4);


        ExerciseLog latestLog;
        const bool hasLatestLog =
            ExerciseLogRepository::getLatestForExercise(
                exercise.id,
                latestLog
                );

        // Wert
        auto *wertRow = new QHBoxLayout();
        wertRow->addWidget(new QLabel("Wert", card));
        wertRow->addStretch();

        // Der angezeigte Wert entspricht immer dem zuletzt gespeicherten Wert.
        // Wenn noch keine Ausführung existiert, wird der ursprüngliche Wert verwendet.
        QString wertText;

        if (hasLatestLog) {

            wertText =
                QString("%1%2")
                    .arg(latestLog.value, 0, 'g', 15)
                    .arg(
                        latestLog.unit.isEmpty()
                            ? ""
                            : " " + latestLog.unit
                        );

        } else if (exercise.value > 0) {

            wertText =
                QString("%1%2")
                    .arg(exercise.value, 0, 'g', 15)
                    .arg(
                        exercise.unit.isEmpty()
                            ? ""
                            : " " + exercise.unit
                        );

        } else {

            wertText = "–";
        }
        wertRow->addWidget(new QLabel(wertText, card));
        cardLayout->addLayout(wertRow);

        // Ziel
        auto *goalRow = new QHBoxLayout();
        goalRow->addWidget(new QLabel("Ziel", card));
        goalRow->addStretch();
        QString goalText = exercise.goal;

        if (!goalText.isEmpty() && !exercise.unit.isEmpty())
            goalText += " " + exercise.unit;

        if (goalText.isEmpty())
            goalText = "–";

        goalRow->addWidget(
            new QLabel(goalText, card)
            );
        cardLayout->addLayout(goalRow);

        cardLayout->addSpacing(4);

        // Letzte Ausführung
        auto *lastHeader = new QLabel("Letzte Ausführung", card);
        lastHeader->setStyleSheet("color: #888888; font-size: 11px;");
        cardLayout->addWidget(lastHeader);

        auto *lastRow = new QHBoxLayout();

        if (hasLatestLog) {
            const QDateTime performedAt = QDateTime::fromString(
                latestLog.performedAt, "yyyy-MM-dd HH:mm:ss").toLocalTime();
            lastRow->addWidget(new QLabel(performedAt.toString("dd.MM.yyyy"), card));
            lastRow->addStretch();
        } else {
            lastRow->addWidget(new QLabel("–", card));
            lastRow->addStretch();
        }
        cardLayout->addLayout(lastRow);

        // Kategorie-Tag
        if (exercise.categoryId > 0 && categoryNames.contains(exercise.categoryId)) {
            cardLayout->addSpacing(4);
            auto *tagRow = new QHBoxLayout();
            auto *tag    = new QLabel(categoryNames.value(exercise.categoryId), card);
            tag->setStyleSheet(
                "background-color: #2a2a2a;"
                "color: #aaaaaa;"
                "border-radius: 4px;"
                "padding: 2px 8px;"
                "font-size: 11px;");
            tagRow->addWidget(tag);
            tagRow->addStretch();
            cardLayout->addLayout(tagRow);
        }

        // Trennlinie
        auto *separator = new QFrame(card);
        separator->setFrameShape(QFrame::HLine);
        separator->setStyleSheet("color: #333333;");
        cardLayout->addSpacing(4);
        cardLayout->addWidget(separator);

        // Buttons
        auto *btnRow    = new QHBoxLayout();
        auto *editBtn   = new QPushButton("Bearbeiten", card);
        auto *runBtn    = new QPushButton("Ausführen", card);
        runBtn->setStyleSheet("font-weight: bold;");
        btnRow->addWidget(editBtn);
        btnRow->addWidget(runBtn);
        cardLayout->addLayout(btnRow);

        connect(editBtn, &QPushButton::clicked, this, [this, exercise]() {
            editExercise(exercise.id);
        });
        connect(
            runBtn,
            &QPushButton::clicked,
            this,
            [this, exercise]() {

                ExerciseExecutionDialog dialog(
                    exercise,
                    this
                    );

                // Der Dialog liefert Accepted zurück,
                // wenn die Ausführung erfolgreich gespeichert wurde.
                if (dialog.exec() == QDialog::Accepted) {
                    loadExerciseCards();
                    loadHistory();
                }
            }
            );


        // Position im Grid
        const int row    = visibleCount / columnCount;
        const int column = visibleCount % columnCount;
        ui->exerciseCardsLayout->addWidget(card, row, column);
        ++visibleCount;
    }

    // Spacer damit die Cards oben bleiben
    auto *spacer = new QSpacerItem(0, 0, QSizePolicy::Minimum, QSizePolicy::Expanding);
    ui->exerciseCardsLayout->addItem(spacer, visibleCount / columnCount + 1, 0);

    // Leerer Zustand
    if (visibleCount == 0) {
        auto *emptyLabel = new QLabel("Keine Übungen gefunden.", ui->exerciseCardsWidget);
        emptyLabel->setAlignment(Qt::AlignCenter);
        ui->exerciseCardsLayout->addWidget(emptyLabel, 0, 0, 1, columnCount);
    }
}

// ── Übung bearbeiten ────────────────────────────────────────────────────────

void MainWindow::editExercise(int exerciseId)
{
    Exercise exercise;
    if (!ExerciseRepository::getById(exerciseId, exercise)) {
        qDebug() << "Übung konnte nicht geladen werden.";
        return;
    }

    // Der zuletzt eingetragene Wert wird nur für die Anzeige im Eingabefeld verwendet.
    // Der gespeicherte Übungswert selbst wird dadurch nicht verändert.
    ExerciseLog latestLog;
    const bool hasLatestLog =
        ExerciseLogRepository::getLatestForExercise(
            exercise.id,
            latestLog
            );

    QDialog dialog(this);
    Ui::ExerciseDialog dialogUi;
    dialogUi.setupUi(&dialog);
    dialog.setWindowTitle("Übung bearbeiten");

    dialogUi.categoryComboBox->clear();
    dialogUi.categoryComboBox->addItem("– keine –", -1);

    for (const Category &cat :
         CategoryRepository::getForHobby(currentHobbyId)) {

        dialogUi.categoryComboBox->addItem(
            cat.name,
            cat.id
            );
    }

    dialogUi.categoryComboBox->addItem(
        "+ Kategorie erstellen",
        -2
        );

    connect(
        dialogUi.categoryComboBox,
        &QComboBox::activated,
        &dialog,
        [&dialog, &dialogUi, this](int index) {

            if (dialogUi.categoryComboBox->itemData(index).toInt() != -2)
                return;

            bool ok = false;

            QString categoryName = QInputDialog::getText(
                                       &dialog,
                                       "Neue Kategorie",
                                       "Name der Kategorie:",
                                       QLineEdit::Normal,
                                       "",
                                       &ok
                                       ).trimmed();

            if (!ok || categoryName.isEmpty())
                return;

            int categoryId = 0;

            if (!CategoryRepository::add(
                    currentHobbyId,
                    categoryName,
                    categoryId)) {

                qDebug() << "Kategorie konnte nicht gespeichert werden.";
                return;
            }

            const int createIndex =
                dialogUi.categoryComboBox->findData(-2);

            dialogUi.categoryComboBox->insertItem(
                createIndex,
                categoryName,
                categoryId
                );

            dialogUi.categoryComboBox->setCurrentIndex(
                createIndex
                );
        }
        );

    // Der Button zeigt abhängig vom aktuellen Status die passende Aktion an.
    dialogUi.archiveButton->setText(
        exercise.archived
            ? "Wiederherstellen"
            : "Archivieren"
        );


    // Archivieren und Wiederherstellen sind eigenständige Aktionen.
    // Deshalb wird dafür nicht der normale Speichern-Button benötigt.
    connect(
        dialogUi.archiveButton,
        &QPushButton::clicked,
        &dialog,
        [&]() {

            const bool newState = !exercise.archived;

            if (ExerciseRepository::setArchived(
                    exercise.id,
                    newState)) {

                dialog.reject();
                loadExerciseCards();
            } else {
                qDebug() << "Archivstatus konnte nicht geändert werden.";
            }
        }
        );

    connect(
        dialogUi.deleteButton,
        &QPushButton::clicked,
        &dialog,
        [&]() {

            const auto result = QMessageBox::warning(
                &dialog,
                "Übung löschen",
                "Möchtest du diese Übung wirklich löschen?\n\n"
                "Alle zugehörigen Ausführungen werden ebenfalls gelöscht.",
                QMessageBox::Yes | QMessageBox::No,
                QMessageBox::No
                );

            if (result != QMessageBox::Yes)
                return;

            if (ExerciseRepository::remove(exercise.id)) {

                dialog.accept();
                loadExerciseCards();

            } else {

                qDebug() << "Übung konnte nicht gelöscht werden.";
            }
        }
        );

    // Bestehende Werte eintragen

    dialogUi.nameLineEdit->setText(exercise.name);
    dialogUi.descriptionLineEdit->setText(exercise.description);

    if (hasLatestLog) {
        // Wenn bereits eine Ausführung existiert, zeigen wir deren Wert an.
        dialogUi.valueLineEdit->setText(
            QString::number(latestLog.value, 'g', 15)
            );
    } else {
        // Ohne Ausführung verwenden wir den ursprünglichen Übungswert.
        dialogUi.valueLineEdit->setText(
            QString::number(exercise.value, 'g', 15)
            );
    }

    dialogUi.unitLineEdit->setText(exercise.unit);
    dialogUi.goalLineEdit->setText(exercise.goal);

    const int categoryIndex =
        dialogUi.categoryComboBox->findData(exercise.categoryId);

    if (categoryIndex >= 0)
        dialogUi.categoryComboBox->setCurrentIndex(categoryIndex);

    if (dialog.exec() != QDialog::Accepted)
        return;

    QString name =
        dialogUi.nameLineEdit->text().trimmed();

    if (name.isEmpty())
        return;

    QString description =
        dialogUi.descriptionLineEdit->text().trimmed();

    int categoryId =
        dialogUi.categoryComboBox->currentData().toInt();

    double value =
        dialogUi.valueLineEdit->text()
            .replace(',', '.')
            .toDouble();

    QString unit =
        dialogUi.unitLineEdit->text().trimmed();

    QString goal =
        dialogUi.goalLineEdit->text().trimmed();

    if (ExerciseRepository::update(
            exerciseId,
            name,
            description,
            categoryId,
            value,
            unit,
            goal)) {

        loadExerciseCards();
        // Verlauf aktualisieren, damit auch alte Logs den neuen Übungsnamen anzeigen.
        loadHistory();

    } else {

        qDebug() << "Übung konnte nicht aktualisiert werden.";
    }
}
void MainWindow::loadHistory()
{
    // Nur dynamisch erzeugte Verlaufselemente entfernen.
    // Die festen UI-Elemente (Empty-State und Spacer) bleiben bestehen.
    for (int i = ui->historyEntriesLayout->count() - 1; i >= 0; --i) {

        QLayoutItem *item =
            ui->historyEntriesLayout->itemAt(i);

        QWidget *widget = item->widget();

        if (auto *entry =
            qobject_cast<HistoryEntryWidget *>(widget)) {

            ui->historyEntriesLayout->removeWidget(entry);
            entry->deleteLater();
        }

        if (auto *header =
            qobject_cast<HistoryDateHeaderWidget *>(widget)) {

            ui->historyEntriesLayout->removeWidget(header);
            header->deleteLater();
        }
    }

    // Alle Verlaufseinträge zunächst gemeinsam sammeln.
    // So können wir später alle Übungen unabhängig voneinander
    // nach dem tatsächlichen Ausführungszeitpunkt sortieren.
    struct HistoryItem
    {
        Exercise exercise;
        ExerciseLog log;
        QDateTime performedAt;
    };

    QList<HistoryItem> historyItems;
    const QString searchText =
        ui->historySearchLineEdit->text().trimmed();

    // Alle vorhandenen Übungen laden.
    const QList<Exercise> exercises =
        ExerciseRepository::getForHobby(currentHobbyId);

    // Übungen werden verarbeitet, wenn "Alle" oder "Übungen"
    // ausgewählt wurde. Bei "Routinen" werden sie übersprungen.
    const bool showExercises =
        historyFilter == HistoryFilter::All ||
        historyFilter == HistoryFilter::Exercises;

    if (showExercises) {

        for (const Exercise &exercise : exercises) {
            // Wenn eine Suche aktiv ist, nur Übungen mit passendem Namen übernehmen.
            if (!searchText.isEmpty() &&
                !exercise.name.contains(searchText, Qt::CaseInsensitive)) {

                continue;
            }

            // Alle Ausführungen dieser Übung laden.
            const QList<ExerciseLog> logs =
                ExerciseLogRepository::getForExercise(exercise.id);

            for (const ExerciseLog &log : logs) {

                const QDateTime performedAt =
                    QDateTime::fromString(
                        log.performedAt,
                        "yyyy-MM-dd HH:mm:ss"
                        ).toLocalTime();

                historyItems.append({
                    exercise,
                    log,
                    performedAt
                });
            }
        }
    }

    // Alle Einträge gemeinsam vom neuesten zum ältesten sortieren.
    std::sort(
        historyItems.begin(),
        historyItems.end(),
        [](const HistoryItem &a, const HistoryItem &b) {
            return a.performedAt > b.performedAt;
        }
        );

    // Merkt sich den Tag des zuletzt eingefügten Eintrags.
    QDate lastDate;

    for (const HistoryItem &item : historyItems) {

        const Exercise &exercise = item.exercise;
        const ExerciseLog &log = item.log;
        const QDateTime &performedAt = item.performedAt;

        const QDate currentDate =
            performedAt.date();

        // Neuer Tag -> neuen Datums-Header einfügen.
        if (currentDate != lastDate) {

            auto *header =
                new HistoryDateHeaderWidget(
                    ui->historyEntriesWidget
                    );

            QString dateText;

            const QDate today =
                QDate::currentDate();

            if (currentDate == today) {
                dateText = "HEUTE";
            }
            else if (currentDate == today.addDays(-1)) {
                dateText = "GESTERN";
            }
            else {
                dateText =
                    currentDate.toString("dd.MM.yyyy");
            }

            header->setDateText(dateText);

            ui->historyEntriesLayout->addWidget(header);

            lastDate = currentDate;
        }

        // Wert mit der im Log gespeicherten Einheit anzeigen.
        QString valueText;

        if (log.value != 0.0) {
            valueText =
                QString("%1%2")
                    .arg(log.value, 0, 'g', 15)
                    .arg(
                        log.unit.isEmpty()
                            ? ""
                            : " " + log.unit
                        );
        }

        // Dauer in Minuten und Sekunden umwandeln.
        QString durationText;

        if (log.durationSeconds > 0) {

            const int minutes =
                log.durationSeconds / 60;

            const int seconds =
                log.durationSeconds % 60;

            durationText =
                QString("%1:%2 min")
                    .arg(minutes)
                    .arg(
                        seconds,
                        2,
                        10,
                        QChar('0')
                        );
        }

        auto *entry =
            new HistoryEntryWidget(
                ui->historyEntriesWidget
                );

        entry->setData(
            false,
            exercise.name,
            valueText,
            durationText,
            performedAt.toString("HH:mm")
            );

        connect(
            entry,
            &HistoryEntryWidget::clicked,
            this,
            [this, exercise]() {

                ExerciseExecutionDialog dialog(
                    exercise,
                    this
                    );

                // Wenn eine neue Ausführung gespeichert wurde,
                // müssen Verlauf und Übungsdaten direkt aktualisiert werden.
                if (dialog.exec() == QDialog::Accepted) {
                    loadExerciseCards();
                    loadHistory();
                }
            }
            );

        ui->historyEntriesLayout->addWidget(entry);

    }

    // Prüfen, ob mindestens ein echter Verlaufseintrag vorhanden ist.
    bool hasEntries = false;

    for (int i = 0; i < ui->historyEntriesLayout->count(); ++i) {

        QLayoutItem *item =
            ui->historyEntriesLayout->itemAt(i);

        if (qobject_cast<HistoryEntryWidget *>(item->widget())) {
            hasEntries = true;
            break;
        }
    }

    // Empty-State nur anzeigen, wenn keine Einträge vorhanden sind.
    ui->historyEmptyStateLabel->setVisible(!hasEntries);
}

MainWindow::~MainWindow()
{
    delete ui;
};
