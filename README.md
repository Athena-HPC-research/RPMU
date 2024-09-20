# RPMU
RPMU (Raspberry Power Measurement Utility) is a tool meant for measuring and tracking the power usage of the raspberry pi 5 CPU.

It has the following modes:
1) Terminal
2) Background

Terminal mode is meant for debugging, it will constantly display the volts and the ampers the CPU is consuming with a 4 decimal digit accuracy.

Example
```sh
./rpmu
```

Background mode is meant for prolonged measurements for measuring the power usage for benchmarking purposes. It requires two parameters, the name of the output file and the length of time to run. The background mode runs as a daemon.

The following example demonstrated how to run the RPMU program for 120 minutes. This example will output 120 files (so that the memory usage stays as low as possible) named file0 all the way to file119. Each file records a single minute of time with 60 measurements each.

Example
```sh
./rpmu file 120
```