#ifndef FCT_TICKET
#define FCT_TICKET
#include "type_ticket.h"
#include <stdlib.h>

void add_in_tail_ticket(int number, struct Node_ticket *node_ticket);
void init_ticket(struct Node_ticket *node_ticket);
void set_reservation_ticket(struct Ticket *ticket, enum Reserved reservation);
void list_ticket(struct Node_ticket *ticket, char *message);

void add_in_tail_reservation(char *last_name, char *first_name, struct Node_reservation *node_reserve, struct Ticket *ticket);

#endif