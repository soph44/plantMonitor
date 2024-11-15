from flask import Flask, render_template, jsonify
import json
import serial
import time
import logging
from datetime import datetime

# app = Flask(__name__)

# Logger setup
timestamp = datetime.now().strftime("_%Y%m%d_%H%M%S")
logger = logging.getLogger(f"PlantMonitoring")
logger.setLevel(logging.INFO)
console_handler = logging.StreamHandler()
console_handler.setLevel(logging.INFO)
formatter = logging.Formatter('%(asctime)s - %(name)s - %(levelname)s - %(message)s')
console_handler.setFormatter(formatter)
logger.addHandler(console_handler) 

# Initialize connection to board and wait for stable connection
def initialize_board_connection(serialPort, baudRate, delay, timeout):
    logger.info(f"Function: initialize_board_connection - {serialPort}, {baudRate}")
    # Serial Port (Arduino) to read from
    serialPort = serial.Serial(serialPort, baudRate)
    time.sleep(delay) # wait for board connection to stabilize

    # Check for incoming data as an indication that the Arduino is ready
    start_time = time.time()
    while True:
        serialPort.write(b"CMD: CHECK READY\n")
        time.sleep(0.5)
        if serialPort.in_waiting > 0:
            while serialPort.in_waiting > 0:
                response = serialPort.readline().decode('utf-8').strip() 
                logger.info(f"Arduino connection is stable: {response}")
            return serialPort
        if time.time() - start_time > timeout:
            logger.info("Timeout: No response from Arduino.")
            return None
        
# Function to request Arduino to change MUX address
def select_address(serialPort, selectAddress):
    logger.info(f"Function: select_address - {selectAddress}")
    # Send command to select address
    message = "CMD: SELECT\n"
    address = selectAddress + "\n"
    serialPort.write(message.encode())
    time.sleep(0.5)
    serialPort.write(address.encode())

    return None

# Function to request Arduino to send sensor data and then load into JSON
def request_data(serialPort):
    logger.info(f"Function: request_data")
    serialPort.write(b"CMD: SEND DATA\n")
    time.sleep(2) # delay for Arduino to process command and send data over serial

    if serialPort.in_waiting > 0:
        response = serialPort.readline().decode('utf-8').strip()  # Read the response
        try:
            data = json.loads(response)  # Parse the JSON data
            logger.info(f"request data response - {data}")
            return data
        except json.JSONDecodeError:
            logger.info("Failed to decode JSON")
            return None
    else:
        logger.info("Serial Port line is empty")
    return None


def reset_serial_buffer(serialPort):
    logger.info(f"Function: flush_both_ends")
    serialPort.reset_input_buffer()
    serialPort.reset_output_buffer()
    time.sleep(5)
    serialPort.write(b"CMD: RESET BUFFER\n")
    time.sleep(5)
    serialPort.reset_input_buffer()
    serialPort.reset_output_buffer()
    time.sleep(5)


# Route to display JSON data on a webpage
# @app.route('/')
# def index():
#     select_address("TBD")
#     data = request_data()  # Get the data from the JSON file
#     if data:
#         return render_template('index.html', data=data)
#     else:
#         return "Error loading data", 500

if __name__ == "__main__":
    # app.run(debug=True)

    # Serial Port Address and Settings
    comPort = "/dev/cu.usbserial-1420"
    baudRate = 9600

    serialPortArd = initialize_board_connection(comPort, baudRate, delay=3, timeout=5)
    # reset_serial_buffer(serialPortArd)
    # select_address(ardPort, "TBD")
    # while serialPortArd.in_waiting > 0:
    #     response = serialPortArd.readline().decode('utf-8').strip()  # Read the response
    #     print(response)
    data = request_data(serialPortArd)
    data = request_data(serialPortArd)
    data = request_data(serialPortArd)
    # print(data)