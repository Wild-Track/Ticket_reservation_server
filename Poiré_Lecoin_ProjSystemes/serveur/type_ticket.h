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

struct Reservation
{
    char *last_name;
    char *first_name;
    struct Ticket *ticket;
};

struct Node_reservation
{
    struct Reservation *current;
    struct Node_reservation *next;
};

#endif