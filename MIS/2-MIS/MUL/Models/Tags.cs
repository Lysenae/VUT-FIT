// Author: Daniel Klimaj (xklima22@stud.fit.vutbr.cz) 

using System;
using System.IO;
using System.Linq;
using System.Drawing;
using System.Drawing.Imaging;

namespace Models
{
    public class Tags
    {
        private TagLib.File _File { get; set; }

        public Tags(TagLib.File tagLibFile)
        {
            _File = tagLibFile;
        }

        public void Save()
        {
            _File.Save();
        }

        public void Close()
        {
            _File.Dispose();
        }

        public Image GetCoverImage()
        {
            try
            {
                if (_File.Tag.Pictures.Length > 0)
                {
                    var bin = (_File.Tag.Pictures[0].Data.Data);
                    return Image.FromStream(new MemoryStream(bin)).GetThumbnailImage(60, 60, null, IntPtr.Zero);
                }
                else
                {
                    return null;
                }
            }
            catch // Failed to parse image data
            {
                return null;
            }
        }

        public void UpdateCoverData(string imgPath)
        {
            _File.Tag.Pictures = new TagLib.IPicture[1] { new TagLib.Picture(imgPath) };
        }

        public void SaveCover()
        {
            var image = GetCoverImage();
            if (image != null)
            {
                string fname = Album.Length > 0 ? Album : "cover";
                image.Save(String.Format("{0}.png", fname), ImageFormat.Png);
            }
        }

        public string Album
        {
            get { return _File.Tag.Album; }
            set { _File.Tag.Album = value; }
        }

        public string AlbumArtists
        {
            get { return String.Join(";", _File.Tag.AlbumArtists); }
            set { _File.Tag.AlbumArtists = value.Split(';').Select(v => v.Trim()).ToArray(); }
        }

        #pragma warning disable 0618 // Ignore obsolete warning
        public string Artists
        {
            // get { return String.Join(";", _File.Tag.Artists); }
            // set { _File.Tag.Artists = value.Split(';').Select(v => v.Trim()).ToArray(); }
            get { return Performers; }
            set { Performers = value; }
        }
        #pragma warning restore 0618

        public string Composers
        {
            get { return String.Join(";", _File.Tag.Composers); }
            set { _File.Tag.Composers = value.Split(';').Select(v => v.Trim()).ToArray(); }
        }

        public string Conductor
        {
            get { return _File.Tag.Conductor; }
            set { _File.Tag.Conductor = value; }
        }

        public uint Year
        {
            get { return _File.Tag.Year; }
            set { _File.Tag.Year = value; }
        }

        public string Copyright
        {
            get { return _File.Tag.Copyright; }
            set { _File.Tag.Copyright = value; }
        }

        public uint Disc
        {
            get { return _File.Tag.Disc; }
            set { _File.Tag.Disc = value; }
        }

        public uint DiscCount
        {
            get { return _File.Tag.DiscCount; }
            set { _File.Tag.DiscCount = value; }
        }

        public string Genres
        {
            get { return String.Join(";", _File.Tag.Genres); }
            set { _File.Tag.Genres = value.Split(';').Select(v => v.Trim()).ToArray(); }
        }

        public string Grouping
        {
            get { return _File.Tag.Grouping; }
            set { _File.Tag.Grouping = value; }
        }

        public string Lyrics
        {
            get { return _File.Tag.Lyrics; }
            set { _File.Tag.Lyrics = value; }
        }

        public string Performers
        {
            get { return String.Join(";", _File.Tag.Performers); }
            set { _File.Tag.Performers = value.Split(';').Select(v => v.Trim()).ToArray(); }
        }

        public string Title
        {
            get { return _File.Tag.Title; }
            set { _File.Tag.Title = value; }
        }

        public uint Track
        {
            get { return _File.Tag.Track; }
            set { _File.Tag.Track = value; }
        }

        public uint TrackCount
        {
            get { return _File.Tag.TrackCount; }
            set { _File.Tag.TrackCount = value; }
        }
    }
}
