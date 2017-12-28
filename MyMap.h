//
//  MyMap.h
//  Proj. 4
//
//  Created by Faith Yu on 8/3/2017.
//  Copyright © 2017 Faith Yu. All rights reserved.
//

#ifndef MyMap_h
#define MyMap_h
#include<stdlib.h>
// MyMap.h

// Skeleton for the MyMap class template.  You must implement the first six
// member functions.


template<typename KeyType, typename ValueType>
class MyMap
{
public:
    MyMap()                     // constructor
    : m_size(0), m_root(nullptr)
    {}
    
    ~MyMap()                    // destructor; deletes all of the tree's nodes
    {
        clear();
    }
    
    void clear()                // deletes all of the trees nodes producing an empty tree
    {
        deleteTree(m_root);
    }
    
    int size() const            // return the number of associations in the map
    {
        if(m_root == nullptr)
            return 0;
        else
            return m_size;
    }
    
    void associate(const KeyType& key, const ValueType& value)
    {
        // if the tree is empty, we create a new Node
        if(m_root == nullptr)
        {
            m_root = new Node(key, value);                          // size increment is down below
        }
        else
        {
            Node* curr = m_root;
            for(;;)
            {
                if(curr->m_key == key)
                {
                    curr->m_value = value;
                    return;                                         // we do not increment the size
                }
                else if (curr->m_key < key)                         // if curr value is smaller than key
                {
                    if(curr->m_right == nullptr)
                    {
                        curr->m_right = new Node(key, value);
                        break;
                    }
                    else
                        curr = curr->m_right;
                }
                else if (curr->m_key > key)                         // if curr value is greater than key
                {
                    if(curr->m_left == nullptr)
                    {
                        curr->m_left = new Node(key, value);
                        break;
                    }
                    else
                        curr = curr->m_left;
                }
            }
        }
        
        m_size++;
    }
    
    const ValueType* find(const KeyType& key) const
    {
        Node* curr = m_root;
        while(curr != nullptr)
        {
            if(curr->m_key == key)
                return &(curr->m_value);
            else if(curr->m_key > key)
                curr = curr->m_left;
            else if(curr->m_key < key)
                curr = curr->m_right;
        }
        // if there is no matching key value
        return nullptr;
    }
    
    // for a modifiable map, return a pointer to modifiable ValueType
    ValueType* find(const KeyType& key)
    {
        return const_cast<ValueType*>(const_cast<const MyMap*>(this)->find(key));
    }
    
    // C++11 syntax for preventing copying and assignment
    MyMap(const MyMap&) = delete;
    MyMap& operator=(const MyMap&) = delete;
    
private:
    // private class Node
    struct Node
    {
        Node(KeyType key, ValueType value)
        :m_key(key),m_value(value)
        {
            m_left = m_right = nullptr;
        }
        
        ~Node()
        {
            delete m_left;
            delete m_right;
        }
        
        KeyType m_key;
        ValueType m_value;
        Node* m_left;
        Node* m_right;
    };
    
    Node* m_root;
    size_t m_size;
    
    void deleteTree(struct Node* node)
    {
        if (node == NULL) return;
        
        /* first delete both subtrees */
        deleteTree(node->m_left);
        deleteTree(node->m_right);
        
        /* then delete the node */
        free(node);
    }
};

#endif /* MyMap_h */
