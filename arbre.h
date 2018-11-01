#ifndef ARBRE_H
#define ARBRE_H

#include <stdio.h>
#include <stdlib.h>

typedef struct noeud* nd;

nd creer_arbre(void * valeur, void * (*allouer_valeur)(void *));
void detruire_arbre(nd * n, void (*liberer_valeur)(void **));

void * get_valeur(nd n);
nd get_gauche(nd n);
nd get_droit(nd n);

int est_feuille(nd n);

void ajouter_noeud_gauche(nd n, void * valeur, void * (*allouer_valeur)(void *));
void ajouter_noeud_droit(nd n, void * valeur, void * (*allouer_valeur)(void *));

void lier_noeud_gauche(nd n, nd gauche);
void lier_noeud_droit(nd n, nd droit);

void supprimer_noeud_gauche(nd n, void (*liberer_valeur)(void **));
void supprimer_noeud_droit(nd n, void (*liberer_valeur)(void **));

nd rechercher_element(nd n, void * valeur, int (*comparer_valeurs)(void *, void *));
void afficher_arbre(nd n, void (*afficher_valeur)(void *));

#endif
