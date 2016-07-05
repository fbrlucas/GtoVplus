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

void get_value(FILE *arq_i, uint8_t s)//, command_t *comando) //verificar se o ponteiro n reseta.????????????????????----------------------------------------
{
	uint8_t k[8]; //8 digitos do valor dos deslocamentos, le ate 999.99999, se maior aumente o indice
	uint8_t a;
	for(int i = 0; i<9; i++)
	{
		a = (fgetc(arq_i));
		if((a == '\n')||(a == ' '))
		{
			fseek(arq_i,-1,SEEK_CUR); //Volta 1 byte do ponteiro de arquivo
			i = 9;
		}
		else
		{
			fseek(arq_i,-1,SEEK_CUR); //Volta 1 byte do ponteiro de arquivo
			k[i] = fgetc(arq_i);
			if(k[i] == '.') //se for ponto, volta o indice do ponteiro e sobrescreve sobre o ponto. Valor irrelevante
			{
				i--;
			}
		}
	}
	comando.value[s] = (float)atoi(k)/100000.0;
	comando.axis[s] = 1;
}

int ler_arq_gcode(FILE *arq_i, FILE *arq_o, command_t *comando)
{
	char a, b, d;
	uint8_t c[2];
	//uint32_t value;

	arq_i = fopen("back.ngc", "r"); //Abre o arquivo em leitura(r)
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
					fseek(arq_i,1,SEEK_CUR); //jump space value
					d = fgetc(arq_i); //AXIS

					if(d == 'Z') //Se o primeiro valor e Z
					{
						get_value(arq_i, 2); //Indice 2 do vetor axis e value - Z
						fprintf(arq_o, "SPEED 70");
						fprintf(arq_o, "\n");
						fprintf(arq_o, "SET posk = TRANS(0,0,%f,0,180,0)", comando->value[2]);
						fprintf(arq_o, "\n");
						fprintf(arq_o, "MOVES posk");
						fprintf(arq_o, "\n");
					}

					if(d == 'Y') //Se o primeiro valor e Y
					{
						get_value(arq_i, 1); //Indice 1 do vetor axis e value - Y
						fprintf(arq_o, "SPEED 70");
						fprintf(arq_o, "\n");
						fprintf(arq_o, "SET posk = TRANS(0,%f,0,0,180,0)", comando->value[1]);
						fprintf(arq_o, "\n");
						fprintf(arq_o, "MOVES posk");
						fprintf(arq_o, "\n");
					}

					if(d == 'X')
					{
						get_value(arq_i, 0); //Indice 0 do vetor axis e value - X

						if(fgetc(arq_i) == ' '){ //Se não acabou ja pula o space
							fseek(arq_i,1,SEEK_CUR); //Pula o byte do Y
							get_value(arq_i, 1); //Indice 1 do vetor axis e value - Y
							fprintf(arq_o, "SPEED 70");
							fprintf(arq_o, "\n");
							fprintf(arq_o, "SET posk = TRANS(%f,%f,0,0,180,0)", comando->value[0], comando->value[1]);
							fprintf(arq_o, "\n");
							fprintf(arq_o, "MOVES posk");
							fprintf(arq_o, "\n");
						}
						else
						{
							fseek(arq_i,-1,SEEK_CUR); //Volta 1 byte do ponteiro de arquivo
							fprintf(arq_o, "SPEED 70");
							fprintf(arq_o, "\n");
							fprintf(arq_o, "SET posk = TRANS(%f,0,0,0,180,0)", comando->value[0]);
							fprintf(arq_o, "\n");
							fprintf(arq_o, "MOVES posk");
							fprintf(arq_o, "\n");
						}
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

	//arq_in = fopen("backcopy.ngc", "r"); //Abre o arquivo em leitura(r)
	//arq_out = fopen("Vplus_Code.txt", "w"); //Abre o arquivo em escrita (w)

	ler_arq_gcode(arq_in, arq_out, &comando);

	printf("G: %f", comando.value[0]);
	printf("G: %f", comando.value[1]);
	printf("G: %f", comando.value[2]);
	return 0;
}


