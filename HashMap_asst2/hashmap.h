/*
* Assignment 2: HashMap template interface
*      TODO: write a comment here.
*
*      You'll notice that the commenting provided is absolutely stunning.
*      It was really fun to read through the starter code, right?
*      Please emulate this commenting style in your code, so your grader
*      can have an equally pleasant time reading your code. :)
*/

#ifndef HASHMAP_H
#define HASHMAP_H

#include <initializer_list>
#include <iostream>             // for cout
#include <iomanip>              // for setw, setprecision, setfill, right
#include <sstream>              // for istringstream
#include <vector>               // for vector
#include "hashmap_iterator.h"

// add any other includes that are necessary

/*
* Template class for a HashMap
*
* K = key type
* M = mapped type
* H = hash function type used to hash a key; if not provided, defaults to std::hash<K>
*
* Notes: When dealing with the Stanford libraries, we often call M the value
* (and maps store key/value pairs).
*
* However, we name it M for mapped type to avoid confusion with value_type.
* value_type is what the container is storing, which is a std::pair<const K, M>.
*
* All STL containers have a value_type and STL algorithms may use the value_type alias,
* so we try our best to follow that convention.
*
* Example:
*      HashMap<std::string, int>
*      This means K = key = std::string,
*                 M = mapped = int,
*                 value_type = std::pair<const std::string, int>.
*
* Concept requirements:
*      - H is function type that takes in some type K, and outputs a size_t.
*      - K and M must be regular (copyable, default constructible, and equality comparable).
*/
template <typename K, typename M, typename H = std::hash<K>>
class HashMap {

public:

    /*
    * Alias for std::pair<const K, M>, used by the STL (such as in std::inserter)
    * As noted above, value_type is not the same as the mapped_type!
    *
    * Usage:
    *      HashMap::value_type val = {3, "Avery"};
    *      map.insert(val);
    */
    using value_type = std::pair<const K, M>;

    /*
    * Default constructor
    * Creates an empty HashMap with default number of buckets and hash function.
    *
    * Usage:
    *      HashMap map;
    *      HashMap map{};
    *
    * Complexity: O(B), B = number of buckets
    */
    HashMap();

    /*
    * Constructor with bucket_count and hash function as parameters.
    *
    * Creates an empty HashMap with a specified initial bucket_count and hash funciton.
    * If no hash function provided, default value of H is used.
    *
    * Usage:
    *      HashMap(10) map;
    *      HashMap map(10, [](const K& key) {return key % 10; });
    *      HashMap map{10, [](const K& key) {return key % 10; });
    *
    * Complexity: O(B), B = number of buckets
    *
    * Notes : what is explicit? Explicit specifies that a constructor
    * cannot perform implicit conversion on the parameters, or use copy-initialization.
    * That's good, as nonsense like the following won't compile:
    *
    * HashMap<int, int> map(1.0);  // double -> int conversion not allowed.
    * HashMap<int, int> map = 1;   // copy-initialization, does not compile.
    */
    explicit HashMap(size_t bucket_count, const H& hash = H());

    
    HashMap(std::initializer_list<value_type> values, size_t bucket_count = kDefaultBuckets, const H& hash = H());

    template<typename InPutLi> HashMap(InPutLi fr ,InPutLi ls);
    /*
    * Destructor.
    *
    * Usage: (implicitly called when HashMap goes out of scope)
    *
    * Complexity: O(N), N = number of elements
    */
    ~HashMap();

    /*
    * Returns the number of (K, M) pairs in the map.
    *
    * Parameters: none
    * Return value: size_t
    *
    * Usage:
    *      if (map.size() < 3) { ... }
    *
    * Complexity: O(1) (inlined because function is short)
    */
    inline size_t size() const noexcept;

    /*
    * Returns whether the HashMap is empty.
    *
    * Parameters: none
    * Return value: bool
    *
    * Usage:
    *      if (map.empty()) { ... }
    *
    * Complexity: O(1) (inlined because function is short)
    */
    inline bool empty() const noexcept;

    /*
    * Returns the load_factor, defined as size/bucket_count.
    *
    * Parameters: none
    * Return value: float
    *
    * Usage:
    *      float load_factor = map.load_factor();
    *
    * Complexity: O(1) (inlined because function is short)
    *
    * Notes: our minimal implementation does not automatically rehash when the load
    * factor is too high. If you want as an extension, you can implement automatic rehashing.
    */
    inline float load_factor() const noexcept;

    /*
    * Returns the number of buckets.
    *
    * Parameters: none
    * Return value: size_t - number of buckets
    *
    * Usage:
    *      size_t buckets = map.bucket_count();
    *
    * Complexity: O(1) (inlined because function is short)
    *
    * Notes: our minimal implementation does not automatically rehash when the load
    * factor is too high. If you want, you can implement automatic rehashing.
    *
    * What is noexcept? It's a guarantee that this function does not throw
    * exceptions, allowing the compiler to optimize this function further.
    * A noexcept function that throws an exception will automatically
    * terminate the program.
    */
    inline size_t bucket_count() const noexcept;

    /*
    * Returns whether or not the HashMap contains the given key.
    *
    * Parameters: const l-value reference to type K, the given key
    * Return value: bool
    *
    * Usage:
    *      if (map.contains("Avery")) { map.at("Avery"); ... }
    *
    * Complexity: O(1) amortized average case, O(N) worst case, N = number of elements
    *
    * Notes: Recall that when using a std::map, you use the map.count(key) function
    * (returns 0 or 1) to check if key exists. In C++20, map.contains(key) will be available.
    * Since contains feels more natural to students who've used the Stanford libraries
    * and will be available in the future, we will implement map.contains(key).
    */
    bool contains(const K& key) const noexcept;

    /*
    * Removes all K/M pairs the HashMap.
    *
    * Parameters: none
    * Return value: none
    *
    * Usage:
    *      map.clear();
    *
    * Complexity: O(N), N = number of elements
    *
    * Notes: clear removes all the elements in the HashMap and frees the memory associated
    * with those elements, but the HashMap should still be in a valid state and is
    * ready to be inserted again, as if it were a newly constructed HashMap with no elements.
    * The number of buckets should stay the same.
    */
    void clear() noexcept;

    /*
    * Inserts the K/M pair into the HashMap, if the key does not already exist.
    * If the key exists, then the operation is a no-op.
    *
    * Parameters: const l-value reference to value_type (K/M pair)
    * Return value:
    *          pair<value_type*, bool>, where:
    *              value_type* - pointer to the value_type inside a node that is just constructed.
    *              bool - true if the element was successfully added,
    *                      false if the element already existed.
    *
    * Note: in general it is a poor idea to return raw pointers to the internal nodes.
    * It is much better to return an iterator instead, which you can optionally do in milestone 8.
    *
    * Usage:
    *      HashMap<int, std::string> map;
    *      map.insert({3, "Avery"});        // inserts key = 3, value = "Avery"
    *      map.insert({3, "Anna"});         // key = 3 already exists, no-op
    *
    * Complexity: O(1) amortized average case
    */
    std::pair<value_type*, bool> insert(const value_type& value);

    /*
    * Erases a K/M pair (if one exists) corresponding to given key from the HashMap.
    * This is a no-op if the key does not exist.
    *
    * Parameters: const l-value reference to K, key to be removed.
    * Return value: true if K/M pair was found and removed, false if key was not found.
    *
    * Usage:
    *      map.erase(3);           // assuming K = int, erases element with key 3, returns true
    *
    * Complexity: O(1) amortized average case, O(N) worst case, N = number of elements
    *
    * Notes: after you implement iterators, a call to erase should maintain the
    * order of existing iterators, other than iterators to the erased K/M.
    */
    bool erase(const K& key);

    /*
    * Returns a l-value reference to the mapped value given a key.
    * If no such element exists, throws exception of type std::out_of_range.
    *
    * Parameters: key of type K.
    * Return value: l-value reference to type V, the mapped value of key.
    *
    * Usage:
    *      map.at(3) = "Avery";            // assuming {3, "Avery"} is in the map.
    *      std::string s = map.at(3);      // s = "Avery"
    *
    * Exceptions: std::out_of_range if key is not in the map.
    *
    * Complexity: O(1) amortized average case, O(N) worst case, N = number of elements
    *
    * Notes: recall that operator[], which you will implement, does not throw exceptions,
    * if a key is not found. Instead, it will create a K/M pair for that key with a default
    * mapped value.
    */
    M& at(const K& key)  ;

    /*
    * Function that will print to std::cout the contents of the hash table as
    * linked lists, and also displays the size, number of buckets, and load factor.
    *
    * Parameters: none
    * Return value: none
    *
    * Usage:
    *      map.debug();
    *
    * Complexity: O(N), N = number of elements.
    *
    * Notes: debug will not compile if either K or V does not support operator<< for std::ostream.
    * this function will crash if your linked list logic is incorrect (eg. forgot to reset the
    * last node's next to nullptr). Check where the source of the compiler error comes from
    * before complaining to us that our starter code doesn't work!
    *
    * Tip: place map.debug() in various places in the test cases to figure out which operation
    * is failing. Super useful when we debugged our code.
    */
    void debug() const;

    /*
    * Resizes the array of buckets, and rehashes all elements. new_buckets could
    * be larger than, smaller than, or equal to the original number of buckets.
    *
    * Parameters: new_buckets - the new number of buckets. Must be greater than 0.
    * Return value: none
    *
    * Usage:
    *      map.rehash(30)
    *
    * Exceptions: std::out_of_range if new_buckets = 0.
    *
    * Complexity: O(N) amortized average case, O(N^2) worst case, N = number of elements
    *
    * Notes: our minimal HashMap implementation does not support automatic rehashing, but
    * std::unordered_map will automatically rehash, even if you rehash to
    * a very small number of buckets. For this reason, std::unordered_map.rehash(0)
    * is allowed and forces an unconditional rehash. We will not require this behavior.
    * If you want, you could implement this.
    *
    * This function is incomplete, and for milestone 1 you should complete the implementation.
    * The test cases provided use a probabilistic time test. If the test fails very occasionally,
    * that is totally fine, as long as most times you run the test cases, it passes that test.
    */
    void rehash(size_t new_buckets);

    M& operator[](const K& key);
    
    //copy assignment 
    HashMap<K, M,H> & operator=(const HashMap<K, M, H>& map);  

        //copy constructor
    HashMap(const HashMap & map);
    // move constructor
    HashMap(HashMap && rhs);

    /*
      * Move assignment operator
      * Move the elements in the rhs r-value HashMap into the lhs HashMap
      * Requirements: the rhs HashMap should have the same <K, M, H>.
      *
      * Usage:
      * 		HashMap<char, int> rhs{{'a', 3}, {'b', 4}, {'c', 6}};
      * 		HashMap<char, int> map;
      * 		map = std::move(rhs); // now rhs should be empty
      *
      * Complexity: O(N), where N = rhs.size();
      */
    HashMap& operator=(HashMap&& rhs);

    friend class HashMapIterator<HashMap, false>;
    friend class HashMapIterator<HashMap,true>;

    using iterator = HashMapIterator<HashMap,false>;
    using const_iterator = HashMapIterator<HashMap ,true>;
     /*
    * Inserts the K/M pair into the HashMap, if the key does not already exist.
    * If the key exists, then the operation is a no-op.
    *
    * Parameters: const l-value reference to value_type (K/M pair)
    * Return value:
    *          pair<iterator, bool>, where:
    *              iterator - iterator to the value_type element with the given key
    *                         this element may have been just added, or may have already existed.
    *              bool - true if the element was successfully added,
    *                      false if the element already existed.
    *
    * Usage:
    *      HashMap<int, std::string> map;
    *      auto [iter1, insert1] = map.insert({3, "Avery"}); // inserts {3, "Avery"}, iter1 points to that element, insert1 = true
    *      auto [iter2, insert2] = map.insert({3, "Anna"});  // no-op, iter2 points to {3, "Avery"}, insert2 = false
    *
    * Complexity: O(1) amortized average case
    */
    std::pair<iterator, bool> insert(const value_type& value , bool flag) noexcept;
    /*
     * Returns an iterator to the first element.
     * This overload is used when the HashMap is non-const.
     *
     * Usage:
     *      auto iter = map.begin();
     */
    iterator begin() ;//?????????noexcept???????????????

    /*
     * Returns an iterator to one past the last element.
     * This overload is used when the HashMap is non-const.
     *
     * Usage:
     *      while (iter != map.end()) {...}
     */
    iterator end() ;

    /*
     * Returns a const_iterator to the first element.
     * This overload is used when the HashMap is const.
     *
     * Usage:
     *      auto iter = cmap.begin();
     */
    const_iterator begin() const ;

    /*
     * Returns an iterator to one past the last element.
     * This overload is used when the HashMap is const.
     *
     * Usage:
     *      while (iter != cmap.end()) {...}
     */
    const_iterator end() const  ;

        /*
    * Erases the K/M pair that pos points to.
    * Behavior is undefined if pos is not a valid and dereferencable iterator.
    *
    * Parameters: const_iterator pos, iterator to element to be removed
    * Return value: the iterator immediately following pos, which may be end().
    *
    * Usage:
    *       auto iter = map.find(3);
    *       auto next = map.erase(iter);    // erases element that iter is pointing to
    *
    * Complexity: O(1) amortized average case, O(N) worst case, N = number of elements
    *
    * Notes: a call to erase should maintain the order of existing iterators,
    * other than iterators to the erased K/M element.
    */
    iterator erase(const_iterator pos);
private:

    /*
    * node structure represented a node in a linked list.
    * Each node consists of a value_type (K/M pair) and a next pointer.
    *
    * This is implemented in the private section as clients should not be dealing
    * with anything related to the node struct.
    *
    * Usage;
    *      HashMap<K, M, H>::node n;
    *      n->value = {3, 4};
    *      n->next = nullptr;
    */
    struct node {
        value_type value;
        node* next;

        /*
        * Constructor with default values, so even if you forget to set next to nullptr it'll be fine.
        *
        * Usage:
        *      node* new_node = node({key, mapped}, next_ptr);
        */
        node(const value_type& value = value_type(), node* next = nullptr) :
            value(value), next(next) {}
    };

    /*
    * Type alias for a pair of node*'s.
    *
    * This is used in find_node.
    *
    * Usage:
    *      auto& [prev, curr] = node_pair{nullptr, new node()};
    */
    using node_pair = std::pair<typename HashMap::node*, typename HashMap::node*>;

    /*
    * Finds the node N with given key, and returns a node_pair consisting of
    * the node whose's next is N, and N. If node is not found, {nullptr, nullptr}
    * is returned. If node found is the first in the list, {nullptr, node} is returned.
    *
    * Example given list: front -> [A] -> [B] -> [C] -> /
    * where A, B, C, D are pointers, then
    *
    * find_node(A_key) = {nullptr, A}
    * find_node(B_key) = {A, B}
    * find_node(C_key) = {B, C}
    * find_node(D_key) = {nullptr, nullptr}
    *
    * Usage:
    *      auto& [prev, curr] = find_node(3);
    *      if (prev == nullptr) { ... }
    *
    * Complexity: O(1) amortized average case, O(N) worst case, N = number of elements
    *
    * Notes: this function is necessary because when erasing, we need to change the
    * next pointer of the node before the one we are erasing.
    */
    node_pair find_node(const K& key) const;

    /* Private member variables */

    /*
    * instance variable: _size, the number of elements, which are K/M pairs.
    * Don't confuse this with the number of buckets!
    */
    size_t _size;

    /*
    * instance variable: _hash_function, a function (K -> size_t) that is used
    * to hash K's to determine which bucket they should be inserted/found.
    *
    * Remember to mod the output of _hash_function by _bucket_count!
    *
    * Usage:
    *      K element = // something;
    *      size_t index = _hash_function(element) % _bucket_count;
    *
    */
    H _hash_function;

    /*
    * The array (vector) of buckets. Each bucket is a linked list,
    * and the item stored in the bucket is the front pointer of that linked list.
    *
    * Usage:
    *      node* ptr = _buckets_array[index];          // _buckets_array is array of node*
    *      const auto& [key, mapped] = ptr->value;     // each node* contains a value that is a pair
    */
    std::vector<node*> _buckets_array;
    using _buckets_array_type = std::vector<node*>;
    /*
    * A constant for the default number of buckets for the default constructor.
    */
    static const size_t kDefaultBuckets = 10;

    template <typename K_, typename M_, typename H_>
    friend std::ostream& operator<<(std::ostream& os, const HashMap<K_, M_, H_>& map);

    template <typename K_, typename M_, typename H_>
    friend bool operator==(const HashMap<K_, M_, H_>& lhs,
       const HashMap<K_, M_, H_>& rhs);

    template <typename K_, typename M_, typename H_>
    friend bool operator!=(const HashMap<K_, M_, H_>& lhs,
       const HashMap<K_, M_, H_>& rhs);

    
    
    /*
    * Finds the first bucket in _buckets_array that is non-empty.
    *
    * Hint: on the assignment, you should NOT need to call this function.
    */
    size_t first_not_empty_bucket() const noexcept;

    /*
    * Creates an iterator that points to the element curr->value.
    *
    * Hint: on the assignment, you should NOT need to call this function.
    */
    iterator make_iterator(node* curr);

};



/*
* Ask compiler to put the template implementation here.
*
* Typically we'd just put everything (interface + implementation) in the .h file
* but the file got a bit too long with the comments, so we split it up.
*/
#include "hashmap.cpp"

#endif // HASHMAP_H
