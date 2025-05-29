# KeepAlive

A Windows application that prevents the system from sleeping. It runs in the system tray and allows you to set a timer for how long you want to prevent sleep mode.

## Features

- System tray icon with context menu
- Multiple duration options (30 minutes, 1 hour, 2 hours, 4 hours, 8 hours)
- Shows remaining time in the tray icon tooltip
- Prevents system sleep while active
- Single instance check
- Automatic sleep prevention when any application is in fullscreen mode

## Building

### Prerequisites

- CMake (version 3.10 or higher)
- Visual Studio with C/C++ development tools
- Windows SDK

### Build Steps

1. Create a build directory:
```bash
mkdir build
cd build
```

2. Configure the project:
```bash
cmake ..
```

3. Build the project:
```bash
cmake --build .
```

The executable will be created in the `build` directory.

## Usage

1. Run the application:
```bash
./KeepAlive
```

2. The application will appear in the system tray. Right-click the icon to:
   - View current status
   - Start/Stop the timer
   - Select duration
   - Exit the application

3. The tray icon tooltip will show the remaining time when active.

## License

This project is licensed under the MIT License - see the LICENSE file for details. 