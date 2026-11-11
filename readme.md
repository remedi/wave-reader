// TODO, how to build and run

Ideas:
- Incoming data might be partial - needs buffering
- Use epoll instead of manually checking all sockets for data
- JSON formatting might break if server sends quotes or escape characters
- Socket disconnect is not being handled
- TcpReader and UdpWriter have similar initialization - merge?

# Task 1

These values were measured using 100ms sampling rate and approximated from data.
Frequency was calculated as the inverse of the period of a full cycle and amplitude
as the maximum deviation from zero.

## out1
- Frequency: 0.5 Hz (period ~2000ms)
- Amplitude: 5
- Shape: Sine wave

## out2
- Frequency: 0.25 Hz (period ~4000ms)
- Amplitude: 5
- Shape: Sine wave

## out3
- Frequency: ? (seems random, but there is some kind of correlation with other waves)
- Amplitude: 5
- Shape: Square pulse
