-------------------
      Zmijice
-------------------

Igrica se pokrece prevodjenjem programa komadama 
make i zatim ./zmijica
Nakon toga otvara se prozor u kome cete videti poligon
ogradjen zidovima i na sred poligona zmijicu (svetlo zeleni kvadratic)
kao i crvenu kuglicu (vockicu) koju zmijica treba da pojede. U levom gornjem uglu nalaze se brojaci koji prikazuju trenutan score kao i najbolji score koji je postignut od pokretanja igrice.
Kad god zmijica pojede vockicu score ce se uvecati za 5.

Kamera je postavljena iznad donjeg desnog coska poligona.
Uzimajuci to u obzir sada posmatramo kako ce se zmijica pomerati.

Kretanje zmijice odredjuje se pritiscima na tastere:
w/W - zmijica ce ici ka gornjem zidu poligona
s/S - zmijica ce ici ka donjem zidu poligona
a/A - zmijica ce ici ka levom zidu poligona
d/D - zmijica ce ici ka desnom zidu poligona

Isti efekat postize se i pritiscima na strelice.
Dovoljno je jednom pritisnuti taster, sve dok se ne pritisne drugi zmijica ima isti smer kretanja.

Kada zmijica pojede vockicu, nova ce se pojaviti na slucajno odabranom mestu a duzina zmijice ce se povecati.
Ukoliko zmijica udari u zid, ona ce se na mestu na kom je udarila skupiti na svoju pocetnu duzinu (koju je imala pri pokretanju igrice).
Isto to dogodice se i ako zmijica udari sama u sebe. 
Prilikom udara score ce se vratiti na nulu a ukoliko je postignut novi najbolji rezultat on ce biti sacuvan u polju best.

Kada ste izgubili, pritiskom na taster r/R (replay) zmijica se postavlja u centar poligona a vockica se stvara na novoj poziciji.

Ukoliko igrac postige score 800 zmijica se skuplja do pocetnih dimenzija a poligon se rotira, sto oznacava da je igrica predjena.

Pritiskom na esc igrica se gasi (podaci o najboljem rezultatu se gube).
