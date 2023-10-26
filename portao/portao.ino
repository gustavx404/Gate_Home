// --- Defini os GPIO do Hardware usado ---
#define   control    2         
#define   endstop    A0        
#define   sentido_a  10                  
#define   sentido_b  11                  

// ---  Salva o modo de operação ---
int   mode_n = 0;              

// --- Flags que salvam o estado do sensor e o  botão ---
int controlStatus = 0;
int endstopStatus = 1;

void setup() 
{
  pinMode(sentido_a, OUTPUT);        
  pinMode(sentido_b, OUTPUT);                                 
  pinMode(control, INPUT_PULLUP);                

  digitalWrite(sentido_a, HIGH);                
  digitalWrite(sentido_b, HIGH);                   
  digitalWrite(endstop, HIGH);

  Serial.begin(9600); 
}

void loop() 
{
  // --- Seta uma variavel pro botão ---
  controlStatus = digitalRead(control);
  // --- Converte o Sensor Hall em Analogico ---
  endstopStatus = map(endstop, 0, 1023, 0, 5);
  // --- Verificação do Botão ---
  if(controlStatus == 0  )
  {
    Serial.println("Portão Acionado");
    mode_n += 1 ; 
    while (digitalRead(control) == LOW);
    if(mode_n > 3) mode_n = 0;           
    delay(250);
  }
  // --- Sensor Hall Polo Norte do Ima ---
  else if(analogRead(endstopStatus) < 400 && mode_n == 3){
    Serial.println("Hall Norte");
    mode_n = 0;
  }
  // --- Sensor Hall Polo Sul do Ima ---
  else if(analogRead(endstopStatus) > 600 && mode_n == 1){
    Serial.println("Hall Sul");
    mode_n = 2;
  }
  else{
    return;
  }
  // --- Switch de controle do motor e lado ---
  switch(mode_n)                           
  { 
    case 0: 
            digitalWrite(sentido_a,  HIGH);       
            digitalWrite(sentido_b,  HIGH);
            break;     
    case 1: 
            digitalWrite(sentido_a, HIGH);    
            digitalWrite(sentido_b,  LOW);
            break;  
    case 2:
            digitalWrite(sentido_a,  HIGH);      
            digitalWrite(sentido_b,  HIGH);
            break;
    case 3: 
            digitalWrite(sentido_a, LOW);    
            digitalWrite(sentido_b, HIGH);
            break; 
  }
}
