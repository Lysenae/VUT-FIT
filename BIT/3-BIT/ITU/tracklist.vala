namespace StreamPlayer
{
  class Tracklist: AbstractView
  {
    /* signaly */
    
    /**
     * Signal, ktory posiela aplikacii celu cestu poloziek
     */
    public signal void file_path(string path);
    public signal void content_changed();
    
    private Gtk.ListStore dummy_store;

    /* konstruktor */
    public Tracklist(ref Gtk.TreeIter track, ref Gtk.TreeIter selection)
    {
      can_delete = false;
      
      store = new Gtk.ListStore(4, typeof(string), typeof(string),
        typeof(string), typeof(string));
      dummy_store = new Gtk.ListStore(4, typeof(string),
        typeof(string), typeof(string), typeof(string));

      Gtk.TreeIter dummy_iter;
      this.dummy_store.append(out dummy_iter);
      this.dummy_store.set(dummy_iter, 0, " ", 1, " ", 2, " ", 3, " ", -1);
      
      this.get_selection().mode = Gtk.SelectionMode.MULTIPLE;
      this.set_model(dummy_store);

      // index je v modeli v 4. stlpci
      this.insert_column_with_attributes(
        -1,"#", new Gtk.CellRendererText(), "text", 3);

       // nazov je v modeli v 1. stl.
      this.insert_column_with_attributes(
        -1, "Skladba", new Gtk.CellRendererText(), "text", 0);

      // dlzka je v modeli v 3. stlpci
      this.insert_column_with_attributes(
        -1, "Dĺžka", new Gtk.CellRendererText(), "text", 2);

      // vyberie prvy riadok ak existuje
      select_first_row(ref track, ref selection);
      
      Gtk.TreeViewColumn col1 = this.get_column(0);
      col1.set_resizable(true);
      Gtk.TreeViewColumn col2 = this.get_column(1);
      col2.set_resizable(true);
            
      this.set_headers_clickable(true);
    }
  
    /**
     * Pridat polozku do tracklistu
     *
     * @param path cesta k polozke
     * @param name nazov polozky
     */
    public void add_item(string name, string duration, string path, string index)
    {
      Gtk.TreeIter iter;
      this.store.append(out iter);
      this.store.set(iter, 0, name, 1, path, 2, duration, 3, index, -1);
      content_changed();
    }
    
    /**
     * Prehrat aktualnu skladbu
     *
     * @param iter odkaz na iterator aktualnej skladby
     * @param streamer odkaz na streamer (prehravac)
     */
    public void play_current(ref Gtk.TreeIter iter, ref Streamer streamer)
    {
      string current_path;
      get_current(ref iter, out current_path);
      if(!(current_path == null))
      {
        file_path(current_path);
      }
      streamer.play(current_path);
    }
    
    /**
     * Prehrat nasledujucu skladbu
     *
     * Prehrat nasledujucu skladbu, ak existuje. Existencia nasledujucej
     * skladby je overena vo funkcii. 
     * Ak existuje, tak je zvolena pomocou funkcie @see select_next().
     *
     * @param iter_track iterator aktualne prehravanaej skladby
     * @param iter_selection iterator aktualne zvolenej skladby (nemusi byt prehravana)
     * @param streamer zvoleny prehravac
     */
    public void play_next(ref Gtk.TreeIter iter_track,
    ref Gtk.TreeIter iter_selection, ref Streamer streamer)
    {
      // kontrola ci su v modeli nejake polozky
      if(is_empty())
      {
        return;
      }
      
      // kontrola ci su iteratory inicializovane
      if(!iter_initialized(iter_selection))
      {
        return;
      }
      
      if(!iter_initialized(iter_track))
      {
        return;
      }
      
      // kontrola ci existuje v modeli nasledujuca skladba
      if(!has_next(iter_selection))
      {
        return;
      }

      // ak streamer prehrava skladbu, tak sa musi zastavit,
      // az potom sa presunie na dalsiu
      if(has_next(iter_track))
      {
        if(streamer.is_playing())
        {
          streamer.set_state_stop();
        }

        //oznacit nasledujucu skladbu za aktualnu
        select_next(ref iter_track, ref iter_selection);
        play_current(ref iter_track, ref streamer);
      }  
      else
      {
        return;
      }
    }
    
    /**
     * Ziskat data o zvolenej skladbe, preposiela adresu polozky
     *
     * @param iter iterator aktualnej skladby
     * @param cur_path retazec s adresou vybranej skladby, mal by by null
     */
    private void get_current(ref Gtk.TreeIter iter,  out string cur_path)
    {
      Gtk.TreeModel model;
      Gtk.TreeSelection selection = this.get_selection();

      // ak v modeli nie je ziadna skladba, tak ukonci volanei funkcie
      if(selection.count_selected_rows() > 0)
      {
        GLib.List<Gtk.TreePath> paths = selection.get_selected_rows(out model);
        Gtk.TreePath first = paths.nth_data(0);
        model.get_iter(out iter, first);
        model.get(iter, 1, out cur_path);
        return;
      }
      cur_path = null;
      return; 
    }
   
    /**
     * Vybere nasledujucu skladbu.
     *
     * Vybere nasledujucu skladbu ked je volana funkcia @see play_next().
     * existencia nasledujucej skladby musi byt vopred overena. 
     *
     * @param track iterator aktualnej skladby
     * @param selection iterator aktualne zvolenej skladby,
     *  nemusi byt prehravana ani rovnaka ako v track iteratore
     */
    private void select_next(ref Gtk.TreeIter track, ref Gtk.TreeIter slct)
    {
      this.model.iter_next(ref track);
      Gtk.TreeSelection selection;
      selection = this.get_selection();
      selection.unselect_all();
      selection.select_iter(track);
      slct = track;
    }
    
    /**
     * Prehrat predchadzajucu skladbu
     *
     * Prehrat predchadzajuca skladbu, ak existuje.
     * Existencia predchadzajucej skladby je overena vo funkcii. 
     * Ak existuje, tak je zvolena pomocou funkcie @see select_previous().
     *
     * @param iter_track iterator aktualne prehravanaej skladby
     * @param iter_selection iterator aktualne zvolenej skladby (nemusi byt prehravana)
     * @param streamer zvoleny prehravac
     */
    public void play_previous(ref Gtk.TreeIter iter_track,
    ref Gtk.TreeIter iter_selection, ref Streamer streamer)
    {
     // kontrola ci su v modeli nejake polozky
      if(is_empty())
      {
        return;
      }
      
      // kontrola ci su iteratory inicializovane
      if(!iter_initialized(iter_selection))
      {
        return;
      }
      
      if(!iter_initialized(iter_track))
      {
        return;
      }
      
      // kontrola existencie predchadzajucej polozky
      if(!has_previous(iter_selection))
      {
        return;
      }

      // ak streamer prehrava skladbu, tak sa musi zastavit, az potom sa presunie na dalsiu
      if(has_previous(iter_track))
      {
        if(streamer.is_playing())
        {
          streamer.set_state_stop();
        }
        select_previous(ref iter_track, ref iter_selection); //returns selection iterator
        play_current(ref iter_track, ref streamer);
      }  
      else
        return;
    }
    
    /**
     * Vybere predchadzajucu skladbu.
     *
     * Vybere predchadzajucu skladbu ked je volana funkcia @see play_previous(ref Gtk.TreeIter iter_track,
     *  ref Gtk.TreeIter iter_selection, ref Streamer streamer), 
     * existencia predchadzajucej skladby musi byt vopred overena. 
     *
     * @param track iterator aktualnej skladby
     * @param selection iterator aktualne zvolenej skladby, nemusi byt prehravana ani rovnaka ako v track iteratore
     */
    private void select_previous(ref Gtk.TreeIter track, ref Gtk.TreeIter slct)
    {
      this.model.iter_previous(ref track);
      Gtk.TreeSelection selection;
      selection = this.get_selection();
      selection.unselect_all();
      selection.select_iter(track); // new item in view is selected
      slct = track;
    }
    
    /**
     * Prideli kazdej skladbe ulozenej v modeli index poradia.
     */
    public void index_tracks()
    {
      if(this.size() == 0)
        return;
        
      Gtk.TreeIter iter;
      uint i = 1;
      
      if(!this.store.get_iter_first(out iter))
      {
        return;
      }
      else
      {
        this.store.set(iter, 3, (i.to_string() + "."));
        while(this.store.iter_next(ref iter))
        {
          i++;
          this.store.set(iter, 3, (i.to_string() + "."));
        }
        return;
      }
    }
    
    /**
     * Odstrani vybrane polozky.
     *
     * @param current_track aktualne prehravana skladba
     * @param streamer odkaz na prahravac
     */
    public void delete_items(ref Gtk.TreeIter current_track, ref Streamer streamer)
    {
      if(!can_delete)
      {
        return;
      }
    
      Gtk.TreeSelection selection = this.get_selection();
      int count = selection.count_selected_rows();

      if(count == 0)
      {
        return;
      }
      
      Gtk.TreeIter iter;
      Gtk.TreeModel model;
      GLib.List<Gtk.TreePath> paths = selection.get_selected_rows(out model);
      Gtk.ListStore store = (Gtk.ListStore) model;
      
      for(int i=0; i<count; i++)
      {
        Gtk.TreePath path = paths.nth_data(0);
        //print("%s\n", path.to_string());
        
        if(store.get_iter(out iter, path))
        {
          if(iter == current_track)
          {
            print("Currently played track found. Stopping player...\n");
            streamer.set_state_stop();
          }
          
          if(!store.remove(iter))
          {
            print("Trying to remove the last item\n");
          }
          paths.remove(path);
          print("Successfully removed\n");
          this.index_tracks();
          content_changed();
        }
        else
        {
          GLib.printerr("Error while trying to remove item(s) from tracklist\n");
          return;
        }
      }
    }
    
    public void set_dummy_model(bool m, ref Gtk.TreeIter current_track, ref Gtk.TreeIter current_selection)
    {
      if(m)
        this.set_model(dummy_store);
      else
        this.set_model(store);
      this.select_first_row(ref current_track, ref current_selection);
    }
    
  }
}
