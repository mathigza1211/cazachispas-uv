// Cazachispas UV - deteccion de arcos por fotones UV + ruido de corriente
// Transcrito del informe (seccion 6.4). Prototipo de 12 V; sin validar aun en hardware real.
const int PIN_UV = A0;          // sensor GUVA-S12SD
const int PIN_I = A1;           // sensor de corriente ACS712
const int PIN_RELE = 7;
const int PIN_BUZZER = 8;
const int PIN_LED_ROJO = 9;
const int PIN_LED_VERDE = 10;
const int PIN_REARME = 2;

// Muchos modulos rele se activan con LOW; si el tuyo es al reves, invierte estos dos valores
const int RELE_ENERGIA_ON = LOW;
const int RELE_ENERGIA_OFF = HIGH;

const int UMBRAL_UV = 40;           // cuentas sobre la linea base (calibrar)
const int UMBRAL_RUIDO = 25;        // variacion de corriente en cuentas (calibrar)
const int PULSOS_PARA_CORTE = 3;
const unsigned long VENTANA_MS = 2000;

float baseUV = 0;
int pulsos = 0;
unsigned long inicioVentana = 0;
bool uvAlto = false;
bool disparado = false;

void calibrar() {
  long suma = 0;
  for (int i = 0; i < 200; i++) { suma += analogRead(PIN_UV); delay(5); }
  baseUV = suma / 200.0;
  pulsos = 0;
  inicioVentana = millis();
}

int ruidoCorriente() {
  int minimo = 1023, maximo = 0;
  for (int i = 0; i < 100; i++) {
    int v = analogRead(PIN_I);
    if (v < minimo) minimo = v;
    if (v > maximo) maximo = v;
  }
  return maximo - minimo;   // si la corriente es estable, esto es casi 0
}

void energiaOn() {
  digitalWrite(PIN_RELE, RELE_ENERGIA_ON);
  digitalWrite(PIN_LED_VERDE, HIGH);
  digitalWrite(PIN_LED_ROJO, LOW);
  digitalWrite(PIN_BUZZER, LOW);
}

void cortar(const char* motivo) {
  disparado = true;
  digitalWrite(PIN_RELE, RELE_ENERGIA_OFF);
  digitalWrite(PIN_LED_VERDE, LOW);
  digitalWrite(PIN_LED_ROJO, HIGH);
  digitalWrite(PIN_BUZZER, HIGH);
  Serial.print("CORTE: ");
  Serial.println(motivo);
}

void setup() {
  pinMode(PIN_RELE, OUTPUT);
  pinMode(PIN_BUZZER, OUTPUT);
  pinMode(PIN_LED_ROJO, OUTPUT);
  pinMode(PIN_LED_VERDE, OUTPUT);
  pinMode(PIN_REARME, INPUT_PULLUP);
  Serial.begin(115200);
  calibrar();
  energiaOn();
}

void loop() {
  if (disparado) {
    if (digitalRead(PIN_REARME) == LOW) {   // una persona reviso y rearma
      disparado = false;
      calibrar();
      energiaOn();
    }
    return;
  }

  int uv = analogRead(PIN_UV);
  int ruido = ruidoCorriente();
  bool uvAhora = uv > baseUV + UMBRAL_UV;

  // la linea base sigue los cambios lentos (sol) solo cuando no hay chispa
  if (!uvAhora) baseUV = 0.995 * baseUV + 0.005 * uv;

  // flanco de subida = un pulso UV nuevo
  if (uvAhora && !uvAlto) {
    if (millis() - inicioVentana > VENTANA_MS) { pulsos = 0; inicioVentana = millis(); }
    pulsos++;
  }
  uvAlto = uvAhora;

  if (uvAhora && ruido > UMBRAL_RUIDO) cortar("UV + corriente inestable");
  else if (pulsos >= PULSOS_PARA_CORTE) cortar("chispas UV repetidas");

  // para ver la grafica en vivo: Herramientas > Serial Plotter
  Serial.print(uv); Serial.print(",");
  Serial.print(baseUV + UMBRAL_UV);
  Serial.print(","); Serial.println(ruido);
}
