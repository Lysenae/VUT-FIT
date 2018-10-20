namespace StreamPlayer
{
  class TagReader: GLib.Object
  {
    private static Gst.Pipeline pipe;
    private static Gst.Element dec;
    private static Gst.Element sink;
    private static Gst.Message msg;
    
    private static TagStore m_tag_store;
    
    private static void print_one_tag(Gst.TagList list, string tag)
    {
      uint i;
      uint num;
      
      num = list.get_tag_size(tag);
      for(i=0; i<num; ++i)
      {
        Gst.Value val;
        val = list.get_value_index(tag, i);
        
        if(val.holds(typeof(string)))
        {
          //GLib.print("\t%20s : %s\n", tag, val.get_string());
          if(tag == "title")
          {
            m_tag_store.title = val.get_string();
          }
          if(tag == "genre")
          {
            m_tag_store.genre = val.get_string();
          }
          if(tag == "artist")
          {
            m_tag_store.artist = val.get_string();
          }
          if(tag == "album-artist")
          {
            m_tag_store.album_artist = val.get_string();
          }
          if(tag == "album")
          {
            m_tag_store.album = val.get_string();
          }
        }
        else if(val.holds(typeof(uint)))
        {
          //GLib.print("\t%20s : %u\n", tag, val.get_uint());
          if(tag == "track-number")
          {
            m_tag_store.track_number = val.get_uint();
          }
        }
        /*else if(val.holds(typeof(double)))
        {
          GLib.print("\t%20s : %g\n", tag, val.get_double());
        }*/
        /*else if(val.holds(typeof(bool)))
        {
          GLib.print("\t%20s : %s\n", tag, (val.get_boolean() ? "true" : "false"));
        }*/
        /*else if(val.holds(typeof(Gst.Buffer)))
        {
          GLib.print("\t%20s : buffer size %u\n", tag, val.get_buffer().size);
        }*/
        else if(val.holds(GLib.Type.from_name("GstDate")))
        {
          GLib.Date date;
          GLib.warn_if_fail(list.get_date(tag, out date));
          //GLib.print("\t%20s : %u\n", tag, date.get_year());
          if(tag == "date")
          {
            m_tag_store.year = date.get_year();
          }
        }
        /*else
        {
          GLib.print("\t%20s : tag of type '%s'\n", tag, val.type_name());
        }*/
      }
    }
    
    static void on_new_pad(Gst.Pad pad)
    {
      Gst.Pad sinkpad;
      
      sinkpad = sink.get_static_pad("sink");
      
      if(!sinkpad.is_linked())
      {
        if(pad.link(sinkpad) != Gst.PadLinkReturn.OK)
          GLib.printerr("Failed to link pads!\n");
      }
    }
    
    public static void get_tags(string uri, out TagStore tag_store)
    {
      tag_store = new TagStore();
      if(uri == null)
        return;

      m_tag_store = new TagStore();
      pipe = new Gst.Pipeline("pipeline");
      
      dec = Gst.ElementFactory.make("uridecodebin", null);
      dec.set("uri", uri, null);
      pipe.add(dec);
      
      sink = Gst.ElementFactory.make("fakesink", null);
      pipe.add(sink);
      
      dec.pad_added.connect(on_new_pad);
      
      pipe.set_state(Gst.State.PAUSED);
      
      while(true)
      {
        Gst.TagList tags = null;
        
        msg = pipe.bus.timed_pop_filtered(Gst.CLOCK_TIME_NONE, Gst.MessageType.ASYNC_DONE | Gst.MessageType.TAG | Gst.MessageType.ERROR);

        if(msg.type != Gst.MessageType.TAG)
          break;
        
        msg.parse_tag(out tags);
        
        //GLib.print("Got tags from element %s:\n", msg.src.name);
        tags.foreach((Gst.TagForeachFunc)print_one_tag);
        //GLib.print("\n");
        
        tag_store.copy_content(m_tag_store);
      }
      
      if(msg.type == Gst.MessageType.ERROR)
        GLib.printerr("Got error\n");
        
      pipe.set_state(Gst.State.NULL);
      
      return;
    }
  }
  
  class TagStore: GLib.Object
  {
    private string _artist;
    private string _title;
    private string _album;
    private string _album_artist;
    private string _genre;
    private uint _year;
    private uint _track_number;
    
    public TagStore()
    {
      _artist = "";
      _title = "";
      _album = "";
      _album_artist = "";
      _genre = "";
      _year = 0;
      _track_number = 0;
    }
    
    public TagStore.from_tag_store(TagStore store)
    {
      this._artist = store.artist;
      this._title = store.title;
      this._album = store.album;
      this._album_artist = store.album_artist;
      this._genre = store.genre;
      this._year = store.year;
      this._track_number = store.track_number;
    }
    
    public void copy_content(TagStore store)
    {
      this.artist = store.artist;
      this.title = store.title;
      this.album = store.album;
      this.album_artist = store.album_artist;
      this.genre = store.genre;
      this.year = store.year;
      this.track_number = store.track_number;
    }

    public string artist
    {
      get
      {
        return _artist;
      }
      set
      {
        _artist = value;
      }
    }
    
    public string title
    {
      get
      {
        return _title;
      }
      set
      {
        _title = value;
      }
    }
    
    public string album
    {
      get
      {
        return _album;
      }
      set
      {
        _album = value;
      }
    }
    
    public string album_artist
    {
      get
      {
        return _album_artist;
      }
      set
      {
        _album_artist = value;
      }
    }
    
    public string genre
    {
      get
      {
        return _genre;
      }
      set
      {
        _genre = value;
      }
    }
    
    public uint year
    {
      get
      {
        return _year;
      }
      set
      {
        _year = value;
      }
    }
    
    public uint track_number
    {
      get
      {
        return _track_number;
      }
      set
      {
        _track_number = value;
      }
    }
  }
}
