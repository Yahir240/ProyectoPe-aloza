Proyecto Pe�aloza 
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include "corrector.h"
#define ALFABETO "abcdefghijklmnopqrstuvwxyzáéíóú"
#define MAX_PALABRAS 50000

// Crear el diccionario completo
void Diccionario(char* szNombre, char szPalabras[][TAMTOKEN], int iEstadisticas[], int& iNumElementos) {
    FILE* archivo;
    if (fopen_s(&archivo, szNombre, "r") != 0) {
        printf("No se pudo abrir el archivo: %s.\n", szNombre);
        return;
    }

    char palabra[TAMTOKEN];
    iNumElementos = 0;

    while (fscanf_s(archivo, "%49s", palabra, (unsigned)_countof(palabra)) != EOF) {
        int inicio = 0; // Índice de inicio para dividir palabras tras separadores
        for (int i = 0; palabra[i]; i++) {
            // Si encontramos una coma o punto, procesamos la subpalabra actual
            if (palabra[i] == ',' || palabra[i] == '.') {
                if (i > inicio) {
                    char subPalabra[TAMTOKEN];
                    int j = 0;
                    for (int k = inicio; k < i && j < TAMTOKEN - 1; k++) {
                        if (isalnum((unsigned char)palabra[k]) || strchr("áéíóúÁÉÍÓÚ'&$#%!<[]>‐-+/\".", palabra[k])) {
                            subPalabra[j++] = tolower((unsigned char)palabra[k]);
                        }
                    }
                    subPalabra[j] = '\0'; // Termina la subpalabra

                    if (strlen(subPalabra) > 0) {
                        // Agregar la subpalabra al diccionario
                        int existe = -1;
                        int found = 0; // Bandera para verificar si ya existe
                        for (int l = 0; l < iNumElementos && !found; l++) {
                            if (strcmp(szPalabras[l], subPalabra) == 0) {
                                existe = l;
                                found = 1; // Encontrado
                            }
                        }

                        if (!found && iNumElementos < NUMPALABRAS) {
                            strcpy_s(szPalabras[iNumElementos], TAMTOKEN, subPalabra);
                            iEstadisticas[iNumElementos] = 1;
                            iNumElementos++;
                        }
                        else if (found) {
                            iEstadisticas[existe]++;
                        }
                        }
                }
                inicio = i + 1; // Reiniciar después del separador
            }
        }

// Procesar cualquier palabra restante después del último separador
        if (palabra[inicio]) {
            char subPalabra[TAMTOKEN];
            int j = 0;
            for (int k = inicio; palabra[k] && j < TAMTOKEN - 1; k++) {
                if (isalnum((unsigned char)palabra[k]) || strchr("áéíóúÁÉÍÓÚ'&$#!%<[]>‐-+/\".", palabra[k])) {
                     subPalabra[j++] = tolower((unsigned char)palabra[k]);
                }
            }
            subPalabra[j] = '\0'; // Termina la subpalabra

            if (strlen(subPalabra) > 0) {
                // Agregar la subpalabra al diccionario
                int existe = -1;
                int found = 0; // Bandera para verificar si ya existe
                for (int l = 0; l < iNumElementos && !found; l++) {
                    if (strcmp(szPalabras[l], subPalabra) == 0) {
                        existe = l;
                        found = 1; // Encontrado
                    }
                }

                if (!found && iNumElementos < NUMPALABRAS) {
                    strcpy_s(szPalabras[iNumElementos], TAMTOKEN, subPalabra);
                    iEstadisticas[iNumElementos] = 1;
                    iNumElementos++;
                }
                else if (found) {
                    iEstadisticas[existe]++;
                }
                }
        }
    }
    fclose(archivo);

    // Ordenar el diccionario alfabéticamente
    for (int i = 0; i < iNumElementos - 1; i++) {
        for (int j = i + 1; j < iNumElementos; j++) {
            if (strcmp(szPalabras[i], szPalabras[j]) > 0) {
                char temp[TAMTOKEN];
                strcpy_s(temp, TAMTOKEN, szPalabras[i]);
                strcpy_s(szPalabras[i], TAMTOKEN, szPalabras[j]);
                strcpy_s(szPalabras[j], TAMTOKEN, temp);

                int tempEstadistica = iEstadisticas[i];
                iEstadisticas[i] = iEstadisticas[j];
                iEstadisticas[j] = tempEstadistica;
            }
        }
    }
}