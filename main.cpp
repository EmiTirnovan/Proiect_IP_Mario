#include <fstream>
#include <winbgim.h>
#include <windows.h>
#include <string.h>
#include <mmsystem.h>


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
int stop;
char tasta;
int latimeFereastra = latime * MAX; // Dimensiunea ferestrei in functie de latimea jocului
    int inaltimeFereastra = latime * MAX;
string directie;

bool gameOver = false;

int scor,nrstelute;

int nrLinii,nrColoane,i,j;
char harta[MAX][MAX];
char car, car2, car3;
bool poateSari = true;

int inimicMobilX, inimicMobilY; // Pozitia curenta a inamicului mobil
int limitaStanga, limitaDreapta; // Limitele de miscare

bool directieInamicMobil = true; // True pentru dreapta, False pentru stanga

int inimicStaticX, inimicStaticY; // Pozitia inamicului static


ifstream fisier("harta.txt");
ifstream fisier2("harta-mediu.txt");

void afiseazaMeniu(int latimeFereastra, int inaltimeFereastra);

// Fereastra "Cum se joaca"

void cumSeJoaca(int latimeFereastra, int inaltimeFereastra)
{   PlaySound("menu.wav", NULL, SND_ASYNC);
    initwindow(latimeFereastra, inaltimeFereastra, "Cum se joaca?");

    // Afisam imaginea de fundal
    readimagefile("background.jpg", 0, 0, latimeFereastra, inaltimeFereastra);

    // Instructiunile jocului
    settextstyle(DEFAULT_FONT, HORIZ_DIR, 2);
    char instructiuni[][100] = {
        "- Mario trebuie sa adune toate stelutele.",
        "- Foloseste tastele SAGETI pentru a te misca.",
        "- Evita inamicii, altfel vei pierde.",
        "- Aduna toate stelutele pentru a castiga!"
    };
    int numarInstructiuni = sizeof(instructiuni) / sizeof(instructiuni[0]);
    int totalInaltimeText = numarInstructiuni * textheight("A") + (numarInstructiuni - 1) * 20; // Spacing intre randuri

    int startY = (inaltimeFereastra - totalInaltimeText) / 2; // Pozitionare pe verticala pentru instructiuni

    // Titlul "INSTRUCTIUNI"
    settextstyle(COMPLEX_FONT, HORIZ_DIR, 3);
    setcolor(WHITE);
    int titluLatime = textwidth("INSTRUCTIUNI");
    int titluInaltime = textheight("INSTRUCTIUNI");
    int titluY = startY - titluInaltime - 30; // Pozitionam titlul deasupra instructiunilor
    outtextxy((latimeFereastra - titluLatime) / 2, titluY, "INSTRUCTIUNI");

    // Afisam instructiunile
    for (int i = 0; i < numarInstructiuni; ++i)
    {
        int latimeText = textwidth(instructiuni[i]);
        outtextxy((latimeFereastra - latimeText) / 2, startY + i * (textheight("A") + 20), instructiuni[i]);
    }

    // Butonul BACK
    int butonLatime = 150;
    int butonInaltime = 50;

    int x1 = (latimeFereastra - butonLatime) / 2; // Centrat pe orizontala
    int y1 = inaltimeFereastra - inaltimeFereastra / 3; // Pozitionat 1/3 din inaltimea ferestrei
    int x2 = x1 + butonLatime;
    int y2 = y1 + butonInaltime;

    // Afisam imaginea pentru butonul BACK
    readimagefile("back-button.jpg", x1, y1, x2, y2);

    // Asteptam click pe buton
    while (true)
    {
        if (ismouseclick(WM_LBUTTONDOWN))
        {
            int mouseX = mousex(), mouseY = mousey();
            clearmouseclick(WM_LBUTTONDOWN);

            // Verificam daca s-a dat click
            if (mouseX >= x1 && mouseX <= x2 && mouseY >= y1 && mouseY <= y2)
            {
                closegraph();
                afiseazaMeniu(latimeFereastra, inaltimeFereastra);
                return; // Revenim la meniul principal
            }
        }
    }
}

void afiseazaMeniu(int latimeFereastra, int inaltimeFereastra)
{
    PlaySound("menu.wav", NULL, SND_ASYNC);
    // Dimensiuni pentru fundal si butoane
    const int fundalLatime = latimeFereastra;
    const int fundalInaltime = inaltimeFereastra;
    const int butonLatime = 300;  // Dimensiune buton mai mare
    const int butonInaltime = 100; // Dimensiune buton mai mare

    // Initializare fereastra
    initwindow(latimeFereastra, inaltimeFereastra, "Meniu Mario");

    // Afisare imagine fundal
    readimagefile("background.jpg", 0, 0, fundalLatime, fundalInaltime);

    // Coordonate pentru butoane centrate
    int playX1 = (latimeFereastra - butonLatime) / 2;
    int playY1 = (inaltimeFereastra - (2 * butonInaltime + 50)) / 2; // Mijloc pe verticala
    int playX2 = playX1 + butonLatime;
    int playY2 = playY1 + butonInaltime;

    int cumX1 = playX1;
    int cumY1 = playY2 + 50; // Spatiu intre butoane
    int cumX2 = cumX1 + butonLatime;
    int cumY2 = cumY1 + butonInaltime;

    // Afisare butoane
    readimagefile("play-button.jpg", playX1, playY1, playX2, playY2);
    readimagefile("howto-button.jpg", cumX1, cumY1, cumX2, cumY2);

    while (true)
    {
        if (ismouseclick(WM_LBUTTONDOWN))
        {
            int mouseX = mousex(), mouseY = mousey();
            clearmouseclick(WM_LBUTTONDOWN);

            // Verificam clic pe "Play Now"
            if (mouseX >= playX1 && mouseX <= playX2 && mouseY >= playY1 && mouseY <= playY2)
            {
                closegraph();
                return; // Trecem la joc
            }

            // Verificam clic pe "Cum se joaca?"
            if (mouseX >= cumX1 && mouseX <= cumX2 && mouseY >= cumY1 && mouseY <= cumY2)
            {
                closegraph();
                cumSeJoaca(latimeFereastra, inaltimeFereastra);
                return;
            }
        }
    }
}

void afiseazaMario()
{
    int y = imario * latime, x = jmario * latime;

    // Verificam daca Mario este pe o scara si NU este in ipostaza -10 sau 10
    if (harta[imario][jmario] == '#' && ipostaza != -10 && ipostaza != 10)
    {
        readimagefile("Mario-spate1.jpg", x, y, x + latime, y + latime);
        return;
    }

    // Restul cazurilor pentru diferite ipostaze
    switch (ipostaza)
    {
    case -10:
        readimagefile("Mario-spate1.jpg", x, y, x + latime, y + latime);
        break;
    case 10:
        readimagefile("Mario-spate2.jpg", x, y, x + latime, y + latime);
        break;
    case 1:
        readimagefile("Mario.jpg", x, y, x + latime, y + latime);
        break;
    case 2:
        readimagefile("Mario2.jpg", x, y, x + latime, y + latime);
        break;
    case 3:
        readimagefile("Mario3.jpg", x, y, x + latime, y + latime);
        break;
    case -1:
        readimagefile("Mario4.jpg", x, y, x + latime, y + latime);
        break;
    case -2:
        readimagefile("Mario5.jpg", x, y, x + latime, y + latime);
        break;
    case -3:
        readimagefile("Mario6.jpg", x, y, x + latime, y + latime);
        break;
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
    if (c=='S')
        readimagefile("stea.jpg",latime*j,latime*i,latime*(j+1),latime*(i+1));
}


void stergeMario()
{
    afiseazaPoza(harta[imario][jmario],imario,jmario);
}
void incarcaHarta2();

void afiseazaIesireJoc()
{
    PlaySound("menu.wav", NULL, SND_ASYNC);
    int latimeFereastra = latime * MAX;
    int inaltimeFereastra = latime * MAX;

    // Initializare fereastra "Iesire din joc"
    initwindow(latimeFereastra, inaltimeFereastra, "Iesire din joc");
    cleardevice();

    // Fundal pentru fereastra
    readimagefile("background.jpg", 0, 0, latimeFereastra, inaltimeFereastra);

    // Textul "AI TERMINAT JOCUL!"
    settextstyle(COMPLEX_FONT, HORIZ_DIR, 4);
    setcolor(WHITE);
    int textLatime = textwidth("AI TERMINAT JOCUL!");
    int textInaltime = textheight("AI TERMINAT JOCUL!");
    int textX = (latimeFereastra - textLatime) / 2;
    int textY = (inaltimeFereastra / 2) - 150;
    outtextxy(textX, textY, "AI TERMINAT JOCUL!");

    // Dimensiuni buton
    int butonLatime = 300;
    int butonInaltime = 80;

    // Coordonate buton "Iesire"
    int exitX1 = (latimeFereastra - butonLatime) / 2;
    int exitY1 = (inaltimeFereastra - butonInaltime) / 2;
    int exitX2 = exitX1 + butonLatime;
    int exitY2 = exitY1 + butonInaltime;

    // Imagine buton "Exit"
    readimagefile("exit.jpg", exitX1, exitY1, exitX2, exitY2);

    // Asteptam click pe buton
    while (true)
    {
        if (ismouseclick(WM_LBUTTONDOWN))
        {
            int mouseX = mousex(), mouseY = mousey();
            clearmouseclick(WM_LBUTTONDOWN);

            // Verificam daca s-a dat click pe butonul "Iesire"
            if (mouseX >= exitX1 && mouseX <= exitX2 && mouseY >= exitY1 && mouseY <= exitY2)
            {
                closegraph();
                exit(0); // Iesire din joc
            }
        }
    }
}

void afiseazaScor(int &stop)
{
    int i;
    for (i = 1; i <= scor*2; i= i+2)
    {
        PlaySound("stea.wav", NULL, SND_ASYNC);
        readimagefile("Stea.jpg", 30 * i, 0, 30 * i + 30, 30);
        readimagefile("Stea.jpg", 30 * i, 40, 30 * i + 30, 70);

    }
    afiseazaMario();
    if (scor < nrstelute)
    {
        for (i = 800; i <= 1200; i += 30) Beep(i, 20);
    }
    else
    {
        closegraph();

        // Daca jocul cu "harta-mediu.txt" este terminat, apelam fereastra de iesire
        if (stop != 0)
        {
            afiseazaIesireJoc(); // Afisam fereastra "Iesire din joc"
        }
        else
        {
            PlaySound("go", NULL, SND_ASYNC);
            // Afisare fereastra pentru nivelul urmator
            int latimeFereastra = latime * MAX;
            int inaltimeFereastra = latime * MAX;

            // Initializeaza fereastra pentru mesajul de trecere la nivel
            initwindow(latimeFereastra, inaltimeFereastra, "Nivelul urmator");

            // Seteaza fundalul cu imaginea "background.jpg"
            readimagefile("background.jpg", 0, 0, latimeFereastra, inaltimeFereastra);

            // Afisam textul "FELICITĂRI!" centrat, deasupra butonului
            settextstyle(COMPLEX_FONT, HORIZ_DIR, 4); // Font complex, marime 4
            setcolor(WHITE);
            int textLatime = textwidth("FELICITARI!");
            int textInaltime = textheight("FELICITARI!");
            int textX = (latimeFereastra - textLatime) / 2; // Centrat orizontal
            int textY = (inaltimeFereastra / 2) - 150;      // Pozitionat deasupra butonului
            outtextxy(textX, textY, "FELICITARI!");

            // Coordonatele pentru butonul centrat
            int butonLatime = 200;
            int butonInaltime = 100;
            int x1 = (latimeFereastra - butonLatime) / 2;
            int y1 = (inaltimeFereastra - butonInaltime) / 2;
            int x2 = x1 + butonLatime;
            int y2 = y1 + butonInaltime;

            // Afisam imaginea "next-button.jpg"
            readimagefile("next-button.jpg", x1, y1, x2, y2);

            // Asteapta interactiunea utilizatorului
            while (true)
            {
                if (ismouseclick(WM_LBUTTONDOWN))
                {
                    int mouseX = mousex(), mouseY = mousey();
                    clearmouseclick(WM_LBUTTONDOWN);

                    // Verifica daca s-a dat click pe buton
                    if (mouseX >= x1 && mouseX <= x2 && mouseY >= y1 && mouseY <= y2)
                    {
                        closegraph();
                        stop = 1;
                        incarcaHarta2();

                        ipostaza = 1;
                        afiseazaMario();
                        scor = 0;
                        return;
                    }
                }
            }
        }
    }
}


void urmatoareaIpostaza(int &stop)
{
    if (directie == "dreapta")
    {
        if (abs(ipostaza) == 10) ipostaza = 1;
        if (ipostaza < 0) ipostaza = -ipostaza;
        ipostaza++;
        if (ipostaza == 4) ipostaza = 1;

        // Verificam blocul la dreapta si permitem scara
        if (jmario < nrColoane - 1 && (harta[imario][jmario + 1] == '.' || harta[imario][jmario + 1] == '#' || harta[imario + 1][jmario + 1] == '@'))
        {
            jmario++;
            if (harta[imario][jmario] == '*') // Daca gaseste o stea
            {
                scor++;
                harta[imario][jmario] = '.';
                afiseazaScor(stop);
            }
        }
    }
    else if (directie == "stanga")
    {
        if (abs(ipostaza) == 10) ipostaza = -1;
        if (ipostaza > 0) ipostaza = -ipostaza;
        ipostaza--;
        if (ipostaza == -4) ipostaza = -1;

        // Verificam blocul la stanga si permitem scara
        if (jmario > 0 && (harta[imario][jmario - 1] == '.' || harta[imario][jmario - 1] == '#' || harta[imario + 1][jmario - 1] == '@'))
        {
            jmario--;
            if (harta[imario][jmario] == '*') // Daca gaseste o stea
            {
                scor++;
                harta[imario][jmario] = '.';
                afiseazaScor(stop);
            }
        }
    }
    else if (directie == "sus")
    {
        if (abs(ipostaza) == 10) ipostaza = -ipostaza;
        else ipostaza = 10;

        if (imario > 0 && harta[imario - 1][jmario] == '#') // Urca pe scari
            imario--;
    }
    else if (directie == "jos")
    {
        if (abs(ipostaza) == 10) ipostaza = -ipostaza;
        else ipostaza = -10;

        if (imario < nrLinii - 1 && harta[imario + 1][jmario] == '#') // Coboara pe scari
            imario++;
    }
}


void verificaSiCadeMario()
{
    while (imario < nrLinii - 1 && (harta[imario + 1][jmario] == '.' || harta[imario + 1][jmario] == '%'))
    {
        stergeMario();
        imario++;
        afiseazaMario();
        delay(50);
    }

    // Mario a atins solul (orice altceva decat spatiu sau nor)
    if (harta[imario + 1][jmario] != '.' && harta[imario + 1][jmario] != '%')
    {
        poateSari = true; // Mario poate sari din nou
    }
}

void sareMario()
{
    PlaySound("jump.wav", NULL, SND_ASYNC);
    if (!poateSari) return; // Blochez saritura daca Mario deja sare
    poateSari = false;

    int inaltimeSaritura = 2; // Cate linii urca Mario
    int latimeSaritura = 1;   // Cate coloane poate sari Mario in lateral
    int durataSaritura = 50;  // Delay intre miscari

    // Dependenta directiei
    int directieLateral = 0;
    if (directie == "dreapta") {
        directieLateral = 1;
    } else if (directie == "stanga") {
        directieLateral = -1;
    }

    // Mario sare in sus si lateral (daca este specificata directia)
    for (int i = 0; i < inaltimeSaritura; i++)
    {
        if (imario > 0 && (harta[imario - 1][jmario] == '.' || harta[imario - 1][jmario] == '%'))
        {
            stergeMario();
            imario--;
            if (directieLateral != 0 && jmario + directieLateral >= 0 && jmario + directieLateral < nrColoane &&
                (harta[imario][jmario + directieLateral] == '.' || harta[imario][jmario + directieLateral] == '%'))
            {
                jmario += directieLateral; // Se deplaseaza si in lateral
            }
            afiseazaMario();
            delay(durataSaritura);
        }
        else
            break; // Daca Mario nu poate urca, iesim din bucla
    }

    // Mario cade in jos
    verificaSiCadeMario();
}


void incarcaHarta()
{
    fisier>>nrLinii>>nrColoane;
    initwindow(latime*nrColoane,latime*nrLinii,"Mario pe scari");
    setbkcolor(WHITE);
    cleardevice();
    nrstelute=0;
    for (i=0; i<nrLinii; i++)
    {
        for (j=0; j<nrColoane; j++)
        {
            fisier>>car;
            if (car=='*') nrstelute++;
            if (car=='M')
            {
                imario=i;
                jmario=j;
                car='.';
            }
            if (car == 'I')
            {
                inimicMobilX = i;
                inimicMobilY = j;
                limitaStanga = j - 1; // Limita spre stanga
                limitaDreapta = j + 1; // Limita spre dreapta
                car = '.'; // Eliminam inamicul din harta
            }

            if (car == 'S')
            {
                inimicStaticX = i;
                inimicStaticY = j;
                car = '.'; // Eliminam inamicul din harta vizual
            }
            harta[i][j]=car;
            afiseazaPoza(harta[i][j],i,j);
        }
    }
    fisier.close();

    scor=0;
}

void incarcaHarta2()
{
    fisier2>>nrLinii>>nrColoane;
    initwindow(latime*nrColoane,latime*nrLinii,"Mario pe scari");
    setbkcolor(WHITE);
    cleardevice();
    nrstelute=0;
    for (i=0; i<nrLinii; i++)
    {
        for (j=0; j<nrColoane; j++)
        {
            fisier2>>car2;
            if (car2=='*') nrstelute++;
            if (car2=='M')
            {
                imario=i;
                jmario=j;
                car2='.';
            }
            if (car2 == 'I')
            {
                 if (car2 == 'I')
            {
                inimicMobilX = i;
                inimicMobilY = j;
                limitaStanga = j - 1; // Limita spre stanga
                limitaDreapta = j + 1; // Limita spre dreapta
                car2 = '.'; // Eliminam inamicul din harta
            }
            }
            if (car2 == 'S')
            {
                inimicStaticX = i;
                inimicStaticY = j;
                car2 = '.'; // Eliminam inamicul din harta vizual
            }

            harta[i][j]=car2;
            afiseazaPoza(harta[i][j],i,j);
        }
    }
    fisier2.close();
    scor=0;
}

void afiseazaInamici()
{
    // Afisam inamicul mobil
    readimagefile("inamic-mobil.jpg", latime * inimicMobilY, latime * inimicMobilX,
                  latime * (inimicMobilY + 1), latime * (inimicMobilX + 1));

    // Afisam inamicul static
    readimagefile("inamic-static.jpg", latime * inimicStaticY, latime * inimicStaticX,
                  latime * (inimicStaticY + 1), latime * (inimicStaticX + 1));
}

void miscaInamicMobil() {
    // Stergem imaginea inamicului de la pozitia curenta
    readimagefile("fundal.jpg", latime * inimicMobilY, latime * inimicMobilX,
                  latime * (inimicMobilY + 1), latime * (inimicMobilX + 1));

    // Actualizam pozitia inamicului mobil in functie de directie
    if (directieInamicMobil) { // Se misca spre dreapta
        if (inimicMobilY < limitaDreapta) {
            inimicMobilY++;
        } else {
            directieInamicMobil = false; // Schimbam directia
        }
    } else { // Se misca spre stanga
        if (inimicMobilY > limitaStanga) {
            inimicMobilY--;
        } else {
            directieInamicMobil = true; // Schimbam directia
        }
    }

    // Afisam imaginea inamicului la noua pozitie
    readimagefile("inamic-mobil.jpg", latime * inimicMobilY, latime * inimicMobilX,
                  latime * (inimicMobilY + 1), latime * (inimicMobilX + 1));
}
int main();
void verificaColiziuneCuInamici(int latimeFereastra, int inaltimeFereastra);

void reseteazaJoc()
{
    // Reincarcam prima harta si resetam variabilele
    fisier.open("harta.txt");
    incarcaHarta();
    fisier.close();

    ipostaza = 1;          // Resetam ipostaza lui Mario
    scor = 0;              // Resetam scorul
    directie = "dreapta";  // Resetam directia
    gameOver = false;      // Resetam starea jocului
}

void afiseazaGameOver(int latimeFereastra, int inaltimeFereastra)
{
    PlaySound("dead.wav", NULL, SND_ASYNC);
    // Initializeaza fereastra "Game Over"
    initwindow(latimeFereastra, inaltimeFereastra, "Game Over");
    cleardevice();

    // Incarca imaginea de fundal "game-over.jpg"
    readimagefile("game-over.jpg", 0, 0, latimeFereastra, inaltimeFereastra);

    // Dimensiuni butoane
    int butonLatime = 300;
    int butonInaltime = 80;

    // Cream un spatiu mai mare intre butoane
    int spatiuIntreButoane = 40;

    // Coordonate buton "TRY AGAIN"
    int retryX1 = (latimeFereastra - butonLatime) / 2;
    int retryY1 = (inaltimeFereastra - 3 * butonInaltime - spatiuIntreButoane) / 2; // Centrat vertical
    int retryX2 = retryX1 + butonLatime;
    int retryY2 = retryY1 + butonInaltime;

    // Coordonate buton "EXIT"
    int exitX1 = retryX1;
    int exitY1 = retryY2 + spatiuIntreButoane; // Sub butonul "TRY AGAIN" cu spatiu
    int exitX2 = exitX1 + butonLatime;
    int exitY2 = exitY1 + butonInaltime;

    // Incarca imaginea pentru butonul "TRY AGAIN"
    readimagefile("try-again.jpg", retryX1, retryY1, retryX2, retryY2);

    // Incarca imaginea pentru butonul "EXIT"
    readimagefile("exit.jpg", exitX1, exitY1, exitX2, exitY2);

    // Asteptam interactiunea utilizatorului
    while (true)
    {
        if (ismouseclick(WM_LBUTTONDOWN))
        {
            int mouseX = mousex(), mouseY = mousey();
            clearmouseclick(WM_LBUTTONDOWN);

            // Verificam daca s-a dat click pe "TRY AGAIN"
            if (mouseX >= retryX1 && mouseX <= retryX2 && mouseY >= retryY1 && mouseY <= retryY2)
            {
                closegraph();
                reseteazaJoc(); // Resetam jocul
                return;
            }

            // Verificam daca s-a dat click pe "EXIT"
            if (mouseX >= exitX1 && mouseX <= exitX2 && mouseY >= exitY1 && mouseY <= exitY2)
            {
                closegraph();
                exit(0); // Inchide jocul
            }
        }
    }
}

void verificaColiziuneCuInamici(int latimeFereastra, int inaltimeFereastra)
{
    if ((imario == inimicMobilX && jmario == inimicMobilY) ||
        (imario == inimicStaticX && jmario == inimicStaticY))
    {
        // Mario a atins un inamic
        closegraph();

        // Afiseaza ecranul "Game Over"
        afiseazaGameOver(latimeFereastra, inaltimeFereastra);
    }
}

int main()
{
    int latimeFereastra = latime * MAX; // Dimensiunea ferestrei in functie de latimea jocului
    int inaltimeFereastra = latime * MAX;


    stop = 0;
    afiseazaMeniu(latimeFereastra, inaltimeFereastra); // Afisam meniul
    incarcaHarta();
    ipostaza=1;
    afiseazaMario();
    afiseazaInamici();
    PlaySound("start.wav", NULL, SND_ASYNC);
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
        urmatoareaIpostaza(stop);
        afiseazaMario();
        afiseazaInamici();
        miscaInamicMobil();         // Miscam inamicul mobil
        verificaColiziuneCuInamici(latimeFereastra, inaltimeFereastra);
        verificaSiCadeMario();



        delay(50);

    }
    while (tasta!=ESC);    // tasta Esc (escape)

    closegraph();

    return 0;
}
