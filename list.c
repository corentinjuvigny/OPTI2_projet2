#include <stdio.h>
#include <stdlib.h>
#include "list.h"

void push(list_t *l, edge_t e)
{
    list_t q = (list_t)malloc(sizeof(*q));
    q->s = e;
    q->next = *l;
    *l = q;
}

edge_t pull(list_t *l)
{
    edge_t s = {0,0};
    if (!isEmptyl(*l)) {
        list_t q = *l;
        s = (*l)->s;
        *l = (*l)->next;
        free(q);
    }
    return s;
}

edge_t lget(list_t l,long n)
{
  long cmp = 0;
  edge_t res = {0,0};
  while (l != NULL) {
    if (cmp == n) {
      res = l->s;
      break;
    }
    l = l->next;
  }
  return res;
}

int isInl(list_t l, edge_t e)
{
    list_t m = l;
    while (!isEmptyl(m)) {
        if (edge_cmp(&(m->s),&e))
          return 1;
        m = m->next;
    }
    return 0;
}

void rm_l(list_t *l, edge_t e)
{
    list_t ant = initl();
    list_t p = *l;
    while (!isEmptyl(p)) {
        if (edge_cmp(&(p->s),&e)) {
            if (isEmptyl(ant)) {
                *l = p->next;
                free(p);
            }
            else {
                ant->next = p->next;
                free(p);
            }
            break;
        }
        ant = p;
        p = p->next;
    }
}

list_t copyl(list_t l)
{
    list_t m = initl();
    list_t current = l;
    while (!isEmptyl(current)) {
        push(&m,current->s);
        current = current->next;
    }
    return m;
}

void map(list_t l,void (*func)(edge_t *e))
{
  while (l != NULL) {
    func(&l->s);
    l = l->next;
  }
}

list_t list_diff(Clist_t l1, Clist_t l2)
{
  list_t l = initl();
  list_t current1 = l1, current2 = l2;
  char b;
  while (current1 != NULL) {
    b = 0;
    current2 = l2;
    while (current2 != NULL) {
      if (edge_cmp(&current1->s,&current2->s))
        b = 1;
      current2 = current2->next;
    }
    if (!b)
      push(&l,current1->s);
    current1 = current1->next;
  }
  return l;
}

void printfl(list_t l)
{
    list_t current = l;
    while (current != NULL) {
        fprintf(stdout,"-> [%ld,%ld]"
                ,current->s.first
                ,current->s.second
                );
        current = current->next;
    }
    fprintf(stdout,"\n");
}

/*fonction de parcours*/

int isInprec(Clist_t* V, int i, edge_t k)
{
    int j;
    for (j = 0; j < i; j++)
      if (isInl(V[j], k) == 1)
        return 0;
    return 1;
}

long nbrOfElement(Clist_t l)
{
    long n = 0;
    list_t q = l;
    while (!isEmptyl(q)) {
        n++;
        q = q->next;
    }
    return n;
}

void freel(list_t l)
{
    list_t q;
    while (!isEmptyl(l)) {
        q = l;
        l = l->next;
        free(q);
    }
}
