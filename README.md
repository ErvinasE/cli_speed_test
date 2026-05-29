A lightweight, command-line interface (CLI) application written in C to test internet download and upload speeds. This tool automatically determines your geographic location, selects the most optimal server based on latency, and calculates your network speeds in Megabits per second (Mbps).

🚀 Features
Automated Testing: Run the application without arguments to automatically detect your location, find the best server, and perform both download and upload tests.

Location Detection: Resolves the user's physical location (Country and Continent Code) using the ip-api.com service.

Optimal Server Selection: Parses a local speedtest_server_list.json file via cJSON and pings servers to find the lowest latency host in your country or continent.

Speed Measurement: Evaluates download and upload speeds by transferring a 32MB payload, enforcing a strict 15-second timeout for both operations.

Targeted Operations: Uses getopt to provide CLI flags, allowing users to run specific parts of the program individually.

Fallback Logic: Automatically expands the search to the wider continent if no responsive servers are found within the specific country.

🛠️ Dependencies
To compile and run this application, you must have the following libraries installed on your system:

GCC (or any standard C compiler)

libcurl: Used for executing HTTP GET/POST requests and handling data transfers.

cJSON: Used for parsing the server list and location API responses.

Installing Dependencies (Ubuntu/Debian)
Bash
sudo apt-get update
sudo apt-get install build-essential libcurl4-openssl-dev libcjson-dev
🏗️ Build Instructions
A Makefile is provided for easy compilation. Simply run the following command in the root directory of the repository:

Bash
make
This will generate an executable named speedtest.

💻 Usage
Make sure the speedtest_server_list.json file is present in the same directory as the executable.

1. Run the full automated test (Location -> Best Server -> Upload -> Download):

Bash
./speedtest
2. Run individual commands:

-l: Determine and print your current location.

Bash
./speedtest -l
-p: Find and print the best server for your location based on ping.

Bash
./speedtest -p
-d <host>: Perform a 32MB download test from a specific server host.

Bash
./speedtest -d example.com:8080
-u <host>: Perform a 32MB upload test to a specific server host.

Bash
./speedtest -u example.com:8080
