#include <fstream>
#include <winbgim.h>
#include <windows.h>
#include <string.h>

using namespace std;

#define MAX 20
#define latime 45

#define SUS 72
#define JOS 80
#define STG 75
#define DRP 77
#define ESC 27
#define SPC 32
#define TAB 9
#define BKS 8

int imario, jmario, ipostaza;
char tasta;
string directie;

int scor,nrstelute;

int nrLinii,nrColoane,i,j;
char harta[MAX][MAX];
char car;

ifstream fisier("harta.txt");

void afiseazaMeniu(int latimeFereastra, int inaltimeFereastra);

// Fereastra "Cum se joaca"
void cumSeJoaca(int latimeFereastra, int inaltimeFereastra) {
    initwindow(latimeFereastra, inaltimeFereastra, "Cum se joaca?");
    setbkcolor(LIGHTGRAY);
    cleardevice();

    // Titlul "Cum se joaca"
    settextstyle(COMPLEX_FONT, HORIZ_DIR, 3);
    setcolor(BLACK);
    outtextxy(50, 50, "Cum se joaca:");

    // Instrucțiunile jocului
    settextstyle(DEFAULT_FONT, HORIZ_DIR, 2);
    outtextxy(50, 120, "- Mario trebuie sa adune toate stelutele.");
    outtextxy(50, 160, "- Foloseste tastele SAGETI pentru a te misca.");
    outtextxy(50, 200, "- Evita inamicii, altfel vei pierde.");
    outtextxy(50, 240, "- Aduna toate stelutele pentru a castiga!");

    // Butonul BACK
    int butonLatime = 150;
    int butonInaltime = 50;
    int x1 = (latimeFereastra - butonLatime) / 2; // Centrat pe orizontală
    int y1 = 300; // Poziționat sub text
    int x2 = x1 + butonLatime;
    int y2 = y1 + butonInaltime;

    // Stil buton
    setfillstyle(SOLID_FILL, BLUE);
    bar(x1, y1, x2, y2);

    // Textul BACK
    setcolor(WHITE);
    settextstyle(BOLD_FONT, HORIZ_DIR, 2);
    outtextxy(x1 + 40, y1 + 15, "Back");

    // Așteptăm click pe buton
    while (true) {
        if (ismouseclick(WM_LBUTTONDOWN)) {
            int mouseX = mousex(), mouseY = mousey();
            clearmouseclick(WM_LBUTTONDOWN);

            // Verificam daca s-a dat click
            if (mouseX >= x1 && mouseX <= x2 && mouseY >= y1 && mouseY <= y2) {
                closegraph();
                afiseazaMeniu(latimeFereastra, inaltimeFereastra);
                return; // Revenim la meniul principal
            }
        }
    }
}

void afiseazaMeniu(int latimeFereastra, int inaltimeFereastra) {
    initwindow(latimeFereastra, inaltimeFereastra, "Meniu Mario");

    setbkcolor(RED);
    cleardevice();

    // Titlul jocului
    settextstyle(COMPLEX_FONT, HORIZ_DIR, 4);
    setcolor(BLACK);
    outtextxy(latimeFereastra / 2 - 200, 50, "Mario: Jocul ");

    // Butonul JOACA ACUM
    int butonLatime = 200;
    int butonInaltime = 80;
    int x1 = (latimeFereastra - butonLatime) / 2;
    int y1 = 200;
    int x2 = x1 + butonLatime;
    int y2 = y1 + butonInaltime;

    setfillstyle(SOLID_FILL, GREEN);
    bar(x1, y1, x2, y2);
    setcolor(WHITE);
    settextstyle(BOLD_FONT, HORIZ_DIR, 2);
    outtextxy(x1 + 40, y1 + 25, "Joaca acum!");

    // Butonul CUM SE JOACA
    int x3 = x1, y3 = 320, x4 = x3 + butonLatime, y4 = y3 + butonInaltime;

    setfillstyle(SOLID_FILL, ERANGE);
    bar(x3, y3, x4, y4);
    setcolor(WHITE);
    settextstyle(BOLD_FONT, HORIZ_DIR, 2);
    outtextxy(x3 + 25, y3 + 25, "Cum se joaca?");

    while (true) {
        if (ismouseclick(WM_LBUTTONDOWN)) {
            int mouseX = mousex(), mouseY = mousey();
            clearmouseclick(WM_LBUTTONDOWN);

            // Clic pe JOACA ACUM
            if (mouseX >= x1 && mouseX <= x2 && mouseY >= y1 && mouseY <= y2) {
                closegraph();
                return; // Merge in joc
            }

            // Clic pe CUM SE JOACA
            if (mouseX >= x3 && mouseX <= x4 && mouseY >= y3 && mouseY <= y4) {
                closegraph();
                cumSeJoaca(latimeFereastra, inaltimeFereastra);
                return;
            }
        }
    }
}



void afiseazaMario()
{ int y=imario*latime, x=jmario*latime;
    switch(ipostaza) {
        case -10: readimagefile("Mario-spate1.jpg", x,y,x+latime,y+latime); break;

        case 10: readimagefile("Mario-spate2.jpg", x,y,x+latime,y+latime); break;
        case 1:
            readimagefile("Mario.jpg", x,y,x+latime,y+latime); break;
        case 2:
            readimagefile("run.gif", x,y,x+latime,y+latime); break;
        case 3:
            readimagefile("Mario3.jpg", x,y,x+latime,y+latime); break;
        case -1:
            readimagefile("Mario4.jpg", x,y,x+latime,y+latime); break;
        case -2:
            readimagefile("Mario5.jpg", x,y,x+latime,y+latime); break;
        case -3:
            readimagefile("Mario6.jpg", x,y,x+latime,y+latime); break;
    }
}

void afiseazaPoza(char c, int i, int j)
{
    if (c=='@')
        readimagefile("iarba.jpg",latime*j,latime*i,latime*(j+1),latime*(i+1));
    if (c=='#')
        readimagefile("scara.jpg",latime*j,latime*i,latime*(j+1),latime*(i+1));
    if (c=='*')
        readimagefile("stea.jpg",latime*j,latime*i,latime*(j+1),latime*(i+1));
    if (c=='.')
        readimagefile("fundal.jpg",latime*j,latime*i,latime*(j+1),latime*(i+1));
        if (c=='%')
        readimagefile("nor.jpg",latime*j,latime*i,latime*(j+1),latime*(i+1));
}

void stergeMario()
{
    afiseazaPoza(harta[imario][jmario],imario,jmario);
}

void afiseazaScor() {
    int i;
    for (i = 1; i <= scor; i++) {
        readimagefile("Emi-stea.jpg", 30 * i, 0, 30 * i + 30, 30);
    }
    afiseazaMario();
    if (scor < nrstelute) {
        for (i = 800; i <= 1200; i += 30) Beep(i, 20);
    } else {
        closegraph();

        // Afișare fereastră "Felicitări!"
        int latimeFereastra = 400;
        int inaltimeFereastra = 300;
        initwindow(latimeFereastra, inaltimeFereastra, "Felicitari!");
        setbkcolor(LIGHTGRAY);
        cleardevice();

        // Titlul felicitări
        settextstyle(COMPLEX_FONT, HORIZ_DIR, 3);
        setcolor(BLACK);
        outtextxy(50, 50, "Felicitari! Ai castigat!");

        // Butonul "Joaca din nou"
        int butonLatime = 200;
        int butonInaltime = 50;
        int x1 = (latimeFereastra - butonLatime) / 2;
        int y1 = 200;
        int x2 = x1 + butonLatime;
        int y2 = y1 + butonInaltime;

        setfillstyle(SOLID_FILL, BLUE);
        bar(x1, y1, x2, y2);
        setcolor(WHITE);
        settextstyle(BOLD_FONT, HORIZ_DIR, 2);
        outtextxy(x1 + 20, y1 + 15, "Joaca din nou");

        // Așteptăm click pe buton
        while (true) {
            if (ismouseclick(WM_LBUTTONDOWN)) {
                int mouseX = mousex(), mouseY = mousey();
                clearmouseclick(WM_LBUTTONDOWN);

                // Verificăm dacă s-a dat click pe buton
                if (mouseX >= x1 && mouseX <= x2 && mouseY >= y1 && mouseY <= y2) {
                    closegraph();
                    afiseazaMeniu(latimeFereastra, inaltimeFereastra); // Reîncepem jocul
                    return;
                }
            }
        }
    }
}



void urmatoareaIpostaza()
{
    if (directie=="dreapta")
        {
            if (abs(ipostaza)==10) ipostaza=1;
            if (ipostaza<0) ipostaza=-ipostaza;
            ipostaza++; if (ipostaza==4) ipostaza=1;
            if (jmario<nrColoane-1 && harta[imario+1][jmario+1]!='.')
            {
                jmario++;
                if (harta[imario][jmario]=='*')
                {
                    scor++; harta[imario][jmario]='.'; afiseazaScor();
                }
            }
        }
    else
    if (directie=="stanga")
        {
            if (abs(ipostaza)==10) ipostaza=-1;
            if (ipostaza>0) ipostaza=-ipostaza;
            ipostaza--; if (ipostaza==-4) ipostaza=-1;
            if (jmario>0)
            {
                jmario--;
                if (harta[imario][jmario]=='*')
                {
                    scor++; harta[imario][jmario]='.'; afiseazaScor();
                }
            }
        }
    if (directie=="sus")
    {
        if (abs(ipostaza)==10) ipostaza=-ipostaza;
        else ipostaza=10;
        if (harta[imario-1][jmario]=='#') imario--;
    }
    if (directie=="jos")
    {
        if (abs(ipostaza)==10) ipostaza=-ipostaza;
        else ipostaza=-10;
        if (harta[imario+1][jmario]=='#') imario++;
    }
}

void verificaSiCadeMario() {
    while (imario < nrLinii - 1 && harta[imario + 1][jmario] == '.') {
        stergeMario();
        imario++;
        afiseazaMario();
        delay(100);
    }
}


void incarcaHarta()
{
    fisier>>nrLinii>>nrColoane;
    initwindow(latime*nrColoane,latime*nrLinii,"Mario pe scari");
    setbkcolor(WHITE); cleardevice();
    nrstelute=0;
    for (i=0;i<nrLinii;i++)
    {
        for (j=0;j<nrColoane;j++)
            {
                fisier>>car;
                if (car=='*') nrstelute++;
                if (car=='M') {imario=i; jmario=j; car='.';}
                harta[i][j]=car;
                afiseazaPoza(harta[i][j],i,j);
            }
    }
    fisier.close();

    scor=0;
}

int main()
{
    int latimeFereastra = latime * MAX; // Dimensiunea ferestrei în funcție de lățimea jocului
    int inaltimeFereastra = latime * MAX;

    afiseazaMeniu(latimeFereastra, inaltimeFereastra); // Afișăm meniul
    incarcaHarta();
    ipostaza=1;
    afiseazaMario();
    //PlaySound("SuperMarioBros.wav", NULL, SND_ASYNC);
     directie="dreapta";
    do
    {
        tasta=getch(); if (tasta==0) tasta=getch();
        if (tasta==STG && jmario>0 && harta[imario+1][jmario-1]!='.') directie="stanga";
        if (tasta==DRP && jmario<nrColoane-1 && harta[imario+1][jmario+1]!='.') directie="dreapta";
        if (tasta==SUS && harta[imario-1][jmario]=='#') directie="sus";
        if (tasta==JOS && harta[imario+1][jmario]=='#') directie="jos";
        stergeMario();
        urmatoareaIpostaza();
        afiseazaMario();
         verificaSiCadeMario();

        delay(200);
    } while (tasta!=ESC);  // tasta Esc (escape)

    closegraph();

    return 0;
}
