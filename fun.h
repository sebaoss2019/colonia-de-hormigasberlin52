#include <bits/stdc++.h>
#include <cstdio>
#include <ctime>

using namespace std;

typedef pair < float, float> ff;
typedef vector <int> vi;
typedef vector <float> vf;
typedef vector <vf> vvf;
typedef vector <vi> vvi;
typedef vector <ff> vff;

float costo, costo_inicial;
vff puntos;
vvf distancias;
vvf heuristica;
vvf feromona;
vvf refuerzo;

float distancia(float x1, float x2, float y1, float y2)
{
	return sqrt((x2-x1)*(x2-x1) + (y2-y1)*(y2-y1));
}

void Leer_archivo()
{
	int nodo,it=0;
	float x,y,dist;
	char cadena[20];
	ff par; 
	//abriendo archivo de texto
	FILE *f;
	f = fopen("entrada.txt","r");
	while(feof(f) == 0)					//Lectura del archivo
	{
		fgets(cadena,20,f);
		if (cadena[0] >= '0' && cadena[0] <= '9')
		{		
			sscanf(cadena,"%d %f %f",&nodo,&x,&y);
			par = make_pair(x,y);
			puntos.push_back(par);
			it++;
		}
	}

	//Calcula y almacena la matriz de heuristica
	heuristica.resize(puntos.size()+1);
	distancias.resize(puntos.size()+1);
	for (int i = 1; i < heuristica.size(); ++i)
	{
		distancias[i].resize(puntos.size()+1,0);
		heuristica[i].resize(puntos.size()+1,0);
	}

	for (int i = 0; i < puntos.size(); ++i)
	{
		for (int j = i; j < puntos.size(); ++j)
		{
			dist = distancia(puntos[i].first,puntos[j].first,puntos[i].second,puntos[j].second);
			heuristica[i+1][j+1] = 1/dist;
			if (i!=j)
			{
				heuristica[j+1][i+1] = 1/dist;
			}
			distancias[i+1][j+1] = dist;
			if (i!=j)
			{
				distancias[j+1][i+1] = dist;
			}
		}
	}

	fclose(f);
}

vi iniciar_feromona()
{
	int num, pos;
	float d;

	vi v,recorrido,visitado;
	for (int i = 0; i < puntos.size(); ++i)
	{
		v.push_back(i+1);
	}
	
	num = rand() % v.size();
	recorrido.push_back(v[num]);
	visitado.resize(distancias.size(),0);
	visitado[recorrido[0]] = 1;
	costo = 0;
	
	for (int i = 0; i < distancias.size()-1; ++i)
	{
		d = 10000000.0;
		for (int j = 1; j < distancias[recorrido[i]].size(); ++j)
		{
			if(distancias[recorrido[i]][j] < d && recorrido[i] != j && visitado[j] == 0)
			{
				d = distancias[recorrido[i]][j];
				pos = j;
			}
		}
		recorrido.push_back(pos);
		visitado[pos] = 1;
	}
	
	for (int i = 0; i < recorrido.size()-1; ++i)
	{
		costo += distancias[recorrido[i]][recorrido[i+1]];
	}

	costo += distancias[recorrido[recorrido.size()-1]][recorrido[0]]; 
	
	//Valor inicial de feromona despues de la primera solucion
	feromona.resize(puntos.size()+1);
	for (int i = 0; i < feromona.size(); ++i)
	{
		feromona[i].resize(puntos.size()+1,1/costo);
		feromona[i][i] = 0;
	}

	//Matriz de refuerzo de la feromona
	refuerzo.resize(puntos.size()+1);
	for (int i = 0; i < refuerzo.size(); ++i)
	{
		refuerzo[i].resize(puntos.size()+1,1/costo);
		refuerzo[i][i] = 0;
	}
	costo_inicial = costo;
	cout<<"Costo inicial: "<<costo_inicial<<endl;
	return recorrido;
}

vi asignar_hormigas(int n_hormigas)
{
	int num;
	vi v,h;
	for (int i = 0; i < puntos.size(); ++i)
	{
		v.push_back(i+1);
	}
	//Asignar hormigas aleatoriamente a una ciudad sin repetir
	for(int i = 0; i < n_hormigas; i++){
		num = rand() % v.size();
		h.push_back(v[num]);
		v.erase(v.begin()+num);
	}
	return h;
} 

int seleccion_ruleta(int nodo, vi &v, int coef)
{
	vf ruleta;
	float value,num,prob,total = 0;
	int pos;

	num = 1+rand()%100;
	prob = num+rand()%((int)num+50);
	prob = num/prob;

	ruleta.resize(v.size(),0);
	for (int i = 1; i < v.size(); ++i)
	{
		if (v[i] == 0) 
		{
			value = refuerzo[nodo][i]*pow(heuristica[nodo][i], coef);
			ruleta[i] = value;
			total+=value;
		}
	}

	//Ruleta
	for (int i = 1; i < ruleta.size(); ++i)
	{
		ruleta[i] = (ruleta[i] / total) + ruleta[i-1];
	}
	//selecciOn del valor segUn la ruleta
	for (int i = 0; i < ruleta.size(); ++i)
	{
		if (prob > ruleta[i])
		{
			pos = i;
		}
	}

	if (pos == ruleta.size()-1)
	{
		
		return pos;
	}
	else{
		
		return pos+1;
	}
}

//Una hormiga va desde la ciudad "nodo" hasta otra ciudad.
int trancision(int nodo, vi &visit, float prob_limite, float coeficiente,int it)
{
	int J, pos;
	float prob, num, max = -1.0, value; 
	num = 1+rand()%100;
	prob = num+rand()%((int)num+50);
	prob = num/prob;

	//seleccion de la proxima ciudad a visitar segun la probabilidad limite q0.
	if (prob <= prob_limite) 
	{
		//printf("Nodo escogido por maximizacion de heuristica ");
		if (it == 10000)
		{
			//cout<<"Nodo escogido por maximizacion de heuristica ";
		}
		for (int i = 1; i < heuristica[nodo].size(); ++i)
		{
			//si la ciudad es distinta de si misma y no ha sido visitada por esta hormiga.
			if (i != nodo && visit[i] != 1) 
			{
				//obtener el maximo de la siguiente operación.
				value = refuerzo[nodo][i]*pow(heuristica[nodo][i], coeficiente);
				if (value > max)
				{
					if (it > 800)
					{
						
					}
					max = value;
					pos = i;
				}
			}
		}
		visit[pos] = 1;
		
		return pos;
	}
	else{ //seleccion por ruleta con una probabilidad 1 - q0

		J = seleccion_ruleta(nodo, visit, coeficiente);
		
		visit[J] = 1;
		return J;
	}
}

void actualizacion_local(float factor_ev, vi &rutas)
{
	//Actualiza la matriz de refuerzo de feromona en el enlace que se desplazo la hormiga.
	//la matriz de feromona es siempre el nivel de feromona en el tiempo 0.
	int pos = rutas.size() - 1;
	if (pos == 51)
	{
		refuerzo[rutas[pos]][rutas[0]] = (1 - factor_ev)*refuerzo[rutas[pos]][rutas[0]] + factor_ev*feromona[rutas[pos]][rutas[0]];

		refuerzo[rutas[0]][rutas[pos]] = (1 - factor_ev)*refuerzo[rutas[0]][rutas[pos]] + factor_ev*feromona[rutas[0]][rutas[pos]];
	}
	refuerzo[rutas[pos]][rutas[pos-1]] = (1 - factor_ev)*refuerzo[rutas[pos]][rutas[pos-1]] + factor_ev*feromona[rutas[pos]][rutas[pos-1]];

	refuerzo[rutas[pos-1]][rutas[pos]] = (1 - factor_ev)*refuerzo[rutas[pos-1]][rutas[pos]] + factor_ev*feromona[rutas[pos-1]][rutas[pos]];
}

vi buscar_solucion(vvi &rutas, vi &sol)
{
	float nuevo_costo;
	vi recorrido;
	bool validate = false;
	//buscar nueva solucion en la matriz actual de rutas con costo minimo.
	for (int i = 0; i < rutas.size(); ++i)
	{
		nuevo_costo = 0;
		for (int j = 0; j < rutas[i].size()-1; ++j)
		{
			nuevo_costo = nuevo_costo + distancias[rutas[i][j]][rutas[i][j+1]];
		}
		nuevo_costo = nuevo_costo + distancias[rutas[i][rutas[i].size()-1]][rutas[i][0]];
		if (nuevo_costo < costo)
		{
			costo = nuevo_costo;
			recorrido = rutas[i];
			validate = true;
		}
	}
	if (validate)
	{
		return recorrido;	
	}
	else{
		return sol;
	}
}

void actualizacion_global(vi &solucion, float factor_ev)
{
	int pos;
	for (int i = 1; i < refuerzo.size(); ++i)
	{
		//buscar el nodo dentro de la solucion.
		for (int j = 0; j < solucion.size(); ++j)
		{
			if (solucion[j] == i)
			{
				pos = j;
				j = solucion.size();
			}
		}
		for (int j = i+1; j < refuerzo[i].size(); ++j)
		{
		
			if (pos != solucion.size()-1)
			{
				//actualizar feromona 
				if (j == solucion[pos+1])
				{
					refuerzo[i][j] = (1 - factor_ev)*refuerzo[i][j] + factor_ev*(1/costo);
					refuerzo[j][i] = (1 - factor_ev)*refuerzo[j][i] + factor_ev*(1/costo);
				}
		
				else if(pos != 0){
					//si el nodo i no es el primero en ser visitado, actulizar enlaces distintos al nodo previo en la solucion.
					if (j != solucion[pos-1])
					{
						refuerzo[i][j] = (1 - factor_ev)*refuerzo[i][j];
						refuerzo[j][i] = (1 - factor_ev)*refuerzo[j][i];
					}
				}
				else{
					if (j != solucion[solucion.size()-1])
					{
						refuerzo[i][j] = (1 - factor_ev)*refuerzo[i][j];
						refuerzo[j][i] = (1 - factor_ev)*refuerzo[j][i];								
					}
				}
			}

			//si el nodo i es ultimo visitado, actualizar los enlaces distinto a su nodo previo en la solucion.
			else{
				if (j != solucion[pos-1] && j != solucion[0])
				{
					refuerzo[i][j] = (1 - factor_ev)*refuerzo[i][j];
					refuerzo[j][i] = (1 - factor_ev)*refuerzo[j][i];	
				}
				//reforzar la solucion desde el ultimo nodo retornando al nodo de inicio
				else if (j == solucion[0])
				{
					refuerzo[i][j] = (1 - factor_ev)*refuerzo[i][j] + factor_ev*(1/costo);
					refuerzo[j][i] = (1 - factor_ev)*refuerzo[j][i] + factor_ev*(1/costo);
				}
			}
		}
	}
}
