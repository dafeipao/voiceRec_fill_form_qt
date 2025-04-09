#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSettings>
#include "col_name_mode_widget.h"
#include "ccopy_tablewidget.h"
#include "VoiceRecorder.h"
#include "VoiceRecorderThread.h"
#include "webiatws.h"
#include "TextProcessor.h"
#include "configdialog.h"
QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void RecProcessText(const QString& text);
    WebIATWS *iat;
    void debug_rec(const QString &text1);
private:
    QPoint point_toCorrect(QPoint point);

    // void RecProcessText();
    QPoint sel_next_pos();

private slots:
    void on_row_num_valueChanged(int arg1);

    void on_col_num_valueChanged(int arg1);

    void on_set_all_limit_clicked();

    void on_left_top_r_textChanged(const QString &arg1);

    void on_left_top_c_textChanged(const QString &arg1);

    void on_right_buttom_r_textChanged(const QString &arg1);

    void on_right_buttom_c_textChanged(const QString &arg1);

    void updateSelection(QPoint point);

    void onSelectionChanged();

    void update_init();
private:
    Ui::MainWindow *ui;
    ccopy_tablewidget *tableWidget;
    QPoint sel_lt;
    QPoint sel_rb;
    QPoint sel_now;
    VoiceRecorderThread *recorderThread;
    TextProcessor *textprocessor;

private slots:
    void openConfigDialog();
};
#endif // MAINWINDOW_H
