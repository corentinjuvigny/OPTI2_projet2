#include "pl.h"

#define NAMESIZE 25

static void get_location(list_t l,long *tab,edge_t e)
{
  size_t s = 0,c = 0;
  while (l != NULL) {
    if ( l->s.first == e.first && l->s.second != e.second ) {
      tab[s++] = c;
    }
    c++;
    l = l->next;
  }
  tab[s] = -1;
}

static int any_in(long *tab,size_t i)
{
  long j = (long)i;
  long cmp = 0;
  while (tab[cmp++] > 0) {
    if ( tab[cmp] == j )
     return 1;
  }
  return 0; 
}

static int populate(CPXENVptr env,CPXLPptr lp,Graph *g)
{
  int    status = 0;
  size_t i,j,k,l,t,pos;
  size_t nbredges = grph_nbr_edges(g);
  size_t numcols = g->sz + nbredges + 2 * nbredges * g->sz;
  size_t numrows = g->sz * nbredges + nbredges + g->sz + 1;
  size_t numnz = numcols * numrows;
  size_t lmt_zv  = g->sz;
  size_t lmt_xij = g->sz + nbredges;
  size_t lmt_yij = numcols;
  double obj[numcols];
  char   xctype[numcols];
  char const *colname[numcols];
  CPXNNZ *rmatbeg = NULL;
  CPXDIM *rmatind = NULL;
  double *rmatval = NULL;
  double *rhs = NULL;
  char   *sense = NULL;
  const char* *rowname = NULL;

  list_t edges_l = NULL,current = NULL;
  
  status = CPXXchgobjsen(env,lp,CPX_MIN); /* Problem is minimisation */
  if ( status )  goto TERMINATE;

  for (i = 0; i < g->sz; i++) {
    for (j = i; j < g->sz; j++) {
      if ( g->m_adj[i][j] == 1 ) {
        edge_t e = {i,j};
        push(&edges_l,e);
      }
    }
  }

  /* ajouter l'ensemble des colonnes : zi,xij,yijv */

  /* initialisation des zv */
  for (i = 0; i < lmt_zv; i++) {
    char *tmp = malloc(NAMESIZE*sizeof(char));
    snprintf(tmp,NAMESIZE,"z%ld",i);
    obj[i] = 1.0;
    xctype[i] = 'B';
    colname[i] = tmp;
  }
  
  current = edges_l;
  for (; i < lmt_xij; i++) {
    if ( current == NULL ) {
      fprintf(stderr,"Error nbedges != size edges_l\n");
      goto TERMINATE;
    }
    char *tmp = malloc(NAMESIZE*sizeof(char));
    snprintf(tmp,NAMESIZE,"x%ld_%ld",current->s.first,current->s.second);
    obj[i] = 0.0;
    xctype[i] = 'B';
    colname[i] = tmp;
    current = current->next;
  }

  for (k = 0; k < g->sz; k++) {
    current = edges_l;
    while (current != NULL) {
      if ( i > numcols ) {
        fprintf(stderr,"i = %ld > numcols = %ld\n",i,numcols);
        goto TERMINATE;
      }
      char *tmp1 = malloc(NAMESIZE*sizeof(char));
      char *tmp2 = malloc(NAMESIZE*sizeof(char));
      snprintf(tmp1,NAMESIZE,"y%ld_%ld_%ld",current->s.first,current->s.second,k);
      snprintf(tmp2,NAMESIZE,"y%ld_%ld_%ld",current->s.second,current->s.first,k);
      obj[i] = obj[i+1] = 0.0;
      xctype[i] = xctype[i+1] = 'B';
      colname[i] = tmp1;
      colname[i+1] = tmp2;
      current = current->next;
      i += 2;
    }
  }

  status = CPXXnewcols(env,lp,numcols,obj,NULL,NULL,xctype,colname);
  if ( status )  goto TERMINATE;
 
  rmatbeg = malloc(numrows * sizeof(*rmatbeg));
  rmatind = malloc(numnz * sizeof(*rmatind));
  rmatval = malloc(numnz * sizeof(*rmatval));
  rhs = malloc(numrows * sizeof(*rhs));
  sense = malloc(numrows * sizeof(*sense));
  rowname = malloc(numrows * sizeof(*rowname));

  /* ajouter première contrainte */

  l = t = 0;
  rmatbeg[l] = t;
  for (i = 0; i < lmt_zv; i++) { //z_v
    rmatind[i] = i;
    rmatval[i] = 0.0;
  }
  for (;i < lmt_xij; i++) {
    rmatind[i] = i;
    rmatval[i] = 1.0;
  }
  for (;i < lmt_yij; i++) {
    rmatind[i] = i;
    rmatval[i] = 0.0;
  }
  sense[l] = 'E';
  rhs[l] = (double)g->sz - 1.0;
  do {
    char *tmp = malloc(NAMESIZE*sizeof(char));
    snprintf(tmp,NAMESIZE,"c%ld",l);
    rowname[l] = tmp;
  } while(0);
  t += numcols;
  l++;

  /* ajouter deuxième contrainte */

  for (k = 0; k < g->sz; k++) {
    pos = 0;
    current = edges_l;
    while (current != NULL) {
      char *tmp = malloc(NAMESIZE*sizeof(char));
      snprintf(tmp,NAMESIZE,"c%ld",l);
      rowname[l] = tmp;
      rmatbeg[l] = t;
      for (i = 0; i < lmt_zv; i++) {
        rmatind[t+i] = i;
        rmatval[t+i] = 0.0;
      }
      for (; i < lmt_xij; i++) {
        rmatind[t+i] = i;
        if ( i - lmt_zv == pos ) {
          rmatval[t+i] = -1.0;
        } else {
          rmatval[t+i] = 0.0;
        }
      }
      for (; i < lmt_yij; i++) {
        rmatind[t+i] = i;
        if ( i - lmt_xij == 2 * k * pos ) {
          rmatval[t+i] = 1.0;
          rmatval[t+i+1] = 1.0;
        } else {
          rmatval[t+i] = 0;
        }
      }
      sense[l] = 'E';
      rhs[l] = 0.0;
      current = current->next;
      pos++;
      t += numrows;
      l++;
    }
  }

  /* ajouter troisième contrainte */

  pos = 0;
  current = edges_l;
  while (current != NULL) {
    long tab[nbredges],cmp = 0;
    char *tmp = malloc(NAMESIZE*sizeof(char));
    snprintf(tmp,NAMESIZE,"c%ld",l);
    rowname[l] = tmp;
    rmatbeg[l] = t;
    for (i = 0; i < lmt_zv; i++) {
        rmatind[t+i] = i;
        rmatval[t+i] = 0.0;
    }
    for (; i < lmt_xij; i++) {
      rmatind[t+i] = i;
      if ( i - lmt_zv == pos ) {
        rmatval[t+i] = 1.0;
      } else {
        rmatval[t+i] = 0.0;
      }
    }
    get_location(edges_l,tab,current->s);
    while (tab[cmp] > 0) {
      tab[cmp] = 2 * tab[cmp] + current->s.second * nbredges;
      cmp++;
    }
    for (; i < lmt_yij; i++) {
      rmatind[t+i] = i;
      if ( any_in(tab,i-lmt_xij) ) {
        rmatval[t+i] = 1.0;
      } else {
        rmatval[t+i] = 0.0;
      } 
    }
    sense[l] = 'E';
    rhs[l] = 1.0;
    current = current->next;
    pos++;
    t += numrows;
    l++;
  }

  /* ajouter quatrième contrainte */

  for (k = 0; k < g->sz; k++) {
    long tab[nbredges];
    edge_t e = {k,k};
    char *tmp = malloc(NAMESIZE*sizeof(char));
    snprintf(tmp,NAMESIZE,"c%ld",l);
    rowname[l] = tmp;
    rmatbeg[l] = t;
    for (i = 0; i < lmt_zv; i++) {
      rmatind[t+i] = i;
      if (i != k) {
        rmatval[t+i] = 0.0;
        continue;
      }
      rmatval[t+i] = -(double)grph_degree_vertex(g,i);
    }
    get_location(edges_l,tab,e);
    for (; i < lmt_xij; i++) {
      rmatind[t+i] = i;
      if ( any_in(tab,i-lmt_zv) ) {
        rmatval[t+i] = 1.0;
      } else {
        rmatval[t+i] = 0.0;
      }
    }
    for (; i < lmt_yij; i++) {
      rmatind[t+i] = i;
      rmatval[t+i] = 0.0;
    }
    sense[l] = 'L';
    rhs[l] = 2.0;
    t += numrows;
    l++;
  }

  status = CPXXaddrows(env,lp,0,numrows,numnz,rhs,sense,rmatbeg,
                       rmatind,rmatval,NULL,rowname);
   if ( status )  goto TERMINATE;

TERMINATE:
  for (i = 0; i < numrows; i++) {
    const char *tmp = rowname[i];
    xfree((void*)tmp);
  }
  xfree((void*)rowname);
  xfree(sense);
  xfree(rhs);
  xfree(rmatval);
  xfree(rmatind);
  xfree(rmatbeg);
  freel(edges_l);
  for (i = 0; i < numcols; i++) {
    const char *tmp = colname[i];
    xfree((void*)tmp);
  }

  return status;
}

int mbvst_pl(Graph *g)
{
  int status = 0;
  int solstat;
  double objval;
  CPXENVptr env = NULL;
  CPXLPptr lp = NULL;
  
  env = CPXXopenCPLEX(&status);
  if ( env == NULL ) {
    char errmsg[CPXMESSAGEBUFSIZE];
    fprintf(stderr, "Could not open CPLEX environment.\n");
    CPXXgeterrorstring(env, status, errmsg);
    fprintf (stderr, "%s", errmsg);
    goto TERMINATE;
  }

  status = CPXXsetintparam(env,CPXPARAM_ScreenOutput,CPX_ON);
  if ( status ) {
    fprintf (stderr, 
             "Failure to turn on screen indicator, error %d.\n", status);
    goto TERMINATE;
  }
  
  lp = CPXXcreateprob(env, &status, "MBVST_pl");
  if ( lp == NULL ) {
    fprintf (stderr, "Failed to create LP.\n");
    goto TERMINATE;
  }

  status = populate(env,lp,g);
  if ( status ) {
    fprintf (stderr, "Failed to populate problem.\n");
    goto TERMINATE;
  }

  status = CPXXmipopt (env, lp);
  if ( status ) {
    fprintf (stderr, "Failed to optimize MIP.\n");
    goto TERMINATE;
  }

  solstat = CPXXgetstat (env, lp);
  printf ("\nSolution status = %d\n", solstat);

  status = CPXXgetobjval (env, lp, &objval);
  if ( status ) {
     fprintf (stderr,"No MIP objective value available.  Exiting...\n");
     goto TERMINATE;
  }

  printf ("Solution value  = %f\n\n", objval);


  status = CPXXwriteprob (env, lp, "myMipex.lp", NULL);
  if ( status ) {
     fprintf (stderr, "Failed to write LP to disk.\n");
     goto TERMINATE;
  }

TERMINATE:
  if ( lp != NULL ) {
    status = CPXXfreeprob(env,&lp);
    if ( status ) {
      fprintf (stderr, "CPXXfreeprob failed, error code %d.\n", status);
    }
  }
  
  if ( env != NULL ) {
    status = CPXXcloseCPLEX(&env);
    /* Note that CPXXcloseCPLEX produces no output,
      so the only way to see the cause of the error is to use
      CPXXgeterrorstring.  For other CPLEX routines, the errors will
      be seen if the CPXPARAM_ScreenOutput indicator is set to CPX_ON. */

    if ( status ) {
      char errmsg[CPXMESSAGEBUFSIZE];
      fprintf (stderr, "Could not close CPLEX environment.\n");
      CPXXgeterrorstring(env, status, errmsg);
      fprintf (stderr, "%s", errmsg);
    }
  }
  return g->sz;
}
