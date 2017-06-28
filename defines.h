#ifndef _DEFINES_
#define _DEFINES_

//Cada um usa apenas o seu
#define ID_TYPE_ULTRASSOM 	(0x00)
#define ID_TYPE_ATUADOR 	(0x01)
#define ID_TYPE_FUMACA 		(0x02)
#define ID_TYPE_CAMERA		(0x03)

//Identificar que é resposta do comando CMD_GET_INFO
#define RESPONSE			(0xB0)
//Exemplo
//ID_TYPE_X = ID_TYPE_FUMACA | RESPONSE = 0xB2

//Apenas gateway usa
#define ID_TYPE_GATEWAY 	(0xAA)
#define ID_TYPE_BROADCAST 	(0xBB)

//Apenas gateway da comandos
#define CMD_ENABLE			(0xFF)
#define CMD_DISABLE			(0x00)
#define CMD_GET_INFO		(0x0F) //Comando para receber informações

//Apenas para pessoal da camera
#define CMD_GET_PHOTO		(0xF0)

#define TRUE				(0x01)
#define FALSE				(0x00)


/*Estrutura para resposta dos sensores*/
/*ID_TYPE_X | ID | DATA*/
/*ID_TYPE_X é uma das constantes de tipo*/
/*Onde ID é um identificador 0, 1, 2, 3...*/
/*Data é 1 para detectado e 0 para nao detectado*/

/*Estrutura para resposta da camera*/
/*ID_TYPE_CAMERA | ID | SEQ | TOTAL |NUM_BYTES | DATA*/
/*Onde ID é um identificador 0, 1, 2, 3...*/
/*SEQ é o número do pacote*/
/*NUM_BYTES é o número de bytes de dado*/
/*DATA são os bytes da imagem enviados no pacote*/
/*TOTAL é o número de pacotes*/

/*Estrutura para comandos*/
/*ID_TYPE_GATEWAY | CMD_X*/
/*Onde CMD_X é uma das constantes de comando*/

#endif
