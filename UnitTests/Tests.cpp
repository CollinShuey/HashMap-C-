#include "HashMap.h"
#include "timer.h"
#include <chrono>
#include <iostream>
#include <unordered_map>
#include <random>
#include <utility>
#include <limits>


std::random_device rd;
std::uniform_int_distribution<> dis(1, 1000);


using namespace std;

const std::string ANSI_COLOR_RED = "\033[31m";
const std::string ANSI_COLOR_GREEN = "\033[32m";
const std::string ANSI_COLOR_YELLOW = "\033[33m";
const std::string ANSI_COLOR_BLUE = "\033[34m";
const std::string ANSI_COLOR_RESET = "\033[0m";

void insert_existing_key() {
    cout << "Here I create an Hashmap<int,int> with capacity 10 and insert two entries with the same keys" << endl;
    cout << "I chose to throw an exception when this happens so a logic error exception should be thrown when this function runs" << endl;
    HashMap<int,int> h1 = {10, .5};
    h1.insert(5,12);
    h1.insert(5,27);
}
void remove_on_empty() {
    cout << "Here I create a Hashmap<int,int> and insert nothing, if I try to remove something an exception will be thrown as that is not allowed" << endl;
    HashMap<int,int> h1 = {10, .5};
    h1.remove(3);

}
void find_on_empty() {
    cout << "Here I create a Hashmap<int,int> and insert nothing, find returns an optional that should contain no value and not return an int" << endl;
    cout << "I assign a variable" << ANSI_COLOR_GREEN << " i" << ANSI_COLOR_RESET <<   " to = h1.find(12) and im going to cout i.has_value() which should print 0 for false!" << endl;
    HashMap<int,int> h1 = {10, .5};
    auto i = h1.find(12);
    cout << i.has_value() << endl << endl;
}
void insert_test() {
    cout << "Here I create a Hashmap<int,int> with capacity 10, I insert 5 which because it has capacity 10 will be placed at index 5 with offset 0" << endl;
    cout << "Then I insert 55 which will have to probe over to index 6 because 5 is taken and will have offset 1, lets confirm this first" << endl;
    HashMap<int,int> h1 = {10, .5};
    h1.insert(5,10);
    h1.insert(55,10);
    cout << "index 5 contains: " << endl;
    h1.check_contents(5);
    cout << "index 6 contains: " << endl;
    h1.check_contents(6);
    cout << "Now we know that works if we insert 4 to get index 4 with offset 0 and then insert 44 to probe alone to index 5 which has a lower offset (0)" << endl;
    cout << "compared to what 44 would have if it was placed there so they will put 44 there with offset 1 and probe forward with 5 which has same offset" << endl;
    cout << "of 55 so it will probe forward into index 7 with offset 2, lets confirm this" << endl;
    h1.insert(4,10);
    h1.insert(44,10);
    cout << "index 4 contains: " << endl;
    h1.check_contents(4);
    cout << "index 5 contains: " << endl;
    h1.check_contents(5);
    cout << "index 6 contains: " << endl;
    h1.check_contents(6);
    cout << "index 7 contains: " << endl;
    h1.check_contents(7);

    cout << "Now that is confirmed lets remove one so then I can insert two more without needing to resize just this second" << endl;
    h1.remove(5);
    cout << "We need to make sure the wrap around works so I'm going to insert 9 so it maps to idx 9 and then 99 so then that probes to idx 0" << endl;
    h1.insert(9,10);
    h1.insert(99,10);
    cout << "with offset 1" << endl;
    cout << "index 9 contains: " << endl;
    h1.check_contents(9);
    cout << "index 0 contains: " << endl;
    h1.check_contents(0);
    cout << "Now that is confirmed we must test resize, resize will find the next biggest prime number and make that the new capacity of the new array" << endl;
    cout << "It should contain all the same elements but most likely in different places has it needed to rehash all of them according to new capacity" << endl;
    cout << "So I will check using find on the ones I know are in there and check if they are in same location after I insert 8 which will make our lf > .5" << endl;
    h1.insert(88,10);

    cout << "Couting all 6 finds, should all cout 1 for true: " << endl;
    cout << h1.find(4).has_value() << endl;
    cout << h1.find(44).has_value() << endl;
    cout << h1.find(9).has_value() << endl;
    cout << h1.find(99).has_value() << endl;
    cout << h1.find(88).has_value() << endl;
    cout << h1.find(55).has_value() << endl;
    cout << "88 shouldn't have mapped to index 8 now because the next prime*2 of 10 is 23 so its 88 % 23 = 19 so index 19 should have 88 lets see" << endl;
    cout << "index 19 contains: " << endl;
    h1.check_contents(19);
    cout << "Correct" << endl << endl;
}
void find_test() {
    cout << "find is straight forward but we need to check if 'Robin' trick works. We could just stop until we find a empty slot or when we find" << endl;
    cout << "key we are looking for that DOESN'T have offset -1, but that would be a waste of time as Robin's trick says we can stop looking when" << endl;
    cout << "the thing we are looking for offset is higher then the offset of whatever idx you are at because if the thing you are looking for existed" << endl;
    cout << "those values would have been swapped, we must show this is true as well, first I create hashmap<int,int> with cap 10 and lf = .7" << endl;
    cout << "then I insert 4 (I insert 4 with value 10), 44, 54, 33, in that order if I search for 64, it should find me nothing but should also cout 'key not found'" << endl;
    cout << "because it should have to probe through 4,44,54 until it reaches empty, but if i try to find 3 it should cout 'key not found because Robin Trick'"  <<endl;
    cout << "because i put debugging code to test to see which way it breaks, I would obviously remove this before production, but it should test 3 first" <<endl;
    cout << "see its occupied but then scan 4 but see that has offset 0 when itself would have offset 1 and break" << endl;

    HashMap<int,int> h1{10, .7};
    h1.insert(4,10);
    h1.insert(44,10);
    h1.insert(54,10);
    h1.insert(33,10);
    cout << "Result for find 64: " << h1.find(64).has_value() << endl;
    cout << "Result for find 3: " << h1.find(3).has_value() << endl;
    cout << "Note this also printed 0 for false for both of them" << endl;

    cout << "Lets find something we know is in and make sure it returns true for the optional.has_value()" << endl;
    cout << "Result for finding 4: " << h1.find(4).has_value() << " It should also return a value for .value() so Value: " << h1.find(4).value() << endl;
    cout << "It couted the correct value with correlating key" << endl;
}
void remove_test() {
    cout << "This is also straightforward but we need to check we implemented the 'Robin' part correctly We walk until we find empty spot " << endl;
    cout << "And we can resuse the find code for this as we should also stop the value we are looking for offsets in said slot is higher then the " << endl;
    cout << "current value in the slot offset, then when we remove, we should actually move everything to the right over until we find empty slot" << endl;
    cout << "or a value with offset 0, then that last spot we should we moved an item over from mark it empty with offset= -1 and we are done" << endl;
    cout << "Im going to create hashmap<int,int> cap 10 with lf .7 then insert 4,44,54,7, in that order, when we remove 44, we should move over 54" << endl;
    cout << "but not 7, and where 54 was (index 6) should be set to -1 for offset, and each one shifted over should have one less offset." << endl;
    HashMap<int,int> h1 {10, .7};
    h1.insert(4,10);
    h1.insert(44,10);
    h1.insert(54,10);
    h1.insert(7,10);
    h1.remove(44);
    cout << "index 4 contains: " << endl;
    h1.check_contents(4);
    cout << "index 5 contains: " << endl;
    h1.check_contents(5);
    cout << "index 6 contains: " << endl;
    h1.check_contents(6);
    cout << "index 7 contains: " << endl;
    h1.check_contents(7);
    cout << "that works" << endl;
}

void std_map_timing_test_insert() {
    cout << "Here I test the std::unorded_map to see how fast it handles 10,000 inserts" <<endl;
    cout << "I got an average 1300 microseconds" << endl;
    auto start = std::chrono::system_clock::now();
    unordered_map<int,int> u_map;
    for (int i = 0; i < 10000; i++) {
        u_map.insert({dis(rd),1});
    }
    auto end = std::chrono::system_clock::now();

    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    cout << "unordered duration of 10000 inserts: " << duration.count() << endl;
}
void robinhood_timing_test_insert() {
    cout << "Here I test the my RobinHashmap to see how fast it handles 10,000 inserts" <<endl;
    cout << "I got 750 microseconds usually. Does this mean I'm a better programmer then Bjarne?" << endl;
    auto start = std::chrono::system_clock::now();
    HashMap<int,int> u_map {101, .5};
    for (int i = 0; i < 10000; i++) {
        u_map.insert_no_except_for_testing(dis(rd),1);
    }
    auto end = std::chrono::system_clock::now();

    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    cout << "Robin hash map duration of 10000 inserts: " << duration.count() << endl;
    cout << endl;
    cout << "Lets see what happens if we increase our lf by 20%" << endl;
    auto start2 = std::chrono::system_clock::now();
    HashMap<int,int> u_map2 {101, .7};
    for (int i = 0; i < 10000; i++) {
        u_map.insert_no_except_for_testing(dis(rd),1);
    }
    auto end2 = std::chrono::system_clock::now();

    auto duration2 = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    cout << "Bubba hash map<101,.7> duration of 10000 inserts: " << duration.count() << endl;
    cout << "I end up getting even lower, but when I tried .8 load factor I started climbing in time" << endl;
    cout << "So we can definitely increase from .5 load factor to waste less space but anything above .7 might not be worth" << endl;

}
void std_map_timing_test_insert_and_remove() {
    cout << "Here I test the std u map to see how fast it handles 10000 inserts and 10000 removes" <<endl;
    cout << "Got around 2300 micro seconds each time" << endl;
    auto start = std::chrono::system_clock::now();
    unordered_map<int,int> u_map;
    for (int i = 0; i < 10000; i++) {
        u_map.insert({dis(rd),1});
    }
    for (int i = 0; i < 10000; i++) {
        u_map.erase(dis(rd));
    }
    auto end = std::chrono::system_clock::now();

    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    cout << "unordered duration of 10000 inserts and 10000 removes: " <<duration.count() << endl;
}
void RobinHashMap_timing_test_insert_and_remove() {
    cout << "Here I test 10000 inserts then 10000 removes on my hash map" <<endl;
    cout << "I get around 1500 micro seconds each time, still beating Bjarne!" << endl;
    auto start = std::chrono::system_clock::now();
    HashMap<int,int> u_map;
    for (int i = 0; i < 10000; i++) {
        u_map.insert_no_except_for_testing(dis(rd),1);
    }
    for (int i = 0; i < 10000; i++) {
        u_map.remove_no_except(dis(rd));
    }
    auto end = std::chrono::system_clock::now();

    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    cout << "my hash map duration of 10000 inserts and 10000 removes: " <<duration.count() << endl;
    cout << "We can almost for certain say that my Hash map is implemented correctly here because our timing is on par for what it should be." << endl;
}

void robinhood_property_test() {
    cout << "Here I made a member function just to test the robinhood property to truly see if it works on large size of 10000, It returns 1 for true and 0 for false" << endl;
    cout << "It is because the Robin Hashing property ensures the offsets of the elements to the right of any element you are looking at, unless its empty, is to increase by 1" << endl;
    cout << "Or be the same, it obviously stops counting if it hits 0 because that means that element is in right spot." << endl;
    HashMap<int,int> u_map;
    for (int i = 0; i < 10000; i++) {
        u_map.insert_no_except_for_testing(dis(rd),1);
    }
    bool i = u_map.check_robinhood_property();
    cout << "Result: " << i << endl;
}

void copy_constructor_test() {
    cout << "Here I create one hashmap and insert 2 and 4 into it, copy constuct one out of that one and then change the original one to have something" << endl;
    cout << "at idx 3 but that change shouldnt carry over to the copy constructed one" << endl;

    HashMap<int,int> h1 {10, .5};
    h1.insert(2,10);
    h1.insert(4,10);
    HashMap<int,int> h2 = h1;
    h1.insert(3,10);
    cout << "First hash map: " << endl;
    cout << "index 2 contains: " << endl;
    h1.check_contents(2);
    cout << "index 3 contains: " << endl;
    h1.check_contents(3);
    cout << "index 4 contains: " << endl;
    h1.check_contents(4);
    cout << "Second hash map: " << endl;
    cout << "index 2 contains: " << endl;
    h2.check_contents(2);
    cout << "index 3 contains: " << endl;
    h2.check_contents(3);
    cout << "index 4 contains: " << endl;
    h2.check_contents(4);


}

void copy_assignment_test(){
    cout << "First we create two hash maps first one insert 4 and second one insert 3, then copy first one into second one and then add 5 to first one" << endl;
    cout << "the first one should be the only one that has 5 in it and the second should have 4 because it copied over" << endl;
    HashMap<int,int> h1 {10, .5};
    HashMap<int,int> h2 {10, .5};
    h1.insert(4,10);
    h2.insert(3,10);
    h2 = h1;
    h1.insert(5,10);
    cout << "First hash map: " << endl;
    cout << "index 4 contains: " << endl;
    h1.check_contents(4);
    cout << "index 5 contains: " << endl;
    h1.check_contents(5);

    cout << "second hash map: " << endl;

    cout << "index 4 contains: " << endl;
    h1.check_contents(4);
    cout << "index 5 contains: " << endl;
    h2.check_contents(5);

}


void move_assignment_test() {
    cout << "For move Assignment it I inserted 4 into the first hash map and then made a second one and inserted 3, then move assign the first one into second" << endl;
    cout << "Then the first hash map should be deleted, which if you try to print it, it gives you segmentation fault, and the second one should have 4 but not 3 now" << endl;
    HashMap<int,int> h1 {10, .5};
    h1.insert(4,10);
    HashMap<int,int> h2 {10, .5};
    h2.insert(3,10);
    h2 = std::move(h1);



    cout << "Second hash map: " << endl;
    cout << "index 3 contains: " << endl;
    h2.check_contents(3);
    cout << "index 4 contains: " << endl;
    h2.check_contents(4);


}

void move_copy_test() {
    cout << "This is the move copy test, first I initialized a hashmap and inserted something at key 4. Then said h2 = move(h1) if you check the contents" << endl;
    cout << "of h2 at key 4 it will hold the given value, and if you try to check in to h1 (the original) it will give error 139 as we deleted the memory."  << endl;

    HashMap<int,int> h1{10,.5};
    h1.insert(4,10);
    HashMap<int,int> h2  = std::move(h1);
    cout << "h2 contents after move" << endl;
    h2.check_contents(4);
    cout << "h1 contents after move" << endl;
    h1.check_contents(4);
}

void best_lf_test_for_nsize(int size) {
    double lf = 0.1;
    double min = std::numeric_limits<double>::infinity();
    double min_lf = std::numeric_limits<double>::infinity();
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(1, 10000);

    const int num_iterations = 10;
    const int lf_tests = 10;

    for (int i = 1; i <= lf_tests; ++i) {
        double total_duration = 0.0;

        for (int j = 0; j < num_iterations; ++j) {
            HashMap<int, int> h1{size, lf};
            auto start = std::chrono::system_clock::now();

            for (int k = 0; k < size; ++k) {
                h1.insert_no_except_for_testing(dis(gen), 1);
            }
            for (int k = 0; k < size; ++k) {
                h1.remove_no_except(dis(gen));
            }
            auto end = std::chrono::system_clock::now();

            total_duration += std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
        }

        double avg_duration = total_duration / num_iterations;
        if (avg_duration < min) {
            min = avg_duration;
            min_lf = lf;
        }

        lf += 0.1; // Increment load factor for next test
    }

    std::cout << "Minimum average time: " << min << " microseconds, Best load factor: " << min_lf << std::endl;
}

void compare_std_vs_HashMap(int test_iterations,int size){

    long long total_time_umap = 0.0;
    for (int i = 0;i < test_iterations;i++){
        auto start = std::chrono::system_clock::now();
        unordered_map<int,int> u_map;
        for (int j = 0; j < size; j++) {
            u_map.insert({dis(rd),1});
        }
        for (int k = 0; k < size; k++) {
            u_map.erase(dis(rd));
        }
        auto end = std::chrono::system_clock::now();

        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
        total_time_umap += duration.count();
    }
    long long total_robin_time = 0.0;
    for(int n = 0;n < test_iterations;n++){
        auto start = std::chrono::system_clock::now();
        HashMap<int,int> u_map;
        for (int i = 0; i < size; i++) {
            u_map.insert_no_except_for_testing(dis(rd),1);
        }
        for (int i = 0; i < size; i++) {
            u_map.remove_no_except(dis(rd));
        }
        auto end = std::chrono::system_clock::now();

        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
        total_robin_time += duration.count();
    }

    double average_umap_time = static_cast<double>(total_time_umap) / test_iterations;
    double average_robin_time = static_cast<double>(total_robin_time) / test_iterations;
    double percentage = ((average_umap_time - average_robin_time) / average_umap_time) * 100;
    cout << "Average std unordered map timing: "<< average_umap_time << endl;
    cout << "Average RobinHashing map timing: " << average_robin_time << endl;
    cout << "Percent faster: " << percentage << "%" << endl;

}

int main(){
    /**
     * Through much of this testing code I used a hashmap with a capacity of 10 to make things simpler to show and to think about. I also demonstrate
     * much of the implementation with a hashmap that has ints for both keys and values for simplicity but do have some other tests testing other data types.
     * When comparing it against the std unordered map I use the default capacity and different load factors to see how it compares to the std one.
     */
    // I recomend viewing these one at a time
//    insert_existing_key();
//    remove_on_empty();
//    find_on_empty();
//    insert_test();
//    find_test();
//    remove_test();
    /*
     * for these timing tests I used a different insert and remove function to not throw an exception when duplicate because I insert random numbers and remove
     * random nums. I still believe throwing exceptions is the way to go as most code ran isn't watched by humans so I think its better to gracefully crash someones
     * code rather than them to think its working when its not... I would remove these extra functions before production.
     */
//    std_map_timing_test_insert();
//    bubba_map_timing_test_insert();
//    std_map_timing_test_insert_and_remove();
//    BubbaHashMap_timing_test_insert_and_remove();
//    robinhood_property_test();
//    copy_constructor_test();
//    copy_assignment_test();
//    move_assignment_test();
//    move_copy_test();
//    best_lf_test_for_nsize(10000000);
    compare_std_vs_HashMap(10,10000000);
    //For Larger sizes, my hashmap is around 30% faster than the c++ standard library unordered map
    // For smaller ones its around 30%-50% faster. The best Load Factor for my Hashmap is somewhere
    // around 0.5. However, it depends on if your going to be doing more inserting compared to removing
    // or vice versa. It also depends on the size of the hash map.
    return 0;


};



