#include "exercisedialog.h"

ExerciseDialog::ExerciseDialog(QWidget *parent)
    : QDialog(parent)
{
    ui.setupUi(this);

    // Speichern nur aktiv wenn Name nicht leer
    ui.buttonBox->button(QDialogButtonBox::Save)->setEnabled(false);
    connect(ui.nameLineEdit, &QLineEdit::textChanged, this, [this](const QString &text) {
        ui.buttonBox->button(QDialogButtonBox::Save)->setEnabled(!text.trimmed().isEmpty());
    });
}

void ExerciseDialog::setCategories(const QList<QPair<int, QString>> &categories)
{
    ui.categoryComboBox->clear();
    ui.categoryComboBox->addItem("– keine –", -1);
    for (const auto &[id, name] : categories) {
        ui.categoryComboBox->addItem(name, id);
    }
    ui.categoryComboBox->addItem("+ Kategorie erstellen", -2);
}

QString ExerciseDialog::name()        const { return ui.nameLineEdit->text().trimmed(); }
QString ExerciseDialog::description() const { return ui.descriptionLineEdit->text().trimmed(); }
QString ExerciseDialog::unit()        const { return ui.unitLineEdit->text().trimmed(); }
double  ExerciseDialog::value()       const { return ui.valueSpinBox->value(); }
int     ExerciseDialog::categoryId()  const { return ui.categoryComboBox->currentData().toInt(); }