#include "historydateheaderwidget.h"
#include "ui_historydateheaderwidget.h"

HistoryDateHeaderWidget::HistoryDateHeaderWidget(QWidget *parent)
    : QWidget(parent),
    ui(new Ui::HistoryDateHeaderWidget)
{
    ui->setupUi(this);
}

HistoryDateHeaderWidget::~HistoryDateHeaderWidget()
{
    delete ui;
}

void HistoryDateHeaderWidget::setDateText(const QString &text)
{
    // Die Anzeige wird bewusst von außen gesetzt,
    // damit dieses Widget keine Datumslogik kennen muss.
    ui->dateLabel->setText(text);
}