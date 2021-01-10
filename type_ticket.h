#ifndef TYPE_TICKET
#define TYPE_TICKET

enum Reserved{true, false};

struct Ticket
{
    int number;
    enum Reserved reserved;
};

struct Node_ticket
{
    struct Ticket *current;
    struct Node_ticket *next;
};

#endif