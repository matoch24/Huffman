#include "huffman.h"

struct valeur_huffman {
    char caractere;
    int frequence;
    int code;
    int taille_code;
};

void compresser_fichier(FILE * source, int * nb_bits, int * taille, nd * arbre, FILE * dest) {
    if (source != NULL && nb_bits != NULL && taille != NULL && arbre != NULL && dest != NULL) {
        fseek(source, 0, SEEK_END);
        *taille = ftell(source);
        fseek(source, 0, SEEK_SET);
        
        char * texte = (char *) malloc((*taille + 1) * sizeof(char));
        fread(texte, sizeof(char), *taille, source);
        texte[*taille] = '\0';
        
        char * compression = compresser_texte(texte, nb_bits, arbre);
        fwrite(compression, sizeof(char), (*nb_bits - 1) / 8 + 1, dest);
        
        free(texte);
        free(compression);
    }
}

void decompresser_fichier(FILE * source, int nb_bits, int taille, nd arbre, FILE * dest) {
    if (source != NULL && nb_bits > 0 && taille > 0 && arbre != NULL && dest != NULL) {
        char * texte = (char *) malloc(((nb_bits - 1) / 8 + 1) * sizeof(char));
        fread(texte, sizeof(char), (nb_bits - 1) / 8 + 1, source);
        
        char * decompression = decompresser_texte(texte, nb_bits, taille, arbre);
        fwrite(decompression, sizeof(char), taille, dest);
        
        free(texte);
        free(decompression);
    }
}

char * compresser_texte(char * texte, int * nb_bits, nd * arbre) {
    char * compression = NULL;
    
    if (texte != NULL && nb_bits != NULL && arbre != NULL) {
        *arbre = creer_arbre_huffman(texte);
        size_t len = strlen(texte);
        compression = (char *) calloc(len, sizeof(char));
        *nb_bits = 0;
        
        for (size_t i = 0; i < len; i++) {
            nd elem = rechercher_element(*arbre, &texte[i], comparer_caracteres);
            if (elem != NULL) {
                int code = ((v_huffman *) get_valeur(elem))->code;
                for (int j = ((v_huffman *) get_valeur(elem))->taille_code - 1; j >= 0; j--) {
                    compression[*nb_bits / 8] += ((code & (1 << j)) >> j) << (7 - *nb_bits % 8);
                    *nb_bits += 1;
                }
            }
        }
    }
    
    return compression;
}

char * decompresser_texte(char * texte, int nb_bits, int n, nd arbre) {
    char * decompression = NULL;
    
    if (texte != NULL && nb_bits > 0 && n > 0 && arbre != NULL) {
        decompression = (char *) malloc((n + 1) * sizeof(char));
        
        int i;
        if (est_feuille(arbre)) {
            char caractere = ((v_huffman *) get_valeur(arbre))->caractere;
            for (i = 0; i < nb_bits; i++) {
                decompression[i] = caractere;
            }
        }
        else {
            nd courant = arbre;
            i = 0;
            for (int j = 0; j < nb_bits; j++) {
                if ((texte[j / 8] & (1 << (7 - j % 8))) == 0) {
                    courant = get_gauche(courant);
                }
                else {
                    courant = get_droit(courant);
                }
                
                if (est_feuille(courant)) {
                    decompression[i] = ((v_huffman *) get_valeur(courant))->caractere;
                    i += 1;
                    courant = arbre;
                }
            }
        }
        decompression[i] = '\0';
    }
    
    return decompression;
}

nd creer_arbre_huffman(char * texte) {
    nd arbre = NULL;
    
    if (texte != NULL) {
        int * frequences = calloc(MAX_CHAR, sizeof(int));
        int n = calculer_frequences(texte, frequences);
        nd * liste_arbres = creer_arbres_minimalistes(frequences, n);
        arbre = generer_arbre_huffman(liste_arbres, n);
        remplir_codes_arbre_huffman(arbre);
        
        free(frequences);
        free(liste_arbres);
    }
    
    return arbre;
}

int calculer_frequences(char * texte, int * frequences) {
    int n = 0;
    
    if (texte != NULL && frequences != NULL) {
        size_t len = strlen(texte);
        for (size_t i = 0; i < len; i++) {
            if (frequences[(int) texte[i]] == 0) {
                n += 1;
            }
            frequences[(int) texte[i]] += 1;
        }
    }
    
    return n;
}

nd * creer_arbres_minimalistes(int * frequences, int n) {
    nd * liste_arbres = NULL;
    
    if (frequences != NULL && n > 0) {
        liste_arbres = (nd *) malloc(n * sizeof(nd));
        if (liste_arbres != NULL) {
            int a = 0;
            for (int i = 0; i < MAX_CHAR && a < n; i++) {
                if (frequences[i] != 0) {
                    v_huffman valeur;
                    valeur.caractere = i;
                    valeur.frequence = frequences[i];
                    valeur.code = 0;
                    valeur.taille_code = 0;
                    liste_arbres[a] = creer_arbre(&valeur, allouer_valeur_huffman);
                    a += 1;
                }
            }
        }
    }
    
    return liste_arbres;
}

nd fusionner_arbres(nd a1, nd a2) {
    nd fusion = NULL;
    
    if (a1 != NULL && a2 != NULL) {
        v_huffman valeur;
        valeur.caractere = 0;
        valeur.frequence = ((v_huffman *) get_valeur(a1))->frequence + ((v_huffman *) get_valeur(a2))->frequence;
        valeur.code = 0;
        valeur.taille_code = 0;
        fusion = creer_arbre(&valeur, allouer_valeur_huffman);
        lier_noeud_gauche(fusion, a1);
        lier_noeud_droit(fusion, a2);
    }
    
    return fusion;
}

nd generer_arbre_huffman(nd * liste_arbres, int n) {
    nd arbre = NULL;
    
    if (liste_arbres != NULL) {
        if (n == 1) {
            arbre = liste_arbres[0];
            liste_arbres[0] = NULL;
        }
        else if (n >= 2) {
            for (int len = n; len >= 2; len--) {
                int a1 = 0;
                int a2 = 1;
                int freq1 = ((v_huffman *) get_valeur(liste_arbres[a1]))->frequence;
                int freq2 = ((v_huffman *) get_valeur(liste_arbres[a2]))->frequence;
                
                if (freq1 > freq2) {
                    int tmp;
                    tmp = freq1;
                    freq1 = freq2;
                    freq2 = tmp;
                    
                    a1 = 1;
                    a2 = 0;
                }
                
                for (int i = 2; i < len; i++) {
                    int frequence = ((v_huffman *) get_valeur(liste_arbres[i]))->frequence;
                    if (frequence < freq2) {
                        if (frequence < freq1) {
                            freq2 = freq1;
                            freq1 = frequence;
                            a2 = a1;
                            a1 = i;
                        }
                        else {
                            freq2 = frequence;
                            a2 = i;
                        }
                    }
                }
                
                arbre = fusionner_arbres(liste_arbres[a1], liste_arbres[a2]);
                liste_arbres[a1] = arbre;
                liste_arbres[a2] = liste_arbres[len - 1];
                liste_arbres[len - 1] = NULL;
            }
        }
    }
    
    return arbre;
}

void remplir_codes_arbre_huffman(nd arbre) {
    if (arbre != NULL) {
        if (est_feuille(arbre)) {
            ((v_huffman *) get_valeur(arbre))->code = 0;
            ((v_huffman *) get_valeur(arbre))->taille_code = 1;
        }
        else {
            remplir_codes_arbre_huffman_recursif(arbre, 0, 0);
        }
    }
}

void remplir_codes_arbre_huffman_recursif(nd arbre, int code, int taille_code) {
    if (arbre != NULL) {
        if (est_feuille(arbre)) {
            ((v_huffman *) get_valeur(arbre))->code = code;
            ((v_huffman *) get_valeur(arbre))->taille_code = taille_code;
        }
        else {
            if (get_gauche(arbre) != NULL) {
                remplir_codes_arbre_huffman_recursif(get_gauche(arbre), code << 1, taille_code + 1);
            }
            if (get_droit(arbre) != NULL) {
                remplir_codes_arbre_huffman_recursif(get_droit(arbre), (code << 1) + 1, taille_code + 1);
            }
        }
    }
}

void * allouer_valeur_huffman(void * valeur) {
    void * bloc = NULL;
    
    if (valeur != NULL) {
        bloc = malloc(sizeof(v_huffman));
        if (bloc != NULL) {
            memcpy(bloc, valeur, sizeof(v_huffman));
        }
    }
    
    return bloc;
}

void liberer_valeur_huffman(void ** valeur) {
    if (valeur != NULL && *valeur != NULL) {
        free(*valeur);
        *valeur = NULL;
    }
}

void afficher_valeur_huffman(void * valeur) {
    if (valeur != NULL) {
        if (((v_huffman *) valeur)->taille_code > 0) {
            printf("(%c ", ((v_huffman *) valeur)->caractere);
            for (int i = 1 << (((v_huffman *) valeur)->taille_code - 1); i > 0; i >>= 1) {
                printf("%c", (((v_huffman *) valeur)->code & i) ? '1' : '0');
            }
            printf(") ");
        }
    }
}

int comparer_caracteres(void * v1, void * v2) {
    int res = 0;
    
    if (v1 != NULL && v2 != NULL) {
        res = ((v_huffman *) v1)->caractere == ((v_huffman *) v2)->caractere;
    }
    
    return res;
}
