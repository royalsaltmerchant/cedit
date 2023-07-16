CXX = g++
CXXFLAGS = `wx-config --cxxflags`
LDFLAGS = `wx-config --libs all`

text_editor: main.cpp
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -o $@ $<
