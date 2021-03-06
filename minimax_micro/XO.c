#include <mega16.h>
#include <glcd.h>
#include <font5x7.h>
#include <io.h>
#include <delay.h>
#include  <stdio.h>
#include <string.h>

//---------------------global variables:

int game[3][3];
int br = -1 ;   //best row
int bc = -1 ;   //best column
int win[8];
int mode; //iplayer or 2player
int playerr , count , state ,r,c,k ;
unsigned char row[4]={0xFE,0xFD,0xFB,0xF7};
char txt[2];

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++


int maxx(int s, int d)
{
    if (s > d)
        return s;
    return d;
}

int minn(int s, int d)
{
    if (s < d)
        return s;
    return d;
}




bool isMovesLeft()
{
    int i = 0 , j =0 ;
    for ( i = 0; i<3; i++)
        for ( j = 0; j<3; j++)
            if (game[i][j] == 0)
                return true;
    return false;
}


int evaluate()
{

    int ro = 0 , col =0 ;
    for ( ro = 0; ro<3; ro++)
    {
        if (game[ro][0] == game[ro][1] &&
            game[ro][1] == game[ro][2])
        {
            if (game[ro][0] == 1)
                return +10;
            else if (game[ro][0] == -1)
                return -10;
        }
    }


    for ( col = 0; col<3; col++)
    {
        if (game[0][col] == game[1][col] &&
            game[1][col] == game[2][col])
        {
            if (game[0][col] == 1)
                return +10;

            else if (game[0][col] == -1)
                return -10;
        }
    }


    if (game[0][0] == game[1][1] && game[1][1] == game[2][2])
    {
        if (game[0][0] == 1)
            return +10;
        else if (game[0][0] == -1)
            return -10;
    }

    if (game[0][2] == game[1][1] && game[1][1] == game[2][0])
    {
        if (game[0][2] == 1)
            return +10;
        else if (game[0][2] == -1)
            return -10;
    }

    return 0;
}


int minimax( int depth, bool isMax)
{
    int i = 0 , j =0 ;
    int score = evaluate();

    // evaluated score
    if (score == 10)
        return score;


    if (score == -10)
        return score;


    if (isMovesLeft() == false)
        return 0;


    if (isMax)
    {
        int best = -1000;


        for ( i = 0; i<3; i++)
        {
            for (j = 0; j<3; j++)
            {
                // Check if cell is empty
                if (game[i][j] == 0)
                {
                    // Make the move
                    game[i][j] = 1;


                    best = maxx(best,minimax(depth + 1, !isMax));

                    // Undo the move
                    game[i][j] = 0;
                }
            }
        }
        return best;
    }


    else
    {
        int best = 1000;

        // Traverse all cells
        for (i = 0; i<3; i++)
        {
            for (j = 0; j<3; j++)
            {
                // Check if cell is empty
                if (game[i][j] == 0)
                {
                    // Make the move
                    game[i][j] = -1;
                    best = minn(best, minimax(depth + 1, !isMax));

                    // Undo the move
                    game[i][j] = 0;
                }
            }
        }
        return best;
    }
}


void findBestMove()
{
    int i = 0 , j =0 ;
    int moveVal;
    int bestVal = 1000;

    //glcd_outtextxyf(70,50,"190 st");


    br = -1;
    bc = -1;
    //glcd_outtextxyf(70,50,"194 st");

    for (i = 0; i<3; i++)
    {
        for (j = 0; j<3; j++)
        {
            // Check if cell is empty
            if (game[i][j] == 0)
            {
                // Make the move
                game[i][j] = -1;


                //glcd_outtextxyf(70,50,"214 st");
                moveVal = minimax(0, true);
                //glcd_outtextxyf(70,50,"216 st");
                // Undo the move
                game[i][j] = 0;


                if (moveVal < bestVal)
                {
                    br = i;
					bc = j;
					bestVal = moveVal;
				}
			}
		}
	}
    //glcd_outtextxyf(70,50,"232 st");
}

// Driver code
/*int main()
{
	int board[3][3] =
	{
		{ -1 , 1 , -1 },
		{ 0 , -1,  1},
		{ 0 , 0 , 0}
	};

	Move bestMove = findBestMove(board);

	printf("The Optimal Move is :\n");
	printf("ROW: %d COL: %d\n\n", bestMove.row,
		bestMove.col);
	return 0;
} */


//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

void keypad()
{
    while(1)
    {
        for(r=0;r<4;r++)
        {
           c=4;
           PORTC=row[r];
           DDRC=0x0F;
           if(PINC.4==0)
            c=0;
           if(PINC.5==0)
            c=1;
           if(PINC.6==0)
            c=2;
           if(PINC.7==0)
            c=3;
           if(!(c==4))  //test raha shodn klid
           {
               while(PINC.4==0);
               while(PINC.5==0);
               while(PINC.6==0);
               while(PINC.7==0);
               return;
           }
           delay_ms(5);
        }
    }
}


void init()
{
    playerr=0;    //0 ==> turn of X  and  1 ==> turn of O
    count=0;     // counter for each turn
    state=0;     // 0==>normal    -1==>X wins    1==>O wins    2==>draw
    strcpy(txt,"");
    for(r=0;r<3;r++)
        for(c=0;c<3;c++)
            game[r][c]=0;

    for(r=0;r<8;r++)
        win[r]=0;
    glcd_clear();
     glcd_outtextxyf(0,0,"    1 player ==> 1       2 players ==> 2    ");
     do{
        keypad();
        k=r*4+c;          //num of keys
    }while(k != 8 && k != 9);
    if(k==8)
        mode=1;   //1 player
    if(k==9)
        mode=2;   //2 player

    r=0;
    c=0;
    glcd_clear();
    glcd_outtextxyf(0,0,"press C to start...  press 0 to reset...");

    do{
        keypad();
        k=r*4+c;          //num of keys
    }while(k != 12);

    glcd_clear();
    for(r=3;r>0;r--)     //shomaresh maAkoos
    {
        sprintf(txt,"%d",r);
        glcd_outtext(txt);
        delay_ms(100);
        glcd_clear();
    }
}


void showBoard()
{
    glcd_clear();
    for(r=0;r<22;r+=10)
        for(c=0;c<22;c+=10)
        {
            if(game[r/10][c/10]==1)
                glcd_outtextxyf(c,r,"X");
            else if(game[r/10][c/10]==-1)
                glcd_outtextxyf(c,r,"O");
            else
                glcd_outtextxyf(c,r,"-");
        }
/*
for(c=1;c<=100;c++)
{
glcd_clear();
glcd_outtextxyf(c,1,"salam");
delay_ms(10);
}
     */
}


void winnerCheck()
{
    for(r=0;r<8;r++)
      win[r]=0;
    for(r=0;r<3;r++)
       for(c=0;c<3;c++)
       {
           win[r]+=game[r][c];
           win[r+3]+=game[c][r];
           if(r==c)
              win[6]+=game[r][c];
           if(r+c==2)
              win[7]+=game[r][c];
       }
}



void play()
{
   while(state==0)
   {
      if(mode==2)
      {
          keypad();
          if(r!=3 && c!=3)        //satr o sotun akhar safhe klid nbashad
          {
             if(game[r][c]==0)
             {
                  if (playerr==0)
                      game[r][c]=1;
                  else
                      game[r][c]=-1;
                  playerr=!playerr;   //nobat avaz mishavad
                  count++;
             }
          }
      }

      if (mode==1)
      {
          if (playerr==0)     //turn of x
          {
              keypad();
              if(r!=3 && c!=3)        //satr o sotun akhar safhe klid nbashad
              {
                  if(game[r][c]==0)
                  {
                     game[r][c]=1;

                  }
              }

          }

          if (playerr == 1 )    //turn of O
          {
               if(count==1) //hard code
               {
                  if(game[1][1]==1)   game[2][0]=-1;     //vasat
                  if(game[0][0]==1 ||game[0][2]==1 ||game[2][0]==1 ||game[2][2]==1)   game[1][1]=-1; //gooshe ha
                  if(game[0][1]==1 ||game[1][0]==1)   game[0][0]=-1;      //other
                  if(game[1][2]==1 ||game[2][1]==1)   game[2][2]=-1;      //other

               }
               //glcd_outtextxyf(70,50,"417 ln");


               if(count != 1)
               {
                  findBestMove();
                  //glcd_outtextxyf(70,50,"X win");
                  game[br][bc]= -1;
               }

          }
          playerr=!playerr;   //nobat avaz mishavad
          count++;
      }



      k=r*4+c;
      if(k==13)
         init();
      showBoard();
      winnerCheck();
      for(r=0;r<8;r++)
         if(win[r]==3)
            state=1;
         else if(win[r]==-3)
            state=-1;
      if(count==9 && state==0)
        state=2;
   }

   if(state==1)
       glcd_outtextxyf(70,50,"X win");
   else if(state==-1)
       glcd_outtextxyf(70,50,"O win");
   else if(state==2)
       glcd_outtextxyf(70,50,"Draw");
}


void main(void)
{

GLCDINIT_t glcd_init_data;

// Input/Output Ports initialization
// Port A initialization
// Function: Bit7=In Bit6=In Bit5=In Bit4=In Bit3=In Bit2=In Bit1=In Bit0=In
DDRA=(0<<DDA7) | (0<<DDA6) | (0<<DDA5) | (0<<DDA4) | (0<<DDA3) | (0<<DDA2) | (0<<DDA1) | (0<<DDA0);
// State: Bit7=T Bit6=T Bit5=T Bit4=T Bit3=T Bit2=T Bit1=T Bit0=T
PORTA=(0<<PORTA7) | (0<<PORTA6) | (0<<PORTA5) | (0<<PORTA4) | (0<<PORTA3) | (0<<PORTA2) | (0<<PORTA1) | (0<<PORTA0);

// Port B initialization
// Function: Bit7=In Bit6=In Bit5=In Bit4=In Bit3=In Bit2=In Bit1=In Bit0=In
DDRB=(0<<DDB7) | (0<<DDB6) | (0<<DDB5) | (0<<DDB4) | (0<<DDB3) | (0<<DDB2) | (0<<DDB1) | (0<<DDB0);
// State: Bit7=T Bit6=T Bit5=T Bit4=T Bit3=T Bit2=T Bit1=T Bit0=T
PORTB=(0<<PORTB7) | (0<<PORTB6) | (0<<PORTB5) | (0<<PORTB4) | (0<<PORTB3) | (0<<PORTB2) | (0<<PORTB1) | (0<<PORTB0);

// Port C initialization
// Function: Bit7=In Bit6=In Bit5=In Bit4=In Bit3=In Bit2=In Bit1=In Bit0=In
DDRC=(0<<DDC7) | (0<<DDC6) | (0<<DDC5) | (0<<DDC4) | (0<<DDC3) | (0<<DDC2) | (0<<DDC1) | (0<<DDC0);
// State: Bit7=T Bit6=T Bit5=T Bit4=T Bit3=T Bit2=T Bit1=T Bit0=T
PORTC=(0<<PORTC7) | (0<<PORTC6) | (0<<PORTC5) | (0<<PORTC4) | (0<<PORTC3) | (0<<PORTC2) | (0<<PORTC1) | (0<<PORTC0);

// Port D initialization
// Function: Bit7=In Bit6=In Bit5=In Bit4=In Bit3=In Bit2=In Bit1=In Bit0=In
DDRD=(0<<DDD7) | (0<<DDD6) | (0<<DDD5) | (0<<DDD4) | (0<<DDD3) | (0<<DDD2) | (0<<DDD1) | (0<<DDD0);
// State: Bit7=T Bit6=T Bit5=T Bit4=T Bit3=T Bit2=T Bit1=T Bit0=T
PORTD=(0<<PORTD7) | (0<<PORTD6) | (0<<PORTD5) | (0<<PORTD4) | (0<<PORTD3) | (0<<PORTD2) | (0<<PORTD1) | (0<<PORTD0);

TCCR0=(0<<WGM00) | (0<<COM01) | (0<<COM00) | (0<<WGM01) | (0<<CS02) | (0<<CS01) | (0<<CS00);
TCNT0=0x00;
OCR0=0x00;

TCCR1A=(0<<COM1A1) | (0<<COM1A0) | (0<<COM1B1) | (0<<COM1B0) | (0<<WGM11) | (0<<WGM10);
TCCR1B=(0<<ICNC1) | (0<<ICES1) | (0<<WGM13) | (0<<WGM12) | (0<<CS12) | (0<<CS11) | (0<<CS10);
TCNT1H=0x00;
TCNT1L=0x00;
ICR1H=0x00;
ICR1L=0x00;
OCR1AH=0x00;
OCR1AL=0x00;
OCR1BH=0x00;
OCR1BL=0x00;

ASSR=0<<AS2;
TCCR2=(0<<PWM2) | (0<<COM21) | (0<<COM20) | (0<<CTC2) | (0<<CS22) | (0<<CS21) | (0<<CS20);
TCNT2=0x00;
OCR2=0x00;

// Timer(s)/Counter(s) Interrupt(s) initialization
TIMSK=(0<<OCIE2) | (0<<TOIE2) | (0<<TICIE1) | (0<<OCIE1A) | (0<<OCIE1B) | (0<<TOIE1) | (0<<OCIE0) | (0<<TOIE0);

MCUCR=(0<<ISC11) | (0<<ISC10) | (0<<ISC01) | (0<<ISC00);
MCUCSR=(0<<ISC2);

// USART initialization
// USART disabled
UCSRB=(0<<RXCIE) | (0<<TXCIE) | (0<<UDRIE) | (0<<RXEN) | (0<<TXEN) | (0<<UCSZ2) | (0<<RXB8) | (0<<TXB8);

ACSR=(1<<ACD) | (0<<ACBG) | (0<<ACO) | (0<<ACI) | (0<<ACIE) | (0<<ACIC) | (0<<ACIS1) | (0<<ACIS0);
SFIOR=(0<<ACME);

// ADC initialization
// ADC disabled
ADCSRA=(0<<ADEN) | (0<<ADSC) | (0<<ADATE) | (0<<ADIF) | (0<<ADIE) | (0<<ADPS2) | (0<<ADPS1) | (0<<ADPS0);

// SPI initialization
// SPI disabled
SPCR=(0<<SPIE) | (0<<SPE) | (0<<DORD) | (0<<MSTR) | (0<<CPOL) | (0<<CPHA) | (0<<SPR1) | (0<<SPR0);

// TWI initialization
// TWI disabled
TWCR=(0<<TWEA) | (0<<TWSTA) | (0<<TWSTO) | (0<<TWEN) | (0<<TWIE);

glcd_init_data.font=font5x7;
// No function is used for reading
// image data from external memory
glcd_init_data.readxmem=NULL;
// No function is used for writing
// image data to external memory
glcd_init_data.writexmem=NULL;

glcd_init(&glcd_init_data);

    PORTC=0xFF;
   DDRC=0b00001111; //0x0F
   init();
   showBoard();
   play();


   while(1)
   {
       keypad();
       k=r*4+c;
       if(k==13)
          init();
       showBoard();
       play();
   }
}
