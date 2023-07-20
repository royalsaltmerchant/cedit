CXX = g++ 
MACFLAGS = -mmacosx-version-min=12
CXXFLAGS = `/Users/julianranieri/Desktop/Code_Base/wx/wxWidgets-3.2.2.1/build-release/wx-config --cxxflags --static=yes`
LDFLAGS = `/Users/julianranieri/Desktop/Code_Base/wx/wxWidgets-3.2.2.1/build-release/wx-config --libs all --static=yes`

text_editor: main.cpp
	$(CXX) $(MACFLAGS) $(CXXFLAGS) $(LDFLAGS) -o $@ $<
