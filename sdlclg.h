#ifndef _SDLCLG_H
#define _SDLCLG_H
///////////////////////////////////////////////////////////////////////////////
// sdlclg.h
// Interface de programmation graphique pour le cours de KA0 utilisation la
// la libraire SDL 1.2.14-VC8
// Joan-Sébastien Morales et Stéphane Chassé
// Version 1.0 Création 25 octobre 2009
// Version 1.1 Modification 27 octobre 2009
//   -- Ajout de messages d'erreurs
//   -- Ajout de la fonction RafraichirFenetre
// Version 1.2 Modification 17 novembre 2009
//   -- Ajout du type ImageId
//   -- Ajout de l'énumération Evenement
//   -- Ajout de la fonction AttendreEvenement
// Version 1.21 Modification 24 novembre 2009
//   -- Correction d'un bug avec la fonction AttendreEvenement
// Version 1.22 Modification 12 novembre 2013
//   -- Ajout de la fonction non-bloquante LireEvenement
// Version 1.23 Modification 4 mai 2017
//   -- Correction d'un bug dans LireEvenement dû à SDL_PollEvent
///////////////////////////////////////////////////////////////////////////////
#ifdef _WIN32
#include <SDL.h>
#else
#include <SDL/SDL.h>
#endif
//#include <windows.h>
//#include <vector>
#include <iostream>
#include <string>
using namespace std;

///////////////////////////////////////////////////////////////////////////////
// Type de données utilisé pour conserver les numéros d'images
///////////////////////////////////////////////////////////////////////////////
typedef int ImageId;

///////////////////////////////////////////////////////////////////////////////
// Liste des événements associés au clavier
///////////////////////////////////////////////////////////////////////////////
enum Evenement
{
	EVAucun, EVQuitter,	EVBas, EVHaut, EVGauche, EVDroite, EVControl, EVEspace, EV_C, EV_F3, EV_PageUp, EV_PageDown, EV_End, EV_R
};
///////////////////////////////////////////////////////////////////////////////
// Attendre
// Permet de placer le programme en attente pendant un certain temps
// Intrant: Temps: Nombre de millisecondes
// Extrant: Aucun
///////////////////////////////////////////////////////////////////////////////
void Attendre(int Temps);
///////////////////////////////////////////////////////////////////////////////
// InitialiserAffichage
// Fait apparaître une fenêtre graphique
// Intrant: Titre: Titre de la fenêtre
//			Largeur: Largeur de la fenêtre en pixels
//		    Hauteur: Hauteur de la fenêtre en pixels
// Extrant: Aucun
///////////////////////////////////////////////////////////////////////////////
void InitialiserAffichage(string Titre, int Largeur, int Hauteur);
///////////////////////////////////////////////////////////////////////////////
// RemplirFenetre
// Colore toute la fenêtre d'une certaine couleur
// Intrant: Rouge: Composante rouge de la couleur (0-255)
//          Vert : Composante verte de la couleur (0-255)
//          Bleu : Composante bleue de la couleur (0-255)
// Extrant: Aucun
///////////////////////////////////////////////////////////////////////////////
void RemplirFenetre(int Rouge, int Vert, int Bleu);
///////////////////////////////////////////////////////////////////////////////
// DessinerRectangle
// Permet de dessiner un rectangle d'une certaine couleur dans la fenêtre
// Intrant: PosX: Position horizontale du rectangle.
//          PosY: Position verticale du rectangle.
//          Le point (0,0) est en haut à gauche
//			Largeur: Largeur du rectangle en pixels
//			Hauteur: Hauteur du rectangle en pixels
//          Rouge: Composante rouge de la couleur (0-255)
//          Vert : Composante verte de la couleur (0-255)
//          Bleu : Composante bleue de la couleur (0-255)
// Extrant: Aucun
///////////////////////////////////////////////////////////////////////////////
void DessinerRectangle(int PosX, int PosY, int Largeur, int Hauteur, int Rouge, int Vert, int Bleu);
///////////////////////////////////////////////////////////////////////////////
// ChargerImage
// Permet de charger une image du disque dur vers la mémoire
// Le seul type d'image supporté est le bitmap (BMP)
// Intrant: NomFichier: Nom du fichier image. Le fichier doit être dans le
//                      même répertoire que le projet
// Extrant: un numéro identifiant uniquement l'image
///////////////////////////////////////////////////////////////////////////////
ImageId ChargerImage(string NomFichier);
///////////////////////////////////////////////////////////////////////////////
// AfficherImage
// Permet d'afficher une image à un certain endroit dans le fenêtre graphique
// Intrant: Image: Numéro de l'image
//			PosX: Position horizontale de l'image
//          PosY: Position verticale de l'image
//          Le point (0,0) est en haut à gauche
// Extrant: Aucun
///////////////////////////////////////////////////////////////////////////////
void AfficherImage(ImageId Image, int PosX, int PosY);
///////////////////////////////////////////////////////////////////////////////
// RafraichirFenetre
// Mettre à jour la fenêtre d'affichage pour que les dernières modifications
// soient visibles
// Intrant: Aucun
// Extrant: Aucun
///////////////////////////////////////////////////////////////////////////////
void RafraichirFenetre();
///////////////////////////////////////////////////////////////////////////////
// Screenshot
// Sauver le contenu de la fenêtre
///////////////////////////////////////////////////////////////////////////////
void Screenshot();
///////////////////////////////////////////////////////////////////////////////
// QuitterAffichage
// Ferme la fenêtre graphique
///////////////////////////////////////////////////////////////////////////////
void QuitterAffichage();
///////////////////////////////////////////////////////////////////////////////
// AttendreEvenement
// Attend que l'usager appuie sur une touche du clavier
// Cette fonction est bloquante!!
// Intrant: Aucun
// Extrant: L'événement correspondant à la touche du clavier:
//   EVHaut: Flèche vers le haut
//   EVBas:  Flèche vers le bas
//   EVDroite: Flèche vers la droite
//   EVGauche: Flèche vers la gauche
//   EVQuitter: Esc
///////////////////////////////////////////////////////////////////////////////
Evenement AttendreEvenement();
///////////////////////////////////////////////////////////////////////////////
// LireEvenement
// Vérifie si l'usager a appuyé sur une touche du clavier
// Cette fonction n'est pas bloquante!!
// Intrant: Aucun
// Extrant: L'événement correspondant à la touche du clavier:
//   EVHaut: Flèche vers le haut
//   EVBas:  Flèche vers le bas
//   EVDroite: Flèche vers la droite
//   EVGauche: Flèche vers la gauche
//   EVQuitter: Esc
///////////////////////////////////////////////////////////////////////////////
Evenement LireEvenement();
///////////////////////////////////////////////////////////////////////////////
// VerifierErreur
// Permet d'afficher un message d'erreur
///////////////////////////////////////////////////////////////////////////////
inline void VerifierErreur(bool Test, string Msg)
{
	if(Test)
	{
#ifdef _WIN32
		MessageBoxA(0, Msg.c_str(), "Error", MB_OK | MB_ICONERROR);
#else
        cout << "Error: " << Msg << endl;
#endif
		exit(1);
	}

}
#endif //_SDLCLG_H
