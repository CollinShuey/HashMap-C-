//
// Created by Collin Shuey on 7/19/24.
//

#ifndef MYPROJECT_HASHMAP_H
#define MYPROJECT_HASHMAP_H

#include <functional>
#include <optional>
#include <algorithm>
#include <iostream>

template <typename K, typename V, typename H = std::hash<K>>
class HashMap {
public:
    // Constructor
    HashMap(int cap = 101, double lf = 0.5);

    // Destructor
    ~HashMap();

    // Copy constructor
    HashMap(const HashMap<K, V>& rhs);

    // Move constructor
    HashMap(HashMap<K, V>&& rhs);

    // Copy assignment operator
    HashMap<K, V>& operator=(const HashMap<K, V>& rhs);

    // Move assignment operator
    HashMap<K, V>& operator=(HashMap<K, V>&& rhs);

    // Strictly for testing purposes
    void check_contents(int idx);

    // For testing
    void print_table_size();

    // Insert
    void insert(const K& key, const V& value);

    // Strictly for testing
    void insert_no_except_for_testing(const K& key, const V& value);

    // Find
    std::optional<V> find(const K& key);

    // Remove
    void remove(const K& key);

    // Strictly for testing
    void remove_no_except(const K& key);

    // Size query
    int size() const;

    // Empty query
    bool empty() const;

    // MAX offset function
    int max_offset();

    // AVG offset function
    double average_offset();

    // Check Bubba property
    bool check_robinhood_property();

private:
    struct entry {
        entry() = default;
        entry(const K& k, const V& v, int off = -1);
        K key;
        V value;
        int offset{-1};
    };

    entry* table;
    H hash_func;
    int table_size;
    int capacity;
    double load_factor;

    // Hash function
    int hash(const K& k);

    // Resize function
    void resize(int new_size);

    // Function to test if a number is prime
    bool isPrime(int n);

    // Function to return next prime
    int nextPrime(int n);

    // Helper function for remove
    std::optional<int> remove_find(const K& key);
};

#include "HashMap.tpp"  // Include implementation file for templates

#endif //MYPROJECT_HASHMAP_H
