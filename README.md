# Test Bandwidth

This is a very simple and very rough from various sources bandwidth test for ADB reverse 
connection
Primarily made for checking maximum possible bandwidth for reverse tcp connection through USB cable

To test:
1. Prepare and run server - `g++ server/server.cpp`, `./a.out`
2. Connect target device, make sure it's visible in `adb devices`
3. Run `adb reverse tcp:9757 tcp:9757` - it should display `9757` in return
4. Run app on headset - easiest way to do it is via Android Studio
5. Server should say that client is connected, wait from 5 to X (any) number of seconds.
6. Stop server with Ctrl + C (in terminal), it should display final bandwidth calculated.
