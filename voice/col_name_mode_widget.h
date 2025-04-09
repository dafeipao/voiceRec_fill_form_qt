#ifndef COL_NAME_MODE_WIDGET_H
#define COL_NAME_MODE_WIDGET_H

#include<QWidget>
#include<QLineEdit>
#include<QComboBox>
#include<QLayout>
class col_name_mode_widget:public QWidget{
    Q_OBJECT
public:
    explicit col_name_mode_widget(QWidget *parent = nullptr);
    QString getCurrentLineEdit() const;
    QString getCurrentComboBoxText() const;
private:
    QLineEdit *lineedit;
    QComboBox *comboBox;
    QVBoxLayout *layout;
};


#endif // COL_NAME_MODE_WIDGET_H
