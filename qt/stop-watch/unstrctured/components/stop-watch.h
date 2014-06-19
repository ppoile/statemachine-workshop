#ifndef _STOP_WATCH_H_
#define _STOP_WATCH_H_

#include <iostream>
#include <QObject>
#include <QStateMachine>
#include <QTime>

class StopWatch
  : public QObject
{
  Q_OBJECT

public:
  StopWatch()
    : _machine(this)
    , _active(&_machine)
    , _stopped(&_active)
    , _running(&_active)
    , _cumulatedLapTime()
    , _startTime()
  {
    _active.setInitialState(&_stopped);
    _machine.setInitialState(&_active);

    _active.addTransition(this, SIGNAL(_signalReset()), &_active);
    _stopped.addTransition(this, SIGNAL(_signalStartStop()), &_running);
    _running.addTransition(this, SIGNAL(_signalStartStop()), &_stopped);

    QObject::connect(&_active, SIGNAL(entered()), this, SLOT(_onEnteringActive()));
    QObject::connect(&_running, SIGNAL(entered()), this, SLOT(_onEnteringRunning()));
    QObject::connect(&_running, SIGNAL(exited()), this, SLOT(_onExitingRunning()));

    _machine.start();
  }

  bool isRunning()
  {
    return _machine.configuration().contains(&_running);
  }

  double time()
  {
    QTime totalTime = _cumulatedLapTime.addMSecs(_startTime.elapsed());
    return static_cast<double>(totalTime.msecsSinceStartOfDay()) / 1000;
  }

signals:
  void _signalStartStop();
  void _signalReset();

public slots:
  void doStartStop()
  {
    emit _signalStartStop();
  }

  void doReset()
  {
    emit _signalReset();
  }

private slots:
  void _onEnteringActive()
  {
    _resetCumulatedLapTime();
  }

  void _onEnteringRunning()
  {
    _startTime.start();
  }

  void _onExitingRunning()
  {
    _addLapTimeInMSecs(_startTime.elapsed());
    _startTime = QTime();
  }

private:
  void _resetCumulatedLapTime()
  {
    _cumulatedLapTime = QTime(0, 0);
  }

  void _addLapTimeInMSecs(int timeInMSecs)
  {
    _cumulatedLapTime = _cumulatedLapTime.addMSecs(timeInMSecs);
  }

private:
  QStateMachine _machine;
  QState _active;
  QState _stopped;
  QState _running;
  QTime _cumulatedLapTime;
  QTime _startTime;
};

#endif // #ifndef _STOP_WATCH_H_
