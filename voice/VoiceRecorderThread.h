#ifndef VOICERECORDERTHREAD_H
#define VOICERECORDERTHREAD_H

#include <QThread>
#include "VoiceRecorder.h"

class VoiceRecorderThread : public QThread
{
    Q_OBJECT

public:
    explicit VoiceRecorderThread(QObject *parent = nullptr);
    ~VoiceRecorderThread();

    void run() override;
    void startRecording();
    void stopRecording();

private:
    VoiceRecorder *recorder;
};

#endif // VOICERECORDERTHREAD_H
