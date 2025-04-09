#ifndef VOICERECORDER_H
#define VOICERECORDER_H
#include <QDir>
#include <QStandardPaths>
#include <QObject>
#include <QAudioSource>
#include <QByteArray>
#include <QBuffer>

class VoiceRecorder : public QObject
{
    Q_OBJECT

public:
    explicit VoiceRecorder(QObject *parent = nullptr);
    ~VoiceRecorder();

public slots:
    void startRecording();
    void stopRecording();

private:
    QAudioSource *audioSource;
    QIODevice *audioDevice;
    QByteArray audioBuffer; // 存储录音数据
};

#endif // VOICERECORDER_H
