// Broches contrôle le moteur droit
#define ENA 11
#define IN1 9
#define IN2 10

// Broches contrôle le moteur gauche
#define ENB 5
#define IN3 3
#define IN4 4


// ======= CAPTEURS ULTRASONIQUES =====

// Capteur avant
#define TRG_AV A0
#define ECHO_AV A1

// Capteur gauche
#define TRIG_G A2
#define ECHO_G A3

// Capteur droit
#define TRIG_D A4
#define ECHO_D A5

// ================= PARAMÈTRES =================

// Distance minimale avant de considérer qu'il y a un obstacle
int distanceMur= 15;

// ==================================================

void setup() {

  // Initialisation de la communication série
  Serial.begin(9600);

  // Configuration des broches moteur
  pinMode(ENA, OUTPUT);
  pinMode(ENB, OUTPUT);

  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);

  // Configuration des capteurs ultrasoniques
  pinMode(TRG_AV, OUTPUT);
  pinMode(ECHO_AV, INPUT);

  pinMode(TRIG_G, OUTPUT);
  pinMode(ECHO_G, INPUT);

  pinMode(TRIG_D, OUTPUT);
  pinMode(ECHO_D, INPUT);
}

// ==================================================

void loop() {

  // Lecture des distances mesurées
  int distanceAvant = mesurerDistance(TRG_AV, ECHO_AV);
  int distanceGauche  = mesurerDistance(TRIG_G, ECHO_G);
  int distanceDroite = mesurerDistance(TRIG_D, ECHO_D);

  // LOGIQUE DE DÉPLACEMENT DANS LE LABYRINTHE

  // Si l'avant est libre
  if (distanceAvant > distanceMur) {

    Serial.println("AVANCER");
    avancer(distanceGauche, distanceDroite);
  }

  // Si un mur est détecté devant
  else {

    arreterRobot();
    delay(300);

    // Si la droite est libre
    if (distanceDroite > distanceMur) {

      Serial.println("TOURNER A DROITE");
      tournerDroite();
    }

    // Sinon si la gauche est libre
    else if (distanceGauche > distanceMur) {

      Serial.println("TOURNER A GAUCHE");
      tournerGauche();
    }

    // Sinon demi-tour
    else {

      Serial.println("DEMI-TOUR");
      demiTour();
    }
  }
}

// FONCTION DE MESURE DE DISTANCE

int mesurerDistance(int trigPin, int echoPin) {

  // Génération de l'impulsion ultrasonique
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);

  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);

  digitalWrite(trigPin, LOW);

  // Mesure du temps de retour de l'écho
  long duration = pulseIn(echoPin, HIGH, 30000);

  // Si aucun écho reçu
  if (duration == 0)
    return 999;

  // Conversion temps -> distance (cm)
  return duration * 0.034 / 2;
}

// ==================================================
// AVANCER AVEC CORRECTION DE TRAJECTOIRE
// ==================================================

void avancer(int distanceGauche, int distanceDroite) {

  int vitesseDroite = 175;
  int vitesseGauche = 190;

  // Trop proche du mur gauche
  if (distanceGauche < 5) {

    vitesseDroite += 10;
    vitesseGauche -= 10;
  }

  // Trop proche du mur droit
  if (distanceDroite < 5) {

    vitesseDroite -= 10;
    vitesseGauche += 10;
  }

  analogWrite(ENA, vitesseDroite);
  analogWrite(ENB, vitesseGauche);

  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);

  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
}

// ==================================================
// ARRÊT DU ROBOT
// ==================================================

void arreterRobot() {

  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);

  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
}

// ==================================================
// TOURNER À DROITE
// ==================================================

void tournerDroite() {

  analogWrite(ENA, 175);
  analogWrite(ENB, 190);

  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);

  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);

  // Durée calibrée pour un angle proche de 90°
  delay(260);

  arreterRobot();
  delay(300);
}

// ==================================================
// TOURNER À GAUCHE
// ==================================================

void tournerGauche() {

  analogWrite(ENA, 175);
  analogWrite(ENB, 190);

  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);

  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);

  // Durée calibrée pour un angle proche de 90°
  delay(280);

  arreterRobot();
  delay(300);
}

// ==================================================
// DEMI-TOUR (180°)
// ==================================================

void demiTour() {

  analogWrite(ENA, 175);
  analogWrite(ENB, 190);

  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);

  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);

  // Durée calibrée pour un demi-tour
  delay(470);

  arreterRobot();
  delay(300);
}