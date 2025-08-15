#!/bin/bash

# Configuration
CAN_INTERFACE="vcan0"

# Setup VCAN interface
echo "Setting up $CAN_INTERFACE..."
sudo modprobe vcan 2>/dev/null || true
if ! ip link show "$CAN_INTERFACE" &>/dev/null; then
    sudo ip link add dev "$CAN_INTERFACE" type vcan
    sudo ip link set up "$CAN_INTERFACE"
    echo "$CAN_INTERFACE created and activated."
else
    echo "$CAN_INTERFACE already exists."
fi

# Cleanup on exit
cleanup() {
    echo -e "\nShutting down..."
    sudo ip link del "$CAN_INTERFACE" 2>/dev/null || true
    exit 0
}
trap cleanup INT TERM

# Start monitoring
echo -e "\nMonitoring $CAN_INTERFACE (Press Ctrl+C to stop)..."
candump -t a "$CAN_INTERFACE"

# If candump exits unexpectedly, run cleanup
cleanup