#ifndef __LIST__
#define __LIST__

#include "include.h"

typedef struct _Edge_t edge_t;
struct _Edge_t {
  size_t first;
  size_t second;
};

Inline int edge_cmp(const edge_t *a,const edge_t *b)
{ return (a->first == b->first && a->second == b->second) || (a->first == b->second && a->second == b->first); }

typedef struct _Link* list_t;
typedef const list_t Clist_t;
struct _Link {
    edge_t s;
    list_t next;
};

#ifdef __cplusplus
extern "C" {
#endif

/*creer une liste vide*/
Inline list_t initl()
{ return NULL; }

/*test si la liste est vide*/
Inline int isEmptyl(Clist_t l)
{ return l == NULL; }

/*ajoute l'arête e dans la liste l*/
extern void push(list_t* l, edge_t e);

/*renvoie le dernier element de la liste et le supprime de la liste*/
extern edge_t pull(list_t* l);

/*renvoie le nième élément de la liste*/
extern edge_t lget(Clist_t l,long n);

/*renvoie 1 si l'arête est dans la liste, 0 sinon*/
extern int isInl(Clist_t l, edge_t e);

/*supprime l'élément e dans la liste*/
extern void rm_l(list_t* l, edge_t e);

/*renvoie la copie de la liste l mais dans le sens oppose des elements*/
extern list_t copyl(Clist_t l);

/*applique la fonction func à tous les éléments de l*/
extern void map(list_t l, void (*func)(edge_t *e));

/*renvoie la liste l1 - l2*/
extern list_t list_diff(Clist_t l1, Clist_t l2);

/*affiche la list l sur le flux stderr*/
extern void printfl(Clist_t l);

/*renvoie 1 si k se trouve dans les i premières listes contenues dans V*/
extern int isInprec(Clist_t* l, int i, edge_t k);

/*renvoie le nombre d'éléments de la liste V*/
extern unsigned long nbrOfElement(Clist_t l);

/*libère l'espace mémoire alloué à l*/
extern void freel(list_t l);

#ifdef __cplusplus
}
#endif

#endif
