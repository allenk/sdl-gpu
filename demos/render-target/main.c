#include "SDL.h"
#include "SDL_gpu.h"
#include <math.h>
#include "compat.h"
#include "common.h"


int main(int argc, char* argv[])
{
	printRenderers();
	
	GPU_Target* screen = GPU_Init(800, 600, 0);
	if(screen == NULL)
		return -1;
    
    //GPU_SetVirtualResolution(1200, 800);
	
	printCurrentRenderer();
	
	GPU_Image* image2 = GPU_LoadImage("data/test2.bmp");
	if(image2 == NULL)
		return -1;
	
	GPU_Image* image3 = GPU_LoadImage("data/test.bmp");
	if(image3 == NULL)
		return -1;
	
	
	GPU_Image* mode1image = GPU_CreateImage(300, 300, 4);
	if(mode1image == NULL)
		return -1;
		
	GPU_Image* mode2image = GPU_CreateImage(400, 400, 4);
	if(mode2image == NULL)
		return -1;
    
	GPU_LoadTarget(mode1image);
	GPU_LoadTarget(mode2image);
	
	SDL_Color circleColor = {255, 0, 0, 128};
	SDL_Color circleColor2 = {0, 0, 255, 128};
	
	
	Uint32 startTime = SDL_GetTicks();
	long frameCount = 0;
	
	Uint8* keystates = SDL_GetKeyState(NULL);
    
    int mode = 0;
	int x = 0;
	int y = 0;
	
	Uint8 done = 0;
	SDL_Event event;
	while(!done)
	{
		while(SDL_PollEvent(&event))
		{
			if(event.type == SDL_QUIT)
				done = 1;
			else if(event.type == SDL_KEYDOWN)
			{
				if(event.key.keysym.sym == SDLK_ESCAPE)
					done = 1;
				else if(event.key.keysym.sym == SDLK_SPACE)
                {
					mode++;
					if(mode > 1)
                        mode = 0;
                }
			}
		}
		
		if(keystates[KEY_UP])
			y -= 1;
		else if(keystates[KEY_DOWN])
			y += 1;
		if(keystates[KEY_LEFT])
			x -= 1;
		else if(keystates[KEY_RIGHT])
			x += 1;
		
		GPU_Clear(screen);
		
		if(mode == 0)
        {
            GPU_Target* mode1target = mode1image->target;
            
            GPU_ClearRGBA(mode1target, 0, 255, 0, 255);
            GPU_Blit(image3, NULL, mode1target, image3->w/2, image3->h/2);
            
            SDL_Color red = {255, 0, 0, 255};
            
            //GPU_BlitScale(image2, NULL, mode1target, mode1target->w/2, mode1target->h/2, 0.7f, 0.7f);
            //GPU_BlitScale(image2, NULL, mode1target, x, y, 0.7f, 0.7f);
            
            GPU_BlitTransform(image2, NULL, mode1target, x, y, 360*sin(SDL_GetTicks()/1000.0f), 0.7f*sin(SDL_GetTicks()/2000.0f), 0.7f*sin(SDL_GetTicks()/2000.0f));
            
            GPU_Line(mode1target, 0, 0, mode1target->w, mode1target->h, red);
            GPU_CircleFilled(mode1target, 70, 70, 20, circleColor);
        
            GPU_Blit(mode1image, NULL, screen, mode1image->w/2 + 50, mode1image->h/2 + 50);
            GPU_BlitScale(mode1image, NULL, screen, mode1image->w*2 + 50, mode1image->h/2 + 50, 0.7f, 0.7f);
            
            GPU_CircleFilled(screen, 50 + 70, 50 + 70, 20, circleColor2);
            
            GPU_Blit(image2, NULL, screen, screen->w - image3->w - image2->w/2, screen->h - image2->h/2);
            GPU_Blit(image3, NULL, screen, screen->w - image3->w/2, screen->h - image3->h/2);
        }
        else if(mode == 1)
        {
            GPU_Target* mode2target = mode2image->target;
            
            GPU_ClearRGBA(mode2target, 255, 255, 255, 255);
            
            SDL_Color red = {255, 0, 0, 255};
            
            GPU_Line(mode2target, 0, 0, mode2target->w, mode2target->h, red);
            GPU_Line(mode2target, 0, mode2target->h, mode2target->w, 0, red);
            GPU_TriFilled(mode2target, mode2target->w/2, mode2target->h/2 + 10, mode2target->w/2 - 10, mode2target->h/2 + 20, mode2target->w/2 + 10, mode2target->h/2 + 20, red);
            
            GPU_Blit(image2, NULL, mode2target, x, y);
            
            
            SDL_Color blue = {0, 0, 255, 255};
            
            GPU_Line(screen, 0, 0, screen->w, screen->h, blue);
            GPU_Line(screen, 0, screen->h, screen->w, 0, blue);
            
            GPU_Blit(mode2image, NULL, screen, mode2image->w/2, mode2image->h/2);
        }
		
		GPU_Flip(screen);
		
		frameCount++;
		if(frameCount%500 == 0)
		{
			printf("Average FPS: %.2f\n", 1000.0f*frameCount/(SDL_GetTicks() - startTime));
			printf("x,y: (%d, %d)\n", x, y);
		}
	}
	
	printf("Average FPS: %.2f\n", 1000.0f*frameCount/(SDL_GetTicks() - startTime));
	
	GPU_FreeImage(mode2image);
	GPU_FreeImage(mode1image);
	GPU_Quit();
	
	return 0;
}


