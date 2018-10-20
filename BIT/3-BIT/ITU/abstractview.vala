namespace StreamPlayer
{ 
  abstract class AbstractView: Gtk.TreeView
  {
    protected Gtk.ListStore store;
    protected GLib.List<Gtk.TreePath> t_paths;
    
    protected bool deletable_items;
  
    public Gtk.ListStore list_store
    {
      get
      {
        return store;
      }
    }
    
    public GLib.List<Gtk.TreePath> paths
    {
      get
      {
        Gtk.TreeModel model;
        Gtk.TreeSelection selection = this.get_selection();
        t_paths = selection.get_selected_rows(out model);
        return t_paths;
      }
    }


    /** 
     * Ak existuje, tak je zvolena prva polozka modelu.
     * Vyznam ma len ak sa nacitava pri spustani aplikacie uz existujuci tracklist.
     *
     * @param track iterator aktualnej skladby
     * @param selection iterator aktualne zvolenej skladby, nemusi byt prehravana ani rovnaka ako v track iteratore
     *
     * @retruns true ak sa podarilo vybrat prvu polozku 
     */
    protected bool select_first_row(ref Gtk.TreeIter track, ref Gtk.TreeIter selection)
    {
      Gtk.TreeIter first;  
      if(this.model.get_iter_first(out first))
      {
        Gtk.TreeSelection slct;
        slct = this.get_selection();
        slct.select_iter(first);
        track = first;
        selection = first;
        return true;
      }
      return false;
    }
    
    /**
     * Zistit ci je v modeli nejaka polozka.
     *
     * @param void
     *
     * @returns true ak je model prazdny
     */
    public bool is_empty()
    {
      Gtk.TreeIter iter;
      if(!this.model.get_iter_first(out iter))
        return true;
      return false;
    }
    
    /**
     * Zistuje pocet poloziek v modeli.
     *
     * @param void
     *
     * @returns uint pocet poloziek v modeli
     */
    public uint size()
    {
      if(this.is_empty())
        return 0;
        
      Gtk.TreeIter iter;
      
      if(!this.model.get_iter_first(out iter))
      {
        return 0;
      }
      else
      {
        uint i = 1;
        while(this.model.iter_next(ref iter))
        {
          i++;
        }
        return i;
      }
    }
    
    /**
     * Zistuje ci pre vybranu polozku existuje nasledujuca polozka v modeli.
     *
     * @param current sucasne vybrana polozka
     *
     * @returns true ak existuje nasledujuca polozka
     */
    public bool has_next(Gtk.TreeIter current)
    {
      if(!this.model.iter_next(ref current))
        return false;
      else
        return true;
    }
    
    /**
     * Zistuje ci pre vybranu polozku existuje predchadzajuca polozka v modeli.
     *
     * @param current sucasne vybrana polozka
     *
     * @returns true ak existuje predchadzajuca polozka
     */
    public bool has_previous(Gtk.TreeIter current)
    {
      if(!this.model.iter_previous(ref current))
        return false;
      else
        return true;
    }
    
    /**
     * Zistuje ci vybrany iterator zodpoveda nejakej polozke modelu.
     *
     * @param iter kontrolovany iterator
     *
     * @returns true ak v modeli existuje zodpovedajuca polozka
     */
    public bool iter_initialized(Gtk.TreeIter iter)
    {
      Gtk.TreeIter i;
      
      if(!this.store.get_iter_first(out i))
      {
        return false;
      }
      else
      {
        if(iter == i)
        { 
          return true;
        }
        else
        {
          while(this.store.iter_next(ref i))
          {
            if(iter == i)
              return true;
          }
          return false;
        }
      }
    }
    
    public bool can_delete
    {
      get
      {
        return deletable_items;
      }
      set
      {
        deletable_items = value;
      }
    }
  }
}
