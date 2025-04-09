#ifndef CCOPY_TABLEWIDGET_H
#define CCOPY_TABLEWIDGET_H

#include <QTableWidget>
#include <QApplication>
#include <QClipboard>
#include <QKeyEvent>
#include <QMenu>
#include "col_name_mode_widget.h"
class ccopy_tablewidget : public QTableWidget {
    Q_OBJECT

public:
    explicit ccopy_tablewidget(QTableWidget *parent = nullptr);
    void setCellColor(QPoint sel_lt, QPoint sel_rb);
    void setCellColor(QPoint sel_now);
    void setCellColor(QPoint sel_lt, QPoint sel_rb,QPoint sel_now);
protected:
    void keyPressEvent(QKeyEvent *event) override;
    void contextMenuEvent(QContextMenuEvent *event) override;

signals:
    void selectedPointChanged(QPoint point);

private:
    void copyToClipboard();
    void deleteSelectedCells();
    void showContextMenu(const QPoint &pos);
    void setStartPosition();
};

#endif // CCOPY_TABLEWIDGET_H
