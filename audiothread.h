#ifndef DLTHREAD_H
#define DLTHREAD_H

#include <QtCore>
#include <QObject>
#include <QSoundEffect>
#include <QThread>

enum Voice_Type{empty,stone,diamond,bomb,pass,timer};

class AudioThread : public QObject
{
    Q_OBJECT
public:
    AudioThread(QObject *parent = nullptr);
    ~AudioThread();
public:
    QSoundEffect *effect;
    QThread *thread;

signals:
    void doPlay();

public slots:
    void play_pop();//播放小球与板相撞的音效
    void play_times();//播放时停音效
    bool play(int type);//播放碎石音效
};

#endif // DLTHREAD_H
