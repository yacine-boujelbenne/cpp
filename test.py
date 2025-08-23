import subprocess
import os
import tempfile
import time

def compile_cpp():
    """Compile the C++ program"""
    # List all C++ files that need to be compiled
    cpp_files = [
        "./SimulationManager.cpp",
        "./src/CanTp.cpp",
        "./src/Receiver.cpp",
        "./src/Sender.cpp"
    ]
    
    # Check if all files exist
    for file in cpp_files:
        if not os.path.exists(file):
            print(f"Error: {file} not found")
            return False
    
    # Compile the program
    compile_cmd = ["g++", "-std=c++11", "-o", "can_simulator"] + cpp_files
    result = subprocess.run(compile_cmd, capture_output=True, text=True)
    
    if result.returncode != 0:
        print("Compilation failed:")
        print(result.stderr)
        return False
    
    print("Compilation successful")
    return True

def run_test(inputs, description):
    """Run the program with provided inputs and return output"""
    try:
        # Start the process
        process = subprocess.Popen(
            ["./can_simulator"],
            stdin=subprocess.PIPE,
            stdout=subprocess.PIPE,
            stderr=subprocess.PIPE,
            text=True
        )
        
        # Send inputs
        input_str = "\n".join(inputs) + "\n"
        stdout, stderr = process.communicate(input_str)
        
        print(f"\n=== {description} ===")
        print("Output:")
        print(stdout)
        if stderr:
            print("Errors:")
            print(stderr)
        
        return process.returncode, stdout, stderr
    except Exception as e:
        print(f"Error running test: {e}")
        return -1, "", str(e)

def test_basic_functionality():
    """Test basic program functionality"""
    inputs = [
        "100",  # Tx ID
        "200",  # Rx ID
        "TestECU",  # ECU name
        "0"     # Quit
    ]
    
    return run_test(inputs, "Basic functionality test")

def test_message_sending():
    """Test sending a message"""
    inputs = [
        "100",  # Tx ID
        "200",  # Rx ID
        "TestECU",  # ECU name
        "1",    # Send message
        "Hello, CAN bus!",  # Message content
        "0"     # Quit
    ]
    
    return run_test(inputs, "Message sending test")

def test_message_receiving():
    """Test receiving a message"""
    inputs = [
        "100",  # Tx ID
        "200",  # Rx ID
        "TestECU",  # ECU name
        "2",    # Receive message
        "0"     # Quit
    ]
    
    return run_test(inputs, "Message receiving test")

def test_settings():
    """Test settings menu"""
    inputs = [
        "100",  # Tx ID
        "200",  # Rx ID
        "TestECU",  # ECU name
        "3",    # Settings
        "1",    # ID setup
        "150",  # New Tx ID
        "250",  # New Rx ID
        "0",    # Back to settings
        "0",    # Back to main menu
        "0"     # Quit
    ]
    
    return run_test(inputs, "Settings test")

def test_bus_settings():
    """Test bus settings"""
    inputs = [
        "100",  # Tx ID
        "200",  # Rx ID
        "TestECU",  # ECU name
        "3",    # Settings
        "2",    # Bus settings
        "1",    # Separation time
        "5",    # New separation time
        "2",    # Block size
        "8",    # New block size
        "0",    # Back to settings
        "0",    # Back to main menu
        "0"     # Quit
    ]
    
    return run_test(inputs, "Bus settings test")

def main():
    # Compile the C++ program first
    if not compile_cpp():
        return
    
    # Run tests
    tests = [
        test_basic_functionality,
        test_message_sending,
        test_message_receiving,
        test_settings,
        test_bus_settings
    ]
    
    for test in tests:
        returncode, stdout, stderr = test()
        
        # Check if test passed (program exited normally)
        if returncode == 0:
            print("✓ Test passed")
        else:
            print("✗ Test failed")
        
        print("-" * 50)

if __name__ == "__main__":
    main()