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
    while(i < nl1->taille && j < nl2->taille){
        s->nb_compare++;
        if(nl1->liste_entier[i] < nl2->liste_entier[j]){
            s->nb_permutation++;
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
        s->nb_permutation++;
        nlfinal->liste_entier[c] = nl1->liste_entier[i];
        i++;
        c++;
    }

    while(j < nl2->taille){
        s->nb_permutation++;
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

void ajouter_valeur(tas *t, int x, stat *s){
    t->l[t->p] = x;
    t->p++;

    int current_p = t->p - 1;
    while(current_p != 0){
        s->nb_compare++;
        if(t->l[current_p] > t->l[current_p/2]){
            s->nb_permutation++;
            swap(&(t->l[current_p]), &(t->l[current_p/ 2]));
            current_p = current_p/2;
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

void affichage_csv(stat_tris *les_tris){
    int k = 0;
    int nb_tris = les_tris->taille;

    // printf("NB TRIS : %d\n", nb_tris);

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
        fprintf(fichier, ",%s", les_tris->groupes[i]->nom);
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
}

void affichage_gnu(stat_tris *les_tris){
    int k = 0;
    int nb_tris = les_tris->taille;

    // printf("NB TRIS : %d\n", nb_tris);

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
    return;
}

void test(int taille_mini, int nb_listes, int nb_expes, liste *(*generateur) (int)){

    stat **selections = calloc(nb_listes * nb_expes, sizeof(stat *));
    if (!selections) {
        fprintf(stderr, "Erreur d'allocation memoire pour la liste de stats selections\n");
        exit(EXIT_FAILURE);
    }
    stat **insertions = calloc(nb_listes * nb_expes, sizeof(stat *));
    if (!insertions) {
        fprintf(stderr, "Erreur d'allocation memoire pour la liste de stats insertions\n");
        exit(EXIT_FAILURE);
    }
    stat **bubbles = calloc(nb_listes * nb_expes, sizeof(stat *));
    if (!bubbles) {
        fprintf(stderr, "Erreur d'allocation memoire pour la liste de stats bubbles\n");
        exit(EXIT_FAILURE);
    }
    stat **fusions = calloc(nb_listes * nb_expes, sizeof(stat *));
    if (!fusions) {
        fprintf(stderr, "Erreur d'allocation memoire pour la liste de stats fusions\n");
        exit(EXIT_FAILURE);
    }
    stat **quicksorts = calloc(nb_listes * nb_expes, sizeof(stat *));
    if (!quicksorts) {
        fprintf(stderr, "Erreur d'allocation memoire pour la liste de stats quicksot\n");
        exit(EXIT_FAILURE);
    }
    stat **tass = calloc(nb_listes * nb_expes, sizeof(stat *));
    if (!tass) {
        fprintf(stderr, "Erreur d'allocation memoire pour la liste de stats tas\n");
        exit(EXIT_FAILURE);
    }

    for(int i = 0; i < nb_expes * nb_listes; i++){
        selections[i] = init_stat();
        insertions[i] = init_stat();
        bubbles[i] = init_stat();
        fusions[i] = init_stat();
        quicksorts[i] = init_stat();
        tass[i] = init_stat();
    }


    stat_groupe *sgs = calloc(1, sizeof(stat_groupe));
    if (!sgs) {
        fprintf(stderr, "Erreur d'allocation memoire pour le groupe de stat selection\n");
        exit(EXIT_FAILURE);
    }
    sgs->nom = "Selection";
    sgs->stats = selections;
    sgs->taille = nb_listes * nb_expes;

    stat_groupe *sgi = calloc(1, sizeof(stat_groupe));
    if (!sgi) {
        fprintf(stderr, "Erreur d'allocation memoire pour le groupe de stat insertion\n");
        exit(EXIT_FAILURE);
    }
    sgi->nom = "Insertion";
    sgi->stats = insertions;
    sgi->taille = nb_listes * nb_expes;


    stat_groupe *sgb = calloc(1, sizeof(stat_groupe));
    if (!sgb) {
        fprintf(stderr, "Erreur d'allocation memoire pour le groupe de stat bubbles\n");
        exit(EXIT_FAILURE);
    }
    sgb->nom = "Bubbles";
    sgb->stats = bubbles;
    sgb->taille = nb_listes * nb_expes;


    stat_groupe *sgf = calloc(1, sizeof(stat_groupe));
    if (!sgf) {
        fprintf(stderr, "Erreur d'allocation memoire pour le groupe de stats fuison\n");
        exit(EXIT_FAILURE);
    }
    sgf->nom = "Fusion";
    sgf->stats = fusions;
    sgf->taille = nb_listes * nb_expes;


    stat_groupe *sgq = calloc(1, sizeof(stat_groupe));
    if (!sgq) {
        fprintf(stderr, "Erreur d'allocation memoire pour le groupe de stats quicksorts\n");
        exit(EXIT_FAILURE);
    }
    sgq->nom = "Quicksort";
    sgq->stats = quicksorts;
    sgq->taille = nb_listes * nb_expes;


    stat_groupe *sgt = calloc(1, sizeof(stat_groupe));
    if (!sgt) {
        fprintf(stderr, "Erreur d'allocation memoire pour le groupe de stats Tas\n");
        exit(EXIT_FAILURE);
    }
    sgt->nom = "Tas";
    sgt->stats = tass;
    sgt->taille = nb_listes * nb_expes;


    stat_tris *sttris = calloc(1, sizeof(stat_tris));
    if (!sttris) {
        fprintf(stderr, "Erreur d'allocation memoire pour la liste des stats des tris\n");
        exit(EXIT_FAILURE);
    }

    sttris->groupes = calloc(6, sizeof(stat_groupe *));
    if (!sttris->groupes) {
        fprintf(stderr, "Erreur d'allocation memoire pour la liste des groupes\n");
        exit(EXIT_FAILURE);
    }
    sttris->groupes[0] = sgs;
    sttris->groupes[1] = sgi;
    sttris->groupes[2] = sgb;
    sttris->groupes[3] = sgf;
    sttris->groupes[4] = sgq;
    sttris->groupes[5] = sgt;
    sttris->taille = 6;


    int index = 0;
    for(int j = 1; j <= nb_listes; j++){
        for(int i = 0; i < nb_expes; i++){
            liste *l = generateur(j*taille_mini);

            liste *lc = copier_liste(l);
            selections[index]->taille_liste = lc->taille;
            tri_par_selection(lc, selections[index]);
            freel(lc);


            lc = copier_liste(l);
            insertions[index]->taille_liste = lc->taille;
            tri_par_insertion(lc, insertions[index]);
            freel(lc);

            lc = copier_liste(l);
            bubbles[index]->taille_liste = lc->taille;
            tri_bubble(lc, bubbles[index]);
            freel(lc);

            lc = copier_liste(l);
            fusions[index]->taille_liste = lc->taille;
            freel(tri_fusion(lc, fusions[index]));
            freel(lc);

            lc = copier_liste(l);
            quicksorts[index]->taille_liste = lc->taille;
            quicksort(lc,0, lc->taille-1, quicksorts[index]);
            freel(lc);

            lc = copier_liste(l);
            tass[index]->taille_liste = lc->taille;
            freel(tri_par_tas(lc, tass[index]));
            freel(lc);

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

    for(int i = 0; i < nb_expes * nb_listes; i++){
        free(selections[i]);
        free(insertions[i]);
        free(bubbles[i]);
        free(fusions[i]);
        free(quicksorts[i]);
        free(tass[i]);
    }

    free(selections);
    free(insertions);
    free(bubbles);
    free(fusions);
    free(quicksorts);
    free(tass);

    free(sgs);
    free(sgi);
    free(sgb);
    free(sgf);
    free(sgq);
    free(sgt);

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

    int taille_min = 100;
    int nb_listes = 10;
    int nb_expes = 20;
    
    switch (choix) {
        case 1:
            test(taille_min, nb_listes, nb_expes, liste_entier);
            break;
        case 2:
            test(taille_min, nb_listes, nb_expes, presque_triee);
            break;
        case 3:
            test(taille_min, nb_listes, nb_expes, triee);
            break;
        case 4:
            test(taille_min, nb_listes, nb_expes, bcp_doublons);
            break;
        case 5:
            test(taille_min, nb_listes, nb_expes, reverse);
            break;
        default:
            test(taille_min, nb_listes, nb_expes, liste_entier);
            break;
    }
    
    return EXIT_SUCCESS;
}