using System;
using System.Collections.Generic;
using MqttService.Models;

namespace MqttService.Interfaces
{
    public interface IMicrocontrollerRepository : IDisposable
    {
        void Add(Microcontroller microcontroller);
        IEnumerable<Microcontroller> All();
        void Delete(Microcontroller microcontroller);
        void DeleteAll();
        bool Contains(string deviceId);
        int Count();
        void Update(string deviceId, bool powered);
        void Update(string deviceId, double temperature);
    }
}
