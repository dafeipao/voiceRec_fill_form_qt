#include "configdialog.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QSettings>
ConfigDialog::ConfigDialog(QWidget *parent) : QDialog(parent) {
    setWindowTitle("配置页面");
    setFixedSize(800, 400);

    QVBoxLayout *mainLayout = new QVBoxLayout(this);


    QHBoxLayout *nameLayout = new QHBoxLayout();
    QLabel *nameLabel = new QLabel("hostUrl:", this);
    edit1 = new QLineEdit(this);
    nameLabel->setFixedWidth(50);
    nameLayout->addWidget(nameLabel);
    nameLayout->addWidget(edit1);
    mainLayout->addLayout(nameLayout);


    QHBoxLayout *ageLayout = new QHBoxLayout();
    QLabel *ageLabel = new QLabel("APIKey:", this);
    edit2 = new QLineEdit(this);
    ageLabel->setFixedWidth(50);
    ageLayout->addWidget(ageLabel);
    ageLayout->addWidget(edit2);
    mainLayout->addLayout(ageLayout);


    QHBoxLayout *addressLayout = new QHBoxLayout();
    QLabel *addressLabel = new QLabel("APISecret:", this);
    edit3 = new QLineEdit(this);
    addressLabel->setFixedWidth(50);
    addressLayout->addWidget(addressLabel);
    addressLayout->addWidget(edit3);
    mainLayout->addLayout(addressLayout);


    QHBoxLayout *phoneLayout = new QHBoxLayout();
    QLabel *phoneLabel = new QLabel("APPID:", this);
    edit4 = new QLineEdit(this);
    phoneLabel->setFixedWidth(50);
    phoneLayout->addWidget(phoneLabel);
    phoneLayout->addWidget(edit4);
    mainLayout->addLayout(phoneLayout);


    QHBoxLayout *emailLayout = new QHBoxLayout();
    QLabel *emailLabel = new QLabel("姓名集:", this);
    edit5 = new QLineEdit(this);

    fileButton = new QPushButton("选择文件", this);
    emailLabel->setFixedWidth(50);
    emailLayout->addWidget(emailLabel);
    emailLayout->addWidget(edit5);
    emailLayout->addWidget(fileButton);
    mainLayout->addLayout(emailLayout);

    connect(fileButton, &QPushButton::clicked, this, &ConfigDialog::selectFile);

    loadSettings();

    QPushButton *closeButton = new QPushButton("保存", this);
    connect(closeButton, &QPushButton::clicked, this, &ConfigDialog::saveSettings);
    mainLayout->addWidget(closeButton);

}

void ConfigDialog::selectFile() {
    QString filePath = QFileDialog::getOpenFileName(this, "选择配置文件", "", "文本文件 (*.txt)");
    if (!filePath.isEmpty()) {
        edit5->setText(filePath);
    }
}

// 读取 setting.ini 配置
void ConfigDialog::loadSettings() {
    QSettings settings("setting.ini", QSettings::IniFormat);

    edit1->setText(settings.value("hostUrl", "").toString());
    edit2->setText(settings.value("APIKey", "").toString());
    edit3->setText(settings.value("APISecret", "").toString());
    edit4->setText(settings.value("APPID", "").toString());
    edit5->setText(settings.value("namePath", "").toString());
}

void ConfigDialog::saveSettings() {
    QSettings settings("setting.ini", QSettings::IniFormat);

    settings.setValue("hostUrl", edit1->text());
    settings.setValue("APIKey", edit2->text());
    settings.setValue("APISecret", edit3->text());
    settings.setValue("APPID", edit4->text());
    settings.setValue("namePath", edit5->text());

    settings.sync();  // 确保写入文件
    accept();

    emit config_saved();
}

ConfigDialog::~ConfigDialog() {}
