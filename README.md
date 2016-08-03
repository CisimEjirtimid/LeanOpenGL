Zadaci za 3D protocne sisteme:

1. Ucitati podatke o visinama i teksturu. (Podaci o visinama mogu se učitati u jednokanalnu 16-bitnu teksturu - to je bolja varijanta.)

2. U verteks šejderu kreirati grid za dati teren (matrica je dimenzija 1201x1201). Korak rešetke bi trebalo da bude 70 po X-osi a 90 po Y-osi, mada može da bude i ekvidistantna rešetka. U datoteci n42_e022_3arc_v2.bil nalaze se visine, zabeležene kao 16-bitni označeni celi brojevi.

3. U fragmen šejderu primeniti teksturu n42_e022_1arc_v3.jpg na tako formirani teren.

4. Omogućiti pokretanje kamere (rotaciju i kretanje po terenu).

5. Preneti funkcionalnost kreiranja rešetke na teselacione šejdere. Iz aplikacije zadati nivo detalja (npr. 0 - najviši nivo detalja (očitava se svaki sempl iz fajla sa visinama), 1 - 4x ređa rešetka (očitava se svaki drugi o osvakoj od osa), itd).

6. Formirati wireframe mrežu zadate boje (boja se zadaje u aplikaciji) koja se iscrtava preko solid-modela (tako da se vidi iz kojih trouglova je teren sastavljen). Generisanje mreže ostvariti u geometrijskom šejderu.

7. Korišćenjem transformfeedback mehanzima očitavati visinu terena ispod kamere i na osnovu toga sprečiti da kamera prođe kroz teren prilikom kretanja (najjednostavniji oblik kolizije).

8. Uključiti debug_output i prikupiti sve informacije koje drajver prosleđuje. (Ovo bi trebalo da bude prva tačka, jer je od suštinskog značaja za uspešan rad i lako pronalaženje grešaka).

9. Izmeriti vreme potrebno da se isrta frejm i na CPU i na GPU.

10. Generisati sliku terena rezolucije veće od ekrana (npr. 2048x2048 ili 4096x4096 piksela), korišćenjem Framebuffer objekta, i snimiti je u fajl.



Zadatak će biti ocenjen na osnovu broja urađenih tačaka. Minimum je 3 (prve tri tačke - da se na ekranu vidi statična scena terena sa nalepljenom teksturom). Tačku 8 trebalo bi prvo implementirati, kako bi se lakše pronalazile greške.
