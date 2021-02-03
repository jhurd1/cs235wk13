/***********************************************************************
* Header:
*    SET
* Summary:
*    This class contains the notion of a set: a bucket to hold
*    data for the user that can change sizes dynamically.
*    Of particular interest is the fact that the set only holds
*    unique values, discarding attempts to add duplicates.
*
*    This will contain the class definition of:
*       set                 : similar to std::set
* Author
*    Daniel Perez, Benjamin Dyas
************************************************************************/
#ifndef SET_H
#define SET_H
#include <cstddef>

namespace custom
{
   /************************************************
    * SET
    * Container class that holds unique values
    ***********************************************/
   template <class T>
   class set
   {
   public:
      // constructors & destructor
      set() :numCapacity{ 0 }, numElements{ 0 }, data{ nullptr } {}
      set(int numElements);
      set(const set & rhs);
      ~set() { if(data!= nullptr)delete [] data; }

      // overloaded operators
      set & operator =  (const set & rhs);
      set operator && (const set & rhs);
      set operator || (const set & rhs);
      set operator -  (const set & rhs);

      // standard container interfaces
      int   size()      const { return numElements; }
      int   capacity()  const { return numCapacity; } // is it needed at all?
      bool  empty()     const { return (numElements == 0); }
      void  clear()           { numElements = 0; }

      // container-specific interfaces
      T & operator [] (int index)
      {
         return data[index];
      }
      const T & operator [] (int index) const 
      {
         return data[index];
      }

      // the various iterator interfaces
      class iterator;
      iterator begin()      { return iterator (data); }
      iterator end();
      
      class const_iterator;
      const_iterator cbegin() const { return const_iterator(data); }
      const_iterator cend() const;

      // set-specific interfaces
      iterator    find(T  t); //should this be a reference?
      void        insert(T  t);
      iterator    erase(iterator it);

   private:
      //member variables
      T * data; // dynamically allocated array of T
      int numElements;                
      int numCapacity;

      //member functions
      int findIndex(T t);
      void resize(int newCapacity);
   }; // end class declaration

/************************************************** CONSTRUCTOR DEFINITION ***/

   /**********************************************
    * set : NON-DEFAULT CONSTRUCTOR
    * Preallocate the array to "capacity"
    **********************************************/
   template <class T>
   set<T>::set(int numElements)
   { 
      //in case a zero or negative number is passed
      if (numElements < 1)
      {
         this->numCapacity = 0; //this was commented out, why?
         this->numElements = 0;
         this->data = nullptr;

         return;
      }

      try
      {
         data = new T[numElements];
      }
      catch (...)
      {
         //to verify later on in case allocation fails
         data = nullptr;

         throw "ERROR: Unable to allocate a new buffer for set.";
      }

      //initially capacity and size are the same
      this->numCapacity = numElements;
      this->numElements = 0;
   }

   /*******************************************
    * set :: COPY CONSTRUCTOR
    *******************************************/
   template <class T>
   set<T> :: set(const set <T>& rhs)
   {
      //assert(rhs.numElements >= 0);

      // do nothing if there is nothing to do
      if (rhs.numElements == 0)
      {
         numElements = 0; //this was commented out, why?
         numCapacity = 0; //this was commented out, why?
         data = nullptr;
         return;
      }

      // attempt to allocate
      try
      {
         data = new T[rhs.numElements];
      }
      catch (...)
      {
         //to verify later on in case allocation fails
         data = nullptr;

         throw "ERROR: Unable to allocate a new buffer for set.";
      }

      // copy over the capacity
      this->numElements = rhs.numElements;
      this->numCapacity = rhs.numCapacity;

      // copy the items over one at a time using the assignment operator
      for (int i = 0; i < numElements; i++)
         data[i] = rhs.data[i];
   }

/***************************************** OVERLOADED OPERATORS DEFINITION ***/

   /********************************************
    * SET : ASSIGNMENT OPERATOR OVERLOAD
    ********************************************/
   template <class T>
   set <T>& set <T> :: operator = (const set <T>& rhs)
   {
      // if incoming object is bigger, resize allocated space
      if ( rhs.size() > this->capacity() )
      {
         this->resize(rhs.size());
      }

      // update array size to assigned size
      numElements = rhs.size();

      //Copy data from source to local array
      for (int i = 0; i < numElements; i++)
      {
         data[i] = rhs.data[i];
      }

      return *this;
   }

   /********************************************
    * SET : INTERSECTION OPERATOR OVERLOAD
    * RETURNS: A set by value with the values
    *          found on both sets.
    * PARAMETER: A set by reference
    ********************************************/
   template <class T>
   set <T> set <T> :: operator && (const set <T>& rhs)
   {
      int iLhs = 0;
      int iRhs = 0;
      set <T> setReturn;

      //While we don't make it to the end of either set
      while( iLhs < this->numElements || iRhs < rhs.size() )
      {  
         //This seems redundant, but if we make it to the end
         //   return the result so far.
         if ( iLhs == this->numElements )
         {
            return setReturn;
         }
         else if ( iRhs == rhs.size() )
         {
            return setReturn;
         }
         //If an item matches, insert it in the result and 
         //  move along on both sets
         else if ( this->data[iLhs] == rhs[iRhs] )
         {
            setReturn.insert( data[iLhs] );
            iLhs++;
            iRhs++;
         }
         // if the left is lesser/smaller, advance it
         else if (this->data[iLhs] < rhs[iRhs])
         {
            iLhs++;
         }
         else
         {
            iRhs++;
         }
      }

      return setReturn;
   }

   /********************************************
    * SET : UNION OPERATOR OVERLOAD
    * RETURNS: A set by value with all the
    *          items of both sets.
    * PARAMETER: A set by reference
    ********************************************/
   template <class T>
   set <T> set <T> :: operator || (const set <T>& rhs)
   {
   
      int iLhs = 0;
      int iRhs = 0;
      set <T> setReturn;

      //This seems redundant, but if we make it to the end
      //   return the result so far.
      while( iLhs < this->numElements || iRhs < rhs.size() )
      {
         //If the lhs set makes it to the end, add the rest of
         //   the rhs set
         if ( iLhs == this->numElements )
         {
            setReturn.insert( rhs[iRhs++] );
         }
         //If the rhs set makes it to the end, add the rest of
         //   the lhs set
         else if ( iRhs == rhs.size() )
         {
            setReturn.insert( data[iLhs++] );
         }
         //If they are the same, just add one, it doesn't matter
         else if ( this->data[iLhs] == rhs[iRhs] )
         {
            setReturn.insert( data[iLhs] );
            iLhs++;
            iRhs++;
         }
         //if the left is lesser, add it and move along
         else if (this->data[iLhs] < rhs[iRhs])
         {
            setReturn.insert( data[iLhs++] );
         }
         else
         {
            setReturn.insert( rhs[iRhs++] );
         }
      }
      return setReturn;

   }

   /********************************************
    * SET : SUBTRACTION OPERATOR OVERLOAD
    * RETURNS: the members that are in one set 
    *          but not in another. non-commutative
    * PARAMETER: A set by reference.
    ********************************************/
   template <class T>
   set <T> set <T> :: operator - (const set <T>& rhs)
   {     
      int iLhs = 0;
      int iRhs = 0;
      set <T> setReturn;

      //we only really have to check the left hand side
      //   everything else can be discarded as it not found on the left
      while( iLhs < this->numElements)
      {
         if ( iRhs == rhs.size() )
         {
            //if the rhs makes it to the end, just add everything of the lhs
            setReturn.insert( data[iLhs++] );
         }
         else if ( this->data[iLhs] == rhs[iRhs] )
         {
            //setReturn.insert( data[iLhs] ); //if they match we don't add them, hence the subraction
            iLhs++;
            iRhs++;
         }
         else if (this->data[iLhs] < rhs[iRhs])
         {  
            //if the data on the left hand side is less than rhs,
            //   keep adding it until we catch up to the right's size
            //   same as if the right makes it to the end.
            setReturn.insert( data[iLhs++] );
         }
         else
         {

            //if the left is higher than the right but they don't match
            //   increment the right until it catches up
            iRhs++;

         }
      }
      return setReturn;
   }

/************************************** SET SPECIFIC INTERFACES DEFINITION ***/

   /********************************************
    * SET : FIND
    * Return an interator if an element matches
    * the search, otherwise return an interator
    * to the end()
    *******************************************/
   template <class T>
   typename set <T>::iterator set<T>::find(T t)
   {
      //the name of the array is a pointer to the first element
      iterator it;

      it = ( data + findIndex(t) );
      
      return it;


   }

   /********************************************
    * SET : INSERT
    * Insert an element into the set. If already
    * existent, do nothing.
    *******************************************/
   template <class T>
   void set<T>::insert(T element)
   {
      
      //if needed resize
      if(this->capacity() == this->size()) //size() + 1 == numElements + 1
      {
         resize( numCapacity * 2 );
      }
      
      //if it's already there, don't do anything
      if( findIndex(element) != numElements )
      {
         return;
      }

      int iInsert = findIndex(element);
      int i;

      //as long as it is not already there
      if (data[iInsert] != element)
      {
         for(i = numElements; i > 0; i--)
         {
            if (data[i - 1] < element)
            {
               //This kicks it out at the point the element should be inserted
               //data[i + 1] = data[i];
               break;
            }

            data[i] = data[i-1];              

         }
         
         data[i] = element;
         numElements++;

      }

      // if(data[iInsert] == element && numElements == 0)
      // {
      //    numElements++;
      // }
      
      
      

      

   }

   /********************************************
    * SET : ERASE
    * Find and erase an element from the set.
    *******************************************/
   template <class T>
   typename set <T>::iterator set<T>::erase(iterator it)
   {
      T element = *it;

      int iErase = findIndex(element);

      //if the data at index found matches the data to erase
      if (data[iErase] == element)
      {
         //move everything down and erase it by overwriting it
         for(int i = iErase; i < numElements - 1; i++)
         {
            data[i] = data[i + 1];
         }
         
         //decrease the size
         numElements--;
      }

      return it;
   }
      
/******************************************** PRIVATE FUNCTIONS DEFINITION ***/

   /********************************************
    * set : FIND INDEX
    * Find index of existing element or where it
    * should be inserted.
    *******************************************/
   template <class T>
   int set<T>::findIndex(T searchedElement)
   {
      int indexBegin = 0;
      int indexEnd = numElements - 1;

      //modified binary search
      while (indexBegin <= indexEnd)
         {
            //declaring inside the while loop makes it reset every time
            int indexMiddle = (indexBegin + indexEnd) / 2;

            //if found, return it
            if (searchedElement == data[indexMiddle])
            {   
               //it always finds it in the middle
               return indexMiddle;  
            }
            
            //if searchedElement is lesser than, search first half
            if (searchedElement < data[indexMiddle])
            {
               indexEnd = indexMiddle - 1;
            }
            //else, search second half
            else
            {
               indexBegin = indexMiddle + 1;
            }

         }
      
         return numElements; 
   }

   /********************************************
    * set : RESIZE
    * Dynamically resizes the vector
    *******************************************/
   template <class T>
   void set<T> ::resize(int newCapacity)
   {
      // create a temporary pointer for array swap
      T* tempData = nullptr;

      // The special case of newCapacity being 0 is handled
      // This happens when vector is default constructed with a 
      //    size and capacity of zero. The first time it doesn't double:
      //    it goes from zero to one
      if (newCapacity == 0)
      {
         newCapacity = 1;
      }

      //attempt to allocate
      try
      {
         tempData = new T[newCapacity];
      }
      catch (...)
      {
         throw "ERROR: Unable to allocate a new buffer for set.";
      }
      for (int i = 0; i < numElements; i++)
         tempData[i] = data[i];

      delete[] data;
      
      //By copying the pointer, the nameless array in the heap is transferred 
      data = tempData;


      this->numCapacity = newCapacity;

   }

/***************************************************** ITERATOR DEFINITION ***/

   /**************************************************
    * set ITERATOR
    * An iterator through the internal array of 
    * the set
    *************************************************/
   template <class T>
   class set <T> :: iterator
   {
   public:
      // constructors, destructors, and assignment operator
      iterator() : p(NULL) {}
      iterator(T * p) : p(p) {}
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
            T & operator * () { return *p; }
      const T & operator * () const { return *p; }

      // prefix increment
      iterator & operator ++ ()
      {
         p++;
         return *this;
      }

      // postfix increment
      iterator operator ++ (int postfix)
      {
         iterator tmp(*this);
         p++;
         return tmp;
      }

      // prefix decrement
      iterator& operator -- ()
      {
         p--;
         return *this;
      }

      // postfix decrement
      iterator operator -- (int postfix)
      {
         iterator tmp(*this);
         p--;
         return tmp;
      }

      private:
      T * p;
   };

   /********************************************
    * set :: ITERATOR :: END
    * Note that you have to use "typename" before
    * the return value type
    ********************************************/
   template <class T>
   typename set <T> ::iterator set <T> ::end()
   {
      return iterator(data + numElements);
   }
   
   /**************************************************
    * set CONST_ITERATOR
    * A constant iterator through the internal array of
    * the set
    *************************************************/
   template <class T>
   class set <T> ::const_iterator
   {
   public:
      // constructors, destructors, and assignment operator
      const_iterator() : p(NULL) {              }
      const_iterator(T* p) : p(p) {              }
      const_iterator(const const_iterator& rhs) { *this = rhs; }
      const_iterator& operator = (const const_iterator& rhs)
      {
         this->p = rhs.p;
         return *this;
      }

      // equals, not equals operator
      bool operator != (const const_iterator& rhs) const { return rhs.p != this->p; }
      bool operator == (const const_iterator& rhs) const { return rhs.p == this->p; }

      // dereference operator
      T& operator * () { return *p; }
      const T& operator * () const { return *p; }

      // prefix increment
      const_iterator& operator ++ ()
      {
         p++;
         return *this;
      }

      // postfix increment
      const_iterator operator ++ (int postfix)
      {
         const_iterator tmp(*this);
         p++;
         return tmp;
      }

      // prefix decrement
      const_iterator& operator -- ()
      {
         p--;
         return *this;
      }

      // postfix decrement
      const_iterator operator -- (int postfix)
      {
         const_iterator tmp(*this);
         p--;
         return tmp;
      }

   private:
      T* p;
   };

   /********************************************
    * set :: ITERATOR :: CEND
    * Note that you have to use "typename" before
    * the return value type
    ********************************************/
   template <class T>
   typename set <T> ::const_iterator set <T> ::cend() const //had to make a promise to compiler
   {
      return const_iterator(data + numElements);
   }
}; //end namespace custom    

#endif // set_H
