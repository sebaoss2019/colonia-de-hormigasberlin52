#include "fun.h"

using namespace std;

int main(int argc, char const *argv[])
{
	int tam_colonia, semilla, iteraciones, it=1, continuar;
	float evaporacion, coef_heuristico, prob_lim, mejor_costo;
	vi hormigas;
	vi solucion;
	vvi ruta;
	vvi visitado;
	cout<<"\nIngrese valor de semilla: ";
	cin>>semilla;
	cout<<"\nCantidad de hormigas: ";
	cin>>tam_colonia;
	cout<<"\nFactor de evaporacion(alfa): ";
	cin>>evaporacion;
	cout<<"\nHeuristica(Beta): ";
	cin>>coef_heuristico;
	cout<<"\nProbilidad(q0): ";
	cin>>prob_lim;
	cout<<"\nCondicion de termino: ";
	cin>>iteraciones;
	char cadena[20];
	srand(semilla);
	Leer_archivo();
	
	solucion = iniciar_feromona();
	cout<<"Solucion inicial:"<<endl;
	for (int i = 0; i < solucion.size(); ++i)
	{
		cout<<solucion[i]<<" ";
	}
	cout<<endl;
	while(it <= iteraciones)
	{
		ruta.resize(tam_colonia);
		visitado.resize(tam_colonia);

		for (int i = 0; i < visitado.size(); ++i)
		{
			visitado[i].resize(puntos.size()+1,0);
			visitado[i][0] = 1;
		}
		
		hormigas = asignar_hormigas(tam_colonia); 
		for (int i = 0; i < hormigas.size(); ++i)
		{
			ruta[i].push_back(hormigas[i]);
			visitado[i][hormigas[i]] = 1;
		}

		for (int i = 1; i < puntos.size(); ++i)
		{
			for (int j = 0; j < hormigas.size(); ++j)
			{
				hormigas[j] = trancision(hormigas[j],visitado[j],prob_lim,coef_heuristico,it);
				ruta[j].push_back(hormigas[j]);
			}
			
			for (int j = 0; j < hormigas.size(); ++j)
			{
				actualizacion_local(evaporacion,ruta[j]);
			}
		
		}

		solucion = buscar_solucion(ruta,solucion);
		cout<<"\nSolucion global en iteracion "<<it<<":"<<endl;
		for (int i = 0; i < solucion.size(); ++i)
		{
			cout<<solucion[i]<<" ";
		}
		printf("\nCosto:  %.4f vs costo inicial %.4f\n",costo,costo_inicial);
		actualizacion_global(solucion,evaporacion);
		ruta.clear();
		visitado.clear();
		it++;
		if (it > 750)
		{
			
		}
	}

	return 0;
}
