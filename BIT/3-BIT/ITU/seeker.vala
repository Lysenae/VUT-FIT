namespace StreamPlayer
{
  class Seeker: Gtk.ToolItem
  {
    private Gtk.Scale _seeker;
    private Gtk.Label _label_position;
    private Gtk.Label _label_duration;
    private Gtk.Label _label_slash;
    private Gtk.Table layout;
    private Gtk.Table sublayout;
    
    public Seeker()
    {
      layout = new Gtk.Table(2, 1, true);
      
      _seeker = new Gtk.Scale(Gtk.Orientation.HORIZONTAL, null);
      _seeker.set_range(0.0, 0.0);
      _seeker.width_request = 250;
      _seeker.draw_value = false;
      _seeker.set_sensitive(false);
      layout.attach(_seeker, 0, 1, 0, 1, Gtk.AttachOptions.EXPAND, Gtk.AttachOptions.EXPAND, 3, 3);
      
      sublayout = new Gtk.Table(1, 3, false);
      
      _label_position = new Gtk.Label("00:00");
      _label_duration = new Gtk.Label("00:00");
      _label_slash = new Gtk.Label("/");
      sublayout.attach(_label_position, 0, 1, 0, 1, Gtk.AttachOptions.EXPAND, Gtk.AttachOptions.EXPAND, 2, 3);
      sublayout.attach(_label_slash, 1, 2, 0, 1, Gtk.AttachOptions.EXPAND, Gtk.AttachOptions.EXPAND, 2, 3);
      sublayout.attach(_label_duration, 2, 3, 0, 1, Gtk.AttachOptions.EXPAND, Gtk.AttachOptions.EXPAND, 2, 3);
      
      layout.attach(sublayout, 0, 1, 1, 2, Gtk.AttachOptions.EXPAND, Gtk.AttachOptions.EXPAND, 3, 3);
      
      this.add(layout);
      this.can_focus = false;
      this.can_default = false;
    }
    
    public Gtk.Scale seeker
    {
      get
      {
        return _seeker;
      }
    }
    
    public string position
    {
      get
      {
        return _label_position.get_text();
      }
      set
      {
        _label_position.set_text(value);
      }
    }

    public string duration
    {
      get
      {
        return _label_duration.get_text();
      }
      set
      {
        _label_duration.set_text(value);
      }
    }
  }
}
