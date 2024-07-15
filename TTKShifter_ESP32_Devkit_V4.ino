#include <BleGamepad.h>

// Piny potencjometrów
const int pot1Pin = 36; // Pin potencjometra 1 (GPIO36)
const int pot2Pin = 39; // Pin potencjometra 2 (GPIO39)

// Pin przycisku
const int buttonPin = 15; // Pin przycisku (GPIO15)

// Zmienne do przechowywania wartości potencjometrów i stanu przycisku
int pot1Value = 0;
int pot2Value = 0;
bool buttonState = false;

// Stałe do skalowania wartości potencjometrów
const int potMin = 0;    // Minimalna wartość potencjometrów
const int potMax = 4095; // Maksymalna wartość potencjometrów

// Tablica mapująca wartości potencjometrów na biegi
// Każdy zestaw (X, Y) odpowiada jednemu biegowi
const int gears[][4] = {
  {700, 370, 100, 100}, // Bieg 1: X: 700, Y: 370, tolerancja: +\- 100
  {910, 4095, 100, 100}, // Bieg 2: X: 910, Y: 4095, tolerancja: +\- 100
  {1500, 300, 100, 100}, // Bieg 3: X: 1500, Y: 300, tolerancja: +\- 100
  {1840, 4095, 100, 100}, // Bieg 4: X: 1840, Y: 4095, tolerancja: +\- 100
  {2470, 360, 100, 100}, // Bieg 5: X: 2470, Y: 360, tolerancja: +\- 100
  {2610, 4095, 100, 100}  // Bieg 6: X: 2610, Y: 4095, tolerancja: +\- 100
};

// Liczba biegów
const int numGears = 6;

// Inicjalizacja obiektu BleGamepad
BleGamepad bleGamepad("TTKShifter", "TTK", 100);

void setup() {
  // Inicjalizacja portu szeregowego
  Serial.begin(115200);
  while (!Serial) {
    ; // Czekaj na otwarcie portu szeregowego
  }
  Serial.println("Serial port initialized");

  // Inicjalizacja interfejsu BLE Gamepad
  bleGamepad.begin();
  Serial.println("BLE Gamepad initialized");

  // Ustawienie pinów potencjometrów i przycisku jako wejścia
  pinMode(pot1Pin, INPUT);
  pinMode(pot2Pin, INPUT);
  pinMode(buttonPin, INPUT_PULLUP); // Używamy wewnętrznego rezystora podciągającego
  Serial.println("Pins initialized");
}

void loop() {
  // Odczyt wartości potencjometrów
  pot1Value = analogRead(pot1Pin);
  pot2Value = analogRead(pot2Pin);
  Serial.print("Pot1: ");
  Serial.print(pot1Value);
  Serial.print(" Pot2: ");
  Serial.println(pot2Value);

  // Odczyt stanu przycisku
  buttonState = digitalRead(buttonPin) == LOW; // Przycisk wciśnięty, gdy stan niski
  Serial.print("Button state: ");
  Serial.println(buttonState);

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

  // Jeśli przycisk jest wciśnięty, nadpisujemy biegi 2 i 6
  if (buttonState) {
    if (selectedGear == 2) {
      selectedGear = 7; // Dodatkowy bieg 1
      Serial.println("Additional gear 1 selected");
    } else if (selectedGear == 6) {
      selectedGear = 8; // Dodatkowy bieg 2
      Serial.println("Additional gear 2 selected");
    }
  }

  // Wysyłanie danych do komputera jako HID (przy użyciu BleGamepad library)
  // W tym przykładzie wykorzystujemy przyciski jako biegi
  for (int i = 0; i < numGears + 2; ++i) { // +2 dla dodatkowych biegów
    if (selectedGear == (i + 1)) {
      bleGamepad.press(i + 1); // Naciśnięcie przycisku
      Serial.print("Pressing button ");
      Serial.println(i + 1);
    } else {
      bleGamepad.release(i + 1); // Zwolnienie przycisku
      Serial.print("Releasing button ");
      Serial.println(i + 1);
    }
  }

  // Aktualizacja stanu gamepada
  bleGamepad.sendReport();

  // Opóźnienie zapobiegające zbyt szybkiemu odczytowi
  delay(50);
}