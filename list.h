#ifndef __LIST__
#define __LIST__

#include "include.h"

typedef struct _Edge_t edge_t;
struct _Edge_t {
  size_t f;
  size_t s;
  int alpha;
  int omega;
};

Inline int edge_cmp(const edge_t *a,const edge_t *b)
{ return a->f == b->f && a->s == b->s; }

typedef struct _Link* list;
typedef const list Clist;
struct _Link {
    edge_t s;
    list next;
};

/*creer une liste vide*/
Inline list initl()
{ return NULL; }

/*test si la liste est vide*/
Inline int isEmptyl(Clist l)
{ return l == NULL; }

/*ajoute l'arête e dans la liste l*/
extern void push(list* l, edge_t e);

/*renvoie le dernier element de la liste et le supprime de la liste*/
extern edge_t pull(list* l);

/*renvoie le nième élément de la liste*/
extern edge_t lget(Clist l,long n);

/*renvoie 1 si l'arête est dans la liste, 0 sinon*/
extern int isInl(Clist l, edge_t e);

/*supprime l'élément e dans la liste*/
extern void rm_l(list* l, edge_t e);

/*renvoie la copie de la liste l mais dans le sens oppose des elements*/
extern list copyl(Clist l);

/*applique la fonction func à tous les éléments de l*/
extern void map(list l, void (*func)(edge_t *e));

/*renvoie la liste l1 - l2*/
extern list list_diff(Clist l1, Clist l2);

/*modifie la valeur de alpha de l'élément e*/
extern void list_change_alpha(list *l, edge_t e, int alpha);

/*affiche la list l sur le flux stderr*/
extern void printfl(Clist l);

/*renvoie 1 si k se trouve dans les i premières listes contenues dans V*/
extern int isInprec(Clist* l, int i, edge_t k);

/*renvoie le nombre d'éléments de la liste V*/
extern long nbrOfElement(Clist l);

/*libère l'espace mémoire alloué à l*/
extern void freel(list l);

#endif
