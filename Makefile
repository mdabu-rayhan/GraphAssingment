# Makefile for Dhaka City Routing Solver - All 6 Problems

CXX = g++
CXXFLAGS = -std=c++17 -O2 -Wall
LDFLAGS = -lm

# Target executables
TARGETS = problem1 problem2 problem3 problem4 problem5 problem6

# Header dependencies
HEADERS = graph_utils.h graph_loader.h

# Default target - build all
all: $(TARGETS)
	@echo "✓ All 6 problems compiled successfully!"
	@echo ""
	@echo "Executables:"
	@ls -lh problem[1-6] | awk '{print "  " $$9 " (" $$5 ")"}'

# Individual problem targets
problem1: problem1.cpp $(HEADERS)
	@echo "Compiling Problem 1..."
	$(CXX) $(CXXFLAGS) -o problem1 problem1.cpp $(LDFLAGS)

problem2: problem2.cpp $(HEADERS)
	@echo "Compiling Problem 2..."
	$(CXX) $(CXXFLAGS) -o problem2 problem2.cpp $(LDFLAGS)

problem3: problem3.cpp $(HEADERS)
	@echo "Compiling Problem 3..."
	$(CXX) $(CXXFLAGS) -o problem3 problem3.cpp $(LDFLAGS)

problem4: problem4.cpp $(HEADERS)
	@echo "Compiling Problem 4..."
	$(CXX) $(CXXFLAGS) -o problem4 problem4.cpp $(LDFLAGS)

problem5: problem5.cpp $(HEADERS)
	@echo "Compiling Problem 5..."
	$(CXX) $(CXXFLAGS) -o problem5 problem5.cpp $(LDFLAGS)

problem6: problem6.cpp $(HEADERS)
	@echo "Compiling Problem 6..."
	$(CXX) $(CXXFLAGS) -o problem6 problem6.cpp $(LDFLAGS)

# Test targets
test1: problem1
	@echo "Testing Problem 1..."
	@echo "90.404772 23.855136" | ./problem1 < /dev/stdin
	@echo "90.363501 23.805117" | ./problem1 >> /dev/stdin

test2: problem2
	@echo "Testing Problem 2..."
	@echo "90.404772 23.855136\n90.363501 23.805117" | ./problem2

test3: problem3
	@echo "Testing Problem 3..."
	@echo "90.404772 23.855136\n90.363501 23.805117" | ./problem3

test4: problem4
	@echo "Testing Problem 4..."
	@echo "90.404772 23.855136\n90.363501 23.805117\n10 30" | ./problem4

test5: problem5
	@echo "Testing Problem 5..."
	@echo "90.404772 23.855136\n90.363501 23.805117\n10 30" | ./problem5

test6: problem6
	@echo "Testing Problem 6..."
	@echo "90.404772 23.855136\n90.363501 23.805117\n10 30\n18 0" | ./problem6

# Clean targets
clean:
	@echo "Cleaning build files..."
	rm -f $(TARGETS)
	rm -f *.o
	@echo "✓ Clean complete"

cleanall: clean
	@echo "Cleaning KML files..."
	rm -f problem*.kml
	@echo "✓ All generated files removed"

# Help target
help:
	@echo "Dhaka City Routing Solver - Makefile"
	@echo ""
	@echo "Available targets:"
	@echo "  make          - Build all 6 problems"
	@echo "  make problem1 - Build Problem 1 only"
	@echo "  make problem2 - Build Problem 2 only"
	@echo "  make problem3 - Build Problem 3 only"
	@echo "  make problem4 - Build Problem 4 only"
	@echo "  make problem5 - Build Problem 5 only"
	@echo "  make problem6 - Build Problem 6 only"
	@echo "  make test1-6  - Test individual problems"
	@echo "  make clean    - Remove executables"
	@echo "  make cleanall - Remove executables and KML files"
	@echo "  make help     - Show this help"
	@echo ""
	@echo "Problem descriptions:"
	@echo "  Problem 1: Shortest car route (distance)"
	@echo "  Problem 2: Cheapest route (car + metro)"
	@echo "  Problem 3: Cheapest route (all modes)"
	@echo "  Problem 4: Cheapest route with schedules"
	@echo "  Problem 5: Fastest route with schedules"
	@echo "  Problem 6: Cheapest route with time constraints"

.PHONY: all clean cleanall help test1 test2 test3 test4 test5 test6
