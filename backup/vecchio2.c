void
a2t2(node_t *actual_node, int *sa, unsigned int *lcp, int r, int l, char *word, int size){
  printf("Chiamata: (r,l) -> (%d,%d)\n", r,l);
  // CASO BASE:
  // Si crea una foglia:
  if(r == l-1) {
    char name[25];
    sprintf(name, "%d", sa[r]+1);
    strcpy(actual_node->name, name);
    printf("foglia\n");
    return;
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

  if(min == INT_MAX) return;
  if(l == r) return;
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
  if (mins_p > 0) {
    printf("in while\n");
    int i = 0;
    while (mins[i] != -1 && i < l) {
      node_t *node = node_new("name", "label");
      if (mins[i+1] != -1)
        a2t2(node, sa, lcp, mins[i], mins[i+1], word, size);
      else
        a2t2(node, sa, lcp, mins[i], size, word, size);

      printf("%d\n", min);
      if(min == 0) {
        printf("\tif\n");
        printf("%d\n", node);
        node_append(actual_node, node);
      } else {
        printf("\telse\n");
        char name[25];
        sprintf(name, "I%d", min);
        node_t *intermediate = node_new(name, "label");
        node_append(intermediate, node);
        node_append(actual_node, intermediate);
      }


      i++;
    }
  } else {
    printf("else\n");
    // chiamo ricorsivamente a2t_rec sulle porzioni
    // lcp[r:min] e lcp[min:l]
    // i due sottoalberi che vengono creati dalle due ricorsioni
    // sono i figli del nodo attuale
    char name[25];
    sprintf(name, "I%d", min);
    node_t *r_subtree = node_new(name, "label");
    a2t2(r_subtree, sa, lcp, r, min, word, size);
    node_t *l_subtree = node_new(name, "label");
    a2t2(l_subtree, sa, lcp, min, l, word, size);

    node_append(actual_node, r_subtree);
    node_append(actual_node, l_subtree);
  }

}
