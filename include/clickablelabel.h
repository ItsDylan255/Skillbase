#ifndef CLICKABLELABEL_H
#define CLICKABLELABEL_H

#include <QLabel>

// Ein QLabel, das bei einem Linksklick ein clicked()-Signal aussendet.
// Wird für den Übungsnamen in der Routine- und Übungs-Ausführung
// verwendet, damit der Nutzer zur Fortschrittsseite der Übung springen kann.
class ClickableLabel : public QLabel
{
    Q_OBJECT

public:
    explicit ClickableLabel(QWidget *parent = nullptr);
    explicit ClickableLabel(const QString &text, QWidget *parent = nullptr);

signals:
    // Wird bei einem Linksklick ausgesendet.
    void clicked();

protected:
    void mouseReleaseEvent(QMouseEvent *event) override;
};

#endif // CLICKABLELABEL_H