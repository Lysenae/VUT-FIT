namespace StreamPlayer
{
  namespace Utils
  {
    string shorten_string(string str, uint n_of_chars)
    {
      GLib.StringBuilder builder = new GLib.StringBuilder();
      for(int i=0; i<n_of_chars; i++)
      {
        builder.append_c(str[i]);
      }
      return builder.str + "...";
    }
    
    string calculate_duration_string(int64 ns)
    {
      int64 seconds;
      seconds = ns/1000000000;
      int64 sec_rem;
      sec_rem = ns % 1000000000;
      
      if(!(sec_rem == 0))
        seconds += 1;
        
      int64 minutes;
      
      if(seconds >= 60)
        minutes = seconds/60;
      else
        minutes = 0;
      
      string secs;
      int64 r_sec = seconds % 60;
      
      if(r_sec < 10)
        secs = "0" + r_sec.to_string();
      else
        secs = r_sec.to_string();
      
      return minutes.to_string() + ":" + secs;
    }
  }
}
