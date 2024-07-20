//
// Created by Collin Shuey on 7/19/24.
//

#include "HashMap.h"
// Constructor
template <typename K, typename V, typename H>
HashMap<K, V, H>::HashMap(int cap, double lf)
        : capacity(cap), load_factor(lf), table_size(0) {
    table = new entry[cap];
}

// Destructor
template <typename K, typename V, typename H>
HashMap<K, V, H>::~HashMap() {
    delete[] table;
}

// Copy constructor
template <typename K, typename V, typename H>
HashMap<K, V, H>::HashMap(const HashMap<K, V>& rhs)
        : table_size(rhs.table_size), capacity(rhs.capacity), load_factor(rhs.load_factor), hash_func(rhs.hash_func) {
    table = new entry[rhs.capacity];
    for (int i = 0; i < rhs.capacity; ++i) {
        if (rhs.table[i].offset != -1) {
            table[i] = rhs.table[i];
        }
    }
}

// Move constructor
template <typename K, typename V, typename H>
HashMap<K, V, H>::HashMap(HashMap<K, V>&& rhs)
        : table_size(rhs.table_size), capacity(rhs.capacity), load_factor(rhs.load_factor), hash_func(rhs.hash_func), table(rhs.table) {
    rhs.table = nullptr;
    rhs.table_size = 0;
    rhs.capacity = 0;
    rhs.load_factor = 0;
}

// Copy assignment operator
template <typename K, typename V, typename H>
HashMap<K, V>& HashMap<K, V, H>::operator=(const HashMap<K, V>& rhs) {
    if (this != &rhs) {
        delete[] table;
        table = new entry[rhs.capacity];
        for (int i = 0; i < rhs.capacity; ++i) {
            if (rhs.table[i].offset != -1) {
                table[i] = rhs.table[i];
            }
        }
        table_size = rhs.table_size;
        capacity = rhs.capacity;
        load_factor = rhs.load_factor;
        hash_func = rhs.hash_func;
    }
    return *this;
}

// Move assignment operator
template <typename K, typename V, typename H>
HashMap<K, V>& HashMap<K, V, H>::operator=(HashMap<K, V>&& rhs) {
    if (this != &rhs) {
        delete[] table;
        table = rhs.table;
        table_size = rhs.table_size;
        capacity = rhs.capacity;
        load_factor = rhs.load_factor;
        hash_func = rhs.hash_func;
        rhs.table = nullptr;
        rhs.table_size = 0;
        rhs.capacity = 0;
        rhs.load_factor = 0;
    }
    return *this;
}

// Strictly for testing purposes
template <typename K, typename V, typename H>
void HashMap<K, V, H>::check_contents(int idx) {
    std::cout << "Offset: " << table[idx].offset << std::endl;
    std::cout << "Key: " << table[idx].key << std::endl;
    std::cout << "Value: " << table[idx].value << std::endl;
    std::cout << std::endl;
}

// For testing
template <typename K, typename V, typename H>
void HashMap<K, V, H>::print_table_size() {
    std::cout << table_size << std::endl;
    std::cout << static_cast<double>(table_size) / static_cast<double>(capacity) << std::endl;
}

// Insert
template <typename K, typename V, typename H>
void HashMap<K, V, H>::insert(const K& key, const V& value) {
    if (((static_cast<double>(table_size + 1)) / (static_cast<double>(capacity))) > load_factor) {
        resize(nextPrime(2 * capacity));
    }
    int currentLocation = hash(key);
    entry probing_entry = entry(key, value, 0);
    while (true) {
        if (table[currentLocation].offset == -1) {
            table[currentLocation] = probing_entry;
            table_size += 1;
            return;
        } else {
            if (probing_entry.key == table[currentLocation].key) {
                throw std::logic_error("No duplicate keys");
            }
            if (probing_entry.offset > table[currentLocation].offset) {
                std::swap(table[currentLocation], probing_entry);
            }
        }
        probing_entry.offset += 1;
        currentLocation = (currentLocation + 1) % capacity;
    }
}

// Strictly for testing
template <typename K, typename V, typename H>
void HashMap<K, V, H>::insert_no_except_for_testing(const K& key, const V& value) {
    if (((static_cast<double>(table_size + 1)) / (static_cast<double>(capacity))) > load_factor) {
        resize(nextPrime(2 * capacity));
    }
    int currentLocation = hash(key);
    entry probing_entry = entry(key, value, 0);
    while (true) {
        if (table[currentLocation].offset == -1) {
            table[currentLocation] = probing_entry;
            table_size += 1;
            return;
        } else {
            if (probing_entry.key == table[currentLocation].key) {
                return;
            }
            if (probing_entry.offset > table[currentLocation].offset) {
                std::swap(table[currentLocation], probing_entry);
            }
        }
        probing_entry.offset += 1;
        currentLocation = (currentLocation + 1) % capacity;
    }
}

// Find
template <typename K, typename V, typename H>
std::optional<V> HashMap<K, V, H>::find(const K& key) {
    int currentLocation = hash(key);
    int currOffset{0};
    while (true) {
        if (table[currentLocation].offset == -1) {
            std::cout << "Key not found." << std::endl;
            return std::nullopt;
        } else {
            if (table[currentLocation].key == key) {
                return table[currentLocation].value;
            } else if (table[currentLocation].offset < currOffset) {
                std::cout << "Key not found due to cletus's trick." << std::endl;
                return std::nullopt;
            }
        }
        currOffset += 1;
        currentLocation = (currentLocation + 1) % capacity;
    }
}

// Remove
template <typename K, typename V, typename H>
void HashMap<K, V, H>::remove(const K& key) {
    auto find_remove = remove_find(key);
    if (!find_remove.has_value()) {
        throw std::logic_error("Can't remove non-existent key.");
    } else {
        int remove_idx = find_remove.value();
        table[remove_idx].offset = -1;
        table_size -= 1;
        while (table[(remove_idx + 1) % capacity].offset != 0 && table[(remove_idx + 1) % capacity].offset != -1) {
            std::swap(table[remove_idx], table[(remove_idx + 1) % capacity]);
            table[remove_idx].offset -= 1;
            remove_idx += 1;
        }
    }
}

// Strictly for testing
template <typename K, typename V, typename H>
void HashMap<K, V, H>::remove_no_except(const K& key) {
    auto find_remove = remove_find(key);
    if (!find_remove.has_value()) {
        return;
    } else {
        int remove_idx = find_remove.value();
        table[remove_idx].offset = -1;
        table_size -= 1;
        while (table[(remove_idx + 1) % capacity].offset != 0 && table[(remove_idx + 1) % capacity].offset != -1) {
            std::swap(table[remove_idx], table[(remove_idx + 1) % capacity]);
            table[remove_idx].offset -= 1;
            remove_idx += 1;
        }
    }
}

// Size query
template <typename K, typename V, typename H>
int HashMap<K, V, H>::size() const {
    return table_size;
}

// Empty query
template <typename K, typename V, typename H>
bool HashMap<K, V, H>::empty() const {
    return table_size == 0;
}

// MAX offset function
template <typename K, typename V, typename H>
int HashMap<K, V, H>::max_offset() {
    int max{0};
    for (int i = 0; i < capacity; ++i) {
        if (table[i].offset > max) {
            max = table[i].offset;
        }
    }
    return max;
}

// AVG offset function
template <typename K, typename V, typename H>
double HashMap<K, V, H>::average_offset() {
    double total{0.0};
    for (int i = 0; i < capacity; ++i) {
        if (table[i].offset != -1) {
            total += table[i].offset;
        }
    }
    return total / table_size;
}

// Check Bubba property
template <typename K, typename V, typename H>
bool HashMap<K, V, H>::check_robinhood_property() {
    for (int i = 0; i < capacity; i++) {
        if (table[i].offset != -1) {
            int counter = 0;
            while (i + counter + 1 < capacity) {
                if (table[i + counter + 1].offset == -1 || table[i + counter + 1].offset == 0) {
                    break;
                } else if ((table[i + counter + 1].offset - table[i + counter].offset) > 1) {
                    return false;
                } else {
                    counter += 1;
                }
            }
        }
    }
    return true;
}

// Constructor for entry
template <typename K, typename V, typename H>
HashMap<K, V, H>::entry::entry(const K& k, const V& v, int off)
        : key(k), value(v), offset(off) {}

// Hash function
template <typename K, typename V, typename H>
int HashMap<K, V, H>::hash(const K& k) {
    return hash_func(k) % capacity;
}

// Resize function
template <typename K, typename V, typename H>
void HashMap<K, V, H>::resize(int new_size) {
    entry* new_table = new entry[new_size];
    int old_capacity = capacity;
    capacity = new_size;

    for (int i = 0; i < old_capacity; i++) {
        if (table[i].offset != -1) {
            int currentLocation = hash(table[i].key);
            entry probing_entry = std::move(table[i]);
            probing_entry.offset = 0;
            while (true) {
                if (new_table[currentLocation].offset == -1) {
                    new_table[currentLocation] = std::move(probing_entry);
                    break;
                } else {
                    if (probing_entry.offset > new_table[currentLocation].offset) {
                        std::swap(new_table[currentLocation], probing_entry);
                    }
                }
                probing_entry.offset += 1;
                currentLocation = (currentLocation + 1) % capacity;
            }
        }
    }

    delete[] table;
    table = new_table;
}

// Function to test if a number is prime
template <typename K, typename V, typename H>
bool HashMap<K, V, H>::isPrime(int n) {
    if (n == 2 || n == 3)
        return true;
    if (n == 1 || n % 2 == 0)
        return false;
    for (int i = 3; i * i <= n; i += 2)
        if (n % i == 0)
            return false;
    return true;
}

// Function to return next prime
template <typename K, typename V, typename H>
int HashMap<K, V, H>::nextPrime(int n) {
    if (n % 2 == 0)
        ++n;
    for (; !isPrime(n); n += 2)
        ;
    return n;
}

// Helper function for remove
template <typename K, typename V, typename H>
std::optional<int> HashMap<K, V, H>::remove_find(const K& key) {
    int currentLocation = hash(key);
    int currOffset{0};
    while (true) {
        if (table[currentLocation].offset == -1) {
            return std::nullopt;
        } else {
            if (table[currentLocation].key == key) {
                return currentLocation;
            } else if (table[currentLocation].offset < currOffset) {
                return std::nullopt;
            }
        }
        currOffset += 1;
        currentLocation = (currentLocation + 1) % capacity;
    }
}