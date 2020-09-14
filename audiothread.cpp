#include "audiothread.h"
#include <QUrl>


AudioThread::AudioThread(QObject *parent) :
    QObject(parent)
{
    effect=new QSoundEffect;
    thread=new QThread;
    effect->moveToThread(thread);
    connect(this, SIGNAL(doPlay()), effect, SLOT(play()));
    thread->start();
}

AudioThread::~AudioThread()
{
    if(thread->isRunning())
    {
        thread->terminate();
    }
}

void AudioThread::play_pop()
{

    if(!effect->isPlaying())
    {
        effect->setSource(QUrl::fromLocalFile(":/sources/碰撞.wav"));
        effect->setLoopCount(0);
        effect->setVolume(1.0f);
        emit this->doPlay();
    }
}
void AudioThread::play_times(){
    if(!effect->isPlaying())
    {
        effect->setSource(QUrl::fromLocalFile(":/sources/时停.wav"));
        effect->setLoopCount(0);
        effect->setVolume(1.0f);
        emit this->doPlay();
    }
}

bool AudioThread::play(int voice_type)
{
    if(effect->isPlaying())
    {
        return false;
    }
    QString fileName;
    switch (voice_type)
    {
    case empty:
        return true;
    case stone:
    {
        fileName = "石头.wav";
    }
        break;
    case diamond:
    {
        fileName = "钻石.wav";
    }
        break;
    case bomb:
    {
        fileName="爆炸.wav";
    }
        break;
    case pass:
    {
        fileName = "钻石.wav";
    }
        break;
    case timer:
    {
        fileName = "钻石.wav";
    }
        break;
    default:
    {
        return true;
    }
    }
    effect->setSource(QUrl::fromLocalFile(":/sources/"+fileName));
    effect->setLoopCount(0);
    effect->setVolume(1.0f);
    emit this->doPlay();
    return true;
}
