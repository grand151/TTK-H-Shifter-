#include <Joystick.h>

// Inicjalizacja obiektu Joystick
Joystick_ Joystick;

// Piny potencjometrów
const int pot1Pin = A0; // Pin potencjometra 1
const int pot2Pin = A1; // Pin potencjometra 2

// Zmienne do przechowywania wartości potencjometrów
int pot1Value = 0;
int pot2Value = 0;

// Stałe do skalowania wartości potencjometrów
const int potMin = 0;    // Minimalna wartość potencjometrów
const int potMax = 1023; // Maksymalna wartość potencjometrów

// Tablica mapująca wartości potencjometrów na biegi
// Każdy zestaw (X, Y) odpowiada jednemu biegowi
const int gears[][4] = {
  {817, 654, 50, 50}, // Bieg 1: X: 817, Y: 654, tolerancja: +\- 50
  {817, 0,   50, 50}, // Bieg 2: X: 817, Y: 0,   tolerancja: +\- 50
  {680, 550, 50, 50}, // Bieg 3: X: 680, Y: 550, tolerancja: +\- 50
  {700, 0,   50, 50}, // Bieg 4: X: 700, Y: 0,   tolerancja: +\- 50
  {550, 450, 50, 50}, // Bieg 5: X: 550, Y: 450, tolerancja: +\- 50
  {570, 0,   50, 50}  // Bieg 6: X: 570, Y: 0,   tolerancja: +\- 50
};

// Liczba biegów
const int numGears = 6;

void setup() {
  // Inicjalizacja interfejsu Joystick
  Joystick.begin();

  // Ustawienie pinów potencjometrów jako wejścia
  pinMode(pot1Pin, INPUT);
  pinMode(pot2Pin, INPUT);
}

void loop() {
  // Odczyt wartości potencjometrów
  pot1Value = analogRead(pot1Pin);
  pot2Value = analogRead(pot2Pin);

  // Wyszukiwanie odpowiadającego biegu
  int selectedGear = 0; // Domyślnie neutralny bieg

  for (int i = 0; i < numGears; ++i) {
    int targetX = gears[i][0];
    int targetY = gears[i][1];
    int toleranceX = gears[i][2];
    int toleranceY = gears[i][3];

    // Sprawdzenie, czy aktualne wartości potencjometrów są wystarczająco blisko docelowych wartości
    if (abs(pot1Value - targetX) <= toleranceX && abs(pot2Value - targetY) <= toleranceY) {
      selectedGear = i + 1; // Znaleziony bieg (indeksowanie od 1)
      break;
    }
  }

  // Wysyłanie danych do komputera jako HID (przy użyciu Joystick library)
  // W tym przykładzie wykorzystujemy przyciski jako biegi
  for (int i = 0; i < numGears; ++i) {
    Joystick.setButton(i + 1, (selectedGear == (i + 1)));
  }

  // Opóźnienie zapobiegające zbyt szybkiemu odczytowi
  delay(50);
}