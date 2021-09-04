/* 
 *  GCC Randomizer 
 *  Last Revised 9/3/2021 
 *  
 *  Nintendo library by NicoHood (c) 2014-2016 https://github.com/NicoHood/Nintendo
 *  Entropy library: https://github.com/pmjdebruijn/Arduino-Entropy-Library
 *  
 *  This code remaps the original GCC button inputs to a random set of inputs.
 *  The randomized set is generated at power up. Reset the MCU for a newly random set.
 *  
 */

//********************************************************************************************************************************************************************
// Libraries
#include <Entropy.h>
#include <Nintendo.h>

/**
 * -------------- Byte Structure ----------------
 * 
 * Byte 1   [ * | * | * | S | Y | X | B | A ]
 * Byte 2   [ * | L | R | Z | ^ | v | > | < ]
 * Byte 3   [           Joystick X          ]
 * Byte 4   [           Joystick Y          ]
 * Byte 5   [            C-tick X           ]
 * Byte 6   [            C-tick Y           ]
 * Byte 7   [          Left Trigger         ]
 * Byte 8   [          Right Trigger        ]
 * 
 * -------------- Decimal Values ----------------
 * 
 * Y    0b00001000
 * X    0b00000100
 * B    0b00000010
 * A    0b00000001
 * -    ----------
 * L    0b01000000
 * R    0b00100000
 * Z    0b00010000
 * DU   0b00001000
 * DD   0b00000100
 * DR   0b00000010
 * DL   0b00000001
**/

//********************************************************************************************************************************************************************
// Configuration
//#define verbose_write               // Displays first 2 bytes on serial monitor
#define randomize                   // uncomment = randomize. commented = standard button map

//********************************************************************************************************************************************************************
// Object constructors
CGamecubeController GamecubeController(7);
CGamecubeConsole    GamecubeConsole(8);
Gamecube_Data_t     d = defaultGamecubeData;

//********************************************************************************************************************************************************************
// Global Variables
const size_t n = 11;
//                         {Y,X,B,A,L,R,Z,^,v,>, <}
uint8_t arry[n]          = {0,1,2,3,4,5,6,7,8,9,10};
//uint8_t shuffled_arry[n] = {10,3,8,1,5,6,7,0,9,2,4};       // Test 1
uint8_t shuffled_arry[n] = {0,1,2,3,4,5,6,7,8,9,10};       // Test 2

int8_t shiftY; uint8_t byte1modY, byte2modY;
int8_t shiftX; uint8_t byte1modX, byte2modX;
int8_t shiftB; uint8_t byte1modB, byte2modB;
int8_t shiftA; uint8_t byte1modA, byte2modA;
int8_t shiftL; uint8_t byte1modL, byte2modL;
int8_t shiftR; uint8_t byte1modR, byte2modR;
int8_t shiftZ; uint8_t byte1modZ, byte2modZ;
int8_t shiftDU; uint8_t byte1modDU, byte2modDU;
int8_t shiftDD; uint8_t byte1modDD, byte2modDD;
int8_t shiftDR; uint8_t byte1modDR, byte2modDR;
int8_t shiftDL; uint8_t byte1modDL, byte2modDL;

uint8_t byte1mod;
uint8_t byte2mod;


//********************************************************************************************************************************************************************
// Initializations & Pin Assignments
void setup(){
  //----------- Serial Communication -------------------------
  Serial.begin(115200);

  //----------- Initialize Entropy ---------------------------
  Entropy.initialize();

  //----------- Configuration Setup --------------------------
  #ifdef randomize
    Serial.println("Mode: Randomize Buttons"); Serial.println();
    shuffle();
    display_arry();
    calculate_shift();
  #else
    Serial.println("Mode: Passthrough"); Serial.println();
  #endif
  
}

//-------------------------------------------------------------------------------------------------------------------
void loop(){
  
  if(!GamecubeController.read()){
    Serial.println("Error reading controller.");
    delay(1000);
    return;
  }
  
  GamecubeController.read();  
  auto report = GamecubeController.getReport();
  d.report = report;                                // Set all buttons to original values

  #ifdef randomize
    //----------- Y Button Swap -------------------------------
    if(report.y == 1){
      
      if(byte1modY == 0 && byte2modY == 0){                   // No shift
        d.report.y = 1;
      }
      else if(byte1modY > 0 && byte2modY == 0){               // Shift within byte 1 (left or right)
        d.report.y = 0;
        d.report.buttons0 |= (0b00010000 >> byte1modY);
      }
      else if(byte1modY == 0 && byte2modY > 0){               // Jump into byte 2
        d.report.y = 0;
        d.report.buttons1 |= (0b10000000 >> byte2modY);
      }
  
    }
    //----------- X Button Swap -------------------------------
    if(report.x == 1){
      
      if(byte1modX == 0 && byte2modX == 0){                   // No shift
        d.report.x = 1;
      }
      else if(byte1modX > 0 && byte2modX == 0){               // Shift within byte 1 (left or right)
        d.report.x = 0;
        d.report.buttons0 |= (0b00010000 >> byte1modX);
      }
      else if(byte1modX == 0 && byte2modX > 0){               // Jump into byte 2
        d.report.x = 0;
        d.report.buttons1 |= (0b10000000 >> byte2modX);
      }
  
    }
    //----------- B Button Swap -------------------------------
    if(report.b == 1){
      
      if(byte1modB == 0 && byte2modB == 0){                   // No shift
        d.report.b = 1;
      }
      else if(byte1modB > 0 && byte2modB == 0){               // Shift within byte 1 (left or right)
        d.report.b = 0;
        d.report.buttons0 |= (0b00010000 >> byte1modB);
      }
      else if(byte1modB == 0 && byte2modB > 0){               // Jump into byte 2
        d.report.b = 0;
        d.report.buttons1 |= (0b10000000 >> byte2modB);
      }
  
    }
    //----------- A Button Swap -------------------------------
    if(report.a == 1){
      
      if(byte1modA == 0 && byte2modA == 0){                   // No shift
        d.report.a = 1;
      }
      else if(byte1modA > 0 && byte2modA == 0){               // Shift within byte 1 (left or right)
        d.report.a = 0;
        d.report.buttons0 |= (0b00010000 >> byte1modA);
      }
      else if(byte1modA == 0 && byte2modA >> 0){               // Jump into byte 2
        d.report.a = 0;
        d.report.buttons1 |= (0b10000000 >> byte2modA);
      }
  
    }
    //----------- L Button Swap -------------------------------
    if(report.l == 1){
      
      if(byte1modL == 0 && byte2modL == 0){                   // No shift
        d.report.l = 1;
      }
      else if(byte1modL == 0 && byte2modL > 0){               // Shift within byte 2 (left or right)
        d.report.l = 0;
        d.report.buttons1 |= (0b10000000 >> byte2modL);
      }
      else if(byte1modL > 0 && byte2modL == 0){               // Jump into byte A
        d.report.l = 0;
        d.report.buttons0 |= (0b00010000 >> byte1modL);
      }
      
    }
    //----------- R Button Swap -------------------------------
    if(report.r == 1){
      
      if(byte1modR == 0 && byte2modR == 0){                   // No shift
        d.report.r = 1;
      }
      else if(byte1modR == 0 && byte2modR > 0){               // Shift within byte 2 (left or right)
        d.report.r = 0;
        d.report.buttons1 |= (0b10000000 >> byte2modR);
      }
      else if(byte1modR > 0 && byte2modR == 0){               // Jump into byte A
        d.report.r = 0;
        d.report.buttons0 |= (0b00010000 >> byte1modR);
      }
      
    }
    //----------- Z Button Swap -------------------------------
    if(report.z == 1){
      
      if(byte1modZ == 0 && byte2modZ == 0){                   // No shift
        d.report.z = 1;
      }
      else if(byte1modZ == 0 && byte2modZ > 0){               // Shift within byte 2 (left or right)
        d.report.z = 0;
        d.report.buttons1 |= (0b10000000 >> byte2modZ);
      }
      else if(byte1modZ > 0 && byte2modZ == 0){               // Jump into byte A
        d.report.z = 0;
        d.report.buttons0 |= (0b00010000 >> byte1modZ);
      }
      
    }
    //----------- DU Button Swap -------------------------------
    if(report.dup == 1){
      
      if(byte1modDU == 0 && byte2modDU == 0){                   // No shift
        d.report.dup = 1;
      }
      else if(byte1modDU == 0 && byte2modDU > 0){               // Shift within byte 2 (left or right)
        d.report.dup = 0;
        d.report.buttons1 |= (0b10000000 >> byte2modDU);
      }
      else if(byte1modDU > 0 && byte2modDU == 0){               // Jump into byte 1
        d.report.dup = 0;
        d.report.buttons0 |= (0b00010000 >> byte1modDU);
      }
      
    }
    //----------- DD Button Swap -------------------------------
    if(report.ddown == 1){
      
      if(byte1modDD == 0 && byte2modDD == 0){                   // No shift
        d.report.ddown = 1;
      }
      else if(byte1modDD == 0 && byte2modDD > 0){               // Shift within byte 2 (left or right)
        d.report.ddown = 0;
        d.report.buttons1 |= (0b10000000 >> byte2modDD);
      }
      else if(byte1modDD > 0 && byte2modDD == 0){               // Jump into byte 1
        d.report.ddown = 0;
        d.report.buttons0 |= (0b00010000 >> byte1modDD);
      }
      
    }
    //----------- DR Button Swap -------------------------------
    if(report.dright == 1){
      
      if(byte1modDR == 0 && byte2modDR == 0){                   // No shift
        d.report.dright = 1;
      }
      else if(byte1modDR == 0 && byte2modDR > 0){               // Shift within byte 2 (left or right)
        d.report.dright = 0;
        d.report.buttons1 |= (0b10000000 >> byte2modDR);
      }
      else if(byte1modDR > 0 && byte2modDR == 0){               // Jump into byte 1
        d.report.dright = 0;
        d.report.buttons0 |= (0b00010000 >> byte1modDR);
      }
      
    }
    //----------- DL Button Swap -------------------------------
    if(report.dleft == 1){
      
      if(byte1modDL == 0 && byte2modDL == 0){                   // No shift
        d.report.dleft = 1;
      }
      else if(byte1modDL == 0 && byte2modDL > 0){               // Shift within byte 2 (left or right)
        d.report.dleft = 0;
        d.report.buttons1 |= (0b10000000 >> byte2modDL);
      }
      else if(byte1modDL > 0 && byte2modDL == 0){               // Jump into byte 1
        d.report.dleft = 0;
        d.report.buttons0 |= (0b00010000 >> byte1modDL);
      }
      
    }

    // Turn off analog triggers
    d.report.left = 0;
    d.report.right = 0;

    // Leave joysticks and start buttons unchanged
    d.report.start = report.start;
    d.report.xAxis = report.xAxis;
    d.report.yAxis = report.yAxis;
    d.report.cxAxis = report.cxAxis;
    d.report.cyAxis = report.cyAxis;
  #endif

  #ifdef verbose_write
    Serial.print("Sent to console: "); Serial.print(d.report.buttons0, BIN);
    Serial.print("   "); Serial.println(d.report.buttons1, BIN);
  #endif
  
  GamecubeConsole.write(d);
  if(!GamecubeConsole.write(d)){
    Serial.println("Error writing to console.");
    delay(1000);
  }
  
}

//-------------------------------------------------------------------------------------------------------------------
void shuffle(){
  for(size_t i = 0; i < n - 1; i++){
    size_t j = Entropy.random(0, 11);     // Random value between [0,11)

    int t = shuffled_arry[i];
    shuffled_arry[i] = shuffled_arry[j];
    shuffled_arry[j] = t;
  }
}

//-------------------------------------------------------------------------------------------------------------------
void display_arry(){
  // Decoded buttons
  Serial.println("                 {Y, X, B, A | L, R, Z, ^, v, >,  <}");

  // Display original array
  Serial.print("Original Array = {");
  for(size_t k = 0; k < n; k++){
    Serial.print(arry[k]); 
    if(k < n-1){
      if(k == 3){
        Serial.print(" | ");
      }
      else{     
        Serial.print(", ");
      }
    }
    else{
      Serial.println("}");
    }
  }
  
  // Display shuffled array
  Serial.print("Shuffled Array = {");
  for(size_t k = 0; k < n; k++){
    Serial.print(shuffled_arry[k]); 
    if(k < n-1){
      if(k == 3){
        Serial.print(" | ");
      }
      else{     
        Serial.print(", ");
      }
    }
    else{
      Serial.println("}");
    }
  }
  Serial.println();
}

//-------------------------------------------------------------------------------------------------------------------
void calculate_shift(){
  for(int i=0; i<11; i++){
    int ogloc = i;
    int count = 0;
    int newloc;
    do{
      newloc = shuffled_arry[count];
      count++;
    }while(newloc != ogloc);
    count--;                  // Must subtract 1 because loop operation occurs before condition is checked, overshooting by 1

    //----------- Y Shift Logic -------------------------------
    if(ogloc == 0){
      shiftY = count-ogloc;        // Gives number of positions that Y has moved right(+) or left(-) in the shuffled array
      Serial.print("Y");

      //Profile A
      if(shiftY == 0){
        // Case 0: No change
        byte1modY = 0;
        byte2modY = 0;
        Serial.print(" | Case 0");
      }
      else if(shiftY < 0){
        // Case 1: Shift left
        byte1modY = shiftY + ogloc + 1;
        byte2modY = 0;
        Serial.print(" | Case 1");
      }
      else if(shiftY > 0 && shiftY < 4 - ogloc){
        // Case 2: Shift right, within byte 1
        byte1modY = shiftY + ogloc + 1;
        byte2modY = 0;
        Serial.print(" | Case 2");
      }
      else if(shiftY > 0 && shiftY >= 4 - ogloc){
        // Case 3: Shift right, jump to byte 2
        byte1modY = 0;
        byte2modY = shiftY - (4 - ogloc) + 1;
        Serial.print(" | Case 3");
      }
      Serial.print(" | shift: "); Serial.print(shiftY);    
      Serial.print(" | byte1mod: "); Serial.print(byte1modY);
      Serial.print(" | byte2mod: "); Serial.print(byte2modY);
      Serial.println();
      
    }
    
    //----------- X Shift Logic -------------------------------
    if(ogloc == 1){
      shiftX = count-ogloc;        // Gives number of positions that X has moved right(+) or left(-) in the shuffled array
      Serial.print("X");

      //Profile A
      if(shiftX == 0){
        // Case 0: No change
        byte1modX = 0;
        byte2modX = 0;
        Serial.print(" | Case 0");
      }
      else if(shiftX < 0){
        // Case 1: Shift left
        byte1modX = shiftX + ogloc + 1;
        byte2modX = 0;
        Serial.print(" | Case 1");
      }
      else if(shiftX > 0 && shiftX < 4 - ogloc){
        // Case 2: Shift right, within byte 1
        byte1modX = shiftX + ogloc + 1;
        byte2modX = 0;
        Serial.print(" | Case 2");
      }
      else if(shiftX > 0 && shiftX >= 4 - ogloc){
        // Case 3: Shift right, jump to byte 2
        byte1modX = 0;
        byte2modX = shiftX - (4 - ogloc) + 1;
        Serial.print(" | Case 3");
      }
      Serial.print(" | shift: "); Serial.print(shiftX);    
      Serial.print(" | byte1mod: "); Serial.print(byte1modX);
      Serial.print(" | byte2mod: "); Serial.print(byte2modX);
      Serial.println();
    }
    
    //----------- B Shift Logic -------------------------------
    if(ogloc == 2){
      shiftB = count-ogloc;        // Gives number of positions that B has moved right(+) or left(-) in the shuffled array
      Serial.print("B");

      //Profile A
      if(shiftB == 0){
        // Case 0: No change
        byte1modB = 0;
        byte2modB = 0;
        Serial.print(" | Case 0");
      }
      else if(shiftB < 0){
        // Case 1: Shift left
        byte1modB = shiftB + ogloc + 1;
        byte2modB = 0;
        Serial.print(" | Case 1");
      }
      else if(shiftB > 0 && shiftB < 4 - ogloc){
        // Case 2: Shift right, within byte 1
        byte1modB = shiftB + ogloc + 1;
        byte2modB = 0;
        Serial.print(" | Case 2");
      }
      else if(shiftB > 0 && shiftB >= 4 - ogloc){
        // Case 3: Shift right, jump to byte 2
        byte1modB = 0;
        byte2modB = shiftB - (4 - ogloc) + 1;
        Serial.print(" | Case 3");
      }
      Serial.print(" | shift: "); Serial.print(shiftB);    
      Serial.print(" | byte1mod: "); Serial.print(byte1modB);
      Serial.print(" | byte2mod: "); Serial.print(byte2modB);
      Serial.println();
    }

    //----------- A Shift Logic -------------------------------
    if(ogloc == 3){
      shiftA = count-ogloc;        // Gives number of positions that A has moved right(+) or left(-) in the shuffled array
      Serial.print("A");

      //Profile A
      if(shiftA == 0){
        // Case 0: No change
        byte1modA = 0;
        byte2modA = 0;
        Serial.print(" | Case 0");
      }
      else if(shiftA < 0){
        // Case 1: Shift left
        byte1modA = shiftA + ogloc + 1;
        byte2modA = 0;
        Serial.print(" | Case 1");
      }
      else if(shiftA > 0 && shiftA < 4 - ogloc){
        // Case 2: Shift right, within byte 1
        byte1modA = shiftA + ogloc + 1;
        byte2modA = 0;
        Serial.print(" | Case 2");
      }
      else if(shiftA > 0 && shiftA >= 4 - ogloc){
        // Case 3: Shift right, jump to byte 2
        byte1modA = 0;
        byte2modA = shiftA - (4 - ogloc) + 1;
        Serial.print(" | Case 3");
      }
      Serial.print(" | shift: "); Serial.print(shiftA);    
      Serial.print(" | byte1mod: "); Serial.print(byte1modA);
      Serial.print(" | byte2mod: "); Serial.print(byte2modA);
      Serial.println();
    }

    //----------- L Shift Logic -------------------------------
    if(ogloc == 4){
      shiftL = count-ogloc;        // Gives number of positions that L has moved right(+) or left(-) in the shuffled array
      Serial.print("L");

      //Profile B
      if(shiftL == 0){
        // Case 0: No change
        byte1modL = 0;
        byte2modL = 0;
        Serial.print(" | Case 0");
      }
      else if(shiftL > 0){
        // Case 1: Shift right
        byte1modL = 0;
        byte2modL = shiftL + (ogloc - 4) + 1;
        Serial.print(" | Case 1");
      }
      else if(shiftL < 0 && shiftL >= 4 - ogloc){
        // Case 2: Shift left, within byte 2
        byte1modL = 0;
        byte2modL = shiftL + (ogloc - 4) + 1;
        Serial.print(" | Case 2");
      }
      else if(shiftL < 0 && shiftL < 4 - ogloc){
        // Case 3: Shift left, jump to byte 1
        byte1modL = shiftL + ogloc + 1;
        byte2modL = 0;
        Serial.print(" | Case 3");
      }
      Serial.print(" | shift: "); Serial.print(shiftL);    
      Serial.print(" | byte1mod: "); Serial.print(byte1modL);
      Serial.print(" | byte2mod: "); Serial.print(byte2modL);
      Serial.println();
    }

    //----------- R Shift Logic -------------------------------
    if(ogloc == 5){
      shiftR = count-ogloc;        // Gives number of positions that R has moved right(+) or left(-) in the shuffled array
      Serial.print("R");

      //Profile B
      if(shiftR == 0){
        // Case 0: No change
        byte1modR = 0;
        byte2modR = 0;
        Serial.print(" | Case 0");
      }
      else if(shiftR > 0){
        // Case 1: Shift right
        byte1modR = 0;
        byte2modR = shiftR + (ogloc - 4) + 1;
        Serial.print(" | Case 1");
      }
      else if(shiftR < 0 && shiftR >= 4 - ogloc){
        // Case 2: Shift left, within byte 2
        byte1modR = 0;
        byte2modR = shiftR + (ogloc - 4) + 1;
        Serial.print(" | Case 2");
      }
      else if(shiftR < 0 && shiftR < 4 - ogloc){
        // Case 3: Shift left, jump to byte 1
        byte1modR = shiftR + ogloc + 1;
        byte2modR = 0;
        Serial.print(" | Case 3");
      }
      Serial.print(" | shift: "); Serial.print(shiftR);    
      Serial.print(" | byte1mod: "); Serial.print(byte1modR);
      Serial.print(" | byte2mod: "); Serial.print(byte2modR);
      Serial.println();
    }

    //----------- Z Shift Logic -------------------------------
    if(ogloc == 6){
      shiftZ = count-ogloc;        // Gives number of positions that Z has moved right(+) or left(-) in the shuffled array
      Serial.print("Z");

      //Profile B
      if(shiftZ == 0){
        // Case 0: No change
        byte1modZ = 0;
        byte2modZ = 0;
        Serial.print(" | Case 0");
      }
      else if(shiftZ > 0){
        // Case 1: Shift right
        byte1modZ = 0;
        byte2modZ = shiftZ + (ogloc - 4) + 1;
        Serial.print(" | Case 1");
      }
      else if(shiftZ < 0 && shiftZ >= 4 - ogloc){
        // Case 2: Shift left, within byte 2
        byte1modZ = 0;
        byte2modZ = shiftZ + (ogloc - 4) + 1;
        Serial.print(" | Case 2");
      }
      else if(shiftZ < 0 && shiftZ < 4 - ogloc){
        // Case 3: Shift left, jump to byte 1
        byte1modZ = shiftZ + ogloc + 1;
        byte2modZ = 0;
        Serial.print(" | Case 3");
      }
      Serial.print(" | shift: "); Serial.print(shiftZ);    
      Serial.print(" | byte1mod: "); Serial.print(byte1modZ);
      Serial.print(" | byte2mod: "); Serial.print(byte2modZ);
      Serial.println();
    }

    //----------- DU Shift Logic -------------------------------
    if(ogloc == 7){
      shiftDU = count-ogloc;        // Gives number of positions that DU has moved right(+) or left(-) in the shuffled array
      Serial.print("DU");

      //Profile B
      if(shiftDU == 0){
        // Case 0: No change
        byte1modDU = 0;
        byte2modDU = 0;
        Serial.print(" | Case 0");
      }
      else if(shiftDU > 0){
        // Case 1: Shift right
        byte1modDU = 0;
        byte2modDU = shiftDU + (ogloc - 4) + 1;
        Serial.print(" | Case 1");
      }
      else if(shiftDU < 0 && shiftDU >= 4 - ogloc){
        // Case 2: Shift left, within byte 2
        byte1modDU = 0;
        byte2modDU = shiftDU + (ogloc - 4) + 1;
        Serial.print(" | Case 2");
      }
      else if(shiftDU < 0 && shiftDU < 4 - ogloc){
        // Case 3: Shift left, jump to byte 1
        byte1modDU = shiftDU + ogloc + 1;
        byte2modDU = 0;
        Serial.print(" | Case 3");
      }
      Serial.print(" | shift: "); Serial.print(shiftDU);    
      Serial.print(" | byte1mod: "); Serial.print(byte1modDU);
      Serial.print(" | byte2mod: "); Serial.print(byte2modDU);
      Serial.println();
    }

    //----------- DD Shift Logic -------------------------------
    if(ogloc == 8){
      shiftDD = count-ogloc;        // Gives number of positions that DD has moved right(+) or left(-) in the shuffled array
      Serial.print("DD");

      //Profile B
      if(shiftDD == 0){
        // Case 0: No change
        byte1modDD = 0;
        byte2modDD = 0;
        Serial.print(" | Case 0");
      }
      else if(shiftDD > 0){
        // Case 1: Shift right
        byte1modDD = 0;
        byte2modDD = shiftDD + (ogloc - 4) + 1;
        Serial.print(" | Case 1");
      }
      else if(shiftDD < 0 && shiftDD >= 4 - ogloc){
        // Case 2: Shift left, within byte 2
        byte1modDD = 0;
        byte2modDD = shiftDD + (ogloc - 4) + 1;
        Serial.print(" | Case 2");
      }
      else if(shiftDD < 0 && shiftDD < 4 - ogloc){
        // Case 3: Shift left, jump to byte 1
        byte1modDD = shiftDD + ogloc + 1;
        byte2modDD = 0;
        Serial.print(" | Case 3");
      }
      Serial.print(" | shift: "); Serial.print(shiftDD);    
      Serial.print(" | byte1mod: "); Serial.print(byte1modDD);
      Serial.print(" | byte2mod: "); Serial.print(byte2modDD);
      Serial.println();
    }

    //----------- DR Shift Logic -------------------------------
    if(ogloc == 9){
      shiftDR = count-ogloc;        // Gives number of positions that DR has moved right(+) or left(-) in the shuffled array
      Serial.print("DR");

      //Profile B
      if(shiftDR == 0){
        // Case 0: No change
        byte1modDR = 0;
        byte2modDR = 0;
        Serial.print(" | Case 0");
      }
      else if(shiftDR > 0){
        // Case 1: Shift right
        byte1modDR = 0;
        byte2modDR = shiftDR + (ogloc - 4) + 1;
        Serial.print(" | Case 1");
      }
      else if(shiftDR < 0 && shiftDR >= 4 - ogloc){
        // Case 2: Shift left, within byte 2
        byte1modDR = 0;
        byte2modDR = shiftDR + (ogloc - 4) + 1;
        Serial.print(" | Case 2");
      }
      else if(shiftDR < 0 && shiftDR < 4 - ogloc){
        // Case 3: Shift left, jump to byte 1
        byte1modDR = shiftDR + ogloc + 1;
        byte2modDR = 0;
        Serial.print(" | Case 3");
      }
      Serial.print(" | shift: "); Serial.print(shiftDR);    
      Serial.print(" | byte1mod: "); Serial.print(byte1modDR);
      Serial.print(" | byte2mod: "); Serial.print(byte2modDR);
      Serial.println();
    }

    //----------- DL Shift Logic -------------------------------
    if(ogloc == 10){
      shiftDL = count-ogloc;        // Gives number of positions that DL has moved right(+) or left(-) in the shuffled array
      Serial.print("DL");

      //Profile B
      if(shiftDL == 0){
        // Case 0: No change
        byte1modDL = 0;
        byte2modDL = 0;
        Serial.print(" | Case 0");
      }
      else if(shiftDL > 0){
        // Case 1: Shift right
        byte1modDL = 0;
        byte2modDL = shiftDL + (ogloc - 4) + 1;
        Serial.print(" | Case 1");
      }
      else if(shiftDL < 0 && shiftDL >= 4 - ogloc){
        // Case 2: Shift left, within byte 2
        byte1modDL = 0;
        byte2modDL = shiftDL + (ogloc - 4) + 1;
        Serial.print(" | Case 2");
      }
      else if(shiftDL < 0 && shiftDL < 4 - ogloc){
        // Case 3: Shift left, jump to byte 1
        byte1modDL = shiftDL + ogloc + 1;
        byte2modDL = 0;
        Serial.print(" | Case 3");
      }
      Serial.print(" | shift: "); Serial.print(shiftDL);    
      Serial.print(" | byte1mod: "); Serial.print(byte1modDL);
      Serial.print(" | byte2mod: "); Serial.print(byte2modDL);
      Serial.println();
    }
  }
}
