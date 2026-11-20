#include "invaders.h"
#include <iostream>
using namespace std;

position ::position(coord x, coord y):p1(x), p2(y){}
coord position:: x() const
{
    return this->p1;
}
coord position::y() const
{
    return this->p2;
}
void position::setx(coord x)
{
    this->p1 = x;
}
void position::sety(coord y)
{
    this->p2 = y;
}

std::string position:: to_string() const
{
    return "(" +std::to_string(this->p1) +", " + std::to_string(this->p2)+")";
}

bool position:: egale(const position& pos) const
{

    return this->p1 == pos.p1 && this->p2 == pos.p2;
}

bool position:: differente(const position& pos) const
{
    return !this->egale(pos);
}

taille::taille(coord largeur, coord hauteur): hauteur(hauteur), largeur(largeur) {}
coord taille:: h () const
{
    return this->hauteur;
}
coord taille:: w () const
{
    return this->largeur;
}
void taille:: setw(coord w)
{
    this->largeur  = w;
}

objet::objet(position point):point(point)
{

}
position objet:: pos() const
{
    return this->point;
}
void objet:: setposition(const position& p)
{
    this->point = p ;
}


projectile::projectile(position p):objet(p) // taille par defaut
{

}


objet_actif::objet_actif(position p,coord xmin, coord xmax, direction d):objet(p), xmin_(xmin), xmax_(xmax), d(d)
{

}

joueur::joueur(position p, taille t,coord xmin, coord xmax, direction d):objet_actif(p,xmin,xmax,d), t(t)
{

}

invader::invader(position p, coord xmin, coord xmax, direction d):objet_actif(p,xmin,xmax,d) // taille par defaut
{

}

coord objet_actif:: xmin() const
{
    return this->xmin_;
}
coord objet_actif::xmax() const
{
    return this->xmax_;
}
direction objet_actif:: dir()const
{
    return this->d;
}
void objet_actif::setdir(direction d)
{
    this->d = d;
}

taille projectile::tai() const
{
    return taille(1,1);
}
taille joueur::tai() const
{
    return this->t;
}
taille invader::tai() const
{
    return taille(11,8);
}


void joueur::malus()
{
    coord nouvelle = this->t.w();
    if(nouvelle * 2 <=100)
    {
        this->t.setw(nouvelle * 2);
    }
    else
    {
        this->t.setw(100);
    }

}

void joueur::bonus()
{
    coord nouvelle = this->t.w();
    if(nouvelle / 2 >=3)
    {
        this->t.setw(nouvelle / 2);
    }
    else
    {
        this->t.setw(3);

    }

}

bool objet::contientposition(const position& p) const // droite (<) et gauche (<=)
{
    return (p.x() >=this->point.x()  && p.x() <this->point.x() + this->tai().w()) && (p.y() >=this->point.y()  && p.y() <this->point.y() + this->tai().h());
}


char joueur:: typeobjet() const
{
    return 'J';
}
char invader:: typeobjet() const
{
    return 'I';
}
char projectile:: typeobjet() const
{
    return 'P';
}

void objet::sortieflux(ostream& os) const
{
    os << this->typeobjet() << " " <<this->pos().to_string();
}

void projectile::appliquerdeplacement()
{
    coord nouv = this->pos().y() -1 ;
    if (this->pos().y() > 0) this->setposition(position(this->pos().x(), nouv)); // va vers le haut

}


// la largeur est prise en compte a droite
void objet_actif::appliquerdeplacement()
{
    if (this->dir() != direction::stop)
    {
        if (this->dir() == direction::droite) // il faut prendre en considration la largeur parce que on avance avec la largeur qui joue un role aussi
        {
            coord nouv = this->pos().x() + tai().w() + 1 ; // on calcule avec la largeur en compte
            if (nouv <= this->xmax()) this->setposition(position(nouv - tai().w(), this->pos().y())); // va vers la droite (ici on avance la position de x )


        }
        if (this->dir() == direction::gauche) // donc ici x-1 mais on risque d'avoir la situation: x = 0 du coup avec -1 on a une valuer comme 490000 qui satisfait la condition de nou >= xmin donc on va vers le gauche qui est fausse parce que on depasse le bord a gauche  -> la solution est de: tester d'abord si x == xmin
        {
            if (this->pos().x() > this->xmin_)
            {
                // on peut aller vers la gauche
                this->setposition(position(this->pos().x() - 1, this->pos().y()));
            }

        }
    }

}


void invader::appliquerdeplacement()
{
    objet_actif::appliquerdeplacement(); // appel au parent
    // traitement de plus pour invader
    if(this->dir() == direction::gauche && this->pos().x() == this->xmin())
    {
        coord nouv = this->pos().y() + 1 ;
        this->setposition(position(this->pos().x(), nouv)); // va vers le bas
        this->setdir(direction::droite);
    }
    if (this->dir() == direction::droite && this->pos().x() + tai().w() == this->xmax())
    {
        coord nouv = this->pos().y() + 1 ;
        this->setposition(position(this->pos().x(), nouv)); // va vers le bas
        this->setdir(direction::gauche);
    }

}

jeu::jeu() : joueur_(position(144,180),taille(9,8), 10, 310, direction::stop)
{
}


void jeu:: ajouter(const invader& in)
{
    this->liste_invader.push_back(in);

}

bool jeu::ajouter(const projectile& pro)
{
    for (auto ite = this->liste_projectiles.begin(); ite != this->liste_projectiles.end(); ite++)
    {
        if (pro.pos().egale((*ite).pos())) return false; // la taille des projectiles est 1*1 donc il suffit d'appliquer egale au lieu de continet position
    }
    this->liste_projectiles.push_back(pro);
    return true;

}

const ::joueur& jeu:: joueur() const
{
    return this->joueur_;
}

const list <invader>& jeu:: invaders() const
{
    return this->liste_invader;
}
const list <projectile>& jeu:: projectiles() const
{
    return this->liste_projectiles;
}

void jeu::tirjoueur()
{
    // le projectile apparait a la position ou le x est au milieu de joueur + le y est just en dessus de joueur
    coord x = this->joueur_.pos().x() + (this->joueur_.tai().w() / 2) ; // si tu veux avancer vers le milieu d'un joueur, il suffit de partir de la gauche (x) et traverser (+) la moitier de la larguer de l'objet J
    coord y = this->joueur_.pos().y();
    if (y > 0) // pour ne pas depasser le haut de jeu parce que si:  y = 0 et je fais -1 sans tester je risque d'avoir une valeur comme 490000 qui est fausse -> donc je verifier avant
    {
        this->ajouter(projectile(position(x,y-1)));
    }
}

void jeu::directionjoueur(direction d)
{
    this->joueur_.setdir(d);
}

void jeu::afficher(std::ostream& os) const
{
    this->joueur_.sortieflux(os);
    os <<"\n";
    for(auto ite = this->liste_projectiles.begin(); ite != this->liste_projectiles.end(); ite++)
    {
        ite->sortieflux(os);
        os <<"\n";
    }
    for(auto ite = this->liste_invader.begin(); ite != this->liste_invader.end(); ite++)
    {
        ite->sortieflux(os);
        os <<"\n";
    }
}


void jeu:: projectiledisparition()
{
    auto ite = this->liste_projectiles.begin();
    while(ite != this->liste_projectiles.end())
    {
        if (ite->pos().y() == 0) ite = this->liste_projectiles.erase(ite);
        else
        {
            ite++;
        }
    }

}

void jeu:: projectilecollision()
{
    auto ite1 = this->liste_invader.begin();
    while(ite1 != this->liste_invader.end())
    {
        bool touch = false;
        auto ite2 = this->liste_projectiles.begin();  // initialisation ici pour reprednre des le debut de la liste
        while (ite2 != this->liste_projectiles.end())
        {
            if (ite1->contientposition(ite2->pos())) // est ce que il touche each other ou pas
            {
                ite2= this->liste_projectiles.erase(ite2);
                ite1= this->liste_invader.erase(ite1);
                touch = true; // on asupprimer l'invader
                break; // il suffit d'avoir un seul projectile qui touche l'invader
            }
            else
            {
                ite2++;
            }
        }

        if (!touch) // on a pas supprimer l'invader, si on a supprimer l'invader temp != false  donc we skip cette etape et on reprend depuis @ envoye par erase, sinon on incremente au prochain invader
        {
            ite1++;
        }
    }
}



void jeu::tourdejeu()
{
    this->joueur_.appliquerdeplacement();
    for (auto ite = this->liste_invader.begin(); ite != this->liste_invader.end(); ite++)
    {
        ite->appliquerdeplacement();
    }
    for (auto ite = this->liste_projectiles.begin(); ite != this->liste_projectiles.end(); ite++)
    {
        ite->appliquerdeplacement();

    }

    this->projectiledisparition(); // y==0 supprime
    this->projectilecollision();

}


