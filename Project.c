#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define INF -1

struct nodo3{
  int el;
  struct nodo3 *next;
};

typedef struct nodo3 *lista;

struct nodo2{
    int el; //autonomia veicolo
    struct nodo2 *root;
    struct nodo2 *sx;
    struct nodo2 *dx;
};

typedef struct nodo2 *veicolo;

struct nodo1{
    int km; //distanza inizio autostrada
    veicolo v;
    struct nodo1 *root;
    struct nodo1 *sx;
    struct nodo1 *dx;
};

typedef struct nodo1 *stazione;

int numero_stazioni=0;
veicolo path=NULL;
stazione a=NULL;
stazione inversa=NULL;
lista stampa=NULL;
int check_new_stazione=0;
int check_delete_stazione=0;
int check_auto=0;
int num_staz=0;

stazione NewStazione(stazione a, int dist);
stazione CancStazione(stazione tmp, stazione z);
stazione ricerca_stazione(stazione a, int distanza);
stazione NewAuto(stazione a, int dist,int autonomia);
veicolo CancAuto(veicolo a, int autonomia);
veicolo ricerca_veicolo(veicolo a, int distanza);
void pianifica_percorso(stazione a, int start, int end);
int autonomia_max(veicolo v);
veicolo tree_successore_veicolo(veicolo x);
stazione tree_successore_stazione(stazione x);
veicolo tree_minimo_veicolo(veicolo x);
stazione tree_minimo_stazione(stazione x);
veicolo tree_aggiungiVeicolo(veicolo a, int el);
stazione tree_aggiungiStazione(stazione a, int el);
stazione tree_rimuovi_stazione(stazione a);
veicolo tree_rimuovi_veicolo(veicolo a);
lista crea_percorso(lista percorso, int start, int end, stazione a, int *prec, int num);
stazione tree_massimo_stazione(stazione x);
stazione tree_predecessore_stazione(stazione x);

int main(){
int input1=0, input2=0, input3=0;
int i=0, j=0;
char x[10],y[10],z[10];
char comando[20];
stazione tmp=NULL;

while((comando[0]=getc_unlocked(stdin)) != EOF){
  i=1;
  do{
    comando[i]=getc_unlocked(stdin);
    i++;
  }while(comando[i-1]!=' ');
  i=0;
  if(strncmp(comando, "aggiungi-stazione", 17) == 0){
    do{
      x[i]=getc_unlocked(stdin);
      i++;
    }while(x[i-1] != ' ');
    i=0;
    do{
      y[i]=getc_unlocked(stdin);
      i++;
    }while(y[i-1] != ' ' && y[i-1] != '\n');
    i=0;
    input1 = atoi(x);
    input2 = atoi(y);
    a = NewStazione(a, input1);
    if(check_new_stazione == 1){
      tmp = ricerca_stazione(a, input1);
      for(j=0; j<input2; j++){
        do{
          z[i]=getc_unlocked(stdin);
          i++;
        }while(z[i-1] != ' ' && z[i-1] != '\n');
          input3=atoi(z);
          if(ricerca_veicolo(tmp->v, input3) == NULL){
            tmp->v = tree_aggiungiVeicolo(tmp->v, input3);
          }
        i=0;
    }
  }else{
    while(getc_unlocked(stdin) != '\n'){}
  }
  }
if(strncmp(comando, "demolisci-stazione", 18) == 0){
  do{
      x[i]=getc_unlocked(stdin);
      i++;
    }while(x[i-1] != '\n');
    i=0;
    input1 = atoi(x);
  tmp = ricerca_stazione(a, input1);
  if(tmp==NULL){
    printf("non demolita\n");
  }else{
  a = CancStazione(a, tmp);
  printf("demolita\n");
  numero_stazioni--;
  }
}
if(strncmp(comando, "aggiungi-auto", 13) == 0){
    do{
      x[i]=getc_unlocked(stdin);
      i++;
    }while(x[i-1] != ' ');
    i=0;
    do{
      y[i]=getc_unlocked(stdin);
      i++;
    }while(y[i-1] != '\n');
    i=0;
    input1 = atoi(x);
    input2 = atoi(y);
  a = NewAuto(a, input1, input2);
 }
if(strncmp(comando, "rottama-auto", 12) == 0){
  check_auto=0;
  i=0;
  do{
      x[i]=getc_unlocked(stdin);
      i++;
    }while(x[i-1] != ' ');
    i=0;
    do{
      y[i]=getc_unlocked(stdin);
      i++;
    }while(y[i-1] != '\n');
    i=0;
    input1 = atoi(x);
    input2 = atoi(y);
  tmp=ricerca_stazione(a, input1);
  if(tmp==NULL){
    printf("non rottamata\n");
  }else{
  tmp->v = CancAuto(tmp->v, input2);
  }
  if(check_auto==1){
  printf("rottamata\n");
  }
}
if(strncmp(comando, "pianifica-percorso", 18) == 0){
   do{
      x[i]=getc_unlocked(stdin);
      i++;
    }while(x[i-1] != ' ');
    i=0;
    do{
      y[i]=getc_unlocked(stdin);
      i++;
    }while(y[i-1] != '\n');
    i=0;
    input1 = atoi(x);
    input2 = atoi(y);
  pianifica_percorso(a, input1, input2);
}
}
return 0;
}

stazione NewStazione(stazione a, int dist){
  stazione tmp=NULL;
    check_new_stazione = 0;
    tmp = ricerca_stazione(a, dist);
    if(tmp==NULL)
    a = tree_aggiungiStazione(a, dist);
    if(check_new_stazione == 0){
      printf("non aggiunta\n");
      return a;
    }
    if(check_new_stazione == 1){
      numero_stazioni++;
      printf("aggiunta\n");
      return a;
    }
    return a;
}

stazione tree_aggiungiStazione(stazione a, int el){
  stazione x=a;
  stazione y=NULL;
  stazione new = malloc(sizeof(struct nodo1));
  new->km = el;
  new->v=NULL;
  new->dx = NULL;
  new->sx = NULL;
  new->root=NULL;
  while(x!=NULL){
    y=x;
    if(x->km > el){
      x=x->sx;
    }else{
      x=x->dx;
    }
  }
  new->root=y;
  if(y==NULL){
    check_new_stazione=1;
    a = new;
  }else{
    if(y->km > el){
      check_new_stazione=1;
      y->sx=new;
    }else{
      check_new_stazione=1;
      y->dx=new;
    }
  }
  return a;
}

stazione CancStazione(stazione a, stazione z){
  stazione x=NULL;
  stazione y=NULL;
  if(z->sx == NULL || z->dx == NULL){
   y = z;
  }else{
   y = tree_successore_stazione(z);
  }
  if(y->sx != NULL){
    x = y->sx;
  }else{
   x = y->dx;
  }
  if(x!=NULL){
    x->root = y->root;
  }
  if(y->root == NULL){
    a = x;
  }else{
    if(y == y->root->sx){
      y->root->sx = x;
    }else{
      y->root->dx = x;
    }
  }
  if(y != z){
   z->km = y->km;
   z->v = y->v;
  }
  return a;
}

stazione ricerca_stazione(stazione a, int distanza){
  while(a != NULL && a->km != distanza){
      if(distanza < a->km){
      a = a->sx;
      }else{
        a = a->dx;
      }
  }
  return a;
}

stazione tree_successore_stazione(stazione x){
  stazione y=NULL;
  if (x->dx != NULL){
    return tree_minimo_stazione(x->dx);
  }
  y = x->root;
  while(y != NULL && x == y->dx){
    x = y;
    y = y->root;
  }
  return y;
}

stazione tree_minimo_stazione(stazione x){
    while(x->sx != NULL){
    x = x->sx;
  }
  return x;
}

stazione NewAuto(stazione a, int dist, int autonomia){
  stazione tmp=a;
  tmp = ricerca_stazione(a, dist);
  if(tmp != NULL){
  if(ricerca_veicolo(tmp->v, autonomia) == NULL){
    tmp->v = tree_aggiungiVeicolo(tmp->v, autonomia);
  }
  printf("aggiunta\n");
  return a;
}
printf("non aggiunta\n");
  return a;
}

veicolo tree_aggiungiVeicolo(veicolo a, int el){
  veicolo tmp=a;
  veicolo y=NULL;
  veicolo new = malloc(sizeof(struct nodo2));
  new->el = el;
  new->dx = NULL;
  new->sx = NULL;
  new->root=NULL;
  while(tmp!=NULL){
    y=tmp;
    if(tmp->el > el){
      tmp=tmp->sx;
    }else{
      tmp=tmp->dx;
    }
  }
  new->root=y;
  if(y==NULL){
    a = new;
  }else{
    if(y->el > el){
      y->sx=new;
    }else{
      y->dx=new;
    }
  }
  return a;
}

veicolo CancAuto(veicolo a, int autonomia){
  veicolo x=NULL;
  veicolo y=NULL;
  veicolo z=ricerca_veicolo(a, autonomia);
  check_auto=0;
  if(z==NULL){
    printf("non rottamata\n");
    return a;
  }else{
  check_auto=1;
  if(z->sx == NULL || z->dx == NULL){
   y = z;
  }else{
   y = tree_successore_veicolo(z);
  }
  if(y->sx != NULL){
    x = y->sx;
  }else{
   x = y->dx;
  }
  if(x!=NULL){
    x->root = y->root;
  }
  if(y->root == NULL){
    a = x;
  }else{
    if(y == y->root->sx){
      y->root->sx = x;
    }else{
      y->root->dx = x;
    }
  }
  if(y != z){
   z->el = y->el;
  }
  return a;
  }
}

veicolo tree_successore_veicolo(veicolo x){
  veicolo y=NULL;
  if (x->dx != NULL){
    return tree_minimo_veicolo(x->dx);
    }
    y = x->root;
    while(y != NULL && x == y->dx){
      x = y;
      y = y->root;
    }
    return y;
}

veicolo tree_minimo_veicolo(veicolo x){
  while(x!=NULL && x->sx != NULL){
    x = x->sx;
  }
  return x;
}

veicolo ricerca_veicolo(veicolo a, int autonomia){
  while(a != NULL && a->el != autonomia){
    if(autonomia < a->el){
     a = a->sx;
    }else{
      a = a->dx;
    }
}
return a;
}

void pianifica_percorso(stazione a, int start, int end){
  stazione copia = NULL;
  stazione z=NULL;
  stazione controllo=NULL;
  int i=0, j=0, max=0, count=0, k=0, ultima_stazione=0, max_j=0, flag_end=0;
  lista percorso = NULL;

  if(start <= end){
    z=ricerca_stazione(a, start);
    copia=z;
    count=numero_stazioni;
    int ricerca[count];
    int prec[count];
    memset(ricerca, 0, sizeof(ricerca));
    memset(prec, INF, sizeof(prec));
    i=0;
    ricerca[i]=1;
    prec[i]=i;
    controllo=tree_successore_stazione(copia);
    while(copia!=NULL && copia->km <= end && flag_end==0){
      ricerca[i]=1;
      j=max_j;
      max = autonomia_max(copia->v);
      j++;
      if((copia->km == start || copia->km == ultima_stazione) && (max < (controllo->km - copia->km))){
        flag_end=2;
      }
      if(copia->km + max > ultima_stazione){
        while(controllo != NULL && (controllo->km - max) <= copia->km){
        if(controllo->km == end){
          flag_end=1;
        }
        if(j > max_j){
            max_j=j;
        }
        if(ricerca[j] == 0){
              prec[j]=i;
              ricerca[j]=1;
        }
        ultima_stazione=controllo->km;
        j++;
        controllo=tree_successore_stazione(controllo);
      }
    }
    copia=tree_successore_stazione(copia);
    i++;
    }
    copia=z;
    i=0;
    percorso = crea_percorso(percorso, start, end, copia, prec, count);
    if(percorso == NULL || flag_end==2){
      printf("nessun percorso\n");
    }else{
    while(percorso->next != NULL){
      printf("%d ", percorso->el);
      percorso=percorso->next;
    }
    printf("%d", percorso->el);
    printf("\n");
    }
  }

    if(start > end){
        z=ricerca_stazione(a, start);
        copia=z;
        count=numero_stazioni;
        int ricerca[count];
        int ricerca_tmp[count];
        int prec[count];
        memset(ricerca, 0, sizeof(ricerca));
        memset(ricerca_tmp, 0, sizeof(ricerca_tmp));
        memset(prec, INF, sizeof(prec));
        i=0;
        ricerca[i]=1;
        prec[i]=i;
        controllo = tree_predecessore_stazione(copia);
        while(copia!=NULL && copia->km >= end && flag_end==0){
        max = autonomia_max(copia->v);
          if(copia->km == start && ricerca_tmp[i]==0){
          ricerca_tmp[i]=1;
          ricerca[i]=1;
          j=i;
          j++;
          while(controllo != NULL && ((controllo->km) + max) >= copia->km && flag_end==0){
           if(controllo->km == end){
            flag_end=1;
            }
              if(ricerca[j] == 0){
                prec[j]=i;
                ricerca[j]=1;
              }
              ultima_stazione =controllo->km;
              j++;
              controllo =tree_predecessore_stazione(controllo);
          }
          if(controllo->km > ultima_stazione){
            flag_end=1;
          }
          k=j;
          i=j-1;
          copia=tree_successore_stazione(controllo);
          }else if(copia->km ==ultima_stazione){
          ricerca[i]=1;
          ricerca_tmp[i]=1;
          j=k;
          while(controllo != NULL && ((controllo->km) + max) >= copia->km && flag_end==0){
           if(controllo->km == end){
            flag_end=1;
            }
              if(ricerca[j] == 0){
                prec[j]=i;
                ricerca[j]=1;
              }
              j++;
              ultima_stazione=controllo->km;
              controllo =tree_predecessore_stazione(controllo);
          }
          k=j;
          i--;
          copia=tree_successore_stazione(copia);
          }else if(ricerca_tmp[i]==1){
          copia=ricerca_stazione(a, ultima_stazione);
          max=autonomia_max(copia->v);
          i=j-1;
          if(ricerca_tmp[i]==1){
            flag_end=1;
          }
          ricerca[i]=1;
          ricerca_tmp[i]=1;
          j=k;
          while(controllo != NULL && ((controllo->km) + max) >= copia->km && flag_end==0){
            if(controllo->km == end){
            flag_end=1;
            }
            if(ultima_stazione > controllo->km){
            ultima_stazione=controllo->km;;
            }
              if(ricerca[j] == 0){
                prec[j]=i;
                ricerca[j]=1;
              }
              j++;
              controllo=tree_predecessore_stazione(controllo);
            }
          copia=tree_successore_stazione(copia);
          k=j;
          i--;
        }else{
            ricerca[i]=1;
            ricerca_tmp[i]=1;
            j=k;
            while(controllo != NULL && ((controllo->km) + max) >= copia->km && flag_end==0){
            if(controllo->km == end){
            flag_end=1;
            }
              if(ricerca[j] == 0){
                prec[j]=i;
                ricerca[j]=1;
              }
              j++;
              ultima_stazione=controllo->km;
              controllo=tree_predecessore_stazione(controllo);
            }
             k=j;
            i--;
            copia=tree_successore_stazione(copia);
        }
        }
        copia=z;
        i=0;
    percorso = crea_percorso(percorso, start, end, copia, prec, count);
    if(percorso == NULL){
      printf("nessun percorso\n");
    }else{
    while(percorso->next != NULL){
      printf("%d ", percorso->el);
      percorso=percorso->next;
    }
    printf("%d", percorso->el);
    printf("\n");
    }
  }
}

lista crea_percorso(lista percorso, int start, int end, stazione a, int *prec, int num){
  if(start <= end){
  stazione copia=a;
  int i=0;
  lista new = malloc(sizeof(struct nodo3));
  new->el = end;
  new->next=NULL;
  if(percorso!=NULL){
    new->next=percorso;
    percorso=new;
  }else{
    percorso=new;
  }
  if(start == end){
    return percorso;
  }else{
    if(copia!=NULL){
      while(i<num && copia->km != end){
        copia=tree_successore_stazione(copia);
        i++;
      }
      if(i >= num || i==INF){
        return NULL;
      }else{
        i = prec[i];
        copia=a;
        if(i >= num || i==INF){
          return NULL;
        }else{
          while(i != 0 && copia != NULL){
            copia=tree_successore_stazione(copia);;
            i--;
          }
          }
          if(copia != NULL)
          percorso = crea_percorso(percorso, start, copia->km, a, prec, num);
      }
    }else{
      return NULL;
    }
  }
  return percorso;
  }else{
  stazione copia=a;
  int i=0;
  lista new = malloc(sizeof(struct nodo3));
  new->el = end;
  new->next=NULL;
  if(percorso!=NULL){
    new->next=percorso;
    percorso=new;
  }else{
    percorso=new;
  }
  if(start == end){
    return percorso;
  }else{
    if(copia!=NULL){
      while(i<num && copia->km != end){
        copia=tree_predecessore_stazione(copia);;
        i++;
      }
      if(i >= num || i==INF){
        return NULL;
      }else{
        i = prec[i];
        copia=a;
        if(i >= num || i==INF){
          return NULL;
        }else{
          while(i != 0 && copia!=NULL){
            copia=tree_predecessore_stazione(copia);;
            i--;
          }
          }
          if(copia != NULL)
          percorso = crea_percorso(percorso, start, copia->km, a, prec, num);
      }
    }else{
      return NULL;
    }
  }
  return percorso;
  }
}

int autonomia_max(veicolo v){
  if(v!=NULL){
  while(v->dx != NULL){
    v = v->dx;
  }
  return v->el;
  }
  return 0;
}

stazione tree_predecessore_stazione(stazione x){
  stazione y=NULL;
  if (x->sx != NULL){
    return tree_massimo_stazione(x->sx);
  }
  y = x->root;
  while(y != NULL && x == y->sx){
    x = y;
    y = y->root;
  }
  return y;
}

stazione tree_massimo_stazione(stazione x){
    while(x->dx != NULL){
    x = x->dx;
  }
  return x;
}
