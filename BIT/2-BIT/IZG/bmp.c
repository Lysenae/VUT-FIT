/******************************************************************************
 * Projekt - Zaklady pocitacove grafiky - IZG
 * spanel@fit.vutbr.cz
 *
 * $Id: bmp.c 416 2015-03-10 08:07:46Z spanel $
 *
 * Opravy a modifikace:
 * - 2015/3 (MS): Ukladani BMP pres SDL knihovnu
 */

#include "bmp.h"

/* knihovna SDL */
#include "SDL.h"


/****************************************************************************
 * Ulozeni obsahu obrazovky do souboru ve formatu BMP
 */ 

int saveBitmap(const char * filename, S_RGBA * buffer, int width, int height)
{
    SDL_Surface     * image;
    Uint8           * dst;
    S_RGBA          * src;
    int             x, y, bpp, result;

    IZG_ASSERT(filename && buffer && width >= 0 && height >= 0);

    /* vytvoreni SDL surface */
    image = SDL_SetVideoMode(width, height, 32, SDL_SWSURFACE);

    /* test typu pixelu */
    bpp = image->format->BytesPerPixel;
    IZG_ASSERT(bpp == 4);
    
    /* kopie bufferu */
    for( y = 0; y < height; ++y )
    {
        dst = (Uint8 *)image->pixels + y * image->pitch;
        src = buffer + y * width;
        for( x = 0; x < width; ++x, dst += bpp, ++src )
        {
            *((Uint32 *)dst) = SDL_MapRGB(image->format, src->red, src->green, src->blue);
        }
    }

    /* ulozeni do BMP obrazku */
    result = SDL_SaveBMP(image, filename);
    SDL_FreeSurface(image);
    return result;
}


/*****************************************************************************/
/*****************************************************************************/
