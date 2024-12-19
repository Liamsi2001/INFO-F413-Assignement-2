# Makefile for Contract Algorithm Project

# Compiler
CXX = g++
CXXFLAGS = -std=c++17 -O2 -Wall
PYTHON = python  # Python executable

# Targets
TARGET = main
OBJECTS = main.o contract.o deterministic_min_cut.o fast_cut.o brute_force_min_cut.o
HEADERS = contract.hpp deterministic_min_cut.hpp fast_cut.hpp brute_force_min_cut.hpp 

# Plot-related files
PLOT_SCRIPT = plot_graph_results.py
CSV_FILES = fast_simple.csv fast_exotic.csv contract_simple.csv contract_exotic.csv
PLOT_OUTPUT = plots/* 

# Build the main executable
$(TARGET): $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJECTS)

# Compile main.cpp
main.o: main.cpp $(HEADERS)
	$(CXX) $(CXXFLAGS) -c main.cpp

# Compile contract.cpp
contract.o: contract.cpp $(HEADERS)
	$(CXX) $(CXXFLAGS) -c contract.cpp

# Compile deterministic_min_cut.cpp
deterministic_min_cut.o: deterministic_min_cut.cpp deterministic_min_cut.hpp
	$(CXX) $(CXXFLAGS) -c deterministic_min_cut.cpp

# Compile fast_cut.cpp
fast_cut.o: fast_cut.cpp fast_cut.hpp
	$(CXX) $(CXXFLAGS) -c fast_cut.cpp

# Compile brute_force_min_cut.cpp
brute_force_min_cut.o: brute_force_min_cut.cpp brute_force_min_cut.hpp
	$(CXX) $(CXXFLAGS) -c brute_force_min_cut.cpp

# Rule to run the main program and generate CSV
data: $(TARGET)
	./$(TARGET)

# Rule to generate plots
plot: $(CSV_FILES)
	$(PYTHON) $(PLOT_SCRIPT)

# Clean up build files
clean:
	rm -f $(TARGET) $(OBJECTS) $(CSV_FILES) $(PLOT_OUTPUT)

# Default rule (runs data and plot)
all: data plot
