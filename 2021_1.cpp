#include <iostream>
#include <vector>
#include <regex>
#include <mutex>
#include <thread>
#include <string>

using namespace std;

const char* PORUKA = "\n-------------------------------------------------------------------------------\n"
"0. PROVJERITE DA LI PREUZETI ZADACI PRIPADAJU VASOJ GRUPI (G1/G2)\n"
"1. SVE KLASE TREBAJU POSJEDOVATI ADEKVATAN DESTRUKTOR\n"
"2. NAMJERNO IZOSTAVLJANJE KOMPLETNIH I/ILI POJEDINIH DIJELOVA DESTRUKTORA CE BITI OZNACENO KAO TM\n"
"3. SPASAVAJTE PROJEKAT KAKO BI SE SPRIJECILO GUBLJENJE URADJENOG ZADATKA\n"
"4. ATRIBUTI, NAZIVI FUNKCIJA, TE BROJ I TIP PARAMETARA MORAJU BITI IDENTICNI ONIMA KOJI SU KORISTENI U TESTNOM CODE-U, "
"OSIM U SLUCAJU DA POSTOJI ADEKVATAN RAZLOG ZA NJIHOVU MODIFIKACIJU. OSTALE "
"POMOCNE FUNKCIJE MOZETE IMENOVATI I DODAVATI PO ZELJI.\n"
"5. IZUZETAK BACITE SAMO U FUNKCIJAMA U KOJIMA JE TO NAZNACENO.\n"
"6. FUNKCIJE KOJE NE IMPLEMENTIRATE TREBAJU BITI OBRISANE (KAKO POZIV TAKO I DEFINICIJA)!\n"
"7. NA KRAJU ISPITA SVOJE RJESENJE KOPIRATE U .DOCX FAJL (IMENOVAN BROJEM INDEKSA)!\n"
"8. RJESENJA ZADATKA POSTAVITE NA FTP SERVER U ODGOVARAJUCI FOLDER!\n"
"9. NEMOJTE POSTAVLJATI VISUAL STUDIO PROJEKTE, VEC SAMO .DOCX FAJL SA VASIM RJESENJEM!\n"
"-------------------------------------------------------------------------------\n";


const char* crt = "\n-------------------------------------------\n";
enum eRazred { PRVI = 1, DRUGI, TRECI, CETVRTI };
const char* ispisiRazred[] = { "PRVI", "DRUGI", "TRECI", "CETVRTI" };

char* GetNizKaraktera(const char* sadrzaj) {
    if (sadrzaj == nullptr)return nullptr;
    int vel = strlen(sadrzaj) + 1;
    char* temp = new char[vel];
    strcpy_s(temp, vel, sadrzaj);
    return temp;
}

bool ValidirajBrojTelefona(string s) {
    return regex_match(s, regex("\\+[0-9]{2,3}0?[1-9]{2}[0-9]{2,3}[-\\s]?[0-9]{2,3}"));
}

template<class T1, class T2>
class Kolekcija {
    T1* _elementi1;
    T2* _elementi2;
    int _trenutno;
    bool _omoguciDupliranje;
    bool imaIstih(T1 elem1, T2 elem2) {
        for (int i = 0; i < _trenutno; i++) {
            if (elem1 == _elementi1[i] && elem2 == _elementi2[i])
                return true;
        }
        return false;
    }
public:
    Kolekcija(bool omoguciDupliranje = true) {
        _elementi1 = nullptr;
        _elementi2 = nullptr;
        _omoguciDupliranje = omoguciDupliranje;
        _trenutno = 0;
    }
    Kolekcija(const Kolekcija& k) {
        _trenutno = k._trenutno;
        _omoguciDupliranje = k._omoguciDupliranje;
        _elementi1 = new T1[_trenutno];
        _elementi2 = new T2[_trenutno];
        for (int i = 0; i < _trenutno; i++) {
            _elementi1[i] = k._elementi1[i];
            _elementi2[i] = k._elementi2[i];
        }
    }
    Kolekcija& operator = (const Kolekcija& k) {
        if (this != &k) {
            _trenutno = k._trenutno;
            _omoguciDupliranje = k._omoguciDupliranje;
            delete[] _elementi1;
            delete[] _elementi2;
            _elementi1 = new T1[_trenutno];
            _elementi2 = new T2[_trenutno];
            for (int i = 0; i < _trenutno; i++) {
                _elementi1[i] = k._elementi1[i];
                _elementi2[i] = k._elementi2[i];
            }
        }
        return *this;
    }
    ~Kolekcija() {
        delete[]_elementi1; _elementi1 = nullptr;
        delete[]_elementi2; _elementi2 = nullptr;
    }
    T1& getElement1(int lokacija)const { return _elementi1[lokacija]; }
    T2& getElement2(int lokacija)const { return _elementi2[lokacija]; }
    int getTrenutno() { return _trenutno; }
    friend ostream& operator<< (ostream& COUT, const Kolekcija& obj) {
        for (size_t i = 0; i < obj._trenutno; i++)
            COUT << obj.getElement1(i) << " " << obj.getElement2(i) << endl;
        return COUT;
    }
    void AddElement(T1 elem1, T2 elem2) {
        if (!_omoguciDupliranje) {
            if (imaIstih(elem1, elem2))
                throw exception("Nije moguce dodati iste elemente u kolekciju.");
        }
        T1* temp1 = new T1[_trenutno + 1];
        T2* temp2 = new T2[_trenutno + 1];
        for (int i = 0; i < _trenutno; i++) {
            temp1[i] = _elementi1[i];
            temp2[i] = _elementi2[i];
        }
        temp1[_trenutno] = elem1;
        temp2[_trenutno] = elem2;
        delete[] _elementi1;
        delete[] _elementi2;
        _elementi1 = temp1;
        temp1 = nullptr;
        _elementi2 = temp2;
        temp2 = nullptr;
        _trenutno++;
    }
    Kolekcija<T1, T2> operator() (int pocetak, int kraj) {
        if (pocetak < 0 || kraj >= _trenutno || pocetak > kraj) {
            throw exception("Unijeli ste nevazeci interval");
        }
        Kolekcija<T1, T2> nova;
        for (int i = pocetak; i <= kraj; i++) {
            nova.AddElement(_elementi1[i], _elementi2[i]);
        }
        return nova;
    }
};
class Datum {
    int* _dan, * _mjesec, * _godina;
public:
    Datum(int dan = 1, int mjesec = 1, int godina = 2000) {
        _dan = new int(dan);
        _mjesec = new int(mjesec);
        _godina = new int(godina);
    }
    Datum(const Datum& d) {
        _dan = new int(*d._dan);
        _mjesec = new int(*d._mjesec);
        _godina = new int(*d._godina);
    }
    Datum& operator = (const Datum& d) {
        if (this != &d) {
            *_dan = *d._dan;
            *_mjesec = *d._mjesec;
            *_godina = *d._godina;
        }
        return *this;
    }
    bool operator == (const Datum& d) {
        return (*_dan == *d._dan) && (*_mjesec == *d._mjesec) && (*_godina == *d._godina);
    }
    bool operator>(const Datum drugi) {
        return this->toDays() > drugi.toDays();
    }
    ~Datum() {
        delete _dan; _dan = nullptr;
        delete _mjesec; _mjesec = nullptr;
        delete _godina; _godina = nullptr;
    }
    friend ostream& operator<< (ostream& COUT, const Datum& obj) {
        COUT << *obj._dan << "." << *obj._mjesec << "." << *obj._godina;
        return COUT;
    }
    int toDays() const{
        return *_godina * 365 + *_mjesec * 30 + *_dan;
    }
};

class Predmet {
    char* _naziv;
    Kolekcija<Datum*, int>* _ocjene;
public:
    Predmet(const char* naziv = "", Datum datum = Datum(), int ocjena = 0) {
        _naziv = GetNizKaraktera(naziv);
        _ocjene = new Kolekcija<Datum*, int>();
        if (ocjena > 0)
            AddOcjena(datum, ocjena);
    }
    Predmet(const Predmet& p) {
        _naziv = GetNizKaraktera(p._naziv);
        _ocjene = new Kolekcija<Datum*, int>(*p._ocjene);
    }
    Predmet& operator = (const Predmet& p) {
        if (this != &p) {
            delete[] _naziv;
            _naziv = GetNizKaraktera(p._naziv);
            _ocjene = new Kolekcija<Datum*, int>(*p._ocjene);
        }
        return *this;
    }
    bool operator == (const Predmet& p) {
        return strcmp(_naziv, p._naziv) == 0;
    }
    ~Predmet() {
        delete[] _naziv; _naziv = nullptr;
    }
    char* GetNaziv() { return _naziv; }
    Kolekcija<Datum*, int> GetOcjene() { return _ocjene; }
    friend ostream& operator<< (ostream& COUT, Predmet& obj) {
        COUT << "Predmet:" << obj._naziv << endl;
        COUT << "Ocjene:" << endl;
        for (int i = 0; i < obj._ocjene->getTrenutno(); i++) {
            COUT << "Ocjena:" << obj._ocjene->getElement2(i) << ". Datum:" << *obj._ocjene->getElement1(i) << endl;
        }
        COUT << "Prosjek ocjena: " << obj.getProsjekPredmeta() << endl;
        return COUT;
    }
    void AddOcjena(Datum& d, int ocjena) {
        _ocjene->AddElement(new Datum(d), ocjena);
    }
    float getProsjekPredmeta() {
        float suma = 0.0;
        for (int i = 0; i < _ocjene->getTrenutno(); i++) {
            suma += _ocjene->getElement2(i);
        }
        return suma / _ocjene->getTrenutno();
    }
};
class Uspjeh {
    eRazred _razred;
    //char* se odnosi na napomenu o polozenom predmetu
    Kolekcija<Predmet*, const char*> _polozeniPredmeti;
public:
    Uspjeh(eRazred razred = PRVI) {
        _razred = razred;
    }
    Uspjeh(const Uspjeh& u) {
        _razred = u._razred;
        _polozeniPredmeti = u._polozeniPredmeti;
    }
    Uspjeh& operator = (const Uspjeh& u) {
        if (this != &u) {
            _razred = u._razred;
            _polozeniPredmeti = u._polozeniPredmeti;
        }
        return *this;
    }
    Kolekcija<Predmet*, const char*> GetPredmeti() { return _polozeniPredmeti; }
    eRazred GetERazred() { return _razred; }
    friend ostream& operator<< (ostream& COUT, const Uspjeh& obj) {
        COUT << obj._razred << " " << obj._polozeniPredmeti << endl;
        return COUT;
    }
    float getProsjekRazreda() {
        float suma = 0.0;
        for (int i = 0; i < _polozeniPredmeti.getTrenutno(); i++) {
            suma += _polozeniPredmeti.getElement1(i)->getProsjekPredmeta();
        }
        return suma / _polozeniPredmeti.getTrenutno();
    }
};
mutex m;
class Kandidat {
    char* _imePrezime;
    string _emailAdresa;
    string _brojTelefona;
    vector<Uspjeh> _uspjeh;
    void PosaljiMail(eRazred razred, float prosjek) {
        m.lock();
        cout << "FROM:info@fit.ba" << endl;
        cout << "   TO : " << _emailAdresa << endl;
        cout << "  Postovani "<< _imePrezime <<", evidentirali ste uspjeh za"<< ispisiRazred[razred] <<" razred.Dosadasnji uspjeh(prosjek)" << endl;
        cout << " na nivou " << ispisiRazred[razred] << " razreda iznosi " << prosjek << ", a ukupni uspjeh u toku skolovanja iznosi " << getUkupanProsjek() << "." << endl;
        cout << "  Pozdrav." << endl;
        cout << "  FIT Team." << endl;
        m.unlock();
    }
    void PosaljiSMS(eRazred razred, float prosjek) {
        m.lock();
        cout << "Svaka cast za uspjeh " << prosjek << " u " << ispisiRazred[razred] << " razredu." << endl;
        m.unlock();
    }
public:
    Kandidat(const char* imePrezime, string emailAdresa, string brojTelefona) {
        _imePrezime = GetNizKaraktera(imePrezime);
        _emailAdresa = emailAdresa;
        _brojTelefona = ValidirajBrojTelefona(brojTelefona) ? brojTelefona : "NOT VALID";//izvrsiti validaciju broja telefona
    }
    Kandidat(const Kandidat& k) {
        _imePrezime = GetNizKaraktera(k._imePrezime);
        _emailAdresa = k._emailAdresa;
        _uspjeh = k._uspjeh;
    }
    Kandidat& operator = (const Kandidat& k) {
        if (this != &k) {
            delete[] _imePrezime;
            _imePrezime = GetNizKaraktera(k._imePrezime);
            _emailAdresa = k._emailAdresa;
            _uspjeh = k._uspjeh;
        }
        return *this;
    }
    ~Kandidat() {
        delete[] _imePrezime; _imePrezime = nullptr;
    }
    friend ostream& operator<< (ostream& COUT, Kandidat& obj) {
        COUT << obj._imePrezime << " " << obj._emailAdresa << " " << obj._brojTelefona << endl;
        for (size_t i = 0; i < obj._uspjeh.size(); i++)
            COUT << obj._uspjeh[i];
        return COUT;
    }
    vector<Uspjeh>& GetUspjeh() { return _uspjeh; }
    string GetEmail() { return _emailAdresa; }
    string GetBrojTelefona() { return _brojTelefona; }
    char* GetImePrezime() { return _imePrezime; }
    /*
    uspjeh (tokom srednjoskolskog obrazovanja) se dodaje za svaki predmet na nivou razreda.
    tom prilikom onemoguciti:
    - dodavanje istih (moraju biti identicne vrijednosti svih clanova tipa Predmet) predmeta na nivou jednog razreda,
    - dodavanje predmeta kod kojih je prosjecna ocjena manja od 2.5
    - dodavanje vise od 5 predmeta na nivou jednog razreda
    razredi (predmeti ili uspjeh) ne moraju biti dodavani sortiranim redoslijedom (npr. prvo se moze dodati uspjeh za II razred, pa onda za I razred i sl.).
    Metoda vraca true ili false u zavisnosti od (ne)uspjesnost izvrsenja
    */
    bool AddPredmet(eRazred razred, Predmet& predmet, const char* napomena) {
        for (int i = 0; i < _uspjeh.size(); i++) {
            if (razred == _uspjeh[i].GetERazred()) {
                for (int j = 0; j < _uspjeh[i].GetPredmeti().getTrenutno(); j++) {
                    if (&predmet == _uspjeh[i].GetPredmeti().getElement1(j))
                        return false;
                }
                if (predmet.getProsjekPredmeta() < 2.5)
                    return false;
                if (_uspjeh[i].GetPredmeti().getTrenutno() >= 5)
                    return false;
                _uspjeh[i].GetPredmeti().AddElement(&predmet, napomena);
                float prosjek = _uspjeh[i].getProsjekRazreda();
                thread mail(&Kandidat::PosaljiMail, this, razred, prosjek);
                mail.join();
                if (prosjek >= 4.5) {
                    if (ValidirajBrojTelefona(_brojTelefona)) {
                        thread sms(&Kandidat::PosaljiSMS, this, razred, prosjek);
                        sms.join();
                    }
                    else {
                        cout << "Broj nije validan" << endl;
                    }
                }
                return true;
            }
        }
        Uspjeh u(razred);
        u.GetPredmeti().AddElement(&predmet, napomena);
        _uspjeh.push_back(u);
        float temp = u.getProsjekRazreda();
        thread mail(&Kandidat::PosaljiMail, this, razred, temp);
        mail.join();
        return true;
    }
    float GetProsjecanBrojDana() {
        float prosjek = 0.0;
        for (int i = 0; i < _uspjeh.size(); i++) {
            for (int j = 0; j < _uspjeh[i].GetPredmeti().getTrenutno(); j++) {
                for (int k = 0; k < _uspjeh[i].GetPredmeti().getElement1(j)->GetOcjene().getTrenutno() - 1; k++) {
                    prosjek = _uspjeh[i].GetPredmeti().getElement1(j)->GetOcjene().getElement1(k)->toDays() + _uspjeh[i].GetPredmeti().getElement1(j)->GetOcjene().getElement1(k + 1)->toDays();
                }
                return prosjek / _uspjeh[i].GetPredmeti().getElement1(j)->GetOcjene().getTrenutno();
            }
        }
    }
    Kolekcija<Predmet, float> operator () (Datum* d1, Datum* d2) {
        Kolekcija<Predmet, float> nova;
        float prosjecanBrojDana = GetProsjecanBrojDana();
        for (int i = 0; i < _uspjeh.size(); i++) {
            for (int j = 0; j < _uspjeh[i].GetPredmeti().getTrenutno(); j++) {
                for (int k = 0; k < _uspjeh[i].GetPredmeti().getElement1(j)->GetOcjene().getTrenutno(); k++) {
                    if (_uspjeh[i].GetPredmeti().getElement1(j)->GetOcjene().getElement1(k) > d1 && d2 > _uspjeh[i].GetPredmeti().getElement1(j)->GetOcjene().getElement1(k)) {
                        nova.AddElement(*_uspjeh[i].GetPredmeti().getElement1(j), GetProsjecanBrojDana());
                    }
                }
            }
        }
        return nova;
    }
    float getUkupanProsjek() {
        float suma = 0.0;
        for (int i = 0; i < _uspjeh.size(); i++) {
            suma += _uspjeh[i].getProsjekRazreda();
        }
        return suma / _uspjeh.size();
    }
};

void main() {

    cout << PORUKA;
    cin.get();

    Datum
        datum19062021(19, 6, 2021),
        datum20062021(20, 6, 2021),
        datum30062021(30, 6, 2021),
        datum05072021(5, 7, 2021);

    int kolekcijaTestSize = 9;
    Kolekcija<int, int> kolekcija1(false);
    for (int i = 0; i <= kolekcijaTestSize; i++)
        kolekcija1.AddElement(i, i);

    try {
        //ukoliko nije dozvoljeno dupliranje elemenata (provjeravaju se T1 i T2), metoda AddElement baca izuzetak
        kolekcija1.AddElement(3, 3);
    }
    catch (exception& err) {
        cout << err.what() << crt;
    }
    cout << kolekcija1 << crt;

    /*objekat kolekcija2 ce biti inicijalizovan elementima koji se u objektu kolekcija1 nalaze na lokacijama 1 - 4
    ukljucujuci i te lokacije. u konkretnom primjeru to ce biti parovi sa vrijednostima: 1 1 2 2 3 3 4 4*/
    Kolekcija<int, int> kolekcija2 = kolekcija1(1, 4);
    cout << kolekcija2 << crt;
    try {
        //primjeri u kojima opseg nije validan, te bi funkcija trebala baciti izuzetak
        Kolekcija<int, int> temp1 = kolekcija1(1, 14);//imamo 10 elemenata
        Kolekcija<int, int> temp2 = kolekcija1(-1, 8);//lokacija -1 ne postoji
    }
    catch (exception& err) {
        cout << err.what() << crt;
    }

    //parametri: nazivPredmeta, datum, prva ocjena
    Predmet Matematika("Matematika", datum19062021, 5),
        Fizika("Fizika", datum20062021, 5),
        Hemija("Hemija", datum30062021, 2),
        Engleski("Engleski", datum05072021, 5);

    Matematika.AddOcjena(datum05072021, 3);
    Matematika.AddOcjena(datum05072021, 3);

    // ispisuje: naziv predmeta, ocjene (zajedno sa datumom polaganja) i prosjecnu ocjenu na predmetu
    // ukoliko predmet nema niti jednu ocjenu prosjecna treba biti jednaka 0
    cout << Matematika << endl;


    /*
    broj telefona mora biti u formatu
    - znak +
    - pozivni broj drzave (2 ili 3 cifre)
    - pozivni broj operatera (2 cifre) npr. 063, 061, 065 pri cemu je broj 0 opcionalan
    - prvi dio broja (2 ili 3 cifre)
    - razmak ili crtica, oboje opcionalno tj. broj telefona ne mora sadrzavati niti jedno niti drugo
     - drugi dio broja (2 ili 3 cifre)
    ukoliko broj telefona nije u validnom formatu, njegova vrijednost se postavlja na NOT VALID
    */

    if (ValidirajBrojTelefona("+38761222333"))
        cout << "Broj telefona validan" << crt;
    if (ValidirajBrojTelefona("+38761222-333"))
        cout << "Broj telefona validan" << crt;
    if (ValidirajBrojTelefona("+38761222 333"))
        cout << "Broj telefona validan" << crt;
    if (ValidirajBrojTelefona("+387061222 333"))
        cout << "Broj telefona validan" << crt;
    if (!ValidirajBrojTelefona("+38761 222 333"))
        cout << "Broj NIJE telefona validan" << crt;
    if (!ValidirajBrojTelefona("+387 61222 333"))
        cout << "Broj NIJE telefona validan" << crt;


    Kandidat jasmin("Jasmin Azemovic", "jasmin@fit.ba", "+38761222333");
    Kandidat adel("Adel Handzic", "adel@edu.fit.ba", "+387061222 333");
    Kandidat brojTelefonaNotValid("Ime Prezime", "korisnik@klix.ba", "+387 61222 333");
  
    /*
    uspjeh (tokom srednjoskolskog obrazovanja) se dodaje za svaki predmet na nivou razreda.
    tom prilikom onemoguciti:
    - dodavanje istih (moraju biti identicne vrijednosti svih clanova tipa Predmet) predmeta na nivou jednog razreda,
    - dodavanje predmeta kod kojih je prosjecna ocjena manja od 2.5
    - dodavanje vise od 5 predmeta na nivou jednog razreda
    razredi (predmeti ili uspjeh) ne moraju biti dodavani sortiranim redoslijedom (npr. prvo se moze dodati uspjeh za II razred, pa onda za I razred i sl.).
    Metoda vraca true ili false u zavisnosti od (ne)uspjesnost izvrsenja
    */
    if (jasmin.AddPredmet(DRUGI, Fizika, "Napomena 1"))
        cout << "Predmet uspjesno dodan!" << crt;
    if (jasmin.AddPredmet(DRUGI, Hemija, "Napomena 2"))
        cout << "Predmet uspjesno dodan!" << crt;
    if (jasmin.AddPredmet(PRVI, Engleski, "Napomena 3"))
        cout << "Predmet uspjesno dodan!" << crt;
    if (jasmin.AddPredmet(PRVI, Matematika, "Napomena 4"))
        cout << "Predmet uspjesno dodan!" << crt;
    //Matematiku je vec dodana u prvom razredu
    if (jasmin.AddPredmet(PRVI, Matematika, "Napomena 5"))
        cout << "Predmet uspjesno dodan!" << crt;
  
    /*nakon evidentiranja uspjeha na bilo kojem predmetu kandidatu se salje email sa porukom:
    FROM:info@fit.ba
    TO: emailKorisnika
    Postovani ime i prezime, evidentirali ste uspjeh za X razred. Dosadasnji uspjeh (prosjek)
    na nivou X razreda iznosi Y, a ukupni uspjeh u toku skolovanja iznosi Z.
    Pozdrav.
    FIT Team.
    ukoliko je prosjek na nivou tog razreda veci od 4.5 kandidatu se salje SMS sa porukom: "Svaka cast za uspjeh 4.X u X razredu". Slanje SMS-a izvrsiti samo u slucaju da je broj telefona validna, u protivnom ispisati poruku �BROJ TELEFONA NIJE VALIDAN�
    slanje poruka i emailova implemenitrati koristeci zasebne thread-ove.
    */
    cout << jasmin << crt;
  
    //vraca kolekciju predmeta koji sadrze najmanje jednu ocjenu evidentiranu u periodu izmedju proslijedjenih datuma
    //float se odnosi na prosjecan broj dana izmedju ostvarenih ocjena na predmetu
    Kolekcija<Predmet, float> jasminUspjeh = jasmin(new Datum(18, 06, 2021), new  Datum(21, 06, 2021));
    cout << jasminUspjeh << crt;
  
    cin.get();
    system("pause>0");
}