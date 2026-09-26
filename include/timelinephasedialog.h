#ifndef TIMELINEPHASEDIALOG_H
#define TIMELINEPHASEDIALOG_H

#include <QDialog>
#include <QString>
#include <QDate>

namespace Ui {
class TimelinePhaseDialog;
}

class TimelinePhaseDialog : public QDialog
{
    Q_OBJECT

public:
    explicit TimelinePhaseDialog(QWidget *parent = nullptr);
    ~TimelinePhaseDialog();

    QString name() const;
    QString description() const;
    QDate startDate() const;
    QDate endDate() const;
    // Setzt die vorgeschlagenen Start- und Enddaten für eine neue Phase.
    void setDateRange(
        const QDate &startDate,
        const QDate &endDate
        );

protected:
    void accept() override;

private:
    Ui::TimelinePhaseDialog *ui;
};

#endif