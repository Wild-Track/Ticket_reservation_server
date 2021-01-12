#include<stdio.h>
#include<string.h>
#include<time.h>
#include"fct_ticket.h"

void add_in_tail_ticket(int number, struct Node_ticket *node_ticket)
{
    if(node_ticket->next == NULL)
    {
        struct Ticket* t;
        struct Node_ticket* nt;
        
        nt = (struct Node_ticket*) malloc(sizeof(struct Node_ticket));
        t = (struct Ticket*) malloc(sizeof(struct Ticket));
        
        t->number = number;
        t->reserved = false;
        
        nt->current = t;
        nt->next = NULL;
        
        node_ticket->next = nt;
    }
    else 
    {
        add_in_tail_ticket(number, node_ticket->next);
    }
}


void init_ticket(struct Node_ticket *node_ticket)
{
    struct Ticket* ticket;
    
    ticket = (struct Ticket*) malloc(sizeof(struct Ticket));
    
    ticket->number = 1;
    ticket->reserved = false;
    
    node_ticket->current = ticket;
    node_ticket->next = NULL;
    
    
    for(int i = 2; i <= 100; i++)
    {
        add_in_tail_ticket(i, node_ticket);
    }
    
}


void set_reservation_ticket(struct Ticket *ticket, enum Reserved reservation)
{
    (*ticket).reserved = reservation;
}


void list_ticket(struct Node_ticket *node_ticket, char *message)
{
    char temp[13];
    struct Node_ticket *p;

    sprintf(temp, "%d", node_ticket->current->number);
    strcat(message, temp);
    
    for(p = node_ticket->next; p != NULL; p = p->next)
    {
        if(p->current->reserved == false)
        {
            sprintf(temp, ",%d", p->current->number);
            strcat(message, temp);
        }
    }
    strcat(message, "\n");
}


void add_in_tail_reservation(char *last_name, char *first_name, struct Node_reservation *node_reservation, struct Ticket *ticket)
{
    if(node_reservation->current == NULL)
    {
        char *random;
        random = (char*) malloc(10 * sizeof(char));
        struct Reservation *reservation; 
        reservation = (struct Reservation*) malloc(sizeof(struct Reservation));
        
        reservation->first_name = first_name;
        reservation->last_name = last_name;
        reservation->ticket = ticket;
        
        srand(time(NULL));
        for(int i = 0; i < 10; i++)
        {
            random[i] = rand() % 10;
        }
        reservation->file_number = random;
        
        node_reservation->current = reservation;
        node_reservation->next = NULL;
    }
    else if(node_reservation->next == NULL)
    {
        char *random;
        random = (char *)malloc(10 * sizeof(char));
        struct Reservation *r;
        struct Node_reservation *nr;

        nr = (struct Node_reservation *)malloc(sizeof(struct Node_reservation));
        r = (struct Reservation *)malloc(sizeof(struct Reservation));

        r->first_name = first_name;
        r->last_name = last_name;
        r->ticket = ticket;

        srand(time(NULL));
        for (int i = 0; i < 10; i++)
        {
            random[i] = rand() % 10;
        }
        r->file_number = random;

        nr->current = r;
        nr->next = NULL;

        node_reservation->next = nr;
    }
    else
    {
        add_in_tail_reservation(last_name, first_name, node_reservation, ticket);
    }
    
}
