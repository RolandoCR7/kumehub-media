/*
  Control PI digital de velocidad para una cinta transportadora
  Diseño de controladores · Rolando Flores, Nicolás Garrido

  Planta supuesta (informe): G(s) = 1/(s + 4)  ->  K = 0,25 (rev/s)/V, tau = 0,25 s
  Muestreo con ZOH: T = 0,1 s  ->  a = e^(-4T) = 0,6703, b = (1 - a)/4 = 0,0824
  PI discreto: u[k] = Kp*e[k] + I[k],  I[k] = I[k-1] + Ki*T*e[k]
  Ganancias iniciales: Kp = 3 V/(rev/s), Ki = 15 V/(rev/s)/s
  (polos de lazo cerrado en z = 0,65 doble: sin sobrepaso con la planta 1/(s+4)).
  Hay que reajustarlas cuando se mida la planta real (actividades 3.3 y 4.2).

  Conexiones (Arduino UNO + L298N + motor DC 12 V con encoder Hall):
    D9  -> ENA del L298N (PWM, sacar el jumper de ENA)
    D7  -> IN1,  D8 -> IN2   (sentido de giro)
    D2  -> canal A del encoder (interrupción INT0)
    A0  -> cursor del potenciómetro 10 k (referencia de velocidad)
    D4  -> pulsador a GND (marcha / parada)
    D5  -> salida del sensor IR de cajas (opcional)
    GND del Arduino, del L298N y de la fuente de 12 V unidos.
*/

// ---------- pines ----------
const byte PIN_PWM = 9;
const byte PIN_IN1 = 7;
const byte PIN_IN2 = 8;
const byte PIN_ENC_A = 2;
const byte PIN_POT = A0;
const byte PIN_BOTON = 4;
const byte PIN_IR = 5;
const byte PIN_LED = 13;

// ---------- parámetros ----------
const float T = 0.1;                     // período de muestreo [s]
const unsigned long T_US = 100000UL;     // el mismo período en microsegundos
const float PULSOS_POR_VUELTA = 11.0 * 34.0;  // 11 PPR del encoder x reducción 1:34 (ajustar a tu motor)
const float V_FUENTE = 12.0;             // tensión de la fuente del motor [V]
const float REF_MAX = 2.8;               // referencia máxima [rev/s] del rodillo
const float DIAMETRO_RODILLO_CM = 3.0;

float Kp = 3.0;   // V por (rev/s)
float Ki = 15.0;  // V por (rev/s) por segundo

// ---------- estado ----------
volatile long pulsos = 0;
float integral = 0;
bool enMarcha = true;
bool irAnterior = HIGH;
unsigned long cajas = 0;
unsigned long tSiguiente;

void contarPulso() { pulsos++; }

void setup() {
  pinMode(PIN_PWM, OUTPUT);
  pinMode(PIN_IN1, OUTPUT);
  pinMode(PIN_IN2, OUTPUT);
  pinMode(PIN_ENC_A, INPUT_PULLUP);
  pinMode(PIN_BOTON, INPUT_PULLUP);
  pinMode(PIN_IR, INPUT_PULLUP);
  pinMode(PIN_LED, OUTPUT);

  digitalWrite(PIN_IN1, HIGH);  // la cinta avanza en un solo sentido
  digitalWrite(PIN_IN2, LOW);
  attachInterrupt(digitalPinToInterrupt(PIN_ENC_A), contarPulso, RISING);

  Serial.begin(115200);
  Serial.println(F("ref_rev_s,vel_rev_s,u_V"));  // encabezado para el Serial Plotter
  tSiguiente = micros() + T_US;
}

void loop() {
  leerBoton();
  contarCajas();

  // El lazo se ejecuta exactamente cada T: muestreo + ZOH (el PWM queda fijo hasta la próxima muestra)
  if ((long)(micros() - tSiguiente) < 0) return;
  tSiguiente += T_US;

  // 1) Medir: pulsos contados durante el último período
  noInterrupts();
  long n = pulsos;
  pulsos = 0;
  interrupts();
  float y = n / (PULSOS_POR_VUELTA * T);  // velocidad del rodillo [rev/s]

  // 2) Referencia desde el potenciómetro
  float r = enMarcha ? analogRead(PIN_POT) * (REF_MAX / 1023.0) : 0.0;

  // 3) PI con saturación y anti-windup (integración condicional)
  float e = r - y;
  float iNueva = integral + Ki * T * e;
  float u = Kp * e + iNueva;
  if (u > V_FUENTE) {
    u = V_FUENTE;
    if (e < 0) integral = iNueva;       // solo integra si ayuda a salir de la saturación
  } else if (u < 0) {
    u = 0;
    if (e > 0) integral = iNueva;
  } else {
    integral = iNueva;
  }
  if (!enMarcha) { u = 0; integral = 0; }

  // 4) Actuar: tensión media al motor mediante PWM
  analogWrite(PIN_PWM, (int)(u / V_FUENTE * 255.0 + 0.5));

  // 5) Registrar para comparar con la simulación (actividad 4.3)
  Serial.print(r, 3); Serial.print(',');
  Serial.print(y, 3); Serial.print(',');
  Serial.println(u, 2);
}

void leerBoton() {
  static bool anterior = HIGH;
  static unsigned long tCambio = 0;
  bool actual = digitalRead(PIN_BOTON);
  if (actual != anterior && millis() - tCambio > 50) {
    tCambio = millis();
    if (actual == LOW) enMarcha = !enMarcha;
    anterior = actual;
  }
  digitalWrite(PIN_LED, enMarcha);
}

void contarCajas() {
  bool ir = digitalRead(PIN_IR);
  if (irAnterior == HIGH && ir == LOW) cajas++;  // flanco: una caja entra al haz
  irAnterior = ir;
}
