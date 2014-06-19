#include <boost/statechart/event.hpp>
#include <boost/statechart/simple_state.hpp>
#include <boost/statechart/state_machine.hpp>
#include <boost/statechart/transition.hpp>
#include <chrono>
#include <iostream>

struct IGetTime
{
  virtual double time() const = 0;
};

struct EvStartStop
  : boost::statechart::event<EvStartStop>
{};

struct EvReset
  : boost::statechart::event<EvReset>
{};

struct Running;
struct Active;
struct StopWatch
  : boost::statechart::state_machine<StopWatch, Active>
{
  StopWatch()
  {
    initiate();
  }

  double time() const
  {
    return state_cast<const IGetTime&>().time();
  }

  bool isRunning() const
  {
    try {
      state_cast<const Running&>();
    }
    catch (std::bad_cast&) {
      return false;
    }
    return true;
  }

  void doStartStop()
  {
    process_event(EvStartStop());
  }

  void doReset()
  {
    process_event(EvReset());
  }
};

struct Stopped;
struct Active
  : boost::statechart::simple_state<Active, StopWatch, Stopped>
{
public:
  typedef boost::statechart::transition<EvReset, Active> reactions;

  Active()
    : _cumulatedLapTime(0.0)
  {}

  void addLapTime(double lapTime)
  {
    _cumulatedLapTime += lapTime;
  }

  double cumulatedLapTime() const
  {
    return _cumulatedLapTime;
  }

private:
  double _cumulatedLapTime;
};

struct Stopped
  : IGetTime
  , boost::statechart::simple_state<Stopped, Active>
{
public:
  typedef boost::statechart::transition<EvStartStop, Running> reactions;

  virtual double time() const
  {
    return context<Active>().cumulatedLapTime();
  }
};

struct Running
  : IGetTime
  , boost::statechart::simple_state<Running, Active>
{
public:
  typedef boost::statechart::transition<EvStartStop, Stopped> reactions;

  Running()
    : _startTime(std::chrono::steady_clock::now())
  {}

  ~Running()
  {
    context<Active>().addLapTime(_runningTime());
  }

  virtual double time() const
  {
    return context<Active>().cumulatedLapTime() + _runningTime();
  }

 private:
  double _runningTime() const
  {
    return std::chrono::duration_cast<std::chrono::milliseconds>(
        std::chrono::steady_clock::now() - _startTime).count() / 1000;
  }

private:
  std::chrono::time_point<std::chrono::steady_clock> _startTime;
};
