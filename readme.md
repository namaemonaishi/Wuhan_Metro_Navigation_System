# Wuhan_Metro_Navigation_System

## Requirements

| Files / Packages | Version | Description                                                  |
| ---------------- | ------- | ------------------------------------------------------------ |
| DEVC++           | 5.11    | -                                                            |
| GCC              | 4.9.2   | -                                                            |
| qqee_lib         | -       | Used for base64 encoding                                     |
| File.txt         | -       | Save information about the route, including the total length of the route, intermediate station information, total travel time, and transfer information between stations |

## Data structure design

- ### Overall architecture

  The overall structure used in this system is similar to a bidirectional adjacency list but differs from it in that the connections between transfer nodes link the entire table into a graph.

  ![](.\pic\1.jpg)

  Considering the 7 lines of the Wuhan Metro independently, each line is simplified into a single adjacency list model. The header node stores information about each station, and each subsequent station is stored as an individual node. These stations, together with the header node, form the overall structure of the entire graph. Once all 7 lines are stored, we can connect the corresponding transfer stations using pointers to achieve the purpose of transfers.

- ### Header node

  The header node is used to store the information of a single line. It mainly includes the capacity, total distance, average distance between stations, and the number of stations. The specific definition is shown in Table:

  | NAME          | TYPE    | Description                                        |
  | ------------- | ------- | -------------------------------------------------- |
  | time          | Int     | The time required for a train to complete one trip |
  | distance      | Float   | The total distance of the entire line              |
  | avg_distance  | Float   | The average distance between two stations          |
  | station_num   | Int     | the total number of station                        |
  | first_station | Pointer | Points to the starting station of the line         |

- ### Station node

  Used to represent a station, storing information such as the station name, station number, and the lines for transferring at the station. Its definition is shown in Table below:

  | NAME             | TYPE     | Description                                        |
  | ---------------- | -------- | -------------------------------------------------- |
  | name             | Char*    | Name of station                                    |
  | Transferable     | Int      | Whether the station allows transfers               |
  | prior            | Stationp | Points to the Prior station on the same line       |
  | next             | Stationp | Points to the Next station on the same line        |
  | tranfer_to_idx   | Int      | index to the line that can be transferred to       |
  | trasnfer_to_node | Stationp | Points to the next node that can be transferred to |

## Header file

- ### find_way.h

  Using recursive DFS search to find the shortest path. The function for finding the path can be composed of multiple functions.

  Function 1: findway function:

  - This function is the core of the entire pathfinding system. First, it saves the current station in a container and marks the current line as used. Then, it checks if the destination station has been reached. If so, it saves the route information and compares it with the previously found routes. If the current route is better according to the current criteria, it stores the route. Then, it clears the container of saved route nodes and resets the used line nodes.
  - If the destination station has not been reached, it checks if the current and destination stations are on the same line. If they are, it recursively calculates the result by assigning the destination station variable to the findway function of the starting station. This calculates the direct result while updating the distance and total time variables for recursion.
  - If the stations are not on the same line, it starts from the beginning of the line at the starting station and iterates through all the stations. For each transfer station found, it recursively calls the function with the transfer station as the new starting point. It also updates the distance and total time variables for recursion.

​		Function 2: printway function
​	This function is responsible for printing the saved routes. First, it checks if any routes have been saved, then it uses the map function to adjust the order of the saved routes. It then prints the starting and ending points of each route, as well as the information of key transfer stations. Finally, it prints the distance, ticket price, time, and other information of the route.

​		Function 3: compare struct for determining the sorting order
This struct is responsible for adjusting the order of the routes. If the sorting is based on time, the corresponding time function body is used. If the sorting is based on ticket price, the struct for sorting by ticket price is used. It contains only one boolean variable. The value of this variable is used to sort the routes based on their length.

- ### time.h

  Calculate and return the elapsed time based on the preset rules. This time is composed of the time spent on riding, the time spent on transferring between stations, and the waiting time generated by simulating the real-time operation of the subway.

- ### other.h

  Calculate the congestion based on the current time and specific line, and calculate the fare based on different lines. Return the calculated results.

- ### main.cpp

  - Read the saved data from the 'file' file, which contains the pre-saved data. Save the data of each station into the corresponding data structure. First, read the line information and build the head node. Then, read the station information, build station nodes, and establish connections. Save the cached line for transfer nodes and connect them accordingly to construct the entire graph.
  - Responsible for reading user input and responding accordingly. First, this module reads the username and password entered by the user and passes them to the data storage module for matching. After a successful match, it reads the functionality the user wants to choose. Once the user selects a functionality, it reads the corresponding data such as time, line, starting station, etc. If the data does not meet the required format, it prompts the user and terminates the process.
  - Based on the specific requirements of the user, call and activate the specified function in findway.h to initiate the process of finding the optimal route.

### Sample Input and output

- Read the data files containing line information and station information into the system and construct the graph. The requirement is to display the stations of a specified line in sequence or in a graphical format. The function code for this is 0.

  Example Output: 'Please enter the line you want to print:'

  Example Input: '2'

  Example Output: 'Tianhe Airport Station ---> Aviation Headquarters Station ---> ... ---> Canglong East Street Station ---> Fozuling Station ---> (End Point)'

- Specify the line and starting station, and sequentially find the next station from that station. At transfer stations, prompt for a transfer to another line and continue browsing to the subsequent stations. The input options are as follows: enter 1 for the next station in the forward direction, enter 2 for the next station in the reverse direction, enter 3 to transfer to another line, enter 4 to exit. The function code for this is 1.

  Example input: "1"
  Example output: "Please specify the line."

  Example input: "2"
  Example output: "Please specify the starting station."

  Example input: "街道口站" (Street Entrance Station)
  Example output: "The station has been found. Enter 1 for the next station in the forward direction, enter 2 for the next station in the reverse direction, enter 3 to transfer to another line, enter 4 to exit. Note: You can transfer at this station."

  Example input: "1"
  Example output: "This is Guangbutun Station."

  Example input: "2"
  Example output: "This is Street Entrance Station. Note: You can transfer at this station."

  Example input: "3"
  Example output: "Please specify which line to transfer to? Line 0.8 or Line 1.0."

  Example input: "0"
  Example output: "This is Street Entrance Station, on Line 8."

  Example input: "1"
  Example output: "This is Mafangshan Station."

  Example input: "2"
  Example output: "This is Xiaohongshan Station."

  Example input: "4"
  Example output: "The system has exited."

- Set the current time for subsequent matching with the train schedule.

  To set the current time, and display the arrival and departure times of trains at a specified station on a specified line, in both directions.
  
Sample output: "Please enter the line you want to take."
  Sample input: "2"
  Sample output: "Please specify the starting station."
Sample input: "Huazhong University of Science and Technology Station"
  Sample output: "Station found. Please enter the boarding time as X hours X minutes."
  Sample input: "8 0"
Sample output: "Reminder: Under normal circumstances, the subway runs every 3 minutes from 6 am to 11 pm.
  The next train towards the starting station will arrive at 8:00 AM and depart at 8:01 AM. Please be prepared.
  The next train towards the destination station will arrive at 8:00 AM and depart at 8:01 AM. Please be prepared."

-  Find the shortest reasonable route for trips 1-3, with the requirement that the arrival time at the destination station falls within the normal operating time of that line. Otherwise, it will be considered unreachable. This function corresponds to functionality number 3.

  The program will prompt for the input of the starting point, destination, line, and station, as well as the current time. It will check if the arrival meets the requirements. The program will print all the transfer stations along the way. The reasonable routes will be sorted in ascending order of ticket prices.
  
  Sample output: "Please enter the line you want to take."
  Sample input: "4"
  Sample output: "Please specify the starting station."
  Sample input: "Tiejilu Station"
  Sample output: "Station found. Please enter the destination line."
  Sample input: "6"
  Sample output: "Please specify the destination station."
  Sample input: "Shiqiao Station"
  Sample output: "Station found. Please enter the current time as X hours X minutes."
  Sample input: "8 0"
  Sample output: "Solution 1:
  Tiejilu Station ---> (Transfer to Line 8) Yuejiazui Station ---> (Transfer to Line 7) Xujiapeng Station ---> (Transfer to Line 6) Xiangganglu Station ---> Shiqiao Station ---> (Arrival at destination)
  Total cost: 5   Total distance: 13.846219   Total travel time: 35.0
  Solution 2:
  Tiejilu Station ---> (Transfer to Line 8) Yuejiazui Station ---> (Transfer to Line 3) Zhaojiatiao Station ---> (Transfer to Line 6) Xiangganglu Station ---> Shiqiao Station ---> (Arrival at destination)
  Total cost: 5   Total distance: 14.421062   Total travel time: 41.0
  Solution 3:
  Tiejilu Station ---> (Transfer to Line 8) Yuejiazui Station ---> (Transfer to Line 1) Huangpulu Station ---> (Transfer to Line 7) Sanyanglu Station ---> (Transfer to Line 6) Xiangganglu Station ---> Shiqiao Station ---> (Arrival at destination)
  Total cost: 5   Total distance: 14.088016   Total travel time: 41.0"
  
- Calculate the congestion percentage between stations during the specified time period as an indicator of the crowding level. Use the congestion factor to adjust the weighted travel time, and recommend the routes in ascending order of the weighted total time.

  Input remains the same as in (4).

  Sample output: "Solution 1:
  Tiejilu Station ---> (Transfer to Line 8) Yuejiazui Station ---> (Transfer to Line 3) Zhaojiatiao Station ---> (Transfer to Line 6) Xiangganglu Station ---> Shiqiao Station ---> (Arrival at destination)
  Total cost: 5   Total distance: 14.421062   Total travel time: 48.6   Congestion percentage: 0.27
  Solution 2:
  Tiejilu Station ---> (Transfer to Line 8) Yuejiazui Station ---> (Transfer to Line 1) Huangpulu Station ---> (Transfer to Line 7) Sanyanglu Station ---> (Transfer to Line 6) Xiangganglu Station ---> Shiqiao Station ---> (Arrival at destination)
  Total cost: 5   Total distance: 14.088016   Total travel time: 48.6   Congestion percentage: 0.26
  Solution 3:
  Tiejilu Station ---> (Transfer to Line 8) Yuejiazui Station ---> (Transfer to Line 7) Xujiapeng Station ---> (Transfer to Line 6) Xiangganglu Station ---> Shiqiao Station ---> (Arrival at destination)
  Total cost: 5   Total distance: 13.846219   Total travel time: 49.2   Congestion percentage: 0.16

-  Configure the rush hour commuter flow, shopping flow, and entertainment flow, and analyze their impact on subway traffic. The program allows adjusting the congestion level of each line at the current time to observe the changes in optimal routes.

  The program will prompt for the input of the desired line, starting station, destination line, destination station, current time, and congestion information for specific time intervals. The user can specify the congestion level for each line during different time intervals. The program will calculate and display the optimal routes considering the congestion factors.

  Sample output: "Please enter the line you want to take."
  Sample input: "4"
  Sample output: "Please specify the starting station."
  Sample input: "Tiejilu Station"
  Sample output: "Station found. Please enter the destination line."
  Sample input: "6"
  Sample output: "Please specify the destination station."
  Sample input: "Shiqiao Station"
  Sample output: "Station found. Please enter the current time as X hours X minutes."
  Sample input: "8 0"
  Sample output: "Please enter the line, time interval, and congestion level to modify, in the order of X line X time X minute to X time X minute X congestion level."
  Sample input: "8 8 0 10 0 0.8"
  Sample output: "Please indicate whether you want to use the congestion avoidance system with a congestion level of 50."
  Sample input: "1"
  Sample output: "Solution 1:
  Tiejilu Station ---> (Transfer to Line 2) Hongshan Square Station ---> (Transfer to Line 6) Jianghanlu Station ---> Shiqiao Station ---> (Arrival at destination)
  Total cost: 6 Total distance: 21.219257 Total travel time: 67.4 Congestion percentage: 0.27
  Solution 2:
  Tiejilu Station ---> (Transfer to Line 2) Hongshan Square Station ---> (Transfer to Line 7) Pangxiejia Station ---> (Transfer to Line 6) Xiangganglu Station ---> Shiqiao Station ---> (Arrival at destination)
  Total cost: 7 Total distance: 25.339359 Total travel time: 70.2 Congestion percentage: 0.27
  Solution 3:
  Tiejilu Station ---> (Transfer to Line 2) Zhongnanlu Station ---> (Transfer to Line 6) Jianghanlu Station ---> Shiqiao Station ---> (Arrival at destination)
  Total cost: 7 Total distance: 24.170609 Total travel time: 76.4 Congestion percentage: 0.27"

- (6) Incorporating the passenger flow or congestion level of the subway lines into the factors for recommending routes, it is possible to design an analysis model that prioritizes avoiding excessive congestion and provides alternative transfer routes.

  Inputs:

  1. Desired subway line
  2. Starting station
  3. Destination line
  4. Destination station
  5. Current time (in hours and minutes, e.g., 8 0 for 8:00 AM)
  6. Configuration of line, time interval, and congestion level (format: line start_time end_time congestion, e.g., 8 8 0 10 0 0.8 to set the congestion level of Line 8 from 8:00 AM to 10:00 AM as 0.8)
  7. Choice of using the congestion avoidance system with a 50% congestion threshold (1 for yes, 0 for no