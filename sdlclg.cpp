///////////////////////////////////////////////////////////////////////////////
// sdlclg.cpp
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
#include <windows.h>
#endif
#include <vector>
//#include <iostream>
#include "sdlclg.h"

SDL_Surface *ecran = 0;
vector<SDL_Surface*> Images;

/*inline void VerifierErreur(bool Test, string Msg)
{
	if(Test)
	{
#ifdef _WIN32
		MessageBoxA(0, Msg.c_str(), "Erreur", MB_OK | MB_ICONERROR);
#else
        cout << "Erreur: " << Msg << endl;
#endif
		exit(1);
	}

}*/
void Attendre(int Temps)
{
	VerifierErreur(Temps<=0,"Delay: Invalid time");
	SDL_Delay(Temps);
}
void InitialiserAffichage(string Titre, int Largeur, int Hauteur)
{
	VerifierErreur(Largeur<=0,"InitialiserAffichage: Largeur invalide");
	VerifierErreur(Hauteur<=0,"InitialiserAffichage: Hauteur invalide");

	SDL_Init(SDL_INIT_VIDEO);
	ecran = SDL_SetVideoMode(Largeur, Hauteur, 32, SDL_HWSURFACE);

	VerifierErreur(!ecran, "InitialiserAffichage: Impossible de charger le mode vidéo");
	SDL_WM_SetCaption(Titre.c_str(), 0);

	SDL_EnableKeyRepeat(100, 100);
}

void RemplirFenetre(int Rouge, int Vert, int Bleu)
{
	VerifierErreur(Rouge<0 || Rouge>255, "RemplirFenetre: Couleur rouge invalide");
	VerifierErreur(Vert<0 || Vert>255, "RemplirFenetre: Couleur verte invalide");
	VerifierErreur(Bleu<0 || Bleu>255, "RemplirFenetre: Couleur bleue invalide");

	SDL_FillRect(ecran, 0, SDL_MapRGB(ecran->format, Rouge, Vert, Bleu));
}

void DessinerRectangle(int PosX, int PosY, int Largeur, int Hauteur,
					   int Rouge, int Vert, int Bleu)
{
	VerifierErreur(PosX<0, "DessinerRectangle: PosX invalide");
	VerifierErreur(PosY<0, "DessinerRectangle: PosY invalide");
	VerifierErreur(Largeur<0, "DessinerRectangle: Largeur invalide");
	VerifierErreur(Hauteur<0, "DessinerRectangle: Hauteur invalide");
	VerifierErreur(Rouge<0 || Rouge>255, "DessinerRectangle: Couleur rouge invalide");
	VerifierErreur(Vert<0 || Vert>255, "DessinerRectangle: Couleur verte invalide");
	VerifierErreur(Bleu<0 || Bleu>255, "DessinerRectangle: Couleur bleue invalide");

	SDL_Surface *rectangle = 0;
	SDL_Rect position;
	rectangle = SDL_CreateRGBSurface(SDL_HWSURFACE, Largeur, Hauteur, 32, 0, 0, 0, 0);
	position.x = PosX;
	position.y = PosY;
	SDL_FillRect(rectangle, NULL, SDL_MapRGB(ecran->format, Rouge, Vert, Bleu));
	// Collage de la surface sur l'écran
	SDL_BlitSurface(rectangle, NULL, ecran, &position);
	SDL_FreeSurface(rectangle); // Libération de la surface
}
ImageId ChargerImage(string NomFichier)
{
	SDL_Surface* Image = SDL_LoadBMP(NomFichier.c_str());

	VerifierErreur(!Image, string("ChargerImage: Impossible de charger l'image ")+ NomFichier);

	Images.push_back(Image);
	return Images.size()-1;
}
void AfficherImage(ImageId Image, int PosX, int PosY)
{
	VerifierErreur(Image<0 || static_cast<unsigned int>((Image)>Images.size()-1), "Numéro d'image invalide");
	VerifierErreur(PosX<0, "AfficherImage: PosX invalide");
	VerifierErreur(PosY<0, "AfficherImage: PosY invalide");

	SDL_Rect position;
	position.x = PosX;
	position.y = PosY;
	SDL_SetColorKey(Images[Image], SDL_SRCCOLORKEY,
		SDL_MapRGB(Images[Image]->format, 0, 0, 0));
	SDL_BlitSurface(Images[Image], NULL, ecran, &position);
}
void RafraichirFenetre()
{
	SDL_Flip(ecran);
}
void Screenshot()
{
	SDL_SaveBMP(ecran, "dump.bmp");
}
void QuitterAffichage()
{
	//SDL_SaveBMP(ecran, "dump.bmp");
	for(unsigned int i=0;i<Images.size();i++)
	{
		SDL_FreeSurface(Images[i]);
	}
	SDL_Quit();
}

/*Evenement AttendreEvenement()
{
	Evenement e = EVAucun;
	SDL_Event event;
	bool Valide =false;
	while(!Valide)
	{
		while (SDL_WaitEvent(&event))
		{
            switch(event.type)
            {
            case SDL_QUIT:
                e = EVQuitter;
                Valide=true;
                break;
            case SDL_KEYDOWN:
                switch(event.key.keysym.sym)
                {
                case SDLK_UP: // Flèche haut
                    e = EVHaut;
                    Valide=true;
                    break;
                case SDLK_DOWN: // Flèche bas
                    e = EVBas;
                    Valide=true;
                    break;
                case SDLK_RIGHT: // Flèche droite
                    e = EVDroite;
                    Valide=true;
                    break;
                case SDLK_LEFT: // Flèche gauche
                    e =  EVGauche;
                    Valide=true;
                    break;
                case SDLK_ESCAPE: // Appui sur la touche Echap, on arrête le programme
                    e = EVQuitter;
                    Valide=true;
                    break;
                default:    // Shut up compiler warnings
                    break;
                }
                break;
            }
		}
	}
	return e;
}*/

Evenement LireEvenement()
{
	Evenement e = EVAucun;
	SDL_Event event;

	while (SDL_PollEvent(&event))
	{
        switch(event.type)
        {
        case SDL_QUIT:
            e = EVQuitter;
            break;
        case SDL_KEYDOWN:
            switch(event.key.keysym.sym)
            {
            case SDLK_UP: // Flèche haut
                e = EVHaut;
                break;
            case SDLK_DOWN: // Flèche bas
                e = EVBas;
                break;
            case SDLK_RIGHT: // Flèche droite
                e = EVDroite;
                break;
            case SDLK_LEFT: // Flèche gauche
                e =  EVGauche;
                break;
            case SDLK_SPACE: // Barre d'Espace
                e = EVEspace;
                break;
            case SDLK_c: // Lettre C
                e = EV_C;
                break;
            case SDLK_r: // Lettre R
                e = EV_R;
                break;
            case SDLK_F3: // F3
                e = EV_F3;
                break;
            case SDLK_PAGEUP: // Page up
                e = EV_PageUp;
                break;
            case SDLK_PAGEDOWN: // Page down
                e = EV_PageDown;
                break;
            case SDLK_END: // End
                e = EV_End;
                break;
            case SDLK_LCTRL: // Control gauche
                e =  EVControl;
                break;
            case SDLK_ESCAPE: // Appui sur la touche Echap, on arrête le programme
                e = EVQuitter;
                break;
            default:    // Shut up compiler warnings
                break;
            }
            break;
        }
	}

	return e;
}
