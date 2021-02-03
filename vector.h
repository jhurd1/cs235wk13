/***********************************************************************
* Header:
*    Vector
* Summary:
*    This class contains the notion of a vector: a bucket to hold
*    data for the user that can change sizes dynamically
*
*    This will contain the class definition of:
*       vector                  : similar to std::vector
*       vector :: iterator      : an iterator through the vector
*       vector :: const_iterator: a constant iterator through the vector
* Author
*    Daniel Perez
************************************************************************/

#ifndef VECTOR_H
#define VECTOR_H

#include <cassert>  // because I am paranoid

// a little helper macro to write debug code
#ifdef NDEBUG
#define Debug(statement)
#else
#define Debug(statement) statement
#endif // !NDEBUG

namespace custom
{

    /************************************************
    * VECTOR
    * A class that holds stuff and that changes
    * sizes dynamically
    ***********************************************/
    template <class T>
    class vector
    {
    public:
        // constructors and destructors
        vector() :numCapacity{ 0 }, numElements{ 0 }, data{ nullptr } {}
        vector(int numElements);
        vector(int numElements, const T& t);
        vector(const vector & rhs);
        ~vector() { if(data!= nullptr)delete [] data; }

        //overloaded operators
        vector & operator = (const vector & rhs);
   
        // standard container interfaces
        // array treats size and max_size the same
        int   size()      const { return numElements; }
        int   capacity()  const { return numCapacity; }
        bool  empty()     const { return (numElements == 0); }
        void  clear();
   
        // container-specific interfaces
        // what would happen if I passed -1 or something greater than num?
        T & operator [] (int index)
        {
            return data[index];
        }
        const T & operator [] (int index) const 
        {
            return data[index];
        }

        void push_back(const T & element);
   
        // the various iterator interfaces
        class iterator;
        iterator begin()      { return iterator (data); }
        iterator end();

        class const_iterator;
        const_iterator cbegin() const { return const_iterator(data); }
        const_iterator cend() const;

        // a debug utility to display the array
        // this gets compiled to nothing if NDEBUG is defined
        void display() const; 
   
    private:
        T * data;                       // dynamically allocated array of T
        int numElements;                
        int numCapacity;

        void resize(int newCapacity);
    };

    /**************************************************
    * VECTOR ITERATOR
    * An iterator through the internal array of 
    * the vector
    *************************************************/
    template <class T>
    class vector <T> ::iterator
    {
    public:
        // constructors, destructors, and assignment operator
        iterator() : p(NULL) {              }
        iterator(T * p) : p(p) {              }
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
    * VECTOR :: ITERATOR :: END
    * Note that you have to use "typename" before
    * the return value type
    ********************************************/
    template <class T>
    typename vector <T> ::iterator vector <T> ::end()
    {
        return iterator(data + numElements);
    }

    /**********************************************
    * VECTOR : NON-DEFAULT CONSTRUCTOR
    *        : ONE PARAMETER
    * Preallocate the array to "capacity"
    **********************************************/
    template <class T>
    vector<T>::vector(int numElements)
    { 
        //in case a zero or negative number is passed
        if (numElements < 1)
        {
            //this->numCapacity = 0;
            this->numElements = 0;
            this->data = nullptr;
            
            /*if (numElements < 0)
                cout << "Size can't be negative: zero space allocated" << endl;*/
            return;
        }

        try
        {
            data = new T[numElements];
        }
        catch (std::bad_alloc)
        {
            //to verify later on in case allocation fails
            data = nullptr;

            throw "ERROR: Unable to allocate buffer";
        }

        //initially capacity and size are the same
        this->numCapacity = numElements;
        this->numElements = numElements;
    }

    /**********************************************
    * VECTOR : NON-DEFAULT CONSTRUCTOR
    *        : TWO PARAMETER
    * Preallocate the array to "capacity"
    * Assign second argument to all members of array
    **********************************************/
    template <class T>
    vector<T>::vector(int numElements,const T &t)
    {
        //in case a zero or negative number is passed
        if (numElements < 1)
        {
            //this->numCapacity = 0;
            this->numElements = 0;
            this->data = nullptr;

            /*if (numElements < 0)
                cout << "Size can't be negative: zero space allocated" << endl;*/
            return;
        }

        // attempt to allocate
        try
        {
            data = new T[numElements];
        }
        catch (std::bad_alloc)
        {
            //to verify later on in case allocation fails
            data = nullptr;

            throw "ERROR: Unable to allocate buffer";
        }

        //initially capacity and size are the same
        this->numCapacity = numElements;
        this->numElements = numElements;

        //Loop through the array assigning value t
        // t is whatever object or variable is passed
        for (int i = 0; i < numElements; ++i)
        {
            data[i] = t;
        }
    }

    /*******************************************
     * VECTOR :: COPY CONSTRUCTOR
     *******************************************/
    template <class T>
    vector <T> ::vector(const vector <T>& rhs)
    {
        //assert(rhs.numElements >= 0);

        // do nothing if there is nothing to do
        if (rhs.numElements == 0)
        {
            //numElements = 0;
            //numCapacity = 0;
            data = nullptr;
            return;
        }

        // attempt to allocate
        try
        {
            data = new T[rhs.numElements];
        }
        catch (std::bad_alloc)
        {
            //to verify later on in case allocation fails
            data = nullptr;

            throw "ERROR: Unable to allocate buffer";
        }

        // copy over the capacity
        this->numElements = rhs.numElements;
        this->numCapacity = rhs.numCapacity;

        // copy the items over one at a time using the assignment operator
        for (int i = 0; i < numElements; i++)
            data[i] = rhs.data[i];
    }

    /********************************************
     * VECTOR : RESIZE
     * Dynamically resizes the vector
     *******************************************/
    template <class T>
    void vector<T> ::resize(int newCapacity)
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
        catch (std::bad_alloc)
        {
            throw "ERROR: Unable to allocate buffer while resizing vector";
        }
        for (int i = 0; i < numElements; i++)
            tempData[i] = data[i];

        delete[] data;
        
        //By copying the pointer, the nameless array in the heap is transferred 
        data = tempData;


        this->numCapacity = newCapacity;

    }

    /********************************************
     * VECTOR : CLEAR
     * sets vector size to zero: doesn't deallocate 
     *******************************************/
    template <class T>
    void vector<T>::clear()
    {
        numElements = 0;

    }

    /********************************************
     * VECTOR : ASSIGN OPERATOR OVERLOAD
     *******************************************/
    template <class T>
    vector <T>& vector <T> :: operator = (const vector <T>& rhs)
    {
        // out of extreme caution, technically not needed
        numElements = 0;

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
     * VECTOR : PUSH_BACK
     * adds new element to array; calls for
     * resizing if the array is at capacity
     *******************************************/
    template <class T>
    void vector<T> :: push_back(const T& element)
    {
        // if Array is at capacity, resize (double)
        if (this->size() == this->capacity())
        {
            resize( (capacity() * 2) );
        }

        // add new element to the end of the array
        data[numElements++] = element;
        
    }
    /***********CONSTANT ITERATOR DECLARATION AND EXPERIMENT***************/

    /**************************************************
    * VECTOR CONST_ITERATOR
    * A constant iterator through the internal array of
    * the vector
    *************************************************/
    template <class T>
    class vector <T> ::const_iterator
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
    * VECTOR :: ITERATOR :: CEND
    * Note that you have to use "typename" before
    * the return value type
    ********************************************/
    template <class T>
    typename vector <T> ::const_iterator vector <T> ::cend() const //had to make a promise to compiler
    {
        return const_iterator(data + numElements);
    }

}; // namespace custom

#endif // VECTOR_H

