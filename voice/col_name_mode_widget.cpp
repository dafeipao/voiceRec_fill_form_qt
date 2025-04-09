#include<col_name_mode_widget.h>

col_name_mode_widget::col_name_mode_widget(QWidget *parent)
    : QWidget(parent){

    lineedit = new QLineEdit("ITEM", this);
    comboBox = new QComboBox(this);

    lineedit->setMinimumSize(50,25);
    comboBox->setMinimumSize(50,25);

    layout = new QVBoxLayout(this);
    layout->addWidget(lineedit);
    layout->addWidget(comboBox);

    comboBox->addItem("姓名");
    comboBox->addItem("成绩");
    comboBox->addItem("1位数字");

    setLayout(layout);
}

QString col_name_mode_widget::getCurrentLineEdit() const {
    return lineedit->text();
}

QString col_name_mode_widget::getCurrentComboBoxText() const {
    return comboBox->currentText();
}
