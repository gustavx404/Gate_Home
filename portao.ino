// ======================================================================================================
// --- Mapeamento de Hardware ---
#define   control   2                            //entrada sinal botão 
#define   endstop A0                              //entrada para fim de curso sensor hall 
#define   sentido_a  10                        //saída a para controle de direção do motor
#define   sentido_b  11                        //saída b para controle de direção do motor


int   mode_n = 0;                        //determina o modo de operação

boolean flag   = 1;                           //flag auxiliar do botão
boolean flagEnd   = 0;                           //flag auxiliar sensor hall

void setup() 
{
  pinMode(control, INPUT_PULLUP);                    //entrada botão sinal 
 
  pinMode(sentido_a, OUTPUT);                      //saída a motor
  pinMode(sentido_b, OUTPUT);                     //saída b motor

  digitalWrite(sentido_a, HIGH);                   //controle a do motor inicia em LOW
  digitalWrite(sentido_b, HIGH);                   //controle b do motor inicia em LOW
  digitalWrite(endstop, HIGH);                   // endstop sensor hall

} //end setup

void loop() 
{
    if(digitalRead(endstop) && !flagEnd)        //sinal  verdadeiro e flag limpa?  
  {
      flagEnd = 1;                            //seta flag
      mode_n = 0;                         //incrementa número do modo     
      if(mode_n > 3) mode_n = 0;           //se maior que 3, reinicia
      delay(250);                          //anti-bouncing
  } //end if

  else if(!digitalRead(endstop) && flagEnd)   //sinal falso e flag setada?
  {
      flagEnd = 0;                            //limpa flag
      mode_n = 2;                          //incrementa número do modo
      if(mode_n > 3) mode_n = 1;           //se maior que 3, reinicia
      delay(250);                          //anti-bouncing
  } //end else if
  //apartir desse ponto verdadeiro ou fauso 
  if(digitalRead(control) && !flag)        //sinal  verdadeiro e flag limpa?  
  {
      flag = 1;                            //seta flag
      mode_n += 1;                         //incrementa número do modo     
      if(mode_n > 3) mode_n = 0;           //se maior que 3, reinicia
      delay(250);                          //anti-bouncing
  } //end if

  else if(!digitalRead(control) && flag)   //sinal falso e flag setada?
  {
      flag = 0;                            //limpa flag
      mode_n += 0;                          //incrementa número do modo
      if(mode_n > 3) mode_n = 1;           //se maior que 3, reinicia
      delay(250);                          //anti-bouncing
  } //end else if
  switch(mode_n)                           //switch modo de operação
  { 
     // apartir desse ponto case 0. 1 .2. 3
    case 0: 
            digitalWrite(sentido_a,  HIGH);        //motor parado esperando comando...)
            digitalWrite(sentido_b,  HIGH);
            break;     
    case 1: 
            digitalWrite(sentido_a, HIGH);        //motor gira em um sentido (portão abrindo...)
            digitalWrite(sentido_b,  LOW);
            break;  
     case 2:
            digitalWrite(sentido_a,  HIGH);        //motor parado esperando comando...)
            digitalWrite(sentido_b,  HIGH);
            break;

    case 3: 
            digitalWrite(sentido_a, LOW);        //motor gira em outro sentido (portão fechando...)
            digitalWrite(sentido_b, HIGH);
            break;
    
      } //end switch
    
 
} //end loop