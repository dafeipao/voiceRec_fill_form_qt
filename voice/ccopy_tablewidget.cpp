#include "ccopy_tablewidget.h"

ccopy_tablewidget::ccopy_tablewidget(QTableWidget *parent)
    : QTableWidget(parent) {
    setSelectionMode(QAbstractItemView::ContiguousSelection);
    setSelectionBehavior(QAbstractItemView::SelectItems);
    setContextMenuPolicy(Qt::CustomContextMenu);

    setMinimumSize(400,200);

    connect(this, &QTableWidget::customContextMenuRequested, this, &ccopy_tablewidget::showContextMenu);
}

void ccopy_tablewidget::copyToClipboard() {
    QList<QTableWidgetSelectionRange> ranges = selectedRanges();
    if (ranges.isEmpty()) return;

    QString copiedText;
    bool hasHeader = false;
    QStringList headerContents;

    // 检测是否选择了第 0 行
    for (const auto &range : ranges) {
        if (range.topRow() == 0) {
            hasHeader = true;
            break;
        }
    }

    if (hasHeader) {
        for (int col = 0; col < columnCount(); ++col) {
            QWidget *widget = this->cellWidget(0, col);
            if (widget) {
                // 假设 cellWidget 是一个包含 getCurrentLineEdit() 方法的自定义组件
                auto customWidget = qobject_cast<col_name_mode_widget*>(widget);
                if (customWidget) {
                    headerContents << customWidget->getCurrentLineEdit();
                } else {
                    headerContents << "";
                }
            } else {
                QTableWidgetItem *item = this->item(0, col);
                headerContents << (item ? item->text() : "");
            }
        }
        copiedText += headerContents.join("\t") + "\n";
    }

    // 复制选中的内容
    for (const auto &range : ranges) {
        for (int row = range.topRow(); row <= range.bottomRow(); ++row) {
            if (hasHeader && row == 0) continue; // 已经添加过标题，跳过第 0 行

            QStringList rowContents;
            for (int col = range.leftColumn(); col <= range.rightColumn(); ++col) {
                QTableWidgetItem *item = this->item(row, col);
                rowContents << (item ? item->text() : "");
            }
            copiedText += rowContents.join("\t") + "\n";
        }
    }

    QClipboard *clipboard = QApplication::clipboard();
    clipboard->setText(copiedText);
}

void ccopy_tablewidget::deleteSelectedCells() {
    QList<QTableWidgetSelectionRange> ranges = selectedRanges();
    if (ranges.isEmpty()) return;

    for (const auto &range : ranges) {
        for (int row = range.topRow(); row <= range.bottomRow(); ++row) {
            for (int col = range.leftColumn(); col <= range.rightColumn(); ++col) {
                QTableWidgetItem *item = this->item(row, col);
                if (item) {
                    item->setText(""); // 清空单元格文本
                }
            }
        }
    }
}


void ccopy_tablewidget::keyPressEvent(QKeyEvent *event) {
    if (event->matches(QKeySequence::Copy)) {
        copyToClipboard();
    } else if (event->key() == Qt::Key_Delete) {
        deleteSelectedCells();
    }else {
        QTableWidget::keyPressEvent(event);
    }
}

void ccopy_tablewidget::contextMenuEvent(QContextMenuEvent *event) {
    QMenu menu(this);
    QAction *copyAction = menu.addAction("复制");
    connect(copyAction, &QAction::triggered, this, &ccopy_tablewidget::copyToClipboard);
    menu.exec(event->globalPos());
}

void ccopy_tablewidget::showContextMenu(const QPoint &pos) {
    QMenu menu(this);
    QAction *copyAction = menu.addAction("复制");
    QAction *deleteAction = menu.addAction("删除");
    QAction *selectPointAction  = menu.addAction("设置开始位置");

    connect(copyAction, &QAction::triggered, this, &ccopy_tablewidget::copyToClipboard);
    connect(deleteAction, &QAction::triggered, this, &ccopy_tablewidget::deleteSelectedCells);
    connect(selectPointAction , &QAction::triggered, this, &ccopy_tablewidget::setStartPosition);

    menu.exec(mapToGlobal(pos));
}

void ccopy_tablewidget::setStartPosition() {
    QList<QTableWidgetSelectionRange> ranges = selectedRanges();
    if (ranges.isEmpty()) return; // 如果没有选中任何单元格，直接返回

    int row = ranges.first().topRow();  // 取选区的左上角
    int col = ranges.first().leftColumn();

    QPoint selectedPoint(col, row); // 转换为 QPoint
    emit selectedPointChanged(selectedPoint); // 发送信号
}


void ccopy_tablewidget::setCellColor(QPoint sel_lt, QPoint sel_rb) {
    int rows = rowCount();
    int cols = columnCount();

    // 定义颜色
    QColor lightColor(255, 255, 255);
    QColor darkColor(240, 240, 240);

    // 遍历所有单元格，设置颜色
    for (int row = 0; row < rows; ++row) {
        for (int col = 0; col < cols; ++col) {
            QTableWidgetItem *item = this->item(row, col);
            if (!item) {
                item = new QTableWidgetItem();
                setItem(row, col, item);
            }

            // 判断是否在选中的区域内
            if (row >= sel_lt.y() && row <= sel_rb.y() &&
                col >= sel_lt.x() && col <= sel_rb.x()) {
                item->setBackground(darkColor);  // 选中的区域变深
            } else {
                item->setBackground(lightColor); // 其他区域变浅
            }
        }
    }
}

void ccopy_tablewidget::setCellColor(QPoint sel_lt, QPoint sel_rb, QPoint sel_now) {
    int rows = rowCount();
    int cols = columnCount();

    // 定义颜色
    QColor lightColor(255, 255, 255);
    QColor darkColor(240, 240, 240);
    QColor selColor(180, 180, 180);

    for (int row = 0; row < rows; ++row) {
        for (int col = 0; col < cols; ++col) {
            // 跳过 sel_now 位置，避免它被 darkColor 覆盖
            if (row == sel_now.y() && col == sel_now.x()) {
                continue;
            }

            QTableWidgetItem *item = this->item(row, col);
            if (!item) {
                item = new QTableWidgetItem();
                setItem(row, col, item);
            }

            // 选中区域
            if (row >= sel_lt.y() && row <= sel_rb.y() &&
                col >= sel_lt.x() && col <= sel_rb.x()) {
                item->setBackground(darkColor);
            } else {
                item->setBackground(lightColor);
            }
        }
    }

    // **单独处理 sel_now**
    QTableWidgetItem *selItem = this->item(sel_now.y(), sel_now.x());
    if (!selItem) {  // 确保不会崩溃
        selItem = new QTableWidgetItem();
        setItem(sel_now.y(), sel_now.x(), selItem);
    }
    selItem->setBackground(selColor);
}

void ccopy_tablewidget::setCellColor(QPoint sel_now) {
    int row = sel_now.y();
    int col = sel_now.x();

    if (row >= 0 && row < rowCount() && col >= 0 && col < columnCount()) {
        QTableWidgetItem *item = this->item(row, col);
        if (!item) {
            item = new QTableWidgetItem();
            setItem(row, col, item);
        }
        item->setBackground(QColor(0, 0, 0));
    }
}

