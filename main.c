/*
**********************
***Formules de base***
**********************
X1,Y1,Z1 = coordonnées de départ
X2,Y2,Z2 = coordonnées de départ
Xdep, Ydep, Zdep = déplacements sur les axes X,Y,Z
a,b,c = angles de rotation sur les axes X,Y,Z


***Translations***
X2 = X1 + Xdep
Y2 = Y1 + Ydep
Z2 = Z1 + Zdep

***Rotations***
autour de X :
X2 = X1
Y2 = Y1*cos(a) + Z1*sin(a)
Z2 = Y1*sin(a) - Z1*cos(a)

autour de Y :
X2= X1*cos(b) + Z1*sin(b)
Y2 = Y1
Z2 = X1*sin(b) - Z1*cos(b)

autour de Z :

X2 = X1*cos(c) + Y1*sin(c)
Y2 = X1*sin(c) - Y1*cos(c)
Z2 = Z1

***Projection Coordonnées spaciales vers le plan de l'écran***
Xe,Ye,Ze = coordonnées spaciales
X,Y = coordonnées 2D écran
P = coordonnée Z du point de fuite

Par convention : (et pour faciliter les calcul)
x=0 a gauche
y=0 en haut
+z vers le fond


Q = 1- (Ze/P)
X = Xe/Q
Y = Ye/Q


*/



#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <allegro5/allegro.h>
#include "allegro5/allegro_image.h"
#include <allegro5/allegro_native_dialog.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_audio.h> /* Pour jouer de la musique */
#include <allegro5/allegro_acodec.h> /* Pour charger de la musique encodée */
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>

#include <time.h>
#include <string.h>

#include "main.h"




int main(void)
{
    ScrollTextBLen = strlen(ScrollText) *8;
    ScrollTextPos = 0;


     for (int i=0; i<HGrille;i++)
    {
        for (int j=0;j<(LGrille);j++)
        {
            listeVector[j+(i*(LGrille))].x1 = ((LGrille*32)/2)-(LGrille*32) + (j*32);
            listeVector[j+(i*(LGrille))].y1 = ((HGrille*32)/2)-(HGrille*32) + (i*32);
            //listeVector[j+(i*24)].z1 = -12 +(i*2);
            listeVector[j+(i*(LGrille))].z1 = 0;
            listeVector[j+(i*(LGrille))].CouleurBille = 0;

        }
    }
AffIntroText();
    srand(time(NULL));
    for (int i=0; i<NbFlocon; i++)
    {
        InitFlocon(&listeFlocon[i]);
    }

    for (int i=0;i <HGrille*LGrille;i++)
    {
        listeVector[i].x3 = listeVector[i].x1;
        //listeVector[i].y3 = listeVector[i].y1;
        //listeVector[i].x3 = (rand()%2000)-1000;
        listeVector[i].y3 = (rand()%2000)-1000;
        listeVector[i].z3 = (rand()%2000)-1000;
    }
    Rassemble = true;


    a=0;
    b=0;
    c=0;
    Xdep=0;
    Ydep=0;
    Zdep=-600;

    int fin=0;
    //srand(time(NULL));
    //int x, y;
    //int pongx,pongy;

    ALLEGRO_DISPLAY*display;
    ALLEGRO_KEYBOARD_STATE key;
    ALLEGRO_BITMAP  *billeRouge;
    ALLEGRO_BITMAP  *billeBleu;
    ALLEGRO_BITMAP  *Flocon01;
    ALLEGRO_BITMAP  *Flocon02;
    ALLEGRO_BITMAP  *Flocon03;
    ALLEGRO_BITMAP  *Flocon04;
    ALLEGRO_BITMAP  *Flocon05;
    ALLEGRO_BITMAP  *Flocon06;
    ALLEGRO_BITMAP  *Flocon07;

    ALLEGRO_BITMAP  *PN[12];


    //ALLEGRO_BITMAP*bmp;
    //ALLEGRO_COLOR color;
    ALLEGRO_SAMPLE *sample;
    ALLEGRO_AUDIO_STREAM *musica = NULL;

    if (!al_init())
        erreur("al_init()");

    if (!al_install_keyboard())
        erreur("al_install_keyboard()");
    if (!al_init_primitives_addon())
        erreur("al_init_primitives_addon()");
    if (!al_init_image_addon())
        erreur("al_init_image_addon()");
   al_init_font_addon(); // initialize the font addon
   al_init_ttf_addon();// initialize the ttf (True Type Font) addon

    // il parait que c'est important de créer le DISPLAY avant de charger des bitmap
    display = al_create_display(ScreenW, ScreenH);
    if (!display)
        erreur("al_create_display()");


    if(!al_install_audio()){
        fprintf(stderr, "failed to initialize audio!\n");
        return -1;
    }


    if(!al_init_acodec_addon()) {
        fprintf(stderr, "failed to initialize audio codecs!\n");
        return -1;
    }

    if (!al_reserve_samples(1)){
        fprintf(stderr, "failed to reserve samples!\n");
        return -1;
    }

    //sample = al_load_sample( "metadragh2.mod" );
    //if (!sample) {
    //    printf( "Audio clip sample not loaded!\n" );
    //    return -1;
    //}

    //al_play_sample(sample, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_LOOP, NULL);
    //musica = al_load_audio_stream("xmasmix.mod", 4, 1024);
    musica = al_load_audio_stream("Adamo_TombeLaNeige.compat-full.it", 4, 1024);
    if (!musica)
    {
        fprintf(stderr, "Falha ao carregar audio.\n");
        //al_destroy_event_queue(fila_eventos);
        //al_destroy_display(janela);
        //al_destroy_sample(sample);
        return false;
    }
    //al_attach_audio_stream_to_mixer(musica, al_get_default_mixer());
    //al_set_audio_stream_playing(musica, true);

       ALLEGRO_FONT *font = al_load_ttf_font("BodoniFLF-Roman.ttf",30,0 );

   if (!font){
      fprintf(stderr, "Impossible de charger la fonte.\n");
      return -1;
   }



    /*char filename[255] = {0};
    for (int i=0;i<29;i++)
    {
        sprintf(filename, "Images/bille-bleu%d.png", 8 + (i*2));
        BilleBleu[i] = al_load_bitmap(filename);
        if(!BilleBleu[i]) {
            al_show_native_message_box(display, "Error", "Error", "Failed to load image!",
                                 NULL, ALLEGRO_MESSAGEBOX_ERROR);
            al_destroy_display(display);
            return 0;
        }
        sprintf(filename, "Images/bille-rouge%d.png", 8 + (i*2));
        BilleRouge[i] = al_load_bitmap(filename);
        if(!BilleRouge[i]) {
            al_show_native_message_box(display, "Error", "Error", "Failed to load image!",
                                 NULL, ALLEGRO_MESSAGEBOX_ERROR);
            al_destroy_display(display);
            return 0;
        }
    }*/
        billeRouge = al_load_bitmap("Images/bille-rouge128.png");
        if(!billeRouge) {
            al_show_native_message_box(display, "Error", "Error", "Failed to load image!",
                                 NULL, ALLEGRO_MESSAGEBOX_ERROR);
            al_destroy_display(display);
            return 0;
        }
        billeBleu = al_load_bitmap("Images/bille-bleu128.png");
        if(!billeBleu) {
            al_show_native_message_box(display, "Error", "Error", "Failed to load image!",
                                 NULL, ALLEGRO_MESSAGEBOX_ERROR);
            al_destroy_display(display);
            return 0;
        }
        Flocon01 = al_load_bitmap("Images/Flocon01.png");
        if(!Flocon01) {
            al_show_native_message_box(display, "Error", "Error", "Failed to load image!",
                                 NULL, ALLEGRO_MESSAGEBOX_ERROR);
            al_destroy_display(display);
            return 0;
        }
        Flocon02 = al_load_bitmap("Images/Flocon02.png");
        if(!Flocon02) {
            al_show_native_message_box(display, "Error", "Error", "Failed to load image!",
                                 NULL, ALLEGRO_MESSAGEBOX_ERROR);
            al_destroy_display(display);
            return 0;
        }
        Flocon03 = al_load_bitmap("Images/Flocon03.png");
        if(!Flocon03) {
            al_show_native_message_box(display, "Error", "Error", "Failed to load image!",
                                 NULL, ALLEGRO_MESSAGEBOX_ERROR);
            al_destroy_display(display);
            return 0;
        }
        Flocon04 = al_load_bitmap("Images/Flocon04.png");
        if(!Flocon04) {
            al_show_native_message_box(display, "Error", "Error", "Failed to load image!",
                                 NULL, ALLEGRO_MESSAGEBOX_ERROR);
            al_destroy_display(display);
            return 0;
        }
        Flocon05 = al_load_bitmap("Images/Flocon05.png");
        if(!Flocon05) {
            al_show_native_message_box(display, "Error", "Error", "Failed to load image!",
                                 NULL, ALLEGRO_MESSAGEBOX_ERROR);
            al_destroy_display(display);
            return 0;
        }
        Flocon06 = al_load_bitmap("Images/Flocon06.png");
        if(!Flocon06) {
            al_show_native_message_box(display, "Error", "Error", "Failed to load image!",
                                 NULL, ALLEGRO_MESSAGEBOX_ERROR);
            al_destroy_display(display);
            return 0;
        }
        Flocon07 = al_load_bitmap("Images/Flocon07.png");
        if(!Flocon07) {
            al_show_native_message_box(display, "Error", "Error", "Failed to load image!",
                                 NULL, ALLEGRO_MESSAGEBOX_ERROR);
            al_destroy_display(display);
            return 0;
        }

        PN[0] = al_load_bitmap("Images/PN01.png");
        PN[1] = al_load_bitmap("Images/PN02.png");
        PN[2] = al_load_bitmap("Images/PN03.png");
        PN[3] = al_load_bitmap("Images/PN04.png");
        PN[4] = al_load_bitmap("Images/PN05.png");
        PN[5] = al_load_bitmap("Images/PN06.png");
        PN[6] = al_load_bitmap("Images/PN07.png");
        PN[7] = al_load_bitmap("Images/PN08.png");
        PN[8] = al_load_bitmap("Images/PN09.png");
        PN[9] = al_load_bitmap("Images/PN10.png");
        PN[10] = al_load_bitmap("Images/PN11.png");
        PN[11] = al_load_bitmap("Images/PN12.png");





    int pongA = 1;
    int pongB = 1;
    int pongC = 1;

    int pongX = 1;
    int pongY = 1;
    int pongZ = 1;

    int PongPN = 0;
    int PosPN = 2000;


    //Zdep = 250;

    old_time = al_get_time();
    old_time2 = al_get_time();
    old_time3 = al_get_time();
    fps = 0;
    while(!fin){


        // pour faire joli, on joue avec les rotations
        /*if (pongA==1)
        {
            a = a+0.1;
            if (a >= 5)
            {
                pongA=0;
            }
        }
        if (pongA==0)
        {
            a = a-0.1;
            if (a <= -5)
            {
                pongA=1;
            }
        }*/
        if (pongB==1)
        {
            b = b+0.1;
            if (b >= 45)
            {
                pongB=0;
            }
        }
        if (pongB==0)
        {
            b = b-0.1;
            if (b <= -45)
            {
                pongB=1;
            }
        }
        if (pongC==1)
        {
            c = c+0.1;
            if (c >= 30)
            {
                pongC=0;
            }
        }
        if (pongC==0)
        {
            c = c-0.1;
            if (c <= -30)
            {
                pongC=1;
            }
        }
        //a= 0;
        //b=45;
        //c=0;

        /*a=a+1;
        if (a>=360)
        {
            a=0;
        }*/
        /*
        b=b+2;
        if (b>=360)
        {
            b=0;
        }*/
        /*c=c+1;
        if (c>=360)
        {
            c=0;
        }*/
        // puis avec les déplacements
        if (pongX==1)
        {
            Xdep = Xdep+0.2;
            if (Xdep >= 100)
            {
                pongX=0;
            }
        }
        if (pongX==0)
        {
            Xdep = Xdep-0.2;
            if (Xdep <= -100)
            {
                pongX=1;
            }
        }
        if (pongY==1)
        {
            Ydep = Ydep+0.3;
            if (Ydep >= 50)
            {
                pongY=0;
            }
        }
        if (pongY==0)
        {
            Ydep = Ydep-0.3;
            if (Ydep <= -50)
            {
                pongY=1;
            }
        }
        if (pongZ==1)
        {
            Zdep = Zdep+1.0;
            if (Zdep >= 300)
            {
                pongZ=0;
            }
        }
        if (pongZ==0)
        {
            Zdep = Zdep-1.0;
            if (Zdep <= -1200)
            {
                pongZ=1;
            }
        }


        // récupération de l'état du clavier
        al_get_keyboard_state(&key);

        // si touche [Echap] appuyée fin boucle et quitter
        if(al_key_down(&key,ALLEGRO_KEY_ESCAPE))
            fin=1;
        if(al_key_down(&key,ALLEGRO_KEY_A))
        {
            Effect01PosX=Effect01PosX+1;
            al_rest(1);
        }
        if(al_key_down(&key,ALLEGRO_KEY_Z))
            Effect01PosX=Effect01PosX-1;

        // si [Entrée] changer de couleur
        //if(al_key_down(&key,ALLEGRO_KEY_ENTER))
        //    al_clear_to_color(al_map_rgb(rand()%256,rand()%256,rand()%256));

        //Zdep = 70;


        // test d'un effet sur Z
        /*for (int i=0;i<LGrille;i++)
        {
                listeVector[i].z1 =0;
                listeVector[i + LGrille].z1 =0;
                listeVector[i + LGrille*2].z1 =0;
                listeVector[i + LGrille*3].z1 =0;
                listeVector[i + LGrille*4].z1 =0;
                listeVector[i + LGrille*5].z1 =0;
                listeVector[i + LGrille*6].z1 =0;
        }*/
        if (Vague)
        {
            new_time2 = al_get_time();
            if (new_time2 - old_time2 >= 0.02)
            {
                old_time2 = new_time2;
                Effect01PosX++;
                if (Effect01PosX == LGrille)
                {
                    Effect01PosX = 0;
                }


                Effect01PongZ = true;
                int PosZ = 0;
                for (int i=0;i<LGrille;i++)
                {
                    if (i >= Effect01PosX && Effect01PongZ == true)
                    {
                        PosZ++;
                        if (PosZ == 19)
                            Effect01PongZ = false;
                        if (PosZ == 1)
                            Effect01PosZ = 2*2;
                        if (PosZ == 2)
                            Effect01PosZ = 4*2;
                        if (PosZ == 3)
                            Effect01PosZ = 6*2;
                        if (PosZ == 4)
                            Effect01PosZ = 7*2;
                        if (PosZ == 5)
                            Effect01PosZ = 7*2;
                        if (PosZ == 6)
                            Effect01PosZ = 6*2;
                        if (PosZ == 7)
                            Effect01PosZ = 4*2;
                        if (PosZ == 8)
                            Effect01PosZ = 2*2;
                        if (PosZ == 9)
                            Effect01PosZ = 0;
                        if (PosZ == 10)
                            Effect01PosZ = 0;
                        if (PosZ == 11)
                            Effect01PosZ = -2*2;
                        if (PosZ == 12)
                            Effect01PosZ = -4*2;
                        if (PosZ == 13)
                            Effect01PosZ = -6*2;
                        if (PosZ == 14)
                            Effect01PosZ = -7*2;
                        if (PosZ == 15)
                            Effect01PosZ = -7*2;
                        if (PosZ == 16)
                            Effect01PosZ = -6*2;
                        if (PosZ == 17)
                            Effect01PosZ = -4*2;
                        if (PosZ == 18)
                            Effect01PosZ = -2*2;

                        listeVector[i].y3 = listeVector[i].y1 + Effect01PosZ;
                        listeVector[i + LGrille].y3 = listeVector[i + LGrille].y1 + Effect01PosZ;
                        listeVector[i + LGrille*2].y3 = listeVector[i + LGrille*2].y1 + Effect01PosZ;
                        listeVector[i + LGrille*3].y3 = listeVector[i + LGrille*3].y1 + Effect01PosZ;
                        listeVector[i + LGrille*4].y3 = listeVector[i + LGrille*4].y1 + Effect01PosZ;
                        listeVector[i + LGrille*5].y3 = listeVector[i + LGrille*5].y1 + Effect01PosZ;
                        listeVector[i + LGrille*6].y3 = listeVector[i + LGrille*6].y1 + Effect01PosZ;
                        listeVector[i + LGrille*7].y3 = listeVector[i + LGrille*7].y1 + Effect01PosZ;

                    /*listeVector[i].z3 = listeVector[i].z1 + Effect01PosZ;
                    listeVector[i + LGrille].z3 = listeVector[i + LGrille].z1 + Effect01PosZ;
                    listeVector[i + LGrille*2].z3 = listeVector[i + LGrille*2].z1 + Effect01PosZ;
                    listeVector[i + LGrille*3].z3 = listeVector[i + LGrille*3].z1 + Effect01PosZ;
                    listeVector[i + LGrille*4].z3 = listeVector[i + LGrille*4].z1 + Effect01PosZ;
                    listeVector[i + LGrille*5].z3 = listeVector[i + LGrille*5].z1 + Effect01PosZ;
                    listeVector[i + LGrille*6].z3 = listeVector[i + LGrille*6].z1 + Effect01PosZ;
                    listeVector[i + LGrille*7].z3 = listeVector[i + LGrille*7].z1 + Effect01PosZ;*/
                    }
                    else
                    {
                        listeVector[i].y3 = listeVector[i].y1;
                        listeVector[i + LGrille].y3 = listeVector[i + LGrille].y1;
                        listeVector[i + LGrille*2].y3 = listeVector[i + LGrille*2].y1;
                        listeVector[i + LGrille*3].y3 = listeVector[i + LGrille*3].y1;
                        listeVector[i + LGrille*4].y3 = listeVector[i + LGrille*4].y1;
                        listeVector[i + LGrille*5].y3 = listeVector[i + LGrille*5].y1;
                        listeVector[i + LGrille*6].y3 = listeVector[i + LGrille*6].y1;
                        listeVector[i + LGrille*7].y3 = listeVector[i + LGrille*7].y1;

                    /*listeVector[i].z3 = listeVector[i].z1;
                    listeVector[i + LGrille].z3 = listeVector[i + LGrille].z1;
                    listeVector[i + LGrille*2].z3 = listeVector[i + LGrille*2].z1;
                    listeVector[i + LGrille*3].z3 = listeVector[i + LGrille*3].z1;
                    listeVector[i + LGrille*4].z3 = listeVector[i + LGrille*4].z1;
                    listeVector[i + LGrille*5].z3 = listeVector[i + LGrille*5].z1;
                    listeVector[i + LGrille*6].z3 = listeVector[i + LGrille*6].z1;
                    listeVector[i + LGrille*7].z3 = listeVector[i + LGrille*7].z1;*/
                    }
                }
            }
        }

        if (Scroll)
        {
            new_time3 = al_get_time();
            if (new_time3 - old_time3 >= 0.05)
            {
                AffScrollText();
                ScrollTextPos++;

                old_time3 = new_time3;
            }
        }

        if (Rassemble)
        {
            if (StartRassemble!=0)
                StartRassemble--;
            else
            {
                for (int i=0; i<HGrille*LGrille;i++)
                {
                    /*if (listeVector[i].x3 < listeVector[i].x1)
                        listeVector[i].x3++;
                    if (listeVector[i].x3 > listeVector[i].x1)
                        listeVector[i].x3--;*/
                    if (listeVector[i].y3 < listeVector[i].y1)
                        listeVector[i].y3++;
                    if (listeVector[i].y3 > listeVector[i].y1)
                        listeVector[i].y3--;
                    if (listeVector[i].z3 < listeVector[i].z1)
                        listeVector[i].z3++;
                    if (listeVector[i].z3 > listeVector[i].z1)
                        listeVector[i].z3--;

                }
                CompteurRassemble--;
                if (CompteurRassemble == 0)
                {
                    Rassemble = false;
                    Scroll=true;
                    Neige=true;
                    Vague=true;
                    al_attach_audio_stream_to_mixer(musica, al_get_default_mixer());
                    al_set_audio_stream_playing(musica, true);
                }
            }
        }




        // calcul trigo.
        for (int i=0;i<HGrille*LGrille;i++)
        {

            Trigo(&listeVector[i]);
        }

        // calcul du Z moyen, pour afficher les faces les plus éloigné en 1er
        for (int i=0; i<HGrille*LGrille; i++)
        {
            triFace[i].Face = i;
            triFace[i].Zmoy = (listeVector[i].z2) / 4;
        }
        // puis on tri les faces (avec un tri a bulle, j'ai honte)
        struct TriFace temp;
        for (int j=0; j<(HGrille*LGrille)-1; j++)
        {
            for (int i=0; i<(HGrille*LGrille)-1; i++)
            {
                if (triFace[i].Zmoy > triFace[i+1].Zmoy)
                {
                    temp.Face = triFace[i].Face;
                    temp.Zmoy = triFace[i].Zmoy;
                    triFace[i].Face = triFace[i+1].Face;
                    triFace[i].Zmoy = triFace[i+1].Zmoy;
                    triFace[i+1].Face = temp.Face;
                    triFace[i+1].Zmoy = temp.Zmoy;
                }

            }
        }


            // on efface l'écran.
            //al_clear_to_color(al_map_rgb(10,23,104));
            //DrawRoundedGradientRect( 0, 0, ScreenW, ScreenH, al_map_rgb(60,73,154), al_map_rgb(0,0,54));
            DrawRoundedGradientRect( 0, 0, ScreenW, ScreenH, al_map_rgb(60,0,0), al_map_rgb(0,0,54));

            if (Neige)
            {
                for (int i=0; i<NbFlocon; i++)
                {
                    if (listeFlocon[i].y >= ScreenH + 32)
                    {
                        InitFlocon(&listeFlocon[i]);
                    }
                    else
                    {
                        int dir = rand()%2;
                        if (dir == 0)
                            listeFlocon[i].x = listeFlocon[i].x - rand()%4;
                        if (dir == 1)
                            listeFlocon[i].x = listeFlocon[i].x + rand()%4;
                            listeFlocon[i].y = listeFlocon[i].y + listeFlocon[i].Vistesse;
                    }
                }
            }
            for (int i=0; i<NbFlocon; i++)
            {
                if(listeFlocon[i].Vistesse <=2)
                {
                    if (listeFlocon[i].TypeFlocon == 0)
                        al_draw_scaled_bitmap(Flocon01,0, 0, 32, 32, listeFlocon[i].x, listeFlocon[i].y,listeFlocon[i].TailleFlocon,listeFlocon[i].TailleFlocon, 0);
                    else if (listeFlocon[i].TypeFlocon == 1)
                        al_draw_scaled_bitmap(Flocon02,0, 0, 32, 32, listeFlocon[i].x, listeFlocon[i].y,listeFlocon[i].TailleFlocon,listeFlocon[i].TailleFlocon, 0);
                    else if (listeFlocon[i].TypeFlocon == 2)
                        al_draw_scaled_bitmap(Flocon03,0, 0, 32, 32, listeFlocon[i].x, listeFlocon[i].y,listeFlocon[i].TailleFlocon,listeFlocon[i].TailleFlocon, 0);
                    else if (listeFlocon[i].TypeFlocon == 3)
                        al_draw_scaled_bitmap(Flocon04,0, 0, 32, 32, listeFlocon[i].x, listeFlocon[i].y,listeFlocon[i].TailleFlocon,listeFlocon[i].TailleFlocon, 0);
                    else if (listeFlocon[i].TypeFlocon == 4)
                        al_draw_scaled_bitmap(Flocon05,0, 0, 32, 32, listeFlocon[i].x, listeFlocon[i].y,listeFlocon[i].TailleFlocon,listeFlocon[i].TailleFlocon, 0);
                    else if (listeFlocon[i].TypeFlocon == 5)
                        al_draw_scaled_bitmap(Flocon06,0, 0, 32, 32, listeFlocon[i].x, listeFlocon[i].y,listeFlocon[i].TailleFlocon,listeFlocon[i].TailleFlocon, 0);
                    else if (listeFlocon[i].TypeFlocon == 6)
                        al_draw_scaled_bitmap(Flocon07,0, 0, 32, 32, listeFlocon[i].x, listeFlocon[i].y,listeFlocon[i].TailleFlocon,listeFlocon[i].TailleFlocon, 0);
                }
            }

            // puis on dessine les polygones.
            for (int i=0; i<HGrille;i++)
            {
                for (int j=0;j<(LGrille);j++)
                {
                    /*if (listeVector[j+(i*LGrille)].CouleurBille == 0)
                        al_draw_scaled_bitmap(billeBleu,0, 0, 128, 128, listeVector[j+(i*LGrille)].x, listeVector[j+(i*LGrille)].y,listeVector[j+(i*LGrille)].TailleBille,listeVector[j+(i*LGrille)].TailleBille, 0);
                    else
                        al_draw_scaled_bitmap(billeRouge,0, 0, 128, 128, listeVector[j+(i*LGrille)].x, listeVector[j+(i*LGrille)].y,listeVector[j+(i*LGrille)].TailleBille,listeVector[j+(i*LGrille)].TailleBille, 0);
                    */
                    if (listeVector[triFace[j+(i*LGrille)].Face].CouleurBille == 0)
                        al_draw_scaled_bitmap(billeBleu,0, 0, 128, 128, listeVector[triFace[j+(i*LGrille)].Face].x, listeVector[triFace[j+(i*LGrille)].Face].y,listeVector[triFace[j+(i*LGrille)].Face].TailleBille,listeVector[triFace[j+(i*LGrille)].Face].TailleBille, 0);
                    else
                        al_draw_scaled_bitmap(billeRouge,0, 0, 128, 128, listeVector[triFace[j+(i*LGrille)].Face].x, listeVector[triFace[j+(i*LGrille)].Face].y,listeVector[triFace[j+(i*LGrille)].Face].TailleBille,listeVector[triFace[j+(i*LGrille)].Face].TailleBille, 0);
                }
            }
            /*for (int i=0; i<HGrille;i++)
            {
                for (int j=0;j<(LGrille/2);j++)
                {
                    al_draw_scaled_bitmap(billeBleu,0, 0, 128, 128, listeVector[j+(i*LGrille)].x, listeVector[j+(i*LGrille)].y,listeVector[j+(i*LGrille)].TailleBille,listeVector[j+(i*LGrille)].TailleBille, 0);
                }
            }*/

            if (DeffilePN)
            {
                if (PongPN==0) PongPN = 48;
                    PongPN--;
                if (PosPN== -480) PosPN = 2000;
                    PosPN--;
                al_draw_scaled_bitmap(PN[PongPN/4],0, 0, 480, 160, PosPN, 640,480,160, 0);
            }

            for (int i=0; i<NbFlocon; i++)
            {
                if(listeFlocon[i].Vistesse >=3)
                {
                    if (listeFlocon[i].TypeFlocon == 0)
                        al_draw_scaled_bitmap(Flocon01,0, 0, 32, 32, listeFlocon[i].x, listeFlocon[i].y,listeFlocon[i].TailleFlocon,listeFlocon[i].TailleFlocon, 0);
                    else if (listeFlocon[i].TypeFlocon == 1)
                        al_draw_scaled_bitmap(Flocon02,0, 0, 32, 32, listeFlocon[i].x, listeFlocon[i].y,listeFlocon[i].TailleFlocon,listeFlocon[i].TailleFlocon, 0);
                    else if (listeFlocon[i].TypeFlocon == 2)
                        al_draw_scaled_bitmap(Flocon03,0, 0, 32, 32, listeFlocon[i].x, listeFlocon[i].y,listeFlocon[i].TailleFlocon,listeFlocon[i].TailleFlocon, 0);
                    else if (listeFlocon[i].TypeFlocon == 3)
                        al_draw_scaled_bitmap(Flocon04,0, 0, 32, 32, listeFlocon[i].x, listeFlocon[i].y,listeFlocon[i].TailleFlocon,listeFlocon[i].TailleFlocon, 0);
                    else if (listeFlocon[i].TypeFlocon == 4)
                        al_draw_scaled_bitmap(Flocon05,0, 0, 32, 32, listeFlocon[i].x, listeFlocon[i].y,listeFlocon[i].TailleFlocon,listeFlocon[i].TailleFlocon, 0);
                    else if (listeFlocon[i].TypeFlocon == 5)
                        al_draw_scaled_bitmap(Flocon06,0, 0, 32, 32, listeFlocon[i].x, listeFlocon[i].y,listeFlocon[i].TailleFlocon,listeFlocon[i].TailleFlocon, 0);
                    else if (listeFlocon[i].TypeFlocon == 6)
                        al_draw_scaled_bitmap(Flocon07,0, 0, 32, 32, listeFlocon[i].x, listeFlocon[i].y,listeFlocon[i].TailleFlocon,listeFlocon[i].TailleFlocon, 0);
                }
            }





            new_time = al_get_time();
            fps++;
            if (new_time - old_time >= 1.0)
            {
                old_time = new_time;
                affFps = fps;
                fps = 0;

            }
            //al_draw_textf(font,al_map_rgb(0,0,0), 16, 32,0,"FPS: %d  %f",affFps,Effect01PosX);



            // par defaut, allegro utilise le double buffering. ce qui veut dire qu'on dessine sur un écran invisible
            // et qu'on affiche l'autre, pour éviter les clignotements.
            // ici on échange les écrans.
            //al_rest(0.5);
            al_flip_display();
            }

    // on quite le programme, alors on libere les ressources.
   al_destroy_display(display);



    return 0;
} //END_OF_MAIN()



// a éxécuter une seule fois par boucle car on ne travaille que sur les angles
void InitFlocon(Flocon *f)
{
    f->x = rand()%ScreenW;
    f->y = - (rand()%400 +32);
    f->TypeFlocon = rand()%8;
    f->Vistesse = rand()%4 +1;
    if (f->Vistesse <=2)
        f->TailleFlocon = rand()%4 +8;
    else
        f->TailleFlocon = rand()%4 +12;
}


void Trigo(Vector *v)
{
    float Da = a*PI/180;
    float Db = b*PI/180;
    float Dc = c*PI/180;
    float x0,y0,z0,x1,y1,z1,x2,y2,z2,x3,y3,z3;

    if (Vague)
    {
        x0 = v->x1;
        y0 = v->y3;
        z0 = v->z1;
    }
    else if (Rassemble)
    {
        x0 = v->x3;
        y0 = v->y3;
        z0 = v->z3;
    }
    else
    {
        x0 = v->x1;
        y0 = v->y1;
        z0 = v->z1;
    }
    // rotation sur X
    x1 = x0;
    y1 = y0*cos(Da) + z0*sin(Da);
    z1 = y0*sin(Da) - z0*cos(Da);
    // rotation sur Y
    x2 = x1*cos(Db) + z1*sin(Db);
    y2 = y1;
    z2 = x1*sin(Db) - z1*cos(Db);
    // rotation sur Z
    x3 = x2*cos(Dc) + y2*sin(Dc);
    y3 = x2*sin(Dc) - y2*cos(Dc);
    z3 = z2;
    // déplacements
    v->x2 = x3 + Xdep;
    v->y2 = y3 + Ydep;
    v->z2 = z3 + Zdep;
    // projection sur le plan 2D
    v->Q = 1- (v->z2/P);
    v->x = (v->x2/v->Q) + CenterX;
    v->y = (v->y2/v->Q) + CenterY;
    v->TailleBille = (32/v->Q);
}
void Effect01(Vector *v)
{
}

void erreur(const char*txt)
{
    ALLEGRO_DISPLAY*d;
    d = al_is_system_installed() ? al_get_current_display() : NULL;
    al_show_native_message_box(d, "Erreur", txt, NULL, NULL, 0);
    exit(EXIT_FAILURE);
}

void al_draw_polygon(int vertices, int *points, unsigned char r, unsigned char g, unsigned char b)
{
    ALLEGRO_VERTEX v[vertices];
    for (int i=0; i<vertices; i++)
    {
        v[i].x = points[i*2];
        v[i].y = points[(i*2)+1];
        v[i].z = 0;
        v[i].u = 0;
        v[i].v = 0;
        v[i].color = al_map_rgb(r, g, b);
    }

    al_draw_prim(v, NULL, 0, 0, vertices, ALLEGRO_PRIM_TRIANGLE_FAN);

}

void DrawRoundedGradientRect( float x1, float y1, float x2, float y2,
                ALLEGRO_COLOR c1, ALLEGRO_COLOR c2)
{
    ALLEGRO_VERTEX v[] =
  {
        { x1, y1, 0, 0, 0, c1}, //top left
        { x2, y1, 0, 0, 0, c1}, //top right
        { x1, y2, 0, 0, 0, c2}, //bottom left
        { x2, y2, 0, 0, 0, c2}  //bottom right
  };
    al_draw_prim(v, NULL, NULL, 0, 4, ALLEGRO_PRIM_TRIANGLE_STRIP);
}

void AffIntroText(void)
{

    //ScrollTextPos = 4;

    int Pos = 0;
    //int PosB = ScrollTextPos - ((ScrollTextPos/8)*8);
    //int PosI = 0;


    int j=0;
    for (int i=0; i<LGrille;i++)
    {
        listeVector[i].CouleurBille = (LedFont[IntroText[Pos]][7] >> j) & 1;
        listeVector[i + LGrille].CouleurBille = (LedFont[IntroText[Pos]][6] >> j) & 1;
        listeVector[i + LGrille*2].CouleurBille = (LedFont[IntroText[Pos]][5] >> j) & 1;
        listeVector[i + LGrille*3].CouleurBille = (LedFont[IntroText[Pos]][4] >> j) & 1;
        listeVector[i + LGrille*4].CouleurBille = (LedFont[IntroText[Pos]][3] >> j) & 1;
        listeVector[i + LGrille*5].CouleurBille = (LedFont[IntroText[Pos]][2] >> j) & 1;
        listeVector[i + LGrille*6].CouleurBille = (LedFont[IntroText[Pos]][1] >> j) & 1;
        listeVector[i + LGrille*7].CouleurBille = (LedFont[IntroText[Pos]][0] >> j) & 1;
        j++;
        if (j==8)
        {
            j=0;
            Pos++;
        }
    }


    //ScrollTextPos ++;
    //if (ScrollTextPos >= ScrollTextBLen)
    //    ScrollTextPos = 0;

}

void AffScrollText(void)
{

    //ScrollTextPos = 4;

    int Pos = ScrollTextPos/8;
    int PosB = ScrollTextPos - ((ScrollTextPos/8)*8);
    int PosI = 0;

    if (PosB>0)
    {
    for (int j=0; j<(8-PosB);j++)
    {
        listeVector[j].CouleurBille = (LedFont[ScrollText[Pos]][7] >> j+PosB) & 1;
        listeVector[j + LGrille].CouleurBille = (LedFont[ScrollText[Pos]][6] >> j+PosB) & 1;
        listeVector[j + LGrille*2].CouleurBille = (LedFont[ScrollText[Pos]][5] >> j+PosB) & 1;
        listeVector[j + LGrille*3].CouleurBille = (LedFont[ScrollText[Pos]][4] >> j+PosB) & 1;
        listeVector[j + LGrille*4].CouleurBille = (LedFont[ScrollText[Pos]][3] >> j+PosB) & 1;
        listeVector[j + LGrille*5].CouleurBille = (LedFont[ScrollText[Pos]][2] >> j+PosB) & 1;
        listeVector[j + LGrille*6].CouleurBille = (LedFont[ScrollText[Pos]][1] >> j+PosB) & 1;
        listeVector[j + LGrille*7].CouleurBille = (LedFont[ScrollText[Pos]][0] >> j+PosB) & 1;
    }
        Pos++;
        PosI = 8-PosB;
    }
    else
    {
        //PosB = 8;
        PosI = 0;
    }

//Pos++;

    int j=0;
    for (int i=PosI; i<LGrille;i++)
    {
        listeVector[i].CouleurBille = (LedFont[ScrollText[Pos]][7] >> j) & 1;
        listeVector[i + LGrille].CouleurBille = (LedFont[ScrollText[Pos]][6] >> j) & 1;
        listeVector[i + LGrille*2].CouleurBille = (LedFont[ScrollText[Pos]][5] >> j) & 1;
        listeVector[i + LGrille*3].CouleurBille = (LedFont[ScrollText[Pos]][4] >> j) & 1;
        listeVector[i + LGrille*4].CouleurBille = (LedFont[ScrollText[Pos]][3] >> j) & 1;
        listeVector[i + LGrille*5].CouleurBille = (LedFont[ScrollText[Pos]][2] >> j) & 1;
        listeVector[i + LGrille*6].CouleurBille = (LedFont[ScrollText[Pos]][1] >> j) & 1;
        listeVector[i + LGrille*7].CouleurBille = (LedFont[ScrollText[Pos]][0] >> j) & 1;
        j++;
        if (j==8)
        {
            j=0;
            Pos++;
        }
    }

    /*for (int i=0; i<((LGrille-PosB)/8);i++)
    {
        for (int j=0; j<8;j++)
        {
        //Pos = ScrollText[i];

        listeVector[(i*8)+j+(8-PosB)].CouleurBille = (LedFont[ScrollText[Pos+i]][7] >> j) & 1;
        listeVector[(i*8)+j+(8-PosB) + LGrille].CouleurBille = (LedFont[ScrollText[Pos+i]][6] >> j) & 1;
        listeVector[(i*8)+j+(8-PosB) + LGrille*2].CouleurBille = (LedFont[ScrollText[Pos+i]][5] >> j) & 1;
        listeVector[(i*8)+j+(8-PosB) + LGrille*3].CouleurBille = (LedFont[ScrollText[Pos+i]][4] >> j) & 1;
        listeVector[(i*8)+j+(8-PosB) + LGrille*4].CouleurBille = (LedFont[ScrollText[Pos+i]][3] >> j) & 1;
        listeVector[(i*8)+j+(8-PosB) + LGrille*5].CouleurBille = (LedFont[ScrollText[Pos+i]][2] >> j) & 1;
        listeVector[(i*8)+j+(8-PosB) + LGrille*6].CouleurBille = (LedFont[ScrollText[Pos+i]][1] >> j) & 1;
        listeVector[(i*8)+j+(8-PosB) + LGrille*7].CouleurBille = (LedFont[ScrollText[Pos+i]][0] >> j) & 1;
        }

//Pos++;

    }*/
    //ScrollTextPos ++;
    if (ScrollTextPos >= ScrollTextBLen)
        ScrollTextPos = 0;

}
