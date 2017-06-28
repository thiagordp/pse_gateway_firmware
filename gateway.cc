#include "gateway.h"

int main()
{
	//cout << "To funcionando brow" << endl;	
	Gateway g;

	g.RequestAddres(); //Requisistando os endereços dos dispositivos
	while(1)
	{
		g.TreatZigBee();
	}
	

	return 0;
}
