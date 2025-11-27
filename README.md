# 🔐 Crypto-Com

**A cryptographic communication system with real-time encryption, binary encoding visualization, and network transmission over TCP sockets.**

---

## 📖 Overview

Crypto-Com is a C++ educational project that demonstrates:

- **Symmetric encryption** (XOR cipher with configurable key)
- **Binary encoding** (text ↔ 8-bit binary conversion)
- **Line encoding algorithms** (NRZ and RZ visualization)
- **Network communication** (binary-safe TCP sockets with 8-byte length prefix)
- **Real-time GUI** (ImGui + OpenGL 3.3 Core)
- **Interactive visualization** (waveforms and signal graphs)

---

## 📥 Installing Dependencies

### Ubuntu / Debian

Open a terminal and run the following commands:

```bash
# Update package list
sudo apt update && sudo apt upgrade -y

# Install build dependencies
sudo apt install -y build-essential git pkg-config \
    libglfw3-dev libgl1-mesa-dev libglu1-mesa-dev libx11-dev \
    libxrandr-dev libxinerama-dev libxcursor-dev libxi-dev
```

Verify the installation:
```bash
pkg-config --modversion glfw3
```

### Fedora

Open a terminal and run:

```bash
# Update system
sudo dnf update -y

# Install build dependencies
sudo dnf install -y @development-tools git cmake pkgconfig \
    glfw-devel mesa-libGL-devel libX11-devel libXrandr-devel \
    libXinerama-devel libXcursor-devel libXi-devel
```

Verify the installation:
```bash
pkg-config --modversion glfw3
```

### Arch Linux

Open a terminal and run:

```bash
# Update system
sudo pacman -Syu

# Install build dependencies
sudo pacman -S --needed base-devel git cmake pkg-config \
    glfw-x11 mesa libx11 libxrandr libxinerama libxcursor libxi
```

Verify the installation:
```bash
pkg-config --modversion glfw3
```

### Windows (WSL - Recommended)

#### Step 1: Install WSL

**Windows 11:**
Open PowerShell as Administrator and run:
```powershell
wsl --install
```

**Windows 10:**
Follow the official guide: https://docs.microsoft.com/en-us/windows/wsl/install

#### Step 2: Open WSL and Install Dependencies

Open Ubuntu (WSL) and run:
```bash
sudo apt update && sudo apt upgrade -y

sudo apt install -y build-essential git pkg-config \
    libglfw3-dev libgl1-mesa-dev libglu1-mesa-dev libx11-dev \
    libxrandr-dev libxinerama-dev libxcursor-dev libxi-dev
```

### Windows (Native - MSYS2/MinGW)

#### Step 1: Install MSYS2

Download from: https://www.msys2.org/ and follow the installer.

#### Step 2: Open MSYS2 MinGW 64-bit Shell

Look for "MSYS2 MinGW 64-bit" in the Start Menu.

#### Step 3: Install Dependencies

In the MSYS2 terminal, run:

```bash
# Update MSYS2
pacman -Syu
pacman -Su

# Install dependencies
pacman -S --needed mingw-w64-x86_64-toolchain mingw-w64-x86_64-glfw \
    mingw-w64-x86_64-pkg-config mingw-w64-x86_64-mesa
```

---

## 🚀 Quick Start

### Clone and Compile

**Linux/WSL:**
```bash
# Clone repository
git clone https://github.com/RicardoAffonso0607/Data-Communication.git
cd Data-Communication

# Compile
make clean 

# Run (two terminals)
make receiver  # Terminal 1 (Receiver)
make sender    # Terminal 2 (Sender)
```

**Windows (Native MSYS2):**
```bash
# Clone repository
cd /c/Users/<YourUsername>
git clone https://github.com/RicardoAffonso0607/Data-Communication.git
cd Data-Communication

# Adjust Makefile for MinGW
sed -i 's/-lGL/-lopengl32/g' Makefile

# Compile
make clean

# Run (two terminals)
make receiver  # Terminal 1 (Receiver)
make sender    # Terminal 2 (Sender)
```

---

## 🎯 How to Use

Open the receiver first, so you can connect with the sender after that.

### Sender Mode

1. Open the application: `make sender`
2. Type a message in the **"Original Message"** field.
3. Observe real-time automatic updates:
   - **Encrypted Message** (XOR-encrypted text)
   - **Binary Message** (0s and 1s)
   - **NRZ Message** (+ for 1, 0 for 0)
   - **RZ Message** (+0 for 1, -0 for 0)
4. View the graphs:
   - Encryption waveform graph
   - NRZ encoding graph
   - RZ encoding graph
5. Click **"Send"** to transmit.

### Receiver Mode

1. The receiver automatically listens on the configured port.
2. When a message arrives, you'll see: **"✓ Message Received!"**
3. View the received data:
   - Binary message
   - NRZ encoding
   - RZ encoding
4. Click **"Decrypt"** to reveal the original message.

### Local Test

```bash
# Terminal 1: Start Receiver
make receiver

# Terminal 2: Start Sender
make sender

# In Sender: Type "Hello World", click Send
# In Receiver: Watch message arrive, click Decrypt, see decrypted text
```

### Test on Different Computers

Edit `config.txt`:
```
RECEIVER_IP=<RECEIVER_COMPUTER_IP>
PORT=<RECEIVER_COMPUTER_IP>
```

Then configure firewall and routing (see Troubleshooting section).

---

## 🔧 Make Commands

```bash
make all              # Compile the entire project
make clean            # Remove compiled files
make sender           # Run sender instance
make receiver         # Run receiver instance
make run-both         # Run both (receiver in background)
make killall-app      # Kill all running instances
```

---

## ��️ Project Structure

```
Data-Communication/
├── src/
│   ├── main.cpp                      # Entry point
│   ├── cripto/Crypto.cpp             # XOR encryption & binary conversion
│   ├── socket/Socket.cpp             # TCP communication (binary-safe)
│   ├── encoder/Encoder.cpp           # NRZ/RZ encoding
│   └── window_manager/WindowManager.cpp  # GUI and orchestration
├── include/
│   ├── cripto/Crypto.h
│   ├── socket/Socket.h
│   ├── encoder/Encoder.h
│   ├── window_manager/WindowManager.h
│   └── config/Config.h               # Configuration parser
├── imgui/                            # ImGui library (UI framework)
├── glad/                             # GLAD OpenGL loader
├── config.txt                        # Runtime configuration
├── Makefile
├── README.md                         # This file 
└── build/                            # Compiled artifacts
    └── meu_programa                  # Executable
```

---

## ✨ Features

### Core Functionality
- **Symmetric Encryption** — XOR cipher with configurable key ("mykey123")
- **Binary Encoding** — Text ↔ 8-bit binary conversion
- **Line Encoding** — NRZ and RZ algorithms with visual graphs
- **Network Communication** — TCP sockets with binary-safe framing (8-byte length prefix)
- **Real-time GUI** — ImGui with OpenGL 3.3 Core rendering

### User Interaction
- **Editable Fields** — Modify Original, Encrypted, or Binary messages; all others auto-update
- **Clear-All Behavior** — Empty one field, all dependent fields clear
- **Live Visualization** — Waveform and encoding graphs update in real-time

### Configuration
- **No Recompilation** — Edit `config.txt` to change IP/port

---

## 🔒 Security Notes

- **Encryption**: XOR is a simple, symmetric cipher. Not suitable for real-world security.
- **Key Management**: The key "mykey123" is hardcoded. 
- **Network**: TCP data is transmitted without encryption beyond the XOR cipher. Use a VPN or TLS wrapper for sensitive data on untrusted networks.

---

## 🧪 Testing

### Local Test
```bash
# Terminal 1
make receiver

# Terminal 2
make sender

# In Sender UI: Type "Test Message", click Send
# In Receiver UI: Watch it arrive, click Decrypt
```

### Network Test (Different Networks)

1. Find the Receiver machine's IP: `hostname -I`
2. Edit `config.txt` on the Sender machine
3. Open port 4444 in the firewall (see Troubleshooting section)

### Debug
Run with logs:
```bash
./build/meu_programa 2>&1 | tee run.log
```
Then check `run.log` for errors.

---

## 🐛 Troubleshooting

### Compilation Issues
- **"cannot find -lglfw"** → Install `libglfw3-dev` (Linux) or GLFW via MSYS2 (Windows)
- **GLAD/OpenGL errors** → Ensure GPU drivers and Mesa/OpenGL libraries are installed
- **Font not found** → Roboto-Regular.ttf missing from assets; app uses default font

### Runtime Issues
- **"Failed to connect"** → Check `config.txt` IP/port, firewall rules
- **No GUI window** → On Windows 10 WSL without WSLg, configure X server (VcXsrv)
- **Truncated messages** → Protocol uses 8-byte prefix; if still truncated, check Socket.cpp

---

## 📦 Dependencies

### Build-Time
- **C++17 compiler** (g++, clang, MSVC)
- **pkg-config** (for finding libraries)
- **make** (build automation)

### Runtime
- **GLFW 3.3+** (window/input)
- **OpenGL 3.3 Core** (rendering)
- **libGL/Mesa** (on Linux)
- **opengl32** (on Windows native)

### Bundled
- **ImGui** (UI framework)
- **GLAD** (OpenGL loader)

---

## 🔄 Data Pipeline

```
Original Message
    ↓ [Encrypt with XOR]
Encrypted Message
    ↓ [Convert to Binary (8 bits per byte)]
Binary Message (0s and 1s)
    ↓ [Apply encoding algorithms]
┌─────────────────────────────────┐
│ NRZ Waveform  │  RZ Waveform    │
│ NRZ Message   │  RZ Message     │
└─────────────────────────────────┘
    ↓ [Transmit via TCP Socket]
Receiver (Reverse: Binary → Encrypted → Original)
```

---

## 🚀 Next Steps / Enhancement Ideas

1. **TLS/SSL Integration** — Add encryption layer for network transmission
2. **Multiple Algorithms** — Support AES, RSA, HMAC
3. **File Transfer** — Send files instead of just text
4. **Web Interface** — Build browser-based frontend (WebSocket + HTML5)
5. **Message Persistence** — Store received messages in SQLite/JSON
6. **Group Chat** — Extend to multiple senders/receivers
7. **Key Exchange** — Implement Diffie-Hellman for dynamic key agreement