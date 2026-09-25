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

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

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

        // Wert
        auto *wertRow = new QHBoxLayout();
        wertRow->addWidget(new QLabel("Wert", card));
        wertRow->addStretch();
        QString wertText = (exercise.value > 0)
            ? QString("%1%2").arg(exercise.value, 0, 'f', 1)
                             .arg(exercise.unit.isEmpty() ? "" : " " + exercise.unit)
            : "–";
        wertRow->addWidget(new QLabel(wertText, card));
        cardLayout->addLayout(wertRow);

        // Ziel
        auto *goalRow = new QHBoxLayout();
        goalRow->addWidget(new QLabel("Ziel", card));
        goalRow->addStretch();
        goalRow->addWidget(new QLabel(exercise.goal.isEmpty() ? "–" : exercise.goal, card));
        cardLayout->addLayout(goalRow);

        cardLayout->addSpacing(4);

        // Letzte Ausführung
        auto *lastHeader = new QLabel("Letzte Ausführung", card);
        lastHeader->setStyleSheet("color: #888888; font-size: 11px;");
        cardLayout->addWidget(lastHeader);

        auto *lastRow = new QHBoxLayout();
        ExerciseLog latestLog;
        if (ExerciseLogRepository::getLatestForExercise(exercise.id, latestLog)) {
            const QDateTime performedAt = QDateTime::fromString(
                latestLog.performedAt, "yyyy-MM-dd HH:mm:ss");
            lastRow->addWidget(new QLabel(performedAt.toString("dd.MM.yyyy"), card));
            lastRow->addStretch();
            if (!latestLog.unit.isEmpty()) {
                lastRow->addWidget(new QLabel(
                    QString("%1 %2").arg(latestLog.value).arg(latestLog.unit), card));
            }
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
        connect(runBtn, &QPushButton::clicked, this, [exercise]() {
            qDebug() << "Ausführen:" << exercise.id;
        });

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
    dialogUi.valueLineEdit->setText(QString::number(exercise.value));
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

    } else {

        qDebug() << "Übung konnte nicht aktualisiert werden.";
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}
