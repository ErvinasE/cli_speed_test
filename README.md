# C SPEEDTEST CLI

This tool automatically determines your geographic location, selects the most optimal server based on latency, and calculates your network speeds in Megabits per second (Mbps).

# Results
### Automated test
```bash
./speed_test
Test Results:
Your location: Lithuania
Server: speed-kaunas.telia.lt:8080
Upload speed is : 854.8 Mbps
Download speed is : 930.1 Mbps
```
### Determine location
```bash
./speed_test -l
Test Results:
Country: Lithuania
```
### Find best server
```bash
./speed_test -p
Test Results:
Best server is: speedtest-vno.init.lt:8080
```
### Upload speed test
```bash
./speed_test -u speedtest.litnet.lt:8080
Test Results:
Upload speed is : 865.4 Mbps
```
### Download speed test
```bash
./speed_test -d speedtest.litnet.lt:8080
Test Results:
Download speed is : 924.7 Mbps
```
# Compiling 
### Compile the whole project 
```bash
make
```
### To clean files
```bash
make clean
```
# Usage
### Automatic test (location > best server > upload > download)
```bash
./speed_test
```
### Determining location
```bash
./speed_test -l
```
### Determining best server 
```bash
./speed_test -p
```
### Upload speed
```bash
./speed_test -u (host)
```
### Download speed
```bash
./speed_test -u (host)
```
## Features

* **Automated Testing**: Run the application without arguments to automatically detect your location, find the best server, and perform both download and upload tests.
* **Informing User**: Informs the user the whole time the program is running.
* **Location Detection**: Resolves the user's physical location (Country and Continent Code) using the `ip-api.com` service.
* **Optimal Server Selection**: Parses a local `speedtest_server_list.json` file via `cJSON` and pings servers to find the lowest latency host in your country or continent.
* **Speed Measurement**: Evaluates download and upload speeds by transferring a 32MB payload, enforcing a strict 15-second timeout for both operations.
* **Targeted Operations**: Uses `getopt` to provide CLI flags, allowing users to run specific parts of the program individually.

## Dependencies

To compile and run this application, you must have the following libraries installed on your system:

* **GCC** (or any standard C compiler)
* **libcurl**: Used for executing HTTP GET/POST requests and handling data transfers.
* **cJSON**: Used for parsing the server list and location API responses.

### Installing Dependencies (Ubuntu/Debian)
```bash
sudo apt-get update
sudo apt-get install build-essential libcurl4-openssl-dev libcjson-dev
```
