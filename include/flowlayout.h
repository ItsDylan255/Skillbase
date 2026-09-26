#ifndef FLOWLAYOUT_H
#define FLOWLAYOUT_H

#include <QLayout>
#include <QRect>
#include <QStyle>


// Ordnet seine Kind-Widgets zeilenweise nebeneinander an.
//
// Reicht der verfügbare Platz in der aktuellen Zeile nicht mehr aus,
// beginnt automatisch eine neue Zeile - ähnlich wie Textumbruch,
// nur für Widgets statt für Wörter.
//
// Anders als QHBoxLayout (nie umbrechend) oder QGridLayout
// (feste Spaltenzahl) passt sich FlowLayout dynamisch an die
// aktuelle Breite des Containers an. Genau das braucht die
// Timeline-Seite für die responsiv nebeneinander stehenden
// Phasen-Cards (Regel #30, Responsive Desktop Behavior).
class FlowLayout : public QLayout
{
public:
    explicit FlowLayout(QWidget *parent, int margin = -1, int hSpacing = -1, int vSpacing = -1);
    explicit FlowLayout(int margin = -1, int hSpacing = -1, int vSpacing = -1);
    ~FlowLayout() override;

    void addItem(QLayoutItem *item) override;
    int horizontalSpacing() const;
    int verticalSpacing() const;
    Qt::Orientations expandingDirections() const override;
    bool hasHeightForWidth() const override;
    int heightForWidth(int width) const override;
    int count() const override;
    QLayoutItem *itemAt(int index) const override;
    QSize minimumSize() const override;
    void setGeometry(const QRect &rect) override;
    QSize sizeHint() const override;
    QLayoutItem *takeAt(int index) override;

private:
    int doLayout(const QRect &rect, bool testOnly) const;
    int smartSpacing(QStyle::PixelMetric pm) const;

    QList<QLayoutItem *> itemList;
    int m_hSpace;
    int m_vSpace;
};

#endif
