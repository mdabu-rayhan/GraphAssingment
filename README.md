# Dhaka City Routing System
## Code Samurai 2019 - Multi-Modal Transportation Optimizer

A comprehensive C++ solution for finding optimal routes in Dhaka City using various transportation modes including cars, metro rail, and bus services with real-time scheduling.

---

## 📋 Table of Contents

- [Overview](#overview)
- [Problems Solved](#problems-solved)
- [Project Structure](#project-structure)
- [Requirements](#requirements)
- [Build & Compile](#build--compile)
- [Running the Programs](#running-the-programs)
- [Input Format](#input-format)
- [Output Format](#output-format)
- [Technical Details](#technical-details)
- [Performance Notes](#performance-notes)
- [Datasets](#datasets)

---

## 🎯 Overview

This project implements 6 different routing optimization problems for Dhaka City's transportation network. Each problem progressively increases in complexity, from basic shortest path to time-constrained multi-modal routing with schedules.

**Key Features:**
- ✅ Modular architecture with shared utilities
- ✅ Support for 4 transportation modes (car, metro, 2 bus lines)
- ✅ Real-world Dhaka street network (69,222 nodes)
- ✅ KML output for Google Maps visualization
- ✅ Time-expanded graph for schedule-aware routing
- ✅ Professional build system with Makefile

---

## 📝 Problems Solved

### Problem 1: Shortest Car Route
**Goal:** Find the shortest distance route using only cars.  
**Algorithm:** Dijkstra's shortest path  
**Status:** ✅ Fully tested

### Problem 2: Cheapest Route (Car + Metro)
**Goal:** Find the cheapest route using cars and metro rail.  
**Costs:** Car ৳20/km, Metro ৳5/km  
**Algorithm:** Dijkstra with cost optimization  
**Status:** ✅ Fully tested

### Problem 3: Cheapest Route (All Modes)
**Goal:** Find the cheapest route using all available transportation.  
**Costs:** Car ৳20/km, Metro ৳5/km, Buses ৳7/km  
**Algorithm:** Dijkstra with cost optimization  
**Status:** ✅ Fully tested

### Problem 4: Cheapest Route with Schedules
**Goal:** Find the cheapest route considering vehicle schedules.  
**Algorithm:** Time-expanded graph search  
**Status:** ⚠️ Implemented (computationally intensive)

### Problem 5: Fastest Route with Schedules
**Goal:** Find the fastest route considering schedules and speeds.  
**Algorithm:** Time-expanded graph search  
**Status:** ⚠️ Implemented (computationally intensive)

### Problem 6: Cheapest Route with Deadline
**Goal:** Find the cheapest route that reaches destination before deadline.  
**Algorithm:** Time-expanded graph with constraint checking  
**Status:** ⚠️ Implemented (computationally intensive)

---

## 📂 Project Structure

```
Graph/
├── problem1.cpp              # Shortest car route
├── problem2.cpp              # Cheapest route (car + metro)
├── problem3.cpp              # Cheapest route (all modes)
├── problem4.cpp              # Cheapest with schedules
├── problem5.cpp              # Fastest with schedules
├── problem6.cpp              # Cheapest with deadline
├── graph_utils.h             # Shared utilities (distance, KML generation)
├── graph_loader.h            # Graph construction and dataset loading
├── Makefile                  # Build system
├── Datasets/                 # Transportation network data
│   ├── Roadmap-Dhaka.csv            # Street network (69,222 nodes)
│   ├── Routemap-DhakaMetroRail.csv  # Metro rail network
│   ├── Routemap-BikolpoBus.csv      # Bikolpo bus service
│   └── Routemap-UttaraBus.csv       # Uttara bus service
└── README.md                 # This file
```

---

## 🔧 Requirements

- **Compiler:** g++ with C++17 support
- **Operating System:** Linux/Unix (tested on Kali Linux)
- **Memory:** Minimum 4GB RAM (8GB recommended for problems 4-6)
- **Dependencies:** Standard C++ Library (STL) only - no external libraries

---

## 🛠️ Build & Compile

### Compile All Problems
```bash
make all
```

### Compile Individual Problems
```bash
make problem1    # Compile problem 1
make problem2    # Compile problem 2
# ... and so on
```

### Clean Build
```bash
make clean       # Remove executables only
make cleanall    # Remove executables and generated KML files
```

### View All Options
```bash
make help
```

---

## ▶️ Running the Programs

### Problem 1: Shortest Car Route
```bash
./problem1
```
**Example Input:**
```
Enter source coordinates (longitude latitude): 90.363833 23.834145
Enter destination coordinates (longitude latitude): 90.463833 23.734145
```

**Example Output:**
```
=== Problem 1: Shortest Car Route ===
Source: (90.3638, 23.8341)
Destination: (90.4638, 23.7341)
Shortest distance: 16.66 km
Route saved to: problem1_route.kml
```

---

### Problem 2: Cheapest Route (Car + Metro)
```bash
./problem2
```
**Example Input:**
```
Enter source coordinates (longitude latitude): 90.363833 23.834145
Enter destination coordinates (longitude latitude): 90.463833 23.734145
```

**Example Output:**
```
=== Problem 2: Cheapest Route (Car + Metro) ===
Source: (90.3638, 23.8341)
Destination: (90.4638, 23.7341)
Total cost: ৳295.01
Route saved to: problem2_route.kml
```

---

### Problem 3: Cheapest Route (All Modes)
```bash
./problem3
```
**Example Input:**
```
Enter source coordinates (longitude latitude): 90.363833 23.834145
Enter destination coordinates (longitude latitude): 90.463833 23.734145
```

**Example Output:**
```
=== Problem 3: Cheapest Route (All Modes) ===
Source: (90.3638, 23.8341)
Destination: (90.4638, 23.7341)
Total cost: ৳148.62
Route saved to: problem3_route.kml
```

---

### Problem 4: Cheapest with Schedules
```bash
./problem4
```
**Example Input:**
```
Enter source coordinates (longitude latitude): 90.363833 23.834145
Enter destination coordinates (longitude latitude): 90.364255 23.828335
Enter start time (hour minute, 24-hour format): 10 30
```

**Note:** This may take significant time for distant locations due to time-expanded graph complexity.

---

### Problem 5: Fastest with Schedules
```bash
./problem5
```
**Example Input:**
```
Enter source coordinates (longitude latitude): 90.363833 23.834145
Enter destination coordinates (longitude latitude): 90.364255 23.828335
Enter start time (hour minute, 24-hour format): 10 30
```

---

### Problem 6: Cheapest with Deadline
```bash
./problem6
```
**Example Input:**
```
Enter source coordinates (longitude latitude): 90.363833 23.834145
Enter destination coordinates (longitude latitude): 90.364255 23.828335
Enter start time (hour minute, 24-hour format): 10 30
Enter deadline (hour minute, 24-hour format): 18 30
```

---

## 📥 Input Format

### Geographic Coordinates
- **Format:** `longitude latitude`
- **Example:** `90.363833 23.834145`
- **Coordinate System:** WGS84 (decimal degrees)
- **Region:** Dhaka, Bangladesh

### Time Input
- **Format:** `hour minute` (24-hour format)
- **Example:** `10 30` (10:30 AM) or `18 45` (6:45 PM)
- **Valid Range:** 06:00 - 23:00 (6 AM - 11 PM)

---

## 📤 Output Format

### Console Output
All problems display:
- Problem number and description
- Source and destination coordinates
- Optimization metric (distance/cost/time)
- Route status and KML file name

### KML File Output
Generated KML files can be opened in:
- **Google Earth Desktop**
- **Google Maps** (upload as custom layer)
- **Any KML-compatible mapping software**

**KML File Structure:**
```xml
<?xml version="1.0" encoding="UTF-8"?>
<kml xmlns="http://earth.google.com/kml/2.1">
<Document>
<Placemark>
<name>route.kml</name>
<LineString>
<tessellate>1</tessellate>
<coordinates>
90.363833,23.834145,0
90.363832,23.833879,0
...
</coordinates>
</LineString>
</Placemark>
</Document>
</kml>
```

---

## 🔬 Technical Details

### Algorithms

**Problems 1-3: Standard Dijkstra**
- **Complexity:** O(E log V)
- **State Space:** 69,222 nodes
- **Performance:** Fast (completes in seconds)

**Problems 4-6: Time-Expanded Graph**
- **Complexity:** O(E × T log(V × T))
- **State Space:** 69,222 nodes × 1,020 time slots = ~70 million states
- **Performance:** Computationally intensive (minutes to hours for distant locations)

### Graph Representation
- **Structure:** Adjacency list
- **Nodes:** 69,222 road intersections
- **Edges:** Bidirectional with type labels (road/metro/bikolpo/uttara)
- **Distance Calculation:** Haversine formula for geographic accuracy

### Transportation Modes

| Mode | Cost (৳/km) | Speed (km/h) | Operating Hours |
|------|-------------|--------------|-----------------|
| Car | 20 | 15 | 24/7 |
| Metro | 5 | 30 | 06:00-23:00 |
| Bikolpo Bus | 7 | 20 | 06:00-23:00 |
| Uttara Bus | 7 | 20 | 06:00-23:00 |

### Schedule Information

**Metro Rail:**
- Frequency: Every 15 minutes
- Departure times: XX:00, XX:15, XX:30, XX:45

**Bus Services:**
- Frequency: Every 30 minutes  
- Departure times: XX:00, XX:30

---

## ⚠️ Performance Notes

### Problems 1-3 (Fast)
- Solve instantly for any source-destination pair
- Suitable for production use
- Generate accurate KML visualizations

### Problems 4-6 (Intensive)
- **Algorithm:** Correct implementation
- **Challenge:** Large state space (70M+ states)
- **Current Performance:** May take hours for distant locations
- **Use Cases:** Works well for nearby locations (<5 km)

**Optimization Recommendations for Production:**
1. Implement A* algorithm with geographic heuristic
2. Add search space pruning (limit search radius)
3. Use 5-minute time intervals instead of 1-minute
4. Pre-compute transit network connections
5. Implement bidirectional search
6. Cache frequently requested routes

---

## 📊 Datasets

### Roadmap-Dhaka.csv (3.0 MB)
- Complete street network of Dhaka
- 69,222 nodes (intersections)
- Bidirectional road connections

### Routemap-DhakaMetroRail.csv (12 KB)
- Metro rail network
- Station coordinates
- Route connections

### Routemap-BikolpoBus.csv (12 KB)
- Bikolpo bus service routes
- Stop locations
- Route topology

### Routemap-UttaraBus.csv (7.6 KB)
- Uttara bus service routes
- Stop locations  
- Route topology

**Total Dataset Size:** ~3 MB  
**Format:** CSV (Comma-Separated Values)

---

## 🎓 Usage Examples

### Example 1: Quick Test (Nearby Locations)
```bash
# Test Problem 1 with nearby points
./problem1
# Input: 90.363833 23.834145
#        90.364255 23.828335
# Expected: Route found in < 1 second
```

### Example 2: Visualize Route
```bash
# Generate KML and view in Google Earth
./problem3
# After completion, open problem3_route.kml in Google Earth
```

### Example 3: Rebuild Everything
```bash
# Clean rebuild from source
make cleanall
make all
```

---

## 📄 License

This project was developed as part of Code Samurai 2019 competition.

---

## 🤝 Credits

**Algorithm Design:** Dijkstra's shortest path, Time-expanded graph search  
**Geographic Calculations:** Haversine distance formula  
**Datasets:** Code Samurai 2019 - Dhaka transportation network

---

## 📞 Support

For issues or questions:
1. Check that all CSV files are in the `Datasets/` folder
2. Verify g++ compiler supports C++17
3. Ensure adequate RAM (4GB minimum, 8GB recommended)
4. For problems 4-6, use nearby locations for faster results

---

**Last Updated:** February 2026  
**Version:** 1.0  
**Status:** Production Ready (Problems 1-3), Prototype (Problems 4-6)
