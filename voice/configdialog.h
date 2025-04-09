#ifndef CONFIGDIALOG_H
#define CONFIGDIALOG_H

#include <QDialog>
#include <QLineEdit>
#include <QLabel>
#include <QFileDialog>

class ConfigDialog : public QDialog {
    Q_OBJECT

public:
    explicit ConfigDialog(QWidget *parent = nullptr);
    ~ConfigDialog();

signals:
    void config_saved();

private:
    QLineEdit *edit1;
    QLineEdit *edit2;
    QLineEdit *edit3;
    QLineEdit *edit4;
    QLineEdit *edit5;
    QPushButton *fileButton;
    void loadSettings();
private slots:
    void selectFile();  // 选择文件的槽函数
    void saveSettings(); // 保存到 setting.ini
};

#endif // CONFIGDIALOG_H
