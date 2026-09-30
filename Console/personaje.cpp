#include <iostream>
#include <vector>

using namespace std;

// 30-37 negro, rojo, verde, amarillo, azul, magenta, cyan, blanco
// 90-97 brillantes
const string NEGRO = "\033[30m";
const string ROJO = "\033[31m";
const string VERDE = "\033[32m";
const string AMARILLO = "\033[33m";
const string AZUL = "\033[34m";
const string MAGENTA = "\033[35m";
const string CYAN = "\033[36m";
const string BLANCO = "\033[37m";
const string RESET = "\033[0m";

class Personaje
{
private:
    void mensajeError()
    {
        cout << ROJO << "ERROR: Opción no válida" << RESET << endl;
    }

protected:
    string nombre;
    string clase;
    int vida;
    int vidaOriginal;
    int defensa;
    int ataque;
    int energia;
    int costoEnergiaPorHabilidad;
    int getVida() { return vida; }
    int getPorcentajeVida() { return (int)(vida * 100 / vidaOriginal); }
    virtual void descripcionHabilidad() { cout << ROJO << "<<<DESCRIPCIÓN HABILIDAD>>>" << RESET << endl; };
    virtual void activarHabilidad() { cout << ROJO << "<<<ACTIVAR HABILIDAD>>>" << RESET << endl; }
    void recibirDanio(int ataqueRecibido)
    {
        int danio = ataqueRecibido - defensa;
        if (danio <= 0)
        {
            danio = 1;
        }
        vida -= danio;
        if (vida <= 0)
        {
            cout << ROJO << nombre << " ha muerto." << RESET << endl;
        }
    }

public:
    Personaje(string nombre, string clase, int vida, int defensa, int ataque, int energia, int costoEnergiaPorHabilidad)
    {
        this->nombre = nombre;
        this->clase = clase;
        this->vida = vida;
        this->vidaOriginal = this->vida;
        this->defensa = defensa;
        this->ataque = ataque;
        this->energia = energia;
        this->costoEnergiaPorHabilidad = costoEnergiaPorHabilidad;
    }
    bool estaVivo() { return (vida >= 1); }
    void solicitarHabilidad()
    {
        bool repetir;
        do
        {
            repetir = false;
            string respuesta;
            cout << "Habilidad: ";
            descripcionHabilidad();
            cout << "Costo de energía: " << ROJO << costoEnergiaPorHabilidad << RESET << endl;
            cout << "¿Activar habilidad especial? [S/N]";
            cin >> respuesta;
            if (respuesta == "s" || respuesta == "S")
            {
                if (energia >= costoEnergiaPorHabilidad)
                {
                    energia -= costoEnergiaPorHabilidad;
                    activarHabilidad();
                    cout << VERDE << "Habilidad activada." << RESET << endl;
                }
                else
                {
                    cout << ROJO << "Energía insuficiente." << RESET << endl;
                }
            }
            else
            {
                if (respuesta == "n" || respuesta == "N")
                {
                    cout << ROJO << "Habilidad NO activada." << RESET << endl;
                }
                else
                {
                    mensajeError();
                    repetir = true;
                }
            }
        } while (repetir);
        cout << endl;
    }
    void atacar(vector<Personaje*>& jugadores)
    {
        Personaje* personajeDebil = NULL;
        // recorrer lista
        // elegir el personaje con menor vida
        // atacar ese personaje
        for (Personaje* personaje : jugadores)
        {
            // verifico que no sea misma instancia
            if (personaje != this)
            {
                // asigno primer personaje en primera vuelta al vector
                if (personajeDebil == NULL)
                {
                    if (personaje->estaVivo())
                    {
                        personajeDebil = personaje;
                    }
                }
                else
                {
                    // asigno personaje con menor vida al anterior
                    if (personaje->estaVivo())
                    {
                        if (personaje->getVida() < personajeDebil->getVida())
                        {
                            personajeDebil = personaje;
                        }
                    }
                }
            }
        }

        // Realiza ataque
        if (personajeDebil != NULL)
        {
            cout << AZUL << this->nombre << RESET << " ataca a " << AZUL << personajeDebil->nombre << RESET << endl;
            personajeDebil->recibirDanio(ataque);
        }
        else
        {
            cout << ROJO << "ERROR: Atacar=>Personaje no encontrado." << RESET << endl;
        }
    }
    virtual void resetearHabilidad() { cout << ROJO << "<<<RESETEAR HABILIDAD>>>" << RESET << endl; }
    void stats()
    {
        // ID = Instacia
        cout << AZUL << nombre << RESET << " (ID: " << this << ")" << endl;
        cout << "Clase: " << clase << endl;
        cout << "Vida: " << vida << "/" << vidaOriginal << " (" << getPorcentajeVida() << "%) | ";
        cout << "Defensa: " << defensa << " | ";
        cout << "Ataque: " << ataque << " | ";
        cout << "Energía: " << energia << endl;
    }

    virtual ~Personaje() {}; // Destructor
};

class Caballero : public Personaje
{
private:
    int defensaOriginal;

public:
    Caballero(string nombre) : Personaje(nombre, "Caballero", 90, 90, 30, 50, 25) { defensaOriginal = defensa; }
    void descripcionHabilidad() override { cout << "Aumento de Defensa en un 50%. Aplica solamente en este turno." << endl; }
    void activarHabilidad() override
    {
        defensa *= 1.5;
    }
    void resetearHabilidad() override { defensa = defensaOriginal; }
};
class Guerrero : public Personaje
{
private:
    int ataqueOriginal;

public:
    Guerrero(string nombre) : Personaje(nombre, "Guerrero", 65, 60, 95, 90, 12) { ataqueOriginal = ataque; }
    void descripcionHabilidad() override { cout << "Aumemnto del Ataque. Si la Vida está por encima del 50%, el ataque aumenta en un 25%. Si la Vida está por debajo del 50%, el ataque aumenta en un 50%. Aplica solamente en este turno." << endl; };
    void activarHabilidad() override
    {
        if (getPorcentajeVida() > 50)
        {
            ataque *= 1.25;
        }
        else
        {
            ataque *= 1.50;
        }
    }
    void resetearHabilidad() override { ataque = ataqueOriginal; };
};
class Mago : public Personaje
{
private:
    int ataqueOriginal;

public:
    Mago(string nombre) : Personaje(nombre, "Mago", 45, 30, 90, 100, 40) { ataqueOriginal = ataque; }
    void descripcionHabilidad() override { cout << "Aumento de Ataque en un 50%. Aplica solamente en este turno." << endl; }
    void activarHabilidad() override { ataque *= 1.5; }
    void resetearHabilidad() override { ataque = ataqueOriginal; }
};
class Sacerdote : public Personaje
{
public:
    Sacerdote(string nombre) : Personaje(nombre, "Sacerdote", 60, 50, 55, 90, 27) {}
    void descripcionHabilidad() override { cout << "Aumento de Vida en un 20% (en base a su Vida Original)." << endl; }
    void activarHabilidad() override { vida += vidaOriginal * 0.2; }
    void resetearHabilidad() override { /* No requiere reseteo */ }
};
class Arquero : public Personaje
{
private:
    int ataqueOriginal;

public:
    Arquero(string nombre) : Personaje(nombre, "Arquero", 55, 40, 85, 70, 20) { ataqueOriginal = ataque; }
    void descripcionHabilidad() override { cout << "Aumento de Ataque en un 30%. Aplica solamente en este turno." << endl; }
    void activarHabilidad() override { ataque *= 1.3; }
    void resetearHabilidad() override { ataque = ataqueOriginal; }
};

int seleccionCantidadJugadores()
{
    bool repetir;
    int cantidadJugadores;
    do
    {
        repetir = false;
        cout << "Ingrese la cantidad de jugadores (entre 2 y 10):";
        cin >> cantidadJugadores;
        if (cantidadJugadores < 2 || cantidadJugadores > 10)
        {
            cout << ROJO << "ERROR: Opción no válida" << RESET << endl;
            repetir = true;
        }
    } while (repetir);
    return cantidadJugadores;
}
int seleccionDePersonajes(vector<Personaje*>& jugadores, int cantidadJugadores)
{
    for (int i = 0; i < cantidadJugadores; i++)
    {
        bool repetir;
        do
        {
            repetir = false;
            cout << "JUGADOR " << (i + 1) << endl;
            cout << "Seleccione el personaje:" << endl;
            cout << "[1] Caballero" << endl;
            cout << "[2] Guerrero" << endl;
            cout << "[3] Mago" << endl;
            cout << "[4] Sacerdote" << endl;
            cout << "[5] Arquero" << endl;
            cout << "OPCIÓN: ";
            int personajeElegido;
            cin >> personajeElegido;
            if (personajeElegido >= 1 && personajeElegido <= 5)
            {
                cout << "Ingrese el nombre del personaje: ";
                string nombrePersonaje;
                cin >> nombrePersonaje;
                switch (personajeElegido)
                {
                case 1: // Caballero
                    jugadores.push_back(new Caballero(nombrePersonaje));
                    break;
                case 2: // Guerrero
                    jugadores.push_back(new Guerrero(nombrePersonaje));
                    break;
                case 3: // Mago
                    jugadores.push_back(new Mago(nombrePersonaje));
                    break;
                case 4: // Sacerdote
                    jugadores.push_back(new Sacerdote(nombrePersonaje));
                    break;
                case 5: // Arquero
                    jugadores.push_back(new Arquero(nombrePersonaje));
                    break;
                default:
                    cout << ROJO << "ERROR: Opción no válida" << RESET << endl;
                    repetir = true;
                    break;
                }
                cout << VERDE << "Jugador creado." << RESET << endl;
                jugadores[i]->stats();
                cout << endl;
            }
            else
            {
                cout << ROJO << "ERROR: Opción no válida" << RESET << endl;
                repetir = true;
            }

        } while (repetir);
    }
    return cantidadJugadores;
}
int getCantidadPersonajesVivos(vector<Personaje*>& jugadores)
{
    int cantidadPersonajesVivos = 0;
    for (Personaje* personaje : jugadores)
    {
        if (personaje->estaVivo())
        {
            cantidadPersonajesVivos++;
        }
    }
    return cantidadPersonajesVivos;
}
void combate(vector<Personaje*>& jugadores)
{
    int ronda = 1;
    while (getCantidadPersonajesVivos(jugadores) >= 2)
    {
        cout << VERDE << "COMIENZO DEL TURNO" << RESET << endl;
        cout << VERDE << "RONDA NÚMERO " << ronda << RESET << endl;
        // Seleccion habilidades
        for (Personaje* personaje : jugadores)
        {
            if (personaje->estaVivo())
            {
                personaje->stats();
                personaje->solicitarHabilidad();
            }
        }
        // Ataque
        for (Personaje* personaje : jugadores)
        {
            if (personaje->estaVivo())
            {
                personaje->atacar(jugadores);
            }
        }
        // Reseteo habilidades
        for (Personaje* personaje : jugadores)
        {
            if (personaje->estaVivo())
            {
                personaje->resetearHabilidad();
            }
        }
        ronda++;
        cout << endl;
    }

    cout << VERDE << "GANADOR" << RESET << endl;
    for (Personaje* personaje : jugadores)
    {
        if (personaje->estaVivo())
        {
            personaje->stats();
        }
    }
}
void eliminarPersonajes(vector<Personaje*>& jugadores)
{
    for (Personaje* personaje : jugadores)
    {
        delete personaje;
    }
}

int main()
{
    vector<Personaje*> jugadores;
    int cantidadJugadores = seleccionCantidadJugadores();
    seleccionDePersonajes(jugadores, cantidadJugadores);
    combate(jugadores);
    eliminarPersonajes(jugadores);

    return 0;
}