/*
 ============================================================================
 Name        : Gcode_to_Vplus_conversor.c
 Author      : Fabricio Lucas
 Version     :
 Copyright   : Your copyright notice
 Description : Hello World in C, Ansi-style
 ============================================================================
 */

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "main.h"

command_t comando;

int dimensao_arq1(FILE *arq1) {
	int quantidade_dados1 = 0;
	arq1 = fopen("backcopy.ngc", "r"); //Abre o arquivo em leiteura(r) usando a variavel arq1

	if (arq1 == NULL) {  //Verifica se o arquivo existe
		printf("Erro ao abrir o arquivo 1");
		fclose(arq1);   //Fecha o arquivo
		exit(1);  //Para o programa
	}

	while (!feof(arq1)) {  //executar ate o arquivo chegar ao fim
		if (fgetc(arq1) == '\n') {  //Confere se o elemento em questao é um \t
			quantidade_dados1++; //Variavel com a quantidade de \t no arquivo 1
		}
	}
	fclose(arq1);  //Fecha o arquivo da massa 1
	return (quantidade_dados1);  //Retorna a dimensao da matriz 1
}

int ler_arq_gcode(FILE *arq_i, FILE *arq_o, command_t *comando)
{
	char a, b, d;
	uint8_t c[2];
	//uint32_t value;

	arq_i = fopen("backcopy.ngc", "r"); //Abre o arquivo em leitura(r)
	arq_o = fopen("Vplus_Code.txt", "w"); //Abre o arquivo em escrita (w)

	while(!feof(arq_i)) {  //executar ate o arquivo chegar ao fim

		a = fgetc(arq_i);

		if(b == '\n'){/*
			if((a == 'G')||(a == 'F')||(a == 'X')||(a == 'Y')||(a == 'Z'))
			{
				comando->code = a;
				printf("!!!Hello World-UHUUU!!!");
			}*/

			if(a == 'G')
			{
				comando->op = a;
				c[0] = fgetc(arq_i);
				c[1] = fgetc(arq_i);

				comando->code = atoi(c);

				if(comando->code == 94) //G94
				{

				}

				if(comando->code == 0) //G0
				{
					d = fgetc(arq_i); //space
					d = fgetc(arq_i); //AXIS
					while(1)
					{
						if(d == 'Z') //Se o primeiro valor e Z
						{
							uint8_t k[8]; //8 digitos do valor dos deslocamentos
							for(int i = 0; i<8; i++)
							{
								k[i] = fgetc(arq_i);
								if(k[i] == '.') //se for ponto, volta o indice do ponteiro e sobrescreve sobre o ponto. Valor irrelevante
								{
									i--;
								}
							}
							comando->z_axis = (float)atoi(k)/100000.0;
							comando->have_z = 1;
						}

						if(fgetc(arq_i) == '\n'){ //Verifica se acabou o comando
							fseek(arq_i,-1,SEEK_CUR); //Volta 1 byte do ponteiro de arquivo
							break;
						}

						fseek(arq_i,-1,SEEK_CUR); //Volta 1 byte do ponteiro de arquivo pois teve um getfc

						if(d == 'Y') //Se o primeiro valor e Y
						{
							uint8_t k[8]; //8 digitos do valor dos deslocamentos
							for(int i = 0; i<8; i++)
							{
								k[i] = fgetc(arq_i);
								if(k[i] == '.') //se for ponto, volta o indice do ponteiro e sobrescreve sobre o ponto. Valor irrelevante
								{
									i--;
								}
							}
							comando->y_axis = (float)atoi(k)/100000.0;
							comando->have_y = 1;
						}

						if(fgetc(arq_i) == '\n'){ //Verifica se acabou o comando
							fseek(arq_i,-1,SEEK_CUR); //Volta 1 byte do ponteiro de arquivo
							break;
						}

						fseek(arq_i,-1,SEEK_CUR); //Volta 1 byte do ponteiro de arquivo pois teve um getfc

						if(d == 'X')
						{
							uint8_t k[8]; //8 digitos do valor dos deslocamentos
							for(int i = 0; i<8; i++)
							{
								k[i] = fgetc(arq_i);
								if(k[i] == '.') //se for ponto, volta o indice do ponteiro e sobrescreve sobre o ponto. Valor irrelevante
								{
									i--;
								}
							}
							comando->x_axis = (float)atoi(k)/100000.0;
							comando->have_x = 1;

							if(fgetc(arq_i) == '\n'){
								fseek(arq_i,-1,SEEK_CUR); //Volta 1 byte do ponteiro de arquivo
								break;
							}
							fseek(arq_i,1,SEEK_CUR); //Pula o byte do Y

							for(int i = 0; i<8; i++)
							{
								k[i] = fgetc(arq_i);
								if(k[i] == '.') //se for ponto, volta o indice do ponteiro e sobrescreve sobre o ponto. Valor irrelevante
								{
									i--;
								}
							}
							comando->y_axis = (float)atoi(k)/100000.0;
							comando->have_y = 1;
						}

						if(fgetc(arq_i) == '\n'){ //Verifica se acabou o comando
							fseek(arq_i,-1,SEEK_CUR); //Volta 1 byte do ponteiro de arquivo
							break;
						}

						//fprint no arquivo arq_o

						//comando->x_axis = comando->y_axis = comando->z_axis = 0;
					}

				}
				if(comando->code == 1) //G1
				{

				}
			}

			if(a == 'F')
			{
				comando->op = a;
				printf("!!!Hello World-UHUUU!!!");
			}
			if(a == 'X')
			{
				comando->op = a;
				printf("!!!Hello World-UHUUU!!!");
			}
			if(a == 'Y')
			{
				comando->op = a;
				printf("!!!Hello World-UHUUU!!!");
			}
			if(a == 'Z')
			{
				comando->op = a;
				printf("!!!Hello World-UHUUU!!!");
			}
		}

		b = a;

/*
		if(fgetc(arq_i) == '\n') {
			if()
			{
				fscanf(arq_i, "%c%i", comando->op, comando->code);
			}
		}*/
	}



	fclose(arq_i);  //Fecha o arquivo
	fclose(arq_o);  //Fecha o arquivo
}

int main(void) {

	FILE *arq_in;  //Ponteiro do arquivo 1
	FILE *arq_out;

	ler_arq_gcode(&arq_in, &arq_out, &comando);

	printf("G: %f", comando.x_axis);
	printf("G: %f", comando.y_axis);
	printf("G: %f", comando.z_axis);
	return 0;
}


