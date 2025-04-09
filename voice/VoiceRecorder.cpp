#include "VoiceRecorder.h"
#include <QAudioFormat>
#include <QAudioDevice>
#include <QMediaDevices>
#include <QDateTime>
#include <QFile>
#include <QDebug>
#include "mainwindow.h"

extern MainWindow *globalMainWindow;

VoiceRecorder::VoiceRecorder(QObject *parent)
    : QObject(parent), audioSource(nullptr), audioDevice(nullptr)
{
}

VoiceRecorder::~VoiceRecorder()
{
    stopRecording();
}

void VoiceRecorder::startRecording()
{
    if (audioSource) return; // 避免重复启动

    QAudioFormat format;
    format.setSampleRate(16000);
    format.setChannelCount(1);
    format.setSampleFormat(QAudioFormat::Int16);

    QAudioDevice device = QMediaDevices::defaultAudioInput();
    audioSource = new QAudioSource(device, format);

    audioBuffer.clear(); // 清空上一次的录音数据
    audioDevice = audioSource->start();

    connect(audioDevice, &QIODevice::readyRead, this, [this]() {
        QByteArray data = audioDevice->readAll(); // 读取音频数据
        audioBuffer.append(data); // 追加到缓冲区
    });

    // qDebug() << "开始录音...";
}

void VoiceRecorder::stopRecording()
{
    if (!audioSource) return;

    audioSource->stop();
    delete audioSource;
    audioSource = nullptr;

    // // 指定保存目录
    // QString saveDir = "./recordings";

    // // 检查目录是否存在，如果不存在则创建
    // QDir dir(saveDir);
    // if (!dir.exists()) {
    //     if (!dir.mkpath(saveDir)) { // mkpath() 可以递归创建目录
    //         qWarning() << "无法创建目录：" << saveDir;
    //         return;
    //     }
    // }

    // // 生成文件名
    // QString fileName = saveDir + "/record_" + QDateTime::currentDateTime().toString("yyyyMMdd_HHmmss") + ".pcm";

    // QFile outputFile(fileName);
    // if (outputFile.open(QIODevice::WriteOnly)) {
    //     outputFile.write(audioBuffer);
    //     outputFile.close();
    //     qDebug() << "录音完成，已保存：" << fileName;
    // } else {
    //     qWarning() << "无法保存文件：" << fileName;
    // }

    // QString wavFilePath = "G://voice//recordings//record_20250318_135327.pcm";
    // QFile wavFile(wavFilePath);
    // if (!wavFile.open(QIODevice::ReadOnly)) {
    //     qWarning() << "无法打开文件:" << wavFilePath;
    // }

    // // 读取整个WAV文件内容（含头部信息）
    // QByteArray audioData = wavFile.readAll();
    // wavFile.close();

    globalMainWindow->iat->startRecognition(audioBuffer);

    audioBuffer.clear(); // 清空缓存
}
