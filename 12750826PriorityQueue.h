#ifndef _PR_QUEUE_H
#define _PR_QUEUE_H

/*
 * Some headers that may be useful.
 * The utility header is included for the
 * std::pair<X,Y> class, used below.
 */
#include <vector>
#include <list>
#include <utility>
#include <iostream>

/*
 * This class implements a priority queue ADT
 * with priorities specified in ints.
 * Lower priority values precede higher values in
 * the ordering.
 * The template type E is the element type.
 * See the tests for examples.
 */
template <typename E>
class PriorityQueue {

private:

  /*
   * @desc nodes of paired vector to store priority and elements.
   *       'nodes' because its represented into a tree structure.
   */
   std::vector<std::pair<int,E> > nodes;

public:

  /* Used for debugging purposes */
  void toString()
  {
    typename std::vector<std::pair<int,E> >::iterator it;

    std::cout << "START" << std::endl;

    for(it = nodes.begin(); it != nodes.end(); it++)
    {
      std::cout << "Priority: " << it->first << "E: " << it->second << std::endl;
    }

    std::cout << "END" << std::endl;
  }

  /*
   * @desc  Similar to heapifyDown -- where the function starts at index 0 and compares its subtrees.
   *        heapifyUp compares the currentNode to its parentNode and stops until condition is 0 (reaching the root).
   * @param Int will be used to specify last node in the vector; refer to line #128.
   */
  void heapifyUp(int index)
  {
    std::pair<int,E> parentNode, currentNode;

    while(index > 0)
    {
      parentNode = nodes[(index - 1) / 2]; // (Index - 1) / 2 is the formula to find parent.
      currentNode = nodes[index]; // Index is being specified as the last node.

      // If this condition runs, currentNode will swap with the parentNode, then index
      // will be set to where the parentNode was. Keep doing this until index reaches 0.
      // Once index is 0, break out of the loop.
      if(parentNode.first > currentNode.first)
      {
        // NOTE: swap doesn't understand parentNode or currentNode. We have to refer directly to vector 'nodes'.
        std::swap(nodes[index], nodes[(index - 1) / 2]);
        index = (index - 1) / 2;
      }
      else { break; }
    }
  }

  /*
   * Disclaimer: http://www.geeksforgeeks.org/binary-heap/ guide used to help build this function.
   * @desc  This function rearranges the tree to rectify violated properties. heapifyDown is recursed
   *        to ensure all the right and left subtrees are ordered.
   * @param Index is then used to compare whether the left or right children are less than index.
   */
  void heapifyDown(int index)
  {
    int rightPriority = nodes[2 * index + 2].first; // To find right child's position [2n + 2].
    int leftPriority = nodes[2 * index + 1].first; // To find left child's position is [2n + 1].
    int lowestPriority = index;

    // NOTE: The first condition ensures that it's not reaching a leaf node.
    //       The second condition checks if the left node is less than index argument.
    if(leftPriority < nodes.size() && leftPriority < index)
    {
      lowestPriority = leftPriority;
    }
    if(rightPriority < nodes.size() && rightPriority < index)
    {
      lowestPriority = rightPriority;
    }
    if(lowestPriority != index) // lowestPriority has changed to either leftPriority and/or rightPriority.
    {
      // Swap index with the new lowestPriority and recurse throughout the left and right subtrees.
      std::swap(nodes[index], nodes[lowestPriority]);
      heapifyDown(lowestPriority);
    }
  }
  /*
   * A constructor, if you need it.
   */
  PriorityQueue(){};

  /*
   * @desc This function adds a new element "element" to the queue
   *       with priority "priority".
   */
  void insert(int priority, E element)
  {
    //typename std::vector<std::pair<int,E> >::iterator it;
    std::pair<int,E> parentNode, currentNode;
    std::pair<int, E> newPair(priority, element); // Create new pair of nodes.
    int index;

    if(priority < 0) { return; } // Base case to ensure we don't accept when it's less than 0.
    else
    {
      // Otherwise append new node into the back of the vector.
      nodes.push_back(newPair);
      // Index is set to the last node in the vector because we are inserting
      // new nodes in the last available left of the subtree.
      index = nodes.size() - 1;
      // Call heapifyUp to fix min-heap property by swapping. Keep traversing up
      // and swapping until index hits 0 (reaching the root).
      heapifyUp(index);
    }
  }

  /*
   * @desc  Similar to insert, but takes a whole vector of new things to
   *        add.
   * @param Iterator searches through new_elements and calls insert(int, element)
   *        to add the paired first (int) and second (E) values.
   */
  void insert_all(std::vector<std::pair<int,E> > new_elements)
  {
    typename std::vector<std::pair<int,E> >::iterator it;

    for(it = new_elements.begin(); it != new_elements.end(); it++)
    {
      insert(it->first, it->second);
      //toString();
    }
  }

  /*
   * @desc Takes the lowest priority value element off the queue, and returns it.
   */
  E remove_front()
  {
    E rootElement;

    if(nodes.size() == 1) // If size of node is 1 -- means it's the root
    {
      rootElement = nodes[0].second;
      nodes.pop_back(); // NOTE: Calling rootElement doesn't work as it specifies an index.
      return rootElement;
    }
    else if(nodes.size() > 1) // Remove_front for several elements -- we need to heapifyDown
    {
      rootElement = nodes[0].second;
      nodes.erase(nodes.begin());
      nodes[0] = nodes[nodes.size() - 1];
      // nodes.pop_back();
      heapifyDown(0);
      return rootElement;
    }
    // std::swap(rootElement, nodes.size() - 1);
    return E(); // Returns back to the constructor if vector is empty.
  }

  /*
   * @desc Returns the lowest priority value element in the queue, but leaves
   *       it in the queue.
   */
  E peek()
  {
    // For peek at the 0 index
    if(nodes.size() == 1)
    {
      return nodes[0].second;
    }else if (nodes.size() > 1) // For peek plus several and after several
    {
      return nodes[0].second;
    }
    return E(); // Returns back to the constructor if vector is empty.
  }

  /*
   * @desc   <E> Elements stores the second object from <std::pair>nodes.
   *         Iterate through nodes and push back the second value to elements.
   * @return Elements - containing all the elements in the queue.
   */
  std::vector<E> get_all_elements()
  {
    std::vector<E> elements;
    typename std::vector<std::pair<int, E> >::iterator it;

    for(it = nodes.begin(); it != nodes.end(); it++)
        {
          elements.push_back(it->second);
        }
    return elements;
  }

  /*
   * @desc  Returns true if the queue contains element "element", false
   *        otherwise.
   * @param Elements is used to check if it matches second object in nodes.
   */
  bool contains(E element)
  {
    typename std::vector<std::pair<int,E> >::iterator it;

    for(it = nodes.begin(); it != nodes.end(); it++)
    {
      if(element == it->second)
      {
        return true;
      }
    }
    return false;
  }

  /*
   * @desc  Returns the priority of the element that matches "element". If there
   *        is more than one, return it returns the lowest priority value.
   * @param Element is used to check if it matches with priority of element in nodes.
   */
  int get_priority(E element)
  {
    typename std::vector<std::pair<int,E> >::iterator it;

    for(it = nodes.begin(); it != nodes.end(); it++)
    {
      if(element == it->second) // Does @param element match .second in the vector?
      {
        return it->first; // If so return the priority.
      }
    }
    return -1;
  }

  /*
   * @desc   <int> priorities stores the priority from pair<int,E> nodes. Iterate
   *               through the nodes and Push back the first value from nodes.
   * @return Priorities - containing all the priorities.
   */
  std::vector<int> get_all_priorities()
  {
    std::vector<int> priorities;
    typename std::vector<std::pair<int,E> >::iterator it;

    for(it = nodes.begin(); it != nodes.end(); it++)
    {
      priorities.push_back(it->first);
    }

    return priorities;
  }

  /*
   * @desc  Finds the first element that matches
   *        "element", and changes its priority to "new_priority".
   * @param Element is used to reference a current element in the vector and changes its
   *        priority to new_priority.
   */
  void change_priority(E element, int new_priority)
  {
    typename std::vector<std::pair<int,E> >::iterator it;

    for(it = nodes.begin(); it != nodes.end(); it++)
    {
      if(element == it->second) // When element matches in nodes,
      {
        it->first = new_priority; // call int, in E to change new_priority.
      }
    }
  }

  /*
   * @desc Return the size of elements in queue.
   */
  int size() { return nodes.size(); }

  /*
   * @desc Returns true if the queue has no elements, false otherwise.
   */
  bool empty()
  {
    if(nodes.empty())
    {
      return true;
    }
    return false;
  }
};

#endif
