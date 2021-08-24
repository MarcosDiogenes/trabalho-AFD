#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
AFD e AFN- Complemento de Exercícios. O aluno deve implementar um algoritmo que
verifique se cadeias pertencem ou não a determinadas linguagens através  de AFD
e AFN reconhecedores. O aluno deve gerar um relatório de acordo com a atividade
prevista e descrita no google classroom. 

(upload do Código para o Github da equipe)
*/

// Dupla: Marcos Vinícius Pereira Diógenes e Kefton David Nunes de Melo

void escreverPalavra(char *p);

int main(void)
{

    // Leitura do arquivo de entrada
    FILE *fp;
    fp = fopen("input.txt", "r");
    if (fp == NULL)
    {
        printf("ERRO: Arquivo 'input.txt' não encontrado.\n");
        return 1;
    }

    fseek(fp, 0, SEEK_END);
    int tamanho = ftell(fp);
    rewind(fp);

    char *entradas = malloc(sizeof(char) * (tamanho + 1));
    size_t tamanhoLido = fread(entradas, sizeof(char), tamanho, fp);
    if ((int)tamanhoLido != tamanho)
    {
        printf("ERRO: Arquivo não lido corretamente.\n");
        return 1;
    }
    entradas[tamanho] = '\0';
    fclose(fp);

    // 1 - Definir o alfabeto

    int i = 0;
    char simboloEsperado = 'a';
    char alfabeto[10] = {'0', '0', '0', '0', '0', '0', '0', '0', '0', '0'};
    while (entradas[i] != '\n')
    {
        if (i == 10)
        {
            printf("\nERRO: São permitidos no máximo 10 símbolos no alfabeto.\n");
            return 1;
        }
        if (entradas[i] != simboloEsperado)
        {
            printf("\nERRO: São aceitas somente letras minúsculas e em ordem alfabética para o alfabeto.\n");
            return 1;
        }
        alfabeto[i] = entradas[i];
        i++;
        simboloEsperado++;
    }
    int tamanhoAlfabeto = i;

    // 2 - Definir número de estados

    i++;
    char entradaEstados[2] = {' ', ' '};
    int j = 0;
    while (entradas[i] != '\n')
    {
        if (j > 1)
        {
            printf("\nERRO: Número máximo de estados: 20.\n");
            return 1;
        }
        entradaEstados[j] = entradas[i];
        i++;
        j++;
    }
    int nEstados = atoi(entradaEstados);
    if (nEstados > 20)
    {
        printf("\nERRO: Número máximo de estados: 20.\n");
        return 1;
    }

    // 3 - Número de estados finais

    i++;
    j = 0;
    char entradaFinais[2] = {' ', ' '};
    while (entradas[i] != '\n')
    {
        entradaFinais[j] = entradas[i];
        i++;
        j++;
    }
    int nFinais = atoi(entradaFinais);
    if (nFinais > nEstados)
    {
        printf("\nERRO: Número de estados finais deve ser menor ou igual ao de estados.\n");
        return 1;
    }

    // 4 - Estados finais

    i++;
    int estadosFinais[nFinais];
    j = 0;
    int jEstadosFinais = 0;
    char estadoFinal[2] = {' ', ' '};
    while (1)
    {
        if (entradas[i] == ' ')
        {
            int e = atoi(estadoFinal);
            if (e > nEstados)
            {
                printf("\nERRO: Estado final %d inválido.\n", e);
                return 1;
            }
            estadosFinais[jEstadosFinais] = e;
            jEstadosFinais++;
            j = 0;
            estadoFinal[1] = ' ';
        }
        else if (entradas[i] == '\n')
        {
            int e = atoi(estadoFinal);
            if (e > nEstados)
            {
                printf("\nERRO: Estado final %d inválido.\n", e);
                return 1;
            }
            estadosFinais[jEstadosFinais] = e;
            break;
        }
        else
        {
            estadoFinal[j] = entradas[i];
            j++;
        }
        i++;
    }

    // 5 - Número de transições

    i++;
    j = 0;
    char entradaTransicoes[4] = {' ', ' ', ' ', ' '};
    while (entradas[i] != '\n')
    {
        entradaTransicoes[j] = entradas[i];
        i++;
        j++;
    }
    int nTransicoes = atoi(entradaTransicoes);
    if (nTransicoes > nEstados * tamanhoAlfabeto * nEstados)
    {
        printf("\nERRO: Número de transações deve ser menor ou igual ao número de estados ao quadrado vezes o tamanho do alfabeto.\n");
        return 1;
    }

    // 6 a 5 + nTransicoes -  Definições das transições

    i++;

    int matrizTransicoes[nEstados][tamanhoAlfabeto][nEstados];
    for (int lin = 0; lin < nEstados; lin++)
    {
        for (int col = 0; col < tamanhoAlfabeto; col++)
        {
            for (int alt = 0; alt < nEstados; alt++)
            {
                matrizTransicoes[lin][col][alt] = -1;
            }
        }
    }

    char entradaTransicao[2] = {' ', ' '};
    j = 0;
    char *tipo = "INT";
    int estadoPrevio, simbolo, estadoFuturo, jMatriz, t = 0;
    while (t < nTransicoes)
    {
        if (entradas[i] == '\n')
        {
            j = 0;
            estadoFuturo = atoi(entradaTransicao);
            if (estadoFuturo > nEstados)
            {
                printf("\nERRO: Estado %d inválido.\n", estadoFuturo);
                return 1;
            }
            jMatriz = 0;
            while (matrizTransicoes[estadoPrevio][simbolo][jMatriz] != -1)
                jMatriz++;
            matrizTransicoes[estadoPrevio][simbolo][jMatriz] = estadoFuturo;
            t++;
        }
        else if (entradas[i] == ' ')
        {
            j = 0;
            if (strncmp(tipo, "INT", 3) == 0)
            {
                estadoPrevio = atoi(entradaTransicao);
                if (estadoPrevio > nEstados)
                {
                    printf("\nERRO: Estado %d inválido.\n", estadoPrevio);
                    return 1;
                }
                tipo = "CHAR";
            }
            else if (strncmp(tipo, "CHAR", 3) == 0)
            {
                simbolo = entradaTransicao[0] - 'a';
                tipo = "INT";
            }
        }
        else
        {
            if (strncmp(tipo, "INT", 3) == 0)
            {
                entradaTransicao[j] = entradas[i];
                j++;
            }
            else
            {
                entradaTransicao[0] = entradas[i];
            }
        }
        i++;
    }

    j = 0;
    char entradaPalavras[3] = {' ', ' ', ' '};
    while (entradas[i] != '\n')
    {
        entradaPalavras[j] = entradas[i];
        i++;
        j++;
    }
    int nPalavras = atoi(entradaPalavras);

    // 7 + nTransicoes em diante: Análise das cadeias

    i++;
    char palavra[100];
    int q[nEstados];       // Armazena os possíveis estados do AFnD, com 1 está ativo e 0 quando não está.
    int qFuturo[nEstados]; // Armazena os próximos estados;
    char *res;             // Armazena o resultado da leitura da palavra
    for (int p = 1; p <= nPalavras; p++)
    {
        q[0] = 1;
        for (int e = 1; e < nEstados; e++)
            q[e] = 0;
        j = 0;
        for (int l = 0; l < 100; l++)
        {
            palavra[l] = ' ';
        }
        while (entradas[i] != '\n' && entradas[i] != '\0')
        {
            if (j >= 100)
            {
                printf("\nERRO: A palavra ultrapassou o limite de 100 caracteres.\n");
                return 1;
            }
            palavra[j] = entradas[i];
            i++;
            j++;
        }
        j = 0;
        while (j < 100 && palavra[j] != ' ')
        {
            for (int e = 0; e < nEstados; e++)
            {
                qFuturo[e] = 0;
            }
            for (int e = 0; e < nEstados; e++)
            {
                if (q[e])
                {
                    jMatriz = 0;
                    while (jMatriz < nEstados && matrizTransicoes[e][palavra[j] - 'a'][jMatriz] != -1)
                    {
                        qFuturo[matrizTransicoes[e][palavra[j] - 'a'][jMatriz]] = 1;
                        jMatriz++;
                    }
                }
            }
            for (int e = 0; e < nEstados; e++)
            {
                q[e] = qFuturo[e];
            }
            j++;
        }
        res = "Reprovada";
        for (int k = 0; k < nFinais; k++)
        {
            if (q[estadosFinais[k]])
            {
                res = "Aprovada";
            }
        }
        printf("%d: ", p);
        escreverPalavra(palavra);
        printf(" %s\n", res);
        i++;
    }
    return 0;
}

void escreverPalavra(char *p)
{
    int i = 0;
    while (p[i] != ' ')
        printf("%c", p[i++]);
}
