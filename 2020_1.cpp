#include <iostream>
#include <string>
#include <vector>
#include <thread>
#include <mutex>
#include <regex>

using namespace std;

const char* PORUKA = "\n-------------------------------------------------------------------------------\n"
"1. SVE KLASE TREBAJU POSJEDOVATI ADEKVATAN DESTRUKTOR\n"
"2. NAMJERNO IZOSTAVLJANJE KOMPLETNIH I/ILI POJEDINIH DIJELOVA DESTRUKTORA CE BITI OZNACENO KAO RE\n"
"3. SPASAVAJTE PROJEKAT KAKO BI SE SPRIJECILO GUBLJENJE URADJENOG ZADATKA\n"
"4. NAZIVI FUNKCIJA, TE BROJ I TIP PARAMETARA MORAJU BITI IDENTICNI ONIMA KOJI SU KORISTENI U TESTNOM CODE-U,\n"
"\tOSIM U SLUCAJU DA POSTOJI ADEKVATAN RAZLOG ZA NJIHOVU MODIFIKACIJU. OSTALE\n"
"\tPOMOCNE FUNKCIJE MOZETE IMENOVATI I DODAVATI PO ZELJI.\n"
"5. IZUZETAK BACITE SAMO U FUNKCIJAMA U KOJIMA JE TO NAZNACENO.\n"
"6. FUNKCIJE KOJE NE IMPLEMENTIRATE TREBAJU BITI OBRISANE!\n"
"7. RJESENJA ZADATKA POSTAVITE NA FTP SERVER U ODGOVARAJUCI FOLDER!\n"
"-------------------------------------------------------------------------------\n";

const char* crt = "\n-------------------------------------------\n";
enum eRazred { PRVI = 1, DRUGI, TRECI, CETVRTI };
const char* razredi[] = { "PRVI", "DRUGI", "TRECI", "CETVRTI" };

char* GetNizKaraktera(const char* sadrzaj, bool dealociraj = false) {
    if (sadrzaj == nullptr)return nullptr;
    int vel = strlen(sadrzaj) + 1;
    char* temp = new char[vel];
    strcpy_s(temp, vel, sadrzaj);
    if (dealociraj)
        delete[]sadrzaj;
    return temp;
}

bool ValidirajEmail(string s) {
    return regex_match(s, regex("[a-zA-Z]{1,}\.?[a-zA-Z]{1,}@(edu.fit|fit)\.(ba|com|org)"));
}

template<class T1, class T2>
class Kolekcija {
    T1* _elementi1;
    T2* _elementi2;
    int _trenutno;
    bool _omoguciDupliranje;
    bool postojiElement(T1 elem1, T2 elem2) {
        for (int i = 0; i < _trenutno; i++) {
            if ((elem1 == _elementi1[i] && elem2 == _elementi2[i]) && !_omoguciDupliranje)
                return true;
        }
        return false;
    }
public:
    Kolekcija(bool omoguciDupliranje = true) {
        _elementi1 = nullptr;
        _elementi2 = nullptr;
        _omoguciDupliranje = omoguciDupliranje;
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
            if (_trenutno != 0) {
                delete[] _elementi1;
                delete[] _elementi2;
            }
            _trenutno = k._trenutno;
            _omoguciDupliranje = k._omoguciDupliranje;
            _elementi1 = new T1[_trenutno];
            _elementi2 = new T2[_trenutno];
            for (int i = 0; i < _trenutno; i++) {
                _elementi1[i] = k._elementi1[i];
                _elementi2[i] = k._elementi2[i];
            }
        }
        return *this;
    }
    Kolekcija operator () (int pocetak, int kraj) {
        if (pocetak < 0 || kraj >= _trenutno || pocetak > kraj)
            throw exception("Unijeli ste nemoguci interval");
        Kolekcija<T1, T2> nova;
        for (int i = pocetak; i <= kraj; i++) {
            nova.AddElement(_elementi1[i], _elementi2[i]);
        }
        return nova;
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
    void AddElement(const T1 elem1, const T2 elem2) {
        if (postojiElement(elem1, elem2))
            throw exception("Taj element vec postoji.");
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
        _elementi2 = temp2;
        _trenutno++;
        temp1 = nullptr;
        temp2 = nullptr;
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
    int toDays() const{
        return *_godina * 365 + *_mjesec * 30 + *_dan;
    }
    bool operator == (const Datum& d) {
        if (*_godina == *d._godina && *_mjesec == *d._mjesec && *_dan == *d._dan)
            return true;
        return false;
    }
    bool operator > (const Datum d) {
        return this->toDays() > d.toDays();
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
};

class Predmet {
    char* _naziv;
    //int se odnosi na ocjenu u opsegu od 1 – 5, a datum na momenat postizanja ocjene
    Kolekcija<int, Datum> _ocjene;
public:
    Predmet(const char* naziv = "", int ocjena = 0, Datum datum = Datum()) {
        _naziv = GetNizKaraktera(naziv);
        if (ocjena > 0)
            _ocjene.AddElement(ocjena, datum);
    }
    Predmet(const Predmet& p) {
        _naziv = GetNizKaraktera(p._naziv);
        _ocjene = p._ocjene;
    }
    Predmet& operator = (const Predmet& p) {
        if (this != &p) {
            delete[] _naziv;
            _naziv = GetNizKaraktera(p._naziv);
            _ocjene = p._ocjene;
        }
        return *this;
    }
    bool operator == (const Predmet& p) {
        return strcmp(_naziv, p._naziv) == 0;
    }
    ~Predmet() {
        delete[] _naziv; _naziv = nullptr;
    }
    friend ostream& operator<< (ostream& COUT, Predmet& obj) {
        COUT << "PREDMET:" << obj._naziv << endl;
        COUT << "OCJENE:" << endl;
        for (int i = 0; i < obj._ocjene.getTrenutno(); i++) {
            COUT << "Datum:" << obj._ocjene.getElement2(i) << " Ocjena:" << obj._ocjene.getElement1(i) << endl;
        }
        COUT << "Prosjek ocjena:" << obj.getProsjek();
        return COUT;
    }
    float getProsjek() {
        float suma = 0.0;
        for (int i = 0; i < _ocjene.getTrenutno(); i++) {
            suma += _ocjene.getElement1(i);
        }
        return suma / _ocjene.getTrenutno();
    }
    void AddOcjena(int ocjena, Datum datum) {
        _ocjene.AddElement(ocjena, datum);
    }
    char* GetNaziv() { return _naziv; }
    Kolekcija<int, Datum>& GetOcjene() { return _ocjene; }
};
class Uspjeh {
    eRazred _razred;
    //string se odnosi na napomenu o polozenom predmetu
    Kolekcija<Predmet, string>* _polozeniPredmeti;
public:
    Uspjeh(eRazred razred = PRVI) {
        _razred = razred;
        _polozeniPredmeti = new Kolekcija<Predmet, string>;
    }
    Uspjeh(const Uspjeh& u) {
        _razred = u._razred;
        _polozeniPredmeti = new Kolekcija<Predmet, string>(*u._polozeniPredmeti);
    }
    Uspjeh& operator = (const Uspjeh& u) {
        if (this != &u) {
            _razred = u._razred;
            *_polozeniPredmeti = *u._polozeniPredmeti;
        }
        return *this;
    }
    ~Uspjeh() { delete _polozeniPredmeti; _polozeniPredmeti = nullptr; }
    Kolekcija<Predmet, string>* GetPredmeti() { return _polozeniPredmeti; }
    eRazred GetERazred() { return _razred; }
    friend ostream& operator<< (ostream& COUT, const Uspjeh& obj) {
        COUT << obj._razred << " " << *obj._polozeniPredmeti << endl;
        return COUT;
    }
    float getProsjek() {
        float suma = 0.0;
        for (int i = 0; i < _polozeniPredmeti->getTrenutno(); i++) {
            suma += _polozeniPredmeti->getElement1(i).getProsjek();
        }
        return suma / _polozeniPredmeti->getTrenutno();
    }
};
mutex m;
class Kandidat {
    char* _imePrezime;
    string _emailAdresa;
    string _brojTelefona;
    vector<Uspjeh> _uspjeh;
    void PosaljiMail(eRazred razred, float prosjekRazreda, float prosjekSkolovanja) {
        m.lock();
        cout << "FROM: info@fit.ba\n" << "TO: " << _emailAdresa << endl;
        cout << "Postovani " << _imePrezime << ", evidentirali ste uspjeh za " << razredi[razred] << " razred.\nDosadasnji uspjeh";
        cout << "na nivou " << razredi[razred] << " razreda iznosi " << prosjekRazreda << ", a ukupni uspjeh u toku skolovanja iznosi " << prosjekSkolovanja << "." << endl;
        cout << "Pozdrav." << endl;
        cout << "FIT Team." << crt;
        m.unlock();
    }
    void PosaljiSMS(eRazred razred, float prosjekRazreda) {
        m.lock();
        cout << "Svaka cast za uspjeh -> " << prosjekRazreda << " u " << razredi[razred] << " razredu." << crt;
        m.unlock();
    }
public:
    Kandidat(const char* imePrezime, string emailAdresa, string brojTelefona) {
        _imePrezime = GetNizKaraktera(imePrezime);
        if (ValidirajEmail(emailAdresa))
            _emailAdresa = emailAdresa;
        else
            _emailAdresa = "noSet@edu.fit.ba";
        _brojTelefona = brojTelefona;
    }
    Kandidat(const Kandidat& k) {
        _imePrezime = GetNizKaraktera(k._imePrezime);
        _emailAdresa = k._emailAdresa;
        _brojTelefona = k._brojTelefona;
        _uspjeh = k._uspjeh;
    }
    Kandidat& operator=(const Kandidat& k) {
        if (this != &k) {
            delete[] _imePrezime;
            _imePrezime = GetNizKaraktera(k._imePrezime);
            _emailAdresa = k._emailAdresa;
            _brojTelefona = k._brojTelefona;
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
    bool postojiIsti(eRazred razred, Predmet& p) {
        for (int i = 0; i < _uspjeh.size(); i++) {
            if (razred == _uspjeh[i].GetERazred()) {
                for (int j = 0; j < _uspjeh[i].GetPredmeti()->getTrenutno(); j++) {
                    if (p == _uspjeh[i].GetPredmeti()->getElement1(j))
                        return true;
                }
            }
        }
        return false;
    }
    float getProsjekZaRazred(eRazred razred) {
        float suma = 0.0;
        for (int i = 0; i < _uspjeh.size(); i++) {
            if (_uspjeh[i].GetERazred() == razred) {
                for (int j = 0; j < _uspjeh[i].GetPredmeti()->getTrenutno(); j++) {
                    suma += _uspjeh[i].GetPredmeti()->getElement1(j).getProsjek();
                }
                return suma / _uspjeh[i].GetPredmeti()->getTrenutno();
            }
        }
    }
    bool AddPredmet(eRazred razred, Predmet& p, string napomena) {
        for (int i = 0; i < _uspjeh.size(); i++) {
            if (_uspjeh[i].GetERazred() == razred) {
                if (_uspjeh[i].GetPredmeti()->getTrenutno() >= 5)
                    return false;
                if (p.getProsjek() < 2.5)
                    return false;
                if (postojiIsti(razred, p))
                    return false;
                _uspjeh[i].GetPredmeti()->AddElement(p, napomena);
                float prosjekRazred = getProsjekZaRazred(razred);
                float prosjekSkolovanja = _uspjeh[i].getProsjek();
                thread mail(&Kandidat::PosaljiMail, this, razred, prosjekRazred, prosjekSkolovanja);
                mail.join();
                if (prosjekRazred >= 4.5) {
                    thread sms(&Kandidat::PosaljiSMS, this, razred, prosjekRazred);
                    sms.join();
                }
                return true;
            }
        }
        Uspjeh u(razred);
        u.GetPredmeti()->AddElement(p, napomena);
        _uspjeh.push_back(u);
        float prosjekRazred = getProsjekZaRazred(razred);
        float prosjekSkolovanja = u.getProsjek();
        thread mail(&Kandidat::PosaljiMail, this, razred, prosjekRazred, prosjekSkolovanja);
        mail.join();
        return true;
    }
    float GetProsjecanBrojDana() {
        float prosjek = 0.0;
        for (int i = 0; i < _uspjeh.size(); i++) {
            for (int j = 0; j < _uspjeh[i].GetPredmeti()->getTrenutno(); j++) {
                for (int k = 0; k < _uspjeh[i].GetPredmeti()->getElement1(j).GetOcjene().getTrenutno() - 1; k++) {
                    prosjek = _uspjeh[i].GetPredmeti()->getElement1(j).GetOcjene().getElement2(k).toDays() + _uspjeh[i].GetPredmeti()->getElement1(j).GetOcjene().getElement2(k + 1).toDays();
                }
                return prosjek / _uspjeh[i].GetPredmeti()->getElement1(j).GetOcjene().getTrenutno();
            }
        }
    }
    Kolekcija<Predmet, float> operator()(Datum prvi, Datum drugi) {
        Kolekcija<Predmet, float> nova;
        float prosjecanBrojDana = GetProsjecanBrojDana();
        for (int i = 0; i < _uspjeh.size(); i++) {
            for (int j = 0; j < _uspjeh[i].GetPredmeti()->getTrenutno(); j++) {
                for (int k = 0; k < _uspjeh[i].GetPredmeti()->getElement1(j).GetOcjene().getTrenutno(); k++) {
                    if (_uspjeh[i].GetPredmeti()->getElement1(j).GetOcjene().getElement2(k) > prvi && drugi > _uspjeh[i].GetPredmeti()->getElement1(j).GetOcjene().getElement2(k)) {
                        nova.AddElement(_uspjeh[i].GetPredmeti()->getElement1(j), GetProsjecanBrojDana());
                    }
                }
            }
        }
        return nova;
    }
    Uspjeh* operator[](eRazred razred) {
        Uspjeh* u = nullptr;
        for (int i = 0; i < _uspjeh.size(); i++) {
            if (_uspjeh[i].GetERazred() == razred)
                u = &_uspjeh[i];
        }
        return u;
    }
};

void main() {
    cout << PORUKA;
    cin.get();
    Datum temp,
        datum19062019(19, 6, 2019),
        datum20062019(20, 6, 2019),
        datum30062019(30, 6, 2019),
        datum05072019(5, 7, 2019);

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

    //parametri: nazivPredmeta, prva ocjena, datum
    Predmet Matematika("Matematika", 5, datum19062019),
        Fizika("Fizika", 5, datum20062019),
        Hemija("Hemija", 2, datum30062019),
        Engleski("Engleski", 5, datum05072019);

    Matematika.AddOcjena(3, datum05072019);
    Matematika.AddOcjena(5, datum05072019);

    // ispisuje: naziv predmeta, ocjene (zajedno sa datumom polaganja) i prosjecnu ocjenu na predmetu
    // ukoliko predmet nema niti jednu ocjenu prosjecna treba biti 0
    cout << Matematika << endl;

    if (ValidirajEmail("text.text@edu.fit.ba"))
        cout << "Email validan" << crt;
    if (ValidirajEmail("texttext@edu.fit.ba"))
        cout << "Email validan" << crt;
    if (ValidirajEmail("texttext@fit.ba"))
        cout << "Email validan" << crt;
    if (ValidirajEmail("texttext@fit.com"))
        cout << "Email validan" << crt;
    if (ValidirajEmail("texttext@edu.fit.org"))
        cout << "Email validan" << crt;

    /*
    email adresa mora biti u formatu text.text (sa ili bez tacke),
    dok domena moze biti fit.ba ili edu.fit.ba
    nakon institucije (fit ili edu.fit), osim drzavne (.ba), dozvoljene su domene .com i .org.
    za provjeru validnosti email adrese koristiti globalnu funkciju ValidirajEmail, a unutar nje regex metode.
    validacija email adrese ce se vrsiti unutar konstruktora klase Kandidat, a u slucaju da nije validna
    postaviti je na defaultnu adresu: notSet@edu.fit.ba
    */

    Kandidat jasmin("Jasmin Azemovic", "jasmin@fit.ba", "033 281 172");
    Kandidat adel("Adel Handzic", "adel@edu.fit.ba", "033 281 170");
    Kandidat emailNotValid("Ime Prezime", "korisnik@klix.ba", "033 281 170");

    /*
    uspjeh (tokom srednjoskolskog obrazovanja) se dodaje za svaki predmet na nivou razreda.
    tom prilikom onemoguciti:
    - dodavanje istih (moraju biti identicne vrijednosti svih clanova) predmeta na nivou jednog razreda,
    - dodavanje predmeta kod kojih je prosjecna ocjena manja od 2.5
    - dodavanje vise od 5 predmeta na nivou jednog razreda
    razredi (predmeti ili uspjeh) ne moraju biti dodavani sortiranim redoslijedom (npr. prvo se moze dodati uspjeh za II razred, pa onda za I razred i sl.).
    Funkcija vraca true ili false u zavisnosti od (ne)uspjesnost izvrsenja
    */
    if (jasmin.AddPredmet(DRUGI, Fizika, "Napomena 1"))
        cout << "Predmet uspjesno dodan!" << crt;
    if (jasmin.AddPredmet(DRUGI, Hemija, "Napomena 2"))
        cout << "Predmet uspjesno dodan!" << crt;
    if (jasmin.AddPredmet(PRVI, Engleski, "Napomena 3"))
        cout << "Predmet uspjesno dodan!" << crt;
    if (jasmin.AddPredmet(PRVI, Matematika, "Napomena 4"))
        cout << "Predmet uspjesno dodan!" << crt;
    //ne treba dodati Matematiku jer je vec dodana u prvom razredu
    if (jasmin.AddPredmet(PRVI, Matematika, "Napomena 5"))
        cout << "Predmet uspjesno dodan!" << crt;

    /*nakon evidentiranja uspjeha na bilo kojem predmetu kandidatu se salje email sa porukom:
    FROM:info@fit.ba
    TO: emailKorisnika
    Postovani ime i prezime, evidentirali ste uspjeh za X razred. Dosadasnji uspjeh (prosjek)
    na nivou X razreda iznosi Y, a ukupni uspjeh u toku skolovanja iznosi Z.
    Pozdrav.
    FIT Team.
    ukoliko je prosjek na nivou tog razreda veci od 4.5 kandidatu se salje SMS sa porukom: "Svaka cast za uspjeh 4.X u X razredu".
    slanje poruka i emailova implemenitrati koristeci zasebne thread-ove.
    */
    cout << jasmin << crt;

    //vraca kolekciju predmeta koji sadrze najmanje jednu ocjenu evidentiranu u periodu izmedju proslijedjenih datuma
    //float se odnosi na prosjecan broj dana izmedju ostvarenih ocjena na predmetu
    Kolekcija<Predmet, float> jasminUspjeh = jasmin(Datum(18, 06, 2019), Datum(21, 06, 2019));
    cout << jasminUspjeh << crt;

    Uspjeh* uspjeh_Irazred = jasmin[PRVI];//vraca uspjeh kandidata ostvaren u prvom razredu
    if (uspjeh_Irazred != nullptr)
        cout << *uspjeh_Irazred << crt;

    cin.get();
    system("pause>0");
}