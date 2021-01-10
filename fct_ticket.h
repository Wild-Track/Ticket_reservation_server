#ifndef FCT_TICKET
#define FCT_TICKET
#include "type_ticket.h"
#include <stdlib.h>

void add_in_tail(int number, struct Node_ticket *node_ticket);
void init_ticket(struct Node_ticket *node_ticket);
void set_reservation(struct Ticket *ticket, enum Reserved reservation);
void list(struct Node_ticket *ticket, char *message);

#endif