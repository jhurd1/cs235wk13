 /***********************************************************************
* Header:
*    queue
* Summary:
*    This class contains the notion of a queue: a bucket to hold
*    data for the user that can change sizes dynamically but whose only
*    accessible members are the first item to have been pushed (aka front())
*    and the last element (back()). It uses the first in, first out rule.
*    Of particular interest is the double access being handled with a
*    wrapping of the array, to avoid queue creep.
*
*    This will contain the class definition of:
*       queue                  : similar to std::queue
* Author
*    Benjamin Dyas, Daniel Perez
************************************************************************/
#ifndef QUEUE_H
#define QUEUE_H

namespace custom
{
    /************************************************
    * QUEUE
    * A class that holds an array and that changes
    * sizes dynamically. Only top/last member is
    * accessible.
    ***********************************************/
    template <class T>
    class queue
    {
    public:
        // constructors, destructors
        queue() :numPop{0}, numPush{0}, numCapacity{0}, data{nullptr}{} // default constructor
        queue(int numCapacity);   //non-default constructor
        queue(const queue & rhs); //copy constructor
        ~queue() { if (data != nullptr) delete[] data;}

        // overloaded operators
        queue& operator = (const queue& rhs);

        // container interfaces
        int     size()      const   { return numPush - numPop; }
        int     capacity()  const   { return numCapacity; }
        bool    empty()     const   { return ( size() == 0); }
        void    clear()             { numPush = numPop = 0; }

        // queue specific 
        void    push    (const T& element);
        void    pop     ();
        T&      front   ();
        const T front   () const;
        T&      back    ();
        const T back    () const;


    private:
        //member variables
        T * data;
        int numCapacity;
        int numPush;
        int numPop;


        //methods
        void resize(int newCapacity);
        int  iTail() { return (numPush - 1) % numCapacity; }
        int  iHead() { return numPop % numCapacity; }
    };

    /**********************************************
    * QUEUE : NON-DEFAULT CONSTRUCTOR
    * Preallocate the array to "numCapacity"
    **********************************************/
    template <class T>
    queue<T>::queue(int numCapacity) {

        try
        {
            data = new T[numCapacity];
        }
        catch (std::bad_alloc)
        {
            //if allocation fails, create an empty queue with numCapacity zero
            data = nullptr; 
            this->numCapacity = 0;
            //we could have delegated this to the default constructor
            
            throw "ERROR: Unable to allocate a new buffer for queue";
        }

        this->numCapacity = numCapacity;
        this->numPop = 0;
        this->numPush = 0;

    }

    /**********************************************
    * QUEUE : COPY CONSTRUCTOR
    * 
    **********************************************/ //
    template <class T>
    queue<T>::queue(const queue<T> & rhs) {

        numPop = 0;
        numPush = 0;
        // do nothing if there is nothing to do
        if (rhs.size() == 0)
        {
            numCapacity = 0;
            data = nullptr;
            return;
        }

        // attempt to allocate
        try
        {
            data = new T[rhs.size()];
        }
        catch (std::bad_alloc)
        {
            //to verify later on in case allocation fails
            data = nullptr;

            throw "ERROR: Unable to allocate buffer";
        }

        // copy over the capacity
        this->numCapacity = rhs.size();

        // copy the items over one at a time using the assignment operator        
        for (int i = rhs.numPop; i < rhs.numPush; i++)
        {
            push(rhs.data[i % rhs.numCapacity]);
        }
            
    }

    /**********************************************
    * QUEUE : OVERLOADED ASSIGNMENT OPERATOR
    * 
    **********************************************/
    template <class T>
    queue<T>& queue<T>::operator=(const queue<T>& rhs)
    {
        
        numPush = 0;
        numPop = 0;

        // if incoming object is bigger, resize allocated space
        if (rhs.size() > this->capacity())
        {
            this->resize(rhs.size());
        }

        //Copy data from source to local array
        for (int i = rhs.numPop; i < rhs.numPush; i++)
        {
            push(rhs.data[i % rhs.size()]);
        }

        return *this;

    }

    /**********************************************
    * QUEUE : CLEAR
    *
    **********************************************/
    //decided to implement inline

    /**********************************************
    * QUEUE : PUSH
    *
    **********************************************/
    template <class T>
    void queue<T> ::push(const T& element)
    {
        // if Array is at capacity, resize (double)
        if (this->size() == this->capacity())
        {
            resize((capacity() * 2));
        }

        //increment numPush to move the tail ahead
        numPush++;
        // add new element to the end of the array
        data[iTail()] = element;
    }

    /**********************************************
    * QUEUE : POP
    *
    **********************************************/
    template<class T>
    void queue<T> ::pop()
    {
        if (!empty())
        {
            //increment numPush to "orphan" popped memory
            numPop++;
        }
    }

    /**********************************************
    * QUEUE : FRONT (SETTER)
    *
    **********************************************/
    template<class T>
    T& queue<T>::front()
    {
        if (empty())
        {
            throw "ERROR: attempting to access an element in an empty queue";
        }
        else
        {
            return data[iHead()];
        }
        
    }

    /**********************************************
    * QUEUE : CONST FRONT (GETTER)
    *
    **********************************************/
    template<class T>
    const T queue<T>::front() const
    {
        if (empty())
        {
            throw "ERROR: attempting to access an element in an empty queue";
        }
        else
        {
            return data[iHead()];
        }

    }

    /**********************************************
* QUEUE : BACK (SETTER)
*
**********************************************/
    template<class T>
    T& queue<T>::back()
    {
        if (empty())
        {
            throw "ERROR: attempting to access an element in an empty queue";
        }
        else
        {
            return data[iTail()];
        }

    }

    /**********************************************
    * QUEUE : CONST BACK (GETTER)
    *
    **********************************************/
    template<class T>
    const T queue<T>::back() const
    {
        if (empty())
        {
            throw "ERROR: attempting to access an element in an empty queue";
        }
        else {
            return data[iTail()];
        }

    }

    /**********************************************
    * QUEUE : RESIZE
    *
    **********************************************/
    template <class T>
    void queue<T> ::resize(int max)
    {
        

        // The special case of newCapacity being 0 is handled
        // This happens when vector is default constructed with a 
        //    size and capacity of zero. The first time it doesn't double:
        //    it goes from zero to one
        if (max == 0)
        {
            max = 1;
        }

        // create a temporary pointer for array swap
        T* tempData = nullptr;

        //attempt to allocate
        try
        {
            tempData = new T[max];
        }
        catch (std::bad_alloc)
        {
            throw "ERROR: Unable to allocate a new buffer for Queue.";
            return;
        }
        //Copy data from source to local array

        int tempPush = 0;
        for (int i = numPop; i < numPush; i++)
        {
            tempData[tempPush++] = data[i % size()];
        }

        delete[] data;
        
        numPop = 0;
        numPush = tempPush;
        
        //By copying the pointer, the nameless array in the heap is transferred 
        data = tempData;
        
        this->numCapacity = max;

    }
}; //end namespace custom    

#endif // queue_H