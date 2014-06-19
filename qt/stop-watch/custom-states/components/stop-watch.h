#ifndef _STOP_WATCH_H_
#define _STOP_WATCH_H_

#include <iostream>
#include <QObject>
#include <QStateMachine>
#include <QTime>

class Active
  : public QState
{
public:
  Active(QState *parent)
    : QState(parent)
    , _cumulatedLapTimeInSecs()
  {}

  double cumulatedLapTime() const
  {
    return _cumulatedLapTimeInSecs;
  }

  void resetCumulatedLapTime()
  {
    _cumulatedLapTimeInSecs = 0.0;
  }

  void addLapTime(double timeInSecs)
  {
    _cumulatedLapTimeInSecs += timeInSecs;
  }

protected:
  virtual void onEntry(QEvent *)
  {
    resetCumulatedLapTime();
  }

private:
  double _cumulatedLapTimeInSecs;
};

class Running
  : public QState
{
public:
  Running(QState *parent)
    : QState(parent)
    , _startTime()
  {}

  double time() const
  {
    return static_cast<double>(_startTime.elapsed()) / 1000;
  }

protected:
  virtual void onEntry(QEvent *)
  {
    _startTime.start();
  }

  virtual void onExit(QEvent *)
  {
    static_cast<Active*>(parentState())->addLapTime(time());
    _startTime = QTime();
  }

private:
  QTime _startTime;
};

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
  {
    _active.setInitialState(&_stopped);
    _machine.setInitialState(&_active);

    _active.addTransition(this, SIGNAL(_signalReset()), &_active);
    _stopped.addTransition(this, SIGNAL(_signalStartStop()), &_running);
    _running.addTransition(this, SIGNAL(_signalStartStop()), &_stopped);

    _machine.start();
  }

  bool isRunning()
  {
    return _machine.configuration().contains(&_running);
  }

  double time()
  {
    return _active.cumulatedLapTime() + _running.time();
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

private:
  QStateMachine _machine;
  Active _active;
  QState _stopped;
  Running _running;
};

#endif // #ifndef _STOP_WATCH_H_
