#include "gateway.h"
UART uart2(0, 9600, 8, 0, 1);
int main()
{
	//cout << "To funcionando brow" << endl;	
	uart2.put('A');
	uart2.put('\n');
	Gateway g;

	g.RequestAddres(); //Requisistando os endereços dos dispositivos
	while(1)
	{
		g.TreatZigBee();
		g.TreatWiFi();
	}
	

	return 0;
}
