#include "historyentrywidget.h"
#include "ui_historyentrywidget.h"
#include <QStringList>

HistoryEntryWidget::HistoryEntryWidget(QWidget *parent)
    : QWidget(parent),
    ui(new Ui::HistoryEntryWidget)
{
    ui->setupUi(this);

    // Der Hover-Zustand wird über das zentrale Stylesheet gesteuert.
    // WA_Hover sorgt dafür, dass das Widget auf Mausbewegungen reagieren kann.
    setAttribute(Qt::WA_Hover, true);
}

HistoryEntryWidget::~HistoryEntryWidget()
{
    delete ui;
}

void HistoryEntryWidget::setData(
    bool isRoutine,
    const QString &title,
    const QString &valueText,
    const QString &durationText,
    const QString &timeText
    )
{
    // Das Symbol zeigt auf einen Blick, ob es sich um eine
    // ausgeführte Übung oder Routine handelt.
    ui->entryTypeIconLabel->setText(
        isRoutine ? "◆" : "●"
        );

    ui->entryTitleLabel->setText(title);
    ui->entryTimeLabel->setText(timeText);

    // Die verfügbaren Zusatzinformationen werden gesammelt.
    // Dadurch können Wert und Dauer unabhängig voneinander vorhanden sein.
    QStringList details;

    if (!valueText.isEmpty())
        details.append(valueText);

    if (!durationText.isEmpty())
        details.append(durationText);

    if (details.isEmpty()) {
        // Wenn keine Zusatzinformationen vorhanden sind,
        // wird die zweite Zeile komplett ausgeblendet.
        ui->entrySubtitleLabel->clear();
        ui->entrySubtitleLabel->setVisible(false);
    } else {
        ui->entrySubtitleLabel->setText(
            details.join(" · ")
            );
        ui->entrySubtitleLabel->setVisible(true);
    }


}

void HistoryEntryWidget::mousePressEvent(QMouseEvent *event)
{
    // Der Klick gilt für den gesamten Verlaufseintrag.
    // Dadurch muss der Nutzer nicht exakt auf den Text klicken.
    if (event->button() == Qt::LeftButton) {
        emit clicked();
    }

    // Das normale QWidget-Verhalten bleibt erhalten.
    QWidget::mousePressEvent(event);
}