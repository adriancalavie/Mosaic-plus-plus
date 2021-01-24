#pragma once
#ifndef QMOSAICTHREAD_H
#define QMOSAICTHREAD_H

#include <QObject>
#include <QThread>
#include <QMutex>
#include <qthread.h>

class QMosaicThread : public QObject
{
    Q_OBJECT

private:
    bool active;
    int progressToggleTerminal;
    int workAmount;
    int workSpeed;
    int workDone;
    QMutex lock;

    void setThreadStatus(QString status);
    void workChunk();

public:
    explicit QMosaicThread(QObject* parent = 0);

signals:
    void sendThreadStatus(QString msg);
    void sendProgress(int workDone, int workSpeed);
    void sendFinished();

public slots:
    void doWork();
    void toggle();
    void receiveWorkAmount(int _workAmount);
    void receiveWorkSpeed(int _workSpeed);
};

#endif // QMOSAICTHREAD_H

