
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

          int m = calc_min(a,b) -1;
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

  // CASO BASE:
  // Si crea una foglia:
  if(r == l-1) {
    char name[25];
    sprintf(name, "%d", sa[r]+1);
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

  //cerca i minimi in lcp
  int mins[l];
  memset(mins, -1, sizeof(mins));

  int mins_p = 0;
  for(int i = r + 1; i < l; i++) {
    if (lcp[i] == 0) {
      mins[mins_p] = i;
      mins_p++;
    }
  }

  //chiamate ricorsive per costruzione Suffix Tree
  int i = 0;
  while (mins[i] != -1 || i >= l) {
    node_t *node;
    if (mins[i+1] != -1)
      node = a2t_rec(sa, lcp, mins[i], mins[i+1], word, word_size);
    else
      node = a2t_rec(sa, lcp, mins[i], l, word, word_size);

  char name[25];
  sprintf(name, "I%d", min);
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


    // COSTRUZIONE DELLE ETICHETTE
    // CASO 3 non corretto

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
                int m = calc_min(a,b) -1;
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
