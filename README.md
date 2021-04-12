# TargetTurner
Arduino turning target controller, operates as a web server to allow WiFi control from a tablet or phone. Targets can be controlled simply (i.e. all face, all away), or run complex timed sequences with each target individually controlled.

# Hardware
The firmware is written for an Arduino Mega with an ethernet and MicroSD shield, however the code can be adapted to fit any Arduino and peripherals. As for the targets, there are many options from electric motors to pneumatics. The requried control signals can be produced with minimal code modification.

# Firmware
There are two main parts to the firmware, the server and the GUI. The server is what runs on the Arduino. It has two purposes: serve the GUI files to the client device from the MicroSD; receive command requests to drive the targets. Due to the high latency of processing network events, the entire target sequence is uploaded to the server once which can then be started and stopped at will.

The GUI is stored on the MicroSD card to save space in the limited Arduino storage. Additionally, the entire GUI including JavaScript and CSS are in a single .html file. This simplifies the server and reduces load time.