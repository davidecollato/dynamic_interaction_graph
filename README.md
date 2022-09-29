# Dynamic Interaction Graph

Questa repository contiene il progetto del corso di Programmazione C++ (Unimib) dell'appello del 26 Settembre 2022, in particolare la parte da svolgere con le librerie grafiche Qt.
La traccia del progetto è contenuta nel file Esame-220926.pdf.

## Descrizione
Nel testo d'esame viene chiesto di implementare un dynamic interaction graph, un modello che viene utilizzato per la descrizione di interazioni sociali. È possibile ad esempio identificare ogni nodo con una persona e ogni arco con una o più interazioni tra due persone.
Al tempo 0 viene inizializzato (disegnato e colorato) un numero casuale di nodi.
Con la prima iterazione vengono creati un numero casuale di nodi (anche il caso nessun nodo è contemplato) e un numero casuale di archi (anche il caso nessun arco è contemplato) e si modifica, ove necessario, il colore dei nodi. In seguito viene aumentato di una unità il contatore delle interazioni tra due nodi (se tra i due nodi vi è già un arco, allora passo da 1 interazione a 2 interazioni, se tra due nodi non è già presente un arco, allora ne creo uno) e modifico, ove necessario, il colore dei nodi.
Le istruzioni precedenti si ripetono sino al raggiungimento di 30 nodi, a questo punto il programma termina ma la finestra con il grafo rimane comunque visibile.
per le scelte implementative si rimanda ai commenti fatti nel codice.


## Valutazione progetto 
Il progetto ha ricevuto  una penalizzazione in quanto non è stata implementata una colorbar che esplicitasse il gradiente utilizzato per la colorazione dei nodi.

 - Codice Qt: Manca una colorbar con cui interpretare il colore dei nodi al variaredella cardinalità di edge (-1).
