#include <iostream>
#include <ctime>

using namespace std;

void Mezclar(bool CartasRepartidas[]) {
	for (int iIndex = 0; iIndex < 52; ++iIndex) {
		CartasRepartidas[iIndex] = false;
	}
}

void ImprimirCarta(int iCarta) {

	// Valor de la carta
	const int Rango = (iCarta % 13);
	if (Rango == 0) {
		cout << 'A';
	} else if (Rango < 9) {
		cout << (Rango + 1);
	} else if (Rango == 9) {
		cout << 'T';
	} else if (Rango == 10) {
		cout << 'J';
	} else if (Rango == 11) {
		cout << 'Q';
	} else {
		cout << 'K';
	}
	// Palo a la que pertenece la carta
	const int Palo = (iCarta/13);
	if (Palo == 0) {
		cout << 'O';
	} else if (Palo == 1) {
		cout << 'E';
	} else if (Palo == 2) {
		cout << 'C';
	} else {
		cout << 'B';
	}
}

void ImprimirMano(int iaMano[], const int ContadorCartas) {

	for (int IdCarta = 0; IdCarta < ContadorCartas; ++IdCarta) {
		const int CartaSaliente = iaMano[IdCarta];
		ImprimirCarta(CartaSaliente);
		cout << " ";
	}
	cout << endl;
}

int SiguienteCarta(bool CartasRepartidas[]) {

	bool RepartirCarta	= true;
	int NuevaCarta	= -1;
	do {
		NuevaCarta = (rand() % 52);
		if (!CartasRepartidas[NuevaCarta]) {
			RepartirCarta = false;
		}
	} while (RepartirCarta);
	return NuevaCarta;
}

int Puntuacion(int iaMano[], const int ContadorCartas) {

	int AsValor	= 0;
	int Puntaje	= 0;
	for (int IdCarta = 0; IdCarta < ContadorCartas; ++IdCarta) {
		const int CartaSaliente = iaMano[IdCarta];
		const int Rango = (CartaSaliente % 13);
		if (Rango == 0) {
			++AsValor;
			++Puntaje;
		} else if (Rango < 9) {
			Puntaje = Puntaje + (Rango + 1);
		} else {
			Puntaje = Puntaje + 10;
		}
	}
	while (AsValor > 0 && Puntaje < 12) {
		--AsValor;
		Puntaje = Puntaje + 10;
	}
	return Puntaje;
}

void ImprimirEnConsola(int ManoCasa[], const int PuntajeCasa, int ManoJugador[], const int PuntajeJugador) {

	cout << "Mano de la Casa: Puntos = " << Puntuacion(ManoCasa, PuntajeCasa) << endl;
	ImprimirMano(ManoCasa, PuntajeCasa);
	cout << "Mano del Jugador: Puntos = " << Puntuacion(ManoJugador, PuntajeJugador) << endl;
	ImprimirMano(ManoJugador, PuntajeJugador);
	cout << endl;
}

int main() {
	using namespace std;
	// Comienza el generador de números aleatorios
	time_t qTime;
	time   (&qTime);
	srand  (qTime);

	bool CartasRepartidas[52];
	int  CartasCasa = 0;
	int  ManoCasa[12];
	int  CartasJugador = 0;
	int  ManoJugador[12];

	// Inicia un bucle para cada mano
	while (true) {
		// Mezclar las cartas sin repartir
		Mezclar(CartasRepartidas);
		// Genera las manos. Entrega dos cartas para cada uno
		ManoJugador[0]	= SiguienteCarta(CartasRepartidas);
		ManoCasa[0]		= SiguienteCarta(CartasRepartidas);
		ManoJugador[1]	= SiguienteCarta(CartasRepartidas);
		ManoCasa[1]		= SiguienteCarta(CartasRepartidas);
		CartasCasa		= 2;
		CartasJugador	= 2;

		// Señale una nueva mano.
		cout << "--------------------------------------------------------" << endl;
		cout << "-----------------------Nueva Mano-----------------------" << endl;
		cout << "--------------------------------------------------------" << endl;

		char Eleccion;
		bool Peticion			= true;
		int  iPuntajeJugador	= Puntuacion(ManoJugador, CartasJugador);
		// Obtenga la decision del jugador. Calcule la puntuación y vuelva a mostrar después de cada eleccion.
		do {
			// Imprime las cartas repartidas, pero solo la segunda carta de la Casa.
			cout << "Mano de la Casa" << endl;
			cout << "** ";
			ImprimirCarta(ManoCasa[1]);
			cout << endl;
			cout << "Mano del Jugador: Puntos = " << Puntuacion(ManoJugador, CartasJugador) << endl;
			ImprimirMano(ManoJugador, CartasJugador);

			// Pregúnta al jugador si quiere un seguir o quedarse.
			cout << "Quedarse(q) o Pedir(p): ";
			cin >> Eleccion;
			if (Eleccion == 'p') {
				ManoJugador[CartasJugador] = SiguienteCarta(CartasRepartidas);
				++CartasJugador;
			} else if (Eleccion == 'q') {
				Peticion = false;
			} else {
				cout << "Error: ¡Intenta nuevamente!" << endl;
			}
			cout << endl;
			// Obtenga la puntuación actual del jugador para actualizar y comprobar si se ha pasado.
			iPuntajeJugador	= Puntuacion(ManoJugador, CartasJugador);
		} while (Peticion && iPuntajeJugador < 22);

		// Una vez que el jugador haya terminado de recibir cartas, verifique si ha perdido
		if (iPuntajeJugador > 21) {
			// El jugador se paso. La Casa gana.
			cout << "**** !!!La Casa Gana!!! ****" << endl;
			ImprimirEnConsola(ManoCasa, CartasCasa, ManoJugador, CartasJugador);
		} else {
			// Si el jugador no se pasa y la casa recibe resultados por debajo de 17
			int iPuntajeCasa = Puntuacion(ManoCasa, CartasCasa);
			while (iPuntajeCasa < 17) {
				ManoCasa[CartasCasa] = SiguienteCarta(CartasRepartidas);
				++CartasCasa;
				iPuntajeCasa = Puntuacion(ManoCasa, CartasCasa);
			}
			bool bCasaPide	= (iPuntajeCasa > 21);
			if (bCasaPide) {
				// La Casa se pasa. El jugador gana
				cout << "**** !!!El Jugador Gana!!! ****" << endl;
				ImprimirEnConsola(ManoCasa, CartasCasa, ManoJugador, CartasJugador);
			} else {
				// Comparar puntuaciones y determinar el ganador
				if (iPuntajeJugador == iPuntajeCasa) {
					// Empate en caso que los puntos obtenidos sean iguales
					cout << "**** !!!Empate!!! ****" << endl;
					ImprimirEnConsola(ManoCasa, CartasCasa, ManoJugador, CartasJugador);
				} else if (iPuntajeJugador > iPuntajeCasa) {
					// El jugador Gana
					cout << "**** !!!El Jugador Gana!!! ****" << endl;
					ImprimirEnConsola(ManoCasa, CartasCasa, ManoJugador, CartasJugador);
				} else {
					// La casa gana
					cout << "**** !!!La Casa Gana!!! ****" << endl;
					ImprimirEnConsola(ManoCasa, CartasCasa, ManoJugador, CartasJugador);
				}
			}
		}
	}
	return 0;
}