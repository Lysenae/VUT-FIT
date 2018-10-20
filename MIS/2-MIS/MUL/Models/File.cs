// Author: Daniel Klimaj (xklima22@stud.fit.vutbr.cz) 

using System;
using NAudio.Wave;
using System.Windows;
using System.Windows.Controls;
using System.Diagnostics;

namespace Models
{
    public class File : ListBoxItem
    {
        private Tags _tags;

        private DirectSoundOut _Player { get; set; }
        private AudioFileReader _Reader { get; set; }

        public string FileName { get; private set; }
        public bool IsValid { get; private set; }
        public bool IsPlaying { get; private set; }
        public bool IsPaused { get; private set; }
        public bool IsStopped { get; private set; }

        public File(string fileName)
        {
            try
            {
                _tags   = new Tags(TagLib.File.Create(fileName));
                IsValid = true;
            }
            catch // Unsupported file or something else
            {
                _tags   = null;
                IsValid = false;
            }
            FileName   = fileName;
            IsPlaying  = false;
            IsPaused   = false;
            IsStopped  = false;
            FontWeight = FontWeights.Normal;
            Content    = ToString();
        }

        public void UpdateTags()
        {
            Tags.Save();
        }

        public void Close()
        {
            Tags.Close();
        }

        public void Play()
        {
            if (IsValid)
            {
                if (IsPlaying && IsPaused)
                {
                    _Player.Play();
                    IsPaused  = false;
                    IsStopped = false;
                }
                else if(!IsPlaying)
                {
                    _Player = new DirectSoundOut();
                    _Reader = new AudioFileReader(FileName);
                    _Player.PlaybackStopped += _Player_PlaybackStopped;
                    _Player.Init(_Reader);
                    _Player.Play();
                    IsPlaying = true;
                    IsPaused  = false;
                    IsStopped = false;
                }
            }
        }

        private void _Player_PlaybackStopped(object sender, StoppedEventArgs e)
        {
            if (!IsStopped)
            {
                Debug.WriteLine("Finished");
                OnPlaybackFinished(EventArgs.Empty);
            }
            else
                Debug.WriteLine("User stopped");
        }

        public void Stop()
        {
            if(IsValid && IsPlaying)
            {
                _Player.Stop();
                _Reader.Dispose();
                _Player.Dispose();
                _Reader   = null;
                _Player   = null;
                IsPlaying = false;
                IsPaused  = false;
                IsStopped = true;
            }
        }

        public void Pause()
        {
            if(IsValid && IsPlaying && !IsPaused)
            {
                _Player.Pause();
                IsPaused = true;
            }
            else if(IsValid && IsPlaying && IsPaused)
            {
                _Player.Play();
                IsPaused = false;
            }
        }

        public void SetPosition(int timestamp)
        {
            if (IsValid && IsPlaying)
            {
                _Reader.CurrentTime = TimeSpan.FromSeconds(timestamp);
            }
        }

        public void SetVolume(float value)
        {
            if (IsValid && IsPlaying)
                _Reader.Volume = value >= 0.0f && value <= 1.0f ? value : 0.5f;
        }

        public Tags Tags
        {
            get { return _tags; }
        }

        public TimeSpan Position
        {
            get
            {
                if (_Reader != null)
                    return _Reader.CurrentTime;
                else
                    return new TimeSpan(0, 0, 0);
            }
        }

        public TimeSpan Duration
        {
            get
            {
                if (_Reader != null)
                    return _Reader.TotalTime;
                else
                {
                    var reader = new AudioFileReader(FileName);
                    return reader.TotalTime;
                }
            }
        }

        public override string ToString()
        {
            string name = "";
            if(Tags.Artists.Length > 0)
                name += Tags.Artists + " - ";
            if (Tags.Album != null && Tags.Album.Length > 0)
                name += Tags.Album + " - ";
            name += (" " + Tags.Track.ToString().PadLeft(2, '0') + " - ");
            name += Tags.Title != null && Tags.Title.Length > 0 ? Tags.Title : GetTitle();
            Debug.WriteLine(name);
            name += String.Format(" ({0}:{1})", Duration.Minutes.ToString().PadLeft(2, '0'), Duration.Seconds.ToString().PadLeft(2, '0'));
            return name;
        }

        public string GetTitle()
        {
            return Tags.Title != null && Tags.Title.Length > 0 ? Tags.Title : System.IO.Path.GetFileName(FileName);
        }

        protected virtual void OnPlaybackFinished(EventArgs e)
        {
            PlaybackFinished?.Invoke(this, e);
        }

        public event EventHandler PlaybackFinished;
    }
}
