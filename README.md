This Software is for an Analog/Digital-Converter circuit. This was done in the use of an electret condenser microphone which got amplified by the LT1638IN8 R-R operational amplifier and finally sampled by the MCP3008P ADC. Under use of the lib wiringPi and wiringPiSPI (Tvm G. Henderson) it was possible to get a samplerate of between 35-40ksps at the receiver. The ADC can propably be much faster but its limited by the UDP MTU. This will get an upgrade in the future under the use of an streaming protocol. 
The core of this project was to build an IoT "Base"system for visualization of audiosignals. Signals getting plotted with "gnuplot" which needs to be installed on your OS. So u can use the pipe to send commands to it.

The sender is actually an Raspberry Pi 4th Gen. and the receiver is an Debian based OS. This software does NOT support Windows atm.

Future features:

Write audio to files (.wav .pcm etc.)
Compression method
IPv6 Support
Windows Support
