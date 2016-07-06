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

static void get_value(FILE *arq_i, uint8_t s)//, command_t *comando) //verificar se o ponteiro n reseta.????????????????????----------------------------------------
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

static void read__gcode_write_vplus(FILE *arq_i, FILE *arq_o, command_t *comando)
{
	char a, b, d;
	uint8_t c[2];
	char name[20];
	//uint32_t value;

	arq_i = fopen("front.ngc", "r"); //Abre o arquivo em leitura(r)
	arq_o = fopen("Vplus_Code.V2", "w"); //Abre o arquivo em escrita (w)

	if(arq_i == NULL) //Verifica se o arquivo existe
	{
		printf("Erro ao abrir o arquivo");
		fclose(arq_i);
		fclose(arq_o);
		exit(1); //Para o programa
	}

	setbuf(stdout, NULL); //disable the buffer
	printf("Enter name of program: ");
	scanf("%s", name);
	fprintf(arq_o, ".PROGRAM %s()", name);
	fprintf(arq_o, "\n\n");

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
					fprintf(arq_o, "\tSPEED "FAST_SPEED" MMPS ALWAYS");
					fprintf(arq_o, "\n");
				}
				if(comando->code == 93) //G94
				{
					fprintf(arq_o, "\tSPEED "FAST_SPEED" IPS ALWAYS");
					fprintf(arq_o, "\n");
				}

				if(comando->code == 0) //G0
				{
					fseek(arq_i,1,SEEK_CUR); //jump space value
					d = fgetc(arq_i); //AXIS

					if(d == 'Z') //Se o primeiro valor e Z
					{
						get_value(arq_i, 2); //Indice 2 do vetor axis e value - Z
						fprintf(arq_o, "\tSPEED "FAST_SPEED);
						fprintf(arq_o, "\n");
						fprintf(arq_o, "\tSET posk = TRANS(0,0,%f,0,180,0)", comando->value[2]);
						fprintf(arq_o, "\n");
						fprintf(arq_o, "\tMOVES posk");
						fprintf(arq_o, "\n");
					}

					if(d == 'Y') //Se o primeiro valor e Y
					{
						get_value(arq_i, 1); //Indice 1 do vetor axis e value - Y
						fprintf(arq_o, "\tSPEED "FAST_SPEED);
						fprintf(arq_o, "\n");
						fprintf(arq_o, "\tSET posk = TRANS(0,%f,0,0,180,0)", comando->value[1]);
						fprintf(arq_o, "\n");
						fprintf(arq_o, "\tMOVES posk");
						fprintf(arq_o, "\n");
					}

					if(d == 'X')
					{
						get_value(arq_i, 0); //Indice 0 do vetor axis e value - X

						if(fgetc(arq_i) == ' '){ //Se não acabou ja pula o space
							fseek(arq_i,1,SEEK_CUR); //Pula o byte do Y
							get_value(arq_i, 1); //Indice 1 do vetor axis e value - Y
							fprintf(arq_o, "\tSPEED "FAST_SPEED);
							fprintf(arq_o, "\n");
							fprintf(arq_o, "\tSET posk = TRANS(%f,%f,0,0,180,0)", comando->value[0], comando->value[1]);
							fprintf(arq_o, "\n");
							fprintf(arq_o, "\tMOVES posk");
							fprintf(arq_o, "\n");
						}
						else
						{
							fseek(arq_i,-1,SEEK_CUR); //Volta 1 byte do ponteiro de arquivo
							fprintf(arq_o, "\tSPEED "FAST_SPEED);
							fprintf(arq_o, "\n");
							fprintf(arq_o, "\tSET posk = TRANS(%f,0,0,0,180,0)", comando->value[0]);
							fprintf(arq_o, "\n");
							fprintf(arq_o, "\tMOVES posk");
							fprintf(arq_o, "\n");
						}
					}
				}

				if(comando->code == 1) //G1
				{
					fseek(arq_i,1,SEEK_CUR); //jump space value
					d = fgetc(arq_i); //AXIS

					if(d == 'Z') //Se o primeiro valor e Z
					{
						get_value(arq_i, 2); //Indice 2 do vetor axis e value - Z
						fprintf(arq_o, "\tSET posk = TRANS(0,0,%f,0,180,0)", comando->value[2]);
						fprintf(arq_o, "\n");
						fprintf(arq_o, "\tMOVES posk");
						fprintf(arq_o, "\n");
					}

					if(d == 'Y') //Se o primeiro valor e Y
					{
						get_value(arq_i, 1); //Indice 1 do vetor axis e value - Y
						fprintf(arq_o, "\tSET posk = TRANS(0,%f,0,0,180,0)", comando->value[1]);
						fprintf(arq_o, "\n");
						fprintf(arq_o, "\tMOVES posk");
						fprintf(arq_o, "\n");
					}

					if(d == 'X')
					{
						get_value(arq_i, 0); //Indice 0 do vetor axis e value - X

						if(fgetc(arq_i) == ' '){ //Se não acabou ja pula o space
							fseek(arq_i,1,SEEK_CUR); //Pula o byte do Y
							get_value(arq_i, 1); //Indice 1 do vetor axis e value - Y
							fprintf(arq_o, "\tSET posk = TRANS(%f,%f,0,0,180,0)", comando->value[0], comando->value[1]);
							fprintf(arq_o, "\n");
							fprintf(arq_o, "\tMOVES posk");
							fprintf(arq_o, "\n");
						}
						else
						{
							fseek(arq_i,-1,SEEK_CUR); //Volta 1 byte do ponteiro de arquivo
							fprintf(arq_o, "\tSET posk = TRANS(%f,0,0,0,180,0)", comando->value[0]);
							fprintf(arq_o, "\n");
							fprintf(arq_o, "\tMOVES posk");
							fprintf(arq_o, "\n");
						}
					}
				}
			}

			if(a == 'F')
			{
				get_value(arq_i, 0);
				//fprintf(arq_o, "SPEED %f", comando->value[0]);
				fprintf(arq_o, "\tSPEED "SLOW_SPEED);
				fprintf(arq_o, "\n");
			}
			if(a == 'X')
			{
				get_value(arq_i, 0); //Indice 0 do vetor axis e value - X

				if(fgetc(arq_i) == ' '){ //Se não acabou ja pula o space
					fseek(arq_i,1,SEEK_CUR); //Pula o byte do Y
					get_value(arq_i, 1); //Indice 1 do vetor axis e value - Y
					fprintf(arq_o, "\tSET posk = TRANS(%f,%f,0,0,180,0)", comando->value[0], comando->value[1]);
					fprintf(arq_o, "\n");
					fprintf(arq_o, "\tMOVES posk");
					fprintf(arq_o, "\n");
				}
				else
				{
					fseek(arq_i,-1,SEEK_CUR); //Volta 1 byte do ponteiro de arquivo
					fprintf(arq_o, "\tSET posk = TRANS(%f,0,0,0,180,0)", comando->value[0]);
					fprintf(arq_o, "\n");
					fprintf(arq_o, "\tMOVES posk");
					fprintf(arq_o, "\n");
				}
			}
			if(a == 'Y')
			{
				get_value(arq_i, 1); //Indice 1 do vetor axis e value - Y
				fprintf(arq_o, "\tSET posk = TRANS(0,%f,0,0,180,0)", comando->value[1]);
				fprintf(arq_o, "\n");
				fprintf(arq_o, "\tMOVES posk");
				fprintf(arq_o, "\n");
			}
			if(a == 'Z')
			{
				get_value(arq_i, 2); //Indice 2 do vetor axis e value - Z
				fprintf(arq_o, "\tSET posk = TRANS(0,0,%f,0,180,0)", comando->value[2]);
				fprintf(arq_o, "\n");
				fprintf(arq_o, "\tMOVES posk");
				fprintf(arq_o, "\n");
			}
		}

		b = a;
	}

	fprintf(arq_o, "\n.END"); //Fim do programa V+

	fclose(arq_i);  //Fecha o arquivo
	fclose(arq_o);  //Fecha o arquivo
}

int main(void) {

	FILE *arq_in;  //Ponteiro do arquivo 1
	FILE *arq_out;

	read__gcode_write_vplus(arq_in, arq_out, &comando);

	printf("\nCONVERSÃO CONCLUÍDA!!!!!!!!!!");

	return 0;
}


