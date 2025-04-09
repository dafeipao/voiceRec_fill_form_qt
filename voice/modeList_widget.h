#ifndef MODELIST_WIDGET_H
#define MODELIST_WIDGET_H

#include<QWidget>
#include<QLayout>
#include<QMap>
class modeList_widget :public QWidget{
    Q_OBJECT
public:
    explicit  modeList_widget(QWidget *parent=nullptr);

    QMap<int,QString> item_mode;
    QVBoxLayout *mainLayout;


};



#endif // MODELIST_WIDGET_H
