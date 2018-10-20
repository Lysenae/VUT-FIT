namespace StreamPlayer
{
  class Streamer: GLib.Object
  {
    /* private members */
    private dynamic Gst.Element player;
    private Gst.Bus bus;
    
    /* private fields */
    private bool can_stop;
    private TagStore tagstore;
    
    /* signals */
    public signal void end_of_stream();
    public signal void playback_error(string err_msg);
    public signal void player_stopped();
    public signal void track_info_obtained(TagStore tags);
    public signal void player_playing();
    public signal void query_position_changed(int64 position);
    public signal void track_duration_obtained(int64 duration);
  
    public Streamer()
    {
      can_stop = false;
      player = Gst.ElementFactory.make("playbin2", "player");
      bus = player.get_bus();
      bus.add_watch(bus_callback);
    }
    
    private bool bus_callback(Gst.Bus bus, Gst.Message msg)
    {
      if(msg.type == Gst.MessageType.ERROR)
      {
        GLib.Error err;
        string debug;
        msg.parse_error(out err, out debug);
        playback_error(err.message);
      }
      if(msg.type == Gst.MessageType.EOS)
      {
        end_of_stream();
      }
      else
      {
      }
      return true;
    }
  
    public void play(string? stream)
    {
      if(stream == null)
      {
        return;
      }
      else
      {
        //player.set_state(Gst.State.NULL);
        string complete_uri = "file://" + stream;
        TagReader.get_tags(complete_uri, out tagstore);
        track_info_obtained(tagstore);

        string dur;
        int64 idur;
        print("\nCurrently playing: ");
        get_track_length(complete_uri, out dur, out idur);
        track_duration_obtained(idur);
       
        player.uri = complete_uri;
        can_stop = true;
        player_playing();
        player.set_state(Gst.State.PLAYING);
        get_current_position();
      }
    }
    
    private string get_duration(out int64 n_dur)
    {
        Gst.Format format = Gst.Format.TIME;
        int64 duration;
        if(!player.query_duration(ref format, out duration))
          print("Unable to play the song. Check if you have codecs installed.\n");
        print("%s\n", Utils.calculate_duration_string(duration));
        n_dur = duration;
        player.set_state(Gst.State.NULL);
        return Utils.calculate_duration_string(duration);
    }
    
    public void get_track_length(string uri, out string s_duration, out int64 n_duration)
    {
      player.set_state(Gst.State.NULL);
      print("%s\n", uri);
      player.uri = uri;
      player.set_state(Gst.State.PAUSED);
      GLib.Timer timer = new GLib.Timer();
      timer.start();
      for(double d=0.0; d<= 0.07;)
        d = timer.elapsed();
      timer.stop();
      s_duration = get_duration(out n_duration);
    }
    
    public void get_current_position()
    {
      GLib.Timeout.add(200, get_position);
    }
    
    private bool get_position()
    {
      if(!(is_playing()))
        return false;
      int64 pos;
      Gst.Format format = Gst.Format.TIME;
      if((player.query_position(ref format, out pos)))
        query_position_changed(pos);
      else
        print("chyba\n");
      return true;
    }
    
    public void set_position(int64 pos)
    {
      if(!player.seek_simple(Gst.Format.TIME, Gst.SeekFlags.FLUSH, pos))
        print("Fail\n");
    }
    
    public void stop()
    {
      if(!can_stop)
        return;
      player.set_state(Gst.State.NULL);
      can_stop = false;
      player_stopped();
    }
    
    public void pause()
    {
      player.set_state(Gst.State.PAUSED);
      get_current_position();
    }
    
    public bool is_playing()
    {
      Gst.State state;
      Gst.State pending;
      player.get_state(out state, out pending, Gst.CLOCK_TIME_NONE);
      if(state == Gst.State.PLAYING)
        return true;
      return false;
    }
    
    public bool is_paused()
    {
      Gst.State state;
      Gst.State pending;
      player.get_state(out state, out pending, Gst.CLOCK_TIME_NONE);
      if(state == Gst.State.PAUSED)
        return true;
      return false;
    }
    
    public void set_state_playing()
    {
      player.set_state(Gst.State.PLAYING);
      get_current_position();
    }
    
    public void set_state_stop()
    {
      player.set_state(Gst.State.NULL);
      player_stopped();
    }

/*
    public Gst.State get_current_state()
    {
      Gst.State state;
      Gst.State pending;
      player.get_state(out state, out pending, Gst.CLOCK_TIME_NONE);
      return state;
    }
*/
  }
}
