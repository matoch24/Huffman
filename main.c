#include <stdlib.h>
#include <stdio.h>

#include "arbre.h"
#include "huffman.h"

void * allouer_valeur(void * valeur) {
    void * bloc = NULL;
    
    if (valeur != NULL) {
        bloc = malloc(sizeof(int));
        if (bloc != NULL) {
            memcpy(bloc, valeur, sizeof(int));
        }
    }
    
    return bloc;
}

void liberer_valeur(void ** valeur) {
    if (valeur != NULL && *valeur != NULL) {
        free(*valeur);
        *valeur = NULL;
    }
}

int comparer_valeurs(void * v1, void * v2) {
    int res = 0;
    
    if (v1 != NULL && v2 != NULL) {
        res = *((int *) v1) == *((int *) v2);
    }
    
    return res;
}

void afficher_valeur(void * valeur) {
    if (valeur != NULL) {
        printf("%d ", *((int *) valeur));
    }
}

int main() {
    /**
     * arbre.h
     */
    int a = 1, b = 2, c = 3, d = 4, e = 4;
    
    nd n = creer_arbre(&a, allouer_valeur);
    ajouter_noeud_gauche(n, &b, allouer_valeur);
    ajouter_noeud_droit(n, &c, allouer_valeur);
    ajouter_noeud_droit(get_gauche(n), &d, allouer_valeur);
    afficher_arbre(n, afficher_valeur);
    printf("\n");

    int r = *((int *) get_valeur(rechercher_element(n, &e, comparer_valeurs)));
    printf("%d\n", r);

    detruire_arbre(&n, liberer_valeur);
    
    /**
     * huffman.h
     */
    nd arbre = creer_arbre_huffman("Lorem ipsum dolor sit amet");
    remplir_codes_arbre_huffman(arbre);
    afficher_arbre(arbre, afficher_valeur_huffman);
    printf("\n");
    
    detruire_arbre(&arbre, liberer_valeur_huffman);
    
    int taille;
    int nb_bits;
    char * texte = "Lorem ipsum dolor sit amet";
    char * compression = compresser_texte(texte, &nb_bits, &arbre);
    char * decompression = decompresser_texte(compression, nb_bits, strlen(texte), arbre);
    printf("%s\n", decompression);
    
    detruire_arbre(&arbre, liberer_valeur_huffman);
    free(compression);
    free(decompression);
    
    FILE * entree = fopen("entree", "r");
    FILE * sortie = fopen("sortie", "w+");
    FILE * rendu = fopen("rendu", "w");
    compresser_fichier(entree, &nb_bits, &taille, &arbre, sortie);
    fseek(sortie, 0, SEEK_SET);
    decompresser_fichier(sortie, nb_bits, taille, arbre, rendu);
    
    detruire_arbre(&arbre, liberer_valeur_huffman);
    
    fclose(entree);
    fclose(sortie);
    fclose(rendu);
    
    return EXIT_SUCCESS;
}
