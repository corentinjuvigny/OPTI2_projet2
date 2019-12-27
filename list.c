#include <stdio.h>
#include <stdlib.h>
#include "list.h"

void push(list *l, edge_t e)
{
    list q = (list)malloc(sizeof(*q));
    q->s = e;
    q->next = *l;
    *l = q;
}

edge_t pull(list *l)
{
    edge_t s = {0,0,0,0};
    if (!isEmptyl(*l)) {
        list q = *l;
        s = (*l)->s;
        *l = (*l)->next;
        free(q);
    }
    return s;
}

edge_t lget(list l,long n)
{
  long cmp = 0;
  edge_t res = {0,0,0,0};
  while (l != NULL) {
    if (cmp == n) {
      res = l->s;
      break;
    }
    l = l->next;
  }
  return res;
}

int isInl(list l, edge_t e)
{
    list m = l;
    while (!isEmptyl(m)) {
        if (edge_cmp(&(m->s),&e))
          return 1;
        m = m->next;
    }
    return 0;
}

void rm_l(list *l, edge_t e)
{
    list ant = initl();
    list p = *l;
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

list copyl(list l)
{
    list m = initl();
    list current = l;
    while (!isEmptyl(current)) {
        push(&m,current->s);
        current = current->next;
    }
    return m;
}

void map(list l,void (*func)(edge_t *e))
{
  while (l != NULL) {
    func(&l->s);
    l = l->next;
  }
}

list list_diff(Clist l1, Clist l2)
{
  list l = initl();
  list current1 = l1, current2 = l2;
  char b;
  while (current1 != NULL) {
    b = 0;
    current2 = l2;
    while (current2 != NULL) {
      if (current1->s.f == current2->s.f && current1->s.s == current2->s.s)
        b = 1;
      current2 = current2->next;
    }
    if (!b)
      push(&l,current1->s);
    current1 = current1->next;
  }
  return l;
}

void list_change_alpha(list *l, edge_t e, int alpha)
{
  list current = *l;
  while (current != NULL) {
    if (edge_cmp(&current->s,&e)) {
      current->s.alpha = alpha;
      break;
    }
  }
}

void printfl(list l)
{
    list current = l;
    while (current != NULL) {
        fprintf(stdout,"-> [%ld,%ld,%d,%d]"
                ,current->s.f
                ,current->s.s
                ,current->s.alpha
                ,current->s.omega
                );
        current = current->next;
    }
    fprintf(stdout,"\n");
}

/*fonction de parcours*/

int isInprec(Clist* V, int i, edge_t k)
{
    int j;
    for (j = 0; j < i; j++)
      if (isInl(V[j], k) == 1)
        return 0;
    return 1;
}

long nbrOfElement(Clist l)
{
    long n = 0;
    list q = l;
    while (!isEmptyl(q)) {
        n++;
        q = q->next;
    }
    return n;
}

void freel(list l)
{
    list q;
    while (!isEmptyl(l)) {
        q = l;
        l = l->next;
        free(q);
    }
}
