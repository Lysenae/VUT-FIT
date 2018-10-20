using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Drawing;
using System.Linq;
using System.IO;
using System.Threading.Tasks;
using System.Text.RegularExpressions;

namespace Models
{
    public class SelectionData
    {
        public bool SameAlbum;
        public bool SameArtists;
        public bool SameAlbumArtists;
        public bool SameGenres;
        public bool SameComposers;
        public bool SameConductor;
        public bool SameCopyright;
        public bool SameGrouping;

        public HashSet<string> AllTitles { get; private set; }
        public HashSet<string> AllAlbums { get; private set; }
        public HashSet<string> AllArtists { get; private set; }
        public HashSet<string> AllAlbumArtists { get; private set; }
        public HashSet<string> AllGenres { get; private set; }
        public HashSet<string> AllComposers { get; private set; }
        public HashSet<string> AllConductors { get; private set; }
        public HashSet<string> AllGroupings { get; private set; }
        public HashSet<string> AllCopyrights { get; private set; }

        public string Title { get; private set; }
        public string Album { get; private set; }
        public string Artists { get; private set; }
        public string AlbumArtists { get; private set; }
        public string Genres { get; private set; }
        public string Composers { get; private set; }
        public string Conductor { get; private set; }
        public string Grouping { get; private set; }
        public string Copyright { get; private set; }
        public uint Year { get; private set; }
        public uint Disc { get; private set; }
        public uint Track { get; private set; }
        public uint TrackCount { get; private set; }
        public uint DiscCount { get; private set; }
        public string Lyrics { get; private set; }
        public Image Image { get; private set; }

        public SelectionData(List<File> selection, List<File> allFiles)
        {
            ResetFlags();
            ResetLists();
            if(selection.Count > 0)
                Process(selection, allFiles);
        }

        private void ResetFlags()
        {
            SameAlbum = true;
            SameArtists = true;
            SameAlbumArtists = true;
            SameGenres = true;
            SameComposers = true;
            SameConductor = true;
            SameCopyright = true;
            SameGrouping = true;
        }

        private void ResetLists()
        {
            AllTitles = new HashSet<string>();
            AllAlbums = new HashSet<string>();
            AllArtists = new HashSet<string>();
            AllAlbumArtists = new HashSet<string>();
            AllGenres = new HashSet<string>();
            AllComposers = new HashSet<string>();
            AllConductors = new HashSet<string>();
            AllGroupings = new HashSet<string>();
            AllCopyrights = new HashSet<string>();
        }

        private void ResetValues()
        {
            Title = "";
            Album = "";
            Artists = "";
            AlbumArtists = "";
            Genres = "";
            Composers = "";
            Conductor = "";
            Grouping = "";
            Copyright = "";
            Year = 0;
            Disc = 0;
            DiscCount = 0;
            Track = 0;
            TrackCount = 0;
            Lyrics = "";
            Image = null;
        }

        private void SetValues(List<File> selection)
        {
            if (selection.Count == 1)
            {
                Title = selection[0].GetTitle();
                Album = selection[0].Tags.Album;
                Artists = selection[0].Tags.Artists;
                AlbumArtists = selection[0].Tags.AlbumArtists;
                Genres = selection[0].Tags.Genres;
                Composers = selection[0].Tags.Composers;
                Conductor = selection[0].Tags.Conductor;
                Grouping = selection[0].Tags.Grouping;
                Copyright = selection[0].Tags.Copyright;
                Image = selection[0].Tags.GetCoverImage();
            }
            else
            {
                Album = SameAlbum ? selection[0].Tags.Album : "";
                Artists = SameArtists ? selection[0].Tags.Artists : "";
                AlbumArtists = SameAlbumArtists ? selection[0].Tags.AlbumArtists : "";
                Genres = SameGenres ? selection[0].Tags.Genres : "";
                Composers = SameComposers ? selection[0].Tags.Composers : "";
                Conductor = SameConductor ? selection[0].Tags.Conductor: "";
                Grouping = SameGrouping ? selection[0].Tags.Grouping : "";
                Copyright = SameCopyright ? selection[0].Tags.Copyright : "";
            }
            Year = selection[0].Tags.Year;
            Disc = selection[0].Tags.Disc;
            Track = selection[0].Tags.Track;
            DiscCount = selection[0].Tags.DiscCount;
            TrackCount = selection[0].Tags.TrackCount;
        }

        private void Process(List<File> selection, List<File> allFiles)
        {
            string currAlbum = null;
            string currArtist = null;
            string currAA = null;
            string currGenre = null;
            string currComposer = null;
            string currConductor = null;
            string currGrouping = null;
            string currCopyright = null;

            bool set = false;

            foreach(File f in allFiles)
            {
                if (f.Tags.Title != null && f.Tags.Title.Length > 0)
                    AllTitles.Add(f.Tags.Title);
                string fname = Path.GetFileName(f.FileName);
                Match match = Regex.Match(fname, @"(?:\d+\.?)?(?:[\s_]?-[\s_]?)?(.*?)\.[^\.]+$");
                if (match.Success)
                {
                    string val = match.Groups[1].Value;
                    val = val.Replace('_', ' ');
                    val = val.Replace('.', ' ');
                    val = Regex.Replace(val, @"\s+", " ");
                    val = val.Trim();
                    if (val.Length > 0)
                        AllTitles.Add(val);

                    if (f.Tags.Album != null && f.Tags.Album.Length > 0)
                        AllAlbums.Add(f.Tags.Album);
                    if (f.Tags.Artists != null && f.Tags.Artists.Length > 0)
                        AllArtists.Add(f.Tags.Artists);
                    if (f.Tags.AlbumArtists != null && f.Tags.AlbumArtists.Length > 0)
                        AllAlbumArtists.Add(f.Tags.AlbumArtists);
                    if (f.Tags.Genres != null && f.Tags.Genres.Length > 0)
                        AllGenres.Add(f.Tags.Genres);
                    if (f.Tags.Composers != null && f.Tags.Composers.Length > 0)
                        AllComposers.Add(f.Tags.Composers);
                    if (f.Tags.Conductor != null && f.Tags.Conductor.Length > 0)
                        AllConductors.Add(f.Tags.Conductor);
                    if (f.Tags.Grouping != null && f.Tags.Grouping.Length > 0)
                        AllGroupings.Add(f.Tags.Grouping);
                    if (f.Tags.Copyright != null && f.Tags.Copyright.Length > 0)
                        AllCopyrights.Add(f.Tags.Copyright);
                }
            }

            foreach (File f in selection)
            {
                if (!set)
                {
                    currAlbum = f.Tags.Album;
                    currArtist = f.Tags.Artists;
                    currAA = f.Tags.AlbumArtists;
                    currGenre = f.Tags.Genres;
                    currComposer = f.Tags.Composers;
                    currConductor = f.Tags.Conductor;
                    currGrouping = f.Tags.Grouping;
                    currCopyright = f.Tags.Copyright;
                }
                else
                {
                    if (SameAlbum && currAlbum != f.Tags.Album)
                        SameAlbum = false;
                    else
                        currAlbum = f.Tags.Album;

                    if (SameArtists && currArtist != f.Tags.Artists)
                        SameArtists = false;
                    else
                        currArtist = f.Tags.Artists;

                    if (SameAlbumArtists && currAA != f.Tags.AlbumArtists)
                        SameAlbumArtists = false;
                    else
                        currAA = f.Tags.AlbumArtists;

                    if (SameGenres && currGenre != f.Tags.Genres)
                        SameGenres = false;
                    else
                        currGenre = f.Tags.Genres;

                    if (SameComposers && currComposer != f.Tags.Composers)
                        SameComposers = false;
                    else
                        currComposer = f.Tags.Composers;

                    if (SameConductor && currConductor != f.Tags.Conductor)
                        SameConductor = false;
                    else
                        currConductor = f.Tags.Conductor;

                    if (SameGrouping && currGrouping != f.Tags.Grouping)
                        SameGrouping = false;
                    else
                        currGrouping = f.Tags.Grouping;

                    if (SameCopyright && currCopyright != f.Tags.Copyright)
                        SameCopyright = false;
                    else
                        currCopyright = f.Tags.Copyright;
                }
                set = true;
            }
            SetValues(selection);
        }
    }
}
