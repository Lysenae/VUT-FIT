namespace StreamPlayer
{
  class TrackInfo: Gtk.ToolItem
  {
    private Gtk.Table layout;
    private Gtk.Label lb_title;
    private Gtk.Label lb_artist;
    private Gtk.Label lb_genre;
    private Gtk.Label lb_year;
    private Gtk.Label lb_album;
    private Gtk.Label lb_track_nr;

    public TrackInfo()
    {
      layout = new Gtk.Table(3, 2, false);
      lb_title = new Gtk.Label("");
      lb_artist = new Gtk.Label("");
      lb_genre = new Gtk.Label("");
      lb_year = new Gtk.Label("");
      lb_album = new Gtk.Label("");
      lb_track_nr = new Gtk.Label("");
      
      lb_title.set_alignment(0, 0.5f);
      lb_album.set_alignment(0, 0.5f);
      lb_artist.set_alignment(0, 0.5f);
      lb_genre.set_alignment(0, 0.5f);
      lb_year.set_alignment(0, 0.5f);
      lb_track_nr.set_alignment(0, 0.5f);
      
      layout.attach(lb_title, 0, 1, 0, 1, Gtk.AttachOptions.FILL, Gtk.AttachOptions.EXPAND, 3, 1);
      layout.attach(lb_album, 0, 1, 1, 2,  Gtk.AttachOptions.FILL, Gtk.AttachOptions.EXPAND, 3, 1);
      layout.attach(lb_artist, 0, 1, 2, 3, Gtk.AttachOptions.FILL, Gtk.AttachOptions.EXPAND, 3, 1);
      layout.attach(lb_genre, 1, 2, 0, 1, Gtk.AttachOptions.FILL, Gtk.AttachOptions.EXPAND, 10, 1);
      layout.attach(lb_year, 1, 2, 1, 2, Gtk.AttachOptions.FILL, Gtk.AttachOptions.EXPAND, 10, 1);
      layout.attach(lb_track_nr, 1, 2, 2, 3, Gtk.AttachOptions.FILL, Gtk.AttachOptions.EXPAND, 10, 1);
      
      this.add(layout);
    }

    private void set_title(string title)
    {
      string conc_title = "";
      
      if(title.length > 30)
      {
        conc_title = Utils.shorten_string(title, 30);
        title = conc_title;
      } 

      string markup_string = """<span font="9" font_family="sans" weight="bold">""" + title + """</span>"""; 
      lb_title.set_markup(markup_string);
    }
    
    private void set_album(string album)
    {
      string conc_album = "";
      
      if(album.length > 30)
      {
        conc_album = Utils.shorten_string(album, 30);
        album = conc_album;
      }
      
      string markup_string = """<span font="9">z </span><span font="9" font_family="sans"  weight="bold">""" + album + """</span>"""; 
      lb_album.set_markup(markup_string);
    }
    
    private void set_artist(string artist)
    {
      string markup_string = """<span font="9">od </span><span font="9" font_family="sans" weight="bold">""" + artist + """</span>""";
      lb_artist.set_markup(markup_string);
    }
    
    private void set_genre(string genre)
    {
      string markup_string = """<span font="9" font_family="sans" weight="bold">Žáner: </span><span font="9">""" + genre + """</span>"""; 
      lb_genre.set_markup(markup_string);
    }
    
    private void set_year(string year)
    {
      string markup_string = """<span font="9" font_family="sans" weight="bold">Rok: </span><span font="9">""" + year + """</span>"""; 
      lb_year.set_markup(markup_string);
    }
    
    private void set_track_number(string number)
    {
      string markup_string = """<span font="9" font_family="sans" weight="bold">Číslo skladby: </span><span font="9">""" + number + """</span>"""; 
      lb_track_nr.set_markup(markup_string);
    }
    
    public void set_info(TagStore tags)
    {
      set_title(tags.title);
      set_album(tags.album);
      set_artist(tags.artist);
      set_genre(tags.genre);
      set_year(tags.year.to_string());
      set_track_number(tags.track_number.to_string());
    }
  }
}
