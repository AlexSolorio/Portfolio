
#ifndef __Lab14__SharedQueue__
#define __Lab14__SharedQueue__

#include <mutex>
//lock before we access anything, unlock before returning
std::mutex lock;
std::mutex print_lock;

template <typename T>
class SharedQueue {
public:
    SharedQueue();
    ~SharedQueue();
    SharedQueue(const SharedQueue<T>&) = delete;
    SharedQueue<T>& operator=(const SharedQueue<T>&) = delete;

    
    //Return true if the queue is empty
    bool IsEmpty() const;
    
    //Enqueue the next item at the tail of the queue.
    void Add(T value);
    
    //Dequeue the next queue element and store it in "item" variable.  The function returns false if the queue is empty and no item can be retrieved.
    bool Remove(T &item);
    void Print();
private:
    struct QueueItem {
        T item;
        QueueItem *next;
    };
    void FreeQueue(QueueItem *);
    QueueItem *head;
    QueueItem *tail;
};

template <typename T>
SharedQueue<T>::SharedQueue()
{
    head = nullptr;
    tail = nullptr;
}

template <typename T>
SharedQueue<T>::~SharedQueue()
{
    FreeQueue(head);
    head = nullptr;
    tail = nullptr;
}

template <typename T>
bool SharedQueue<T>::IsEmpty() const
{
    return (head == nullptr);
}

template <typename T>
void SharedQueue<T>::Add(T value)
{
    QueueItem *newItem = new QueueItem;
    newItem->item = value;
    newItem->next = nullptr;
    lock.lock();
    if (IsEmpty())          //no items
    {
        head = newItem;
        tail = newItem;
    }
    else if (head == tail)  //only one item
    {
        tail = newItem;
        head->next = tail;
    }
    else                    //normal case
    {
        tail->next = newItem;
        tail = newItem;
    }
    lock.unlock();
}

template <typename T>
bool SharedQueue<T>::Remove(T &item)
{
    std::lock_guard<std::mutex> guard (lock);
    if (IsEmpty()){                 //no items
        return false;
    }
    else if (head == tail)          //only one item
    {
        item = head->item;
        delete head;
        head = nullptr;
        tail = nullptr;
    }
    else                            //normal case
    {
        item = head->item;
        QueueItem *tmp = head;
        head = head->next;
        delete tmp;
        tmp = nullptr;
    }
    return true;
}

template <typename T>
void SharedQueue<T>::Print()
{
    print_lock.lock();
    for (QueueItem *n = head; n; n = n->next)
        std::cout << n->item << " ";
    std::cout << "n";
    print_lock.unlock();
}

template <typename T>
void SharedQueue<T>::FreeQueue(QueueItem *n)
{
    if (n == nullptr)
        return;
    FreeQueue(n->next);
    delete n;
}

#endif
