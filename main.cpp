#define SDL_MAIN_USE_CALLBACKS 1
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

struct SDLApplication {
    SDL_Window* window = nullptr;
    const bool* keystate = nullptr;
    SDL_Renderer* renderer = nullptr;
    float squareX = 0.0f;
    float squareY = 0.0f;
    float squareS = 10.0f;
};

// Called once at startup (replaces the code before your while loop)
SDL_AppResult SDL_AppInit(void** appstate, int argc, char* argv[]) {
    if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO)) {
        SDL_Log("Failed: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    SDLApplication* app = new SDLApplication();

    app->window = SDL_CreateWindow("Ibrahim - SDL3", 320, 270, SDL_WINDOW_RESIZABLE);
    if (!app->window) {
        SDL_Log("CreateWindow failed: %s", SDL_GetError());
        delete app;
        return SDL_APP_FAILURE;
    }
    app->keystate = SDL_GetKeyboardState(nullptr);

    *appstate = app;               // SDL hands this pointer back to the other callbacks
    
    // Rener the Window
    app->renderer = SDL_CreateRenderer(app->window, nullptr);
    if (!app->renderer) {
        SDL_Log("CreateRenderer failed: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }



    return SDL_APP_CONTINUE;
}


// Called once per event (replaces the inner SDL_PollEvent loop)
SDL_AppResult SDL_AppEvent(void* appstate, SDL_Event* event) {

    SDLApplication* app = static_cast<SDLApplication*>(appstate);



    if (event->type == SDL_EVENT_QUIT) {
        return SDL_APP_SUCCESS;    // exit main loop, then SDL_AppQuit runs
    }
    else if (event->type == SDL_EVENT_KEY_DOWN) {
        SDL_Log("A Key was pressed: %d", event->key.key);
        switch (event->key.scancode) {
        case SDL_SCANCODE_R: SDL_Log("SDL_SCANCODE_R Key was pressed"); break;
        case SDL_SCANCODE_L: SDL_Log("SDL_SCANCODE_L Key was pressed"); break;
        default: break;
        }
        // Chords are checked with the key state array, not a case label
        if (app->keystate[SDL_SCANCODE_A] && app->keystate[SDL_SCANCODE_S]) {
            SDL_Log("A and S Key was pressed");
        }
    }
    else if (event->type == SDL_EVENT_MOUSE_MOTION) {
        SDL_Log("x, y: %f,%f", event->motion.x, event->motion.y);
    }

    return SDL_APP_CONTINUE;
}

// Called once per frame (replaces the "Application / Game Logic" section)
SDL_AppResult SDL_AppIterate(void* appstate) {
    SDLApplication* app = static_cast<SDLApplication*>(appstate);
    (void)app;
    // Application / Game Logic
    
    // Create Triangle 
    SDL_SetRenderDrawColor(app->renderer, 0, 0, 0, 255);
    SDL_RenderClear(app->renderer);

    SDL_Vertex tri[3] = {
        { {60.0f,  40.0f}, {1.0f, 0.0f, 0.0f, 1.0f}, {0, 0} },  // top    – red
        { {60.0f, 200.0f}, {0.0f, 1.0f, 0.0f, 1.0f}, {0, 0} },  // left   – green
        { {260.0f, 200.0f}, {0.0f, 0.0f, 1.0f, 1.0f}, {0, 0} },  // right  – blue
    };
    SDL_RenderGeometry(app->renderer, nullptr, tri, 3, nullptr, 0);

    // Render Rectangle
    SDL_SetRenderDrawColor(app->renderer, 255, 255, 255, 255);   // yellow
    SDL_FRect rect = { 60.0f, 205.0f, 200.0f, 30.0f };         // x, y, w, h
    SDL_RenderFillRect(app->renderer, &rect);

    // Render Rectangle
    SDL_SetRenderDrawColor(app->renderer, 255, 255, 0, 0);   // Color
    SDL_FRect rect2 = { 60.0f, 240.0f, 200.0f, 30.0f };         // x, y, w, h
    SDL_RenderFillRect(app->renderer, &rect2);


    //Square miving 

    app->squareX += 2.0f;                       // move 2 px per frame
    app->squareY += 2.0f;                       // move 2 px per frame

    if (app->squareX > 320.0f) app->squareX = -20.0f;   // wrap when off the right edge
    if (app->squareY > 270.0f) app->squareY = -20.0f;   // wrap when off the Bottom edge


    //Square 

    SDL_SetRenderDrawColor(app->renderer, 255, 0, 0, 255);   // Color
    SDL_FRect square = { app->squareX, app->squareY, app->squareS, app->squareS };         // x, y, w, h
    SDL_RenderFillRect(app->renderer, &square);




    // Show the trame
    SDL_RenderPresent(app->renderer);


    return SDL_APP_CONTINUE;
}

// Called once at shutdown (replaces SDL_Quit + cleanup)
void SDL_AppQuit(void* appstate, SDL_AppResult result) {
    SDLApplication* app = static_cast<SDLApplication*>(appstate);
    if (app) {
        SDL_DestroyRenderer(app->renderer);
        SDL_DestroyWindow(app->window);
        delete app;
    }
    // SDL_Quit() is called automatically by SDL after this returns
}
