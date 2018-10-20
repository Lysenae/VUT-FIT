using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.ComponentModel;
using System.Diagnostics;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Models
{
    public class TrackList : INotifyPropertyChanged, IDisposable
    {
        public ObservableCollection<File> Tracks { get; private set; }
        public File CurrentTrack { get; set; }
        public bool HasCurrent { get { return CurrentTrack != null; } }
        public List<File> CurrentSelection { get; set; }
        public bool HasSelection { get { return CurrentSelection.Count > 0; } }
        public File QueuedTrack { get; set; }
        public bool HasQueued { get { return QueuedTrack != null; } }
        public int SelectedCount { get { return CurrentSelection.Count; } }

        public TrackList()
        {
            Tracks           = new ObservableCollection<File>();
            CurrentTrack     = null;
            CurrentSelection = new List<File>();
            QueuedTrack      = null;
        }

        public event PropertyChangedEventHandler PropertyChanged;
        private void NotifyPropertyChanged(string property)
        {
            PropertyChanged?.Invoke(this, new PropertyChangedEventArgs(property));
        }

        public void AddTrack(File track)
        {
            Debug.WriteLine("Adding track: {0}", track);
            var matches = Tracks.Where(x => String.Equals(x.FileName, track.FileName)).ToArray();
            if (matches.Length == 0)
            {
                Tracks.Add(track);
                NotifyPropertyChanged("Tracks");
            }
            else
                Debug.WriteLine("Skipping duplicate track {0}", track);
        }

        public void AddTrack(string filePath)
        {
            var f = new File(filePath);
            if (f.IsValid)
                AddTrack(f);
        }

        public void RemoveTrack(File track)
        {
            Tracks.Remove(track);
        }

        public void Select(File track)
        {
            CurrentSelection.Add(track);
        }

        public void Unselect(File track)
        {
            CurrentSelection.Remove(track);
        }

        public File FirstSelected()
        {
            if (CurrentSelection.Count <= 0)
                return null;
            else
                return CurrentSelection[0];
        }

        public SelectionData GetSelectionData()
        {
            return new SelectionData(CurrentSelection, new List<File>(Tracks));
        }

        public bool IsSelectedPlayed()
        {
            foreach (File f in CurrentSelection)
                if (f == CurrentTrack)
                    return true;
            return false;
        }

        public void RemoveSelected()
        {
            List<File> sel = new List<File>(CurrentSelection);
            foreach(File f in sel)
            {
                f.Close();
                Tracks.Remove(f);
            }
            CurrentSelection = new List<File>();
        }

        public void Dispose()
        {
            Debug.WriteLine("Closing files...");
            foreach(File f in Tracks)
            {
                f.Close();
            }
        }
    }
}
