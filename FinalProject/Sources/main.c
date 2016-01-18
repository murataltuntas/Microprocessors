#include <hidef.h> //* common defines and macros 
#include "derivative.h" //* derivative-specific definitions 
#include <stdlib.h>

#define enable() __asm(cli)
#define disable() __asm(sei)
#define NUMBER_OF_WORDS 32
#define NUMBER_OF_GAME 5
#define ROW 4
#define COL 4 

void MSDelay(unsigned int);
void tahmin(unsigned int scnk);
void MSBuzzer(unsigned int itime);
int GetKey() ;
interrupt (((0x10000 - Vrti)/2)-1) void kelimeRti(void);

unsigned int chooser = 0;     // secim
static unsigned int score = 0;// oyun skoru 
unsigned int scrCount = 0;    // score sayacý
unsigned int find = 0;        // harfin bulundugunu bildiren degisken
unsigned int oyunSayisi =0;   // oyun sayýsýný sayan sayac
unsigned int hataCount =0;    // hata sayýsýný sayan sayac
unsigned int congCount = 0;   // congratulations ý ayarlayan sayac
unsigned int goodCount = 0;   // goodbye ý ayarlayan sayac
unsigned int oyunCounter = 1; // kac oyun oynanacagini sayan sayac
unsigned int back = 0;        // en son girilen harfi tutar

// 18 harf
//Harf //| A  | b  | c  | d  | E  | F  | G  | H  | ýi | L  | n  | oö | P  | r  | SÞ | t  | uü | y
//Hex  //| 41 | 42 | 43 | 44 | 45 | 46 | 47 | 48 | 49 | 4C | 4E | 4F | 50 | 52 | 53 | 54 | 55 | 59
//Dec  //| 65 | 66 | 67 | 68 | 69 | 70 | 71 | 72 | 73 | 76 | 78 | 79 | 80 | 82 | 83 | 84 | 85 | 89
//7Seg //| 77 | 7C | 58 | 5E | 79 | 71 | 7D | 76 | 04 | 38 | 54 | 5C | 73 | 50 | 6D | 78 | 1C | 6E

// 7-segment
const char kelimelerSEG[NUMBER_OF_WORDS][4] ={{0x78, 0x79, 0x50, 0x6D}, // Ters
                                              {0x73, 0x1C, 0x6D, 0x1C}, // Pusu
                                              {0x50, 0x77, 0x54, 0x78}, // Rant
                                              {0x6d, 0x04, 0x50, 0x78}, // Sýrt
                                              {0x7d, 0x5c, 0x38, 0x71}, // Golf
                                              {0x7C, 0x77, 0x7C, 0x77}, // Baba
                                              {0x5E, 0x79, 0x5E, 0x79}, // Dede
                                              {0x78, 0x1C, 0x50, 0x73}, // Turp
                                              {0x5E, 0x5C, 0x6D, 0x78}, // Dost
                                              {0x5E, 0x79, 0x50, 0x78}, // Dert
                                              {0x78, 0x79, 0x6D, 0x78}, // Test
                                              {0x78, 0x5C, 0x6D, 0x78}, // Tost
                                              {0x6E, 0x77, 0x6D, 0x77}, // Yasa
                                              {0x6D, 0x77, 0x38, 0x04}, // Salý
                                              {0x71, 0x79, 0x5E, 0x77}, // Feda
                                              {0x7C, 0x79, 0x50, 0x79}, // Bere
                                              {0x6D, 0x79, 0x5E, 0x77}, // Seda
                                              {0x7C, 0x5C, 0x6E, 0x77}, // Boya
                                              {0x7C, 0x5C, 0x50, 0x1C}, // boru
                                              {0x76, 0x77, 0x38, 0x04}, // Halý
                                              {0x77, 0x54, 0x54, 0x79}, // Anne
                                              {0x6D, 0x79, 0x38, 0x79}, // Sele
                                              {0x78, 0x77, 0x6E, 0x78}, // Tayt 
                                              {0x73, 0x5C, 0x78, 0x77}, // Pota
                                              {0x78, 0x77, 0x76, 0x78}, // Taht
                                              {0x78, 0x77, 0x54, 0x79}, // Tane
                                              {0x5C, 0x78, 0x79, 0x38}, // Otel
                                              {0x79, 0x6D, 0x79, 0x50}, // Eser
                                              {0x6D, 0x77, 0x77, 0x78}, // Saat
                                              {0x7C, 0x77, 0x54, 0x78}, // Bant 
                                              {0x5E, 0x77, 0x50, 0x73}, // Darp
                                              {0x6D, 0x77, 0x50, 0x73}  // Sarp
                                             };

// decimal                               
const char kelimelerDEC[NUMBER_OF_WORDS][4] ={{84,69,82,83},  // Ters
                                              {80,85,83,85},  // Pusu
                                              {82,65,78,84},  // Rant
                                              {83,73,82,84},  // Sýrt
                                              {71,79,76,70},  // Golf
                                              {66,65,66,65},  // Baba
                                              {68,69,68,69},  // Dede
                                              {84,85,82,80},  // Turp
                                              {68,79,83,84},  // Dost
                                              {68,69,82,84},  // Dert
                                              {84,69,83,84},  // Test
                                              {84,79,83,84},  // Tost
                                              {89,65,83,65},  // Yasa
                                              {83,65,76,73},  // Salý
                                              {70,69,68,65},  // Feda
                                              {66,69,82,69},  // Bere
                                              {83,69,68,65},  // Seda
                                              {66,79,89,65},  // Boya
                                              {66,79,82,85},  // Boru
                                              {72,65,76,73},  // Halý
                                              {65,78,78,69},  // Anne 
                                              {83,69,76,69},  // Sele
                                              {84,65,89,84},  // Tayt
                                              {80,79,84,65},  // Pota
                                              {84,65,72,84},  // Taht
                                              {84,65,78,69},  // Tane
                                              {79,84,69,76},  // Otel
                                              {69,83,69,82},  // Eser
                                              {83,65,65,84},  // Saat
                                              {66,65,78,84},  // Bant
                                              {68,65,82,80},  // Darp
                                              {83,65,82,80}   // Sarp
                                             };   
                               
// congratulations
const char congratSEG[33][4] ={{0x00, 0x58, 0x5c, 0x54},
                              {0x58, 0x5c, 0x54, 0x7D},
                              {0x5C, 0x54, 0x7D, 0x50},
                              {0x54, 0x7D, 0x50, 0x77},
                              {0x7D, 0x50, 0x77, 0x78},
                              {0x50, 0x77, 0x78, 0x1C},
                              {0x77, 0x78, 0x1C, 0x38},
                              {0x78, 0x1C, 0x38, 0x77},
                              {0x1C, 0x38, 0x77, 0x78},
                              {0x38, 0x77, 0x78, 0x04},
                              {0x77, 0x78, 0x04, 0x5C},
                              {0x78, 0x04, 0x5C, 0x54},
                              {0x04, 0x5c, 0x54, 0x6D},
                              {0x5c, 0x54, 0x6D, 0x00},
                              {0x54, 0x6D, 0x00, 0x58},
                              {0x6D, 0x00, 0x58, 0x5c},
                              {0x00, 0x58, 0x5c, 0x54},
                              {0x58, 0x5c, 0x54, 0x7D},
                              {0x5C, 0x54, 0x7D, 0x50},
                              {0x54, 0x7D, 0x50, 0x77},
                              {0x7D, 0x50, 0x77, 0x78},
                              {0x50, 0x77, 0x78, 0x1C},
                              {0x77, 0x78, 0x1C, 0x38},
                              {0x78, 0x1C, 0x38, 0x77},
                              {0x1C, 0x38, 0x77, 0x78},
                              {0x38, 0x77, 0x78, 0x04},
                              {0x77, 0x78, 0x04, 0x5C},
                              {0x78, 0x04, 0x5C, 0x54},
                              {0x04, 0x5c, 0x54, 0x6D},
                              {0x5C, 0x54, 0x6D, 0x00},
                              {0x54, 0x6D, 0x00, 0x00},
                              {0x6D, 0x00, 0x00, 0x00},
                              {0x00, 0x00, 0x00, 0x00}
                              };
// GOODBYE                             
const char goodByeSEG[17][4] ={{0x00, 0x7D, 0x5c, 0x5C},
                              {0x7D, 0x5c, 0x5C, 0x5E},
                              {0x5C, 0x5C, 0x5E, 0x7C},
                              {0x5C, 0x5E, 0x7C, 0x6E},
                              {0x5E, 0x7C, 0x6E, 0x79},
                              {0x7C, 0x6E, 0x79, 0x00},
                              {0x6E, 0x79, 0x00, 0x7D},
                              {0x79, 0x00, 0x7D, 0x5C},
                              {0x00, 0x7D, 0x5c, 0x5C},
                              {0x7D, 0x5c, 0x5C, 0x5E},
                              {0x5C, 0x5C, 0x5E, 0x7C},
                              {0x5C, 0x5E, 0x7C, 0x6E},
                              {0x5E, 0x7C, 0x6E, 0x79},
                              {0x7C, 0x6E, 0x79, 0x00},
                              {0x6E, 0x79, 0x00, 0x00},
                              {0x79, 0x00, 0x00, 0x00},
                              {0x00, 0x00, 0x00, 0x00}                               
                              };
                                                            
const char winSEG[] = {0x3C, 0x1E, 0x90, 0x54};   // WÝN
const char loseSEG[] = {0x38, 0x3f, 0x6d, 0x78};  // LOST
const char finishSEG[] = {0x00, 0x00, 0x00, 0x00};//                         
unsigned int cvp[] = {0, 0, 0, 0};    
                                                               
void main(void)
{
  unsigned int readDPS;
  unsigned int temp = 1;
  unsigned int sayi1;
  unsigned int i=0;
  unsigned int j=0;
  unsigned int kazanmaDrumu =0;
  unsigned int kazandi=0;
  
  disable();

  
  DDRA=0x0F;  // pb0-3 is out, others input
  DDRB=0xff;                  // 
  DDRP=0xff;                  // 7-segment icin
  DDRH=0x00;    
  DDRT = DDRT | 0b00100000;    // PTT5 as output  (Buzzer icin)
  
  PORTB=0;                                  
  
  TSCR1 =TSCR1 | 0x80; // Enable timer subsystem  
  TSCR2 = 0x05;        // Set overflow time to 87 ms 
  TFLG2 = 0x80;        // Make sure TOF bit clear   
      
  RTICTL=0x31;          //pre scale 2048
  CRGINT=0x80;          //RTI
  CRGFLG=0x80;          //RTI flag
       
  enable();
  
  while (1)
  {
      /* cvp arayi sýfýrlanir */
      for(i=0; i<4; ++i){
         cvp[i] = 0;   
      }
      
      oyunSayisi = (TCNT+temp)%NUMBER_OF_WORDS; // random sayý üretmek icin
      
      back = 0;
      kazandi=1;      
      kazanmaDrumu =0;
      hataCount = 0;
      chooser = 0;     // score u goster
      MSDelay(2000);   // 2 sn boyunca
      
      
      while(1)
      {
      
        chooser = 1;     // kelimenin dogru bilinen harflerini gosterir.
        //MSDelay(2000);   // 2 sn bekle kullanýcý giris yapsýn
                  
        while((sayi1 = GetKey()) == -1);           
      	MSDelay(155);
        sayi1 *= 10;
        
        while((readDPS = GetKey()) == -1);       
        MSDelay(155);
        readDPS += sayi1;
        temp = readDPS;
        
        tahmin(readDPS); // tahmin doðrumu diye check et                         
        chooser=1;       // kelimenin dogru bilinen harflerini gosterir.
        MSDelay(500);   // 500 ms boyunca goster
        
        
            /* cvp arrayi tam doluysa cevap bilinmistir. */
            for(i=0; i<4; ++i){
                 if(cvp[i] == 1){
                    ++kazanmaDrumu;
                 }
            }
            
            /* kazanma durumunda yapilacak olanlar */
            if(kazanmaDrumu == 4){  
                 ++score; 
                 kazandi=0;   
                 chooser=1;       // kelimenin dogru bilinen harflerini gosterir.
                 MSDelay(2000);   // 3 sn boyunca goster               
                 chooser = 4;  // win
                 MSDelay(1000); 
                 chooser = 0;  // score
                 MSDelay(1000);
                 chooser = 4;  // win
                 MSDelay(1000);  
                 break;
                                  
            }else{
                kazanmaDrumu=0;
            }
          
          /* 6 kez yanlis girilme durumunda programdan cik. */ 
          if(hataCount >= 6){
            break;
          }
      }
          
            if(kazanmaDrumu < 4 || hataCount >= 6){
                 chooser = 0;  // score
                 MSDelay(1000);
                 chooser = 7;  // cevap
                 MSDelay(2500);
                 chooser = 5;  // lose
                 MSDelay(1000); 
                 chooser = 0;  // score
                 MSDelay(1000);
                 chooser = 5;  // lose
                 MSDelay(1000); 
                 chooser = 0;  // score
                 MSDelay(1000);
                 chooser = 5;  // lose
                 MSDelay(1000); 
                 chooser = 0;  // score
                 MSDelay(1000);
                 chooser = 5;  // lose
                 MSDelay(1000); 
                 
                 chooser = 9;  // GOODBYE
                 
                 // goodbye yazan dongu
                 while(1){                           
                                 
                       MSDelay(500);
                       PTT = PTT | 0x20;      //make PT5=1
                       MSDelay(10);         //change the delay size to see what happens
                       PTT = PTT & 0xDF;      //Make PT5=0
                       MSDelay(10);         //change delay size....
                       ++goodCount;
                       
                       if(goodCount>=17){
                           break;
                       }
                 }                  
                                    
                 chooser = 6;  // finish
                 MSDelay(100);                                                                  
                 break;
            }
      
            
            if(oyunCounter >= NUMBER_OF_GAME){
                 chooser = 0;  // score
                 MSDelay(1000);
                 chooser = 4;  // win
                 MSDelay(1000); 
                 chooser = 0;  // score
                 MSDelay(1000);
                 chooser = 4;  // win
                 MSDelay(1000); 
                 chooser = 0;  // score
                 MSDelay(1000);
                 chooser = 4;  // win
                 MSDelay(1000); 
                 chooser = 0;  // score
                 MSDelay(1000);
                 chooser = 4;  // win
                 MSDelay(1000); 
                 chooser = 6;  // finish
                 MSDelay(100);                           
                 chooser = 8;  // congraluations
                 
                 // congraluations yazan dongu
                 while(1){                           
                                 
                       MSDelay(500);
                       PTT = PTT | 0x20;      //make PT5=1
                       MSDelay(10);         //change the delay size to see what happens
                       PTT = PTT & 0xDF;      //Make PT5=0
                       MSDelay(10);         //change delay size....
                       ++congCount;
                       
                       if(congCount>=33){
                           break;
                       }
                 }
                 
                                                    
                 chooser = 6;  // finish
                 MSDelay(100);             
                 break;
            
            }
            
     ++oyunCounter;                               
 
  }
  
  __asm(swi);
}

/*######  INTERRUPT icin  #####*/

interrupt (((0x10000 - Vrti)/2)-1) void kelimeRti(void){
    static unsigned char nibble=0;
                                         
    
    const char hex2seven_seg[] = {0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D,
                                  0x7D, 0x07, 0x7F, 0x6F, 0x77, 0x7c,
                                  0x58, 0x5e, 0x79, 0x71};
      if(chooser == 0){   //score
      
          switch (nibble) {
          
            case 0: PTP = 0x0E; // Enable the left-most display 1110 /
                    PORTB = hex2seven_seg[(score>>12)&0x0F];
                    break;
            case 1: PTP = 0x0D; // Enable the next display 1101 /
                    PORTB = hex2seven_seg[(score>>8)&0x0F];
                    break;
            case 2: PTP = 0x0B; // Enable the next display 1011 /
                    PORTB = hex2seven_seg[(score>>4)&0x0F];
                    break;
            case 3: PTP = 0x07; // Enable right-most display 0111 /
                    PORTB = hex2seven_seg[(score)&0x0F];
                    break;
          }
      } else if(chooser == 1){    // kelimenin bilinen kýsýmlarýný gosterir                    
       
          switch (nibble) {     
                
            case 0: PTP = 0x0E; // Enable the left-most display 1110 /
                    if(cvp[0] == 1)
                      PORTB = kelimelerSEG[oyunSayisi][nibble];
                    else
                      PORTB = 0x00;
                    break;
            case 1: PTP = 0x0D; // Enable the next display 1101 /
                    if(cvp[1] == 1)
                      PORTB = kelimelerSEG[oyunSayisi][nibble];
                    else
                      PORTB = 0x00;
                    break;
            case 2: PTP = 0x0B; // Enable the next display 1011 /
                    if(cvp[2] == 1)
                      PORTB = kelimelerSEG[oyunSayisi][nibble];
                    else
                      PORTB = 0x00;
                    break;
            case 3: PTP = 0x07; // Enable right-most display 0111 /
                    if(cvp[3] == 1)
                      PORTB = kelimelerSEG[oyunSayisi][nibble];
                    else
                      PORTB = 0x00;
                    break;
          }                              
      }else if(chooser == 4){     // win               
       
        switch (nibble) {
                
            case 0: PTP = 0x0E; // Enable the left-most display 1110 /
                    PORTB = winSEG[nibble];
                    break;
            case 1: PTP = 0x0D; // Enable the next display 1101 /
                    PORTB = winSEG[nibble];
                    break;
            case 2: PTP = 0x0B; // Enable the next display 1011 /
                    PORTB = winSEG[nibble];
                    break;
            case 3: PTP = 0x07; // Enable right-most display 0111 /
                    PORTB = winSEG[nibble];
                    break;
          }                              
      }else if(chooser == 5){     // lose               
       
        switch (nibble) {
                
            case 0: PTP = 0x0E; // Enable the left-most display 1110 /
                    PORTB = loseSEG[nibble];
                    break;
            case 1: PTP = 0x0D; // Enable the next display 1101 /
                    PORTB = loseSEG[nibble];
                    break;
            case 2: PTP = 0x0B; // Enable the next display 1011 /
                    PORTB = loseSEG[nibble];
                    break;
            case 3: PTP = 0x07; // Enable right-most display 0111 /
                    PORTB = loseSEG[nibble];
                    break;
          }                              
      }else if(chooser == 6){     // finish (hepsi 0)               
       
        switch (nibble) {
                
            case 0: PTP = 0x0E; // Enable the left-most display 1110 /
                    PORTB = finishSEG[nibble];
                    break;
            case 1: PTP = 0x0D; // Enable the next display 1101 /
                    PORTB = finishSEG[nibble];
                    break;
            case 2: PTP = 0x0B; // Enable the next display 1011 /
                    PORTB = finishSEG[nibble];
                    break;
            case 3: PTP = 0x07; // Enable right-most display 0111 /
                    PORTB = finishSEG[nibble];
                    break;
          }                              
      }else if(chooser == 7){    // cevap                      
       
          switch (nibble) {     
                
            case 0: PTP = 0x0E; // Enable the left-most display 1110 /
                    PORTB = kelimelerSEG[oyunSayisi][nibble];
                    break;
            case 1: PTP = 0x0D; // Enable the next display 1101 /
                    PORTB = kelimelerSEG[oyunSayisi][nibble];
                    break;
            case 2: PTP = 0x0B; // Enable the next display 1011 /
                    PORTB = kelimelerSEG[oyunSayisi][nibble];
                    break;
            case 3: PTP = 0x07; // Enable right-most display 0111 /
                    PORTB = kelimelerSEG[oyunSayisi][nibble];
                    break;
          }                              
      }else if(chooser == 8){    // congratulations                      
       
          switch (nibble) {     
                
            case 0: PTP = 0x0E; // Enable the left-most display 1110 /
                    PORTB = congratSEG[congCount][nibble];
                    break;
            case 1: PTP = 0x0D; // Enable the next display 1101 /
                    PORTB = congratSEG[congCount][nibble];
                    break;
            case 2: PTP = 0x0B; // Enable the next display 1011 /
                    PORTB = congratSEG[congCount][nibble];
                    break;
            case 3: PTP = 0x07; // Enable right-most display 0111 /
                    PORTB = congratSEG[congCount][nibble];
                    break;
          }                              
      }else if(chooser == 9){    // goodBye                      
       
          switch (nibble) {     
                
            case 0: PTP = 0x0E; // Enable the left-most display 1110 /
                    PORTB = goodByeSEG[goodCount][nibble];
                    break;
            case 1: PTP = 0x0D; // Enable the next display 1101 /
                    PORTB = goodByeSEG[goodCount][nibble];
                    break;
            case 2: PTP = 0x0B; // Enable the next display 1011 /
                    PORTB = goodByeSEG[goodCount][nibble];
                    break;
            case 3: PTP = 0x07; // Enable right-most display 0111 /
                    PORTB = goodByeSEG[goodCount][nibble];
                    break;
          }                              
      }
      
      
    nibble = (nibble + 1) % 4;
    CRGFLG = 0x80; // Clear the RTI flag /                            
}


/*#########  FONKSIYONLAR  #########*/

void MSDelay(unsigned int itime)  //msec delay
{
  unsigned int i; unsigned int j;
  for(i=0;i<itime;i++)
    for(j=0;j<4000;j++);
}

void tahmin(unsigned int scnk){
      int i=0;
      
      
      for(i=0; i<4; ++i){
          // secili kelimede istenen harf varmi
          if(scnk == kelimelerDEC[oyunSayisi][i]){
             cvp[i] = 1;
             ++find;           
          }
      }
  
      if((find == 0) && (back != scnk)){
         MSBuzzer(1);
         hataCount++;
         back = scnk;
      }
      
      find = 0;
    
}


void MSBuzzer(unsigned int itime)  //msec BUZZER
{
  unsigned int i; unsigned int j;
  for(i=0;i<itime;i++)
    for(j=0;j<100;j++){
       PTT = PTT | 0x20;      //make PT5=1
       MSDelay(10);         //change the delay size to see what happens
       PTT = PTT & 0xDF;      //Make PT5=0
       MSDelay(10);         //change delay size.... 
    }
}



// Gets pressed key number from keypad
// Waits for any key to pressed otherwise won't return.
int GetKey() 
{
    int row,
        column;
                
    const char row_mask[ROW] = { 0xFE,0xFD,0xFB,0xF7 };
    const char col_mask[COL] = { 0x10,0x20,0x40,0x80 };        
    const unsigned int keys[ROW][COL] = { 1,2,3,10,
                                                  4,5,6,11,
                                                  7,8,9,12,
                                                  14,0,15,13 };
                
    for(row=0 ; row < ROW ; ++row){
        
        PORTA = 0xFF;// Reset PortA
        for(column=0; column < COL ;++column){
                  
            PORTA = PORTA & row_mask[row]; // Set specific row to 0
                                      
            if( (PORTA & col_mask[column]) == 0 ){
            // Check if any key is pressed
                
                  MSDelay(50);                      
                  // Wait 50ms and check again for make sure key is pressed.
                  if( (PORTA & col_mask[column]) == 0 ){
                        return keys[row][column]; 
                  }                   
                 
            }
            
        }
    
    }    
    
    return -1;
    
}