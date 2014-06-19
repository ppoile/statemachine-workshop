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
  {}

  bool isRunning()
  {
    return false;
  }

  double time()
  {
    return 0.0;
  }
};

#endif // #ifndef _STOP_WATCH_H_
