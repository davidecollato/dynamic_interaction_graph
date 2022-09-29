#include "graph.h"
#include <QBrush>
#include <QtMath>
#include <QObject>
#include <QDebug>

//Costruttore
Graph::Graph(QWidget *parent) : QWidget(parent)
{
    //Fisso la size del widget
    this->setFixedSize(this->size());

    //Setto a false l'esistenza di tutti i nodi
    for(unsigned int i = 0; i < 30; ++i) {
        exists[i] = false;
    }

    //Riempio tutta la matrice delel adiacenze con zero
    for(unsigned int i = 0; i < 30; ++i) {
        for(unsigned int j = 0; j < 30; ++j) {
            adjacency_matrix[i][j] = 0;
        }
    }

    //Ho zero nodi inizializzati
    n_nodes = 0;

    //Inizializzo i Qpoint a (0,0)
    for(unsigned int i = 0; i < 30; ++i) {
        nodes[i] = QPoint(0,0);
    }
}

//Metodo che ritorna il numero di nodi inizializzati
unsigned int Graph::number_of_nodes() const
{
    return n_nodes;
}

//Metodo che esegue l'iterazione al tempo T0 : creo un numero randomico di nodi (tra 1 e 8 per non avere
//nè il grafico vuoto (a T0 non è contemplato come stato) nè avere il grafico subito pieno (30 nodi)
void Graph::initialize()
{
    srand(time(NULL));
    int n_nodi = rand()%(7) + 1;

    for(int i = 0; i < n_nodi; ++i) {
        exists[i] = true;
    }

    n_nodes = n_nodi;
}

//Metodo che eseguo l'n-esima iterazione : crea un numero casuale di nodi, crea un numero casuale di archi
//tra i nodi inizializzati (tra 0 e 5 per non avere il grafo eccessivamente affollato) e in seguito aumenta
//di una unità il numero di interazioni tra due nodi (se questi due nodi non avevano interazioni, cioè
//il numero di interazioni era zero, cioè non erano collegati, allora tra questi viene creato un arco)
void Graph::iteration()
{
    if(number_of_nodes() == 0) {
        initialize();
        return;
    }
    if(number_of_nodes() >= 30) {
        return;
    }

    srand(time(NULL));

    unsigned int remained_nodes = 30 - number_of_nodes();
    unsigned int n_nodi = rand()%(remained_nodes + 1);
    n_nodes = number_of_nodes() + n_nodi;
    for(unsigned int i = 0; i < number_of_nodes(); ++i) {
        exists[i] = true;
    }

    int n1, n2;
    //int n_archi = rand()%(number_of_nodes() + 1); //completa libertà sul numero di nodi da creare
    int n_archi = rand()%(5 + 1); //per diminuire il numero di archi (se no sono troppi)
    for(int i = 0; i < n_archi; ++i) {
        n1 = rand()%(number_of_nodes() + 1);
        n2 = rand()%(number_of_nodes() + 1);
        //Scelgo due nodi random tra quelli disponibili
        if((adjacency_matrix[n1][n2] == 0) && (n1 != n2)) {
            //Creo l'arco tra i due nodi
            adjacency_matrix[n1][n2] = 1;
            adjacency_matrix[n2][n1] = 1;
        }
    }
    //int n_iter = rand()%(number_of_nodes() + 1);
    int n_iter = rand()%(5 + 1);
    //Tramite questa variabile booleana mi assicuro che qualcosa venga modificato
    bool is_not_modified = true;
    for(int i = 0; i < n_iter; ++i) {
        n1 = rand()%(number_of_nodes() + 1);
        n2 = rand()%(number_of_nodes() + 1);

        if(n1 != n2) { //anche la creazione di nuovi archi è contemplata
            adjacency_matrix[n1][n2]++;
            adjacency_matrix[n2][n1]++;
            is_not_modified = false;
        }
    }
    if(is_not_modified) { //se nulla è stato modificato nel for precedente
        for(unsigned int i = 0; i < 30 && is_not_modified; ++i) {
            for(unsigned int j = 0; j < 30 && is_not_modified; ++j) {
                if(i != j) { //anche la creazione di nuovi archi è contemplata
                    adjacency_matrix[i][j]++;
                    adjacency_matrix[j][i]++;
                    is_not_modified = false;
                }
            }
        }
    }
}

//Metodo che, presi in input due interi, crea un QPoint con questi due interi e controlla
//1) che tale QPoint non sia già presente nell'array che contiene contiene tutti i punti inizializzati
//(e quindi che i nodi non possano sovrapporsi)
//2) che i centri dei due punti siano sufficientemente distanti (per evitare che i nodi si intersechino)
bool Graph::is_occupied(int x, int y)
{
    float distance = 0;
    QPoint p = QPoint(x, y);
    for(unsigned int i = 0; i < 30; ++i) {
        if(nodes[i] == p) {
            return true;
        }
        distance = sqrt((x-nodes[i].x())*(x-nodes[i].x()) + (y-nodes[i].y())*(y-nodes[i].y()));
        if(distance <= 10*5) {
            return true;
        }
    }

    return false;
}

//Metodo che implementa il delay di un secondo tra un'istruzione e l'altra
void Graph::delay()
{
    QTime dieTime = QTime::currentTime().addSecs(1);
    while(QTime::currentTime() < dieTime) {
        QCoreApplication::processEvents(QEventLoop::AllEvents, 1000);
    }
}

//Metodo che implementa il gradiente, dipendentemente dal valore v/d(si va da v/d = 0 che è blu fino a
// v/d = 1 che è circa arancione)
QColor Graph::colorize(int v, int d)
{
    if(v > d) {
        v = v % d;
    }
    qreal rv = qreal(v) / d;
    QColor color = QColor::fromHsl(205 - (205 - 42) * rv, 200, 135);
    return color;
}

//Override del paintEvent, al cui interno si eseguono le istruzioni
void Graph::paintEvent(QPaintEvent *)
{
    if(number_of_nodes() >= 30) {
        return;
    }
    if(number_of_nodes() == 0){
        initialize();
        paint();
    } else {
        delay();
        iteration();
        paint();

    }
}

//Metodo paint, che si occupa di disegnare i nodi, gli archi e il contatore delle interazioni sugli archi
void Graph::paint()
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);

    QColor colorIteration;

    //Aggiorno il valore sulla diagonale (numero massimo di interazioni)
    for(unsigned int i = 0; i < number_of_nodes(); ++i) {
        for(unsigned int j = 0; j < number_of_nodes(); ++j) {
            if(i > j || i == j) {
                ;
            } else {
                if(adjacency_matrix[i][j] != 0) {
                    if(adjacency_matrix[i][i] < adjacency_matrix[i][j]){
                        adjacency_matrix[i][i] = adjacency_matrix[i][j];
                    }
                    if(adjacency_matrix[j][j] < adjacency_matrix[i][j]){
                        adjacency_matrix[j][j] = adjacency_matrix[i][j];
                    }
                }
            }
        }
    }

    srand(time(NULL));

    //Creo e disegno i nodi
    QPoint p;
    for(unsigned int i = 0; i < number_of_nodes(); ++i) {
        if(nodes[i] == QPoint(0,0)) {
            do {
                int x = rand()%(600) + 20;
                int y = rand()%(440) + 20;
                p = QPoint(x, y);
            } while(is_occupied(p.x(), p.y()));
            nodes[i] = p;
        }
        painter.setPen(QPen(Qt::black));
        colorIteration = colorize(adjacency_matrix[i][i], 10);
        painter.setBrush(QBrush(colorIteration));
        painter.drawEllipse(nodes[i], 5, 5);
    }

    int n_inter;
    QString n_inter_str;
    QPoint half_point;
    QString inter_txt;

    for(unsigned int i = 0; i < 30; ++i) {
        for(unsigned int j = 0; j < 30; ++j) {
            if(i > j || i == j) {
                ;
            } else {
                if((adjacency_matrix[i][j] != 0)) {
                    if((nodes[i] != QPoint(0,0)) && (nodes[j] != QPoint(0,0))) {
                        painter.setPen(QPen(Qt::black, 0.5));
                        painter.drawLine(nodes[i], nodes[j]);

                        half_point = QPoint((nodes[i].x()+nodes[j].x())/2, (nodes[i].y()+nodes[j].y())/2);
                        n_inter = static_cast<int>(adjacency_matrix[i][j]);
                        inter_txt = QString('+').append(n_inter_str.setNum(n_inter));
                        painter.drawText(half_point, inter_txt);
                    }
                }
            }
        }
    }
    if(number_of_nodes() < 30)
        update();
}
