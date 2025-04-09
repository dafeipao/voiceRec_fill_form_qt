/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 6.8.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QAction *settings;
    QWidget *centralwidget;
    QGridLayout *gridLayout_2;
    QHBoxLayout *horizontalLayout_2;
    QWidget *widget_tt;
    QWidget *widget;
    QGridLayout *gridLayout_3;
    QVBoxLayout *verticalLayout_2;
    QHBoxLayout *horizontalLayout;
    QLabel *label;
    QSpinBox *row_num;
    QLabel *label_2;
    QSpinBox *col_num;
    QSpacerItem *verticalSpacer;
    QWidget *widget_2;
    QGridLayout *gridLayout;
    QVBoxLayout *verticalLayout;
    QLabel *label_3;
    QHBoxLayout *horizontalLayout_3;
    QLabel *label_8;
    QLabel *label_4;
    QLineEdit *left_top_r;
    QLabel *label_5;
    QLineEdit *left_top_c;
    QLabel *label_6;
    QHBoxLayout *horizontalLayout_4;
    QLabel *label_7;
    QLabel *label_9;
    QLineEdit *right_buttom_r;
    QLabel *label_10;
    QLineEdit *right_buttom_c;
    QLabel *label_11;
    QPushButton *set_all_limit;
    QLabel *voice_rec;
    QPushButton *pushButton;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName("MainWindow");
        MainWindow->resize(924, 673);
        settings = new QAction(MainWindow);
        settings->setObjectName("settings");
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName("centralwidget");
        gridLayout_2 = new QGridLayout(centralwidget);
        gridLayout_2->setObjectName("gridLayout_2");
        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName("horizontalLayout_2");
        widget_tt = new QWidget(centralwidget);
        widget_tt->setObjectName("widget_tt");

        horizontalLayout_2->addWidget(widget_tt);

        widget = new QWidget(centralwidget);
        widget->setObjectName("widget");
        QSizePolicy sizePolicy(QSizePolicy::Policy::Fixed, QSizePolicy::Policy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(widget->sizePolicy().hasHeightForWidth());
        widget->setSizePolicy(sizePolicy);
        gridLayout_3 = new QGridLayout(widget);
        gridLayout_3->setObjectName("gridLayout_3");
        verticalLayout_2 = new QVBoxLayout();
        verticalLayout_2->setObjectName("verticalLayout_2");
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName("horizontalLayout");
        label = new QLabel(widget);
        label->setObjectName("label");
        QFont font;
        font.setPointSize(15);
        label->setFont(font);

        horizontalLayout->addWidget(label);

        row_num = new QSpinBox(widget);
        row_num->setObjectName("row_num");
        row_num->setFont(font);
        row_num->setValue(60);

        horizontalLayout->addWidget(row_num);

        label_2 = new QLabel(widget);
        label_2->setObjectName("label_2");
        label_2->setFont(font);

        horizontalLayout->addWidget(label_2);

        col_num = new QSpinBox(widget);
        col_num->setObjectName("col_num");
        col_num->setFont(font);
        col_num->setValue(2);

        horizontalLayout->addWidget(col_num);


        verticalLayout_2->addLayout(horizontalLayout);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding);

        verticalLayout_2->addItem(verticalSpacer);

        widget_2 = new QWidget(widget);
        widget_2->setObjectName("widget_2");
        gridLayout = new QGridLayout(widget_2);
        gridLayout->setObjectName("gridLayout");
        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName("verticalLayout");
        label_3 = new QLabel(widget_2);
        label_3->setObjectName("label_3");

        verticalLayout->addWidget(label_3);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setObjectName("horizontalLayout_3");
        label_8 = new QLabel(widget_2);
        label_8->setObjectName("label_8");

        horizontalLayout_3->addWidget(label_8);

        label_4 = new QLabel(widget_2);
        label_4->setObjectName("label_4");

        horizontalLayout_3->addWidget(label_4);

        left_top_r = new QLineEdit(widget_2);
        left_top_r->setObjectName("left_top_r");

        horizontalLayout_3->addWidget(left_top_r);

        label_5 = new QLabel(widget_2);
        label_5->setObjectName("label_5");

        horizontalLayout_3->addWidget(label_5);

        left_top_c = new QLineEdit(widget_2);
        left_top_c->setObjectName("left_top_c");

        horizontalLayout_3->addWidget(left_top_c);

        label_6 = new QLabel(widget_2);
        label_6->setObjectName("label_6");

        horizontalLayout_3->addWidget(label_6);


        verticalLayout->addLayout(horizontalLayout_3);

        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setObjectName("horizontalLayout_4");
        label_7 = new QLabel(widget_2);
        label_7->setObjectName("label_7");

        horizontalLayout_4->addWidget(label_7);

        label_9 = new QLabel(widget_2);
        label_9->setObjectName("label_9");

        horizontalLayout_4->addWidget(label_9);

        right_buttom_r = new QLineEdit(widget_2);
        right_buttom_r->setObjectName("right_buttom_r");

        horizontalLayout_4->addWidget(right_buttom_r);

        label_10 = new QLabel(widget_2);
        label_10->setObjectName("label_10");

        horizontalLayout_4->addWidget(label_10);

        right_buttom_c = new QLineEdit(widget_2);
        right_buttom_c->setObjectName("right_buttom_c");

        horizontalLayout_4->addWidget(right_buttom_c);

        label_11 = new QLabel(widget_2);
        label_11->setObjectName("label_11");

        horizontalLayout_4->addWidget(label_11);


        verticalLayout->addLayout(horizontalLayout_4);

        set_all_limit = new QPushButton(widget_2);
        set_all_limit->setObjectName("set_all_limit");

        verticalLayout->addWidget(set_all_limit);


        gridLayout->addLayout(verticalLayout, 1, 0, 1, 1);

        voice_rec = new QLabel(widget_2);
        voice_rec->setObjectName("voice_rec");

        gridLayout->addWidget(voice_rec, 0, 0, 1, 1);


        verticalLayout_2->addWidget(widget_2);

        pushButton = new QPushButton(widget);
        pushButton->setObjectName("pushButton");
        pushButton->setMinimumSize(QSize(400, 400));
        pushButton->setFont(font);

        verticalLayout_2->addWidget(pushButton);


        gridLayout_3->addLayout(verticalLayout_2, 0, 0, 1, 1);


        horizontalLayout_2->addWidget(widget);


        gridLayout_2->addLayout(horizontalLayout_2, 0, 0, 1, 1);

        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName("menubar");
        menubar->setGeometry(QRect(0, 0, 924, 21));
        MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName("statusbar");
        MainWindow->setStatusBar(statusbar);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "MainWindow", nullptr));
        settings->setText(QCoreApplication::translate("MainWindow", "\351\205\215\347\275\256\346\270\205\345\215\225", nullptr));
        label->setText(QCoreApplication::translate("MainWindow", "\350\241\214\346\225\260", nullptr));
        label_2->setText(QCoreApplication::translate("MainWindow", "\345\210\227\346\225\260", nullptr));
        label_3->setText(QCoreApplication::translate("MainWindow", "\350\257\255\351\237\263\350\276\223\345\205\245\350\214\203\345\233\264", nullptr));
        label_8->setText(QCoreApplication::translate("MainWindow", "\345\267\246\344\270\212\350\247\222", nullptr));
        label_4->setText(QCoreApplication::translate("MainWindow", "\350\241\214\345\217\267\357\274\210", nullptr));
        label_5->setText(QCoreApplication::translate("MainWindow", "\357\274\214\345\210\227\345\217\267", nullptr));
        label_6->setText(QCoreApplication::translate("MainWindow", "\357\274\211", nullptr));
        label_7->setText(QCoreApplication::translate("MainWindow", "\345\217\263\344\270\213\350\247\222", nullptr));
        label_9->setText(QCoreApplication::translate("MainWindow", "\350\241\214\345\217\267\357\274\210", nullptr));
        label_10->setText(QCoreApplication::translate("MainWindow", "\357\274\214\345\210\227\345\217\267", nullptr));
        label_11->setText(QCoreApplication::translate("MainWindow", "\357\274\211", nullptr));
        set_all_limit->setText(QCoreApplication::translate("MainWindow", "\350\256\276\347\275\256\344\270\272\345\205\250\351\203\250\345\215\225\345\205\203\346\240\274", nullptr));
        voice_rec->setText(QCoreApplication::translate("MainWindow", "\350\257\206\345\210\253\347\273\223\346\236\234", nullptr));
        pushButton->setText(QCoreApplication::translate("MainWindow", "\346\214\211\344\275\217\350\257\264\350\257\235", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
