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
bool poateSari = true;

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
    // Dimensiuni pentru fundal și butoane
    const int fundalLatime = latimeFereastra;
    const int fundalInaltime = inaltimeFereastra;
    const int butonLatime = 200;
    const int butonInaltime = 80;

    // Inițializare fereastră
    initwindow(latimeFereastra, inaltimeFereastra, "Meniu Mario");

    // Afișare imagine fundal
    readimagefile("background.jpg", 0, 0, fundalLatime, fundalInaltime);

    // Coordonate pentru butoane
    int playX1 = (latimeFereastra - butonLatime) / 2;
    int playY1 = 200;
    int playX2 = playX1 + butonLatime;
    int playY2 = playY1 + butonInaltime;

    int cumX1 = playX1;
    int cumY1 = playY1 + 100;
    int cumX2 = cumX1 + butonLatime;
    int cumY2 = cumY1 + butonInaltime;

    int difX1 = playX1;
    int difY1 = cumY1 + 100;
    int difX2 = difX1 + butonLatime;
    int difY2 = difY1 + butonInaltime;

    // Afișare butoane
    readimagefile("play-button.jpg", playX1, playY1, playX2, playY2);
    readimagefile("howto-button.jpg", cumX1, cumY1, cumX2, cumY2);
    readimagefile("difficulty-button.jpg", difX1, difY1, difX2, difY2);

    // Dropdown opțiuni (imagini pentru dificultăți)
    int optiuneLatime = 200, optiuneInaltime = 50;
    int optiuneX1 = difX1, optiuneX2 = difX2;
    int optiuneY1 = difY2, optiuneY2 = optiuneY1 + optiuneInaltime;

    char dificultate[20] = "Mediu";

    while (true) {
        if (ismouseclick(WM_LBUTTONDOWN)) {
            int mouseX = mousex(), mouseY = mousey();
            clearmouseclick(WM_LBUTTONDOWN);

            // Verificăm clic pe "Play Now"
            if (mouseX >= playX1 && mouseX <= playX2 && mouseY >= playY1 && mouseY <= playY2) {
                closegraph();
                return; // Trecem la joc
            }

            // Verificăm clic pe "Cum se joacă?"
            if (mouseX >= cumX1 && mouseX <= cumX2 && mouseY >= cumY1 && mouseY <= cumY2) {
                closegraph();
                cumSeJoaca(latimeFereastra, inaltimeFereastra);
                return;
            }

            // Verificăm clic pe "Dificultate"
            if (mouseX >= difX1 && mouseX <= difX2 && mouseY >= difY1 && mouseY <= difY2) {
                // Desenăm dropdown-ul cu opțiuni
                readimagefile("easy-button.jpg", optiuneX1, optiuneY1, optiuneX2, optiuneY1 + optiuneInaltime);
                readimagefile("medium-button.jpg", optiuneX1, optiuneY1 + optiuneInaltime, optiuneX2, optiuneY1 + 2 * optiuneInaltime);
                readimagefile("hard-button.jpg", optiuneX1, optiuneY1 + 2 * optiuneInaltime, optiuneX2, optiuneY1 + 3 * optiuneInaltime);

                while (true) {
                    if (ismouseclick(WM_LBUTTONDOWN)) {
                        mouseX = mousex();
                        mouseY = mousey();
                        clearmouseclick(WM_LBUTTONDOWN);

                        // Selectăm opțiunea "Usor"
                        if (mouseX >= optiuneX1 && mouseX <= optiuneX2 && mouseY >= optiuneY1 && mouseY < optiuneY1 + optiuneInaltime) {
                            strcpy(dificultate, "Usor");
                           // strcpy(fisierHarta, "harta.txt");
                            break;
                        }
                        // Selectăm opțiunea "Mediu"
                        else if (mouseX >= optiuneX1 && mouseX <= optiuneX2 && mouseY >= optiuneY1 + optiuneInaltime && mouseY < optiuneY1 + 2 * optiuneInaltime) {
                            strcpy(dificultate, "Mediu");
                          //  strcpy(fisierHarta, "harta-mediu.txt");
                            break;
                        }
                        // Selectăm opțiunea "Dificil"
                        else if (mouseX >= optiuneX1 && mouseX <= optiuneX2 && mouseY >= optiuneY1 + 2 * optiuneInaltime && mouseY < optiuneY1 + 3 * optiuneInaltime) {
                            strcpy(dificultate, "Dificil");
                          //  strcpy(fisierHarta, "harta-dificil.txt");
                            break;
                        }
                    }
                }

                // Ștergem dropdown-ul după selecție
                setfillstyle(SOLID_FILL, LIGHTBLUE);
                bar(optiuneX1, optiuneY1, optiuneX2, optiuneY1 + 3 * optiuneInaltime);

                // Afișăm nivelul selectat pe buton
                readimagefile((strcmp(dificultate, "Usor") == 0 ? "easy-button.jpg" :
                              (strcmp(dificultate, "Mediu") == 0 ? "medium-button.jpg" : "hard-button.jpg")),
                              difX1, difY1, difX2, difY2);
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
    if (directie == "dreapta")
    {
        if (abs(ipostaza) == 10) ipostaza = 1;
        if (ipostaza < 0) ipostaza = -ipostaza;
        ipostaza++;
        if (ipostaza == 4) ipostaza = 1;

        // Verificăm blocul la dreapta și permitem scara
        if (jmario < nrColoane - 1 && (harta[imario][jmario + 1] == '.' || harta[imario][jmario + 1] == '#' || harta[imario + 1][jmario + 1] == '@'))
        {
            jmario++;
            if (harta[imario][jmario] == '*') // Dacă găsește o stea
            {
                scor++;
                harta[imario][jmario] = '.';
                afiseazaScor();
            }
        }
    }
    else if (directie == "stanga")
    {
        if (abs(ipostaza) == 10) ipostaza = -1;
        if (ipostaza > 0) ipostaza = -ipostaza;
        ipostaza--;
        if (ipostaza == -4) ipostaza = -1;

        // Verificăm blocul la stânga și permitem scara
        if (jmario > 0 && (harta[imario][jmario - 1] == '.' || harta[imario][jmario - 1] == '#' || harta[imario + 1][jmario - 1] == '@'))
        {
            jmario--;
            if (harta[imario][jmario] == '*') // Dacă găsește o stea
            {
                scor++;
                harta[imario][jmario] = '.';
                afiseazaScor();
            }
        }
    }
    else if (directie == "sus")
    {
        if (abs(ipostaza) == 10) ipostaza = -ipostaza;
        else ipostaza = 10;

        if (imario > 0 && harta[imario - 1][jmario] == '#') // Urcă pe scări
            imario--;
    }
    else if (directie == "jos")
    {
        if (abs(ipostaza) == 10) ipostaza = -ipostaza;
        else ipostaza = -10;

        if (imario < nrLinii - 1 && harta[imario + 1][jmario] == '#') // Coboară pe scări
            imario++;
    }
}

void verificaSiCadeMario() {
    while (imario < nrLinii - 1 && (harta[imario + 1][jmario] == '.' || harta[imario + 1][jmario] == '%')) {
        stergeMario();
        imario++;
        afiseazaMario();
        delay(50);
    }

    // Mario a atins solul (orice altceva decât spațiu sau nor)
    if (harta[imario + 1][jmario] != '.' && harta[imario + 1][jmario] != '%') {
        poateSari = true; // Mario poate sări din nou
    }
}

void sareMario() {
    if (!poateSari) return; // Blochez săritura dacă Mario deja sare
    poateSari = false;

    int inaltimeSaritura = 3; // Câte linii urcă Mario
    int durataSaritura = 100; // Delay între mișcări

    // Mario sare în sus
    for (int i = 0; i < inaltimeSaritura; i++) {
        if (imario > 0 && (harta[imario - 1][jmario] == '.' || harta[imario - 1][jmario] == '%')) {
            stergeMario();
            imario--;
            afiseazaMario();
            delay(durataSaritura);
        }
        else
            break; // Dacă Mario nu poate urca, ieșim din buclă
    }

    // Mario cade în jos
    verificaSiCadeMario();
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
        tasta=getch();
        if (tasta==0) tasta=getch();

        if (tasta==STG && jmario>0) directie="stanga";
        if (tasta==DRP && jmario<nrColoane-1 ) directie="dreapta";
        if (tasta==SUS && harta[imario-1][jmario]=='#') directie="sus";
        if (tasta==JOS && harta[imario+1][jmario]=='#') directie="jos";
        if (tasta == SPC) sareMario();
        stergeMario();
        urmatoareaIpostaza();
        afiseazaMario();
         verificaSiCadeMario();

        delay(200);
    } while (tasta!=ESC);  // tasta Esc (escape)

    closegraph();

    return 0;
}
