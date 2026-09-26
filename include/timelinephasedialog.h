#ifndef TIMELINEPHASEDIALOG_H
#define TIMELINEPHASEDIALOG_H

#include <QDialog>
#include <QString>
#include <QDate>
#include <QPushButton>

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

    bool validateInput();
    // Setzt die vorgeschlagenen Start- und Enddaten für eine neue Phase.
    void setDateRange(
        const QDate &startDate,
        const QDate &endDate
        );
    void setName(const QString &name);
    void setDescription(const QString &description);
    QPushButton *saveButton() const;
    void accept() override;

protected:


private:
    Ui::TimelinePhaseDialog *ui;
};

#endif