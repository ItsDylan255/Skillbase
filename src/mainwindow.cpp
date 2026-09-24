#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QInputDialog>
#include "hobbyrepository.h"
#include "exerciserepository.h"
#include <QDebug>
#include "hobbynoterepository.h"
#include <QSignalBlocker>
#include "ui_exercise_dialog.h"
#include "categoryrepository.h"
#include <QGroupBox>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>

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

    connect(ui->dashboardButton, &QPushButton::clicked, this, [this]() {
        ui->pageStack->setCurrentWidget(ui->dashboardPage);
    });



    connect(ui->statisticsButton, &QPushButton::clicked, this, [this]() {
        ui->pageStack->setCurrentWidget(ui->statisticsPage);
    });

    connect(ui->settingsButton, &QPushButton::clicked, this, [this]() {
        ui->pageStack->setCurrentWidget(ui->settingsPage);
    });

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

    connect(ui->hobbyNotesTextEdit, &QPlainTextEdit::textChanged, this, [this]() {
        if (currentHobbyId == 0) {
            return;
        }

        HobbyNoteRepository::setContent(
            currentHobbyId,
            ui->hobbyNotesTextEdit->toPlainText()
            );
    });

    connect(ui->hobbyList, &QListWidget::itemClicked, this, [this](QListWidgetItem *item) {

        if (item->text() == "+ hinzufügen") {
            bool ok;

            QString hobbyName = QInputDialog::getText(
                this,
                "Neues Hobby",
                "Name des Hobbys:",
                QLineEdit::Normal,
                "",
                &ok
                );

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

        currentHobby = item->text();
        currentHobbyId = item->data(Qt::UserRole).toInt();
        {
            QSignalBlocker blocker(ui->hobbyNotesTextEdit);

            ui->hobbyNotesTextEdit->setPlainText(
                HobbyNoteRepository::getContent(currentHobbyId)
                );
        }

       loadExerciseCards();

        ui->dashboardTab->setChecked(true);
        ui->hobbyPageStack->setCurrentWidget(ui->dashboardHobbyPage);

        ui->pageStack->setCurrentWidget(ui->hobbyPage);
    });

    connect(ui->addExerciseButton, &QPushButton::clicked, this, [this]() {
        QDialog dialog(this);
        Ui::ExerciseDialog dialogUi;
        dialogUi.setupUi(&dialog);

        dialogUi.categoryComboBox->clear();

        dialogUi.categoryComboBox->addItem("– keine –", -1);

        // Load the categories belonging to the currently selected hobby.
        for (const Category &category : CategoryRepository::getForHobby(currentHobbyId)) {
            dialogUi.categoryComboBox->addItem(category.name, category.id);
        }

        // This entry is handled separately because it does not represent a category.
        dialogUi.categoryComboBox->addItem("+ Kategorie erstellen", -2);

        connect(
            dialogUi.categoryComboBox,
            &QComboBox::activated,
            &dialog,
            [&dialog, &dialogUi, this](int index) {

                if (dialogUi.categoryComboBox->itemData(index).toInt() != -2) {
                    return;
                }

                bool ok = false;

                QString categoryName = QInputDialog::getText(
                                           &dialog,
                                           "Neue Kategorie",
                                           "Name der Kategorie:",
                                           QLineEdit::Normal,
                                           "",
                                           &ok
                                           ).trimmed();

                if (!ok || categoryName.isEmpty()) {
                    dialogUi.categoryComboBox->setCurrentIndex(0);
                    return;
                }

                int categoryId = 0;

                if (!CategoryRepository::add(
                        currentHobbyId,
                        categoryName,
                        categoryId
                        )) {

                    qDebug() << "Kategorie konnte nicht gespeichert werden.";
                    dialogUi.categoryComboBox->setCurrentIndex(0);
                    return;
                }

                // Add the newly created category before the special
                // "create category" entry and select it immediately.
                const int createIndex =
                    dialogUi.categoryComboBox->findData(-2);

                dialogUi.categoryComboBox->insertItem(
                    createIndex,
                    categoryName,
                    categoryId
                    );

                dialogUi.categoryComboBox->setCurrentIndex(createIndex);
            }
            );

        if (dialog.exec() != QDialog::Accepted) {
            return;
        }

        // Der Name ist das einzige Pflichtfeld.
        QString name = dialogUi.nameLineEdit->text().trimmed();

        if (name.isEmpty()) {
            return;
        }

        QString description =
            dialogUi.descriptionLineEdit->text().trimmed();

        int categoryId =
            dialogUi.categoryComboBox->currentData().toInt();

        QString unit =
            dialogUi.unitLineEdit->text().trimmed();

        double value =
            dialogUi.valueLineEdit->text()
                .replace(',', '.')
                .toDouble();

        QString goal =
            dialogUi.goalLineEdit->text().trimmed();

        int exerciseId;

        if (ExerciseRepository::add(
                currentHobbyId,
                name,
                description,
                categoryId,
                value,
                unit,
                goal,
                exerciseId
                )) {

            loadExerciseCards();

        } else {
            qDebug() << "Übung konnte nicht gespeichert werden.";
        }
    });

}

void MainWindow::loadExerciseCards()
{
    // Alle bisher angezeigten Cards entfernen, der Spacer bleibt erhalten.
    while (ui->exerciseCardsLayout->count() > 1) {
        QLayoutItem *item = ui->exerciseCardsLayout->takeAt(0);

        if (item->widget()) {
            item->widget()->deleteLater();
        }

        delete item;
    }

    if (currentHobbyId == 0) {
        return;
    }

    const QList<Exercise> exercises =
        ExerciseRepository::getForHobby(currentHobbyId);

    const int columnCount = 2;

    for (int i = 0; i < exercises.size(); ++i) {
        const Exercise &exercise = exercises.at(i);

        auto *card =
            new QGroupBox(exercise.name, ui->exerciseCardsWidget);

        auto *layout = new QVBoxLayout(card);

        auto *descriptionLabel =
            new QLabel(exercise.description, card);

        auto *startValueLabel =
            new QLabel(
                "Startwert: " +
                    QString::number(exercise.value) +
                    (exercise.unit.isEmpty() ? "" : " " + exercise.unit),
                card
                );

        auto *goalLabel =
            new QLabel(
                "Ziel: " + exercise.goal,
                card
                );

        auto *lastExecutionLabel =
            new QLabel(
                "Letzte Ausführung: –",
                card
                );

        auto *buttonLayout = new QHBoxLayout();

        auto *editButton =
            new QPushButton("Bearbeiten", card);

        auto *executeButton =
            new QPushButton("Ausführen", card);

        buttonLayout->addWidget(editButton);
        buttonLayout->addWidget(executeButton);

        layout->addWidget(descriptionLabel);
        layout->addWidget(startValueLabel);
        layout->addWidget(goalLabel);
        layout->addWidget(lastExecutionLabel);
        layout->addLayout(buttonLayout);

        const int row = i / columnCount;
        const int column = i % columnCount;

        ui->exerciseCardsLayout->addWidget(
            card,
            row,
            column
            );

        connect(editButton, &QPushButton::clicked, this, [this, exercise]() {
            editExercise(exercise.id);
        });

        connect(executeButton, &QPushButton::clicked, this, [exercise]() {
            qDebug() << "Ausführen:" << exercise.id;
        });
    }
}

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

    // Die Kategorien des aktuellen Hobbys in das Dropdown laden.
    dialogUi.categoryComboBox->clear();
    dialogUi.categoryComboBox->addItem("– keine –", -1);

    for (const Category &category :
         CategoryRepository::getForHobby(currentHobbyId)) {

        dialogUi.categoryComboBox->addItem(
            category.name,
            category.id
            );
    }

    // Dieser Eintrag dient zum Erstellen einer neuen Kategorie.
    dialogUi.categoryComboBox->addItem(
        "+ Kategorie erstellen",
        -2
        );

    connect(
        dialogUi.categoryComboBox,
        &QComboBox::activated,
        &dialog,
        [&dialog, &dialogUi, this](int index) {

            if (dialogUi.categoryComboBox->itemData(index).toInt() != -2) {
                return;
            }

            bool ok = false;

            QString categoryName = QInputDialog::getText(
                                       &dialog,
                                       "Neue Kategorie",
                                       "Name der Kategorie:",
                                       QLineEdit::Normal,
                                       "",
                                       &ok
                                       ).trimmed();

            if (!ok || categoryName.isEmpty()) {
                return;
            }

            int categoryId = 0;

            if (!CategoryRepository::add(
                    currentHobbyId,
                    categoryName,
                    categoryId
                    )) {

                qDebug() << "Kategorie konnte nicht gespeichert werden.";
                return;
            }

            // Die neue Kategorie vor dem Eintrag zum Erstellen weiterer
            // Kategorien einfügen und direkt auswählen.
            const int createIndex =
                dialogUi.categoryComboBox->findData(-2);

            dialogUi.categoryComboBox->insertItem(
                createIndex,
                categoryName,
                categoryId
                );

            dialogUi.categoryComboBox->setCurrentIndex(createIndex);
        }
        );

    // Die gespeicherten Werte der Übung in den Dialog übernehmen.
    dialogUi.nameLineEdit->setText(exercise.name);
    dialogUi.descriptionLineEdit->setText(exercise.description);
    dialogUi.valueLineEdit->setText(QString::number(exercise.value));
    dialogUi.unitLineEdit->setText(exercise.unit);
    dialogUi.goalLineEdit->setText(exercise.goal);

    // Die gespeicherte Kategorie im Dropdown auswählen.
    const int categoryIndex =
        dialogUi.categoryComboBox->findData(exercise.categoryId);

    if (categoryIndex >= 0) {
        dialogUi.categoryComboBox->setCurrentIndex(categoryIndex);
    }

    if (dialog.exec() != QDialog::Accepted) {
        return;
    }

    QString name =
        dialogUi.nameLineEdit->text().trimmed();

    if (name.isEmpty()) {
        return;
    }

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
            goal
            )) {

        loadExerciseCards();

    } else {
        qDebug() << "Übung konnte nicht aktualisiert werden.";
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}

