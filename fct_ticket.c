#include<stdio.h>
#include<string.h>
#include"fct_ticket.h"

void add_in_tail(int number, struct Node_ticket *node_ticket)
{
    if(node_ticket->next == NULL)
    {
        struct Ticket t;
        struct Node_ticket nt;
        
        // t = (Ticket*) malloc(sizeof(Ticket));
        
        nt.current = &t;
        nt.next = NULL;
        
        t.number = number;
        t.reserved = false;
        
        node_ticket->next = &nt;
    }
    else 
    {
        add_in_tail(number, node_ticket->next);
    }
}


struct Node_ticket *init_ticket()
{
    struct Node_ticket node_ticket;
    struct Ticket ticket;
    
    ticket.number = 1;
    ticket.reserved = false;
    
    node_ticket.current = &ticket;
    node_ticket.next = NULL;
    
    
    for(int i = 2; i <= 100; i++)
    {
        add_in_tail(i, &node_ticket);
    }
    
    return &node_ticket;
}


void set_reservation(struct Ticket *ticket, enum Reserved reservation)
{
    (*ticket).reserved = reservation;
}


void list(struct Node_ticket *node_ticket, char *message)
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