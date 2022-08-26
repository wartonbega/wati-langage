#include <vector>
#include <iostream>
#include <map>
#include <string>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

#include "../src/include/node.hpp"
#include "../src/include/variables.hpp"
#include "../src/include/visitor.hpp"
#include "../src/include/main.hpp"

SDL_Window *win = NULL;
SDL_Renderer *ren = NULL;

int delay = 10;
bool quit = false;

std::map<std::string, SDL_Texture *> tex;
std::map<std::string, TTF_Font *> fonts;



bool texture_exist(std::string name)
{
    if (tex.find(name) != tex.end())
    {
        return true;
    }
    return false;
}

w_variable *basic_return()
{
    w_variable *t = new w_variable();
    t->type = 2; // INT
    t->content = (void *)(new int(0));
    return t;
}

extern "C" w_variable *get_key(std::vector<w_variable *> args, variable_table variables_t, std::string reference, int thread_id)
{
    if (args.size() != 1 || args[0]->get_type() != "int")
    {
        error("!get_key : doit avoir un argument de type 'int'", reference, thread_id);
    }

    int a = *(int *)(args[0]->content);

    int r_;
    const Uint8 *state = SDL_GetKeyboardState(NULL);
    if (state[SDL_SCANCODE_RETURN])
    {
        r_ = 1;
    }
    else
        r_ = 0;

    w_variable *r = new w_variable();
    r->type = 2; // int
    r->content = (void *)(new int(r_));
    return r;
}

extern "C" w_variable *set_color(std::vector<w_variable *> args, variable_table variables_t, std::string reference, int thread_id)
{
    // r, v, b, a
    if (args.size() != 4)
    {
        error("!init_window necessite 2 arguments, `rouge`, `vert`, `bleue` et `alpha`", reference, thread_id);
    }

    w_variable *r = args[0];
    w_variable *v = args[1];
    w_variable *b = args[2];
    w_variable *a = args[3];
    if (r->get_type() != "int" || v->get_type() != "int" || b->get_type() != "int" || a->get_type() != "int")
    {
        error("!init_window necessite 2 arguments, `rouge`, `vert`, `bleue` et `alpha`, de type 'int'", reference, thread_id);
    }
    int rouge = *(int *)r->content;
    int vert = *(int *)v->content;
    int bleue = *(int *)b->content;
    int alpha = *(int *)a->content;

    SDL_SetRenderDrawColor(ren, rouge, vert, bleue, alpha);
    return basic_return();
}

extern "C" w_variable *fill_rect(std::vector<w_variable *> args, variable_table variables_t, std::string reference, int thread_id)
{
    // x, y, width, height
    if (args.size() != 4)
    {
        error("!fill_rect necessite 2 arguments, `x`, `y`, `largeur` et `longueure`", reference, thread_id);
    }

    w_variable *x_ = args[0];
    w_variable *y_ = args[1];
    w_variable *w = args[2];
    w_variable *h = args[3];
    if (w->get_type() != "int" || h->get_type() != "int" || x_->get_type() != "int" || y_->get_type() != "int")
    {
        error("!fill_rect necessite 2 arguments, `x`, `y`, `largeur` et `longueure`, de type 'int'", reference, thread_id);
    }

    int x = *(int *)x_->content;
    int y = *(int *)y_->content;
    int width = *(int *)w->content;
    int height = *(int *)h->content;

    SDL_Rect r;
    r.x = x;
    r.y = y;
    r.w = width;
    r.h = height;
    SDL_RenderFillRect(ren, &r);

    return basic_return();
}

extern "C" w_variable *draw_text(std::vector<w_variable *> args, variable_table variables_t, std::string reference, int thread_id)
{
    // x, y, text, font
    if (args.size() != 6)
    {
        error("!draw_text necessite 6 arguments, 'x', 'y', 'h', 'w', 'text', 'police'", reference, thread_id);
    }

    w_variable *arg0 = args[0]; // x -> int
    w_variable *arg1 = args[1]; // y -> int
    w_variable *arg2 = args[2]; // h -> int
    w_variable *arg3 = args[3]; // w -> int
    w_variable *arg4 = args[4]; // text -> char
    w_variable *arg5 = args[5]; // font -> char

    int x = *(int *)arg0->content;
    int y = *(int *)arg1->content;
    int h = *(int *)arg2->content;
    int w = *(int *)arg3->content;

    std::string text_content = *(std::string *)arg4->content;
    std::string font_name = *(std::string *)arg5->content;

    //this opens a font style and sets a size
    TTF_Font* Sans = fonts[font_name];
    Uint8 r;
    Uint8 v;
    Uint8 b;
    SDL_GetRenderDrawColor(ren, &r, &v, &b, nullptr);
    SDL_Color c = {r, v, b};
    SDL_Surface* surfaceMessage = TTF_RenderText_Solid(Sans, text_content.c_str(), c); 

    SDL_Texture* Message = SDL_CreateTextureFromSurface(ren, surfaceMessage);

    SDL_Rect Message_rect;
    Message_rect.x = x;
    Message_rect.y = y;
    Message_rect.w = w;
    Message_rect.h = h;

    SDL_RenderCopy(ren, Message, NULL, &Message_rect);

    SDL_FreeSurface(surfaceMessage);
    SDL_DestroyTexture(Message);
    return basic_return();
}

extern "C" w_variable *init_window(std::vector<w_variable *> args, variable_table variables_t, std::string reference, int thread_id)
{
    TTF_Init();
    // width, height
    if (args.size() != 2)
    {
        error("!init_window necessite 2 arguments, `largeur` et `longueure`", reference, thread_id);
    }

    w_variable *w = args[0];
    w_variable *h = args[1];
    if (w->get_type() != "int" || h->get_type() != "int")
    {
        error("!init_window necessite 2 arguments, `largeur` et `longueure` de type 'int'", reference, thread_id);
    }

    int width = *(int *)w->content;
    int height = *(int *)w->content;

    SDL_Init(SDL_INIT_EVERYTHING);
    SDL_CreateWindowAndRenderer(width, height, 0, &win, &ren);

    SDL_SetRenderDrawColor(ren, 0, 0, 0, 0);
    SDL_RenderClear(ren);
    SDL_RenderPresent(ren);

    return basic_return();
}

extern "C" w_variable *quit_window(std::vector<w_variable *> args, variable_table variables_t, std::string reference, int thread_id)
{
    SDL_DestroyRenderer(ren);
    SDL_DestroyWindow(win);
    SDL_Quit();
    quit = true;
    return basic_return();
}

extern "C" w_variable *set_delay(std::vector<w_variable *> args, variable_table variables_t, std::string reference, int thread_id)
{
    if (args.size() != 1)
    {
        error("!set_delay necessite 1 arguments : `longueure`", reference, thread_id);
    }

    w_variable *d = args[0];
    if (d->get_type() != "int")
    {
        error("!set_delay necessite 1 arguments : `longueure`, de type 'int'", reference, thread_id);
    }
    int de = *(int *)d->content;
    delay = de;
    return basic_return();
}

extern "C" w_variable *clear(std::vector<w_variable *> args, variable_table variables_t, std::string reference, int thread_id)
{
    SDL_RenderClear(ren);
    return basic_return();
}

extern "C" w_variable *render(std::vector<w_variable *> args, variable_table variables_t, std::string reference, int thread_id)
{
    SDL_RenderPresent(ren);

    return basic_return();
}

extern "C" w_variable *get_mouse_pos(std::vector<w_variable *> args, variable_table variables_t, std::string reference, int thread_id)
{
    int x;
    int y;
    SDL_GetMouseState(&x, &y);

    w_variable *list = visitor_new_object("list", new node("*"), variables_t, thread_id); // events
    w_variable *arg = new w_variable();
    arg->type = 2; // int
    arg->content = (void *)(new int(x));
    variable_table variables_t_bis = variable_table(variables_t);
    variables_t_bis.assign("self", list);
    variables_t_bis.assign("content", arg);

    w_function *plus = functions["!list.plus"];

    visitor_visit(plus->trunc, variables_t_bis, thread_id);

    arg = new w_variable();
    arg->type = 2; // int
    arg->content = (void *)(new int(y));
    variables_t_bis.assign("content", arg);
    visitor_visit(plus->trunc, variables_t_bis, thread_id);

    return list;
}

extern "C" w_variable *set_window_title(std::vector<w_variable *> args, variable_table variables_t, std::string reference, int thread_id)
{
    if (args.size() != 1)
    {
        error("la fonction !set_window_title necessite un argument", reference, thread_id);
    }

    w_variable *arg = args[0];
    if (arg->get_type() != "char")
    {
        error("la fonction !set_window_title necessite un argument de type 'char', pas '" + arg->get_type() + "'", reference, thread_id);
    }
    std::string title = *(std::string *)arg->content;
    SDL_SetWindowTitle(win, title.c_str());
    return basic_return();
}

extern "C" w_variable *draw_texture(std::vector<w_variable *> args, variable_table variables_t, std::string reference, int thread_id)
{
    if (args.size() != 5)
    {
        error("la fonction !draw_texture necessite 5 arguments : nom, x, y, largeur, hauteur", reference, thread_id);
    }

    w_variable *arg = args[0];
    w_variable *arg1 = args[1];
    w_variable *arg2 = args[2];
    w_variable *arg3 = args[3];
    w_variable *arg4 = args[4];

    std::string name = *(std::string *)arg->content;
    int x = *(int *)arg1->content;
    int y = *(int *)arg2->content;
    int width = *(int *)arg3->content;
    int height = *(int *)arg4->content;

    SDL_Rect texture_rect;
    texture_rect.x = x;  
    texture_rect.y = y; 
    texture_rect.w = width;
    texture_rect.h = height;

    if (!texture_exist(name))
    {
        error("la texture '" + name + "' n'existe pas", reference, thread_id);
    }

    SDL_Texture *t= tex[name];

    SDL_RenderCopy(ren, t, NULL, &texture_rect);

    return basic_return();
}

extern "C" w_variable *draw_rotated_texture(std::vector<w_variable *> args, variable_table variables_t, std::string reference, int thread_id)
{
    if (args.size() != 6)
    {
        error("la fonction !draw_texture necessite 6 arguments : nom, x, y, largeur, hauteur et angle", reference, thread_id);
    }

    w_variable *arg = args[0];
    w_variable *arg1 = args[1];
    w_variable *arg2 = args[2];
    w_variable *arg3 = args[3];
    w_variable *arg4 = args[4];
    w_variable *arg5 = args[5];

    std::string name = *(std::string *)arg->content;
    int x = *(int *)arg1->content;
    int y = *(int *)arg2->content;
    int width = *(int *)arg3->content;
    int height = *(int *)arg4->content;
    int angle = *(int *)arg5->content;

    SDL_Rect texture_rect;
    texture_rect.x = x;  
    texture_rect.y = y; 
    texture_rect.w = width;
    texture_rect.h = height;

    if (!texture_exist(name))
    {
        error("la texture '" + name + "' n'existe pas", reference, thread_id);
    }

    SDL_Texture *texture= tex[name];

    int w, h;
    SDL_QueryTexture(texture, NULL, NULL, &w, &h);

    SDL_Texture *target = SDL_CreateTexture(ren, SDL_PIXELFORMAT_RGBA4444, SDL_TEXTUREACCESS_TARGET, w, h);
    SDL_SetRenderTarget(ren, target);
    SDL_RenderCopyEx(ren, texture, NULL, NULL, angle, NULL, SDL_FLIP_NONE);
    SDL_SetRenderTarget(ren, NULL);
    
    SDL_RenderCopyEx(ren, target, NULL, NULL, angle, NULL, SDL_FLIP_NONE );
    SDL_RenderCopy(ren, target, NULL, &texture_rect);

    SDL_DestroyTexture(target);
    return basic_return();
}

extern "C" w_variable *rotate_texture(std::vector<w_variable *> args, variable_table variables_t, std::string reference, int thread_id)
{
    if (args.size() != 2)
    {
        error("la fonction !rotate_texture necessite 2 arguments : nom, angle", reference, thread_id);
    }
    w_variable *arg = args[0];
    w_variable *arg1 = args[1];

    std::string name = *(std::string *)arg->content;
    int angle = *(int *)arg1->content;


    if (!texture_exist(name))
    {
        error("la texture '" + name + "' n'existe pas", reference, thread_id);
    }

    SDL_Texture *texture = tex[name];

    int w, h;
    SDL_QueryTexture(texture, NULL, NULL, &w, &h);

    SDL_Texture *target = SDL_CreateTexture(ren, SDL_PIXELFORMAT_RGBA4444, SDL_TEXTUREACCESS_TARGET, w, h);
    SDL_SetRenderTarget(ren, target);
    SDL_RenderCopyEx(ren, texture, NULL, NULL, angle, NULL, SDL_FLIP_NONE);
    SDL_SetRenderTarget(ren, NULL);
    SDL_DestroyTexture(texture);
    tex[name] = target;

    return basic_return();
}

extern "C" w_variable *load_texture_as(std::vector<w_variable *> args, variable_table variables_t, std::string reference, int thread_id)
{
    if (args.size() != 2)
    {
        error("la fonction !load_texture_as necessite deux arguments", reference, thread_id);
    }

    w_variable *arg = args[0]; // filename
    w_variable *arg2 = args[1]; // name of the texture
    if (arg->get_type() != "char" || arg2->get_type() != "char")
    {
        error("la fonction !load_texture_as necessite deux arguments de type 'char'", reference, thread_id);
    }

    std::string filename = *(std::string *)arg->content;
    filename = base_dir + "/" + filename;
    std::string name = *(std::string *)arg2->content;
    SDL_Texture *t = IMG_LoadTexture(ren, filename.c_str());
    if (t == NULL)
    {
        // as we are opening a file, we need to know the sdl error.
        error("la texture '" + filename + "' (" + name + ") n'as pas put être chargée. Peut être que le fichier n'existe pas. Erreur SDL : " + SDL_GetError(), reference, thread_id);
    }
    tex[name] = t;

    return basic_return();
}

extern "C" w_variable *load_font_as(std::vector<w_variable *> args, variable_table variables_t, std::string reference, int thread_id)
{
    if (args.size() != 3)
    {
        error("la fonction !load_font_as necessite 3 arguments : 'chemin', 'nom' et 'taille'", reference, thread_id);
    }

    w_variable *arg = args[0]; // filename
    w_variable *arg2 = args[1]; // name of the texture
    w_variable *arg3 = args[2]; // name of the texture
    if (arg->get_type() != "char" || arg2->get_type() != "char")
    {
        error("la fonction !load_font_as necessite 3 arguments de type ('char', 'char', 'int')", reference, thread_id);
    }

    std::string filename = *(std::string *)arg->content;
    if (base_dir != "")
        filename = base_dir + "/" + filename;
    
    std::string name = *(std::string *)arg2->content;
    int size = *(int *)arg3->content;

    TTF_Font* Sans = TTF_OpenFont(filename.c_str(), size);

    if (Sans == NULL)
    {
        // as we are opening a file, we need to know the sdl error.
        error("!draw_text : police de caractère inconnue : '" + filename + "'. Erreur SDL : " + SDL_GetError(), reference, thread_id);
    }
    fonts[name] = Sans;

    return basic_return();
}

extern "C" w_variable *mainloop(std::vector<w_variable *> args, variable_table variables_t, std::string reference, int thread_id)
{
    if (thread_id != 0)
    {
        std::string err = "la fonction !mainloop doit être lancée depuis le thread principale";
        error(err, reference, thread_id);
    }
    if (args.size() != 1)
    {
        error("la fonction !main_loop necessite un argument", reference, thread_id);
    }

    w_variable *fonction_presume = args[0];
    if (fonction_presume->get_type() != "fonction")
    {
        error("la fonction !main_loop necessite un argument de type 'fonction', pas '" + fonction_presume->get_type() + "'", reference, thread_id);
    }

    std::string func_name = *(std::string *)fonction_presume->content;
    if (!function_exist(func_name, functions))
    {
        error("la fonction '" + func_name + "' n'existe pas", reference, thread_id);
    }

    while (!quit)
    {

        w_variable *list = visitor_new_object("list", new node("*"), variables_t, thread_id); // events
        SDL_Event e;
        while (SDL_PollEvent(&e) != 0) // poll for event
        {
            std::string res;
            std::string cont = "";
            Uint32 t = e.type;
            if (t == SDL_QUIT)
            {
                quit = 1;
                res = "Quit";
                return basic_return();
            }
            else if (t == SDL_KEYDOWN)
            {
                res = "KeyDown";
                cont = SDL_GetKeyName(e.key.keysym.sym);
            }
            else if (t == SDL_KEYUP)
            {
                res = "KeyUp";
                cont = SDL_GetKeyName(e.key.keysym.sym);
            }
            else if (t == SDL_MOUSEBUTTONDOWN)
                res = "MouseButtonDown";
            else if (t == SDL_MOUSEBUTTONUP)
                res = "MouseButtonUp";
            else if (t == SDL_MOUSEMOTION)
                res = "MouseMotion";
            else
                break;

            if (!res.empty())
            {

                w_variable *type_ = new w_variable(res);
                w_variable *val_ = new w_variable(cont);

                w_object *o = new w_object();
                o->name = "Event";
                o->attribute_attribution("type", type_);
                o->attribute_attribution("val", val_);

                w_variable *arg = new w_variable();
                arg->type = 3; // object
                arg->content = (void *)o;

                variable_table variables_t_bis = variable_table(variables_t);
                variables_t_bis.assign("self", list);
                variables_t_bis.assign("content", arg);

                w_function *plus = functions["!list.plus"];
                visitor_visit(plus->trunc, variables_t_bis, thread_id);
            }
            // std::cout << "HELLO" << std::endl;
        }
        node *args = new node("()");
        node *events = new node("*");
        events->pre_value = list;
        args->push_child(events);
        w_variable *ret = visitor_funcall(func_name, args, variables_t, thread_id);

        std::cout << std::flush; // we need to flush the output otherwise nothing will print untill the end of the loop

        if (ret->get_type() != "int" || *(int *)ret->content != 1)
        {
            quit = 1;
            break;
        }

        SDL_Delay(delay);
    }

    return basic_return();
}