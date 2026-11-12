# wave-reader

wave-reader is a client application reading three streams of floating point
values from TCP ports 4001-4003. The application also controls the behavior
of one of the streams using a control channel in UDP port 4000. The streams
are printed to standard output in JSON format.

wave-reader contains two different executables:
- `client1` for reading the streams with 100ms intervals
- `client2` for reading the streams with 20ms intervals and controlling stream 1

## How to build

The application uses a standard CMake project structure. To build the project,
run:

```
mkdir -p build && cd build && cmake .. && make
```

wave-reader has been verified to work on x86_64 Linux system with GCC 9.4.0
and CMake 3.16.3.

## Approach

The first objective was to figure out what kind of data the TCP ports were
producing. I started by inspecting the streams with `nc` tool and quickly
noticed that the streams were sine waves and that the different streams
produced the values with varying intervals. Also I noticed that the streams
were not producing the values evenly and there was glitching with timing.

These findings led me to write a very minimal non-blocking C++ wrapper for
TCP sockets. I knew I also had to send UDP datagrams so I made sure that
the same abstraction would work for that as well. Using the wrapper I was able
to read the streams and output them to stdout in JSON format.

At this stage I considered threading or epoll, but the non-blocking read
worked well enough that I decided to keep it simple. If I would have decided
to use threading, I would have needed to introduce locking mechanism and
signaling. On the other hand, if I would have decided to use epoll, I would
have needed to expose the socket file descriptor out of the wrapper.

I initially implemented the sampling and printing intervals with `sleep_for`
function, but it turned out that it caused the intervals to drift. Replacing
it with `sleep_until` fixed the issue nicely, because I could set the wakeup
time precisely when the next interval was due.

I piped the output of the application to a JSON file and exported it to
a spreadsheet so that I could visually plot the signals. From here it was clear
that the streams 1 and 2 were sine waves and stream 3 was square pulse. I
answered the task 1 questionnaire. At the time I believed that stream 3 was
somehow correlated with the other streams, but later (after working with the
control protocol) I found out that it was likely just randomness in the pulse
widths.

The next task was to find out how the control protocol worked. The provided
specification was almost complete, but it was lacking the object properties.
After writing a wrapper for sending UDP datagrams, I searched the entire 16
bit value range for active properties on the server, and found out six
different properties; enabled (14), min_duration (42), max_duration (43),
amplitude (170), frequency (255) and glitch_chance (300). The objects were
self-evidently values 1-3 and it seemed that there were no other objects on
the server.

The rest of the task was just parsing the string representation of the third
stream to a floating point value and based on that sending two control protocol
messages (setting frequency and amplitude) when a certain threshold value was
reached.

## Correctness

How do I know the solution is correct? There are known issues with the
application, that are listed below, so the application is not correct in all
possible scenarios. However, visually inspecting the output of the `nc` tool
and comparing it to the outputs of `client1` and `client2` I have confidence
that for practical purposes the application is correct.

## Ideas and known issues

- Incoming data might be partial - needs buffering
- Use epoll instead of manually checking all sockets for data
- JSON formatting might break if server sends quotes or escape characters
- Socket disconnect is not being handled
- TcpReader and UdpWriter have similar initialization - merge?
- There is no error handling or retry logic for failed control messages
- Floating point conversion of out3 value might break depending on locale

## Task 1

These values were measured using 100ms sampling rate and approximated from data.
Frequency was calculated as the inverse of the period of a full cycle and amplitude
as the maximum deviation from zero.

### out1
- Frequency: 0.5 Hz (period ~2000ms)
- Amplitude: 5
- Shape: Sine wave

### out2
- Frequency: 0.25 Hz (period ~4000ms)
- Amplitude: 5
- Shape: Sine wave

### out3
- Frequency: ? (seems random, but there is some kind of correlation with other waves)
- Amplitude: 5
- Shape: Square pulse
