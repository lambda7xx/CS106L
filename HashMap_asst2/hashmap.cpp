/*
* Assignment 2: HashMap template implementation (BASIC SOLUTION)
*      Notes: this file is what we call a .tpp file. It's not a .cpp file,
*      because it's not exactly a regular source code file. Instead, we are
*      defining the template definitions for our HashMap class.
*
*      TODO: write a comment here.
*
*      You'll notice that the commenting provided is absolutely stunning.
*      It was really fun to read through the starter code, right?
*      Please emulate this commenting style in your code, so your grader
*      can have an equally pleasant time reading your code. :)
*/

#include "hashmap.h"
#include <hashmap_iterator.h>


template <typename K, typename M, typename H>
HashMap<K, M, H>::HashMap() : HashMap(kDefaultBuckets) { }

template <typename K, typename M, typename H>
HashMap<K, M, H>::HashMap(size_t bucket_count, const H& hash) :
   _size(0),
   _hash_function(hash),
   _buckets_array(bucket_count, nullptr) { }

template <typename K, typename M, typename H>
HashMap<K, M, H>::~HashMap() {
   clear();
}

template <typename K, typename M, typename H>
inline size_t HashMap<K, M, H>::size() const noexcept {
   return _size;
}

template <typename K, typename M, typename H>
inline bool HashMap<K, M, H>::empty() const noexcept {
   return size() == 0;
}

template <typename K, typename M, typename H>
inline float HashMap<K, M, H>::load_factor() const noexcept {
   return static_cast<float>(size())/bucket_count();
};

template <typename K, typename M, typename H>
inline size_t HashMap<K, M, H>::bucket_count() const noexcept {
   return _buckets_array.size();
};

template <typename K, typename M, typename H>
bool HashMap<K, M, H>::contains(const K& key) const noexcept {
   return find_node(key).second != nullptr;
}

template <typename K, typename M, typename H>
void HashMap<K, M, H>::clear() noexcept {
   for (auto& curr : _buckets_array) {
       while (curr != nullptr) {
           auto trash = curr;
           curr = curr->next;
           delete trash;
       }
   }
   _size = 0;
}

template <typename K, typename M, typename H>
std::pair<typename HashMap<K, M, H>::value_type*, bool>
           HashMap<K, M, H>::insert(const value_type& value) {
   const auto& [key, mapped] = value;
   auto [prev, node_to_edit] = find_node(key);
   size_t index = _hash_function(key) % bucket_count();

   if (node_to_edit != nullptr) return {&(node_to_edit->value), false};
   _buckets_array[index] = new node(value, _buckets_array[index]);

   ++_size;
   return {&(_buckets_array[index]->value), true};
}

template <typename K, typename M, typename H>
M& HashMap<K, M, H>::at(const K& key)   {
   auto [prev, node_found] = find_node(key);
   if (node_found == nullptr) {
       //value_type val = {key ,{}};
       //return val.second;
       throw std::out_of_range("HashMap<K, M, H>::at: key not found");
   }
   return node_found->value.second;
}

template <typename K, typename M, typename H>
typename HashMap<K, M, H>::node_pair HashMap<K, M, H>::find_node(const K& key) const {
   size_t index = _hash_function(key) % bucket_count();
   auto curr = _buckets_array[index];
   node* prev = nullptr; // if first node is the key, return {nullptr, front}
   while (curr != nullptr) {
       const auto& [found_key, found_mapped] = curr->value;
       if (found_key == key) return {prev, curr};
       prev = curr;
       curr = curr->next;
   }
   return {nullptr, nullptr}; // key not found at all.
}

template <typename K, typename M, typename H>
void HashMap<K, M, H>::debug() const {
   std::cout << std::setw(30) << std::setfill('-') << '\n' << std::setfill(' ')
             << "Printing debug information for your HashMap implementation\n"
             << "Size: " << size() << std::setw(15) << std::right
             << "Buckets: " << bucket_count() << std::setw(20) << std::right
             << "(load factor: " << std::setprecision(2) << load_factor() << ") \n\n";

   for (size_t i = 0; i < bucket_count(); ++i) {
       std::cout << "[" << std::setw(3) << i << "]:";
       auto curr = _buckets_array[i];
       while (curr != nullptr) {
           const auto& [key, mapped] = curr->value;
           // next line will not compile if << not supported for K or M
           std::cout <<  " -> " << key << ":" << mapped;
           curr = curr->next;
       }
       std::cout <<  " /" <<  '\n';
   }
   std::cout << std::setw(30) << std::setfill('-') << '\n';
}

template <typename K, typename M, typename H>
bool HashMap<K, M, H>::erase(const K& key) {
    auto [prev, node_to_erase] = find_node(key);
    if (node_to_erase == nullptr) {
        return false;
    } else {
        size_t index = _hash_function(key) % bucket_count();
        (prev ? prev->next : _buckets_array[index]) = node_to_erase->next;
        delete node_to_erase;
        --_size;
        return true;
    }
}

template <typename K, typename M, typename H>
void HashMap<K, M, H>::rehash(size_t new_bucket_count) {
    if (new_bucket_count == 0) {
        throw std::out_of_range("HashMap<K, M, H>::rehash: new_bucket_count must be positive.");
    }

    std::vector<node*> new_buckets_array(new_bucket_count);
    /* Optional Milestone 1: begin student code */

        // Hint: you should NOT call insert, and you should not call
        // new or delete in this function. You must reuse existing nodes.
        //(void) new_buckets_array; // remove this line after you start implementing
    for (size_t i = 0; i < bucket_count(); ++i) { 
     auto curr = _buckets_array[i];
     while (curr != nullptr) {
        const auto& [key, mapped] = curr->value;
        std::cout <<  " -> " << key << ":" << mapped;
        size_t index = _hash_function(key) % new_bucket_count;//在新的bucket的索引
        new_buckets_array[index] = new  node(curr->value,new_buckets_array[index]);
        curr = curr->next;
    }
}
    _buckets_array.clear();
    _buckets_array = new_buckets_array;
    /* end student code */

}
//2A 
template <typename K, typename M, typename H> 
 M &  HashMap<K, M, H>::operator[](const K &key ){
   auto [prev, node_found] = find_node(key);
   if(node_found == nullptr) {
    //   std::cout<<"default in operaror"<<std::endl;
       value_type val = {key ,{}};
       std::pair<HashMap<K, M, H>::value_type*, bool> p = insert(val);
       return p.first->second;
   }
   return node_found->value.second;
}

template <typename K_, typename M_, typename H_>
    bool operator==(const HashMap<K_, M_, H_>& lhs,
       const HashMap<K_, M_, H_>& rhs){
    if(lhs.empty() != rhs.empty() || lhs.size() != rhs.size()) {
        return false;
    }
    size_t bucket_count = lhs.bucket_count();
    for(size_t i = 0;  i < bucket_count ; i++) {
        auto curr = lhs._buckets_array[i];
        while(curr != nullptr) {
            const auto& [key, mapped] = curr->value;
            if(rhs.contains(key) == false) {
                return false;
            }
            auto [prev, node_found] = rhs.find_node(key);
            if(node_found == nullptr) {
                return false;
            }
            if(node_found->value.second != mapped) {
                return false;
            }
            curr  =curr->next;
        }
    }
    return true;
}

 template <typename K_, typename M_, typename H_>
     bool operator!=(const HashMap<K_, M_, H_>& lhs,
             const HashMap<K_, M_, H_>& rhs){
    //bool res = operator==(lhs, rhs);
    return !(lhs == rhs);
}
template <typename K_, typename M_, typename H_>
    std::ostream& operator<<(std::ostream& os, const HashMap<K_, M_, H_>& map){
        if(map.empty()) {
            os<<"{}";
            return os;
        }
        os <<"{";
        bool first = false;
        size_t bucket_counts = map.bucket_count();
        for(size_t i = 0;  i < bucket_counts ; i++) {
                auto curr = map._buckets_array[i];
                 if(curr != nullptr && first ) {
                     os<<", ";
                 }
                while(curr != nullptr) {
                first = true;
                const auto& [key, value] = curr->value;
                os<<key<<":" << value ;
                curr  =curr->next;
                if(curr != nullptr) {
                    os<<", ";
                }
                }
        }
    os <<"}";
    return os;
}
//copy assignment
//考虑到一种情况，参数map和自己是同一个东西咋办
//如果这时候clear了，参数也没有了
template <typename K, typename M, typename H>
HashMap<K, M,H> & HashMap<K,M,H>::operator=(const HashMap<K, M, H> &map) {
   if(this == &map) {
       return *this;
   }
   clear();
   _hash_function = map._hash_function;
   _buckets_array.resize(map.bucket_count());
   size_t bucket_count = map.bucket_count();
   auto bucket_array = map._buckets_array;
   for(auto & curr : bucket_array) {
       while(curr != nullptr) {
           insert(curr->value);
           curr = curr->next;
       }
   }
   return *this;
}
//copy constructor
template <typename K, typename M, typename H> 
HashMap<K, M,H>::HashMap(const HashMap<K, M,H> & map) {
    //clear();
    if(this == &map) {
   }
    clear();
    _hash_function = map._hash_function;
   _buckets_array.resize(map.bucket_count());
   size_t bucket_count = map.bucket_count();
   auto bucket_array = map._buckets_array;
   for(auto & curr : bucket_array) {
       while(curr != nullptr) {
           insert(curr->value);
           curr = curr->next;
       }
   }
}

// move constructor
template <typename K, typename M, typename H>
HashMap<K, M, H>::HashMap(HashMap&& rhs)
:_size(std::move(rhs._size)),
 _hash_function(std::move(rhs._hash_function)),
 _buckets_array(std::move(rhs._buckets_array))
{
}

template <typename K, typename M, typename H> 
HashMap<K, M,H> & HashMap<K,M,H>::operator=(HashMap<K, M,H> && rhs){
 _size =std::move(rhs._size);
 _hash_function= std::move(rhs._hash_function);
 _buckets_array = std::move(rhs._buckets_array);
 return *this;
}
/*
    Milestone 2-3: begin student code

    Here is a list of functions you should implement:
    Milestone 2
        - operator[]
        - operator<<
        - operator== and !=
        - make existing functions const-correct

    Milestone 3
        - copy constructor
        - copy assignment
        - move constructor
        - move assignment
*/

// A_initializer_list_ctor
template <typename K, typename M, typename H> 
HashMap<K, M, H>::HashMap(std::initializer_list<value_type> values,size_t bucket_count , const H& hash)
:   _size(0),
   _hash_function(hash),
   _buckets_array(bucket_count, nullptr)  {
    for(auto value : values) {
        insert(value);
    }
}

//a range actor
template <typename K, typename M, typename H> 
template<typename Input> 
HashMap<K,M,H>::HashMap(Input fr, Input la):HashMap() {
 auto temp = fr;
 while(temp != la) {
     insert(*temp);
     temp++;
 }
}

template <typename K, typename M, typename H> 
typename HashMap<K, M, H>::iterator HashMap<K, M,H>::begin() noexcept {
    //find the node first is not empty 
    const size_t first_index = first_not_empty_bucket();
    node * node = _buckets_array[first_index];
    return HashMapIterator<HashMap,false>(node,_buckets_array,first_index);
}

template <typename K, typename M, typename H> 
typename HashMap<K, M, H>::iterator HashMap<K, M,H>::end() noexcept {
    return HashMapIterator<HashMap,false>(nullptr,_buckets_array,bucket_count());
}


//找到第一个桶
template <typename K, typename M, typename H>  
size_t HashMap<K,M,H>::first_not_empty_bucket() const noexcept {
    size_t  first_index = -1;
    const size_t bucket_counts = bucket_count();
    for(size_t i = 0;  i < bucket_counts ; i++) {
        auto curr = _buckets_array[i];
        if(curr != nullptr) {
            first_index = i;
            return first_index;
        }
    }   
  return first_index;
}

template <typename K, typename M, typename H> 
typename HashMap<K, M, H>::const_iterator HashMap<K, M,H>::begin() const  noexcept {
    //find the node first is not empty 
    const size_t first_index = first_not_empty_bucket();
    node * node = _buckets_array[first_index];
    return HashMapIterator<HashMap,true>(node,_buckets_array,first_index);
}

template <typename K, typename M, typename H> 
typename HashMap<K, M, H>::const_iterator HashMap<K, M,H>::end() const noexcept {
    return HashMapIterator<HashMap,true>(nullptr,_buckets_array,bucket_count());
}

