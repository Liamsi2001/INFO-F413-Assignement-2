# Makefile for Contract Algorithm Project

# Compiler
CXX = g++
CXXFLAGS = -std=c++17 -O2 -Wall
PYTHON = python  # Python executable

# Targets
TARGET = main
OBJECTS = main.o contract.o deterministic_min_cut.o
HEADERS = contract.hpp deterministic_min_cut.hpp

# Plot-related files
PLOT_SCRIPT = plot_graph_results.py
CSV_FILES = contract_simple.csv contract_exotic.csv
PLOT_OUTPUT = plots/combined_results.png

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

# Rule to run the main program and generate CSV
data: $(TARGET)
	./$(TARGET)

# Rule to generate plots
plot: $(CSV_FILES)
	$(PYTHON) $(PLOT_SCRIPT)

# Clean up build files
clean:
	rm -f $(TARGET) $(OBJECTS) $(CSV_FILES)
	rm -rf plots

# Default rule (runs data and plot)
all: data plot
