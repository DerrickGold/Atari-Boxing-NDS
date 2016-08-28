/*Atari boxing

By Derrick Gold aka. BassAceGold

*/



// Includes
#include <PA9.h>       // Include for PA_Lib
#include "gfx/all_gfx.h"
#include "gfx/all_gfx.c"

/*
#include "ds_compobottom.h"
#include "ds_compotop.h"
#include "controls.h"
#include "topscreen.h"
//#include "splash.h"
*/

void time();

typedef struct{
	s16 x,y;
	s16 bottom,right;
	s32 centery,centerx;
	int hits;
	int gethit,anima,move;
}fightertype;
fightertype player1;
fightertype player2;

typedef struct{
	s16 mins,seconds,mili,start;
}timer1type;
timer1type maintime;

    u8 flip=0;
    u8 ropecolision=0;
    u8 max=1;
    char text[100];
    char timetext[100];
    int starttime=0,starttime2=0,timeplayer1=0,timeplayer2=0;
    int movetimer1=0,movetimer2=0,movetimer3=0;
    int punch=0;
    int twoplayer=0;
//----------------------------------------------------------------------------------------------------------
void generalvariables(){
  
    //player 1 info
    player1.x=42;
    player1.y=22;

    //player 2 info
    player2.x=149;
    player2.y=55;


    //main timer
    maintime.mins=2;
    maintime.seconds=0;
    maintime.mili=59;
    maintime.start=1;

}
//----------------------------------------------------------------------------------------------------------
void spriteload(){
    PA_LoadSpritePal(0,0,(void*)bsprite1_Pal);
    PA_LoadSpritePal(0,1,(void*)bsprite2_Pal);
                    
    PA_CreateSprite(0,0,(void*)bsprite1_Sprite,OBJ_SIZE_64X64, 1,0,player1.x, player1.y);
    PA_CreateSprite(0,1,(void*)bsprite2_Sprite,OBJ_SIZE_64X64,1,1,player2.x, player2.y);

    PA_SetSpritePrio(0,0,2);
    PA_SetSpritePrio(0,1,2);
    
}
//----------------------------------------------------------------------------------------------------------
void BGload(){
    PA_Init16cBg(1, 3);
    PA_SetBgPalCol(1, 1, PA_RGB(0, 19, 0));
    
    PA_EasyBgLoad      (0,3,base);
    PA_EasyBgLoad      (0,2,ropes);
}    

//----------------------------------------------------------------------------------------------------------
void movement(){
    //controls
    player1.x+=(Pad.Held.Right-Pad.Held.Left)*2;//players movement
    player1.y+=(Pad.Held.Down-Pad.Held.Up)*2;

    if(twoplayer==0){
        player2.x+=(Pad.Held.A-Pad.Held.Y)*2;//players movement
        player2.y+=(Pad.Held.B-Pad.Held.X)*2;
    }


    if((Pad.Newpress.Up)||(Pad.Newpress.Down)||(Pad.Newpress.Left)||(Pad.Newpress.Right))player1.gethit=0;

    PA_SetSpriteXY(0, 0, player1.x, player1.y);//sets both players positions on screen
    PA_SetSpriteXY(0, 1, player2.x, player2.y);
        
  //sprite flipping
   if(flip==0){        
        if(player1.x>player2.x+64){
			player1.x-=30;
			player2.x+=30;
            flip=1;
            PA_SetSpriteHflip(0, 0, 1);
            PA_SetSpriteHflip(0,1,1);

        }
    }  
    if(flip==1){
        if(player1.right<player2.x){
		    player1.x+=30;
			player2.x-=30;
            flip=0;
            PA_SetSpriteHflip(0, 0,0);  
            PA_SetSpriteHflip(0, 1,0);
        }  
    }

}    
//---------------------------------------------------------------------------------------------------------
void topscreeninfo(){
    PA_16cText(1,65,1,255,20,"Atari Boxing!",1,4,100);
    PA_16cText(1,100,30,255,50,"Fight!",1,4,100);

    
    sprintf(text,  "%d   ", player1.hits/5);
    PA_16cText(1,30 , 150, 80, 170, text, 1, 4, 100);    

    sprintf(text,  "%d   ", player2.hits/5);
    PA_16cText(1,220 , 150, 240, 170, text, 1, 4, 100);
}
//----------------------------------------------------------------------------------------------------------    
void matchoutcome(){

    if (player1.hits>player2.hits){
        PA_16cText(1,100,80,255,100,"P1 WINS",1,4,100);
        PA_16cText(1,50,100,255,120,"Press Start for rematch",1,3,100);
    }

	if (player1.hits<player2.hits){
		PA_16cText(1,100,80,255,100,"P2 WINS",1,4,100);
		PA_16cText(1,50,100,255,120,"Press Start for rematch",1,3,100);
	}

	if (player1.hits==player2.hits){
		PA_16cText(1,100,80,255,100,"TIE",1,4,100);
		PA_16cText(1,50,100,255,120,"Press Start for rematch",1,3,100);    
	}
}

//----------------------------------------------------------------------------------------------------------
void restart(){
    maintime.mins=2;
    maintime.seconds=0;
    maintime.mili=59;
    player1.hits=0;
    player2.hits=0;
    player1.x=42;
    player1.y=22;
    player2.x=149;
    player2.y=55;
    PA_16cErase (1);
    maintime.start=1;    
}
//----------------------------------------------------------------------------------------------------------
void playerinformation(){

    player1.right=player1.x+64;
    player1.bottom=player1.y+64;
    player1.centery=player1.y+32;
    player1.centerx=player1.x+32;

    player2.centerx=player2.x+32;
    player2.centery=player2.y+32;
    player2.bottom=player2.y+64;
    player2.right=player2.x+64;

}

//----------------------------------------------------------------------------------------------------------
void attacks(){
//returns the frame each sprite are on
    int frame1=PA_GetSpriteAnimFrame (0,0);
    int frame2=PA_GetSpriteAnimFrame (0,1);

    s8 num=PA_RandMax(max);//random gen for the punches, decides if to use left or right hand when punching

//punching animations player 1
	
	if(!(Pad.Held.Up)||(Pad.Held.Down)){
    		if((Pad.Newpress.L)&&((frame1==0)||(frame1==3))&&((frame2==0)||(frame2==3))){
        		if (player2.centery<player1.centery){PA_StartSpriteAnimEx (0,0,0,2,12,0,1); }
        		if(player2.centery>player1.centery){PA_StartSpriteAnimEx (0,0,3,5,12,0,1);}
       	 		if(player2.centery==player1.centery){
           			 if(num==0)PA_StartSpriteAnimEx (0,0,0,2,7,0,1);
            				if(num==1)PA_StartSpriteAnimEx (0,0,3,5,7,0,1);
       	 		}
   	 	}
	}

//punching animations player 2
    if((Pad.Newpress.R)&&((frame2==0)||(frame2==3))&&((frame1==0)||(frame1==3))){
        if (player1.centery<player2.centery){PA_StartSpriteAnimEx (0,1,0,2,12,0,1);}
        if(player1.centery>player2.centery){PA_StartSpriteAnimEx (0,1,3,5,12,0,1);}
        if(player1.centery==player2.centery){
            if(num==1)PA_StartSpriteAnimEx (0,1,0,2,12,0,1);
            if(num==0)PA_StartSpriteAnimEx (0,1,3,5,12,0,1);
        }
    }
}
//----------------------------------------------------------------------------------------------------------
void punchingcollision(){

    int frame1=PA_GetSpriteAnimFrame (0,0);
    int frame2=PA_GetSpriteAnimFrame (0,1);
//collsion for punching

    if(((player1.right>=player2.centerx+8)&&(player1.x<=player2.x+25))&&(((player1.centery<player2.centery-8)&&
    (player1.centery>player2.y-1))||((player1.centery>player2.centery+6)&&(player1.centery<player2.centery+30)))){
 
	if(Pad.Newpress.L){
		if (player2.y<player1.y) player2.gethit=1;
        	if(player2.y>player1.y)player2.gethit=2;
	}
	if(Pad.Newpress.R){
		if (player1.y<player2.y) player1.gethit=1;
        	if(player1.y>player2.y)player1.gethit=2;
	}

	if((frame1==2)||(frame1==5)){
	    if(player1.x<player2.x)player2.x++;
	     if(player1.x>player2.x)player2.x--;
            player1.hits+=1;
            starttime=1;
            PA_StartSpriteAnimEx (0,1,6,6,6,0,1);
            player2.anima+=1;
        }
        if((frame2==2)||(frame2==5)){
	    if(player1.x<player2.x)player1.x--;
	    if(player1.x>player2.x)player1.x++;
            player2.hits+=1;
            starttime2=1;
            PA_StartSpriteAnimEx (0,0,6,6,6,0,1);
	    player1.anima=1;
        }
    }

}
//----------------------------------------------------------------------------------------------------------
void blockingcollision(){
        int frame1=PA_GetSpriteAnimFrame (0,0);
    int frame2=PA_GetSpriteAnimFrame (0,1);
    s8 num=PA_RandMax(max);//random gen for the punches, decides if to use left or right hand when punching

//blocking collision
    if((player1.x+10>player2.x)&&(player1.x+30<player2.x+45)){

        if(((player1.y<player2.centery+17)&&(player1.y>player2.centery-2))||((player1.bottom<player2.centery-2)&&(player1.bottom-4>player2.y-2))||
        ((player1.bottom<player2.bottom)&&(player1.bottom>player2.bottom-12))||
        ((player1.y<player2.y+13)&&(player1.y>player2.y-2))){

                if((Pad.Newpress.L)&&((frame1==0)||(frame1==3))){
                    if (player2.y<player1.y)PA_StartSpriteAnimEx (0,0,0,1,12,0,1);
                    if(player2.y>player1.y)PA_StartSpriteAnimEx (0,0,3,4,12,0,1);

                    if(player2.y==player1.y){
                        if(num==0)PA_StartSpriteAnimEx (0,0,0,2,12,0,1);
                        if(num==1)PA_StartSpriteAnimEx (0,0,3,5,12,0,1);
                    }
                }
                if((Pad.Newpress.R)&&((frame2==0)||(frame2==3))){
                    if (player1.y<player2.y)PA_StartSpriteAnimEx (0,1,0,1,12,0,1);
                    if(player1.y>player2.y)PA_StartSpriteAnimEx (0,1,3,4,12,0,1);

                if(player1.y==player2.y){
                    if(num==1)PA_StartSpriteAnimEx (0,1,0,2,12,0,1);
                    if(num==0)PA_StartSpriteAnimEx (0,1,3,5,12,0,1);
                }
            }
        }
    }    
}
//----------------------------------------------------------------------------------------------------------
void ringcollision(){
    if(flip==0)ropecolision=0;
    if(flip==2)ropecolision=1;  

    if(ropecolision==0){

//collision to prevent each player from walking through each other
//X collision
        if(player1.x-5>=player2.x){
            if(((player1.y+6>player2.y+6)&&(player1.y+5<player2.y+55))||
            ((player1.y+55>player2.y+5)&&(player1.y+55<player2.y+55))){
                player1.x-=2;
                player2.x+=2;
            }
        }
//y collision
        if((player1.x-5>=player2.x)&&((player1.y+5<=player2.y+56)&&(player1.y+7>=player2.y+7)))player1.y+=2;
        if((player1.x-5>=player2.x)&&((player1.y+5<=player2.y+54)&&(player1.y+55>=player2.y+3)))player1.y-=2;

        if(player1.x<=26)player1.x+=2;
        if(player1.y<=14)player1.y+=2;
        if(player1.x+30>=228)player1.x-=2;
        if(player1.y+55>=173)player1.y-=2;

        if(player2.x+30<=26)player2.x+=2;
        if(player2.y<=14)player2.y+=2;
        if(player2.x+60>=228)player2.x-=2;
        if(player2.y+55>=173)player2.y-=2;
    
    }

    if(ropecolision==1){
        if(player1.x+40<=player2.x+36){
            if(((player1.y+6>player2.y+6)&&(player1.y+5<player2.y+55))||
            ((player1.y+55>player2.y+5)&&(player1.y+55<player2.y+55))){
                player1.x+=2;
                player2.x-=2;
            }
        }
//y collision
        if((player1.x+40<=player2.x+36)&&((player1.y+5<=player2.y+56)&&(player1.y+7>=player2.y+7)))player1.y+=2;
        if((player1.x+40<=player2.x+36)&&((player1.y+5<=player2.y+54)&&(player1.y+55>=player2.y+3)))player1.y-=2;


        if(player1.x+30<=26)player1.x+=2;
        if(player1.y<=14)player1.y+=2;
        if(player1.x+60>=228)player1.x-=2;
        if(player1.y+55>=173)player1.y-=2;

        if(player2.x<=26)player2.x+=2;
        if(player2.y<=14)player2.y+=2;
        if(player2.x+32>=228)player2.x-=2;
        if(player2.y+55>=173)player2.y-=2;

    }
}
//----------------------------------------------------------------------------------------------------------
void animations(){

//player 2 animations for getting hit
    if(starttime>0){
	 punch=0;
        timeplayer2++;
        if(timeplayer2==8)PA_StartSpriteAnimEx (0,1,0,0,7,0,1);
        if(timeplayer2==15){
            timeplayer2=0;
            player2.anima=0;
            starttime=0;
	    player2.gethit=0;
        }
    }

    if((player2.gethit==1)&&(player2.anima>0)){
        if(player2.centery>player1.y)player2.y+=3;
        
    }

    if((player2.gethit==2)&&(player2.anima>0)){
        if(player2.centery<player1.bottom)player2.y-=3;
        
    }

//player 1 animations for getting hit
    if(starttime2>0){
        punch=0;
        timeplayer1++;
        if(timeplayer1==8)PA_StartSpriteAnimEx (0,0,0,0,7,0,1);
        if(timeplayer1==15){
            player1.anima=0;
            timeplayer1=0;
	    player1.gethit=0;
            starttime2=0;
        }
    }

    if((player1.gethit==1)&&(player1.anima>0)){
        if(player1.centery>player2.y)player1.y+=3;
    }

    if((player1.gethit==2)&&(player1.anima>0)){
        if(player1.centery<player2.bottom)player1.y-=3;
    }


}
//----------------------------------------------------------------------------------------------------------    

    
    

//----------------------------------------------------------------------------------------------------------    
void AI(){
	int frame2=PA_GetSpriteAnimFrame (0,1);
	int frame1=PA_GetSpriteAnimFrame (0,0);
	s32 dist=PA_Distance (player1.x,player1.y,player2.x,player2.y);
    
        if(player1.centerx!=player2.centerx){
            punch=0;
            movetimer1++;
            if(movetimer1>=45){
            if(player2.x>player1.x)player2.x-=2;
            if(player2.x<player1.x)player2.x+=2;
            }    
        }

        if(player1.y!=player2.centery){
            punch=0;
            movetimer2++;
            if(movetimer2>=45){
            	if(player2.y>player1.centery)player2.y-=2;
            	if(player2.centery<player1.y)player2.y+=2;
            }    
        }


	if(player2.gethit>0){
		punch=0;
		movetimer3++;
		if(movetimer3>=1){
	    		if(player2.x>player1.x)player2.x+=2;
	   		 if(player2.x<player1.x)player2.x-=2;
		}
		movetimer3=0;
	}
	

        if((dist/64==15)||(dist/64==2)||(dist/64==5)||(dist/64==10))punch=1;
        
        
        if((punch==1)&&((frame2==0)||(frame2==3))&&(player1.gethit==0)&&((frame1==0)||(frame1==3)||(frame1==1)||(frame1==4))){
            if (player1.y<player2.y){
		PA_StartSpriteAnimEx (0,1,0,2,12,0,1); 
		player1.gethit=1;
		player2.y--;
		}
            if(player1.y>player2.y){
		PA_StartSpriteAnimEx (0,1,3,5,12,0,1);
		player1.gethit=2;
		player2.y++;
		}
            
            if(player2.x>player1.x)player2.x+=1;
	    if(player2.x<player1.x)player2.x-=1;
            punch-=0;
        }
        
        
    	
		

        if((Pad.Newpress.Up)||(Pad.Newpress.Down)||(Pad.Newpress.Right)||(Pad.Newpress.Left)){
            movetimer1=0;
            movetimer2=0;
	   
        }

       if(dist/64>20) player2.gethit=0; 

}


//----------------------------------------------------------------------------------------------------------    
// Function: main()
int main(int argc, char ** argv)
{
    PA_Init();    // Initializes PA_Lib
    PA_InitVBL(); // Initializes a standard VBL
    
      //  splashscreens();
    spriteload();
    BGload();
    PA_InitRand();
    
   
    generalvariables();
    PA_InitText(1,0);
    // Infinite loop to keep the program running
    while (1)
    {	
	 
    
        if(Pad.Newpress.Select)twoplayer--;
        if(twoplayer==-1)twoplayer+=2;

        if(twoplayer==1){
            PA_OutputText(1,5,22,"Select for 2 players!  ");
            AI();
        
        }

        if(twoplayer==0){
            PA_OutputText(1,5,22,"Select for 1 player!  ");
        
        }

        movement();
        
        
        attacks();
        punchingcollision();
        animations();

        blockingcollision();
        ringcollision();

	playerinformation();
         topscreeninfo();
              time();
         PA_WaitForVBL();
         PA_16cErase (1);//refreshes top screen
    }
    
    return 0;
} // End of main()

//----------------------------------------------------------------------------------------------------------
void time(){

    if(maintime.start==1){
        maintime.mili--;
        if(maintime.mili<=-1){
                maintime.seconds--;
                maintime.mili=59;
        }

        if(maintime.seconds==-1){
                maintime.mins--;
                maintime.seconds=59;
        }
}

    if((maintime.mins==0)&&(maintime.seconds==0)){
        maintime.start=0;
        PA_16cErase (1);
        maintime.seconds--;
    }

    if(maintime.start==0){
        matchoutcome();

        PA_WaitFor(Pad.Newpress.Start);
            if(Pad.Newpress.Start){
            restart();    
            }
    }
    PA_16cText(1,100,80,255,100,"Time:",1,4,100);
    sprintf(timetext,  "%d: %02d    ", maintime.mins,maintime.seconds);
    PA_16cText(1,100,100,255,120,timetext, 1, 4, 100);
    
}

