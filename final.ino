const int s0Pin = 5;   // Pin S0 multipleksera podłączony do pinu cyfrowego 2 Arduino Uno
const int s1Pin = 4;   // Pin S1 multipleksera podłączony do pinu cyfrowego 3 Arduino Uno
const int s2Pin = 3;   // Pin S2 multipleksera podłączony do pinu cyfrowego 4 Arduino Uno
const int s3Pin = 2;   // Pin S3 multipleksera podłączony do pinu cyfrowego 5 Arduino Uno
const int sigPin = 10; 
const int sigPin1 = 11; // Pin sygnałowy multipleksera podłączony do pinu analogowego A0 Arduino Uno
const int sigPin2 = 12;
const int sigPin3 = 13;
int latchPin = 40;           // Latch pin of 74HC595 is connected to Digital pin 5
int clockPin = 41;           // Clock pin of 74HC595 is connected to Digital pin 6
int dataPin = 42; 
int tab_current[8][8];
int tab_prev[8][8];
char board[8][8] = {
                    { 'R' , 'H' , 'C' , 'K' , 'Q' , 'C' , 'H' , 'R' },
                    { 'P' , 'P' , 'P' , 'P' , 'P' , 'P' , 'P' , 'P' },
                    { ' ' , ' ' , ' ' , ' ' , ' ' , ' ' , ' ' , ' ' },
                    { ' ' , ' ' , ' ' , ' ' , ' ' , ' ' , ' ' , ' ' },
                    { ' ' , ' ' , ' ' , ' ' , ' ' , ' ' , ' ' , ' ' },
                    { ' ' , ' ' , ' ' , ' ' , ' ' , ' ' , ' ' , ' ' },
                    { 'p' , 'p' , 'p' , 'p' , 'p' , 'p' , 'p' , 'p' },
                    { 'r' , 'h' , 'c' , 'k' , 'q' , 'c' , 'h' , 'r' }
                    };
byte leds_line_a = 0; //Np. B10010001 oznacza a1-on, a2-off, a3-off,a4-on.a5-off,a6-off,a7-off, a8-on
byte leds_line_b = 0;
byte leds_line_c = 0;
byte leds_line_d = 0;
byte leds_line_e = 0;
byte leds_line_f = 0;
byte leds_line_g = 0;
byte leds_line_h = 0;


int SetChannel (byte channel)
{
  digitalWrite(s0Pin, bitRead(channel, 0));
  digitalWrite(s1Pin, bitRead(channel, 1));
  digitalWrite(s2Pin, bitRead(channel, 2));
  digitalWrite(s3Pin , bitRead(channel, 3));
}



void setup() {
  pinMode(s0Pin, OUTPUT);
  pinMode(s1Pin, OUTPUT);
  pinMode(s2Pin, OUTPUT);
  pinMode(s3Pin, OUTPUT);
  pinMode(sigPin, INPUT);
   pinMode(sigPin1, INPUT);
   pinMode(sigPin2, INPUT);
   pinMode(sigPin3, INPUT);
   pinMode(latchPin, OUTPUT);
pinMode(dataPin,OUTPUT);
pinMode(clockPin,OUTPUT);
  //pinMode(Pin, INPUT);
  Serial.begin(9600);
  updateShiftRegister();
 check_line();
 copy_tab();
}

int i1 =-1,j1=-1,i2,j2;
char move_fig = 'a';

void loop() {
//copy_tab();

//check_line(); //zapisanie do tablicy stanu każdego pola

compare_stage(); //porówananie z poprzednim stanem (informacja o tym jaki pionek i z którego pola został podniesiony)

Serial.println((String)"Move_fig "+ move_fig);
Serial.println((String)"i1 "+ i1);
Serial.println((String)"j1 "+ j1);

move(move_fig, i1, j1); //podniesienie pionka 

updateShiftRegister(); //włączenie diod

copy_tab();

compare_stage2(); //czekamy na połozenie pionka

update_board();

switchoff_light();

copy_tab();
}

void update_board(){
  board[i2][j2] = move_fig;
  Serial.println(move_fig);
  board[i1][j1]=' ';
  for (int i = 0; i < 8; i++) {
    for (int j = 0; j < 8; j++) {
      Serial.print(board[i][j]);
      Serial.print(" ,");
    }
    Serial.println();
  } 
}

void light(int i, int j)
{
  if(i==0)
  {
    bitSet(leds_line_a,j);
  }
  else if(i==1)
  {
     bitSet(leds_line_b,j);
  }
  else if(i==2)
  {
     bitSet(leds_line_c,j);
  }
  else if(i==3)
  {
     bitSet(leds_line_d,j);
  }
  else if(i==4)
  {
     bitSet(leds_line_e,j);
  }
  else if(i==5)
  {
     bitSet(leds_line_f,j);
  }
  else if(i==6)
  {
     bitSet(leds_line_g,j);
  }
  else if(i==7)
  {
     bitSet(leds_line_h,j);
  }
}

void check_line ()
{

for (int i = 0; i < 16; i++) {
    SetChannel(i);
  delay(10);
  int test=0; 
  if(i<8){
      tab_current[test][i]=digitalRead(sigPin);

  }

  else{
      test=1;
      tab_current[test][i-8]=digitalRead(sigPin);

  }
}
for (int i = 0; i < 16; i++) {
    SetChannel(i);
  delay(10);
  int test=2; 
  if(i<8){
      tab_current[test][i]=digitalRead(sigPin1);

  }

  else{
      test=3;
      tab_current[test][i-8]=digitalRead(sigPin1);

  }
}
for (int i = 0; i < 16; i++) {
    SetChannel(i);
  delay(10);
  int test=4; 
  if(i<8){
      tab_current[test][i]=digitalRead(sigPin2);

  }

  else{
      test=5;
      tab_current[test][i-8]=digitalRead(sigPin2);

  }
}
for (int i = 0; i < 16; i++) {
    SetChannel(i);
  delay(10);
  int test=6; 
  if(i<8){
      tab_current[test][i]=digitalRead(sigPin3);

  }

  else{
      test=7;
      tab_current[test][i-8]=digitalRead(sigPin3);

  }
}}


void compare_stage() {
int b1=-1;
int b2=-1;

  while (true) {
    check_line();
    for (int i = 0; i < 8; i++) {
      for (int j = 0; j < 8; j++) {
        if (tab_current[i][j] != tab_prev[i][j]) {
          i1 = i;
          j1 = j;
          b1=i;
          b2=j;
          move_fig = board[i][j];
          break;

        }
        else{
        //Serial.println(i);
        //delay(100);
        }
      }
      if (b1 != -1 && b2 != -1) {
       // Serial.println(i1);
       // Serial.println("elo");
        break;
      }
    }

    if (b1 != -1 && b2 != -1) {
      // Do something with the differing element (i1, j1) and its value (move_fig)
      break; // Exit the while loop when the differing element is found
    }
  }
  
}

void compare_stage2() {
int b1=-1;
int b2=-1;

  while (true) {
    check_line();
    for (int i = 0; i < 8; i++) {
      for (int j = 0; j < 8; j++) {
        if (tab_current[i][j] != tab_prev[i][j]) {
          i2 = i;
          j2 = j;
          b1=i;
          b2=j;
          break;

        }
        else{
        //Serial.println(i);
        //delay(100);
        }
      }
      if (b1 != -1 && b2 != -1) {
       // Serial.println(i1);
       // Serial.println("elo");
        break;
      }
    }

    if (b1 != -1 && b2 != -1) {
      // Do something with the differing element (i1, j1) and its value (move_fig)
      break; // Exit the while loop when the differing element is found
    }
  }
  
}

void move(char move_fig, int i, int j){
  if(move_fig=='R'){
    D_RookMoves(i,j);
  }
  else if(move_fig=='r'){
    m_RookMoves(i,j);
  }
  else if(move_fig=='H'){
    D_KnightMoves(i,j);
  }
  else if(move_fig=='h'){
    m_KnightMoves(i,j);
  }
  else if(move_fig=='C'){
    D_BishopMoves(i,j);
  }
  else if(move_fig=='c'){
    m_BishopMoves(i,j);
  }
  else if(move_fig=='K'){
    D_KingMoves(i,j);
  }
  else if(move_fig=='k'){
    m_KingMoves(i,j);
  }
  else if(move_fig=='Q'){
    D_QueenMoves(i,j);
  }
  else if(move_fig=='q'){
    m_QueenMoves(i,j);
  }
  else if(move_fig=='P'){
    D_PawnMoves(i,j);
  }
  else if(move_fig=='p'){
    m_PawnMoves(i,j);
  }
}

void D_PawnMoves(int i, int j) {

  if (i < 7 && board[i+1][j] == ' ') {
  light(i+1,j);
  }


  if (i == 1 && board[2][j] == ' ' && board[3][j] == ' ') {
  light(2,j);
  light(3,j);
  }

  if (i < 7 && j < 7 && board[i + 1][j + 1] == 'r' || i > 0 && j > 0 && board[i + 1][j + 1] == 'h' || i > 0 && j > 0 && board[i + 1][j + 1] == 'c' || i > 0 && j > 0 && board[i + 1][j + 1] == 'k' || i > 0 && j > 0 && board[i + 1][j + 1] == 'q' || i > 0 && j > 0 && board[i + 1][j + 1] == 'p') {
  light(i+1,j+1);
  }


  if (i < 7 && j > 0 && board[i + 1][j - 1] == 'r' || i > 0 && j > 0 && board[i + 1][j - 1] == 'h' || i > 0 && j > 0 && board[i + 1][j - 1] == 'c' || i > 0 && j > 0 && board[i + 1][j - 1] == 'k' || i > 0 && j > 0 && board[i + 1][j - 1] == 'q' || i > 0 && j > 0 && board[i + 1][j - 1] == 'p') {
  light(i+1,j-1);
  }
}


void D_RookMoves(int i, int j) {
  int row, col;
  
  // w górę
  for (row = i + 1; row <= 7; row++) {
    col = j;
    if (board[row][col] == ' ') {
      light(row, col);
    } else {
      break;
    }
  }
  if (row <= 7 && (board[row][col] == 'p' || board[row][col] == 'r' || board[row][col] == 'h' || board[row][col] == 'c' || board[row][col] == 'k' || board[row][col] == 'q')) {
    light(row, col);
  }

  // w dół
  for (row = i - 1; row >= 0; row--) {
    col = j;
    if (board[row][col] == ' ') {
      light(row, col);
    } else {
      break;
    }
  }
  if (row >= 0 && (board[row][col] == 'p' || board[row][col] == 'r' || board[row][col] == 'h' || board[row][col] == 'c' || board[row][col] == 'k' || board[row][col] == 'q')) {
    light(row, col);
  }

  // w prawo
  for (col = j + 1; col <= 7; col++) {
    row = i;
    if (board[row][col] == ' ') {
      light(row, col);
    } else {
      break;
    }
  }
  if (col <= 7 && (board[row][col] == 'p' || board[row][col] == 'r' || board[row][col] == 'h' || board[row][col] == 'c' || board[row][col] == 'k' || board[row][col] == 'q')) {
    light(row, col);
  }

  // w lewo
  for (col = j - 1; col >= 0; col--) {
    row = i;
    if (board[row][col] == ' ') {
      light(row, col);
    } else {
      break;
    }
  }
  if (col >= 0 && (board[row][col] == 'p' || board[row][col] == 'r' || board[row][col] == 'h' || board[row][col] == 'c' || board[row][col] == 'k' || board[row][col] == 'q')) {
    light(row, col);
  }
}

void D_BishopMoves(int i, int j) {
  int row, col;
  for (row = i - 1, col = j - 1; row >= 0 && col >= 0; row--, col--) {
    if (board[row][col] == ' ') {
      light(row, col);
    } else {
      break;
    }
  }

  if (row >= 0 && col >= 0 && (board[row][col] == 'p' || board[row][col] == 'r' || board[row][col] == 'h' || board[row][col] == 'c' || board[row][col] == 'k' || board[row][col] == 'q')) {
    light(row, col);
  }

  for (row = i + 1, col = j - 1; row <= 7 && col >= 0; row++, col--) {
    if (board[row][col] == ' ') {
      light(row, col);
    } else {
      break;
    }
  }

  if (row <= 7 && col >= 0 && (board[row][col] == 'p' || board[row][col] == 'r' || board[row][col] == 'h' || board[row][col] == 'c' || board[row][col] == 'k' || board[row][col] == 'q')) {
    light(row, col);
  }

  for (row = i - 1, col = j + 1; row >= 0 && col <= 7; row--, col++) {
    if (board[row][col] == ' ') {
      light(row, col);
    } else {
      break;
    }
  }

  if (row >= 0 && col <= 7 && (board[row][col] == 'p' || board[row][col] == 'r' || board[row][col] == 'h' || board[row][col] == 'c' || board[row][col] == 'k' || board[row][col] == 'q')) {
    light(row, col);
  }

  for (row = i + 1, col = j + 1; row <= 7 && col <= 7; row++, col++) {
    if (board[row][col] == ' ') {
      light(row, col);
    } else {
      break;
    }
  }

  if (row <= 7 && col <= 7 && (board[row][col] == 'p' || board[row][col] == 'r' || board[row][col] == 'h' || board[row][col] == 'c' || board[row][col] == 'k' || board[row][col] == 'q')) {
    light(row, col);
  }
}

void D_QueenMoves(int i, int j) {

  D_RookMoves(i, j);
  

  D_BishopMoves(i, j);
}

void D_KingMoves(int i, int j) {
  int kingMoves[8][2] = {
    {-1, -1}, {-1, 0}, {-1, 1},
    {0, -1}, {0, 1},
    {1, -1}, {1, 0}, {1, 1}
  };
  
  for (int k = 0; k < 8; k++) {
    int newRow = i + kingMoves[k][0];
    int newCol = j + kingMoves[k][1];
    
    if (newRow >= 0 && newRow <= 7 && newCol >= 0 && newCol <= 7 && (board[newRow][newCol] == ' ' || board[newRow][newCol] == 'p' || board[newRow][newCol] == 'r' || board[newRow][newCol] == 'h' || board[newRow][newCol] == 'c' || board[newRow][newCol] == 'k' || board[newRow][newCol] == 'q')) 
    {
        light(newRow,newCol);
    }
  }
}

void D_KnightMoves(int i, int j) {
  int knightMoves[8][2] = {
    {-2, -1}, {-2, 1}, {-1, -2}, {-1, 2},
    {1, -2}, {1, 2}, {2, -1}, {2, 1}
  };
  
  for (int k = 0; k < 8; k++) {
    int newRow = i + knightMoves[k][0];
    int newCol = j + knightMoves[k][1];
    
    if (newRow >= 0 && newRow <= 7 &&
        newCol >= 0 && newCol <= 7 &&
        (board[newRow][newCol] == ' ' || board[newRow][newCol] == 'p' || board[newRow][newCol] == 'r' || board[newRow][newCol] == 'h' || board[newRow][newCol] == 'c' || board[newRow][newCol] == 'k' || board[newRow][newCol] == 'q')) {
      light(newRow,newCol);
    }
  }
}

void m_BishopMoves(int i, int j) {
  int row, col;
  for (row = i - 1, col = j - 1; row >= 0 && col >= 0; row--, col--) {
    if (board[row][col] == ' ') {
      light(row, col);
    } else {
      break;
    }
  }

  if (row >= 0 && col >= 0 && (board[row][col] == 'P' || board[row][col] == 'R' || board[row][col] == 'H' || board[row][col] == 'C' || board[row][col] == 'K' || board[row][col] == 'Q')) {
    light(row, col);
  }

  for (row = i + 1, col = j - 1; row <= 7 && col >= 0; row++, col--) {
    if (board[row][col] == ' ') {
      light(row, col);
    } else {
      break;
    }
  }

  if (row <= 7 && col >= 0 && (board[row][col] == 'P' || board[row][col] == 'R' || board[row][col] == 'H' || board[row][col] == 'C' || board[row][col] == 'K' || board[row][col] == 'Q')) {
    light(row, col);
  }

  for (row = i - 1, col = j + 1; row >= 0 && col <= 7; row--, col++) {
    if (board[row][col] == ' ') {
      light(row, col);
    } else {
      break;
    }
  }

  if (row >= 0 && col <= 7 && (board[row][col] == 'P' || board[row][col] == 'R' || board[row][col] == 'H' || board[row][col] == 'C' || board[row][col] == 'K' || board[row][col] == 'Q')) {
    light(row, col);
  }

  for (row = i + 1, col = j + 1; row <= 7 && col <= 7; row++, col++) {
    if (board[row][col] == ' ') {
      light(row, col);
    } else {
      break;
    }
  }

  if (row <= 7 && col <= 7 && (board[row][col] == 'P' || board[row][col] == 'R' || board[row][col] == 'H' || board[row][col] == 'C' || board[row][col] == 'K' || board[row][col] == 'Q')) {
    light(row, col);
  }
}


void m_RookMoves(int i, int j) {
  int row, col;
  
  // w górę
  for (row = i + 1; row <= 7; row++) {
    col = j;
    if (board[row][col] == ' ') {
      light(row, col);
    } else {
      break;
    }
  }
  if (row <= 7 && (board[row][col] == 'P' || board[row][col] == 'R' || board[row][col] == 'H' || board[row][col] == 'C' || board[row][col] == 'K' || board[row][col] == 'Q')) {
    light(row, col);
  }

  // w dół
  for (row = i - 1; row >= 0; row--) {
    col = j;
    if (board[row][col] == ' ') {
      light(row, col);
    } else {
      break;
    }
  }
  if (row >= 0 && (board[row][col] == 'P' || board[row][col] == 'R' || board[row][col] == 'H' || board[row][col] == 'C' || board[row][col] == 'K' || board[row][col] == 'Q')) {
    light(row, col);
  }

  // w prawo
  for (col = j + 1; col <= 7; col++) {
    row = i;
    if (board[row][col] == ' ') {
      light(row, col);
    } else {
      break;
    }
  }
  if (col <= 7 && (board[row][col] == 'P' || board[row][col] == 'R' || board[row][col] == 'H' || board[row][col] == 'C' || board[row][col] == 'K' || board[row][col] == 'Q')) {
    light(row, col);
  }

  // w lewo
  for (col = j - 1; col >= 0; col--) {
    row = i;
    if (board[row][col] == ' ') {
      light(row, col);
    } else {
      break;
    }
  }
  if (col >= 0 && (board[row][col] == 'P' || board[row][col] == 'R' || board[row][col] == 'H' || board[row][col] == 'C' || board[row][col] == 'K' || board[row][col] == 'Q')) {
    light(row, col);
  }
}

void m_QueenMoves(int i, int j) {

  m_RookMoves(i, j);
  

  m_BishopMoves(i, j);
}


void m_KingMoves(int i, int j) {
  int kingMoves[8][2] = {
    {-1, -1}, {-1, 0}, {-1, 1},
    {0, -1}, {0, 1},
    {1, -1}, {1, 0}, {1, 1}
  };
  
  for (int k = 0; k < 8; k++) {
    int newRow = i + kingMoves[k][0];
    int newCol = j + kingMoves[k][1];
    
    if (newRow >= 0 && newRow <= 7 && newCol >= 0 && newCol <= 7 && (board[newRow][newCol] == ' ' || board[newRow][newCol] == 'P' || board[newRow][newCol] == 'R' || board[newRow][newCol] == 'H' || board[newRow][newCol] == 'C' || board[newRow][newCol] == 'K' || board[newRow][newCol] == 'Q')) 
    {
        light(newRow,newCol);
    }
  }
}

void m_KnightMoves(int i, int j) {
  int knightMoves[8][2] = {
    {-2, -1}, {-2, 1}, {-1, -2}, {-1, 2},
    {1, -2}, {1, 2}, {2, -1}, {2, 1}
  };
  
  for (int k = 0; k < 8; k++) {
    int newRow = i + knightMoves[k][0];
    int newCol = j + knightMoves[k][1];
    
    if (newRow >= 0 && newRow <= 7 &&
        newCol >= 0 && newCol <= 7 &&
        (board[newRow][newCol] == ' ' || board[newRow][newCol] == 'P' || board[newRow][newCol] == 'R' || board[newRow][newCol] == 'H' || board[newRow][newCol] == 'C' || board[newRow][newCol] == 'K' || board[newRow][newCol] == 'Q')) {
      light(newRow,newCol);
    }
  }
}

void m_PawnMoves(int i, int j) {

  if (i > 0 && board[i-1][j] == ' ') {
  light(i-1,j);
  }


  if (i == 6 && board[5][j] == ' ' && board[4][j] == ' ') { 
  light(5,j);
  light(4,j);
  }

  if (i > 0 && j > 0 && board[i - 1][j - 1] == 'R' || i > 0 && j > 0 && board[i - 1][j - 1] == 'H' || i > 0 && j > 0 && board[i - 1][j - 1] == 'C' || i > 0 && j > 0 && board[i - 1][j - 1] == 'K' || i > 0 && j > 0 && board[i - 1][j - 1] == 'Q' || i > 0 && j > 0 && board[i - 1][j - 1] == 'P') {
  light(i-1,j-1);
  }


  if (i > 0 && j < 7 && board[i - 1][j + 1] == 'R' || i > 0 && j > 0 && board[i - 1][j + 1] == 'H' || i > 0 && j > 0 && board[i - 1][j + 1] == 'C' || i > 0 && j > 0 && board[i - 1][j + 1] == 'K' || i > 0 && j > 0 && board[i - 1][j + 1] == 'Q' || i > 0 && j > 0 && board[i - 1][j + 1] == 'P') {
  light(i-1,j+1);
  }
}

void updateShiftRegister()
{
   digitalWrite(latchPin, LOW);
   shiftOut(dataPin, clockPin, MSBFIRST,leds_line_h);
   shiftOut(dataPin, clockPin, MSBFIRST,  leds_line_g);
   shiftOut(dataPin, clockPin, MSBFIRST,  leds_line_f);
   shiftOut(dataPin, clockPin, MSBFIRST, leds_line_e);
   shiftOut(dataPin, clockPin, MSBFIRST, leds_line_d);
   shiftOut(dataPin, clockPin, MSBFIRST, leds_line_c);
   shiftOut(dataPin, clockPin, MSBFIRST, leds_line_b);  // leds b jako pierwsze bo najpierw leci od konca i przesuwa
   shiftOut(dataPin, clockPin, MSBFIRST, leds_line_a);
   
   digitalWrite(latchPin, HIGH);
}

void switchoff_light(){
//wyłącznik wszystkich swiateł
leds_line_a = 0; 
leds_line_b = 0;
leds_line_c = 0;
leds_line_d = 0;
leds_line_e = 0;
leds_line_f = 0;
leds_line_g = 0;
leds_line_h = 0;
updateShiftRegister();


}

void copy_tab(){
  for (int i=0;i<8;i++){
    for (int j=0;j<8;j++){
    tab_prev[i][j]=tab_current[i][j];
  }
}}

