#include "data_types.h"
#include <SDL2/SDL.h>
#include <string>
#include <vector>
class DisplayManager{
    public:
    static DisplayManager *GetInstance();
    ~DisplayManager();
    SDL_Renderer *Init(const std::string &sWindowTitle, int iWindowHeight, int iWindowWidth); 
    void InitFrame();
    void Render();

    void AddColorPalette(WADPalette &palette);
    
    uint8_t *GetScreenBuffer();

    protected:
    DisplayManager();
    SDL_Window   *m_pWindow;
    SDL_Renderer *m_pRenderer;
    SDL_Surface  *m_pRGBBuffer;
    SDL_Surface  *m_pScreenBuffer;
    SDL_Texture  *m_pTexture;
    static DisplayManager *instance;
    int m_iWindowWidth;
    int m_iWindowHeight;
    int m_iBufferPitch;

    std::vector<WADPalette> m_ColorPalettes; 
};