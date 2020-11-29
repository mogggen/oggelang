#ifndef ERROR_VIEW_H
#define ERROR_VIEW_H 

#include <vector>
#include "view.h"
#include "file_location.h"
#include "block_alloc.h"

struct Error
{
    FileLocation loc;
    char* err_string;
};

class ErrorView : public View
{
public:
    std::vector<Error> errors;
    BlockAlloc error_alloc;
    
    void draw(Window* window, Area* area);
    
    const char* name() const {return "Errors";};
};

void create_error_view(ErrorView* view);
void clear_errors(ErrorView* view);
void close(ErrorView* view);

#endif /* ERROR_VIEW_H */
