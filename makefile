CXX = g++
MACFLAGS = -mmacosx-version-min=12
CXXFLAGS = `/Users/julianranieri/Desktop/Code_Base/wx/wxWidgets-3.2.2.1/build-release/wx-config --cxxflags --static=yes`
LDFLAGS = `/Users/julianranieri/Desktop/Code_Base/wx/wxWidgets-3.2.2.1/build-release/wx-config --libs all --static=yes`
SRC = main.cpp Editor.cpp
OBJ = $(SRC:.cpp=.o)

text_editor: $(OBJ)
	$(CXX) $(MACFLAGS) $(OBJ) $(LDFLAGS) -o $@

%.o: %.cpp
	$(CXX) $(MACFLAGS) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ) text_editor
