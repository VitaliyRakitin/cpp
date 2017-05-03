#ifndef ALLOCATOR_POINTER
#define ALLOCATOR_POINTER
#include <cstddef>
using namespace std;
// Forward declaration. Do not include real class definition
// to avoid expensive macros calculations and increase compile speed
class Allocator;

struct point{
    void* first;
    void* last;
    point* next;
    point* prev;
};


class Pointer {
public:
    Pointer();
    Pointer(point *new_ptr);
    Pointer(point *prev_node, point *next_node, void *new_first, void *new_last);
    ~Pointer();

    void *get() const;
    void *get_last() const;
    void *get_next() const;
    void *get_prev() const;

    void create(point *prev_node, point *next_node, void *new_first, void *new_last);
    size_t get_size();
    void free();

private:
    point* ptr;
};

#endif //ALLOCATOR_POINTER