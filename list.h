/***********************************************************************
 * Header:
 *    This is the header file for a "list" abstract data type
 * Summary:
 *    This class contains a custom implementation of the std::list ADT.
 *    it will declare and define a class that holds a linked list of
 *    nodes and all corresponding functions to manipulate it.
 *    Node and iterator nested classes will also be defined.
 *
 * Author
 *    Daniel Perez, Jamie Hurd, Benjamin Dyas
 ************************************************************************/
#ifndef LIST_H
#define LIST_H

namespace custom
{
   template <class T> class list;
   /************************************************
    * CLASS NODE
    * Container class element that contains info
    * on previous and next elements in a list 
    ***********************************************/
   template<class T>
   class Node{

      friend class list<T>;

      public:
      //constructors
      Node() : data{NULL}, pNext{nullptr}, pPrev{nullptr} {}
      Node(const T & element) : data{element}, pNext{nullptr}, pPrev{nullptr} {} 

      T data; // container for the data
      Node * pNext; //pointer to next node
      Node * pPrev; //pointer to previous node
   };
   /************************************************
    * CLASS LIST
    * 
    * 
    ***********************************************/
   template<class T>
   class list {
   private:
      //nested class

      //members
      Node<T> * pHead;
      Node<T> * pTail;
      int numElements;

   public:
      //constructors and destructor
      list() :pHead{ nullptr }, pTail{ nullptr }, numElements { 0 } {}
      list(const list & rhs);
      ~list() {if(pHead != nullptr){clear();}}
      
      //overloaded operators
      list & operator =  (const list & rhs);

      
      // the various iterator interfaces
      class iterator;
      iterator begin() { return iterator(pHead); }
      iterator end()  { return iterator(nullptr); }

      class const_iterator;
      const_iterator cbegin() const { return const_iterator(pHead); }
      const_iterator cend() const  { return const_iterator(nullptr); }

      class reverse_iterator;
      reverse_iterator rbegin() { return reverse_iterator (pTail); }
      reverse_iterator rend() { return reverse_iterator(nullptr); }

      class const_reverse_iterator;
      const_reverse_iterator crbegin() const { return const_reverse_iterator (pTail); }
      const_reverse_iterator crend() const  { return const_reverse_iterator(nullptr); }

      // standard container interfaces
      int       size() const { return numElements; }
      bool      empty()const { return (numElements == 0); }
      void      clear();
      void      push_back(const T& element);
      void      push_front(const T& element);
      void      pop_back();
      void      pop_front();
      T &       back();
      const T   back() const;
      T &       front();
      const T   front() const;

      iterator  find(const T & element);
      iterator  erase(iterator it);
      void      insert(iterator it, const T& t);

   };

   /********************************************
    * FUNCTION:  LIST :: COPY CONSTRUCTOR
    * RETURNS:   ...
    * PARAMETER: ...
    ********************************************/
   template <class T>
   list<T> :: list(const list<T> & rhs)
   {
      this->pHead = nullptr;
      this->pTail = nullptr;
      for (Node<T> * p = rhs.pHead; p; p = p->pNext)
      {
         this->push_back(p->data);
      }

      numElements = rhs.numElements;

   }

/***************************************** OVERLOADED OPERATORS DEFINITION ***/

   /********************************************
    * FUNCTION:  Assignment Operator Overload
    * RETURNS:   ...
    * PARAMETER: ...
    ********************************************/
   template <class T>
   list<T> & list <T> :: operator = (const list <T> & rhs)
   {
      //clear the list before copying new information over
      this->clear();

      //reset the pointers
      this->pHead = nullptr;
      this->pTail = nullptr;

      //standard copy loop for nodes
      for (Node<T> * p = rhs.pHead; p; p = p->pNext)
      {
         this->push_back(p->data);
      }

      return *this;
   }
   /***************************************************** ITERATOR DEFINITION ***/

   /**************************************************
    * LIST ITERATOR
    * An iterator through the internal array of 
    * the list
    *************************************************/
   template <class T>
   class list <T> :: iterator
   {
   public:
      //friend
      friend class list<T>;
   
      // constructors, destructors, and assignment operator
      iterator() : p(NULL) {}
      iterator(Node<T> * p) : p(p) {}
      iterator(const iterator & rhs) { *this = rhs; }

      iterator & operator = (const iterator & rhs)
      {
         this->p = rhs.p;
         return *this;
      }

      // equals, not equals operator
      bool operator != (const iterator & rhs) const { return rhs.p != this->p; }
      bool operator == (const iterator & rhs) const { return rhs.p == this->p; }

      // dereference operator
            T & operator * () { return p->data; }
      const T & operator * () const { return  p->data; }

      // prefix increment a
      iterator & operator ++ ()
      {
         p = p->pNext;
         return *this;
      }

      // postfix increment
      iterator operator ++ (int postfix)
      {
         iterator tmp(*this);
         p = p->pNext;
         return tmp;
      }

      // prefix decrement
      iterator& operator -- ()
      {
         p = p->pPrev;
         return *this;
      }

      // postfix decrement
      iterator operator -- (int postfix)
      {
         iterator tmp(*this);
         p = p->pPrev;
         return tmp;
      }

      private:
      Node<T> * p;
   };

   /**************************************************END ITERATOR DEFINITION ***/
   /********************************************* REVERSE_ITERATOR DEFINITION ***/

   /**************************************************
    * LIST REVERSE ITERATOR
    * An iterator through the internal array of 
    * the list in reverse
    *************************************************/
   template <class T>
   class list <T> :: reverse_iterator
   {
   public:
      //friend
      friend class list<T>;
   
      // constructors, destructors, and assignment operator
      reverse_iterator() : p(NULL) {}
      reverse_iterator(Node<T> * p) : p(p) {}
      reverse_iterator(const reverse_iterator & rhs) { *this = rhs; }

      reverse_iterator & operator = (const reverse_iterator & rhs)
      {
         this->p = rhs.p;
         return *this;
      }

      // equals, not equals operator
      bool operator != (const reverse_iterator & rhs) const { return rhs.p != this->p; }
      bool operator == (const reverse_iterator & rhs) const { return rhs.p == this->p; }

      // dereference operator
            T & operator * () { return p->data; }
      const T & operator * () const { return p->data; }

      // prefix increment
      reverse_iterator & operator ++ ()
      {
         p = p->pPrev;
         return *this;
      }

      // postfix increment
      reverse_iterator operator ++ (int postfix)
      {
         reverse_iterator tmp(*this);
         p = p->pPrev;
         return tmp;
      }

      // prefix decrement
      reverse_iterator& operator -- ()
      {
         p = p->pNext;
         return *this;
      }

      // postfix decrement
      reverse_iterator operator -- (int postfix)
      {
         reverse_iterator tmp(*this);
         p = p->pNext;
         return tmp;
      }

      private:
      Node<T> * p;
   };

   /******************************************END REVERSE_ITERATOR DEFINITION ***/
   
   /********************************************
    * FUNCTION:     CLEAR
    * DESCRIPTION:  clears the list
    * PARAMETER:    N/A
    ********************************************/
   template <class T>
   void list<T>::clear()
   {  
      //while there are nodes still
      while(pHead != nullptr)
      {
         //delete the node and update the head
         Node<T> * pDelete = pHead;
         pHead = pHead->pNext;
         delete pDelete;

         //decrease size as it deletes
         --numElements;
      }

      //reset the pointers
      pHead = nullptr;
      pTail = nullptr;

   }

   /********************************************
   * FUNCTION:  PUSH_BACK
   * DESCRIPTION: adds an element to the back
   * PARAMETER:   an element
   ********************************************/
   template <class T>
   void list<T> ::push_back(const T& element)
   {  
      //new node to be included
      Node<T> * pNew = nullptr;

      //try to allocate new node
      try
      {
         pNew = new Node<T>(element);
         
      }
      catch (std::bad_alloc)
      {
         std::cout <<  "ERROR: unable to allocate a new node for a list" << std::endl;
      }

      //if it's the first item to be pushed back
      if(pTail == nullptr && pHead == nullptr)
      {
         pHead = pNew;
         pTail = pNew;
      }
      else if(pTail != nullptr)
      {
         //we first modify the inserted node, before modifying the list
         pNew->pNext = pTail->pNext;
         pNew->pPrev = pTail;

         //make pNew the previous link now
         pTail->pNext = pNew;

         //update pTail
         pTail = pNew;
      }

      numElements++;
   }

   /********************************************
   * FUNCTION:  PUSH_FRONT
   * DESCRIPTION: adds an element to the front
   * PARAMETER:   an element
   ********************************************/
   template <class T>
   void list<T> ::push_front(const T& element)
   {  
      iterator it(pHead);
      insert(it, element);
   }

   /********************************************
   * FUNCTION:  POP_BACK
   * DESCRIPTION: removes the back element
   * PARAMETER:   N/A
   ********************************************/
   template <class T>
   void list<T> ::pop_back()
   {  
      if (!empty())
      {
         erase(this->back());
      }

   }
   
   /********************************************
   * FUNCTION:    POP_FRONT
   * DESCRIPTION: removes the front element
   * PARAMETER:   N/A
   ********************************************/
   template <class T>
   void list<T> ::pop_front()
   {  
      if (!empty())
      {
         erase(this->front());
      }

   }

   /********************************************
   * FUNCTION:  BACK
   * RETURNS:   the back of the list
   * PARAMETER: N/A
   ********************************************/
   template<class T>
   T& list<T>::back()
   {
      if (empty())
      {
         throw "ERROR: unable to access data from an empty deque";
      }
      else
      {
         return pTail->data;
      }

   }

   /********************************************
   * FUNCTION:  BACK (CONST)
   * RETURNS:    a const back of the list
   * PARAMETER: ...
   ********************************************/
   template<class T>
   const T list<T>::back() const
   {
      if (empty())
      {
         throw "ERROR: unable to access data from an empty deque";
      }
      else
      {
         return pTail->data;
      }

   }

   /********************************************
   * FUNCTION:  FRONT
   * RETURNS:   the front of the list
   * PARAMETER: ...
   ********************************************/
   template<class T>
   T& list<T>::front()
   {
      if (empty())
      {
         throw "ERROR: unable to access data from an empty deque";
      }
      else
      {
         return pHead->data;
      }  
        
   }

   /********************************************
   * FUNCTION:  FRONT (CONST)
   * RETURNS:   a const front of the list
   * PARAMETER: ...
   ********************************************/
   template<class T>
   const T list<T>::front() const
   {
      if (empty())
      {
         throw "ERROR: unable to access data from an empty deque";
      }
      else
      {
         return pHead->data;
      }

   }

   /********************************************
    * FUNCTION:    FIND
    * DESCRIPTION: searches for a value
    * PARAMETER:   receives an element to find
    ********************************************/
   template <class T>
   typename list<T>::iterator list<T>::find(const T & element)
   {
      //Loop to traverse the linked list
      for(Node<T> * p = pHead; p; p = p->pNext)
      {
         //if the items match, return it
         if(p->data == element)
         {
            return iterator(p);
         }
      }

      //otherwise return 0x0
      return iterator(nullptr);
   }
   
   /********************************************
    * FUNCTION:     ERASE
    * DESCRIPTION:  removes the requested item
    * PARAMETER:    an iterator
    ********************************************/
   template <class T>
   typename list<T>::iterator list<T>::erase(list<T>::iterator it)
   {
   
     Node<T> * pRemove;
     pRemove = it.p;

     //if a nullptr is passed
      if (nullptr == pRemove)
      {
         //do nothing
         return nullptr;
      }

      if (pRemove == pHead)
      {
         pHead = pRemove->pNext;
      }

      //if it's not the first element (pPrev != null)
      if (pRemove->pPrev)
      {
         //update the pointer on preceding node
         pRemove->pPrev->pNext = pRemove->pNext;
         if(pRemove->pPrev->pNext == nullptr)
         {
            pTail = pRemove->pPrev;
         }
      }
      //if it's not the last element (pNext != null)
      if (pRemove->pNext)
      {
         //update the pointer on trailing node
         pRemove->pNext->pPrev = pRemove->pPrev;
      }

      //This pointer is created so that we can erase one of the two
      //   without losing the connection to the rest of the list
      Node<T> * pReturn;
      
      //If there is a preceding node
      if(pRemove->pPrev)
      {
         //make pReturn point at the preceding element
         pReturn = pRemove->pPrev;
      }
      else
      {
         //make pReturn point at the trailing element
         pReturn = pRemove->pNext;
      }

      //the actual deletion of the now orphaned node
      delete pRemove;

      numElements--;

      return iterator(pReturn);
   }

   /********************************************
    * FUNCTION:     INSERT
    * DESCRIPTION:  inserts a value into the list 
    * PARAMETER:    an iterator and data
    ********************************************/
   template <class T>
   void list<T>::insert(iterator it, const T& t)
   {
      Node<T> * pNew = new Node<T>(t);
      iterator nullIt(nullptr);

      //If it's null
      if (it == nullIt && !(pHead == nullptr && pTail == nullptr))
      {
         pNew->pPrev = pTail;
         pNew->pPrev->pNext = pNew;
         pTail = pNew;
      }
      if (it != nullIt)
      {
         pNew->pNext = it.p;
         pNew->pPrev = it.p->pPrev;
         it.p->pPrev = pNew;
         if (pNew->pPrev)
         {
            pNew->pPrev->pNext = pNew;
         }
      }
      
      if(pNew->pPrev == nullptr)
      {
         pHead = pNew;
      }
      if(pNew->pNext == nullptr)
      {
         pTail = pNew;
      }
      
      numElements++;

   }
   


/*********************************************CONST_REVERSE_ITERATOR DEFINITION ***/

   /**************************************************
    * LIST: CONST_REVERSE_ITERATOR
    * An iterator through the internal array of 
    * the list
    *************************************************/
   template <class T>
   class list <T> :: const_reverse_iterator
   {
   public:
      //friend
      friend class list<T>;
   
      // constructors, destructors, and assignment operator
      const_reverse_iterator() : p(NULL) {}
      const_reverse_iterator(Node<T> * p) : p(p) {}
      const_reverse_iterator(const const_reverse_iterator & rhs) { *this = rhs; }

      const_reverse_iterator & operator = (const const_reverse_iterator & rhs)
      {
         this->p = rhs.p;
         return *this;
      }

      // equals, not equals operator
      bool operator != (const const_reverse_iterator & rhs) const { return rhs.p != this->p; }
      bool operator == (const const_reverse_iterator & rhs) const { return rhs.p == this->p; }

      // dereference operator
            T & operator * () { return p->data; }
      const T & operator * () const { return p->data; }

      // prefix increment
      const_reverse_iterator & operator ++ ()
      {
         p = p->pPrev;
         return *this;
      }

      // postfix increment
      const_reverse_iterator operator ++ (int postfix)
      {
         const_reverse_iterator tmp(*this);
         p = p->pPrev;
         return tmp;
      }

      // prefix decrement
      const_reverse_iterator& operator -- ()
      {
         p = p->pNext;
         return *this;
      }

      // postfix decrement
      const_reverse_iterator operator -- (int postfix)
      {
         const_reverse_iterator tmp(*this);
         p = p->pNext;
         return tmp;
      }

      private:
      Node<T> * p;
   };

   /******************************************END CONST_REVERSE_ITERATOR DEFINITION ***/
   /***************************************************** CONST_ITERATOR DEFINITION ***/

   /**************************************************
    * LIST: CONST_ITERATOR
    * An iterator through the internal array of 
    * the list
    *************************************************/
   template <class T>
   class list <T> :: const_iterator
   {
   public:
      //friend
      friend class list<T>;
   
      // constructors, destructors, and assignment operator
      const_iterator() : p(NULL) {}
      const_iterator(Node<T> * p) : p(p) {}
      const_iterator(const const_iterator & rhs) { *this = rhs; }

      const_iterator & operator = (const const_iterator & rhs)
      {
         this->p = rhs.p;
         return *this;
      }

      // equals, not equals operator
      bool operator != (const const_iterator & rhs) const { return rhs.p != this->p; }
      bool operator == (const const_iterator & rhs) const { return rhs.p == this->p; }

      // dereference operator
            T & operator * () { return p->data; }
      const T & operator * () const { return  p->data; }

      // prefix increment a
      const_iterator & operator ++ ()
      {
         p = p->pNext;
         return *this;
      }

      // postfix increment
      const_iterator operator ++ (int postfix)
      {
         const_iterator tmp(*this);
         p = p->pNext;
         return tmp;
      }

      // prefix decrement
      const_iterator& operator -- ()
      {
         p = p->pPrev;
         return *this;
      }

      // postfix decrement
      const_iterator operator -- (int postfix)
      {
         const_iterator tmp(*this);
         p = p->pPrev;
         return tmp;
      }

      private:
      Node<T> * p;
   };

   /**************************************************END ITERATOR DEFINITION ***/
}; //end namespace custom
#endif