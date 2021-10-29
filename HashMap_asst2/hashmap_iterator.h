#ifndef HASHMAPITERATOR_H
#define HASHMAPITERATOR_H
/* Milestone 6 (optional): implement iterators for your HashMap class */

#include <iterator>     // for std::forward_iterator_tag
#include <functional>   // for std::conditional_t
#include <vector>
// forward declaration for the HashMap class
template <typename K, typename M, typename H> class HashMap;

/*
* Template class for a HashMapIterator
*
* Map = the type of HashMap this class is an iterator for.
* IsConst = whether this is a const_iterator class.
*
* Concept requirements:
* - Map must be a valid class HashMap<K, M, H>
*/

template <typename Map, bool IsConst = true>
class HashMapIterator {
public:
        /*
     * This alias is very important. When dealing with const_iterator, the value_type is always const, and
     * that prevents the client from modifying the elements via a const_iterator. The meta-function
     * std::conditional_t changes the value_type (at compile-time) to a const one if IsConst is true.
     */
    using value_type        =   std::conditional_t<IsConst, const typename Map::value_type, typename Map::value_type>;

    /*
     * Public aliases for this iterator class. Important so STL functions like std::iterator_traits
     * can parse this class for important information, like its iterator category.
     */
    using iterator_category =   std::forward_iterator_tag;
    using difference_type   =   std::ptrdiff_t;
    using pointer           =   value_type*;
    using reference         =   value_type&;
        /*
     * Friend declarations so the HashMap class this iterator is for can access the attributes of its iterators.
     * Also, to make conversions between iterator and const_iterator easy, we declare the corresponding
     * iterator and const_iterators as friends.
     */
    friend Map;
    friend HashMapIterator<Map, true>;
    friend HashMapIterator<Map, false>;
       /*
     * Conversion operator: converts any iterator (iterator or const_iterator) to a const_iterator.
     *
     * Usage:
     *      iterator iter = map.begin();
     *      const_iterator c_iter = iter;    // implicit conversion
     *
     * Implicit conversion operators are usually frowned upon, because they can cause
     * some unexpected behavior. This is a rare case where a conversion operator is
     * very convenient. Many of the iterator functions in the HashMap class are
     * secretly using this conversion.
     *
     * Note: conversion in the opposite direction (const to non-const) is not safe
     * because that gives the client write access the map itself is const.
     */
    operator HashMapIterator<Map, true>() const {
        return HashMapIterator<Map, true>(node_,bucket_array_, bucket_index_);
        // HashMapIterator(node * node ,bucket_array_type bucket_array, size_t  bucket_index);
    }

    /*
     * Dereference operators: defines the behavior of dereferencing an iterator.
     *
     * Usage:
     *      auto [key, value] = *iter;
     *      auto value = iter->second;
     *      iter->second = 3; // if iter is a regular iterator (not a const_iterator)
     *
     * Note that dereferencing an invalid or end() iterator is undefined behavior.
     */
    reference operator*() const;
    pointer operator->() const;

    /*
     * Increment operators: moves the iterator to point to the next element, or end().
     *
     * Usage:
     *      ++iter;         // prefix
     *      iter++;         // postfix
     *
     * Note: the prefix operator first increments, and the returns a reference to itself (which is incremented).
     * The postfix operator returns a copy of the original iterator, while the iterator itself is incremented.
     *
     * Note that incrementing an invalid or end() iterator is undefined behavior.
     */
    HashMapIterator<Map, IsConst>& operator++();
    HashMapIterator<Map, IsConst> operator++(int);

    /*
     * Equality operator: decides if two iterators are pointing to the same element.
     *
     * Usage:
     *      if (iter == map.end()) {...};
     */
    template <typename Map_, bool IsConst_>
    friend bool operator==(const HashMapIterator<Map_, IsConst_>& lhs, const HashMapIterator<Map_, IsConst_>& rhs);

    /*
     * Inequality operator: decides if two iterators are pointing to different elements.
     *
     * Usage:
     *      if (iter != map.end()) {...};
     */
    template <typename Map_, bool IsConst_>
    friend bool operator!=(const HashMapIterator<Map_, IsConst_>& lhs, const HashMapIterator<Map_, IsConst_>& rhs);

    /*
     * Special member functions: we explicitly state that we want the default compiler-generated functions.
     * Here we are following the rule of zero. You should think about why that is correct.
     */
    HashMapIterator(const HashMapIterator<Map, IsConst>& rhs) = default;
    HashMapIterator<Map, IsConst>& operator=(const HashMapIterator<Map, IsConst>& rhs) = default;

    HashMapIterator(HashMapIterator<Map, IsConst>&& rhs) = default;
    HashMapIterator<Map, IsConst>& operator=(HashMapIterator<Map, IsConst>&& rhs) = default;
       // return node_ == rhs.node_ && bucket_array_ == rhs.bucket_array_ && bucket_index_ == rhs.bucket_index_;

    using node = typename Map::node;
    using bucket_array_type = typename Map::_buckets_array_type;
    HashMapIterator(node * node ,bucket_array_type * bucket_array, size_t  bucket_index);

private:

    /*
     * Determines what is the type of the _buckets_array that the HashMap is using.
     */
    //using bucket_array_type = typename Map::bucket_array_type;

    size_t bucket_index_ ;//这个node属于哪个bucket的索引
    node * node_;
    bucket_array_type * bucket_array_;
};


//构造函数
template <typename Map, bool IsConst >
HashMapIterator<Map,IsConst>::HashMapIterator(node * node ,bucket_array_type * bucket_array, size_t  bucket_index)
:node_(node),
 bucket_array_(bucket_array),
 bucket_index_(bucket_index)
{
}

template <typename Map, bool IsConst>
typename HashMapIterator<Map, IsConst>::reference HashMapIterator<Map, IsConst>::operator*() const{
 return node_->value;
}

template <typename Map, bool IsConst> 
typename HashMapIterator<Map, IsConst>::pointer HashMapIterator<Map, IsConst>::operator->() const {
    return &(node_->value);
}
//todo 
template <typename Map, bool IsConst>
HashMapIterator<Map, IsConst>&  HashMapIterator<Map, IsConst>::operator++() {
    /*if(node_){
        node_ = node_->next;
    } else {
        //找到下一个node
        size_t index = bucket_index_+1;
        //std::cout<<"endnd"<<std::endl;
        for(index; index < (*bucket_array_).size(); index++){
            auto curr  = (*bucket_array_)[index];
            if(curr != nullptr){
                node_ = curr;
                bucket_index_ = index;
                break;
            }
        }
        if(index ==(*bucket_array_).size() ){
            node_ = nullptr;
            bucket_index_  =(*bucket_array_).size();
        }
    }*/
    node_ = node_->next; // _node can't be nullptr - that would be incrementing end()
    if (node_ == nullptr) { // if you reach the end of the bucket, find the next bucket
        for (++bucket_index_; bucket_index_ < (*bucket_array_).size(); ++bucket_index_) {
            node_ = (*bucket_array_)[bucket_index_];
            if (node_ != nullptr) {
                return *this;
            }
        }
    }
    return *this;
}

//todo 
template <typename Map, bool IsConst>
HashMapIterator<Map, IsConst>  HashMapIterator<Map, IsConst>::operator++(int) {
    auto copy = *this;
    ++(*this);
    return copy;
}

template <typename Map_, bool IsConst_>
bool operator==(const HashMapIterator<Map_, IsConst_>& lhs, const HashMapIterator<Map_, IsConst_>& rhs){
    return lhs.node_ == rhs.node_;
}

template <typename Map_, bool IsConst_>
bool operator!=(const HashMapIterator<Map_, IsConst_>& lhs, const HashMapIterator<Map_, IsConst_>& rhs){
    return !(lhs == rhs);
}


// "White. A blank page or canvas. His favorite. So many possibilities...""









#endif // HASHMAPITERATOR_H
