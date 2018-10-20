using Sqlite;

namespace StreamPlayer
{
  class LibDb
  {
    private Database dbfile;      // descriptor databaze
    protected string filename = "tracks.db";
    protected string errmsg = "";   // buffer pro chyby z funkce dbfile.exec
    protected string tablename = "track_info";  // jmeno tabulky databaze

    // title, artist, genre, year, album, track_nr, path
    /*public void fill()
    {
      for (int i = 65; i < 75; i++)
      {
        string[] metadata;
        metadata = {i.to_string(), (100+i).to_string(), (200+i).to_string(),
          (300+i).to_string(), (400+i).to_string(),
          (500+i).to_string(), (600+i).to_string(), (700+i).to_string()};

        //insert(metadata);
        if(insert(metadata))
          stdout.printf("%s, %s, %s, %s, %s, %s, %s %s\n", metadata[0], metadata[1],
            metadata[2], metadata[3], metadata[4], metadata[5], metadata[6], metadata[7]);
      }
    }*/

    /**
     * Otevreni souboru s SQLite databazi.
     *
     * @param filename string se jmenem souboru
     *
     * @return true podarilo otevrit soubor databaze
     */
    protected bool open(string filename)
    {
      if(Database.open_v2(filename, out this.dbfile, OPEN_READWRITE | OPEN_CREATE) != OK)
        return false;
      return true;
    }

    /**
     * Zjistuje zda tabulka s jmenem z 'tablename' jiz v databazi existuje nebo ne.
     *
     * @param tablename string se jmeno hledane tabulky
     *
     * @return true pokud tabulka jiz existuje
     */
    /*
    protected bool check_if_exists(string tablename)
    {
      Statement records;
      string tcheck = """
      SELECT name FROM sqlite_master
      WHERE type = `table` AND name LIKE `%s`;
  """.printf (tablename);
      if (this.dbfile.prepare_v2(tcheck,tcheck.length, out records) != OK)
  if (records.step() == ROW)
  {
    stdout.printf("table: %s\n", records.column_text(0));
    return true;
  }
      return false;
    }
*/
    /**
     * Vytvoreni tabulky pro uchovani metadat o skladbe.
     * Pomocna funkce pro init() a trunctate()
     *
     * @return true pokud se poradarilo vytvorit tabulku
     */
    protected bool create()
    {
      string createq = """
      CREATE TABLE IF NOT EXISTS %s(
          id    INTEGER PRIMARY KEY AUTOINCREMENT,
          title TEXT    NOT NULL,
          artist  TEXT    NOT NULL,
          genre TEXT    NOT NULL,
          year  TEXT,
          album TEXT,
          track_nr  TEXT,
          path  TEXT  NOT NULL,
          duration TEXT NOT NULL
      );
      """.printf (this.tablename);

      if (this.dbfile.exec(createq,null,out this.errmsg) != OK)
      {
        print("DB Error: Create: %s\n", this.errmsg);
        return false;
      }
      return true;
    }

    /**
     * Odstraneni tabulku se skladbami.
     * Pomocna funkce pro trunctate().
     *
     * @return true pokud se povedlo smazat tabulku
     */
    /*protected bool drop()
    {
      string dropq = """
        DROP TABLE %s;
      """.printf(this.tablename);

      if (this.dbfile.exec(dropq,null,out this.errmsg) != OK)
      {
        print("DB Error: Drop: %s\n", this.errmsg);
        return false;
      }
      return true;

    }*/

    /**
     * Vytvari string clausule WHERE na zaklade filteru
     *
     * @param filter pole poli pro filtrovani [][0] = nazev sloupce, [][1] = hodnota
     *
     * @return string vraci vytvoreny retezec 
     */
    protected string make_where(string[,] filter)
    {
      string whereq = "WHERE ";
      int i = 0;

      for(int j=0; j < filter.length[0]; j++)
      {
        if (i > 0)
          whereq += " AND ";

        whereq += filter[j,0];
        whereq += " = `";
        whereq += filter[j,1];
        whereq += "`";
        i++;
      }

      if (i == 0)
        whereq = "";

      return whereq;
    }

    /**
     * Inicializuje databazi (otevre spojeni a vytvori tabulku pokud neexistuje)
     *
     * @return true pokud se povedlo inicilizovat databazi
     */
    public bool init()
    {
      if (!this.open(this.filename))
  return false;

      //if (!this.check_if_exists(this.tablename))
      return this.create();
      //return true;
    }

    /**
     * Vlozeni podstatnych metadat skladby do tabulky
     *
     * @param metadata pole stringu s metadaty o skladbe [0] => 'title', [1] => 'artist',
     * [2] => 'genre', [3] => 'year', [4] => 'album', [5] => 'track_nr', [6] => 'path'
     *
     * @return true pokud se povedlo vlozit skladbu do tabulky
     */
    public bool insert(string[] metadata)
    {
      if(metadata.length != 8)
      {
        print("Invalid metadata size\n");
        return false;
      }

      string insertq = """
        INSERT INTO %s(title, artist, genre, year, album, track_nr, path, duration)
          VALUES ('%s', '%s', '%s', '%s', '%s', '%s', '%s', '%s');
          """.printf(this.tablename, metadata[0].replace("'", ""),
          metadata[1].replace("'", ""), metadata[2].replace("'", ""),
          metadata[3].replace("'", ""), metadata[4].replace("'", ""),
          metadata[5].replace("'", ""), metadata[6].replace("'", ""),
          metadata[7].replace("'", ""));

      if (this.dbfile.exec(insertq,null,out this.errmsg) != OK)
      {
        print("DB Error: Insert: %s (\"%s\")\n", this.errmsg, insertq);
        return false;
      }
      return true;
    }

    /**
     * Provedeni operace select na tabulkou databaze s pouzitim filtru skladeb
     *
     * @param filter pole string poli viz. make_where
     * @param records Statement objekt obsahujici nactena data
     *
     * @return true kdyz podari vybrat polozky z databaze
     */
    public int tracks(string[,] filter, out Statement records)
    {
      string whereq; 
      int count = -1;
      string selectq;
      Statement stcount;

      if (filter[0,0] != "")
        whereq = this.make_where(filter);
      else
        whereq = "";

      selectq = """
        SELECT COUNT(id) FROM %s %s;
      """.printf(this.tablename, whereq);

      if (this.dbfile.prepare_v2(selectq, selectq.length, out stcount) == OK)
      {
        stcount.step();
        count = stcount.column_int(0);
      }
      
      selectq = """
        SELECT * FROM %s %s;
      """.printf(this.tablename, whereq);

      if (this.dbfile.prepare_v2(selectq, selectq.length, out records) != OK)
        return -2;
      return count;
    }

    /**
     * Vybere unikatni hodnoty ve vybranem sloupci
     *
     * @param filter pole string poli viz. make_where
     * @param records Statement objekt obsahujici nactena data
     * @param collumn string se jmenem sloupce se kterym chceme pracovat
     *
     * @return true kdyz podari vybrat polozky z databaze
     */
    public int distinct_column(string[,] filter, out Statement records,
    string column, string extra)
    {
      string whereq;
      int count = -1;
      Statement stcount;
      if (filter[0,0] != "")
        whereq = this.make_where(filter);
      else
        whereq = "";
      
      string selectq = """
        SELECT COUNT(DISTINCT %s) FROM %s %s;
      """.printf(column, this.tablename, whereq);
      if (this.dbfile.prepare_v2(selectq, selectq.length, out stcount) == OK)
      {
        stcount.step();
        count = stcount.column_int(0);
      }

      selectq = """
        SELECT DISTINCT %s %s FROM %s %s;
      """.printf(column, extra, this.tablename, whereq);

      if (this.dbfile.prepare_v2(selectq, selectq.length, out records) != OK)
        return -2;

      return count;
    }

    /**
     * Ziskani jedne skladby z databaze v programem zpracovatelne forme
     *
     * @param records objekt Statement obsahujici vystup operace 'SELECT'
     * @param track pole stringu s rozparsovanymi hodnotami zaznamu databaze
     *
     * @return true kdyz byla nalezen zaznam v records
     */
    public void get_rows(int rows, ref Statement records, out string[] record)
    {
      int cols = records.column_count();
      string[] rec = {};
      while (records.step() == Sqlite.ROW)
      {
        for (int i=0; i < cols; i++)
          rec += records.column_text(i);
      }
      record = rec;
    }

    /**
     * Vymaze vyprazdni tabulku
     *
     * @return true kdyz se podari vyprazdnit tabulku, jinak false
     */
    /*public bool trunctate()
    {
      if (!this.drop())
        return false;
      if (!this.create())
        return false;
      return true;
    }*/

    /**
     * Zpristupnuje chybu pri praci s databazi. Typ chyby zalezi na provadene funkci.
     * Create, insert, drop nastavuji 'errmsg'. Open, select nastavuji 'dberr'.
     *
     * @param type string s vyber typu zpravu, ktery chceme vypsat 'errmsg' / 'dberr'
     *
     * @return string s chybou pro danou funkci
     */
    /*public string get_errormsg(string type)
    {
      if (type == "errmsg")
        return this.errmsg;
      else if (type == "dberr")
        return this.dbfile.errmsg();
      return "";
    }*/
  }
}
