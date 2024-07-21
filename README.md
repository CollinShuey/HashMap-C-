# HashMap C++
A template Hashmap class that I made implementing an algorithm called Robinhood hashing.

## Description

Robinhood hashing allows for a fast and memory efficient Hashtable. My template class uses this algorithm and it achieves an average of speed 30% faster then the c++ standard library unordered map which also uses a underlying array for an hashtable. The main idea of Robinhood hashing is when there is a collision between two keys, you compare how far each one of them is away from their "home" (the place they hashed into). If the current item has a lower offset (distance from place it hashed into) then you linearly probe along until you find one with lower offset, or an empty space in memory. This allows us to keep elements as close as we can to their original "home". This allows the Find function to have a nice trick, if we are trying to find something and the current element has a lower offset then the element we are trying to find has, that means we can stop looking and return an false/exception. Why? Because if the element has a lower offset then the one we are looking for would have, we would have already found it as Robinhood hashing wouldn't allow this to happen. Remove also allows us to shift everything over left in the memory if it has an offset bigger then 0. This makes removes slower but subsequent finds/inserts faster and more efficient.


### Cmake build

* This is how I set up my Cmake, just make sure to include given directories and source files if you wish to use/add too.

```
cmake_minimum_required(VERSION 3.26)
project(MyProject)

# Set the C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Add the executable
add_executable(MyProject
        UnitTests/tests.cpp
        HashMap/HashMap.tpp
        HashMap/HashMap.h
        # Add source files here
        # List other necessary .cpp files here
)

# Specify include directories
target_include_directories(MyProject PRIVATE
        ${PROJECT_SOURCE_DIR}/HashMap
        ${PROJECT_SOURCE_DIR}/Utils
)
```


