//============================================================================
// Name    	: SDLTest.cpp
// Author  	: Viola
// Version 	:
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
using namespace std;

/*This source code copyrighted by Lazy Foo' Productions (2004-2013)
and may not be redistributed without written permission.*/

//The headers
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <string>
#include <SDL_ttf.h>
#include <iomanip>
#include <sstream>

#define SCREEN_WIDTH  500
#define SCREEN_HEIGHT 500
#define SCREEN_BPP  24
#define FRAMES_PER_SECOND  30

//The surfaces
SDL_Surface *screen = NULL;

SDL_Surface *Background = NULL; //penguines picture
SDL_Surface *startBackground = NULL;
SDL_Surface *startBackground2 = NULL;
SDL_Surface *message = NULL;
SDL_Surface *youwin = NULL;
SDL_Surface *youlose = NULL;
SDL_Surface *tie = NULL;
SDL_Surface *number = NULL;

Mix_Chunk *bbsound = NULL;
Mix_Chunk *movesound = NULL;
Mix_Chunk *movesound2 = NULL;
Mix_Chunk *comwin = NULL;
Mix_Chunk *tiesound = NULL;
Mix_Chunk *userwin = NULL;

Mix_Music *music = NULL;

//The event structure
SDL_Event event;
SDL_Surface *buttonSheet1 = NULL;
SDL_Surface *buttonSheet2 = NULL;
SDL_Surface *buttonSheet3 = NULL;


//The TTF font
TTF_Font *font = NULL;
TTF_Font *font2 = NULL;

//The font color
SDL_Color textColor ={ 0, 100, 255 };
SDL_Color textColor2 ={ 0, 20, 200 };

//The areas of the sprite sheet
SDL_Rect Puzzle_clip[25];
int  	Puzzle_array[25];
int  	computer_array[25]={25,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24};
int  	empty_num=20;
int  	k=0;
int  	n=-1;
int  	cnt=0;
int     t=-1;
int     countl=0;
int    time1;
int    time2;
int    timing;
int compulate11(int l[25]);


void input (int cells[]){
	int i;
	for(i=0;i<25;i++){

    	cin>>cells[i];
	}
}
typedef enum
{
	GAMEOVER_UNKNOWN = 0, GAMEOVER_USER_WIN = 1, GAMEOVER_COMPUTER_WIN = 2, GAMEOVER_TIE = 3
} GameOverWinStatus;
class GameOver
{
public:
	GameOver();

	void setWinStatus(int status);
	int getWinStatus();

	void setUserScore(int score);
	int getUserScore();

	void setComputerScore(int score);
	int getComputerScore();

	void setTryAgain(bool answer);
	bool getTryAgain();

	void handle_events();
	void show();

	void selfTest();

private:
	int winStatus;    	// 0: GAMEOVER_UNKNOWN, 1: GAMEOVER_COMPUTER_WIN, 2: GAMEOVER_USER_WIN, 3: GAMEOVER_TIE
	int userScore;
	int computerScore;
	bool tryAgain;
};

SDL_Surface *load_image_alpha( std::string filename )
{
    //The image that's loaded
    SDL_Surface* loadedImage = NULL;

    //The optimized surface that will be used
    SDL_Surface* optimizedImage = NULL;

    //Load the image
    loadedImage = IMG_Load(filename.c_str());

    //If the image loaded
    if (loadedImage != NULL)
    {
   	 //Create an optimized surface
   	 optimizedImage = SDL_DisplayFormat(loadedImage);

   	 //Free the old surface
   	 SDL_FreeSurface(loadedImage);

   	 //If the surface was optimized
   	 if (optimizedImage != NULL)
   	 {
   		 //Color key surface
   		 SDL_SetColorKey(optimizedImage, SDL_SRCCOLORKEY,
   				 SDL_MapRGB(optimizedImage->format, 0, 0xFF, 0xFF));
   	 }
    }

    //Return the optimized surface
    return optimizedImage;
}

void apply_surface( int x, int y, SDL_Surface* source, SDL_Surface* destination, SDL_Rect* clip = NULL )
{
	//Holds offsets
	SDL_Rect offset;

	//Get offsets
	offset.x = x;
	offset.y = y;

	//Blit
	SDL_BlitSurface( source, clip, destination, &offset );
}

void set_clips()
{
	int i;

	//Clip the sprites，此為500*500的圖片所調整的每個拼圖長寬設定
	for (i=0;i<25;i++) //500x500
	{
    	Puzzle_clip[i].x= 100*(i%5);
    	Puzzle_clip[i].y= 100*(i/5);
    	Puzzle_clip[i].w= 100;
    	Puzzle_clip[i].h= 100;
	}


}



bool init()
{
	if( SDL_Init( SDL_INIT_EVERYTHING ) == -1 )
	{
    	return false;
	}

	screen = SDL_SetVideoMode( SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_BPP, SDL_SWSURFACE );

	if( screen == NULL )
    	return false;

	if (TTF_Init() == -1)
   	 {
   		 return false;
   	 }
	if( Mix_OpenAudio( 22050, MIX_DEFAULT_FORMAT, 2, 4096 ) == -1 )
    	return false;

	SDL_WM_SetCaption( "abcde", NULL );//change the title
	set_clips();

	return true;
}

bool load_files()
{
	//Load the sprite sheet
	Background = load_image_alpha( "background.png" );//change the background
	startBackground=load_image_alpha("pict.png");
	startBackground2=load_image_alpha("pict3.png");
	movesound = Mix_LoadWAV( "Heart.wav" );//change the music
	movesound2 = Mix_LoadWAV( "pikachu.wav" );
	comwin = Mix_LoadWAV( "comwin.wav" );
	tiesound= Mix_LoadWAV( "tie.wav" );
	userwin = Mix_LoadWAV( "userwin.wav" );
	bbsound = Mix_LoadWAV( "bb.wav" );
	buttonSheet1 = load_image_alpha( "1.png" );
	buttonSheet2 = load_image_alpha( "2.png" );
	buttonSheet3 = load_image_alpha( "3.png" );
	number = load_image_alpha( "number.png" );
	youwin = load_image_alpha("win.png");
	youlose = load_image_alpha("LOSE.png");
	tie =load_image_alpha("peace.png");
	font = TTF_OpenFont("Funny2.ttf", 45);
	font2 = TTF_OpenFont("Funny2.ttf", 80);

	music = Mix_LoadMUS( "star.wav" );
    	if( music == NULL )
        	return false;
    	//Load the button sprite sheet

        	//If everything loaded fine
        	return true;

	//If everything loaded fine
	return true;
}

void clean_up()
{
	//Free the surface
	SDL_FreeSurface( Background );
	SDL_FreeSurface( startBackground );
	SDL_FreeSurface( startBackground2 );
	SDL_FreeSurface( number );
	Mix_FreeChunk(movesound);
	Mix_FreeChunk(movesound2);
	Mix_FreeChunk(userwin);
	Mix_FreeChunk(tiesound);
	Mix_FreeChunk(comwin);
	Mix_FreeChunk(bbsound);
	Mix_FreeMusic( music );
	SDL_FreeSurface( buttonSheet1 );
	SDL_FreeSurface( buttonSheet2 );
	SDL_FreeSurface( buttonSheet3 );
	SDL_FreeSurface( tie );
	SDL_FreeSurface( youlose );
	SDL_FreeSurface( youwin );


	//Quit SDL
	SDL_Quit();
}

int pressed(int x, int y)
{
	int i;

	for (i=0;i<25;i++)
	{
    	if(x>=Puzzle_clip[i].x && x<=(Puzzle_clip[i].x+Puzzle_clip[i].w) &&  y>=Puzzle_clip[i].y && y<=(Puzzle_clip[i].y+Puzzle_clip[i].h))
        	return i;
	}
}

void becomeasempty(int x)
{
	int temp,i;
	temp=Puzzle_array[x];
	for(i=0;i<25;i++){
    	if(temp==computer_array[i]){
        	break;
    	}
	}
	computer_array[i]=-1;
	Puzzle_array[x]=-1;
}
void becomeasempty2(int value)

{
	int temp=value,i;
	for(i=0;i<25;i++){
		if(value==computer_array[i]){
			break;
		}
	}
	computer_array[i]=-1;
	for(i=0;i<25;i++){
		if(temp==Puzzle_array[i]){
			break;
		}
	}
	if(n==2&&Puzzle_array[i]==-2){
		Puzzle_array[i]=-2;
	}
	else{
		Puzzle_array[i]=-1;
	}

}
void becomeasempty22(int x)
{

	int i;
	for(i=0;i<25;i++){
      	if(x==Puzzle_array[i]){
             	break;
            	}
   	}
	Puzzle_array[i]=-2;
	for(i=0;i<25;i++){
        	if(x==computer_array[i]){
            	break;
        	}
    	}
	//computer_array[i]=-1;


}
void becomeasempty222(int x)
{
	int temp,i;
	temp=Puzzle_array[x];
	for(i=0;i<25;i++){
    	if(temp==computer_array[i]){
        	break;
    	}
	}
	computer_array[i]=-1;
	Puzzle_array[x]=-2;
}

void handle_events()
{
	int x,y,hit_num,ori=-1;
	if( event.type == SDL_MOUSEBUTTONDOWN )
	{
    	//If the left mouse button was pressed
    	if( event.button.button == SDL_BUTTON_LEFT )
    	{

    	 time2=SDL_GetTicks();
    	 timing=time2-time1;
        	x = event.button.x;
        	y = event.button.y;
        	hit_num=pressed(x,y);
        	if(ori!=hit_num)
        	{
            	cnt++;
            	countl=1;
            	time1=SDL_GetTicks();
        	}

        	if(timing>1500 && n!=0 && cnt>1){
        		Mix_PlayChannel(-1,bbsound,0);
        		if(n==2){
        			becomeasempty22(ori);
        		    ori=hit_num;

        		}

          	}
        	else{
        	becomeasempty(hit_num);
        	ori=hit_num;
        	Mix_PlayChannel(-1,movesound,0);
        	}

    	}
	}
}

void handle_events2()
{
	int x,y;

	if( event.type == SDL_MOUSEBUTTONDOWN )
	{
    	//If the left mouse button was pressed
    	if( event.button.button == SDL_BUTTON_LEFT )
    	{

        	x = event.button.x;
        	y = event.button.y;
        	if( k==0 )
            	{
                	if(x > 330 && x < 480 && y > 251 && y<332){
                    	t=0;
                	}
                	else if(x > 330 && x < 480 && y > 332 && y<414){
                    	t=1;

                	}
                	else if(x > 330 && x < 480 && y > 414 && y<496){
                    	t=2;

                	}
            	}
         	Mix_PlayChannel(-1,movesound2,0);

    	}
	}
}


void Show_Puzzle()
{
	int i;
	if(t==3){
     	for (i=0;i<25;i++)
        	{
              	if (Puzzle_array[i]!=-1 && Puzzle_array[i]!=-2)
                   	apply_surface(Puzzle_clip[i].x,Puzzle_clip[i].y,Background,screen,&Puzzle_clip[Puzzle_array[i]]-1);
              	else if(Puzzle_array[i]==-2)
             	 SDL_FillRect( screen, &Puzzle_clip[i], SDL_MapRGB( screen->format, 255, 100, 100 ) );
            	else
                 	SDL_FillRect( screen,&Puzzle_clip[i], SDL_MapRGB( screen->format, 0x00, 0x00, 0x00 ) );//用紅色填充窗口
          	}
 	}

	if(t==0){
    	apply_surface( 0, 0, startBackground2, screen, NULL );
   	// apply_surface( 330, 332, buttonSheet2, screen, NULL );
    	n=t;
    	t=4;
    	k++;
	}

	else if(t==1){

	 apply_surface( 330, 332, buttonSheet2, screen, NULL );
	// apply_surface( 20,180, number, screen, NULL );

    	n=t;
    	t=4;
    	k++;

	}

	else if(t==2){

	 //apply_surface( 0, 0, startBackground2, screen, NULL );
    	apply_surface( 330, 414, buttonSheet3, screen, NULL );
    	//apply_surface( 20,180, number, screen, NULL );

    	n=t;

    	t=4;

    	k++;

	}
 else if(t==4){
		apply_surface(0,0,startBackground2,screen,NULL);
    	input( Puzzle_array);
    	t=3;
	}

}
int  verticalscore(int cells[]) {
	int connect = 0, add = 0, i, j;
	for (i = 0; i < 5; i++) {
    	add = 0;
    	for (j = 0; j < 5; j++) {
        	if (cells[i + j * 5] == -1)
            	add++;
    	}
    	if (add == 5){
        	connect++;
    	}
	}
	return 10*(connect);
}


int  horizontalscore(int cells[]) {
	int connect = 0,  i, j,add=0;
	for (i = 0; i < 5; i++) {
    	add = 0;
    	for (j = 0; j < 5; j++){
        	if (cells[i * 5 + j] == -1){
            	add++;
        	}
    	}
    	if (add == 5){
        	connect++;
    	}
	}
	return 10*(connect);
}


int  crossscore(int cells[]) {
	int connect = 0, i, add=0;

	for ( i = 0; i < 25; i += 6){
    	if (cells[i] == -1)
        	add ++;
	}
	if (add == 5){
    	connect++;
	}

	add=0;
	for ( i = 4; i <= 20; i += 4){
    	if (cells[i]== -1)
        	add++;
	}
	if (add == 5){
    	connect++;
	}
	return 10*(connect);
}

int  special_L(int cells[]) {
	int connect = 0, i, add=0, j;

	for (i = 0; i < 5; i++) {
    	if (cells[i * 5] == -1)
        	add++;
	}
	if (add == 5){
    	connect++;
	}
	add=0;
	for (j = 0; j < 5; j++){
    	if (cells[4 * 5 + j] == -1){
        	add++;
    	}
	}
	if (add == 5){
    	connect++;
	}

	if(connect==2){
    	return 20;
	}
	else{
    	return 0;
	}

}
int  special_T(int cells[]) {
	int connect = 0, i, add=0, j;

	for (i = 0; i < 5; i++) {
    	if (cells[2 + i * 5] == -1)
        	add++;
	}
	if (add == 5){
    	connect++;
	}
	add=0;
	for (j = 0; j < 5; j++){
    	if (cells[0 * 5 + j] == -1){
        	add++;
    	}
	}
	if (add == 5){
    	connect++;
	}
	if(connect==2){
    	return 20;
	}
	else{
    	return 0;
	}

}
int  totascorel(int cells[]) {

	return (verticalscore(cells) +
        	horizontalscore(cells) +
        	crossscore(cells))+special_T(cells)+special_L(cells);
}
int compulate11(int l[25]){
 int s;

	int seat[5][5];

	int group[12]={0};

	int i,j,counter=0,counterg=0,value=0,thelargest=0;

	for(i=0;i<5;i++){

    	for(j=0;j<5;j++){

        	seat[i][j]=l[counter];

        	counter=counter+1;

        	if(seat[i][j]==-1)

            	group[counterg]=group[counterg]+1;
    	}
    	counterg+=1;
	}

	for(j=0;j<5;j++){
    	for(i=0;i<5;i++){
        	if(seat[i][j]==-1)
            	group[counterg]+=1;
    	}

    	counterg+=1;
	}

	for(i=0;i<5;i++){

    	if(seat[i][i]==-1)

        	group[counterg]+=1;
	}
	counterg+=1;

	for(i=0;i<5;i++){

    	if(seat[i][4-i]==-1){

        	group[counterg]+=1;
    	}
	}
	for(i=0;i<12;i++){

    	if(group[i]<5){
        	thelargest=i;
        	s=i;
        	break;
    	}
	}

	for(i=s;i<12;i++) {
    	if(group[thelargest]<group[i] && group[i]<5)
        	thelargest=i;
	}

	if(thelargest==11){

    	for(i=0;i<5;i++){
        	if(seat[i][4-i]!=-1){
            	value=seat[i][4-i];
            	return value;
        	}
    	}
	}
	else if (thelargest==10){

    	for(i=0;i<5;i++){
        	if(seat[i][i]!=-1){
            	value=seat[i][i];
            	return value;
        	}
    	}
	}

	else if(thelargest<5){
    	for(i=0;i<5;i++){
        	if(seat[thelargest][i]!=-1){
            	value=seat[thelargest][i];
            	return value;
        	}
    	}
	}
	else {

    	for(i=0;i<5;i++){

        	if(seat[i][thelargest-5]!=-1){
            	value=seat[i][thelargest-5];
            	return value;
        	}
        }
	}
	return value;
}

bool Puzzle_Check()
{
	int i;

	for (i=0;i<25;i++)
	{
    	if (Puzzle_array[i]!=-1)
        	if (i!=Puzzle_array[i])
            	return false;
	}
	return true;
}
GameOver::GameOver()
{
	winStatus = GAMEOVER_UNKNOWN;
	userScore = 0;
	computerScore = 0;
	tryAgain = false;
}

void GameOver::setWinStatus(int status)
{
	winStatus = status;
}

int GameOver::getWinStatus()
{
	return winStatus;
}

void GameOver::setUserScore(int score)
{
	userScore = score;
}

int GameOver::getUserScore()
{
	return userScore;
}

void GameOver::setComputerScore(int score)
{
	computerScore = score;
}

int GameOver::getComputerScore()
{
	return computerScore;
}

void GameOver::setTryAgain(bool answer)
{
	tryAgain = answer;
}

bool GameOver::getTryAgain()
{
	return tryAgain;
}

void GameOver::handle_events()
{
	if (winStatus == GAMEOVER_UNKNOWN)
	{
    	return;
	}

	if (event.type == SDL_KEYDOWN)
	{
    	switch (event.key.keysym.sym)
    	{
    	case SDLK_y:
        	tryAgain = true;
        	break;

    	case SDLK_q:
        	tryAgain = false;
        	break;

    	default:
        	break;
    	}
	}
}

void GameOver::show()
{
	switch (winStatus)
	{
	case GAMEOVER_UNKNOWN:
    	break;

	case GAMEOVER_COMPUTER_WIN:

    	apply_surface(0 ,0 ,youlose,screen,NULL);
    	message = TTF_RenderText_Solid(font2, "You Lose !!!", textColor2);
    	apply_surface(40, 90, message, screen,NULL);
    	break;

	case GAMEOVER_USER_WIN:
    	apply_surface(0, 0, youwin, screen);
    	message = TTF_RenderText_Solid(font2, "You Win !!!", textColor2);
    	apply_surface(50, 90, message, screen);
    	break;

	case GAMEOVER_TIE:
    	apply_surface(0, 0, tie, screen);
    	message = TTF_RenderText_Solid(font2, "TIE !!!", textColor2);
    	apply_surface(50, 90, message, screen);
    	break;
	}

	stringstream ss("");
	SDL_Surface *message2;

	if (winStatus != GAMEOVER_UNKNOWN)
	{
    	ss << "Com: " << computerScore << " VS " << "User: " << userScore;
    	message2 = TTF_RenderText_Solid(font, ss.str().c_str(), textColor);
    	apply_surface(10, 270, message2, screen);
	}
}

void GameOver::selfTest()
{
	//To Test these code, please un-comment one of them.
	setComputerScore(80);
	setUserScore(60);
	//setWinStatus(GAMEOVER_USER_WIN);
	setWinStatus(GAMEOVER_COMPUTER_WIN);
	//setWinStatus(GAMEOVER_TIE);
}
int main( int argc, char* args[] )
{
	int mygrade=0, comgrade=0;
	GameOver g1;
	GameOver gameOver;
	bool quit = false;
	int  StartTick, EndTick;

	if( init() == false )
    	return 1;

	if( load_files() == false )
    	return 1;

   apply_surface(0,0,startBackground,screen,NULL);


	if( SDL_Flip( screen ) == -1 )
    	return 1;



	if( Mix_PlayingMusic() == 0 )
	{
             	if( Mix_PlayMusic( music, -1 ) == -1 )
             	return 1;
	}
	else
	{
          	if( Mix_PausedMusic() == 1 )
                   	Mix_ResumeMusic();
        	else
                     	Mix_PauseMusic();
	}


	SDL_Delay(1000);//開始停留時間
	while( quit == false )
	{

    	StartTick=SDL_GetTicks();

    	if ( SDL_PollEvent( &event ) )
    	{
        	if(k==0){
            	handle_events2();
        	}
        	else{
        	handle_events();
        	}
        	if( event.type == SDL_QUIT )
            	quit = true;
    	}
    	if (Puzzle_Check())
        	apply_surface(0,0,Background,screen,NULL);

    	else
        	Show_Puzzle();


    	if( SDL_Flip( screen ) == -1 )
        	return 1;

    	EndTick=SDL_GetTicks();
    	if( (EndTick-StartTick) < (1000 / FRAMES_PER_SECOND) ) //FRAMES_PER_SECOND=10
        	SDL_Delay( ( 1000 / FRAMES_PER_SECOND ) - (EndTick-StartTick) );
    	if(countl==1)
    	    	 becomeasempty2(compulate11(computer_array));
    	if (Puzzle_Check())

    	   apply_surface(0,0,Background,screen,NULL);
    	else
    	   Show_Puzzle();

        if( SDL_Flip( screen ) == -1 )
    	    return 1;
        countl=-1;
    	if(cnt==7){
        	break;
    	}

	}




	mygrade= totascorel(Puzzle_array);
	comgrade= totascorel(computer_array);
	g1.setComputerScore(comgrade);
	g1.setUserScore(mygrade);
	g1.setWinStatus(GAMEOVER_COMPUTER_WIN);
	g1.show();
	if(  mygrade>comgrade){

   	g1. setWinStatus(GAMEOVER_USER_WIN);
   	g1.show();
   	Mix_PlayChannel(-1,userwin,0);

	}
	else if(  mygrade<comgrade){
     	g1. setWinStatus(GAMEOVER_COMPUTER_WIN);
       	g1.show();
       	Mix_PlayChannel(-1,comwin,0);
    }
	else if(  mygrade==comgrade){
       	g1. setWinStatus(GAMEOVER_TIE);
       	g1.show();
      	Mix_PlayChannel(-1,tiesound,0);
    }
	if( SDL_Flip( screen ) == -1 )
      	return 1;

	SDL_Delay(4000);
	clean_up();

	return 0;

}
//1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25





