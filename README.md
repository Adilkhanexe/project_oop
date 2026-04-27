# Collaborative Drawing Application

Real-time collaborative drawing app over LAN 
with client-server architecture.

## Features
- Real-time drawing synchronized across LAN clients
- Drawing tools: pen, line, eraser
- Color picker
- Clear canvas
- Client-server architecture using TCP/IP sockets

## Tech Stack
- C++
- Embarcadero RAD Studio
- TCP/IP Sockets

## Architecture
- Server manages connections and broadcasts 
  drawing data to all connected clients
- Each client sends drawing events to the server 
  in real-time

## How to Run
1. Open the project in Embarcadero RAD Studio
2. Build and run the Server application
3. Build and run one or more Client applications
4. Connect clients to the server via local IP address
