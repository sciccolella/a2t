#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include "sais.h"


typedef struct Node {
  char name[25];
  char label[255];

  struct Node *first_child;
  struct Node *next_sibling;
  struct Node *parent;
} node_t;

node_t *
node_new(char *name, char *label) {
  node_t *node = malloc(sizeof(node_t));
  strcpy(node->name, name);
  strcpy(node->label, label);
  node->first_child = NULL;
  node->next_sibling = NULL;
  node->parent = NULL;

  return node;
}

void
node_append(node_t *parent, node_t *node){
  node->next_sibling = parent->first_child;
  parent->first_child = node;
  node->parent = parent;
}

void
destroy_tree(node_t *node){
  if (node == NULL) return;
  destroy_tree(node->first_child);
  destroy_tree(node->next_sibling);
  free(node);
}

void
print_tree_rec(node_t *node) {
  if (node == NULL) return;
  print_tree_rec(node->first_child);
  print_tree_rec(node->next_sibling);
  if(node->parent != NULL)
    printf("\t%s -> %s [label=%s];\n",
              node->parent->name,
              node->name,
              node->label);
}

/**
Stampa in DOT code.
Usare il seguente sito per la visualizzazione dell'albero:
http://www.webgraphviz.com/
*/
void
print_tree(node_t *root){
  printf("%s\n", "digraph g {");
  print_tree_rec(root);
  printf("%s\n", "}");
}

int
min(int a, int b) {
  if (a < b) return a;
  return b;
}

int
lcp2(char *a, char *b){
  int i = 0;
  while(a[i] == b[i]){
    i++;
  }
  return i;
}

void
label_constr_rec(char *buff, node_t *node, int *sa, char *word, int word_size) {
  node_t *r_child = node->first_child;
  node_t *l_child = node->first_child->next_sibling;

  // Caso base della ricorsione:
  // viene calcolato come CASO 1
  if (r_child->first_child == NULL &&
        l_child->first_child == NULL) {
          int a = atoi(r_child->name);
          int b = atoi(l_child->name);
          char label[255];
          int l = lcp2(word+a, word+b);

          // Label del figlio destro:
          // w[SA[r] + lcp(r,l): ]
          strcpy(label, word + a + l);
          //label[word_size - a -l] = '\0';
          strcpy(r_child->label, label);

          // Label del figlio sinistro:
          // w[SA[l] + lcp(r,l): ]
          strcpy(label, word + b + l);
          //label[word_size - b -l] = '\0';
          strcpy(l_child->label, label);

          int m = min(a,b) -1;
          l++;
          strcpy(label, word+m);
          label[l] = '\0';

          strcpy(buff, label);
          return;
        }

  // Passo ricorsivo:
  // buff contiene l'etichetta del percorso dalla radice al nodo attuale
  if (r_child->first_child == NULL){
    
  } else {

  }



  strcpy(buff, "x");
}

node_t *
a2t_rec(int *sa, unsigned int *lcp, int r, int l, char *word, int word_size){

  //In questo caso il nodo che si va a creare è una foglia
  if(r == l-1) {
    char name[25];
    sprintf(name, "%d", sa[r]+1);
      //DA CANCELLARE <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
      //int start = sa[r] + lcp[r];
      //char label[255];
      //strcpy(label, word+start);
      return node_new(name, "label");
  }


  // cerco il minimo tra lcp[r+1] e lcp[l-1]
  // NOTA: r e l sono i chiamanti qindi loro non
  // devono essere conteggiati nella ricerca del minimo
  int min = INT_MAX;
  for(int i = r + 1; i < l; i++){
    if (lcp[i] < min) {
      min = i;
    }
  }

  char name[25];
  sprintf(name, "I%d", min);
  //Se il nodo n è intermedio allora:  <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<CANCELLARE
  //label[n] = word[sa[n] + lcp[n] : word_size - 1]
  //int start = sa[r] + lcp[r];
  //char label[255];
  //strncpy(label, word+start, word_size - start - 1);
  //label[word_size - start - 1] = '\0';
  node_t *actual_node = node_new(name, "label");

  // chiamo ricorsivamente a2t_rec sulle porzioni
  // lcp[r:min] e lcp[min:l]
  // i due sottoalberi che vengono creati dalle due ricorsioni
  // sono i figli del nodo attuale
  node_t *r_subtree = a2t_rec(sa, lcp, r, min, word, word_size);
  node_t *l_subtree = a2t_rec(sa, lcp, min, l, word, word_size);

  node_append(actual_node, r_subtree);
  node_append(actual_node, l_subtree);

  return actual_node;

}

node_t *
a2t(int *sa, unsigned int *lcp, int size, char *word, int word_size) {
  node_t *root = node_new("root","root");

  //cerca gli 0 in lcp
  int zeros[size];
  memset(zeros, -1, sizeof(zeros));

  int zeros_p = 0;
  for(int i = 0; i < size; i++) {
    if (lcp[i] == 0) {
      zeros[zeros_p] = i;
      zeros_p++;
    }
  }


  //chiamate ricorsive per costruzione Suffix Tree
  int i = 0;
  while (zeros[i] != -1 || i >= size) {
    node_t *node;
    if (zeros[i+1] != -1)
      node = a2t_rec(sa, lcp, zeros[i], zeros[i+1], word, word_size);
    else
      node = a2t_rec(sa, lcp, zeros[i], size, word, word_size);


    // CASO 2
    // In questo caso il nodo ritornato dalla chiamata ricorsiva è una foglia
    // il cui padre è la radice perciò posso sapere subito la sua etichetta:
    // label[n] = word[sa[n]:]
    if (node->first_child == NULL){
      int start = atoi(node->name) -1;
      char label[255];
      strcpy(label, word+start);
      //label[word_size - start] = '\0';
      strcpy(node->label, label);
    } else {
        // CASO 1
        node_t *r_child = node->first_child;
        node_t *l_child = node->first_child->next_sibling;
        if (r_child->first_child == NULL &&
              l_child->first_child == NULL){
                int a = atoi(r_child->name);
                int b = atoi(l_child->name);
                char label[255];
                int l = lcp2(word+a, word+b);

                // Label del figlio destro:
                // w[SA[r] + lcp(r,l): ]
                strcpy(label, word + a + l);
                //label[word_size - a -l] = '\0';
                strcpy(r_child->label, label);

                // Label del figlio sinistro:
                // w[SA[l] + lcp(r,l): ]
                strcpy(label, word + b + l);
                //label[word_size - b -l] = '\0';
                strcpy(l_child->label, label);

                // Label del nodo attuale:
                // w[ min{a,b}: min{a,b} + lcp(a,b)]
                int m = min(a,b) -1;
                l++;
                strcpy(label, word+m);
                label[l] = '\0';
                //label[word_size -m -l] = '\0';
                strcpy(node->label, label);
              } else {

                // CASO 3
                // Risolvibile ricorsivamente
                char label[255];
                if (r_child->first_child == NULL) {
                  label_constr_rec(label, l_child, sa, word, word_size);

                } else {
                  label_constr_rec(label, r_child, sa, word, word_size);

                }

                printf("Rec: %s.\n", label);

              }


      }

      node_append(root, node);
      i++;
    }

  return root;
}


int
main(int argc, const char *argv[]) {

  //TEST with BANANA$ LCP
  /*
  char word[] = "BANANA_";
  int sa[] = {6,5,3,1,0,4,2};
  int lcp[] = {0,0,1,3,0,0,2};
  */

  // Controlla il numero di argomenti:
  if(argc != 2) {
    printf("ERRORE: Uso corretto: ./a2t FILE\n");
    return -1;
  }

  FILE *fp;
  const char *fname;
  unsigned char *T;
  int *SA;
  long n;


  // Apertura del file
  if((fp = fopen(fname = argv[1], "rb")) == NULL) {
          printf("Impossibile aprire il file: %s", fname);
          perror(NULL);
          exit(EXIT_FAILURE);
  }

    /* Get the file size. */
  if(fseek(fp, 0, SEEK_END) == 0) {
          n = ftell(fp);
          rewind(fp);
          if(n < 0) {
                  fprintf(stderr, "%s: Cannot ftell `%s': ", argv[0], fname);
                  perror(NULL);
                  exit(EXIT_FAILURE);
          }
  } else {
          fprintf(stderr, "%s: Cannot fseek `%s': ", argv[0], fname);
          perror(NULL);
          exit(EXIT_FAILURE);
  }

  /* Allocate 5n bytes of memory. */
  T = (unsigned char *)malloc((size_t)n * sizeof(unsigned char));
  SA = (int *)malloc((size_t)n * sizeof(int));
  if((T == NULL) || (SA == NULL)) {
          fprintf(stderr, "%s: Cannot allocate memory.\n", argv[0]);
          exit(EXIT_FAILURE);
  }

  /* Read n bytes of data. */
  if(fread(T, sizeof(unsigned char), (size_t)n, fp) != (size_t)n) {
          fprintf(stderr, "%s: %s `%s': ",
                  argv[0],
                  (ferror(fp) || !feof(fp)) ? "Cannot read from" : "Unexpected EOF in",
                  argv[1]);
          perror(NULL);
          exit(EXIT_FAILURE);
  }
  fclose(fp);

    /* Construct the suffix array. */
  if(sais(T, SA, (int)n) != 0) {
          fprintf(stderr, "%s: Cannot allocate memory.\n", argv[0]);
          exit(EXIT_FAILURE);
  }

  unsigned int lcp[n];
  sais_lcp(T, SA, lcp, n);
  n--;
  T[n] = '\0';

  node_t *root = a2t(SA+1, lcp+1, n, (char*) T, n);

  free(SA);
  free(T);




  /*
  char word[] = "ANANAS_";
  int sa[] = {6,0,2,4,1,3,5};
  int lcp[] = {0,0,3,1,0,2,0};
  */
  /*
  char word[] = "ABRACADABRA_";
  int sa[] = {11,10,7,0,3,5,8,1,4,6,9,2};
  int lcp[] = {0,0,1,4,1,1,0,3,0,0,0,2};
  */




  print_tree(root);

  destroy_tree(root);

  return 0;
}
