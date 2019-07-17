CXX	:= g++
CXXFLAGS := -std=c++1y -Wall -Wextra

SRCS := permutation.cpp main.cpp dm.cpp enumeration.cpp dm_perm.cpp sagemath_alter.cpp 
OBJECTS := $(SRCS:%.cpp=%.o)
DEPS := $(OBJECTS:%.o=%.d)

main.exe: $(OBJECTS)
	$(CXX) -o $@ $^

-include $(DEPS)

%.o: %.cpp
	$(CXX) -c -MMD -MP $(CXXFLAGS) $<

clean:
	rm $(OBJECTS)
	rm $(DEPS)
