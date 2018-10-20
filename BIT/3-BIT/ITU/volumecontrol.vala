namespace StreamPlayer
{
  class VolumeControl: Gtk.ToolItem
  {
    private Gtk.Scale _volctrl;
    private Gtk.Table layout;
    private Gtk.Image icon;

    public VolumeControl()
    {
      icon = null;
      layout = new Gtk.Table(2, 1, true);
      try
      {
        Gdk.Pixbuf pb = new Gdk.Pixbuf.from_file_at_scale("icons/vol_large.png", 20, 20, true);
        icon = new Gtk.Image.from_pixbuf(pb);
      }
      catch
      {
        print("Error loading pixbuf\n");
      }

      
      _volctrl = new Gtk.Scale(Gtk.Orientation.HORIZONTAL, null);
      _volctrl.set_range(0.0, 100.0);
      _volctrl.width_request = 100;
      _volctrl.draw_value = false;
      _volctrl.set_sensitive(true);
      layout.attach(_volctrl, 0, 3, 0, 1, Gtk.AttachOptions.EXPAND, Gtk.AttachOptions.EXPAND, 0, 0);
      if(icon != null)
      {
        layout.attach(icon, 1, 2, 1, 2, Gtk.AttachOptions.SHRINK, Gtk.AttachOptions.SHRINK, 0, 0);
      }

      this.add(layout);
      this.can_focus = false;
      this.can_default = false;
    }

    public Gtk.Scale volume_control
    {
      get
      {
        return _volctrl;
      }
    }
  }
}
