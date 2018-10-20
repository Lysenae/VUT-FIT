namespace StreamPlayer
{
  class Library: AbstractView
  {
    /* signaly */

    /**
     * Signal, ktory posiela aplikacii celu cestu poloziek
     */
    public signal void file_path(string path);
    public signal void content_changed();
    
    //private Gtk.ListStore dummy_store;
    private LibDb db;
    public Gtk.TreeView genre;
    public Gtk.TreeView artist;
    public Gtk.TreeView album;
    public Gtk.TreeView tracks;
   
    protected Gtk.ListStore gestore;
    protected Gtk.ListStore arstore;
    protected Gtk.ListStore alstore;
    protected Gtk.ListStore trstore;


    /* konstruktor */
    public Library()
    {
      this.gestore = new Gtk.ListStore(1,typeof(string));
      this.arstore = new Gtk.ListStore(2,typeof(string),typeof(string));
      this.alstore = new Gtk.ListStore(3,typeof(string),typeof(string),typeof(string));
      this.trstore = new Gtk.ListStore(8,typeof(int),typeof(string),typeof(string),typeof(string),
          typeof(string),typeof(string),typeof(string),typeof(string));

      this.genre = new Gtk.TreeView.with_model(gestore);
      this.artist = new Gtk.TreeView.with_model(arstore);
      this.album = new Gtk.TreeView.with_model(alstore);
      this.tracks = new Gtk.TreeView.with_model(trstore);

      //Gtk.TreeViewColumn colgenre = new Gtk.TreeViewColumn();
      this.genre.insert_column_with_attributes(-1, "Žáner", new Gtk.CellRendererText(),"text",0);
      //Gtk.TreeViewColumn colartist = new Gtk.TreeViewColumn();
      this.artist.insert_column_with_attributes(-1, "Umelec", new Gtk.CellRendererText(),"text",0);
      //Gtk.TreeViewColumn colalbum = new Gtk.TreeViewColumn();
      this.album.insert_column_with_attributes(-1, "Album", new Gtk.CellRendererText(),"text",0);
      //Gtk.TreeViewColumn coltracks = new Gtk.TreeViewColumn();
      this.tracks.insert_column_with_attributes(0, "#", new Gtk.CellRendererText(),"text",0);
      this.tracks.insert_column_with_attributes(1, "Skladba", new Gtk.CellRendererText(),"text",1);
      this.tracks.insert_column_with_attributes(2, "Umelec", new Gtk.CellRendererText(),"text",2);
      this.tracks.insert_column_with_attributes(3, "Žáner", new Gtk.CellRendererText(),"text",3);
      this.tracks.insert_column_with_attributes(4, "Album", new Gtk.CellRendererText(),"text",4);
      this.tracks.insert_column_with_attributes(5, "Rok", new Gtk.CellRendererText(),"text",5);
      this.tracks.insert_column_with_attributes(6, "Poradie", new Gtk.CellRendererText(),"text",6);
      this.tracks.insert_column_with_attributes(7, "Dĺžka", new Gtk.CellRendererText(),"text",7);

      db = new LibDb();
      db.init();
      //db.fill();
/*
      Gtk.TreeViewColumn col1 = this.tracks.get_column(1);
      col1.set_resizable(true);
      col1.set_min_width(250);
      Gtk.TreeViewColumn col2 = this.tracks.get_column(2);
      col2.set_resizable(true);
      col2.set_min_width(150);
*/
      this.tracks.set_headers_clickable(true);
    }

    public void fill()
    {
      Sqlite.Statement gerecs, arrecs, alrecs, trrecs;

      string[,] filter;
      string[] buffer1,buffer2,buffer3,buffer4;
      int count;

      filter = {{"",""}};

      count = db.distinct_column(filter, out gerecs, "genre", "");
      for (int i = 0; i< count; i++)
      {
        db.get_rows(count, ref gerecs, out buffer1);
        string genre = buffer1[i];
        add_genre(genre);
      }

      count = db.distinct_column(filter, out arrecs, "artist", ", genre");
      for (int i = 0; i< 2*count; i++)
      {
        db.get_rows(count, ref arrecs, out buffer2);
        string artist = buffer2[i++];
        string genre = buffer2[i];
        add_artist(artist, genre);
      }

      count = db.distinct_column(filter, out alrecs, "album", ", artist, genre");
      for (int i = 0; i< 3*count; i++)
      {
        db.get_rows(count, ref alrecs, out buffer3);
        string album = buffer3[i++];
        string artist = buffer3[i++];
        string genre = buffer3[i];

        add_album(album, artist, genre);
      }

      count = db.tracks(filter, out trrecs);
      for (int i = 0; i< 8*count; i++)
      {
        db.get_rows(count, ref trrecs, out buffer4);
        int id = int.parse(buffer4[i]);
        string title = buffer4[++i];
        string artist = buffer4[++i];
        string genre = buffer4[++i];
        string year = buffer4[++i];
        string album = buffer4[++i];
        string track_nr = buffer4[++i];
        //string path = buffer4[++i];
        ++i; // miesto path, aby to tam nervalo warning
        string duration = buffer4[++i];

        add_track(id, title, artist, genre, year, album, track_nr, duration);
      }
    }

    private void refill()
    {
      this.trstore.clear();
      this.gestore.clear();
      this.alstore.clear();
      this.arstore.clear();

      fill();
    }

    public void add_genre(string genre)
    {
      Gtk.TreeIter iter;
      this.gestore.append(out iter);
      this.gestore.set(iter, 0, genre, -1);
      content_changed();
    }

    public void add_artist(string artist, string genre)
    {
      Gtk.TreeIter iter;
      this.arstore.append(out iter);
      this.arstore.set(iter, 0, artist, 1, genre, -1);
      content_changed();
    }

    public void add_album(string album, string artist, string genre)
    {
      Gtk.TreeIter iter;
      this.alstore.append(out iter);
      this.alstore.set(iter, 0, album, 1, artist, 2, genre, -1);
      content_changed();
    }

    public void add_track(int id, string title, string artist, string genre,
    string year, string album, string track_nr, string duration)
    {
      Gtk.TreeIter iter;
      this.trstore.append(out iter);
      this.trstore.set(iter, 0, id, 1, title, 2, artist, 3, genre, 4, album,
        5, year, 6, track_nr, 7, duration, -1);
      content_changed();
    }

    /*public bool filter_tree(Gtk.TreeModel model, Gtk.TreeIter iter)
    {
      string val; 
      model.get(iter, 2, out val);
      print("val = %s", val);
      return true;
    }*/

    /*public void on_changed(Gtk.TreeSelection selection)
    {
      Gtk.TreeModel model;
      Gtk.TreeIter iter;
      if (selection.get_selected (out model, out iter)) {
        filter_tree(model,iter);
      }
    }*/

    public bool add_media(string[] data)
    {
      if(db.insert(data))
      {
        refill();
        return true;
      }

      return false;
    }
  }
}
