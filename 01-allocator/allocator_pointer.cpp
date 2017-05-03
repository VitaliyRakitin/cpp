#include "allocator_pointer.h"
Pointer::Pointer(){
    create(nullptr, nullptr, nullptr, nullptr);
}
Pointer::Pointer(point *prev_node, point *next_node, void *new_first, void *new_last){
    create(prev_node, next_node, new_first, new_last);
}

Pointer::Pointer(point *new_ptr){
    if (new_ptr != nullptr)
        ptr = new_ptr;
    else ptr = nullptr;
}

Pointer::~Pointer(){
    free();
}


void* Pointer::get() const{
    if (ptr->first == nullptr)
        return nullptr;
    return ptr->first;
}

void* Pointer::get_last() const{
    if (ptr->last == nullptr)
        return nullptr;
    return ptr->last;
}

void* Pointer::get_next() const{
    if (ptr->next == nullptr)
        return nullptr;
    return ptr->next;
}

void* Pointer::get_prev() const{
    if (ptr->prev == nullptr)
        return nullptr;
    return ptr->prev;
}

/* задать значения Pointer */
void Pointer::create(point *prev_node, point *next_node, void *new_first, void *new_last){
    ptr->prev = prev_node; 
    ptr->next = next_node;
    ptr->first = new_first;
    ptr->last = new_last;
}


/* узнать размер выделенной памяти */
size_t Pointer::get_size(){
    if(ptr == nullptr) 
        return 0;
    if(ptr->first == nullptr) 
        return 0;

    return static_cast<char *>(ptr->last) - static_cast<char *>(ptr->first);
}

void Pointer:: free(){
    if (ptr != nullptr){
        if (ptr->next != nullptr){
            (ptr->next)->prev = ptr->prev;
            if (ptr->prev != nullptr)
                (ptr->prev)->next = ptr->next;
        }
        if (ptr->next != nullptr) 
            ptr->next = nullptr;

        if (ptr->prev != nullptr) 
            ptr->prev = nullptr;

        if (ptr->first != nullptr) 
            ptr->first = nullptr;
    
        if (ptr->last != nullptr) 
            ptr->last = nullptr;    
    }
}
