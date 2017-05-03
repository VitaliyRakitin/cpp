#include "allocator.h"
#include "allocator_pointer.h"
#include "allocator_error.h"


/* выделяем память размера N */
Pointer Allocator::alloc(size_t N) {
    // Память в буфере ещё не выделялась
    // просто создаём нулевой элемент в корне root
    if (root->prev  == nullptr && 
        root->next  == nullptr && 
        root->first == nullptr && 
        root->last  == nullptr &&
        N + sizeof(point) < static_cast<char*>(buff.last) - static_cast<char*>(buff.first))
    {
        root->first = buff.first;
        root->last = static_cast<void*>(static_cast<char*>(buff.first) + N);
        return Pointer(root);
    }

    // добавляем в конец списка
    if (last_node > static_cast<void*>(static_cast<char*>(root->last) + N + sizeof(point))){
        last_node->next = last_node - 1; // добавили новый элемент
        (last_node->next)->prev = last_node; // указатель на следующий из предыдущего = текущий 
        last_node = last_node->next; // last_node = новый
        last_node->first = (last_node->prev)->last; // начало новой строки - конец предыдущей
        last_node->last = static_cast<void*>(static_cast<char*>(root->prev->last) + N); // конец строки
        last_node->next = nullptr; // следующего пока нет
        return Pointer(last_node);
    }

    // посмотрим, есть ли место в дырках 
    for(point *cur = static_cast<point*>(buff.last) - 1; cur->next != nullptr; cur = cur->next){
        void *s = cur->next->first;
        if(s != nullptr){
            size_t sz = static_cast<char*>(s) - static_cast<char*>(cur->last); // размер кусочка
            if(sz >= N){
                last_node--;
                last_node->prev = cur;
                last_node->first = cur->last;
                last_node->last = static_cast<void*>(static_cast<char*>(cur->last) + N);
                last_node->next = cur->next;
                cur->next = last_node;
		        return Pointer(last_node);
            }
        }
    }
throw AllocError(AllocErrorType::NoMemory, "Try defraq");

}


void Allocator::defrag() {
    int i = 0;
    for(point *cur = static_cast<point*>(buff.last) -> next; cur != nullptr; cur = cur->next){
        if(cur->first != nullptr) {
            if (cur->first > cur->prev->last) {
                size_t sz = static_cast<char*>(cur->last) - static_cast<char*>(cur->first);
                memcpy(cur->prev->last, cur->first, sz);
                cur->first = cur->prev->last;
                cur->last = static_cast<char*>(cur->first) + sz;
            }
        }
        else{
            cur->last = cur->prev->last;
        }
    }
}


Allocator::Allocator(void* base, size_t base_size){
    buff.first = base;
    buff.last = static_cast<char *>(base) + base_size;
    root = static_cast<point *>(buff.last);
    last_node = root;
}

void Allocator::realloc(Pointer& pointer, size_t N) {
    void *info_link = pointer.get();
    size_t size = pointer.get_size();
    free(pointer);
    pointer = alloc(N);
    memcpy(pointer.get(), info_link, size);
}

void Allocator::free(Pointer& p){ 
    p.free(); 
}
