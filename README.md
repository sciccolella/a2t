**Nome**: Simone </br>
**Cognome**: Ciccolella </br>
**Matricola**: 762234

Introduzione
-------------
Per la creazione del Suffix Array ho utilizzato senza troppe modifiche il codice presente in *suftest.c*, pertanto anche i parametri sono simili:

$ > ./a2t < FILENAME >

I file allegati contengono le stringhe:
- 1.data: banana_
- 2.data: ananas_
- 3.data: abracadabra_

Ho dovuto utilizzare il simobolo _ come terminatore di stringa invece del simbolo $ a causa della stampa in DOT code, in quanto il simbolo $ è un simbolo riservato.


Struttura dell'albero
----------------------
Per la costruzione dell'albero ho utilizzato una struttura in cui in ogni nodo viene salvato:
 - **Nome**: per le foglie indica il valore corrispondente del SA. **NB:** l'implementazione di *sais* indica i valori di SA 0-based, l'albero creato da *a2t* utilizza invece una notazione 1-based
 - **Label**: nell'albero finale indica l'etichetta tra un nodo e il padre
 - **First Child**: puntatore al primo figlio del nodo
 - **Next Sibling**: puntatore al prossimo fratello del nodo

Creazione etichette dei rami
---------------------------
Non avendo trovato negli appunti (presi in classe e trovati disponibili su moodle) o nel libro un algoritmo che fosse in grado di ricostruire le etichette dei nodi dell'albero ho utilizzato un algoritmo che ho ricavato dal SA e dal LCP in funzione di come ho costruito la struttura dell'albero, dalle prove effettuate l'algoritmo funziona, tuttavia non ho una dimostrazione formale che ne assicuri la correttezza in tutti i casi.

Nella costruzione delle etichette dell'albero il significato della **label** di ogni nodo non indica più l'etichetta tra un nodo e suo padre, ma indica l'etichetta del cammino dalla radice al nodo. L'algoritmo è il seguente:

Sia **n** un nodo dell'albero e siano **figli<sub>n</sub>** i figli di n, intesi come i figli diretti di n, che nel ST possono essere nodi intermedi. Sia **T** il testo su cui vengono creati SA e ST.

Se n è una foglia:
- label[n] = T[SA[n] : ]

Altrimenti al momento di unione tra due sottoalberi in n:
- label[n] = LCP(**figli<sub>n</sub>**) dove si intende la stringa del Longest Common Prefix e non il suo valore numerico
- Per ogni figlio **f** in **figli<sub>n</sub>** label[f] = label[f][LCP(**figli<sub>n</sub>**) : ] dove qui per LCP si intende il valore numerico. Ovvero per ogni etichetta del figlio viene sottratta dal suo inizio la stringa che viene attribuita al padre.

In questo modo le etichette vengono costruite ricorsivamente durante la costruzione del ST ed assumono il significato e il valore delle etichette di un ST.

Stampa dell'albero
---------------------
La stampa dell'albero viene fatta attraverso in DOT code, perchè tra i vari formati che ho trovato per la visualizzazione di un grafo era l'unico che mi permetteva la possibilità di assegnare a ogni ramo una etichetta. Per la visualizzare il grafo il sito che ho utilizzato è il seguente: http://www.webgraphviz.com/ .
