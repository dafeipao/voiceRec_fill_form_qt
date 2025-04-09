#include "VoiceRecorderThread.h"

VoiceRecorderThread::VoiceRecorderThread(QObject *parent)
    : QThread(parent), recorder(nullptr)
{
    recorder = new VoiceRecorder();
}

VoiceRecorderThread::~VoiceRecorderThread()
{
    quit();
    wait();
}

void VoiceRecorderThread::run()
{
    exec(); // 启动事件循环
}

void VoiceRecorderThread::startRecording()
{
    if (!recorder) return;
    QMetaObject::invokeMethod(recorder, "startRecording", Qt::QueuedConnection);
}

void VoiceRecorderThread::stopRecording()
{
    if (!recorder) return;
    QMetaObject::invokeMethod(recorder, "stopRecording", Qt::QueuedConnection);
}
