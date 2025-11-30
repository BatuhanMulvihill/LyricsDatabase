#pragma once

#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <utility>

using namespace std;

template <typename KeyT, typename ValT>
class HashMap {
 private:
  struct ChainNode {
    const KeyT key;
    ValT value;
    ChainNode* next;

    ChainNode(KeyT key, ValT value) : key(key), value(value), next(nullptr) {
    }

    ChainNode(KeyT key, ValT value, ChainNode* next)
        : key(key), value(value), next(next) {
    }
  };

  ChainNode** data;
  size_t sz;
  size_t capacity;

  // Utility members for begin/next
  ChainNode* curr;
  size_t curr_idx;

  void resize(){
    int old_capacity = capacity;
    ChainNode** old_data = data;
    capacity = capacity * 2;
    data = new ChainNode*[capacity];
    for(int i = 0; i < capacity; i++) {
        data[i] = nullptr;
    }
    for(int i = 0; i < old_capacity; i++) {
        ChainNode* current = old_data[i];
        while (current != nullptr) {
            ChainNode* next = current->next;
            size_t newIndex = hash<KeyT>{}(current->key) % capacity;
            current->next = data[newIndex];
            data[newIndex] = current;

            current = next;
        }
    }
    delete[] old_data;
  }
 public:
  /**
   * Creates an empty `HashMap` with 10 buckets.
   */
  HashMap() {
    data = new ChainNode*[10];
    sz = 0;
    capacity = 10;
    for(int i = 0; i < capacity; i++){
      data[i] = nullptr;
    }
  }

  /**
   * Creates an empty `HashMap` with `capacity` buckets.
   */
  HashMap(size_t capacity) {
    this->capacity = capacity;
    sz = 0;
    data = new ChainNode*[capacity];
    for(int i = 0; i < capacity; i++){
      data[i] = nullptr;
    }
    // TODO_STUDENT
  }

  /**
   * Checks if the `HashMap` is empty. Runs in O(1).
   */
  bool empty() const {
    if(sz == 0){
      return true;
    }
    return false;
  }

  /**
   * Returns the number of mappings in the `HashMap`. Runs in O(1).
   */
  size_t size() const {
    // TODO_STUDENT
    return sz;
  }

  /**
   * Adds the mapping `{key -> value}` to the `HashMap`. If the key already
   * exists, does not update the mapping (like the C++ STL map).
   *
   * Uses the hash value of the key to determine the location in the
   * underlying hash table. Creates exactly one new node; resizes by doubling
   * when the load factor exceeds 1.5.
   *
   * On resize, doesn't create new nodes, but rearranges existing ones.
   *
   * Runs in O(L), where L is the length of the longest chain.
   */
  void insert(KeyT key, ValT value) {
    if(double(sz + 1) / capacity > 1.5){
      resize();
    }
    size_t index = hash<KeyT>{}(key) % capacity;
    ChainNode* current = data[index];
    while(current != nullptr){
      if(key == current->key){
        return;
      }
      current = current->next;
    }
    
    data[index] = new ChainNode(key,value,data[index]);
    sz++;
    // TODO_STUDENT
  }

  /**
   * Return a reference to the value stored for `key` in the map.
   *
   * If key is not present in the map, throw `out_of_range` exception.
   *
   * Runs in O(L), where L is the length of the longest chain.
   */
  ValT& at(const KeyT& key) const {
    size_t TheBucket = hash<KeyT>{}(key) % capacity;
    ChainNode* current = data[TheBucket];
    while(current != nullptr){
      if(current->key == key){
        return current->value;
      }
      current = current->next;
      }
    throw out_of_range("Key Not Present in map.");
  }

  /**
   * Returns `true` if the key is present in the map, and false otherwise.
   *
   * Runs in O(L), where L is the length of the longest chain.
   */
  bool contains(const KeyT& key) const {
      size_t TheBucket = hash<KeyT>{}(key) % capacity;
      ChainNode* current = data[TheBucket];
      while(current != nullptr){
        if(current->key == key){
          return true;
        }
        current = current->next;
      }
    return false;
  }

  /**
   * Empties the `HashMap`, freeing all nodes. The bucket array may be left
   * alone.
   *
   * Runs in O(N+B), where N is the number of mappings and B is the number of
   * buckets.
   */
  void clear() {
    for(int i = 0; i < capacity; i++){
      ChainNode* current = data[i];
      while(current != nullptr){
        ChainNode* NextNode = current->next;
        delete current;
        current = NextNode;
      }
      data[i] = nullptr;
    }
    sz = 0;
    // TODO_STUDENT
  }

  /**
   * Destructor, cleans up the `HashMap`.
   *
   * Runs in O(N+B), where N is the number of mappings and B is the number of
   * buckets.
   */
  ~HashMap() {
    clear();
    delete[] data;
  }

  /**
   * Removes the mapping for the given key from the `HashMap`, and returns the
   * value.
   *
   * Throws `out_of_range` if the key is not present in the map. Creates no new
   * nodes, and does not update the key or value of any existing nodes.
   *
   * Runs in O(L), where L is the length of the longest chain.
   */
  ValT erase(const KeyT& key) {
    // TODO_STUDENT
    size_t index = hash<KeyT>{}(key) % capacity;
    ChainNode* current = data[index];
    ChainNode* previous = nullptr;
    while (current != nullptr) {
        if (current->key == key) {
            ValT value = current->value;
            if (previous == nullptr) {
                data[index] = current->next;
            } else {
                previous->next = current->next;
            }
            delete current;
            sz--;
            return value;
        }
        previous = current;
        current = current->next;
    }
    throw out_of_range("Error: Key is not in Map.");
  }

  /**
   * Copy constructor.
   *
   * Copies the mappings from the provided `HashMap`.
   *
   * Runs in O(N+B), where N is the number of mappings in `other`, and B is the
   * number of buckets.
   */
  HashMap(const HashMap& other) {
    // TODO_STUDENT
    sz = other.sz;
    capacity = other.capacity;
    data = new ChainNode*[capacity];
    for(int i = 0; i < capacity; i++){
      if(other.data[i] == nullptr){
        data[i] = nullptr;
        continue;
      }
      data[i] = new ChainNode(other.data[i]->key,other.data[i]->value,nullptr);
      ChainNode* curr = data[i];
      ChainNode* CurrCopy = other.data[i]->next;
      while(CurrCopy != nullptr){
        curr->next = new ChainNode(CurrCopy->key,CurrCopy->value,nullptr);
        curr = curr->next;
        CurrCopy = CurrCopy->next;
      }
    }
  }

  /**
   * Assignment operator; `operator=`.
   *
   * Clears this table, and copies the mappings from the provided `HashMap`.
   *
   * Runs in O((N1+B1) + (N2+B2)), where N1 and B1 are the number of mappings
   * and buckets in `this`, and N2 and B2 are the number of mappings and buckets
   * in `other`.
   */
  HashMap& operator=(const HashMap& other) {
    // TODO_STUDENT
    if(&other == this){
      return *this;
    }
    clear();
    delete[]data;
    sz = other.sz;
    capacity = other.capacity;
    data = new ChainNode*[capacity];
    for(int i = 0; i < capacity; i++){
      if(other.data[i] == nullptr){
        data[i] = nullptr;
        continue;
      }
      data[i] = new ChainNode(other.data[i]->key,other.data[i]->value,nullptr);
      ChainNode* curr = data[i];
      ChainNode* CurrCopy = other.data[i]->next;
      while(CurrCopy != nullptr){
        curr->next = new ChainNode(CurrCopy->key,CurrCopy->value,nullptr);
        curr = curr->next;
        CurrCopy = CurrCopy->next;
      }
    }
    return *this;
  }

  // =====================

  /**
   *
   * Checks if the contents of `this` and `other` are equivalent.
   *
   * Two `HashMap` objects are equivalent if they contain the same
   * mappings from key to value. To be equivalent, the two `HashMap`
   * objects need not have the elements saved in the same order
   * inside of the buckets.
   *
   * Runs in worst-case O(B*L) time, where B is the maximum number
   * of buckets in either of the `HashMap` objects and L is the length
   * of the  largest chain on any of the buckets.
   *
   */
  bool operator==(const HashMap& other) const {
    // TODO_STUDENT
    if(this == &other){
      return true;
    }
    if(sz != other.sz){
      return false;
    }
    for(int i = 0; i < capacity; i++){
      ChainNode* curr = data[i];
      while(curr != nullptr){
        ValT OtherValue = other.at(curr->key);
        if(OtherValue != curr->value){
          return false;
        }
        curr = curr->next;
      }
    }
    return true;
  }

  /**
   * Resets internal state for an iterative traversal.
   *
   * See `next` for usage details. Modifies nothing except for `curr` and
   * `curr_idx`.
   *
   * Runs in worst-case O(B), where B is the number of buckets.
   */
  void begin() {
    curr = nullptr;
    curr_idx = 0;
    while (curr_idx < capacity && data[curr_idx] == nullptr) {
      curr_idx++;
    }

    if (curr_idx < capacity) {
      curr = data[curr_idx];
    }
    // TODO_STUDENT
  }

  /**
   * Uses the internal state to return the "next" key and value
   * by reference, and advances the internal state. Returns `true` if the
   * reference parameters were set, and `false` otherwise.
   *
   * Example usage:
   *
   * ```c++
   * HashMap<string, int> hm;
   * hm.begin();
   * string key;
   * int value;
   * while (hm.next(key, val)) {
   *   cout << key << ": " << val << endl;
   * }
   * ```
   *
   * Does not visit the mappings in any defined order.
   *
   * Modifies nothing except for `curr` and `curr_idx`.
   *
   * Runs in worst-case O(B) where B is the number of buckets.
   */
  bool next(KeyT& key, ValT& value) {
    // TODO_STUDENT
    if (curr == nullptr) {
        return false;
    }
    key = curr->key;
    value = curr->value;
    curr = curr->next;

    while (curr == nullptr && ++curr_idx < capacity) {
      curr = data[curr_idx];
    }
    return true;
  }

  // ===============================================

  /**
   * Returns a pointer to the underlying memory managed by the `HashMap`.
   * For autograder testing purposes only.
   */
  void* get_data() const {
    return this->data;
  }

  /**
   * Returns the capacity of the underlying memory managed by the `HashMap`. For
   * autograder testing purposes only.
   */
  size_t get_capacity() {
    return this->capacity;
  }
};
