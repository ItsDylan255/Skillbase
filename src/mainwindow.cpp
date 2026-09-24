#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QInputDialog>
#include "hobbyrepository.h"
#include "exerciserepository.h"
#include <QDebug>

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

        ui->exerciseList->clear();

        for (const Exercise &exercise : ExerciseRepository::getForHobby(currentHobbyId)) {
            ui->exerciseList->addItem(exercise.name);
        }

        ui->dashboardTab->setChecked(true);
        ui->hobbyPageStack->setCurrentWidget(ui->dashboardHobbyPage);

        ui->pageStack->setCurrentWidget(ui->hobbyPage);
    });

    connect(ui->addExerciseButton, &QPushButton::clicked, this, [this]() {
        bool ok;

        QString exerciseName = QInputDialog::getText(
            this,
            "Neue Übung",
            "Name der Übung:",
            QLineEdit::Normal,
            "",
            &ok
            );

        if (ok && !exerciseName.isEmpty()) {
            int exerciseId;

            if (ExerciseRepository::add(
                    currentHobbyId,
                    exerciseName,
                    "",
                    "",
                    "",
                    exerciseId
                    )) {

                ui->exerciseList->addItem(exerciseName);

            } else {
                qDebug() << "Übung konnte nicht gespeichert werden.";
            }
        }
    });
}

MainWindow::~MainWindow()
{
    delete ui;
}

