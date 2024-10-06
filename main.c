#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <assert.h>
#include <string.h>
#include <time.h>

#define True 1
#define False 0

#define TAILLE 100




struct liste{
    int *liste_entier;
    int taille;
};
typedef struct liste liste;

struct Stat{
    int nb_compare;
    int nb_permutation;
    int taille_liste;
};
typedef struct Stat stat;

struct Stat_groupe{
    char *nom;
    stat **stats;
    int taille;
};
typedef struct Stat_groupe stat_groupe;

struct stat_tris{
    int taille;
    stat_groupe **groupes;
};
typedef struct stat_tris stat_tris;

struct tas{
    int *l;
    int p;
};
typedef struct tas tas;

void quicksort(liste *l, int start, int end, stat *s); 
int leonardo_numbers(int n);
void heapify(liste *l, int start, int end, stat *s);



int alea(int from, int to){
    return rand()%(to+1 - from) + from;
}

liste *liste_entier(int taille){
    liste *l = calloc(1, sizeof(liste));
    if (!l) {
        fprintf(stderr, "Erreur d'allocation memoire pour la liste\n");
        exit(EXIT_FAILURE);
    }
    int *liste = calloc(taille, sizeof(int));
    if (!liste) {
        fprintf(stderr, "Erreur d'allocation memoire pour la liste d'entiers\n");
        exit(EXIT_FAILURE);
    }

    for(int i = 0; i < taille; i++){
        liste[i] = alea(1,100-1);
    }
    l->liste_entier = liste;
    l->taille = taille;
    return l;
}

void freel(liste *l){
    if(l == NULL) return;
    free(l->liste_entier);
    free(l);
}

liste* copier_liste(liste *original) {
    assert(original != NULL);
    liste *copie = calloc(1, sizeof(liste));
    if (!copie) {
        fprintf(stderr, "Erreur d'allocation memoire pour la liste\n");
        exit(EXIT_FAILURE);
    }
    copie->liste_entier = calloc(original->taille, sizeof(int));
    if (!copie->liste_entier) {
        fprintf(stderr, "Erreur d'allocation memoire pour la liste d'entiers\n");
        exit(EXIT_FAILURE);
    }
    copie->taille = original->taille;
    memcpy(copie->liste_entier, original->liste_entier, original->taille * sizeof(int));
    return copie;
}

char *copy_string(char *str){
    int taille = 0;
    while(str[taille] != '\0'){
        taille++;
    }
    char *nstr = calloc(taille+1, sizeof(char));
    int i = 0;
    while(i < taille+1){
        nstr[i] = str[i];
        i++;
    }
    return nstr;
}

void afficher_liste(liste *l){
    printf("[ ");
    for(int i = 0; i < l->taille-1; i++){
        printf("%d, ", l->liste_entier[i]);
    }

    printf("%d ]\n", l->liste_entier[l->taille-1]);
}

int est_trie(liste *l){
    for(int i = 0; i < l->taille-1; i++){
        if(l->liste_entier[i] > l->liste_entier[i+1]) return False;
    }

    return True;
}

void swap(int *a, int *b){
    int temp = *a;
    *a = *b;
    *b = temp;
}

stat *init_stat(){
    stat *s = calloc(1, sizeof(stat));
    if (!s) {
        fprintf(stderr, "Erreur d'allocation memoire pour la stat\n");
        exit(EXIT_FAILURE);
    }

    s->nb_compare = 0;
    s->nb_permutation = 0;
    s->taille_liste = 0;
    return s;
}

liste *presque_triee(int taille){
    liste *l = liste_entier(taille);

    int nb_permute = 0.05 * taille; // jusqu'a 5% non trie
    if(nb_permute <= 0) nb_permute = 1;

    for(int i = 0; i < nb_permute; i++){
        swap(&(l->liste_entier[alea(0, l->taille)]), &(l->liste_entier[alea(0, l->taille)]));
    }
    return l;
}

liste *triee(int taille){
    liste *l = liste_entier(taille);
    stat *s = init_stat();
    quicksort(l, 0, l->taille, s);
    free(s);
    return l;
}

liste *reverse(int taille){
    liste *l = liste_entier(taille);
    stat *s = init_stat();
    quicksort(l, 0, l->taille, s);
    free(s);

    for(int i = 0; i < l->taille / 2; i++){
        swap(&(l->liste_entier[i]), &(l->liste_entier[l->taille-1-i]));
    }

    return l;
}

liste *bcp_doublons(int taille){
    liste *l = liste_entier(taille);

    int nb_doublons = taille*0.8; // jusqu'a 80% de doublons
    if(nb_doublons <= 0) nb_doublons = 1;

    for(int i = 0; i < nb_doublons; i++){
        l->liste_entier[l->taille-1-i] = l->liste_entier[i];
    }

    return l;
}

void tri_par_selection(liste *l, stat *s){
    int st = 0;
    int end = l->taille;

    while(st != end){
        int val_mini = l->liste_entier[st];
        int index_mini = st;
        for(int i = st; i < end; i++){
            s->nb_compare += 1;
            if(l->liste_entier[i] < val_mini){
                val_mini = l->liste_entier[i];
                index_mini = i;
            }
        }

        // ECHANGE
        s->nb_permutation++;
        swap(&l->liste_entier[st], &l->liste_entier[index_mini]);

        st++;
    }
}

void tri_par_insertion(liste *l, stat *s){
    int t = 1;

    while(t < l->taille){
        
        int val = l->liste_entier[t];
        int pos = 0;

        s->nb_compare += 2;
        while(pos <= t && l->liste_entier[pos] < val){
            pos++;
        }
        s->nb_permutation++;
        swap(&l->liste_entier[pos], &val);
        pos++;
        while(pos <= t){
            s->nb_permutation++;
            swap(&l->liste_entier[pos], &val);
            pos++;
        }
        t++;
    }
}

void tri_bubble(liste *l, stat *s){
    int permute = False;
    int end = l->taille-1;
    for(int i = end; i > 0; i--){
        for(int j = 0; j < i; j++){
            s->nb_compare++;
            if(l->liste_entier[j] > l->liste_entier[j+1]){
                s->nb_permutation++;
                swap(&l->liste_entier[j], &l->liste_entier[j+1]); 
                permute = True;
            }
        }   
        if(!permute) break;
    }

}

liste *tri_fusion(liste *l, stat *s){
    if(l->taille == 0){
        liste *nl = calloc(1, sizeof(liste));
        if (!nl) {
            fprintf(stderr, "Erreur d'allocation memoire pour la liste\n");
            exit(EXIT_FAILURE);
        }
        nl->taille = 0;
        return nl;
    }

    if(l->taille == 1){
        liste *nl = calloc(1, sizeof(liste));
        if (!nl) {
            fprintf(stderr, "Erreur d'allocation memoire pour la liste\n");
            exit(EXIT_FAILURE);
        }
        nl->taille = 1;
        int *t = calloc(2, sizeof(int));
        if (!t) {
            fprintf(stderr, "Erreur d'allocation memoire pour la liste d'entiers\n");
            exit(EXIT_FAILURE);
        }
        nl->liste_entier = t;
        nl->liste_entier[0] = l->liste_entier[0];
        return nl;
    }

    // int oldtaille = l->taille;
    // l->taille /= 2;
    // liste *nl1 = tri_fusion(l);

    // l->liste_entier += l->taille;
    // l->taille = oldtaille - l->taille;

    // liste *nl2 = tri_fusion(l);

    int mid = l->taille / 2;

    liste *left = calloc(1, sizeof(liste));
    if (!left) {
        fprintf(stderr, "Erreur d'allocation memoire pour la liste gauche (fusion)\n");
        exit(EXIT_FAILURE);
    }
    left->taille = mid;
    left->liste_entier = calloc(mid, sizeof(int));
    if (!left->liste_entier) {
        fprintf(stderr, "Erreur d'allocation memoire pour la liste gauche (fusion) d'entiers\n");
        exit(EXIT_FAILURE);
    }
    memcpy(left->liste_entier, l->liste_entier, mid * sizeof(int));

    liste *right = calloc(1, sizeof(liste));
    if (!right) {
        fprintf(stderr, "Erreur d'allocation memoire pour la liste droite (fusion)\n");
        exit(EXIT_FAILURE);
    }
    right->taille = l->taille - mid;
    right->liste_entier = calloc(right->taille, sizeof(int));
    if (!right->liste_entier) {
        fprintf(stderr, "Erreur d'allocation memoire pour la liste droite (fusion) d'entiers\n");
        exit(EXIT_FAILURE);
    }
    memcpy(right->liste_entier, l->liste_entier + mid, right->taille * sizeof(int));

    liste *nl1 = tri_fusion(left, s);
    liste *nl2 = tri_fusion(right, s);

    liste *nlfinal = calloc(1, sizeof(liste));
    if (!nlfinal) {
        fprintf(stderr, "Erreur d'allocation memoire pour la liste\n");
        exit(EXIT_FAILURE);
    }
    nlfinal->taille = nl1->taille + nl2->taille;
    nlfinal->liste_entier = calloc(nlfinal->taille, sizeof(int));
    if (!nlfinal->liste_entier) {
        fprintf(stderr, "Erreur d'allocation memoire pour la liste d'entiers\n");
        exit(EXIT_FAILURE);
    }
    
    int i = 0;
    int j = 0;
    int c = 0;

    // Si on considre qu'il y a permutation seulement si c'est un élément de droite qui est mit avant celui de gauche
    // Sinon il faut retirer les //
    while(i < nl1->taille && j < nl2->taille){
        s->nb_compare++;
        if(nl1->liste_entier[i] < nl2->liste_entier[j]){
            // s->nb_permutation++;
            nlfinal->liste_entier[c] = nl1->liste_entier[i];
            i++;
        }
        else{
            s->nb_permutation++;
            nlfinal->liste_entier[c] = nl2->liste_entier[j];
            j++;
        }
        c++;
    }

    while(i < nl1->taille){
        // s->nb_permutation++;
        nlfinal->liste_entier[c] = nl1->liste_entier[i];
        i++;
        c++;
    }

    while(j < nl2->taille){
        // s->nb_permutation++;
        nlfinal->liste_entier[c] = nl2->liste_entier[j];
        j++;
        c++;
    }
    free(left->liste_entier);
    free(left);
    free(right->liste_entier);
    free(right);
    free(nl1->liste_entier);
    free(nl1);
    free(nl2->liste_entier);
    free(nl2);
    return nlfinal;
}

void w_tri_fusion(liste *l, stat *s){
    liste *lres = tri_fusion(l, s);
    freel(lres);
}

void ajouter_valeur(tas *t, int x, stat *s){
    t->l[t->p] = x;
    t->p++;

    int current_p = t->p - 1;
    while(current_p != 0){
        s->nb_compare++;
        if(t->l[current_p] > t->l[(current_p-1)/2]){
            s->nb_permutation++;
            swap(&(t->l[current_p]), &(t->l[(current_p-1)/ 2]));
            current_p = (current_p-1)/2;
        }else{
            break;
        }
    }
}

int enleverMaxTass(tas *s, stat *st){
    int i,j;

    int max = s->l[0];

    s->l[0] = s->l[s->p-1];
    s->p--;

    i = 0;
    int fin = False;

    while(i < s->p/2 && !fin){
        st->nb_compare++;
        if((2*i+1 == s->p-1) || s->l[2*i+1] > s->l[2*i+2]) j = 2*i+1; else j = 2*i+2;
        fin = s->l[i] > s->l[j] ? True : False;
        
        if(!fin){
            st->nb_permutation++;
            swap(&s->l[i], &s->l[j]);
            i=j;
        }

    }

    return max;

}

liste *tri_par_tas(liste *l, stat *s){
    tas *t = calloc(1, sizeof(tas));
    if (!t) {
        fprintf(stderr, "Erreur d'allocation memoire pour le tas \n");
        exit(EXIT_FAILURE);
    }
    t->p = 0;
    t->l = calloc(l->taille, sizeof(int));
    if (!t->l) {
        fprintf(stderr, "Erreur d'allocation memoire pour la liste d'entiers (tas)\n");
        exit(EXIT_FAILURE);
    }

    for(int i = 0; i < l->taille; i++){
        ajouter_valeur(t, l->liste_entier[i], s);
    }

    liste *l2 = calloc(1, sizeof(liste));
    if (!l2) {
        fprintf(stderr, "Erreur d'allocation memoire pour la liste\n");
        exit(EXIT_FAILURE);
    }
    l2->taille = l->taille;
    l2->liste_entier = calloc(l2->taille, sizeof(int));
    if (!l2->liste_entier) {
        fprintf(stderr, "Erreur d'allocation memoire pour la liste d'entiers\n");
        exit(EXIT_FAILURE);
    }

    for(int i = 0; i < l2->taille; i++){
        l2->liste_entier[l2->taille-1-i] = enleverMaxTass(t, s);
    }

    free(t->l);
    free(t);
    return l2;
}

void w_tri_par_tas(liste *l, stat *s){
    liste *lres = tri_par_tas(l, s);
    freel(lres);
}

int partitionner(liste *l, int start, int end, int pivot, stat *s){
    s->nb_permutation++;
    swap(&(l->liste_entier[pivot]), &(l->liste_entier[end]));
    int j = start;

    for(int i = start; i < end; i++){
        s->nb_compare++;
        if(l->liste_entier[i] <= l->liste_entier[end]){
            s->nb_permutation++;
            swap(&(l->liste_entier[i]), &(l->liste_entier[j]));
            j++;
        }
    }
    s->nb_permutation++;
    swap(&(l->liste_entier[end]), &(l->liste_entier[j]));
    return j;
} 

void quicksort(liste *l, int start, int end, stat *s){
    if(start >= end) return;

    int pivot = alea(start, end); // choix du pivot aleatoire
    pivot = partitionner(l, start, end, pivot, s);
    quicksort(l, start, pivot-1, s);
    quicksort(l, pivot+1, end, s);
}

void w_quicksort(liste *l, stat *s){
    quicksort(l, 0, l->taille-1, s);
}

void combsort(liste *l, stat *s){
    int intervalle = l->taille;

    int echange = True;
    while(intervalle > 1 || echange == True){
        intervalle = intervalle/1.3;
        if(intervalle < 1){
            intervalle = 1;
        }

        int i = 0;
        echange = False;
        while(i < (l->taille - intervalle)){
            s->nb_compare++;
            if(l->liste_entier[i] > l->liste_entier[i + intervalle]){
                swap(&l->liste_entier[i], &l->liste_entier[i + intervalle]);
                s->nb_permutation++;
                echange = True;
            }
            i++;
        }
    }
}


// VARIABLES POUR SHELL SORT ! A NE PAS SUPPRIMER
int gaps[] = {701, 301, 132, 57, 23, 10, 4, 1};
int taille_gaps = 8;

void mi_shellsort_ou(liste *l, stat *s){
    int n = l->taille;

    for(int k = 0; k < taille_gaps; k++){
        int m = gaps[k];

        for(int r = 0; r < m; r++){
            for(int i = r+m; i < n; i += m){
                int j = i;
                int x = l->liste_entier[i];
                while(j > r && l->liste_entier[j-m] > x){
                    s->nb_compare++;
                    s->nb_permutation++;
                    l->liste_entier[j] = l->liste_entier[j-m];
                    j -= m;
                }
                l->liste_entier[j] = x;
            }
        }
    }
}

// Implementation provenant de : https://www.geeksforgeeks.org/introduction-to-smooth-sort/ Adaptée de la version python3
// Tri par tas avec arbres de Leonardo
// https://youtu.be/FntVy6lPVyo?si=AOsEEUIU6ghckYe1&t=1149

int leonardo_numbers(int n) {
    if(n <= 1) return 1;
    return leonardo_numbers(n-1) + leonardo_numbers(n-2) + 1;
}

// Reorganise la liste pour respecter la structure d'un tas
void heapify(liste *l, int start, int end, stat *s) {
    int i = start;
    int j = 0;     // pos dans l'arbre
    int k = 0;

    while(k < end - start + 1) {
        if(k & 0xAAAAAAAA) { // ????
            j++;
            i >>= 1;
        } else {
            i += j;
            j >>= 1;
        }
        k++;
    }

    while(i > 0) {
        j >>= 1;
        k = i + j; // Descend dans le tas

        while(k < end) {
            s->nb_compare++;
            if(l->liste_entier[k] > l->liste_entier[k-i]) { // respecte la structure d'un tas ?
                break;
            }
            // Sinon on swap
            swap(&l->liste_entier[k], &l->liste_entier[k-i]);
            k += i;
        }
        i = j;
    }
}

void smoothsort(liste *l, stat *s) {
    int n = l->taille;
    int p = n-1;
    int q = p;
    int r = 0;

    while(p > 0) {
        
        if((r & 0x03) == 0) { // r%4 == 0
            heapify(l, r, q, s);  // On transforme la partie de la liste en tas
        }

        int lnr = leonardo_numbers(r);

        if(lnr == p) {
            r++;
        } else {
            r--;
            q = q - lnr;
            heapify(l, r, q, s);
            q = r - 1;
            r++;
        }

        // Meme fonctionnement que le tri par tas
        swap(&l->liste_entier[0], &l->liste_entier[p]);
        s->nb_permutation++;
        p--;
    }

    // Tri par insertion
    for(int i = 0; i < n-1; i++) {
        int j = i+1;
        while(j > 0 && l->liste_entier[j] < l->liste_entier[j-1]) {
            s->nb_compare++;
            s->nb_permutation++;
            swap(&l->liste_entier[j], &l->liste_entier[j-1]);
            j--;
        }
    }
}



void affichage_csv(stat_tris *les_tris){
    int k = 0;
    int nb_tris = les_tris->taille;

    int est_nul = False;
    int taille_differente = False;

    printf("TAILLE LISTE");
    for(int i = 0; i < nb_tris; i++){
        printf(",%s", les_tris->groupes[i]->nom);
    }
    printf("\n");

    while(k < les_tris->groupes[0]->taille){

        for(int i = 0; i < nb_tris; i++){ // VERIFIE QUE LE STATS SUIVANT POUR CHAQUE GRP N'est pas nul
            if(les_tris->groupes[i]->stats == NULL || les_tris->groupes[i]->stats[k] == NULL){
                est_nul = True;
                break;
            } 
        }
        if(est_nul){
            break;
        }

        int taille_liste = les_tris->groupes[0]->stats[k]->taille_liste;
        for(int i = 0; i < nb_tris; i++){ // VERIFIE QUE TOUTES LES TAILLES DE LA LIGNE SONT IDENTIQUES
            if(les_tris->groupes[i]->stats[k]->taille_liste != taille_liste){
                taille_differente = True;
                break;
            }
        }
        if(taille_differente){
            break;
        }

        printf("%d", taille_liste);
        for(int i = 0; i < nb_tris; i++){
            printf(",%d", les_tris->groupes[i]->stats[k]->nb_compare + les_tris->groupes[i]->stats[k]->nb_permutation);
        }
        printf("\n");

        k++;
    }
}

void ecriture_csv(stat_tris *les_tris){
    FILE *fichier = fopen("listes_stats.csv", "w");


    int k = 0;
    int nb_tris = les_tris->taille;

    int est_nul = False;
    int taille_differente = False;

    fprintf(fichier, "TAILLE LISTE");
    for(int i = 0; i < nb_tris; i++){
        fprintf(fichier, ",%s", les_tris->groupes[i]->nom); // HERE
    }
    fprintf(fichier, "\n");

    while(k < les_tris->groupes[0]->taille){

        for(int i = 0; i < nb_tris; i++){ // VERIFIE QUE LE STATS SUIVANT POUR CHAQUE GRP N'est pas nul
            if(les_tris->groupes[i]->stats == NULL || les_tris->groupes[i]->stats[k] == NULL){
                est_nul = True;
                break;
            } 
        }
        if(est_nul){
            break;
        }

        int taille_liste = les_tris->groupes[0]->stats[k]->taille_liste;
        for(int i = 0; i < nb_tris; i++){ // VERIFIE QUE TOUTES LES TAILLES DE LA LIGNE SONT IDENTIQUES
            if(les_tris->groupes[i]->stats[k]->taille_liste != taille_liste){
                taille_differente = True;
                break;
            }
        }
        if(taille_differente){
            break;
        }

        fprintf(fichier, "%d", taille_liste);
        for(int i = 0; i < nb_tris; i++){
            fprintf(fichier, ",%d", les_tris->groupes[i]->stats[k]->nb_compare + les_tris->groupes[i]->stats[k]->nb_permutation);
        }
        fprintf(fichier, "\n");

        k++;
    }
    fclose(fichier);
}

void affichage_gnu(stat_tris *les_tris){
    int k = 0;
    int nb_tris = les_tris->taille;

    int est_nul = False;
    int taille_differente = False;

    printf("#TAILLE LISTE");
    for(int i = 0; i < nb_tris; i++){
        printf(",%s", les_tris->groupes[i]->nom);
    }
    printf("\n");

    while(k < les_tris->groupes[0]->taille){

        for(int i = 0; i < nb_tris; i++){ // VERIFIE QUE LE STATS SUIVANT POUR CHAQUE GRP N'est pas nul
            if(les_tris->groupes[i]->stats[k] == NULL){
                est_nul = True;
                break;
            } 
        }
        if(est_nul){
            break;
        }

        int taille_liste = les_tris->groupes[0]->stats[k]->taille_liste;
        for(int i = 0; i < nb_tris; i++){ // VERIFIE QUE TOUTES LES TAILLES DE LA LIGNE SONT IDENTIQUES
            if(les_tris->groupes[i]->stats[k]->taille_liste != taille_liste){
                taille_differente = True;
                break;
            }
        }
        if(taille_differente){
            break;
        }

        printf("%d", taille_liste);
        for(int i = 0; i < nb_tris; i++){
            printf("\t%d", les_tris->groupes[i]->stats[k]->nb_compare + les_tris->groupes[i]->stats[k]->nb_permutation);
        }
        printf("\n");

        k++;
    }
}

void ecriture_gnu(stat_tris *les_tris){
    
    FILE *fichier = fopen("resultats.dat", "w");

    int k = 0;
    int nb_tris = les_tris->taille;

    int est_nul = False;
    int taille_differente = False;

    fprintf(fichier, "#TAILLE LISTE");
    for(int i = 0; i < nb_tris; i++){
        fprintf(fichier, ",%s", les_tris->groupes[i]->nom);
    }
    fprintf(fichier, "\n");

    while(k < les_tris->groupes[0]->taille){

        for(int i = 0; i < nb_tris; i++){ // VERIFIE QUE LE STATS SUIVANT POUR CHAQUE GRP N'est pas nul
            if(les_tris->groupes[i]->stats[k] == NULL){
                est_nul = True;
                break;
            } 
        }
        if(est_nul){
            break;
        }

        int taille_liste = les_tris->groupes[0]->stats[k]->taille_liste;
        for(int i = 0; i < nb_tris; i++){ // VERIFIE QUE TOUTES LES TAILLES DE LA LIGNE SONT IDENTIQUES
            if(les_tris->groupes[i]->stats[k]->taille_liste != taille_liste){
                taille_differente = True;
                break;
            }
        }
        if(taille_differente){
            break;
        }

        fprintf(fichier, "%d", taille_liste);
        for(int i = 0; i < nb_tris; i++){
            fprintf(fichier, "\t%d", les_tris->groupes[i]->stats[k]->nb_compare + les_tris->groupes[i]->stats[k]->nb_permutation);
        }
        fprintf(fichier, "\n");

        k++;
    }
    fclose(fichier);
}

void affichage_config_gnu(stat_tris *les_tris){
    printf("# pour avoir un titre sur le diagramme\n"\
    "set title 'Comparaison des algorithmes de tris'\n"\
    "# pour nommer l'axe des abscisses\n"\
    "set xlabel 'Taille de la liste'\n"\
    "# pour nommer l'axe des ordonnées\n"\
    "set ylabel 'Nombre de comparaisons'\n"\
    "# la première courbe est tracée par plot\n"\
    "# en utilisant la colonne 1 pour les abscisses et la colonne 2 pour les ordonnées\n");

    int nb_tris = les_tris->taille;
    if(nb_tris >= 1){
        printf("plot 'resultats.dat' using 1:2 title '%s' with linespoints\n", les_tris->groupes[0]->nom);
    }
    if(nb_tris > 1){
        printf("# les courbes suivantes sont dessinées par l'instruction replot\n");
        for(int i = 1; i < nb_tris; i++){
            printf("replot 'resultats.dat' using 1:%d title '%s' with linespoints\n", i+2, les_tris->groupes[i]->nom);
        }
    }

    printf("# l'instruction suivante attend que l'utilisateur appuie sur une touche pour quitter l'application\n"\
    "pause -1\n");
    return;
}

void ecriture_config_gnu(stat_tris *les_tris){
    FILE *fichier = fopen("resultats.gnu", "w");

    fprintf(fichier, "# pour avoir un titre sur le diagramme\n"\
    "set title 'Comparaison des algorithmes de tris'\n"\
    "# pour nommer l'axe des abscisses\n"\
    "set xlabel 'Taille de la liste'\n"\
    "# pour nommer l'axe des ordonnées\n"\
    "set ylabel 'Nombre doperations '\n"\
    "# la première courbe est tracée par plot\n"\
    "# en utilisant la colonne 1 pour les abscisses et la colonne 2 pour les ordonnées\n");

    int nb_tris = les_tris->taille;
    if(nb_tris >= 1){
        fprintf(fichier, "plot 'resultats.dat' using 1:2 title '%s' with linespoints\n", les_tris->groupes[0]->nom);
    }
    if(nb_tris > 1){
        fprintf(fichier, "# les courbes suivantes sont dessinées par l'instruction replot\n");
        for(int i = 1; i < nb_tris; i++){
            fprintf(fichier, "replot 'resultats.dat' using 1:%d title '%s' with linespoints\n", i+2, les_tris->groupes[i]->nom);
        }
    }

    fprintf(fichier, "# l'instruction suivante attend que l'utilisateur appuie sur une touche pour quitter l'application\n"\
    "pause -1\n");
    fclose(fichier);
    return;
}

void test(int taille_mini, int nb_listes, int nb_expes, liste *(*generateur) (int),int nb_sort_func, void (*func_ptr[])(liste *l, stat *s), char **liste_nom){

    stat ***les_tris = calloc(nb_sort_func, sizeof(stat *));

    if(!les_tris){
        fprintf(stderr, "Erreur d'allocation memoire pour la liste de stats des tris\n");
        exit(EXIT_FAILURE);
    }

    stat_tris *sttris = calloc(1, sizeof(stat_tris));
    if (!sttris) {
        fprintf(stderr, "Erreur d'allocation memoire pour la liste des stats des tris\n");
        exit(EXIT_FAILURE);
    }
    sttris->taille = nb_sort_func;

    sttris->groupes = calloc(nb_sort_func, sizeof(stat_groupe *));
    if (!sttris->groupes) {
        fprintf(stderr, "Erreur d'allocation memoire pour la liste des groupes\n");
        exit(EXIT_FAILURE);
    }

    for(int i = 0; i < nb_sort_func; i++){
        stat **letri = calloc(nb_listes * nb_expes, sizeof(stat *));
        if (!letri) {
            fprintf(stderr, "Erreur d'allocation memoire pour la liste de stats %s\n", liste_nom[i]);
            exit(EXIT_FAILURE);
        }
        for(int j = 0; j < nb_expes * nb_listes; j++){
            letri[j] = init_stat();
        }
        les_tris[i] = letri;

        stat_groupe *grp = calloc(1, sizeof(stat_groupe));
        if (!grp) {
            fprintf(stderr, "Erreur d'allocation memoire pour le groupe de stat %s\n", liste_nom[i]);
            exit(EXIT_FAILURE);
        }
        grp->nom = liste_nom[i];
        grp->stats = les_tris[i];
        grp->taille = nb_listes * nb_expes;
        sttris->groupes[i] = grp;
    }



    int index = 0;
    for(int j = 1; j <= nb_listes; j++){
        for(int i = 0; i < nb_expes; i++){
            liste *l = generateur(j*taille_mini);

            liste *lc = NULL;
            for(int k = 0; k < nb_sort_func; k++){
                lc = copier_liste(l);
                les_tris[k][index]->taille_liste = lc->taille;
                func_ptr[k](lc, les_tris[k][index]);
                freel(lc);
            }

            index++;
            freel(l);
        }
        
    }
    
    // affichage_csv(sttris);
    // affichage_gnu(sttris);
    // config_gnu(sttris);

    ecriture_csv(sttris);
    ecriture_gnu(sttris);
    ecriture_config_gnu(sttris);

    for(int k = 0; k < nb_sort_func; k++){
        for(int i = 0; i < nb_expes * nb_listes; i++){
            free(les_tris[k][i]);
        }
        free(les_tris[k]);
        free(sttris->groupes[k]);
    }
    
    free(les_tris);
    free(sttris->groupes);
    free(sttris);
}

int main(int argc, char **argv){
    // srand(1969);
    srand(time(NULL));
    int choix = 0;
    if(argc >= 2){
        choix = atoi(argv[1]);
    }

    int nb_func = 9;
    void (*func_ptr[])(liste *l, stat *s) = {tri_par_selection, tri_par_insertion, tri_bubble, w_tri_fusion, w_tri_par_tas, w_quicksort, combsort, mi_shellsort_ou, smoothsort};
    char *noms[] = {"Tri par Selection", "Tri par Insertion", "Tri à Bulle", "Tri Fusion", "Tri par tas", "Quicksort", "Tri à Peigne", "Tri de Shell", "Smoothsort"};
    int taille_mini = 100;
    int nb_listes = 10;
    int nb_expes = 20;

    switch (choix) {
        case 1:
            test(taille_mini, nb_listes, nb_expes, liste_entier, nb_func, func_ptr, noms);
            break;
        case 2:
            test(taille_mini, nb_listes, nb_expes, presque_triee, nb_func, func_ptr, noms);
            break;
        case 3:
            test(taille_mini, nb_listes, nb_expes, triee, nb_func, func_ptr, noms);
            break;
        case 4:
            test(taille_mini, nb_listes, nb_expes, bcp_doublons, nb_func, func_ptr, noms);
            break;
        case 5:
            test(taille_mini, nb_listes, nb_expes, reverse, nb_func, func_ptr, noms);
            break;
        default:
            test(taille_mini, nb_listes, nb_expes, liste_entier, nb_func, func_ptr, noms);
            break;
    }


    return EXIT_SUCCESS;
}