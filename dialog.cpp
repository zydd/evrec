#include <QFile>
#include <QTextStream>

#include "dialog.h"
#include "ui_dialog.h"

Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);
}

Dialog::~Dialog() {
    delete ui;
}

void Dialog::accept() {
    QFile out("output.txt");
    out.open(QFile::WriteOnly | QFile::Truncate);
    QTextStream ts(&out);

    ts << "Line edit: '" << ui->lineEdit->text() << "'\n"
       << "Slider: '" << ui->horizontalSlider->value() << "'\n"
       << "Combo box: '" << ui->comboBox->currentText() << "'\n"
       << "Spint box: '" << ui->doubleSpinBox->value() << "'\n"
       << "Dial: '" << ui->dial->value() << "'\n";
    out.close();

    QDialog::accept();
}
