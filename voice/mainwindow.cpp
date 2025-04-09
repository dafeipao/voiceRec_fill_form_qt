#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "Python.h"


#define dout qDebug() << "[" << __FILE__ << " " << __FUNCTION__ << "() " << __LINE__ << " " << "Thread ID: " << QThread::currentThreadId() << "]"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QMenu *menu = menuBar()->addMenu(tr("设置"));

    // 添加 Action
    QAction *configAction = new QAction(tr("配置"), this);
    menu->addAction(configAction);
    connect(configAction, &QAction::triggered, this, &MainWindow::openConfigDialog);

    tableWidget = new ccopy_tablewidget();

    QGridLayout *layout = new QGridLayout();
    ui->widget_tt->setLayout(layout);
    layout->addWidget(tableWidget);

    on_row_num_valueChanged(ui->row_num->value());
    on_col_num_valueChanged(ui->col_num->value());

    sel_now.setX(1);
    sel_now.setY(2);

    connect(tableWidget, &ccopy_tablewidget::selectedPointChanged, this, &MainWindow::updateSelection);
    // connect(tableWidget, &QTableWidget::itemSelectionChanged, this, &MainWindow::onSelectionChanged);//有bug暂时不启用

    QSettings settings("setting.ini", QSettings::IniFormat);

    QString m_hostUrl = settings.value("hostUrl", "").toString();
    QString m_apiKey = settings.value("APIKey", "").toString();
    QString m_apiSecret = settings.value("APISecret", "").toString();
    QString m_appId = settings.value("APPID", "").toString();
    QString namePath=settings.value("namePath", "").toString();

    recorderThread = new VoiceRecorderThread(this);
    connect(ui->pushButton, &QPushButton::pressed, recorderThread, &VoiceRecorderThread::startRecording);
    connect(ui->pushButton, &QPushButton::released, recorderThread, &VoiceRecorderThread::stopRecording);

    iat = new WebIATWS(m_hostUrl,m_apiKey,m_apiSecret,m_appId,this);
    connect(iat,&WebIATWS::finalResultReceived,this,&MainWindow::RecProcessText);

    // QString wavFilePath = "./record_20250310_180656.pcm";
    // QFile wavFile(wavFilePath);
    // if (!wavFile.open(QIODevice::ReadOnly)) {
    //     qWarning() << "无法打开文件:" << wavFilePath;
    // }

    // // 读取整个WAV文件内容（含头部信息）
    // QByteArray audioData = wavFile.readAll();
    // wavFile.close();
    // iat->startRecognition(audioData);


    textprocessor=new TextProcessor(namePath,this);
    // QString text = "张山丰77，王动新，50，王飞扬95,姜玉妹六十6。";
    // text.remove(QRegularExpression("[\\p{P}\\p{S}]"));
    // // 模拟四次调用
    // qDebug() << "模式1(姓名):" << textprocessor->processText("姓名", text);  // 输出：张三
    // qDebug() << "模式2(成绩):" << textprocessor->processText("成绩", text);  // 输出：95
    // qDebug() << "模式3(姓名):" << textprocessor->processText("姓名", text);  // 输出：李四
    // qDebug() << "模式4(成绩):" << textprocessor->processText("成绩", text);  // 输出：66
    // qDebug() << "模式3(姓名):" << textprocessor->processText("姓名", text);  // 输出：李四
    // qDebug() << "模式4(成绩):" << textprocessor->processText("成绩", text);  // 输出：66
    // qDebug() << "模式3(姓名):" << textprocessor->processText("姓名", text);  // 输出：李四
    // qDebug() << "模式4(成绩):" << textprocessor->processText("成绩", text);  // 输出：66

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::openConfigDialog() {
    ConfigDialog dialog(this);
    connect(&dialog, &ConfigDialog::config_saved, this, &MainWindow::update_init);
    dialog.exec();  // 以模态对话框方式打开
}

void MainWindow::update_init() {
    QSettings settings("setting.ini", QSettings::IniFormat);

    QString m_hostUrl = settings.value("hostUrl", "").toString();
    QString m_apiKey = settings.value("APIKey", "").toString();
    QString m_apiSecret = settings.value("APISecret", "").toString();
    QString m_appId = settings.value("APPID", "").toString();
    QString namePath=settings.value("namePath", "").toString();

    iat = new WebIATWS(m_hostUrl,m_apiKey,m_apiSecret,m_appId,this);
    textprocessor =new TextProcessor(namePath,this);
    connect(iat,&WebIATWS::finalResultReceived,this,&MainWindow::RecProcessText);

}


void MainWindow::debug_rec(const QString &text1){
    ui->voice_rec->setText(text1);
}


void MainWindow::RecProcessText(const QString &text1)
{
    qDebug()<<"REC:"<<text1;

    // debug_rec("SWrite: "+text1);
    // QString text = "张山丰77，王动新，50，王霏羊95,姜玉妹六十6。";
    QString text=text1;

    text.remove(QRegularExpression("[\\p{P}\\p{S}]"));
    bool con_flag=true;

    while(con_flag){
        int x_now=sel_now.x();
        int y_now=sel_now.y();

        QWidget *widget = tableWidget->cellWidget(0, x_now-1);
        if (!widget) {
            qDebug() << "Error: widget is nullptr at (0," << x_now-1 << ")";
            break; // 或者 continue，防止崩溃
        }

        auto customWidget = qobject_cast<col_name_mode_widget*>(widget);
        if (!customWidget) {
            qDebug() << "Error: customWidget is nullptr at (0," << x_now-1 << ")";
            break;
        }

        QString mode = customWidget->getCurrentComboBoxText();
        QString res=textprocessor->processText(mode, text);

        if(res!="END_FLAG"){
            QPoint temp=point_toCorrect(sel_now);
            tableWidget->setItem((temp.y()), temp.x(), new QTableWidgetItem(res));
            sel_now=sel_next_pos();
        }else{
            textprocessor->m_currentPos=0;
            con_flag=false;
        }

    }

}

QPoint MainWindow::sel_next_pos()
{
    int width=(ui->col_num->text()).toInt();
    int height=(ui->row_num->text()).toInt();
    QPoint next = sel_now;

    //判断是否在选定区域内
    bool inSelectedArea = (sel_now.x() >= sel_lt.x() && sel_now.x() <= sel_rb.x() &&
                           sel_now.y() >= sel_lt.y() && sel_now.y() <= sel_rb.y());

    if (inSelectedArea) {
        // 在区域内，按照区域遍历逻辑
        if (next.x() < sel_rb.x()) {
            next.setX(next.x() + 1);  // 右移
        } else {
            if (next.y() < sel_rb.y()) {
                next.setX(sel_lt.x()); // 到达边界，换行
                next.setY(next.y() + 1);
            } else {
                // 区域已经遍历完，跳到区域外的下一个点
                if (sel_rb.x() < width) {
                    next.setX(sel_rb.x() + 1);
                    next.setY(sel_rb.y());
                } else if (sel_rb.y() < height+1) {
                    next.setX(1);
                    next.setY(sel_rb.y() + 1);
                } else {
                    next = QPoint(1, 2); // 全部遍历完，返回无效点
                }
            }
        }
    } else {
        // 不在区域内，按照正常扫描逻辑
        if (next.x() < width) {
            next.setX(next.x() + 1);
        } else {
            if (next.y() < height+1) {
                next.setX(1);
                next.setY(next.y() + 1);
            } else {
                next = QPoint(1, 2); // 已经到达最后一个点
            }
        }
    }

    tableWidget->setCellColor(point_toCorrect(sel_lt),point_toCorrect(sel_rb),point_toCorrect(sel_now));

    return next;
}



void MainWindow::on_row_num_valueChanged(int arg1)
{
    this->tableWidget->setRowCount(arg1+1);
    this->tableWidget->resizeColumnsToContents();
}

void MainWindow::on_col_num_valueChanged(int arg1)
{
    this->tableWidget->setColumnCount(arg1);
    for(int i=0;i<arg1;i++){
        col_name_mode_widget* col_item=new col_name_mode_widget();
        this->tableWidget->setCellWidget(0,i,col_item);
    }
    this->tableWidget->update();
    this->tableWidget->resizeRowsToContents();

    for (int col = 0; col < arg1; ++col) {
        QTableWidgetItem *item = this->tableWidget->item(0, col);
        if (!item) {
            item = new QTableWidgetItem();
            this->tableWidget->setItem(0, col, item);
        }
        item->setText("");
        item->setFlags(item->flags() & ~Qt::ItemIsEditable); // 移除可编辑标志
    }
}

QPoint MainWindow::point_toCorrect(QPoint point){
    QPoint p;
    p.setX(point.x()-1);
    p.setY(point.y()-1);
    return p;
}

void MainWindow::updateSelection(QPoint point)
{
    int row=point.y();
    int column=point.x();
    if (row < 0 || column < 0 || row >= tableWidget->rowCount() || column >= tableWidget->columnCount())
        return;

    if (sel_now.x() == column + 1 && sel_now.y() == row + 1) {
        return;
    }

    sel_now.setX(column+1);
    sel_now.setY(row+1);
    tableWidget->setCellColor(point_toCorrect(sel_lt),point_toCorrect(sel_rb),point_toCorrect(sel_now));

    qDebug()<<sel_now;
}

void MainWindow::onSelectionChanged()
{
    QList<QTableWidgetSelectionRange> ranges = tableWidget->selectedRanges();

    int topRow ;
    int bottomRow ;
    int leftColumn ;
    int rightColumn ;

    for (const auto &range : ranges) {
        topRow = range.topRow();
        bottomRow = range.bottomRow();
        leftColumn = range.leftColumn();
        rightColumn = range.rightColumn();
    }

    if((topRow==bottomRow)&&(leftColumn==rightColumn)){
        return;
    }
    else{
        sel_lt.setX(leftColumn+1);
        sel_lt.setY(topRow+1);
        sel_rb.setX(rightColumn+1);
        sel_rb.setY(bottomRow+1);

        // qDebug() << "Selected Range: (" << sel_lt.x() << "," << sel_lt.y() << ") to ("
        //          << sel_rb.x() << "," << sel_rb.y() << ")";
        tableWidget->setCellColor(point_toCorrect(sel_lt),point_toCorrect(sel_rb),point_toCorrect(sel_now));
    }
}

void MainWindow::on_set_all_limit_clicked()
{
    sel_lt.setX(1);
    sel_lt.setY(2);
    sel_rb.setX(ui->col_num->value());
    sel_rb.setY(ui->row_num->value()+1);
    sel_now=sel_lt;

    ui->left_top_r->setText(QString::number(sel_lt.y()));
    ui->left_top_c->setText(QString::number(sel_lt.x()));

    ui->right_buttom_r->setText(QString::number(sel_rb.y()));
    ui->right_buttom_c->setText(QString::number(sel_rb.x()));

    // qDebug()<<sel_lt<<" "<<sel_rb;


    tableWidget->setCellColor(point_toCorrect(sel_lt),point_toCorrect(sel_rb),point_toCorrect(sel_now));
}

void MainWindow::on_left_top_r_textChanged(const QString &arg1)
{
    sel_lt.setY(arg1.toInt());
    tableWidget->setCellColor(point_toCorrect(sel_lt),point_toCorrect(sel_rb),point_toCorrect(sel_now));
}

void MainWindow::on_left_top_c_textChanged(const QString &arg1)
{
    sel_lt.setX(arg1.toInt());
    tableWidget->setCellColor(point_toCorrect(sel_lt),point_toCorrect(sel_rb),point_toCorrect(sel_now));
}


void MainWindow::on_right_buttom_r_textChanged(const QString &arg1)
{
    sel_rb.setY(arg1.toInt());
    tableWidget->setCellColor(point_toCorrect(sel_lt),point_toCorrect(sel_rb),point_toCorrect(sel_now));
}


void MainWindow::on_right_buttom_c_textChanged(const QString &arg1)
{
    sel_rb.setX(arg1.toInt());
    tableWidget->setCellColor(point_toCorrect(sel_lt),point_toCorrect(sel_rb),point_toCorrect(sel_now));
}

