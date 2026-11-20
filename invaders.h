#ifndef INVADERS_H
#define INVADERS_H
#include<string>
#include<ostream>
#include<list>
using coord =unsigned int;

class position
{
private:
    coord p1;
    coord p2 ;
public :
    position(coord x, coord y);
    coord x() const;
    coord y() const;
    void setx(coord x);
    void sety(coord y);
    std::string to_string() const;
    bool egale(const position& pos) const;
    bool differente(const position& pos) const;

};

class taille
{
private :
    coord hauteur;
    coord largeur;

public:
    taille(coord largeur, coord hauteur);
    coord h () const;
    coord w () const;
    void setw(coord w);
};


enum class direction
{
    stop, droite, gauche
};

class objet // classes abstraire
{
private:
    position point;
public:
    objet(position point);
    position pos() const;
    virtual taille tai() const = 0; // a re-definir dans les classes filles, =0 sert a dire quelle est virtuellment pure donc elle a pas d'implementation dans la classe objet
    void setposition(const position& p);
    bool contientposition(const position& p) const;
    virtual char typeobjet() const =0;
    void sortieflux(std::ostream& os) const;
    virtual void appliquerdeplacement() = 0; // chauqe classe a son propre deplacement
    virtual ~objet() = default;  // des que y'a une methode virtualle, le constructeur virtuelle est obligatoire (objet* o mais qui pointe vers un joueur, sana ca que le destructeur dr objet est appele et pa joueur aussi)
};

class projectile : public objet
{
public:
    projectile(position p);
    taille tai() const override;
    char typeobjet() const override;
    void appliquerdeplacement() override;

};

class objet_actif : public objet // une classe abstraite , elle ne definit pas la focntion virtuelle tai()
{
private:
    direction d;
    coord xmin_;
    coord xmax_;


public:
    objet_actif(position p,coord xmin, coord xmax, direction d);
    coord xmin() const;
    coord xmax() const;
    direction dir()const;
    void setdir(direction d);
    void appliquerdeplacement() override;

};

class joueur : public objet_actif
{
private :
    taille t; // un attribut pas comme les autres parce que (invader et projectiles on tpas une taille variable)
public:
    joueur(position p,taille t, coord xmin, coord xmax, direction d);
    void malus() ;
    void bonus();
    taille tai() const override;
    char typeobjet() const override;

};

class invader : public objet_actif
{
private:
public :
    invader(position p,coord xmin, coord xmax, direction d);
    taille tai() const override;
    char typeobjet() const override;
    void appliquerdeplacement() override;

};

class jeu{
private:
    std::list<invader> liste_invader;
    ::joueur joueur_;
    std::list<projectile> liste_projectiles;
    void projectiledisparition();
    void projectilecollision();
public:
    jeu();
    void ajouter(const invader& in);
    bool ajouter(const projectile& pro);
    const ::joueur& joueur() const;
    const std::list<invader>& invaders() const;
    const std::list<projectile>& projectiles() const;
    void tirjoueur();
    void directionjoueur(direction d);
    void afficher(std::ostream& os) const;
    void tourdejeu();

};



#endif // INVADERS_H