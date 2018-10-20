/*
 * Soubor:  main.c
 * Datum:   2014-04-27
 * Autor:   Daniel Klimaj, xklima22@stud.fit.vutbr.cz
 * Projekt: Rivercrossing, projekt c. 2 do predmetu IOS
 * Popis:   Program riesi modifikovany synchornizacny problem River Crossing
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>
#include <ctype.h>

#include <semaphore.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/shm.h>
#include <sys/mman.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <signal.h>

const int TMAX = 5000;
const int TMIN = 0;
const int MS_TO_S = 1000;
const char *fname = "rivercrossing.out";

// Kody chyb programu
enum tecodes
{
  EOK = 0,                  // Vsetko v poriadku
  ECLWRONG,                 // Chybne parametre prikazoveho riadku
  EFOPEN,                   // Chyba pri otvarani suboru
  EISEM,                    // Chybna pri inicializacii semaforu
  EISHM,                    // Chyba pri vytvarani zdielanej pamati
  ERSEM,                    // Chyba pri uvolnovani semaforu
  ERSHM,                    // Chyba pri uvlnovani zdielanej pamati
  EFSG,                     // Chyba pro fork() serf generator
  EFHG,                     // Chyba pri fork() hacker generator
  EFHC,                     // Chyba pri fork() hacker child
  EFSC,                     // Chyba pri fork() serf child
  EUNKNOWN,                 // Neznama chyba
};

// Stavove kody programu
enum tstates
{
  SHELP = 0,                // Napoveda
  SRCROSS,                  // Rivercrossing
};

// Kody typov pasazierov
enum tpsng
{
  NONE = 0,
  HACKER,
  SERF,
};

// Chybove hlasenia kodov chyb
const char *ECODEMSG[] =
{
  [EOK]      = "Vsetko v poriadku\n",
  [ECLWRONG] = "Chybne parametre prikazoveho riadku!\n",
  [EFOPEN]   = "Nepodarilo sa otvorit subor rivercrossing.out!\n",
  [EISEM]    = "Chyba pri inicializacii semaforu!\n",
  [EISHM]    = "Chyba pri vytvarani zdielanej pamati!\n",
  [ERSEM]    = "Nepodarilo sa uvolnit semafor!\n",
  [ERSHM]    = "Nepodarilo sa uvolnit zdielanu pamat!\n",
  [EFSG]     = "Nepodarilo sa vytvorit proces pre generovanie Serfs!\n",
  [EFHG]     = "Nepodarilo sa vytvorit proces pre generovanie Hackers!\n",
  [EFHC]     = "Nepodarilo sa vytvorit potomka Hacker!\n",
  [EFSC]     = "Nepodarilo sa vytvorit potomka Serf!\n",
  [EUNKNOWN] = "Neznama chyba!\n",
};

const char *HELPMSG =
  "Program River Crossing.\n"
  "Autor: Daniel Klimaj (c) 2014\n"
  "Program riesi modifikovany synchornizacny problem River Crossing\n"
  "Pouzitie:\n"
  "\trivercrossing -h\n"
  "\trivercrossing P H S R\n"
  "Popis parametrov:\n"
  "\t-h\tVypise tento text\n"
  "\tP\tPocet osob generovanych v kategoriach hackers a serfs\n"
  "\t\tKladne parne cislo.\n"
  "\tH\tMaximalna doba v milisekundach kedy bude generovbany novy\n"
  "\t\thacker (0-5000)\n"
  "\tS\tMaximalna doba v milisekundach kedy bude generovbany novy\n"
  "\t\tserf (0-5000)\n"
  "\tR\tMaximalna doba plavby v milisekundach\n";

// Struktura obsahujuca spracovane parametre prikazoveho riadku
typedef struct params
{
  int passengers;     // Pocet cestujucich jednej kategorie
  int tmax_hacker;    // Maximalna doba medzi generovaniami 2 hackerov
  int tmax_serf;      // Maximalna doba medzi generovaniami 2 serfov
  int tmax_travel;    // Maximalna doba plavby
  int ecode;          // Chybovy kod
  int state;          // Stavovy kod
} TParams;

// Subor pre zapis vystupov
FILE *output = NULL;

// Semafory
sem_t *sem_hacker_id  = NULL; // Strazi pocitadlo s ID hackerov
sem_t *sem_serf_id    = NULL; // Strazi pocitadlo s ID serfov
sem_t *sem_actions    = NULL; // Strazi pocitadlo akcii
sem_t *sem_hboarding  = NULL; // Povoluje/zakazuje nalodovanie hackerov
sem_t *sem_sboarding  = NULL; // Povoluje/zakazuje nalodovanie serfov
sem_t *sem_dock       = NULL; // Otvara/zatvara molo
sem_t *sem_d_hackers  = NULL; // Strazi pocitadlo hackerov na mole
sem_t *sem_d_serfs    = NULL; // Strazi pocitadlo serfov na mole
sem_t *sem_b_hackers  = NULL; // Strazi pocitadla nalodujucich sa hackerov
sem_t *sem_b_serfs    = NULL; // Strazi pocitadla nalodujucich sa serfov
sem_t *sem_finish     = NULL; // Umoznuje/zakazuje porcesom ukoncit sa
sem_t *sem_writer     = NULL; // Strazi vypisovanie
sem_t *sem_travel     = NULL; // Umoznuje/zakazuje lodi vyplavat
sem_t *sem_istrvl     = NULL; // Signalizuje, ze lod sa plavi
sem_t *sem_transp     = NULL; // Strazi pocitadlo prevezenych
sem_t *sem_check1     = NULL; // Strazi 1. kontrlny blok
sem_t *sem_check2     = NULL; // Strazi 2. kontrolny blok
sem_t *sem_check3     = NULL; // Strazi 3. kontrolny blok
sem_t *sem_hcaptn     = NULL; // Zabezpecuje nalodenie hacker kapitana
sem_t *sem_scaptn     = NULL; // Zabezpecuje nalodenie serf kapitana
sem_t *sem_all_aboard = NULL; // Signalizuje, ze lod je plna

// Zdielana pamat
int *action_id    = NULL; // Pocitadlo akcii
int *cnt_hackers  = NULL; // Pocitadlo hackerov na mole
int *cnt_serfs    = NULL; // Pocitadlo serfov na mole
int *bcnt_hackers = NULL; // Pocitadlo nalodenych heckerov
int *bcnt_serfs   = NULL; // Pocitadlo nalodenych serfov
int *index_hacker = NULL; // Pocitadlo ID hackerov
int *index_serf   = NULL; // Pocitadlo ID serfov
int *board_hacker = NULL; // Kolko hackerov sa ma este nalodit
int *board_serf   = NULL; // Kolko serfov sa ma este nalodit
int *transported  = NULL; // Pocitadlo poctu prevezenych
int *cpt_type     = NULL; // Typ kapitana (HACKER || SERF)
int *landed       = NULL; // Pocet aktualne preplavenych
int *has_cpt      = NULL; // Signalizuje, ci ma lod kapitana
int *acnt_hackers = NULL; // Pocitadlo hackerov, ktory mozu pokracovat k lodi
int *acnt_serfs   = NULL; // Pocitadlo hackerov, ktory mozu pokracovat k lodi

int shm_ctl_action_id    = 0;
int shm_ctl_cnt_hackers  = 0;
int shm_ctl_cnt_serfs    = 0;
int shm_ctl_bcnt_hackers = 0;
int shm_ctl_bcnt_serfs   = 0;
int shm_ctl_index_hacker = 0;
int shm_ctl_index_serf   = 0;
int shm_ctl_board_hacker = 0;
int shm_ctl_board_serf   = 0;
int shm_ctl_transported  = 0;
int shm_ctl_cpt_type     = 0;
int shm_ctl_landed       = 0;
int shm_ctl_has_cpt      = 0;
int shm_ctl_acnt_hackers = 0;
int shm_ctl_acnt_serfs   = 0;

// Prototypy funkcii
void print_ecode(int ecode);
TParams get_params(int argc, char *argv[]);
int rsrc_init();
void rsrc_release();
void quit_on_error();
int shm_inc(int *sn, int val, sem_t *sem);
int shm_vget(int *sn, sem_t *sem);
void shm_vset(int *sn, int val, sem_t *sem);
void hacker(int ttime, int count);
void serf(int ttime, int count);

/*******************************************************************************
 * Hlavny program
 ******************************************************************************/
int main(int argc, char *argv[])
{
  // Spracovanie parametrov
  TParams params = get_params(argc, argv);
  if(params.ecode != EOK)
  {
    print_ecode(params.ecode);
    return EXIT_FAILURE;
  }

  // Vypis napovedy
  if(params.state == SHELP)
  {
    printf("%s", HELPMSG);
    return EXIT_SUCCESS;
  }

  // Riesenie River Crossing problemu
  if(params.state == SRCROSS)
  {
    int ecode = rsrc_init();
    int s_range = 0;
    int h_range = 0;

    // Signal handlery
    signal(SIGTERM, quit_on_error);
    signal(SIGINT, quit_on_error);

    if(ecode != EOK)
    {
      print_ecode(ecode);
      quit_on_error();
    }

    if((output = fopen(fname, "w+")) == NULL)
    {
      print_ecode(EFOPEN);
      rsrc_release();
      return EXIT_FAILURE;
    }

    //output = stdout;

    setbuf(output, NULL);

    pid_t pid_gen_hacker, pid_gen_serf;
    pid_t pid_hacker, pid_serf;
    srand(time(0));

    // Inicializacia pocitadiel
    *action_id    = 0;
    *cnt_hackers  = 0;
    *cnt_serfs    = 0;
    *bcnt_hackers = 0;
    *bcnt_serfs   = 0;
    *board_hacker = 0;
    *board_serf   = 0;
    *transported  = 0;
    *landed       = 0;
    *has_cpt      = 0;
    *acnt_hackers = 0;
    *acnt_serfs   = 0;

    pid_t hackers[params.passengers];
    pid_t serfs[params.passengers];

    // Generovanie cestujucich
    pid_gen_hacker = fork();
    if(pid_gen_hacker == 0)
    {
      for(int j=0; j<params.passengers; j++)
      {
        if(params.tmax_hacker == 0)
        {
          h_range = 0;
        }
        else
        {
          h_range = rand() % (params.tmax_hacker + 1);
        }

        usleep(h_range * MS_TO_S);
        pid_hacker = fork();
        {
          if(pid_hacker == 0)
          {
            hacker(params.tmax_travel, params.passengers);
          }
          else if(pid_hacker > 0)
          {
            hackers[j] = pid_hacker;
          }
          else
          {
            print_ecode(EFHC);
            quit_on_error();
          }
        }
      }
      exit(0);
    }
    else if(pid_gen_hacker > 0)
    {
      if(params.tmax_hacker == 0)
      {
        s_range = 0;
      }
      else
      {
        s_range = rand() % (params.tmax_serf + 1);
      }

      usleep(s_range * MS_TO_S);
      pid_gen_serf = fork();
      if(pid_gen_serf == 0)
      {
        for(int i=0; i<params.passengers; i++)
        {
          pid_serf = fork();
          {
            if(pid_serf == 0)
            {
              serf(params.tmax_travel, params.passengers);
            }
            else if(pid_serf > 0)
            {
              serfs[i] = pid_serf;
            }
            else
            {
              print_ecode(EFSC);
              quit_on_error();
            }
          }
        }
        exit(0);
      }
      else if(pid_gen_serf < 0)
      {
        // Nepodarilo sa vytvorit proces pre generovanie Serfs
        print_ecode(EFSG);
        quit_on_error();
      }
    }
    else
    {
      // Nepodarilo sa vytvorit preoces pre generovanie Hackers
      print_ecode(EFHG);
      quit_on_error();
    }

    for(int i=0; i<params.passengers; i++)
    {
      waitpid(hackers[i], NULL, 0);
      waitpid(serfs[i], NULL, 0);
    }

    rsrc_release();
  } // SRCROSS

  return EXIT_SUCCESS;
}

//##############################################################################

/**
 * Vypise chybove hlasenie odpovedajuce chybovemu kodu.
 * @param ecode kod chyby
 */
void print_ecode(int ecode)
{
  if (ecode < EOK || ecode > EUNKNOWN)
  { ecode = EUNKNOWN; }

  fprintf(stderr, "%s", ECODEMSG[ecode]);
}

/**
 * Spracuje argumenty prikazoveho riadku a vrati ich v strukture TParams.
 * Ak je format argumentov chybny, ukonci program s chybovym kodem.
 * @param argc Pocet argumentov
 * @param argv Pole textovych retazcov s argumentmi
 * @return Vracia analyzovane argumenty prikoazoveho riadku
 */
TParams get_params(int argc, char *argv[])
{
  TParams result =
  {
    .passengers = 0,
    .tmax_hacker = 0,
    .tmax_serf = 0,
    .ecode = EOK,
    .state = SHELP,
  };

  if(argc == 2) // Vypis napovedy
  {
    if(strcmp("-h", argv[1]) == 0)
    {
      result.state = SHELP;
    }
    else
    {
      result.ecode = ECLWRONG;
    }
  } 
  else if(argc == 5) // River Crossing
  {
    bool all_digits = true;
    for(int i=1; i<argc; i++)
      if(!(isdigit(*argv[i])))
        all_digits = false;

    if(all_digits)
    {
      int tmp;
      tmp = atoi(argv[1]);
      if(tmp > 0 && tmp % 2 == 0)
      {
        result.passengers = tmp;
      }
      else
      {
        result.ecode = ECLWRONG;
      }

      tmp = atoi(argv[2]);
      if(tmp >= TMIN && tmp <= TMAX)
      {
        result.tmax_hacker = tmp;
      }
      else
      {
        result.ecode = ECLWRONG;
      }

      tmp = atoi(argv[3]);
      if(tmp >= TMIN && tmp <= TMAX)
      {
        result.tmax_serf = tmp;
      }
      else
      {
        result.ecode = ECLWRONG;
      }

      tmp = atoi(argv[4]);
      if(tmp >= TMIN && tmp <= TMAX)
      {
        result.tmax_travel = tmp;
      }
      else
      {
        result.ecode = ECLWRONG;
      }

      result.state = SRCROSS;
    }
    else
    {
      result.ecode = ECLWRONG;
    }
  }
  else
  {
    result.ecode = ECLWRONG;
  }

  return result;
}

/**
 * Inicializacia semaforov a zdielanej pamati.
 * @return Vracia chybovy kod
 */
int rsrc_init()
{
  int ecode = EOK;

  // Inicializacia semaforov
  if((sem_serf_id    = mmap(NULL, sizeof(sem_t), PROT_READ | PROT_WRITE,
    MAP_ANONYMOUS | MAP_SHARED, 0, 0)) == MAP_FAILED) { ecode =  EISEM; }
  if((sem_hacker_id  = mmap(NULL, sizeof(sem_t), PROT_READ | PROT_WRITE,
    MAP_ANONYMOUS | MAP_SHARED, 0, 0)) == MAP_FAILED) { ecode =  EISEM; }
  if((sem_actions    = mmap(NULL, sizeof(sem_t), PROT_READ | PROT_WRITE,
    MAP_ANONYMOUS | MAP_SHARED, 0, 0)) == MAP_FAILED) { ecode = EISEM; }
  if((sem_hboarding  = mmap(NULL, sizeof(sem_t), PROT_READ | PROT_WRITE,
    MAP_ANONYMOUS | MAP_SHARED, 0, 0)) == MAP_FAILED) { ecode = EISEM; }
  if((sem_sboarding  = mmap(NULL, sizeof(sem_t), PROT_READ | PROT_WRITE,
    MAP_ANONYMOUS | MAP_SHARED, 0, 0)) == MAP_FAILED) { ecode = EISEM; }
  if((sem_dock       = mmap(NULL, sizeof(sem_t), PROT_READ | PROT_WRITE,
    MAP_ANONYMOUS | MAP_SHARED, 0, 0)) == MAP_FAILED) { ecode = EISEM; }
  if((sem_d_hackers  = mmap(NULL, sizeof(sem_t), PROT_READ | PROT_WRITE,
    MAP_ANONYMOUS | MAP_SHARED, 0, 0)) == MAP_FAILED) { ecode = EISEM; }
  if((sem_d_serfs    = mmap(NULL, sizeof(sem_t), PROT_READ | PROT_WRITE,
    MAP_ANONYMOUS | MAP_SHARED, 0, 0)) == MAP_FAILED) { ecode = EISEM; }
  if((sem_b_hackers  = mmap(NULL, sizeof(sem_t), PROT_READ | PROT_WRITE,
    MAP_ANONYMOUS | MAP_SHARED, 0, 0)) == MAP_FAILED) { ecode = EISEM; }
  if((sem_b_serfs    = mmap(NULL, sizeof(sem_t), PROT_READ | PROT_WRITE,
    MAP_ANONYMOUS | MAP_SHARED, 0, 0)) == MAP_FAILED) { ecode = EISEM; }
  if((sem_finish     = mmap(NULL, sizeof(sem_t), PROT_READ | PROT_WRITE,
    MAP_ANONYMOUS | MAP_SHARED, 0, 0)) == MAP_FAILED) { ecode = EISEM; }
  if((sem_writer     = mmap(NULL, sizeof(sem_t), PROT_READ | PROT_WRITE,
    MAP_ANONYMOUS | MAP_SHARED, 0, 0)) == MAP_FAILED) { ecode = EISEM; }
  if((sem_travel     = mmap(NULL, sizeof(sem_t), PROT_READ | PROT_WRITE,
    MAP_ANONYMOUS | MAP_SHARED, 0, 0)) == MAP_FAILED) { ecode = EISEM; }
  if((sem_istrvl     = mmap(NULL, sizeof(sem_t), PROT_READ | PROT_WRITE,
    MAP_ANONYMOUS | MAP_SHARED, 0, 0)) == MAP_FAILED) { ecode = EISEM; }
  if((sem_check1     = mmap(NULL, sizeof(sem_t), PROT_READ | PROT_WRITE,
    MAP_ANONYMOUS | MAP_SHARED, 0, 0)) == MAP_FAILED) { ecode = EISEM; }
  if((sem_check2     = mmap(NULL, sizeof(sem_t), PROT_READ | PROT_WRITE,
    MAP_ANONYMOUS | MAP_SHARED, 0, 0)) == MAP_FAILED) { ecode = EISEM; }
  if((sem_check3     = mmap(NULL, sizeof(sem_t), PROT_READ | PROT_WRITE,
    MAP_ANONYMOUS | MAP_SHARED, 0, 0)) == MAP_FAILED) { ecode = EISEM; }
  if((sem_transp     = mmap(NULL, sizeof(sem_t), PROT_READ | PROT_WRITE,
    MAP_ANONYMOUS | MAP_SHARED, 0, 0)) == MAP_FAILED) { ecode = EISEM; }
  if((sem_hcaptn     = mmap(NULL, sizeof(sem_t), PROT_READ | PROT_WRITE,
    MAP_ANONYMOUS | MAP_SHARED, 0, 0)) == MAP_FAILED) { ecode = EISEM; }
  if((sem_scaptn     = mmap(NULL, sizeof(sem_t), PROT_READ | PROT_WRITE,
    MAP_ANONYMOUS | MAP_SHARED, 0, 0)) == MAP_FAILED) { ecode = EISEM; }
  if((sem_all_aboard = mmap(NULL, sizeof(sem_t), PROT_READ | PROT_WRITE,
    MAP_ANONYMOUS | MAP_SHARED, 0, 0)) == MAP_FAILED) { ecode = EISEM; }

  if(ecode == EOK)
  {
    if(sem_init(sem_serf_id,    1, 1) == -1) { ecode = EISEM; }
    if(sem_init(sem_hacker_id,  1, 1) == -1) { ecode = EISEM; }
    if(sem_init(sem_actions,    1, 1) == -1) { ecode = EISEM; }
    if(sem_init(sem_hboarding,  1, 0) == -1) { ecode = EISEM; }
    if(sem_init(sem_sboarding,  1, 0) == -1) { ecode = EISEM; }
    if(sem_init(sem_dock,       1, 1) == -1) { ecode = EISEM; }
    if(sem_init(sem_d_hackers,  1, 1) == -1) { ecode = EISEM; }
    if(sem_init(sem_d_serfs,    1, 1) == -1) { ecode = EISEM; }
    if(sem_init(sem_b_hackers,  1, 1) == -1) { ecode = EISEM; }
    if(sem_init(sem_b_serfs,    1, 1) == -1) { ecode = EISEM; }
    if(sem_init(sem_finish,     1, 0) == -1) { ecode = EISEM; }
    if(sem_init(sem_writer,     1, 1) == -1) { ecode = EISEM; }
    if(sem_init(sem_travel,     1, 0) == -1) { ecode = EISEM; }
    if(sem_init(sem_istrvl,     1, 1) == -1) { ecode = EISEM; }
    if(sem_init(sem_check1,     1, 1) == -1) { ecode = EISEM; }
    if(sem_init(sem_check2,     1, 1) == -1) { ecode = EISEM; }
    if(sem_init(sem_check3,     1, 1) == -1) { ecode = EISEM; }
    if(sem_init(sem_transp,     1, 1) == -1) { ecode = EISEM; }
    if(sem_init(sem_hcaptn,     1, 0) == -1) { ecode = EISEM; }
    if(sem_init(sem_scaptn,     1, 0) == -1) { ecode = EISEM; }
    if(sem_init(sem_all_aboard, 1, 1) == -1) { ecode = EISEM; }
  }
  else
    return ecode;

  // Inicializacia zdielanej pamati
  if(ecode == EOK)
  {
    if((shm_ctl_action_id    = shmget(IPC_PRIVATE, sizeof(int),
      IPC_CREAT | IPC_EXCL | 0666)) == -1) { ecode = EISHM; }
    if((shm_ctl_cnt_hackers  = shmget(IPC_PRIVATE, sizeof(int),
      IPC_CREAT | IPC_EXCL | 0666)) == -1) { ecode = EISHM; }
    if((shm_ctl_cnt_serfs    = shmget(IPC_PRIVATE, sizeof(int),
      IPC_CREAT | IPC_EXCL | 0666)) == -1) { ecode = EISHM; }
    if((shm_ctl_bcnt_hackers = shmget(IPC_PRIVATE, sizeof(int),
      IPC_CREAT | IPC_EXCL | 0666)) == -1) { ecode = EISHM; }
    if((shm_ctl_bcnt_serfs   = shmget(IPC_PRIVATE, sizeof(int),
      IPC_CREAT | IPC_EXCL | 0666)) == -1) { ecode = EISHM; }
    if((shm_ctl_index_hacker = shmget(IPC_PRIVATE, sizeof(int),
      IPC_CREAT | IPC_EXCL | 0666)) == -1) { ecode = EISHM; }
    if((shm_ctl_index_serf   = shmget(IPC_PRIVATE, sizeof(int),
      IPC_CREAT | IPC_EXCL | 0666)) == -1) { ecode = EISHM; }
    if((shm_ctl_board_hacker = shmget(IPC_PRIVATE, sizeof(int),
      IPC_CREAT | IPC_EXCL | 0666)) == -1) { ecode = EISHM; }
    if((shm_ctl_board_serf   = shmget(IPC_PRIVATE, sizeof(int),
      IPC_CREAT | IPC_EXCL | 0666)) == -1) { ecode = EISHM; }
    if((shm_ctl_transported  = shmget(IPC_PRIVATE, sizeof(int),
      IPC_CREAT | IPC_EXCL | 0666)) == -1) { ecode = EISHM; }
    if((shm_ctl_cpt_type     = shmget(IPC_PRIVATE, sizeof(int),
      IPC_CREAT | IPC_EXCL | 0666)) == -1) { ecode = EISHM; }
    if((shm_ctl_landed       = shmget(IPC_PRIVATE, sizeof(int),
      IPC_CREAT | IPC_EXCL | 0666)) == -1) { ecode = EISHM; }
    if((shm_ctl_has_cpt      = shmget(IPC_PRIVATE, sizeof(int),
      IPC_CREAT | IPC_EXCL | 0666)) == -1) { ecode = EISHM; }
    if((shm_ctl_acnt_hackers = shmget(IPC_PRIVATE, sizeof(int),
      IPC_CREAT | IPC_EXCL | 0666)) == -1) { ecode = EISHM; }
    if((shm_ctl_acnt_serfs   = shmget(IPC_PRIVATE, sizeof(int),
      IPC_CREAT | IPC_EXCL | 0666)) == -1) { ecode = EISHM; }

    if((action_id    = (int *) shmat(shm_ctl_action_id,    NULL, 0)) == NULL)
      ecode = EISHM;
    if((cnt_hackers  = (int *) shmat(shm_ctl_cnt_hackers,  NULL, 0)) == NULL)
      ecode = EISHM;
    if((cnt_serfs    = (int *) shmat(shm_ctl_cnt_serfs,    NULL, 0)) == NULL)
      ecode = EISHM;
    if((bcnt_hackers = (int *) shmat(shm_ctl_bcnt_hackers, NULL, 0)) == NULL)
      ecode = EISHM;
    if((bcnt_serfs   = (int *) shmat(shm_ctl_bcnt_serfs,   NULL, 0)) == NULL)
      ecode = EISHM;
    if((index_hacker = (int *) shmat(shm_ctl_index_hacker, NULL, 0)) == NULL)
      ecode = EISHM;
    if((index_serf   = (int *) shmat(shm_ctl_index_serf,   NULL, 0)) == NULL)
      ecode = EISHM;
    if((board_hacker = (int *) shmat(shm_ctl_board_hacker, NULL, 0)) == NULL)
      ecode = EISHM;
    if((board_serf   = (int *) shmat(shm_ctl_board_serf,   NULL, 0)) == NULL)
      ecode = EISHM;
    if((transported  = (int *) shmat(shm_ctl_transported,  NULL, 0)) == NULL)
      ecode = EISHM;
    if((cpt_type     = (int *) shmat(shm_ctl_cpt_type,     NULL, 0)) == NULL)
      ecode = EISHM;
    if((landed       = (int *) shmat(shm_ctl_landed,       NULL, 0)) == NULL)
      ecode = EISHM;
    if((has_cpt      = (int *) shmat(shm_ctl_has_cpt,      NULL, 0)) == NULL)
      ecode = EISHM;
    if((acnt_hackers = (int *) shmat(shm_ctl_acnt_hackers, NULL, 0)) == NULL)
      ecode = EISHM;
    if((acnt_serfs   = (int *) shmat(shm_ctl_acnt_serfs,   NULL, 0)) == NULL)
      ecode = EISHM;
  }

  return ecode;
}

/**
 * Uvolnenie semaforov a zdielanej pamati.
 */
void rsrc_release()
{
  int ecode = EOK;

  fclose(output);

  // Uvolnenie semaforov
  if(sem_destroy(sem_actions)    == -1) { ecode = ERSEM; }
  if(sem_destroy(sem_hacker_id)  == -1) { ecode = ERSEM; }
  if(sem_destroy(sem_serf_id)    == -1) { ecode = ERSEM; }
  if(sem_destroy(sem_hboarding)  == -1) { ecode = ERSEM; }
  if(sem_destroy(sem_sboarding)  == -1) { ecode = ERSEM; }
  if(sem_destroy(sem_dock)       == -1) { ecode = ERSEM; }
  if(sem_destroy(sem_d_hackers)  == -1) { ecode = ERSEM; }
  if(sem_destroy(sem_d_serfs)    == -1) { ecode = ERSEM; }
  if(sem_destroy(sem_b_hackers)  == -1) { ecode = ERSEM; }
  if(sem_destroy(sem_b_serfs)    == -1) { ecode = ERSEM; }
  if(sem_destroy(sem_finish)     == -1) { ecode = ERSEM; }
  if(sem_destroy(sem_writer)     == -1) { ecode = ERSEM; }
  if(sem_destroy(sem_travel)     == -1) { ecode = ERSEM; }
  if(sem_destroy(sem_istrvl)     == -1) { ecode = ERSEM; }
  if(sem_destroy(sem_check1)     == -1) { ecode = ERSEM; }
  if(sem_destroy(sem_check2)     == -1) { ecode = ERSEM; }
  if(sem_destroy(sem_check3)     == -1) { ecode = ERSEM; }
  if(sem_destroy(sem_transp)     == -1) { ecode = ERSEM; }
  if(sem_destroy(sem_hcaptn)     == -1) { ecode = ERSEM; }
  if(sem_destroy(sem_scaptn)     == -1) { ecode = ERSEM; }
  if(sem_destroy(sem_all_aboard) == -1) { ecode = ERSEM; }

  if(ecode == ERSEM)
    print_ecode(ecode);

  // Uvolnenie zdielanej pamati
  if(shmctl(shm_ctl_action_id,    IPC_RMID, NULL) == -1) { ecode = ERSHM; }
  if(shmctl(shm_ctl_cnt_hackers,  IPC_RMID, NULL) == -1) { ecode = ERSHM; }
  if(shmctl(shm_ctl_cnt_serfs,    IPC_RMID, NULL) == -1) { ecode = ERSHM; }
  if(shmctl(shm_ctl_bcnt_hackers, IPC_RMID, NULL) == -1) { ecode = ERSHM; }
  if(shmctl(shm_ctl_bcnt_serfs,   IPC_RMID, NULL) == -1) { ecode = ERSHM; }
  if(shmctl(shm_ctl_index_hacker, IPC_RMID, NULL) == -1) { ecode = ERSHM; }
  if(shmctl(shm_ctl_index_serf,   IPC_RMID, NULL) == -1) { ecode = ERSHM; }
  if(shmctl(shm_ctl_board_hacker, IPC_RMID, NULL) == -1) { ecode = ERSHM; }
  if(shmctl(shm_ctl_board_serf,   IPC_RMID, NULL) == -1) { ecode = ERSHM; }
  if(shmctl(shm_ctl_transported,  IPC_RMID, NULL) == -1) { ecode = ERSHM; }
  if(shmctl(shm_ctl_cpt_type,     IPC_RMID, NULL) == -1) { ecode = ERSHM; }
  if(shmctl(shm_ctl_landed,       IPC_RMID, NULL) == -1) { ecode = ERSHM; }
  if(shmctl(shm_ctl_has_cpt,      IPC_RMID, NULL) == -1) { ecode = ERSHM; }
  if(shmctl(shm_ctl_acnt_hackers, IPC_RMID, NULL) == -1) { ecode = ERSHM; }
  if(shmctl(shm_ctl_acnt_serfs,   IPC_RMID, NULL) == -1) { ecode = ERSHM; }

  if(ecode == ERSHM)
    print_ecode(ecode);
}

/**
 * Uvolni zdroje a ukonci program s exit code 2.
 */
void quit_on_error()
{
  rsrc_release();
  exit(2);
}

/**
 * Zvysi hodnotu pocitadla v zdielanej pamati a vrati jeho hodnotu.
 * @param sn Ukazovatel na pocitadlo v zdielanej pamati
 * @param val Hodnota, o ktoru sa ma zvysit pocitadlo
 * @param sem Ukazovatel na semafor, ktory chrani zdielanu pamat
 * @return Vracia hodnotu pocitadla
 */
int shm_inc(int *sn, int val,sem_t *sem)
{
  sem_wait(sem);
  *sn = *sn + val;
  sem_post(sem);

  return *sn;
}

/**
 * Vrati hodnotu v zdielanej pamati.
 * @param sn Ukazovatel na pocitadlo v zdielanej pamati
 * @param sem Ukazovatel na semafor, ktory chrani zdielanu pamat
 * @return Vracia hodnotu pocitadla
 */
int shm_vget(int *sn, sem_t *sem)
{
  int val;
  sem_wait(sem);
  val = *sn;
  sem_post(sem);

  return val;
}

/**
 * Nastavi hodnotu pocitadla v zdielanej pamati.
 * @param sn Ukazovatel na pocitadlo v zdielanej pamati
 * @param val Hodnota, na ktoru sa ma nastavit pocitadlo
 * @param sem Ukazovatel na semafor, ktory chrani zdielanu pamat
 */
void shm_vset(int *sn, int val, sem_t *sem)
{
  sem_wait(sem);
  *sn = val;
  sem_post(sem);
}

/**
 * Proces hacker.
 * @param ttime Maximalna dlzka plavby v ms
 * @param count Pocet hackerov
 */
void hacker(int ttime, int count)
{
  bool is_captain = false;
  int h_id = shm_inc(index_hacker, 1, sem_hacker_id);

  sem_wait(sem_writer);
  fprintf(output, "%d: hacker: %d: started\n", shm_inc(action_id, 1,
    sem_actions), h_id);
  fflush(output);
  sem_post(sem_writer);

  // Prichod na molo
  sem_wait(sem_dock);
  int waiting_hackers = shm_inc(cnt_hackers, 1, sem_d_hackers);
  int waiting_serfs = *cnt_serfs;

  sem_wait(sem_writer);
  fprintf(output, "%d: hacker: %d: waiting for boarding: %d:%d\n",
    shm_inc(action_id, 1, sem_actions), h_id, waiting_hackers, waiting_serfs);
  fflush(output);
  sem_post(sem_writer);

  sem_post(sem_dock);

  // Skontrolovat pocet ludi na mole a pripadne povolit nalodenie
  while(true)
  {
    sem_wait(sem_check1);
    if(shm_vget(cnt_hackers, sem_d_hackers) >= 4 ||
      (((shm_vget(cnt_hackers, sem_d_hackers) == 2) ||
      (shm_vget(cnt_hackers, sem_d_hackers) == 3)) &&
      shm_vget(cnt_serfs, sem_d_serfs) >= 2))
    {
      if(shm_vget(has_cpt, sem_transp) == 0)
      {
        sem_wait(sem_dock);   //Uzamkni molo
        sem_wait(sem_istrvl); //Len kapitan moze zahajit/ukoncit plavbu

        // Nastavenie kapitana
        is_captain = true;
        shm_vset(cpt_type, HACKER, sem_b_hackers);
        shm_vset(has_cpt, 1, sem_transp);

        int v;
        sem_getvalue(sem_hcaptn, &v);
        if(v != 0)
        {
          //sem_post(sem_hcaptn);
          sem_wait(sem_hcaptn);
        }

        sem_wait(sem_all_aboard);

        shm_vset(bcnt_hackers, 0, sem_b_hackers);
        shm_vset(bcnt_serfs, 0, sem_b_serfs);

        if(shm_vget(cnt_hackers, sem_d_hackers) >= 4)
        {
          int value;
          sem_getvalue(sem_hboarding, &value);
          shm_vset(board_hacker, 4, sem_b_hackers);
          shm_vset(acnt_hackers, 4, sem_b_hackers);
          shm_inc(cnt_hackers, -4, sem_d_hackers);
          if(value == 0) { sem_post(sem_hboarding); }
        }
        else
        {
          int value1, value2;
          sem_getvalue(sem_hboarding, &value1);
          sem_getvalue(sem_sboarding, &value2);
          shm_vset(board_serf, 2, sem_b_serfs);
          shm_vset(board_hacker, 2, sem_b_hackers);
          shm_vset(acnt_hackers, 2, sem_b_hackers);
          shm_vset(acnt_serfs, 2, sem_b_serfs);
          shm_inc(cnt_serfs, -2, sem_d_serfs);
          shm_inc(cnt_hackers, -2, sem_d_hackers);
          if(value1 == 0) { sem_post(sem_hboarding); }
          if(value2 == 0) { sem_post(sem_sboarding); }
        }
        shm_inc(acnt_hackers, -1, sem_b_hackers);
        sem_post(sem_check1);
        break;
      }
      else // Podmienka splnena, ale kapitan je uz zvoleny
      {
        if(shm_vget(acnt_hackers, sem_b_hackers) > 0)
        {
          shm_inc(acnt_hackers, -1, sem_b_hackers);
          sem_post(sem_check1);
          break;
        }
        else
        {
          sem_post(sem_check1);
          continue;
        }
      }
    }
    else // Podmienka nesplnena, ale stale sa moze plavit
    {
      if(shm_vget(acnt_hackers, sem_b_hackers) > 0)
      {
        shm_inc(acnt_hackers, -1, sem_b_hackers);
        sem_post(sem_check1);
        break;
      }
      else
      {
        sem_post(sem_check1);
        continue;
      }
    }
  }

  // Kapitan musi byt nalodeny, inac nemozno zahajit plavbu
  // Len po nalodeni kapitana sa mozu nalodovat ostatni z jeho kategorie
  if(!is_captain && shm_vget(cpt_type, sem_b_hackers) == HACKER)
  {
    sem_wait(sem_hcaptn);
  }

  // Je este miesto pre hackera?
  sem_wait(sem_hboarding);
  if(shm_vget(board_hacker, sem_b_hackers) > 0)
  {
    shm_inc(bcnt_hackers, 1, sem_b_hackers);
    shm_inc(board_hacker, -1, sem_b_hackers);

    sem_wait(sem_writer);
    fprintf(output, "%d: hacker: %d: boarding: %d:%d\n", shm_inc(action_id, 1,
      sem_actions), h_id, shm_vget(cnt_hackers, sem_d_hackers),
      shm_vget(cnt_serfs, sem_d_serfs));
    fflush(output);
    sem_post(sem_writer);

    if(shm_vget(bcnt_hackers, sem_b_hackers) + shm_vget(bcnt_serfs,
      sem_b_serfs) == 4)
    {
      sem_post(sem_all_aboard);
    }
  }

  sem_post(sem_hcaptn);

  // Povolit dalsie nalodovanie hackerov?
  if(shm_vget(board_hacker, sem_b_hackers) > 0)
  {
    sem_post(sem_hboarding);
  }

  // Captain alebo member?
  sem_wait(sem_all_aboard);
  if(is_captain)
  {
    sem_wait(sem_writer);
    fprintf(output, "%d: hacker: %d: captain\n", shm_inc(action_id, 1,
      sem_actions), h_id);
    fflush(output);
    sem_post(sem_writer);
  }
  else
  {
    sem_wait(sem_writer);
    fprintf(output, "%d: hacker: %d: member\n", shm_inc(action_id, 1,
      sem_actions), h_id);
    fflush(output);
    sem_post(sem_writer);
  }
  sem_post(sem_all_aboard);

  // Zakazat dalsie nalodovanie
  sem_wait(sem_check2);
  if((shm_vget(bcnt_hackers, sem_b_hackers) == 4) ||
    ((shm_vget(bcnt_hackers, sem_b_hackers) == 2) &&
    (shm_vget(bcnt_serfs, sem_b_serfs) == 2)))
  {
    // Vynuluj pocitadla
    shm_vset(board_hacker, 0, sem_b_hackers);
    shm_vset(board_serf, 0, sem_b_serfs);
    shm_vset(bcnt_hackers, 0, sem_b_hackers);
    shm_vset(bcnt_serfs, 0, sem_b_serfs);

    if(shm_vget(bcnt_hackers, sem_b_hackers) == 4)
    {
      int value;
      sem_getvalue(sem_hboarding, &value);
      if(value > 0)
      {
        sem_wait(sem_hboarding);
      }
    }
    else
    {
      int value1, value2; 
      sem_getvalue(sem_hboarding, &value1);
      sem_getvalue(sem_sboarding, &value2);
      if(value1 > 0) { sem_wait(sem_hboarding); }
      if(value2 > 0) { sem_wait(sem_sboarding); }
    }
    sem_post(sem_travel);
  }
  sem_post(sem_check2);

  if(is_captain)
  {
    sem_wait(sem_travel);
    
    // Simulacia plavby
    int range = (ttime == 0) ? 0 : (rand() % (ttime + 1));
    usleep(range * MS_TO_S);
    sem_post(sem_istrvl);
  }
  else
  {
    sem_wait(sem_istrvl);
    sem_post(sem_istrvl);
  }
  sem_wait(sem_writer);
  fprintf(output, "%d: hacker: %d: landing: %d:%d\n",
    shm_inc(action_id, 1, sem_actions), h_id, shm_vget(cnt_hackers,
    sem_d_hackers), shm_vget(cnt_serfs, sem_d_serfs));
  fflush(output);
  sem_post(sem_writer);

  // Zvys pocet prepravenych
  shm_inc(transported, 1, sem_transp);
  shm_inc(landed, 1, sem_transp);


  if(shm_vget(landed, sem_transp) == 4)
  {
    // Otvor molo
    sem_post(sem_dock);
    shm_vset(landed, 0, sem_transp);
    shm_vset(has_cpt, 0, sem_transp);
  }
  
  sem_wait(sem_check3);
  if(shm_vget(transported, sem_transp) == 2*count)
  {
    sem_post(sem_finish);
  }
  sem_post(sem_check3);

  sem_wait(sem_finish);
  fprintf(output, "%d: hacker: %d: finished\n", shm_inc(action_id,
    1, sem_actions), h_id);
  fflush(output);
  sem_post(sem_finish);

  exit(0);
}

/**
 * Proces serf.
 * @param ttime Maximalna dlzka plavby v ms
 * @param count Pocet serfov
 */
void serf(int ttime, int count)
{
  bool is_captain = false;
  int waiting_serfs, waiting_hackers;
  int s_id = shm_inc(index_serf, 1 ,sem_serf_id);

  sem_wait(sem_writer);
  fprintf(output, "%d: serf: %d: started\n", shm_inc(action_id, 1, sem_actions),
    s_id);
  fflush(output);
  sem_post(sem_writer);

  // Prichod na molo
  sem_wait(sem_dock);
  waiting_serfs = shm_inc(cnt_serfs, 1, sem_d_serfs);
  waiting_hackers = *cnt_hackers;

  sem_wait(sem_writer);
  fprintf(output, "%d: serf: %d: waiting for boarding: %d:%d\n",
    shm_inc(action_id, 1, sem_actions), s_id, waiting_hackers, waiting_serfs);
  fflush(output);
  sem_post(sem_writer);
  sem_post(sem_dock);

  // Skontrolovat pocet ludi na mole a pripadne povolit nalodenie
  while(true)
  {
    //fprintf(output, "SERF %d ENTERED WHILE\n", s_id);
    sem_wait(sem_check1);
    if((shm_vget(cnt_serfs, sem_d_serfs) >= 4) ||
      (((shm_vget(cnt_serfs, sem_d_serfs) == 2) ||
      (shm_vget(cnt_serfs, sem_d_serfs) == 3)) &&
      shm_vget(cnt_hackers, sem_d_hackers) >= 2))
    {
      if(shm_vget(has_cpt, sem_transp) == 0)
      {
        sem_wait(sem_dock);   //Uzamkni molo
        sem_wait(sem_istrvl); //Len kapitan moze zahajit/ukoncit plavbu

        // Nastavenie kapitana
        is_captain = true;
        shm_vset(cpt_type, SERF, sem_b_serfs);
        shm_vset(has_cpt, 1, sem_transp);

        int v;
        sem_getvalue(sem_scaptn, &v);
        if(v != 0)
        {
          //sem_post(sem_scaptn);
          sem_wait(sem_scaptn);
        }

        sem_wait(sem_all_aboard);

        shm_vset(bcnt_hackers, 0, sem_b_hackers);
        shm_vset(bcnt_serfs, 0, sem_b_serfs);

        if(shm_vget(cnt_serfs, sem_d_serfs) >= 4)
        {
          int value;
          sem_getvalue(sem_sboarding, &value);
          shm_vset(board_serf, 4, sem_b_serfs);
          shm_vset(acnt_serfs, 4, sem_b_serfs);
          shm_inc(cnt_serfs, -4, sem_d_serfs);
          if(value == 0) { sem_post(sem_sboarding); }
        }
        else
        {
          int value1, value2;
          sem_getvalue(sem_hboarding, &value1);
          sem_getvalue(sem_sboarding, &value2);
          shm_vset(board_serf, 2, sem_b_serfs);
          shm_vset(board_hacker, 2, sem_b_hackers);
          shm_vset(acnt_serfs, 2, sem_b_serfs);
          shm_vset(acnt_hackers, 2, sem_b_hackers);
          shm_inc(cnt_serfs, -2, sem_d_serfs);
          shm_inc(cnt_hackers, -2, sem_d_hackers);
          if(value2 == 0) { sem_post(sem_sboarding); }
          if(value1 == 0) { sem_post(sem_hboarding); }
        }
        shm_inc(acnt_serfs, -1, sem_b_serfs);
        sem_post(sem_check1);
        break;
      }
      else // Podmienka splnena, kapitan uz je zvoleny
      {
        if(shm_vget(acnt_serfs, sem_b_serfs) > 0)
        {
          shm_inc(acnt_serfs, -1, sem_b_serfs);
          sem_post(sem_check1);
          break;
        }
        else
        {
          sem_post(sem_check1);
          continue;
        }
      }
    }
    else // Podmienka nesplnena, ale stale sa moze plavit
    {
      if(shm_vget(acnt_serfs, sem_b_serfs) > 0)
      {
        shm_inc(acnt_serfs, -1, sem_b_serfs);
        sem_post(sem_check1);
        break;
      }
      else
      {
        sem_post(sem_check1);
        continue;
      }
    }
  }
  
  // Kapitan musi byt nalodeny, inac nemozno zahajit plavbu
  // Len po nalodeni kapitana sa mozu nalodovat ostatni z jeho kategorie
  if(!is_captain && shm_vget(cpt_type, sem_b_serfs) == SERF)
  {
    sem_wait(sem_scaptn);
  }
  // Je este miesto pre serfa?
  sem_wait(sem_sboarding);
  if(shm_vget(board_serf, sem_b_serfs) > 0)
  {
    shm_inc(bcnt_serfs, 1, sem_b_serfs);
    shm_inc(board_serf, -1, sem_b_serfs);

    sem_wait(sem_writer);
    fprintf(output, "%d: serf: %d: boarding: %d:%d\n", shm_inc(action_id, 1,
      sem_actions), s_id, shm_vget(cnt_hackers, sem_d_hackers),
      shm_vget(cnt_serfs, sem_d_serfs));
    fflush(output);
    sem_post(sem_writer);

    if(shm_vget(bcnt_hackers, sem_b_hackers) + shm_vget(bcnt_serfs, sem_b_serfs) == 4)
    {
      sem_post(sem_all_aboard);
    }
  }

  sem_post(sem_scaptn);
  
  // Povolit dalsie nalodovanie serfov?
  if((shm_vget(board_serf, sem_b_serfs) > 0))
  {
    sem_post(sem_sboarding);
  }

  // Captain alebo member?
  sem_wait(sem_all_aboard);
  if(is_captain)
  {
    sem_wait(sem_writer);
    fprintf(output, "%d: serf: %d: captain\n", shm_inc(action_id, 1,
      sem_actions), s_id);
    fflush(output);
    sem_post(sem_writer);
  }
  else
  {
    sem_wait(sem_writer);
    fprintf(output, "%d: serf: %d: member\n", shm_inc(action_id, 1,
      sem_actions), s_id);
    fflush(output);
    sem_post(sem_writer);
  }
  sem_post(sem_all_aboard);

  // Zakazat dalsie nalodovanie
  sem_wait(sem_check2);
  if((shm_vget(bcnt_serfs, sem_b_serfs) == 4) ||
      ((shm_vget(bcnt_serfs, sem_b_serfs) == 2) &&
      (shm_vget(bcnt_hackers, sem_b_hackers) == 2)))
  {
    // Vynuluj pocitadla
    shm_vset(board_hacker, 0, sem_b_hackers);
    shm_vset(board_serf, 0, sem_b_serfs);
    shm_vset(bcnt_hackers, 0, sem_b_hackers);
    shm_vset(bcnt_serfs, 0, sem_b_serfs);
    shm_vset(acnt_hackers, 0, sem_b_hackers);
    shm_vset(acnt_serfs, 0, sem_b_serfs);
    
    if(shm_vget(bcnt_serfs, sem_b_serfs) == 4)
    {
      int value;
      sem_getvalue(sem_sboarding, &value);
      if(value > 0)
      {
        sem_wait(sem_sboarding);
      }
      
    }
    else
    {
      int value1, value2; 
      sem_getvalue(sem_hboarding, &value1);
      sem_getvalue(sem_sboarding, &value2);
      if(value1 > 0) { sem_wait(sem_hboarding); }
      if(value2 > 0) { sem_wait(sem_sboarding); }
    }

    sem_post(sem_travel);
  }
  sem_post(sem_check2);

  if(is_captain)
  {
    sem_wait(sem_travel);

    // Simulacia plavby
    int range = (ttime == 0) ? 0 : (rand() % (ttime + 1));
    usleep(range * MS_TO_S);
    sem_post(sem_istrvl);
  }
  else
  {
    sem_wait(sem_istrvl);
    sem_post(sem_istrvl);
  }
  sem_wait(sem_writer);
  fprintf(output, "%d: serf: %d: landing: %d:%d\n", shm_inc(action_id, 1,
    sem_actions), s_id, shm_vget(cnt_hackers, sem_d_hackers),
    shm_vget(cnt_serfs, sem_d_serfs));
  fflush(output);
  sem_post(sem_writer);

  // Zvys pocet prepravenych
  shm_inc(transported, 1, sem_transp);
  shm_inc(landed, 1, sem_transp);

  if(shm_vget(landed, sem_transp) == 4)
  {
    shm_vset(landed, 0, sem_transp);
    shm_vset(has_cpt, 0, sem_transp);

    // Otvor molo
    sem_post(sem_dock);
  }

  sem_wait(sem_check3);
  if(shm_vget(transported, sem_transp) == 2*count)
  {
    sem_post(sem_finish);
  }
  sem_post(sem_check3);

  sem_wait(sem_finish);
  fprintf(output, "%d: serf: %d: finished\n", shm_inc(action_id, 1,
    sem_actions), s_id);
  fflush(output);
  sem_post(sem_finish);

  exit(0);
}

/* main.c */
