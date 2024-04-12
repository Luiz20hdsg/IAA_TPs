#include <iostream>
#include <list>
#include <queue> // fila para usar na BFS
#include <fstream>
#include <string>
#include <cstdio>
#include <algorithm>
#include <vector>

using namespace std;

typedef pair<float, int> pi;

class Grafo{

    int V; // número de vértices
    float custoTotal;
	list<int> *adj; // ponteiro para um array contendo as listas de adjacências
    vector<pair<int,float>> custos;
    vector<pair<int,float>> caminho;
    vector<pair<int, pair<int, int>>> coordenadas;

    public:

    Grafo(int V); // construtor
	void addAresta(int v1, int v2); // adiciona uma aresta no grafo
    void addCustos(int v1, float v2);
    void addCoordenadas(int v1, int v2, int v3);
    void adjacencylist();
    void displayAdjList();
    void printCustos();
    void printCoordenadas();
    void printCaminho();
    int find_vertice(int i, int j);
    

	// faz uma BFS a partir de um vértice
    //BFS
	int bfs(int vi, int vf);
    //IDS
    bool DLS(int src, int target, int limit);
    bool IDDFS(int src, int target, int max_depth);
    //UCS
    void UCS(int s, int t);
    //GREEDY
    int best_first_search(int actual_Src, int target, int n);

};

Grafo::Grafo(int V){

	this->V = V; // atribui o número de vértices
	adj = new list<int>[V]; // cria as listas
    this->custoTotal = 0;

}

void Grafo::addAresta(int v1, int v2){

	// adiciona vértice v2 à lista de vértices adjacentes de v1
	adj[v1].push_back(v2);
    adj[v2].push_back(v1);

}
void Grafo::addCustos(int v1, float v2){

    custos.push_back(make_pair(v1,v2));

}
void Grafo::addCoordenadas(int v1, int v2, int v3){

    coordenadas.push_back(make_pair(v1, make_pair(v2, v3)));

}

void Grafo::displayAdjList(){

     for(int i = 0; i<V; i++) {
      cout << i << "--->";
      list<int> :: iterator it;
      for(it = adj[i].begin(); it != adj[i].end(); ++it) {
         cout << *it << " ";
      }
      cout << endl;
   }
}

// Function to print adjacency list
void Grafo::adjacencylist(){

    for (int i = 0; i < V; i++) {
        cout << i << "->";
        for (int& x : adj[i]) {
            cout << x << " ";
        }
        cout << endl;
    }
}

void Grafo::printCustos(){

    int s = custos.size();
    for (int i=0; i<s; i++)
    {
        // The elements can be directly accessed
        // according to first or second element
        // of the pair.
        cout << custos[i].first << ", " << custos[i].second<<endl;
        
    }

    

}

void Grafo::printCoordenadas(){

    int size = coordenadas.size();
    for (int i=0; i<size; i++)
    {
        // The elements can be directly accessed
        // according to first or second element
        // of the pair.
        cout << coordenadas[i].first << ", " << coordenadas[i].second.first<< ", " << coordenadas[i].second.second << endl;
        

        
    }

}

void Grafo::printCaminho(){

    int s = caminho.size();
    for (int i=0; i<s; i++)
    {
        // The elements can be directly accessed
        // according to first or second element
        // of the pair.
        cout<<"("<<caminho[i].first<<", "<<caminho[i].second<<") ";
        
    }
    cout<<endl;

}

int Grafo::find_vertice(int i , int j){

    int s = coordenadas.size();

    for (int k=0; k<s; k++){
    
        if((coordenadas[k].second.first == i) && (coordenadas[k].second.second == j)){
            
            return coordenadas[k].first;
            
        }
            
           
    }
}
    


int Grafo::bfs(int vi, int vf){

	queue<int> fila;
	bool visitados[V]; // vetor de visitados

	for(int i = 0; i < V; i++)
		visitados[i] = false;

	//cout << "Visitando vertice " << vi << " ...\n";
	visitados[vi] = true; // marca como visitado
    custoTotal = custoTotal + custos[vi].second;
    int v1 = coordenadas[vi].second.first;
    int v2 = coordenadas[vi].second.second;
    caminho.push_back(make_pair(v1,v2));
    if(vi == vf){
        cout<<custoTotal<<" ";
        return 0;
    }
    

	while(true)
	{
		list<int>::iterator it;
		for(it = adj[vi].begin(); it != adj[vi].end(); it++)
		{
			if(!visitados[*it])
			{
				//cout << "Visitando vertice " << *it << " ...\n";
                custoTotal = custoTotal + custos[*it].second;
                int v1 = coordenadas[*it].second.first;
                int v2 = coordenadas[*it].second.second;
                caminho.push_back(make_pair(v1,v2));
				visitados[*it] = true; // marca como visitado
				fila.push(*it); // insere na fila
                if(*it == vf){
                    cout<<custoTotal<<" ";
                    return 0;
                }
			}
		}

		// verifica se a fila NÃO está vazia
		if(!fila.empty())
		{
			vi = fila.front(); // obtém o primeiro elemento
			fila.pop(); // remove da fila
		}
		else
			break;
	}
    //cout<<custoTotal<<" ";
}

bool Grafo::DLS(int src, int target, int limit){

    if (src == target){
        custoTotal = custoTotal + custos[src].second;
        int v1 = coordenadas[src].second.first;
        int v2 = coordenadas[src].second.second;
        caminho.push_back(make_pair(v1,v2));
        return true;
    }
        
   
    // If reached the maximum depth, stop recursing.
    if (limit <= 0)
        return false;
   
    // Recur for all the vertices adjacent to source vertex
    for (auto i = adj[src].begin(); i != adj[src].end(); ++i){
        custoTotal = custoTotal + custos[*i].second;
        int v1 = coordenadas[src].second.first;
        int v2 = coordenadas[src].second.second;
        caminho.push_back(make_pair(v1,v2));
       if (DLS(*i, target, limit-1) == true)
          return true;
    }

    return false;
}

bool Grafo::IDDFS(int src, int target, int max_depth){

    // Repeatedly depth-limit search till the
    // maximum depth.
    for (int i = 0; i <= max_depth; i++)
       if (DLS(src, target, i) == true){
        cout<<custoTotal<<" ";
        return true;
       }
   
    return false;
}

void Grafo::UCS(int actual_Src, int t){


    vector<bool> visited(V, false);
    priority_queue< pi, vector<pi>, greater<pi> > pq;
    priority_queue< pi, vector<pi>, greater<pi> > pq_custos;
    float aux = 0.0;
    float aux2 = 0.0;
    // sorting in pq gets done by first value of pair
    pq.push(make_pair(custos[actual_Src].first, actual_Src));
    custoTotal = custoTotal + custos[actual_Src].second;
    //int s = actual_Src;
    //visited[s] = true;

	//int current=0, pos=0;
	//typedef priority_queue<pQ,vector<pQ>,mycomparison> mypq_type;
	//mypq_type pq;

	//pQ vstart;

	//vstart.path.push_back(s);
	//vstart.cost = 0;

	//vector<int>::iterator i;
	//pq.push(vstart);
    //pair<int, int> x = pq.top();
	while(!pq.empty())
	{
		//pQ currentPQ, tempPQ;
		//current = pq.top().path.back();
		//currentPQ = pq.top();
        //current = pq.top()
        pair<float, int> x = pq.top();
        visited[x.second] = true;
        cout<<"vertice escolhido: "<<x.second<<endl;
        //custoTotal = custoTotal + custos[x.second].second;
        int v1 = coordenadas[x.second].second.first;
        int v2 = coordenadas[x.second].second.second;
        caminho.push_back(make_pair(v1,v2));
        pq.pop();
		// cout<<current<<" "<<currentPQ.cost<<endl;
		//pq.pop();
		if(x.second == t)
		{
			//cout<<"found"<<endl;
			//displayPath(currentPQ);
            //tempPQ.cost += custos[current].second;
            //cout<<" "<<tempPQ.cost<<endl;
			break;
		}
		else{
            list<int>::iterator it;
			for (it = adj[x.second].begin(); it != adj[x.second].end(); it++)
			{
                if (!visited[*it]){
                    aux = custoTotal + custos[*it].second;
                    cout<<"it: "<<*it<<endl;
                    cout<<"custo: "<<aux<<endl;
                    //int v1 = coordenadas[*it].second.first;
                    //int v2 = coordenadas[*it].second.second;
                    //caminho.push_back(make_pair(v1,v2));
                    pq.push(make_pair(aux, *it));
                    pq_custos.push(make_pair(aux,*it));
                    
                    

				    //tempPQ = currentPQ;
				    //tempPQ.path.push_back(*i);
                    //tempPQ.cost += custos[current].second;
				    //pq.push(tempPQ);
                }
                
			}
            cout<<"saiu"<<endl;
		}
	}
    pair<float, int> c = pq_custos.top();
    cout<<pq_custos.top().first<<" ";
   
    
}

int Grafo::best_first_search(int actual_Src, int target, int n){

    vector<bool> visited(n, false);
    priority_queue< pi, vector<pi>, greater<pi> > pq;
    // sorting in pq gets done by first value of pair
    pq.push(make_pair(custos[actual_Src].first, actual_Src));
    int s = actual_Src;
    visited[s] = true;
    while (!pq.empty()) {
        
        pair<int, int> x = pq.top();
        custoTotal = custoTotal + custos[x.second].second;
        int v1 = coordenadas[x.second].second.first;
        int v2 = coordenadas[x.second].second.second;
        caminho.push_back(make_pair(v1,v2));
        pq.pop();
        
        if (x.second == target){
            break;
        }
        
        list<int>::iterator it;
        for (it = adj[x.second].begin(); it != adj[x.second].end(); it++) {
            if (!visited[*it]) {
                visited[*it] = true;
                pq.push(make_pair(custos[*it].second,*it));
            }
        }
        
    }
    cout<<custoTotal<<" ";
}
 

int main(int argc, char* argv[]){

    int n = 0;
    int m = 0;
    int V = 0;
    char mychar;
    vector<int> v;
    int i = 0;
    int j = 0;
    int w = 0;

    if (argc != 7) {

        std::cout << "Formato de entrada invalido!" << std::endl;
        return 0;
    }

    ifstream myfile; 
    myfile.open(argv[1]);
   
    myfile >> m >> n;
    V = m*n;
    Grafo grafo(V);

    mychar = myfile.get();

    while(myfile){

        mychar = myfile.get();
        if(mychar=='\n'){

        }else{

            if(mychar=='@'){
                v.push_back(j);
                //grafo.custos.push_back(make_pair(j,10000.0));
                grafo.addCustos(j,100000.0);
            }
            if(mychar=='.')
                //grafo.custos.push_back(make_pair(j,1.0));
                grafo.addCustos(j,1.0);
            if(mychar==';')
                //grafo.custos.push_back(make_pair(j,1.5));
                grafo.addCustos(j,1.5);
            if(mychar=='+')
                //grafo.custos.push_back(make_pair(j,2.5));
                grafo.addCustos(j,2.5);
            if(mychar=='x')
                //grafo.custos.push_back(make_pair(j,6.0));
                grafo.addCustos(j,6.0);
           
            j++;
        }
    }
    

    myfile.clear();
    myfile.seekg(0);

    myfile >> m >> n;

    if(myfile.is_open()){

        mychar = myfile.get();
        

        while ( myfile ) {

            mychar = myfile.get();
            
            if(find(v.begin(), v.end(), i)!=v.end() || mychar=='\n'){
                
            }else{

                if(i+1<=V-1){

                    if(find(v.begin(), v.end(), i+1)!=v.end()){

                    }else{

                        grafo.addAresta(i, i+1);

                    }

                }

                if(i+m<=V-1){

                    if(find(v.begin(), v.end(), i+m)!=v.end()){

                    }else{

                        grafo.addAresta(i, i+m);

                    }

                    }
   
            }
                
            if( mychar!='\n'){
        
                i= i+1;
                

            }
        
        }

    }else{

        cout<<"Arquivo nao encontrado."<<endl;
        exit(EXIT_FAILURE);
    }


    for(int i=0;i<n;i++){
        for(int j=0;j<m;j++){
            //grafo.coordenadas.push_back(make_pair(w, make_pair(i, j)));
            grafo.addCoordenadas(w,i,j);
            w++;
        }
    }

	

    if ((string(argv[2])=="BFS") || (string(argv[2])=="bfs")) {
		
        long arg1 = strtol(argv[3], NULL, 10);
        long arg2 = strtol(argv[4], NULL, 10);
        long arg3 = strtol(argv[5], NULL, 10);
        long arg4 = strtol(argv[6], NULL, 10);
        int vi = grafo.find_vertice(arg1, arg2);
        int vf = grafo.find_vertice(arg3, arg4);
        grafo.bfs(vi, vf);
        grafo.printCaminho();

	}
    if ((string(argv[2])=="IDS") || (string(argv[2])=="ids")) {
		
        long arg1 = strtol(argv[3], NULL, 10);
        long arg2 = strtol(argv[4], NULL, 10);
        long arg3 = strtol(argv[5], NULL, 10);
        long arg4 = strtol(argv[6], NULL, 10);
        int vi = grafo.find_vertice(arg1, arg2);
        int vf = grafo.find_vertice(arg3, arg4);
        grafo.IDDFS(vi,vf,V);
        grafo.printCaminho();

	}
    if ((string(argv[2])=="UCS") || (string(argv[2])=="ucs")) {
		long arg1 = strtol(argv[3], NULL, 10);
        long arg2 = strtol(argv[4], NULL, 10);
        long arg3 = strtol(argv[5], NULL, 10);
        long arg4 = strtol(argv[6], NULL, 10);
        int vi = grafo.find_vertice(arg1, arg2);
        int vf = grafo.find_vertice(arg3, arg4);
        grafo.UCS(vi,vf);
        grafo.printCaminho();

	}
    if ((string(argv[2])=="Greedy") || (string(argv[2])=="greedy")) {
		long arg1 = strtol(argv[3], NULL, 10);
        long arg2 = strtol(argv[4], NULL, 10);
        long arg3 = strtol(argv[5], NULL, 10);
        long arg4 = strtol(argv[6], NULL, 10);
        int vi = grafo.find_vertice(arg1, arg2);
        int vf = grafo.find_vertice(arg3, arg4);
        grafo.best_first_search(vi,vf,V);
        grafo.printCaminho();

	}
    if ((string(argv[2])=="Astar") || (string(argv[2])=="astar")) {
		
        //grafo.bfs(1);
        grafo.printCaminho();

	}


    myfile.close();

    return 0;
    
}