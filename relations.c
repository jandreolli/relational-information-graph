// relations.c: definit le point d'entree pour l'application.
// ANDREOLLI Justine - L2S4P SDA2 - TD3/TP6 - 2023

typedef int bool;
#define false 0
#define true -1

#include "stdlib.h"
#include "memory.h"
#include "stdio.h"
#include "string.h"

////////////////////////////////////////
// Exercice 1: Classement des Relations

typedef enum {
	FRERE = 2, COUSIN, PARENT, ONCLE, EPOUX, AMI, VIT, CONNAIT,
	CHEF, COLLEGUE, LOCATAIRE, TRAVAILLE, PROPRIETAIRE, SITUE, DECOUVERT
} rtype;

bool est_lien_parente(rtype id)
{
	return (id >= FRERE && id <= EPOUX)? true : false;
}
bool est_lien_professionel(rtype id)
{
	return (id == CHEF || id == COLLEGUE)? true : false;
}
bool est_lien_connaissance(rtype id)
{
	return (id >= AMI && id <= CONNAIT)? true : false;
}
char* toStringRelation(rtype id)
{
	if(id < 1 || id > DECOUVERT){
		fprintf(stderr,"fatal error: Cette relation n'existe pas: %d\n", id);
		exit(EXIT_FAILURE);
	}
	char* tab[] = {"","relation inconnue","frère ou soeur de","cousin ou cousine de",
	"parent de", "oncle ou tante de", "époux ou épouse de", "ami de", "vit à",
	"connait", "chef de", "collègue de", "locataire de", "travaille à",
	"propriétaire de", "situé à", "découvert de"};

	return tab[id];
}

////////////////////////////////////////
// Exercice 2: Liste de pointeurs

typedef struct s_node {
	void *val;  // pointeur vers objet quelconque
	struct s_node *suiv;
} *listeg;

listeg listegnouv() { return NULL; }
listeg adjtete(listeg lst, void *x)
{
	listeg nv = (listeg)malloc(sizeof(struct s_node));

	if (nv == NULL){
		fprintf(stderr,"fatal error: Problème lors de la création du nouveau node\n");
		exit(EXIT_FAILURE);
	}
 	nv->val = x;
 	nv->suiv = lst;
	return nv;
}
bool estvide(listeg lst)
{
	return (lst == NULL)? true : false ;
}
listeg adjqueue(listeg lst, void *x)
{
	if (estvide(lst))
	{
	    return adjtete(lst, x);
	}

	listeg nv = (listeg)malloc(sizeof(struct s_node));
	if (nv == NULL){
		fprintf(stderr,"fatal error: Problème lors de la création du nouveau node\n");
		exit(EXIT_FAILURE);
	}
	nv->val = x;
	nv->suiv = NULL;

	listeg current = lst;
	while (!estvide(current->suiv))
	{
	    current = current->suiv;
	}
	current->suiv = nv;
	return lst;
}
listeg suptete(listeg lst)
{
	if (estvide(lst))
    {
       	fprintf(stderr,"fatal error: lst est NULL\n");
		exit(EXIT_FAILURE);
	}

    listeg newhead = lst->suiv;
    free(lst);

    return newhead;
}
void *tete(listeg lst)
{
	if (estvide(lst))
    {
        return NULL;
    }
    return lst->val;
}
int longueur(listeg lst)
{
	if (estvide(lst)){
		return 0;
	}
	return 1 + longueur(lst->suiv);
}
void detruire(listeg lst)
{
	while (!estvide(lst)){
		lst = suptete(lst);
	}
}
listeg rech(listeg lst, void *x, int(*comp)(void *, void *))
{
	listeg nouv = listegnouv();
    while (!estvide(lst))
    {
        if (comp(lst->val, x) == 0)
        {
            nouv = adjtete(nouv, lst->val);
        }
        lst = lst->suiv;
    }
    return nouv;
}

////////////////////////////////////////
// Exercice 3: Construction du graphe

#define LONG_NOM_MAX 64
typedef enum { PERSONNE=1, OBJET, ADRESSE, VILLE } etype;
typedef struct s_entite {
	char nom[LONG_NOM_MAX]; // le nom de l�entit� p.ex � Peugeot 106 �
	etype ident; // l�identifiant associ�, p.ex OBJET
} *Entite;

//3.1 les structures de donn�es
typedef struct s_sommet {
	// A DEFINIR
	listeg larcs;
	struct s_entite *x;
} *Sommet;

typedef struct s_arc {
	// A DEFINIR
	rtype t;
	struct s_entite *x;
} *Arc;

typedef struct s_relations {
	// A DEFINIR
	listeg lsommet;
} *Relations;

//3.2 les constructeurs
Entite creerEntite(char *s, etype e)
{
	if(strlen(s) >= LONG_NOM_MAX){
		fprintf(stderr, "fatal error: Le nom choisi est trop long\n");
		exit(EXIT_FAILURE);
	}
	Entite nv = (Entite)malloc(sizeof(struct s_entite));
	if(nv == NULL){
		fprintf(stderr, "fatal error: Entite est NULL\n");
		exit(EXIT_FAILURE);
	}
	strncpy(nv->nom, s, LONG_NOM_MAX);
	nv->ident = e;
	return nv;
}
Sommet nouvSommet(Entite e)
{
	Sommet s = (Sommet)malloc(sizeof(struct s_sommet));
	if(s == NULL){
		fprintf(stderr, "fatal error: Sommet est NULL\n");
		exit(EXIT_FAILURE);
	}
	s->larcs = listegnouv();
	s->x = e;
	return s;
}
Arc nouvArc(Entite e, rtype type)
{
	Arc a = (Arc)malloc(sizeof(struct s_arc));
	if(a == NULL){
		fprintf(stderr, "fatal error: Arc est NULL\n");
		exit(EXIT_FAILURE);
	}
	a->t = type;
	a->x = e;
	return a;
}
void relationInit(Relations *g)
{
	*g = (Relations)malloc(sizeof(struct s_relations));
	if (*g == NULL) {
		fprintf(stderr, "fatal error: Relations est NULL\n");
		exit(EXIT_FAILURE);
	}
	(*g)->lsommet = listegnouv();
}

void relationFree(Relations *g)
{
	listeg lst = (*g)->lsommet;

	while(lst != NULL){
		listeg arcs = ((Sommet)lst->val)->larcs;
		while(arcs != NULL){
			free(arcs->val); // libère l'arc 
			((Sommet)lst->val)->larcs = arcs->suiv; // liste pointe vers l'arc suivant;
			listeg tmp = arcs;
			arcs =  arcs->suiv;
			free(tmp); // libère node vers l'arc
		}

		free(((Sommet)lst->val)->x); // libère entité du sommet
		free(lst->val); // libère le sommet
		listeg tmp = lst;
		lst = lst->suiv; // pointe vers le sommet suivant
		free(tmp);
	}
	free(*g);
}

//3.3 les comparaisons
int compEntite(void *e, void *string)
{
	return strcmp(((Entite)e)->nom, (char*)string);
}
int compSommet(void *s, void *string)
{
	return strcmp(((Sommet)s)->x->nom, (char*)string);
}
int compArc(void *a, void *string)
{
	return strcmp(((Arc)a)->x->nom, (char*)string);
}
int compString(void* s, void* string){
	return strcmp((char*)s, (char*)string);
}

//3.4 ajout d'entites et de relations
void adjEntite(Relations g, char *nom, etype t)
{
	listeg checkExist = rech(g->lsommet, (char*)nom, compSommet);
	if (estvide(checkExist))
	{
		Entite e = creerEntite(nom, t);
		Sommet s = nouvSommet(e);
		g->lsommet = adjtete(g->lsommet, (Sommet)s); // ajoute nouveau sommet en tête de liste
	}
	else{
		fprintf(stderr, "Cette entité existe déjà\n");
		free(checkExist);
	}
}
// PRE CONDITION: id doit �tre coh�rent avec les types des sommets correspondants � x et y
//                p.ex si x est de type OBJET, id ne peut pas etre une relation de parente
// PRE CONDITION: strcmp(nom1,nom2)!=0
void adjRelation(Relations g, char *nom1, char *nom2, rtype id)
{
	if(strcmp(nom1, nom2) != 0){
		Sommet s1 = NULL;
		Sommet s2 = NULL;
		listeg s_courant = g->lsommet;
		// cherche les sommets qui possèdent comme entité nom1 et nom2
		while(s_courant != NULL){
			if(compSommet(((Sommet)s_courant->val), nom1) == 0){
				s1 = s_courant->val;
			}
			if(compSommet(((Sommet)s_courant->val), nom2) == 0){
				s2 = s_courant->val;
			}
			s_courant = s_courant->suiv;
		}
		
		if (s1 == NULL || s2 == NULL){
			fprintf(stderr, "fatal error: s1 ou s2 est NULL\n");
			exit(EXIT_FAILURE);
		}

		// Vérifie si une relation existe déjà entre les 2 sommets
		listeg a1 = rech(s1->larcs, (char *)nom2, compArc);
		listeg a2 = rech(s2->larcs, (char *)nom1, compArc);
		Arc arcExiste = tete(a1);
		Arc arcExiste2 = tete(a2);

		if(a1 != NULL && a2 != NULL){
			arcExiste->t = id;
			arcExiste2->t = id;
			free(a1);
			free(a2);
		}
		else{
			Arc nv = nouvArc(s2->x, id);
			s1->larcs = adjtete(s1->larcs, nv);
			Arc nv2 = nouvArc(s1->x, id);
			s2->larcs = adjtete(s2->larcs, nv2);
		}
	}
	else{
		fprintf(stderr, "Les deux sommets sont les mêmes\n");
	}
}

////////////////////////////////////////
// Exercice 4: Explorer les relations entre personnes

// 4.1 listes de relations
listeg en_relation(Relations g, char *x)
{
	listeg rel = rech(g->lsommet, (char *)x, compSommet);
	Sommet s = tete(rel);
	detruire(rel);
	return s == NULL ? NULL : s->larcs;
}
listeg chemin2(Relations g, char *x, char *y)
{
	listeg result = listegnouv();

	listeg x_arcs = en_relation(g, x); // liste larcs du sommet x
	listeg y_arcs = en_relation(g, y);

	while(!estvide(x_arcs)){
		y_arcs = en_relation(g, y);
		while(!estvide(y_arcs)){
			if (strcmp(((Arc)x_arcs->val)->x->nom, ((Arc)y_arcs->val)->x->nom) == 0 &&
			strcmp(((Arc)x_arcs->val)->x->nom, y) != 0 && strcmp(((Arc)y_arcs->val)->x->nom, x) != 0)
			{
				result = adjtete(result, ((Arc)x_arcs->val)->x);
			}
			y_arcs = y_arcs->suiv;
		}
		x_arcs = x_arcs->suiv;
	}
	return result;
}
// 4.2 verifier un lien de parente
// PRE CONDITION: strcmp(x,y)!=0
bool ont_lien_parente(Relations g, char *x, char *y)
{
	listeg lst = en_relation(g, x);
	while(!estvide(lst)){
		if(strcmp(((Arc)lst->val)->x->nom, y) == 0)
		{
			return est_lien_parente(((Arc)lst->val)->t);
		}
		lst = lst->suiv;
	}
	return false;
}

// 4.3 tester connaissances
// PRE CONDITION: les sommets correspondants � x et y sont de type PERSONNE
// PRE CONDITION: strcmp(x,y)!=0
bool se_connaissent(Relations g, char *x, char *y)
{
	listeg lst = en_relation(g, x);
	while(!estvide(lst)){
		if(strcmp(((Arc)lst->val)->x->nom, y) == 0)
		{
			if(est_lien_professionel(((Arc)lst->val)->t) ||
			est_lien_connaissance(((Arc)lst->val)->t) || est_lien_parente(((Arc)lst->val)->t)){
				return true;
			}
		}
		lst = lst->suiv;
	}

	listeg x_arcs = en_relation(g, x);
	listeg y_arcs = en_relation(g, y);

	while(!estvide(x_arcs)){
		y_arcs = en_relation(g, y);
		while(!estvide(y_arcs)){
			if (strcmp(((Arc)x_arcs->val)->x->nom, ((Arc)y_arcs->val)->x->nom) == 0 &&
				strcmp(((Arc)x_arcs->val)->x->nom, y) != 0 &&
				strcmp(((Arc)y_arcs->val)->x->nom, x) != 0 &&
				est_lien_parente(((Arc)x_arcs->val)->t) &&
				est_lien_parente(((Arc)y_arcs->val)->t)
				)
			{
				return true;
			}
			y_arcs = y_arcs->suiv;
		}
		x_arcs = x_arcs->suiv;
	}
	return false;
}
// PRE CONDITION: les sommets correspondants � x et y sont de type PERSONNE
// PRE CONDITION: strcmp(x,y)!=0
bool se_connaissent_proba(Relations g, char *x, char *y)
{
	Sommet s1 = NULL;
	Sommet s2 = NULL;
	listeg s_courant = g->lsommet;
	while(!estvide(s_courant)){
		if(compSommet(((Sommet)s_courant->val), (char *)x) == 0){
			s1 = s_courant->val;
		}
		if(compSommet(((Sommet)s_courant->val), (char *)y) == 0){
			s2 = s_courant->val;
		}
		s_courant = s_courant->suiv;
	}
	
	if (s1 == NULL || s2 == NULL){
		fprintf(stderr, "fatal error: s1 ou s2 est NULL\n");
		exit(EXIT_FAILURE);
	}

	if(s1->x->ident != PERSONNE || s2->x->ident != PERSONNE){
		return false;
	}
	else if(se_connaissent(g, x, y)){
		return false;
	}
	else{
	listeg x_arcs = en_relation(g, x);
	listeg y_arcs = en_relation(g, y);

	while(!estvide(x_arcs)){
		y_arcs = en_relation(g, y);
		while(!estvide(y_arcs)){
			if (strcmp(((Arc)x_arcs->val)->x->nom, ((Arc)y_arcs->val)->x->nom) == 0 &&
				strcmp(((Arc)x_arcs->val)->x->nom, y) != 0 &&
				strcmp(((Arc)y_arcs->val)->x->nom, x) != 0 &&
				((Arc)x_arcs->val)->x->ident == PERSONNE &&
				((Arc)y_arcs->val)->x->ident == PERSONNE &&
				(est_lien_parente(((Arc)x_arcs->val)->t)^
				est_lien_parente(((Arc)y_arcs->val)->t))
				)
			{
				return true;
			}
			y_arcs = y_arcs->suiv;
		}
		x_arcs = x_arcs->suiv;
	}
	return false;
	}
}
// PRE CONDITION: les sommets correspondants � x et y sont de type PERSONNE
// PRE CONDITION: strcmp(x,y)!=0
bool se_connaissent_peutetre(Relations g, char *x, char *y)
{
	Sommet s1 = NULL;
	Sommet s2 = NULL;
	listeg s_courant = g->lsommet;
	while(!estvide(s_courant)){
		if(compSommet(((Sommet)s_courant->val), (char *)x) == 0){
			s1 = s_courant->val;
		}
		if(compSommet(((Sommet)s_courant->val), (char *)y) == 0){
			s2 = s_courant->val;
		}
		s_courant = s_courant->suiv;
	}
	
	if (s1 == NULL || s2 == NULL){
		fprintf(stderr, "fatal error: s1 ou s2 est NULL\n");
		exit(EXIT_FAILURE);
	}

	if(s1->x->ident != PERSONNE || s2->x->ident != PERSONNE){
		return false;
	}
	else if(se_connaissent(g, x, y) || se_connaissent_proba(g, x, y)){
		return false;
	}
	else{
	listeg x_arcs = en_relation(g, x);
	listeg y_arcs = en_relation(g, y);

	while(!estvide(x_arcs)){
		y_arcs = en_relation(g, y);
		while(!estvide(y_arcs)){
			if (strcmp(((Arc)x_arcs->val)->x->nom, ((Arc)y_arcs->val)->x->nom) == 0 &&
				strcmp(((Arc)x_arcs->val)->x->nom, y) != 0 &&
				strcmp(((Arc)y_arcs->val)->x->nom, x) != 0 &&
				((Arc)x_arcs->val)->x->ident == PERSONNE &&
				((Arc)y_arcs->val)->x->ident == PERSONNE)
			{
				return true;
			}
			y_arcs = y_arcs->suiv;
		}
		x_arcs = x_arcs->suiv;
	}
	return false;
	}
}

////////////////////////////////////////
// Exercice 5: Affichages

void affichelg(listeg l, void(*aff)(void *))
{
	while (!estvide(l))
    {
        aff(((Arc)l->val));
        l = l->suiv;
    }
}

void afficheEntite(void *x)
{
	printf("%s:", ((Entite)x)->nom);
	switch(((Entite)x)->ident){
		case PERSONNE:
			printf("personne\n");
			break;
		case OBJET:
			printf("objet\n");
			break;
		case ADRESSE:
			printf("adresse\n");
			break;
		case VILLE:
			printf("ville\n");
			break;
		default:
			printf("erreur: non valide\n");
	}
}
void afficheArc(void *x)
{
	printf("--%s-->", toStringRelation(((Arc)x)->t));
	afficheEntite(((Arc)x)->x);
}
////////////////////////////////////////
// Exercice 6: Parcours
void affiche_degre_relations(Relations r, char *x)
{
	listeg r_alreadyDisplayed = listegnouv(); // liste qui contient les char* déjà affichés
	listeg currentRelations = listegnouv();
	listeg x_relations = en_relation(r, x); // liste des arcs de x (larcs)
	// recréé un node vers chaque arc pour que currentRelations et nextRelations se free pareillement
	while(!estvide(x_relations)){
		currentRelations = adjtete(currentRelations, x_relations->val);
		x_relations = x_relations->suiv;
	}
	r_alreadyDisplayed = adjtete(r_alreadyDisplayed, x); // liste des noms déjà affichés

	printf("%s\n", x);
	int deg = 1;
	while(!estvide(currentRelations)){
		printf("-- %d\n", deg);
		listeg nextRelations = listegnouv();
		while(!estvide(currentRelations)){
			listeg isAlreadyDisplayed1 = rech(r_alreadyDisplayed, ((Arc)currentRelations->val)->x->nom, compString);
			if(estvide(isAlreadyDisplayed1)){
				printf("%s\n", ((Arc)currentRelations->val)->x->nom);
				// ajoute le nom affiché à la liste des noms déjà affichés
				r_alreadyDisplayed = adjtete(r_alreadyDisplayed, ((Arc)currentRelations->val)->x->nom);
				// deg_relation = liste larcs du nouveau nom affiché
				listeg deg_relation = en_relation(r, ((Arc)currentRelations->val)->x->nom);
				while(!estvide(deg_relation)){
					listeg isAlreadyDisplayed2 = rech(r_alreadyDisplayed, ((Arc)deg_relation->val)->x->nom, compString);
					if(estvide(isAlreadyDisplayed2)){
						// ajoute un arc de larcs du nouveau nom affiché dans la liste nextRelations
						nextRelations = adjtete(nextRelations, (Arc)deg_relation->val);
					}
					if(!estvide(isAlreadyDisplayed2)){free(isAlreadyDisplayed2);}
					deg_relation = deg_relation->suiv;
				}
			}
			if(!estvide(isAlreadyDisplayed1)){free(isAlreadyDisplayed1);}
			// libère le node vers l'arc créé dès qu'il n'est plus utilisé et passe au suivant
			currentRelations = suptete(currentRelations);
		}
		deg++;
		currentRelations = nextRelations;
	}
	if(!estvide(r_alreadyDisplayed)){detruire(r_alreadyDisplayed);}
}


int main()
{
	int i,j;
	Relations r; relationInit(&r);
	// ajouter les entites de l'exemple
	char *tabe[] = {"KARL","LUDOVIC","CELINE","CHLOE","GILDAS","CEDRIC","SEVERINE",
		"PEUGEOT 106" ,"1, RUE DE LA RUE","STRASBOURG" };
	for (i = 0; i < 7; i++) adjEntite(r, tabe[i], PERSONNE);
	adjEntite(r, tabe[7], OBJET);
	adjEntite(r, tabe[8], ADRESSE);
	adjEntite(r, tabe[9], VILLE);
	// ajouter les relations de l'exemple
	adjRelation(r, tabe[0], tabe[1], FRERE);
	adjRelation(r, tabe[0], tabe[2], AMI);
	adjRelation(r, tabe[0], tabe[3], CONNAIT);
	adjRelation(r, tabe[0], tabe[5], COUSIN);
	adjRelation(r, tabe[0], tabe[7], PROPRIETAIRE);
	adjRelation(r, tabe[0], tabe[8], PROPRIETAIRE);
	adjRelation(r, tabe[3], tabe[4], VIT);
	adjRelation(r, tabe[5], tabe[6], EPOUX);
	adjRelation(r, tabe[5], tabe[8], LOCATAIRE);
	adjRelation(r, tabe[7], tabe[8], DECOUVERT);
	adjRelation(r, tabe[8], tabe[9], SITUE);

	// explorer les relations
	printf("%s est en relation avec:\n", tabe[0]);
	affichelg(en_relation(r, tabe[0]),afficheArc);
	printf("\n");

	for (i = 0; i < 7; i++) for (j = i + 1; j < 10; j++)
	{
		printf("<%s> et <%s> ont les relations communes:\n", tabe[i], tabe[j]);
		listeg ch = chemin2(r, tabe[i], tabe[j]);
		affichelg(ch, afficheEntite);
		printf("\n");
		detruire(ch);
	}
	printf("\n\n");

	for (i = 0; i < 10; i++) for (j = i + 1; j < 10; j++)
	{
		printf("<%s> et <%s> ont lien de parente: %s\n",
			tabe[i], tabe[j], ont_lien_parente(r, tabe[i], tabe[j]) ? "vrai" : "faux");
	}
	printf("\n");
	for (i = 0; i < 7; i++)
	{
		for (j = i + 1; j < 7; j++)
		{
			printf("<%s> et <%s> se connaissent: %s\n",
				tabe[i], tabe[j], se_connaissent(r, tabe[i], tabe[j]) ? "vrai" : "faux");
			printf("<%s> et <%s> se connaissent tres probablement: %s\n",
				tabe[i], tabe[j], se_connaissent_proba(r, tabe[i], tabe[j]) ? "vrai" : "faux");
			printf("<%s> et <%s> se connaissent peut etre: %s\n",
				tabe[i], tabe[j], se_connaissent_peutetre(r, tabe[i], tabe[j]) ? "vrai" : "faux");
		}
		printf("\n");
	}

	affiche_degre_relations(r, tabe[3]);

	relationFree(&r);

	printf("\nPRESS RETURN\n");
	char buff[64]; fscanf(stdin, "%s", buff);
    return 0;
}
