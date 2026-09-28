#include "routinecardwidget.h"

#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>

RoutineCardWidget::RoutineCardWidget(
    const Routine &routine,
    QWidget *parent
    )
    : QFrame(parent),
    routine(routine)
{
    // Die Karte bekommt eine feste Größe, damit das Grid
    // unabhängig vom Inhalt gleichmäßig bleibt.
    setFixedSize(320, 220);

    setupUi();
}

void RoutineCardWidget::setupUi()
{
    auto *mainLayout = new QVBoxLayout(this);

    mainLayout->setContentsMargins(16, 14, 16, 14);
    mainLayout->setSpacing(8);

    // ---------------------------------------------------------
    // Kopfbereich
    // ---------------------------------------------------------

    auto *headerLayout = new QHBoxLayout();
    headerLayout->setSpacing(8);

    nameLabel = new QLabel(routine.name, this);
    nameLabel->setObjectName("routineCardNameLabel");
    nameLabel->setSizePolicy(
        QSizePolicy::Expanding,
        QSizePolicy::Preferred
        );

    favoriteButton = new QPushButton("☆", this);
    favoriteButton->setObjectName("routineFavoriteButton");
    favoriteButton->setFixedSize(28, 28);
    favoriteButton->setToolTip("Zum Dashboard hinzufügen");

    headerLayout->addWidget(nameLabel);
    headerLayout->addWidget(favoriteButton);

    mainLayout->addLayout(headerLayout);

    // ---------------------------------------------------------
    // Beschreibung
    // ---------------------------------------------------------

    descriptionLabel = new QLabel(routine.description, this);
    descriptionLabel->setObjectName("routineCardDescriptionLabel");
    descriptionLabel->setWordWrap(true);
    descriptionLabel->setMinimumHeight(36);

    mainLayout->addWidget(descriptionLabel);

    // ---------------------------------------------------------
    // Letzte Ausführung
    // ---------------------------------------------------------

    lastExecutionLabel = new QLabel(
        "Letzte Ausführung: Noch nie",
        this
        );

    lastExecutionLabel->setObjectName("routineLastExecutionLabel");

    mainLayout->addWidget(lastExecutionLabel);

    // Der Spacer drückt die Buttons an den unteren Kartenrand.
    mainLayout->addStretch();

    // ---------------------------------------------------------
    // Untere Button-Leiste
    // ---------------------------------------------------------

    auto *buttonLayout = new QHBoxLayout();
    buttonLayout->setSpacing(8);

    archiveButton = new QPushButton(this);
    archiveButton->setObjectName("routineArchiveButton");

    editButton = new QPushButton("Bearbeiten", this);
    editButton->setObjectName("routineEditButton");

    executeButton = new QPushButton("Ausführen", this);
    executeButton->setObjectName("routineExecuteButton");

    buttonLayout->addWidget(archiveButton);
    buttonLayout->addWidget(editButton);
    buttonLayout->addWidget(executeButton);

    mainLayout->addLayout(buttonLayout);

    updateArchiveButton();

    // ---------------------------------------------------------
    // Signale
    // ---------------------------------------------------------

    connect(
        editButton,
        &QPushButton::clicked,
        this,
        [this]() {
            emit editRequested(routine.id);
        }
        );

    connect(
        executeButton,
        &QPushButton::clicked,
        this,
        [this]() {
            emit executeRequested(routine.id);
        }
        );

    connect(
        archiveButton,
        &QPushButton::clicked,
        this,
        [this]() {
            emit archiveRequested(
                routine.id,
                !routine.archived
                );
        }
        );
}

void RoutineCardWidget::updateArchiveButton()
{
    if (routine.archived) {
        archiveButton->setText("Aktivieren");
        archiveButton->setToolTip(
            "Routine wieder aktivieren"
            );
    } else {
        archiveButton->setText("Archivieren");
        archiveButton->setToolTip(
            "Routine archivieren"
            );
    }
}