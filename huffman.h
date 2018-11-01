#ifndef HUFFMAN_H
#define HUFFMAN_H

#include <stdlib.h>
#include <string.h>

#include "arbre.h"

#define MAX_CHAR 256

typedef struct valeur_huffman v_huffman;

void compresser_fichier(FILE * source, int * nb_bits, int * taille, nd * arbre, FILE * dest);
void decompresser_fichier(FILE * source, int nb_bits, int taille, nd arbre, FILE * dest);

char * compresser_texte(char * texte, int * nb_bits, nd * arbre);
char * decompresser_texte(char * texte, int nb_bits, int n, nd arbre);

nd creer_arbre_huffman(char * texte);
int calculer_frequences(char * texte, int * frequences);
nd * creer_arbres_minimalistes(int * frequences, int n);
nd fusionner_arbres(nd a1, nd a2);
nd generer_arbre_huffman(nd * liste_arbres, int n);
void remplir_codes_arbre_huffman(nd arbre);
void remplir_codes_arbre_huffman_recursif(nd arbre, int code, int taille_code);

void * allouer_valeur_huffman(void * valeur);
void liberer_valeur_huffman(void ** valeur);
void afficher_valeur_huffman(void * valeur);
int comparer_caracteres(void * v1, void * v2);

#endif
