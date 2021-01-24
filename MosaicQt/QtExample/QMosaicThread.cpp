#include "QMosaicThread.h"

#include <iostream>
#include <QMutexLocker>

QMosaicThread::QMosaicThread(QObject* parent) :
    QObject(parent),
    active(false),
    progressToggleTerminal(false),
    workAmount(0),
    workDone(0),
    workSpeed(10)
{
    //EMPTY
}

void QMosaicThread::setThreadStatus(QString status)
{
    emit sendThreadStatus(status);
}

void QMosaicThread::doWork()
{
    
    if (active)
    {
        setThreadStatus("Working");
        // Do work
        workChunk();
    }
    else setThreadStatus("    Idle    ");
}

void QMosaicThread::workChunk()
{
    QThread::msleep(workSpeed);
    if (workDone < workAmount)
    {
        workDone++;
        emit sendProgress(workDone, workSpeed);
    }
    else
    {
        emit sendFinished();
        active = false;
    }
}

void QMosaicThread::toggle()
{
    QMutexLocker locker(&lock);
    active = !active;

    if (workDone == workAmount)
    {
        workDone = 0;
        emit sendProgress(workDone, workSpeed);
    }
}

void QMosaicThread::receiveWorkAmount(int _workAmount)
{
    workAmount = _workAmount;
    workDone = 0;
    active = false;
    emit sendFinished();
}

void QMosaicThread::receiveWorkSpeed(int _workSpeed)
{
    QMutexLocker locker(&lock);
    workSpeed = _workSpeed;
}