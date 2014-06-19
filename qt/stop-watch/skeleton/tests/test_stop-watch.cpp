#include <QtTest/QtTest>
#include "stop-watch.h"
#include <unistd.h>

class TestStopWatch
  : public QObject
{
    Q_OBJECT

private slots:
  void verify_inital_state();
  //void verify_start_while_stopped();
  //void verify_stop_while_running();
  //void verify_reset_while_running();
  //void verify_reset_while_stopped();
};

void TestStopWatch::verify_inital_state()
{
  StopWatch stop_watch;
  QCoreApplication::processEvents();

  QCOMPARE(stop_watch.isRunning(), false);
  QCOMPARE(stop_watch.time(), 0.0);
  sleep(1);
  QCOMPARE(stop_watch.time(), 0.0);
}

//void TestStopWatch::verify_start_while_stopped()
//{
//  StopWatch stop_watch;
//  QCoreApplication::processEvents();
//
//  stop_watch.doStartStop(); // start
//
//  QCOMPARE(stop_watch.isRunning(), true);
//  double firstTime = stop_watch.time();
//  QTRY_VERIFY(stop_watch.time() != firstTime);
//}
//
//void TestStopWatch::verify_stop_while_running()
//{
//  StopWatch stop_watch;
//  QCoreApplication::processEvents();
//  stop_watch.doStartStop(); // start
//  QTRY_VERIFY(stop_watch.time() != 0.0);
//
//  stop_watch.doStartStop(); // stop
//
//  QCOMPARE(stop_watch.isRunning(), false);
//  double firstTime = stop_watch.time();
//  sleep(1);
//  QCOMPARE(stop_watch.time(), firstTime);
//}
//
//void TestStopWatch::verify_reset_while_running()
//{
//  StopWatch stop_watch;
//  QCoreApplication::processEvents();
//  stop_watch.doStartStop();  // start
//  QTRY_VERIFY(stop_watch.time() != 0.0);
//
//  stop_watch.doReset();
//
//  QCOMPARE(stop_watch.isRunning(), false);
//  QCOMPARE(stop_watch.time(), 0.0);
//  sleep(1);
//  QCOMPARE(stop_watch.time(), 0.0);
//}
//
//void TestStopWatch::verify_reset_while_stopped()
//{
//  StopWatch stop_watch;
//  QCoreApplication::processEvents();
//  stop_watch.doStartStop(); // start
//  QTRY_VERIFY(stop_watch.time() != 0.0);
//  stop_watch.doStartStop(); // stop
//
//  stop_watch.doReset();
//
//  QCOMPARE(stop_watch.isRunning(), false);
//  QCOMPARE(stop_watch.time(), 0.0);
//  sleep(1);
//  QCOMPARE(stop_watch.time(), 0.0);
//}

QTEST_MAIN(TestStopWatch)
#include "test_stop-watch.moc" // when no corresponding header present
