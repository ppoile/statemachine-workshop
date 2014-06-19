#include <boost/statechart/custom_reaction.hpp>
#include <boost/statechart/event.hpp>
#include <boost/statechart/simple_state.hpp>
#include <boost/statechart/state_machine.hpp>
#include <boost/statechart/transition.hpp>
#include <chrono>
#include <iostream>

struct EvStartStop
  : boost::statechart::event<EvStartStop>
{};

struct EvReset
  : boost::statechart::event<EvReset>
{};

struct EvGetTime
  : boost::statechart::event<EvGetTime>
{
  public:
    EvGetTime(double &time)
      : _time(time)
    {}

    void assign(double time) const
    {
      _time = time;
    }

  private:
    double &_time;
};

struct EvIsRunning
  : boost::statechart::event<EvIsRunning>
{
  public:
    EvIsRunning(bool &isRunning)
      : _isRunning(isRunning)
    {}

    void assign(bool isRunning) const
    {
      _isRunning = isRunning;
    }

  private:
    bool &_isRunning;
};

struct Running;
struct Active;
struct StopWatch
  : boost::statechart::state_machine<StopWatch, Active>
{
  StopWatch()
  {
    initiate();
  }

  bool isRunning()
  {
    bool isRunning;
    process_event(EvIsRunning(isRunning));
    return isRunning;
  }

  double time()
  {
    double time;
    process_event(EvGetTime(time));
    return time;
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
  typedef boost::mpl::list<
    boost::statechart::custom_reaction<EvIsRunning>,
    boost::statechart::custom_reaction<EvGetTime>,
    boost::statechart::transition<EvReset, Active>
  > reactions;

  Active()
    : _cumulatedLapTime(0.0)
  {}

  boost::statechart::result react(const EvIsRunning &event)
  {
    event.assign(false);
    return discard_event();
  }

  boost::statechart::result react(const EvGetTime &event)
  {
    event.assign(cumulatedLapTime());
    return discard_event();
  }

  double cumulatedLapTime() const
  {
    return _cumulatedLapTime;
  }

  void addLapTime(double lapTime)
  {
    _cumulatedLapTime += lapTime;
  }

private:
  double _cumulatedLapTime;
};

struct Stopped
  : boost::statechart::simple_state<Stopped, Active>
{
public:
  typedef boost::statechart::transition<EvStartStop, Running> reactions;
};

struct Running
  : boost::statechart::simple_state<Running, Active>
{
public:
  typedef boost::mpl::list<
    boost::statechart::custom_reaction<EvIsRunning>,
    boost::statechart::custom_reaction<EvGetTime>,
    boost::statechart::transition<EvStartStop, Stopped>
  > reactions;

  Running()
    : _startTime(std::time(0))
  {}

  ~Running()
  {
    context<Active>().addLapTime(_runningTime());
  }

  boost::statechart::result react(const EvIsRunning &event)
  {
    event.assign(true);
    return discard_event();
  }

  boost::statechart::result react(const EvGetTime &event)
  {
    event.assign(_time());
    return discard_event();
  }

 private:
  virtual double _time() const
  {
    return context<Active>().cumulatedLapTime() + _runningTime();
  }

  double _runningTime() const
  {
    return std::difftime(std::time(0), _startTime);
  }

private:
  std::time_t _startTime;
};
