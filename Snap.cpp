/*
Author: Sebastian Vives Faus
Date: 10/10/2019
Description: A program that implements the Snap library to create a graph that uses all the algorithms asked for in the homework. 
Note: A=1, B=2, C=3,..., N=14
*/

#include "Snap.h"
#include <iostream>
#include <chrono>
#include <queue>
#include <stack>
#include <algorithm>
#include <list>

using namespace std;
using namespace std::chrono;

#define N 23//Numero de vertices
#define M 14//Numero de nodos

struct EdgeWeight{
    int ID;
    int weight;
};


// Print graph statistics
void PrintGStats(const char s[], PNEGraph Graph) {
  printf(" %s nodos %d, edges %d, esta vacia? = %s\n",
      s, Graph->GetNodes(), Graph->GetEdges(),
      Graph->Empty() ? "Si" : "No");
}

//Insertar nodos en la grafica (en este caso, seria del 1-14)
void InsertarNodos(PNEGraph Graph) {
    //poner reloj start
  for(int i = 0; i<M; i++){
      auto start = high_resolution_clock::now();
      Graph -> AddNode(i+1);
      auto stop = high_resolution_clock::now();
      cout<<"Added Node: ("<<i+1<<")"<<endl;
      auto duration = duration_cast<microseconds>(stop - start);
        cout << "Duracion para insertar nodo: "<<duration.count() << endl;
  }
  //poner reloj stop

  PrintGStats("Datos de la grafica: ",Graph);
}

//Herramienta para implementar aristas en los nodos indicados:
void InsertarAristasTool(PNEGraph Graph, EdgeWeight VP[], int origen, int destino, int peso){
    int id;
    auto start = high_resolution_clock::now();
    Graph -> AddEdge(origen,destino,-1);
    auto stop = high_resolution_clock::now();
    id = Graph ->GetEId(origen,destino);
    VP[id].ID = id;
    VP[id].weight = peso;
    cout<<"Added edge between "<<origen<<"-"<<destino<<" with ID: "<<VP[id].ID<<" and weight of: "<<VP[id].weight<<endl;
    auto duration = duration_cast<microseconds>(stop - start);
    cout << "Duracion para insertar arista: "<<duration.count() << endl;
}

//Insertar aristas (23) en la grafica (en este caso, seria del 1-14), al igual que su peso.
void InsertarAristas(PNEGraph Graph, EdgeWeight VP[]){
    //En base al diagrama de la presentacion:

    //Poner reloj start
    InsertarAristasTool(Graph,VP,1,4,8);
    InsertarAristasTool(Graph,VP,1,3,8);
    InsertarAristasTool(Graph,VP,4,8,2);
    InsertarAristasTool(Graph,VP,4,7,3);
    InsertarAristasTool(Graph,VP,7,4,6);
    InsertarAristasTool(Graph,VP,8,7,3);
    InsertarAristasTool(Graph,VP,8,9,3);
    InsertarAristasTool(Graph,VP,9,12,4);
    InsertarAristasTool(Graph,VP,9,10,2);
    InsertarAristasTool(Graph,VP,12,9,2);
    InsertarAristasTool(Graph,VP,12,11,8);
    InsertarAristasTool(Graph,VP,11,12,6);
    InsertarAristasTool(Graph,VP,12,14,9);
    InsertarAristasTool(Graph,VP,14,13,2);
    InsertarAristasTool(Graph,VP,13,14,6);
    InsertarAristasTool(Graph,VP,3,10,4);
    InsertarAristasTool(Graph,VP,10,3,10);
    InsertarAristasTool(Graph,VP,3,2,7);
    InsertarAristasTool(Graph,VP,3,5,8);
    InsertarAristasTool(Graph,VP,2,5,7);
    InsertarAristasTool(Graph,VP,4,5,1);
    InsertarAristasTool(Graph,VP,5,6,9);
    InsertarAristasTool(Graph,VP,6,13,4);
    //Poner reloj stop

    PrintGStats("Datos de la grafica: ",Graph);
}
//****************************************************************************************************************************************************************************************************

void PrimA(PNEGraph Graph, EdgeWeight VP[]){
    auto start = high_resolution_clock::now();
    int counter = 0, id, min, destino;
    int nodos [2][M];
    int weights [N];
    //Arreglo para los nodos y sus pesos
    for(int i =0; i<M; i++){
        nodos[0][i] = i+1;
    }

    for(int x = 0; x<M; x++){
        destino = x+1;
        counter=0;
        for(int i = 0; i<M; i++){
            if(Graph -> IsEdge(i+1, destino, true) == 1){//Si existe un vertice entre estos dos nodos:
                id = Graph ->GetEId(i+1,destino);
                //cout<<"There is and edge between "<<i+1<<" and "<<destino<<", which has an ID of "<<id<<" and a weight of "<<VP[id].weight<<endl;
                weights[counter] = VP[id].weight;//Guardar el peso del vertice
                counter++;
            }//end if
        }//end for

                    //Comparar cual vertice tiene menor peso
                min = weights[0];
                for(int k = 0; k<counter;k++){
                    if(weights[k]<min){
                        min = weights[k];
                    }//end if2
                }//end for2

                //cout<<"The smallest weight for "<<destino<<" is "<<min<<endl;
                nodos[1][x] = min;
    }//for principal
    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(stop - start);
    cout << "Duracion para hacer Algoritmo de Prim: "<<duration.count() << endl;
    //Imprimir tabla de PRIM
    nodos[1][0] = NULL;
    cout<<"Prim Algorithm Solution: "<<endl;
    for(int i = 0; i<M; i++){
        cout<<nodos[0][i]<<"\t";
    }
    cout<<endl;
    for(int i = 0; i<M; i++){
        cout<<nodos[1][i]<<"\t";
    }
    cout<<endl;
}//End PrimA


//****************************************************************************************************************************************************************************************************
void printQueue(queue <int> que){
    queue <int> q = que;
    while (!q.empty())
    {
        cout << '\t' << q.front();
        q.pop();
    }
}

void BFS(PNEGraph Graph, int source){
    auto start = high_resolution_clock::now();
    //Crear mi cola y mi arreglo de visitados
    bool visited [M];
    queue <int> que;
    int bfs[M];
    int front, counter = 0;

    //Marcar a todos como no-visitados
    for(int i = 0; i < M; i++){
        visited [i] = false;
    }

    //Manipular el origen
    visited[source-1] = true;
    que.push(source);

    while(!que.empty()){//Mientras que la cola tenga elementos...
        //Imprimir cola
        /*
        cout<<"Q {";
        printQueue(que);
        cout<<" }"<<endl;
        */
        front = que.front();
        bfs[counter] = front;
        counter++;

        //Ver quien esta conectado
        for(int i = 0; i<M; i++){
            if((Graph -> IsEdge(front, i+1, true) == 1) && visited[i]== false){//Si existe un vertice entre estos dos nodos y el nodo aun no esta visitiado
                //cout<<"There is and edge between "<<front<<" and "<<i+1<<endl;
                visited[i] = true;
                que.push(i+1);
            }//end if
        }//end for

        que.pop();
    }
    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(stop - start);
    cout << "Duracion para hacer Algoritmo de BFS: "<<duration.count() << endl;
    cout<<"BFS :"<<"\t";
    for(int i = 0; i < M; i++){
        cout<<bfs[i]<<" ";
    }
}
//****************************************************************************************************************************************************************************************************
void printMN(int A[][M]){
    for(int i = 0; i<M; i++){
            for(int j = 0; j<M; j++){
                if(A[i][j] > 9999){
                   cout<<0<<"\t";
                   //cout<<A[i][j]<<"\t";
                }
                else{
                cout<<A[i][j]<<"\t";
                }
            }//for
            cout<<endl;
        }//for
}
void FloydWarshall(PNEGraph Graph, EdgeWeight VP[]){
    auto start = high_resolution_clock::now();
    int id, temp;
    //Crear mi matriz A
    int A[M][M];
    //Rellenar mi matriz con las aristas
    for(int i = 0; i<M; i++){
        for(int j = 0; j<M; j++){
            if(Graph -> IsEdge(i+1, j+1, true) == 1){
                 id = Graph ->GetEId(i+1,j+1);
                 A[i][j] = VP[id].weight;
            }//if
            else{
                A[i][j] = 9999;
            }
        }//for
    }//for

    for(int i = 0; i<M; i++){
        A[i][i] = 0;
    }

    //Crear mi matriz C
    int C[M][M];

    //Copiar la matriz A en C
    for(int i = 0; i<M; i++){
        for(int j = 0; j<M; j++){
            C[i][j] = A[i][j];
        }//for
    }//for
    /*
    cout<<"Matrix A"<<endl;
    printMN(C);
    */
    //Arreglar matriz C
    for(int k = 0; k<M; k++){
        for(int i = 0; i<M; i++){
            for(int j = 0; j<M; j++){
                temp = C[i][k] + C[k][j];
                if( C[i][j] > temp){
                    C[i][j] = temp;
                }
            }
        }//for
    }//for
    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(stop - start);
    cout << "Duracion para hacer Algoritmo de Floyd Warshall: "<<duration.count() << endl;
    cout<<"Matrix C"<<endl;
    printMN(C);
}

//****************************************************************************************************************************************************************************************************
void printStack(stack<int> ss){
    stack<int> s = ss;
    int x;
    //cout<<" Stack: |";
    while(!s.empty()){
        x = s.top();
        cout<<" "<<x<<" ";
        s.pop();
    }
    //cout<<"|";
}

void DFS( PNEGraph Graph, int source){
    auto start = high_resolution_clock::now();
    bool visited[M];
    stack<int> stack;
    int s, counter=0; int dfs[M];;
    //Ponemos que los nodos no han sido visitados
    for(int i=0; i<M; i++){
        visited[0] = false;
    }
    //Metemos el origen en el stack.
    stack.push(source);

    printStack(stack);
    //DFS
    while(!stack.empty()){//Mientras haya algo dentro del stack
        //printStack(stack);
        //cout<<"\n";
        //Agarramos el elemento hasta arriba del stack
        s = stack.top();
        stack.pop();//Y lo sacamos del stack.

        if(visited[s-1] == false){//Si no hemos visitado el nodo, lo marcamos como visitado.
            visited[s-1] = true;
            dfs[counter] = s;
            counter++;
        }//if

        for(int i = 0; i<M; i++){//Ver que nodos estan conectados a este
            if((Graph -> IsEdge(s, i+1, true) == 1) && visited[i]== false){//Si existe un vertice entre estos dos nodos y el nodo aun no esta visitiado
               // cout<<"There is and edge between "<<s<<" and "<<i+1<<endl;
                stack.push(i+1);
            }//end if
        }//end for


    }

    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(stop - start);
    cout << "Duracion para hacer Algoritmo de DFS: "<<duration.count() << endl;
    cout<<endl;
    cout<<"DFS :"<<"\t";
    for(int i = 0; i < M; i++){
        cout<<dfs[i]<<" ";
    }
    cout<<endl;
}

//****************************************************************************************************************************************************************************************************

struct subset{
    int parent;
    int rank;
};

bool compare(EdgeWeight a, EdgeWeight b){
    return a.weight < b.weight;
}

int GetSrc(int id, PNEGraph Graph){
    int x;
    for (TNEGraph::TEdgeI EI = Graph->BegEI(); EI < Graph->EndEI(); EI++) {
        if( EI.GetId() == id){
            x = EI.GetSrcNId();
        }
    }
    return x;
}

int GetDrc(int id, PNEGraph Graph){
    int x;
    for (TNEGraph::TEdgeI EI = Graph->BegEI(); EI < Graph->EndEI(); EI++) {
        if( EI.GetId() == id){
           x = EI.GetDstNId();
        }
    }
    return x;
}

int find(subset subsets[], int i){
     // If i is the parent of itself
    // find root and make root as parent of i
    // (path compression)
    if (subsets[i].parent != i)
        subsets[i].parent = find(subsets, subsets[i].parent);

    return subsets[i].parent;
}

void unir(subset subsets[], int x, int y)
{
    int xroot = find(subsets, x);
    int yroot = find(subsets, y);

    // Attach smaller rank tree under root of high
    // rank tree (Union by Rank)
    if (subsets[xroot].rank < subsets[yroot].rank)
        subsets[xroot].parent = yroot;
    else if (subsets[xroot].rank > subsets[yroot].rank)
        subsets[yroot].parent = xroot;

    // If ranks are same, then make one as root and
    // increment its rank by one
    else
    {
        subsets[yroot].parent = xroot;
        subsets[xroot].rank++;
    }
}

void Kruskal(PNEGraph Graph, EdgeWeight VP[]){
    //Con ayuda de https://www.geeksforgeeks.org/kruskals-minimum-spanning-tree-algorithm-greedy-algo-2/
    auto start = high_resolution_clock::now();
    list<int> T;
    subset *subsets = new subset[N];
    int x, y, src, drc;



    // Create V subsets with single elements
    for (int i = 0; i < N; i++)
    {
        subsets[i].parent = i;
        subsets[i].rank = 0;
    }

    //Organizar el peso de las aristas
        EdgeWeight VerticeOrd[N];
        for(int i =0; i<N; i++){
            VerticeOrd[i].ID = VP[i].ID;
            VerticeOrd[i].weight = VP[i].weight;
        }

        sort(VerticeOrd, VerticeOrd+N, compare);
        /*cout<<"Vertices Ordenados: \n ID \t Peso \t Origen \t Destino"<<endl;
        for(int i =0; i<N; i++){
            cout<<VerticeOrd[i].ID<<"\t"<<VerticeOrd[i].weight<<"\t"<<GetSrc(VerticeOrd[i].ID, Graph)<<"\t - \t"<<GetDrc(VerticeOrd[i].ID, Graph)<<endl;
        }*/

    //Kruskal

    for(int i =0; i<N; i++){
        src = GetSrc(VerticeOrd[i].ID, Graph); //cout<<"SRC = "<<src;
        drc = GetDrc(VerticeOrd[i].ID, Graph); //cout<<"\t DRC = "<<drc;
        //cout<<endl;
        x = find(subsets, src); //cout<<"X = "<<x;
        y = find(subsets, drc); //cout<<"\t Y = "<<y;
        //cout<<endl;

        if(x != y){
            T.push_back(VerticeOrd[i].ID); //cout<<"Elemento insertado a T "<<VerticeOrd[i].ID<<endl;
            unir(subsets, x, y);
        }
        else{
            //cout<<"Elemento no insertado"<<endl;
        }
    }//end for

    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(stop - start);
    cout << "Duracion para hacer Algoritmo de Kruskal: "<<duration.count() << endl;

    //Print T
    cout<<"Arbol T: \n ID \t Origen \t Destino"<<endl;
        while(!T.empty()){
        x = T.front();
        cout<<x<<"\t"<<GetSrc(x, Graph)<<"\t - \t"<<GetDrc(x, Graph)<<endl;
        T.pop_front();
    }


}

//****************************************************************************************************************************************************************************************************

int MinDistancia(int *distancia, bool *visitado){
    int min = 9999;
    int index;

    for(int i = 0; i<M; i++){
        if(visitado[i] == false && distancia[i] <= min){
            min = distancia[i];
            index = i;
        }
    }
    return index; //Posicion en las tablas

}

void Dijkstra(PNEGraph Graph, EdgeWeight VP[], int source){
    auto start = high_resolution_clock::now();
    //Declarar los otros dos arreglos
    int u = source-1;
    int distancia[M];
    bool visitado[M];

    for(int i =0; i<M; i++){//Rellenar ambas tablas con infinito y falso
        distancia[i] = 9999;
        visitado[i] = false;
    }
    //Iniciar la distancia del origen a 0.
    distancia[source-1] = 0;


    for(int i = 0; i<M; i++){

        /*
        for(int i = 0; i<M; i++){
            cout<<distancia[i]<<"\t";
        }
        cout<<endl;
        for(int i = 0; i<M; i++){
            cout<<visitado[i]<<"\t";
        }
        cout<<endl;
        */


        for(int v=0; v<M;v++){
		//Update dist[v] if not in Dset and their is a path from src to v through u that has distance minimum than current value of dist[v]
        if(Graph -> IsEdge(u+1, v+1, true) == 1){
            //cout<<"Origen: "<<u+1<<"\t Destino: "<<v+1<<endl;
        int id = Graph ->GetEId(u+1,v+1); //cout<<"ID : "<<id<<endl;
        int x = VP[id].weight; //cout<<"Peso : "<<x<<endl;
        //cout<<v+1<<" Visitado? : "<<visitado[v]<<"\t"<<distancia[u]<<" no es 9999 \t U"<<distancia[u]<<"+ X "<<x<<" < "<< distancia[v]<<endl;
			if(!visitado[v] && distancia[u]!=9999 && distancia[u]+x<distancia[v]){
			distancia[v]=distancia[u]+x;

		    }
        }//if
    }//for
    u = MinDistancia(distancia, visitado); //cout<<"U : "<<u+1<<endl;
        visitado[u] = true;
  }//for

    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(stop - start);
    cout << "Duracion para hacer Algoritmo de Dijkstra: "<<duration.count() << endl;

  //Imprimir
  cout<<"Vertex\t\tDistance from source"<<endl;
	for(int i=0;i<M;i++)                       /*will print the vertex with their distance from the source to the console */
	{
		cout<<i+1<<"\t\t"<<distancia[i]<<endl;
	}
}

void DeleteNode(int id,PNEGraph Graph){
    auto start = high_resolution_clock::now();
    Graph -> DelNode(id);
    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(stop - start);
    cout << "Duracion para eliminar nodo: "<<id<<" = "<<duration.count() << endl;
    PrintGStats("Datos de la grafica: ",Graph);
}

void DeleteEdge(int id, PNEGraph Graph){
    auto start = high_resolution_clock::now();
    Graph -> DelEdge(id);
    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(stop - start);
    cout << "Duracion para eliminar la arista con id "<<id<<" = "<<duration.count() << endl;
    PrintGStats("Datos de la grafica: ",Graph);
}


int main(){
    //Constructor de la grafica
    //Crear la grafica
    PNEGraph Graph;
    EdgeWeight VP[N];
    Graph = TNEGraph::New();
    PrintGStats("Datos de la grafica: ",Graph);
    InsertarNodos(Graph);
    InsertarAristas(Graph, VP);

    PrimA(Graph, VP);
    cout<<"BFS \n"<<endl;
    BFS(Graph, 1);
    cout<<"\n"<< "Floyd-Warshall \n"<<endl;
    FloydWarshall(Graph, VP);
    cout<<"DFS \n"<<endl;
    DFS(Graph, 1);
    cout<<"Kruskal \n"<<endl;
    Kruskal(Graph, VP);
    cout<<"Dijkstra \n"<<endl;
    Dijkstra(Graph, VP, 1);

    PrintGStats("Datos de la grafica: ",Graph);
    DeleteEdge(1, Graph);
    DeleteNode(3, Graph);

    return 0;
}
