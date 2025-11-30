(this will be for hashmaps project not lyrics database)

This project is a fully custom implementation of a templated HashMap class in C++, featuring separate chaining with linked lists, dynamic resizing, deep copying, and iterator-style traversal. 
It is designed to mimic the behavior of a basic associative container while providing a hands-on understanding of hashing, memory management, and data structure engineering.

The map stores key-value pairs using an array of bucket pointers, where each bucket leads to a chain of nodes that handle collisions. 
When the load factor exceeds 1.5, the table automatically resizes by doubling its capacity and rehashing all existing nodes. 
This approach ensures balanced performance even as the number of entries grows. The implementation includes support for insertion, lookup, modification, deletion, and containment checks. 
It also throws appropriate exceptions, such as out_of_range, when accessing missing keys.

A complete copy constructor and assignment operator are provided, each performing deep copies of all chains to ensure safety and correctness during object duplication. 
The clear() method properly frees all allocated memory, and the destructor guarantees no memory leaks. 
Additionally, the class supports simple iteration using begin() and next(), allowing the user to traverse all stored mappings in an unspecified order.

Overall, this project demonstrates low-level memory handling, pointer manipulation, hashing techniques, and object-oriented design in C++. 
It serves as a practical example of building a core data structure from scratch without relying on the C++ Standard Library’s unordered_map, giving deeper insight into how hash tables function internally.
