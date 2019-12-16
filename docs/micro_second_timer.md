

quick::MicroSecondTimer
--------------------------
Defined in header `<quick/time.hpp>`


`quick::MicroSecondTimer` is a timer utility useful for measuring the time taken in the processing of a task.

Member Functions
-----------------------------------

## MicroSecondTimer::MicroSecondTimer()
- Creates default instance of DebugStream. Stores the current epoch time in milliseconds.


## MicroSecondTimer::GetElapsedTime() const
- Returns the `current epoch time` - `timer start time`.

## MicroSecondTimer::Restart()
- Sets the `timer start time` to `current epoch time`.

## MicroSecondTimer::GetStartTime() const
- Returns the `timer start time`.


Test Case
-------------------
- [Unit Tests](../tests/time_test.cpp)

