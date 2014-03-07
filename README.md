appRTCDesk
==========

A Desktop client to appRTC

## Description
This is a desktop client for appRTC. It connects to apprtc.appspot.com. It is based on the peerconnection_client that is provided in the reference code. It uses Qt4.

The talk/AppRTCClient directory contains the code for the client.

The code is not production ready, and is provided for experimentation and educational purpose.

## First Integration (ONLY ONCE)
TO DO

## Building
in "trunk":
```
gclient runhooks
ninja -C out/Debug   appRTCdesk
ninja -C out/Release appRTCdesk
```

## HOW TO USE
Write the room number in the text area.
Press Enter or click on Connect. 
If no error message is issued you should be able to see yourself. 
When someone connects to your room he will appears on the right. 
To come back to the connecting window press ESC.
 
## SUPPORTED
This example should build and run on Windows/Ubuntu

## KNOWN ISSUE
Once someone is connected, clearing the peerconnection by disconnecting is throwing an error.
- https://code.google.com/p/webrtc/issues/detail?id=2733
