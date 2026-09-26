#ifndef HISTORYDATEHEADERWIDGET_H
#define HISTORYDATEHEADERWIDGET_H

#include <QWidget>

namespace Ui {
class HistoryDateHeaderWidget;
}

class HistoryDateHeaderWidget : public QWidget
{
    Q_OBJECT

public:
    explicit HistoryDateHeaderWidget(QWidget *parent = nullptr);
    ~HistoryDateHeaderWidget();

    // Setzt die Überschrift des Tages, z. B. "HEUTE" oder "GESTERN".
    void setDateText(const QString &text);

private:
    Ui::HistoryDateHeaderWidget *ui;
};

#endif