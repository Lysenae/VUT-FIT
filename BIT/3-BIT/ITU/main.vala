namespace StreamPlayer
{
  int main(string[] args)
  {
    Gtk.init(ref args);
    Gst.init(ref args);
    
    var window = new StreamPlayer();
    window.show_all();
    Gtk.main();
    

    return 0;
  }
}
