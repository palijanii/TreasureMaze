/**
 * @file deque.cpp
 * Implementation of the Deque class.
 *
 */

template <class T>
Deque<T>::Deque(){
    n1 = 0; //index
    n2 = 0; // size
}

/**
 * Adds the parameter object to the right of the Deque.
 *
 * @param newItem object to be added to the Deque.
 */
template <class T>
void Deque<T>::pushR(T newItem)
{
    data.push_back(newItem);
    n2++;
}

/**
 * Removes the object at the left of the Deque, and returns it to the
 * caller.
 *
 * See .h file for implementation notes. 
 *
 * @return The item that used to be at the left of the Deque.
 */
template <class T>
T Deque<T>::popL()
{

    // T tempData = data[n1];
    // n1++;
    // if (n1 >= n2 - n1) {
    //     data.erase(data.begin(), data.begin()+n1);
    //     data.shrink_to_fit();
    //     n1 = 0;
    //     n2 = data.size();
    // }
    // return tempData;

    T temp = data[n1];
    n2--;
    n1++;  
    if (n2 <= n1) {
        for (int i=n2-1; i>=0; i--) {
            data[i] = data[n1+i];
            data.pop_back();
        }
        if ((int)data.size()>n2) data.pop_back();    
        n1 = 0;
    }
    return temp;


}
/**
 * Removes the object at the right of the Deque, and returns it to the
 * caller.
 *
 * @return The item that used to be at the right of the Deque.
 */
template <class T>
T Deque<T>::popR()
{
    T temp = data[n2-1];
    data.pop_back();
    n2--;
    if (n2 <= n1) {
        for (int i=n1-1; i>=0; i--) {
            data[i] = data[i+n2];
            data.pop_back();
        }
        n1 = 0;        
    }
    return temp;
}

    


/**
 * Finds the object at the left of the Deque, and returns it to the
 * caller. Unlike popL(), this operation does not alter the deque.
 *
 * @return The item at the front of the deque.
 */
template <class T>
T Deque<T>::peekL()
{
    return data[n1];
}

/**
 * Finds the object at the right of the Deque, and returns it to the
 * caller. Unlike popR(), this operation does not alter the deque.
 *
 * @return the value of The item at the right of the deque.
 */
template <class T>
T Deque<T>::peekR()
{
    return data[n1+n2-1];
}

/**
 * Determines if the Deque is empty.
 *
 * @return bool which is true if the Deque is empty, false otherwise.
 */
template <class T>
bool Deque<T>::isEmpty() const
{
    return n2 == 0;
}
