#ifndef GRAPH_H
#define GRAPH_H

#include <QWidget>
#include <QTime>
#include <QPainter>
#include <QPoint>
#include <QCoreApplication>

class Graph : public QWidget
{
    //Array di booleani, se exists[i] = true, allora il nodo i-esimo è stato inizializzato
    bool exists[30];
    //Matrice delle adiacenze, è simmetrica. Se adjacency_matrix[i][j] != 0 allora tra il nodo i-esimo e il
    //nodo j-esimo c'è un arco. Nella diagonale (che non viene usata perché consideriamo ogni altro nodo
    //banalmente collegato con se stesso) viene salvato il valore più alto di interazioni posto di default
    //a 1 e poi incrementato in maniera casuale (come descritto nel punto c)
    unsigned int adjacency_matrix[30][30];
    //Numero dei nodi inizializzati
    unsigned int n_nodes;
    //Array di QPoint che contiene i nodi inizializzati
    QPoint nodes[30];
    Q_OBJECT

public:
    explicit Graph(QWidget *parent = nullptr);

    ~Graph(){}

    void iteration();

    unsigned int number_of_nodes() const;

private:

    void paintEvent(QPaintEvent *);

public:

    void paint();

private:

    void initialize();

    bool is_occupied(int x, int y);

    void delay();

    QColor colorize(int v, int d);

};

#endif // GRAPH_H
