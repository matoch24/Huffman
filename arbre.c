#include "arbre.h"

struct noeud {
    void * valeur;
    struct noeud* gauche;
    struct noeud* droit;
};

nd creer_arbre(void * valeur, void * (*allouer_valeur)(void *)) {
    nd n = NULL;
    
    if (valeur != NULL && allouer_valeur != NULL) {
        n = (nd) malloc(sizeof(struct noeud));
        if (n != NULL) {
            n->valeur = allouer_valeur(valeur);
            n->gauche = NULL;
            n->droit = NULL;
        }
    }
    
    return n;
}

void detruire_arbre(nd * n, void (*liberer_valeur)(void **)) {
    if (n != NULL && *n != NULL && liberer_valeur != NULL) {
        detruire_arbre(&(*n)->gauche, liberer_valeur);
        detruire_arbre(&(*n)->droit, liberer_valeur);
        liberer_valeur(&(*n)->valeur);
        free(*n);
        *n = NULL;
    }
}

void * get_valeur(nd n) {
    void * valeur = NULL;
    
    if (n != NULL) {
        valeur = n->valeur;
    }
    
    return valeur;
}

nd get_gauche(nd n) {
    nd gauche = NULL;
    
    if (n != NULL) {
        gauche = n->gauche;
    }
    
    return gauche;
}

nd get_droit(nd n) {
    nd droit = NULL;
    
    if (n != NULL) {
        droit = n->droit;
    }
    
    return droit;
}

int est_feuille(nd n) {
    int feuille = 0;
    
    if (n != NULL) {
        feuille = (n->gauche == NULL && n->droit == NULL);
    }
    
    return feuille;
}

void ajouter_noeud_gauche(nd n, void * valeur, void * (*allouer_valeur)(void *)) {
    if (n != NULL) {
        n->gauche = creer_arbre(valeur, allouer_valeur);
    }
}

void ajouter_noeud_droit(nd n, void * valeur, void * (*allouer_valeur)(void *)) {
    if (n != NULL) {
        n->droit = creer_arbre(valeur, allouer_valeur);
    }
}

void lier_noeud_gauche(nd n, nd gauche) {
    if (n != NULL) {
        n->gauche = gauche;
    }
}

void lier_noeud_droit(nd n, nd droit) {
    if (n != NULL) {
        n->droit = droit;
    }
}

void supprimer_noeud_gauche(nd n, void (*liberer_valeur)(void **)) {
    if (n != NULL) {
        detruire_arbre(&n->gauche, liberer_valeur);
    }
}

void supprimer_noeud_droit(nd n, void (*liberer_valeur)(void **)) {
    if (n != NULL) {
        detruire_arbre(&n->droit, liberer_valeur);
    }
}

nd rechercher_element(nd n, void * valeur, int (*comparer_valeurs)(void *, void *)) {
    nd res = NULL;
    
    if (n != NULL && valeur != NULL && comparer_valeurs != NULL) {
        if (comparer_valeurs(n->valeur, valeur)) {
            res = n;
        }
        else {
            res = rechercher_element(n->gauche, valeur, comparer_valeurs);
            if (res == NULL) {
                res = rechercher_element(n->droit, valeur, comparer_valeurs);
            }
        }
    }
    
    return res;
}

void afficher_arbre(nd n, void (*afficher_valeur)(void *)) {
    if (n != NULL && afficher_valeur != NULL) {
        afficher_valeur(n->valeur);
        if (n->gauche != NULL) {
            afficher_arbre(n->gauche, afficher_valeur);
        }
        if (n->droit != NULL) {
            afficher_arbre(n->droit, afficher_valeur);
        }
    }
}
